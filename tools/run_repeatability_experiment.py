#!/usr/bin/env python3
"""Measure dispersion and repeatability of reported mean cycle counts.

Two tiers, because the manuscript's numbers are produced by a pipeline with two
potentially variable stages:

  Tier A (reset):    one clean build, one flash, then N reset -> boot -> measure
                     cycles. Isolates run-to-run variability of the reported
                     mean with the binary held bit-identical.
  Tier B (pipeline): R independent clean-build -> flash -> measure cycles.
                     Checks the complete measurement pipeline.
  Tier C (stddev):   builds configured with -DMLKEM_STATS=stddev, in which the
                     firmware computes the population standard deviation of the
                     100 per-iteration cycle counts on-device and prints it.
                     MLKEM_STATS defaults to off, and a
                     flag-off build is bit-identical to one from before the
                     option existed, so other benchmark scripts are unaffected.

The firmware runs its 100-iteration benchmark once at boot and then halts, so a
reset re-runs the whole measurement. The benchmark RNG in the baseline is a
fixed-seed PRNG whose state is reloaded from Flash at boot, so every boot
replays identical inputs; any observed spread is therefore platform behaviour,
not input variation.

Outputs one row per repeat to results.csv, written incrementally.
Run from the repository root with:
  python3 tools/run_repeatability_experiment.py
"""

import argparse
import csv
import hashlib
import importlib.util
import os
import subprocess
import sys
import re
import termios
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

_spec = importlib.util.spec_from_file_location("ib", ROOT / "interactive_benchmark.py")
ib = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(ib)

ELF = ROOT / "build" / "Release" / "STM_ML-KEM_V3.elf"
BOOT_BANNER = "ML-KEM benchmark start"
DONE = "BENCHMARK_DONE"

ITCM_FIELDS = ib.ITCM_FIELDS

# Emitted only by builds configured with -DMLKEM_STATS=stddev. The firmware
# computes the population standard deviation of the 100 per-iteration cycle
# counts on-device and prints it, measured directly rather than inferred from
# order statistics.
STDDEV_RE = re.compile(
    r"^STDDEV (?P<name>Keygen|Encaps|Decaps): sd=(?P<sd>\d+) cycles "
    r"rel=(?P<rel>[0-9.]+) % n=(?P<n>\d+)", re.MULTILINE)
STATS = ("min", "avg", "med", "max")
OPS = ("keygen", "encaps", "decaps")

# ---------------------------------------------------------------- cell set --
# Chosen to cover the small memory-system effects in the primary evaluation:
#   * baseline, at both clock endpoints and all three parameter sets
#   * two near-neutral knobs: Flash ART and DTCM constants
#   * the most consistent non-caching gain: endpoint-selected ITCM placement
#     the endpoint-selected ITCM placement
# Paired against their own baseline at the same clock, so an effect size can be
# compared directly against the measured repeatability spread.
def cell(name, key_size, clock, itcm="off", dtcm="off", flash="off", hw_rng="OFF"):
    flags = ib.itcm_flags_from_profile(itcm)
    return {
        "cell": name,
        "key_size": key_size,
        "clock": clock,
        "itcm": itcm,
        "dtcm_const": dtcm,
        "flash_accel": flash,
        "hw_rng": hw_rng,
        "flags": flags,
    }

ITCM_768 = "keccak+matacc_cache+matacc_opt+frombytes+basemul"
ITCM_512 = "keccak+ntt"
ITCM_1024 = "keccak+matacc_cache+matacc_opt"

CELLS = [
    cell("base_768_24",   "768",  "24mhz"),
    cell("base_768_216",  "768",  "216mhz"),
    cell("art_768_24",    "768",  "24mhz",  flash="art_prefetch"),
    cell("art_768_216",   "768",  "216mhz", flash="art_prefetch"),
    cell("dtcm_768_24",   "768",  "24mhz",  dtcm="ntt"),
    cell("dtcm_768_216",  "768",  "216mhz", dtcm="ntt"),
    cell("itcm_768_24",   "768",  "24mhz",  itcm=ITCM_768),
    cell("itcm_768_216",  "768",  "216mhz", itcm=ITCM_768),
    cell("base_512_216",  "512",  "216mhz"),
    cell("base_1024_216", "1024", "216mhz"),
    # Hardware TRNG: randomness comes from the RNG peripheral, so keypairs
    # differ between iterations AND between boots. Input variation therefore
    # no longer cancels across repeats, and the repeatability of the reported
    # mean must be measured separately rather than inferred from the
    # fixed-seed cells above.
    cell("trng_768_216", "768", "216mhz", hw_rng="ON"),
]

# Tier B runs the full build+flash pipeline; kept small because Tier A already
# characterises the measurement and the build is bit-reproducible.
TIER_B_CELLS = ["base_768_216", "itcm_768_216"]


def build(c, stats="off"):
    cmd = [
        "cmake", "--fresh", "--preset", "Release",
        f"-DUSE_HW_RNG={c.get('hw_rng', 'OFF')}",
        f"-DMLKEM_K={c['key_size']}",
        "-DMLKEM_BACKEND=pqm7", "-DMLKEM_ARCH=m7", "-DMLKEM_OPT=speed",
        "-DMLKEM_KECCAK=m4_opt_m7", "-DMLKEM_VARIANT=m7fspeed",
        f"-DMLKEM_CLOCK={c['clock']}",
        f"-DMLKEM_FLASH_ACCEL={c['flash_accel']}",
        f"-DMLKEM_DTCM_CONST={c['dtcm_const']}",
        "-DMLKEM_PROFILE=off", "-DMLKEM_PUBLIC_HPK=OFF", "-DMLKEM_PUBLIC_PK=OFF",
        "-DMLKEM_BENCH_MODE=sequence", "-DMLKEM_USE_CACHED_A=OFF",
        f"-DMLKEM_STATS={stats}",
    ]
    for field in ITCM_FIELDS:
        cmd.append(f"-DMLKEM_{field.upper()}={c['flags'][field]}")

    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0:
        return None, r.stdout + r.stderr
    r2 = subprocess.run(
        ["cmake", "--build", "--preset", "Release", f"-j{os.cpu_count() or 1}"],
        cwd=ROOT, capture_output=True, text=True)
    log = r.stdout + r.stderr + r2.stdout + r2.stderr
    if r2.returncode != 0:
        return None, log
    return log, log


def flash():
    r = subprocess.run(
        ["STM32_Programmer_CLI", "-c", "port=SWD", "-w", str(ELF), "-v", "-rst"],
        capture_output=True, text=True)
    return r.returncode == 0


def reset_board():
    r = subprocess.run(["STM32_Programmer_CLI", "-c", "port=SWD", "-rst"],
                       capture_output=True, text=True)
    return r.returncode == 0


def flush_serial(dev):
    """Discard any buffered input so a read cannot return the previous boot."""
    try:
        fd = os.open(dev, os.O_RDONLY | os.O_NOCTTY | os.O_NONBLOCK)
        try:
            ib.configure_serial(fd, 115200)
            termios.tcflush(fd, termios.TCIFLUSH)
        finally:
            os.close(fd)
    except OSError:
        pass


def measure(dev, timeout):
    """One reset -> boot -> measure. Returns (parsed, log, status)."""
    flush_serial(dev)
    if not reset_board():
        return None, "", "reset_failed"
    log = ib.read_uart(dev, 115200, timeout)
    # Freshness and completeness checks: exactly one boot and one result block.
    if log.count(BOOT_BANNER) != 1:
        return None, log, f"boot_banner_count_{log.count(BOOT_BANNER)}"
    if log.count(DONE) != 1:
        return None, log, f"done_count_{log.count(DONE)}"
    p = ib.parse_uart(log)
    for m in STDDEV_RE.finditer(log):
        op = m.group("name").lower()
        p[f"{op}_sd"] = m.group("sd")
        p[f"{op}_sd_rel"] = m.group("rel")
    for op in OPS:
        for st in STATS:
            if not p.get(f"{op}_{st}"):
                return None, log, "incomplete_stats"
    if p.get("shared_secret_match") != "1":
        return None, log, "secret_mismatch"
    return p, log, "ok"


FIELDS = (["tier", "cell", "repeat", "status", "key_size", "clock", "itcm",
           "dtcm_const", "flash_accel", "hw_rng", "elf_md5", "ram_used_bytes",
           "flash_used_bytes", "hclk_hz", "iterations"]
          + [f"{op}_{st}" for op in OPS for st in STATS]
          + [f"{op}_sd" for op in OPS] + [f"{op}_sd_rel" for op in OPS]
          + ["shared_secret_match", "wall_seconds"])


def row_for(tier, c, i, status, parsed, md5, ram, fl, secs):
    row = {k: "" for k in FIELDS}
    row.update(tier=tier, cell=c["cell"], repeat=i, status=status,
               key_size=c["key_size"], clock=c["clock"], itcm=c["itcm"],
               dtcm_const=c["dtcm_const"], flash_accel=c["flash_accel"],
               hw_rng=c.get("hw_rng", "OFF"),
               elf_md5=md5, ram_used_bytes=ram, flash_used_bytes=fl,
               wall_seconds=f"{secs:.2f}")
    if parsed:
        row["hclk_hz"] = parsed.get("hclk_hz", "")
        row["iterations"] = parsed.get("iterations", "")
        row["shared_secret_match"] = parsed.get("shared_secret_match", "")
        for op in OPS:
            for st in STATS:
                row[f"{op}_{st}"] = parsed.get(f"{op}_{st}", "")
            row[f"{op}_sd"] = parsed.get(f"{op}_sd", "")
            row[f"{op}_sd_rel"] = parsed.get(f"{op}_sd_rel", "")
    return row


def main():
    ap = argparse.ArgumentParser()
    default_out = ROOT / "benchmark_results" / (
        "repeatability_" + time.strftime("%Y-%m-%d_%H-%M-%S"))
    ap.add_argument("--out", default=str(default_out))
    ap.add_argument("--uart", default="/dev/ttyACM0")
    ap.add_argument("--n-reset", type=int, default=100)
    ap.add_argument("--n-pipeline", type=int, default=20)
    ap.add_argument("--timeout", type=int, default=60)
    ap.add_argument("--only", default="", help="comma-separated cell names")
    ap.add_argument("--tier-c", action="store_true",
                    help="run only Tier C: instrumented builds reporting the "
                         "on-device per-iteration standard deviation")
    ap.add_argument("--n-stats", type=int, default=20)
    args = ap.parse_args()

    out = Path(args.out)
    (out / "raw").mkdir(parents=True, exist_ok=True)
    csv_path = out / "results.csv"
    new = not csv_path.exists()
    fh = open(csv_path, "a", newline="")
    w = csv.DictWriter(fh, fieldnames=FIELDS)
    if new:
        w.writeheader()
        fh.flush()

    cells = CELLS
    if args.only:
        want = {s.strip() for s in args.only.split(",")}
        cells = [c for c in CELLS if c["cell"] in want]

    t_start = time.monotonic()

    # ---- Tier C: instrumented builds reporting on-device per-iteration SD ----
    # Dispersion across the 100 timed iterations, computed on the target from
    # the samples themselves.
    if args.tier_c:
        for c in cells:
            print(f"\n=== Tier C {c['cell']} : stddev build, {args.n_stats} repeats ===",
                  flush=True)
            log, _ = build(c, stats="stddev")
            if log is None:
                w.writerow(row_for("C", c, 0, "build_failed", None, "", "", "", 0))
                fh.flush()
                continue
            md5 = hashlib.md5(ELF.read_bytes()).hexdigest()
            ram, fl = ib.parse_build_memory(log)
            if not flash():
                w.writerow(row_for("C", c, 0, "flash_failed", None, md5, ram, fl, 0))
                fh.flush()
                continue
            for i in range(1, args.n_stats + 1):
                t0 = time.monotonic()
                p, raw, status = measure(args.uart, args.timeout)
                secs = time.monotonic() - t0
                if status == "ok" and not p.get("keygen_sd"):
                    status = "missing_stddev"
                w.writerow(row_for("C", c, i, status, p, md5, ram, fl, secs))
                fh.flush()
                if status != "ok":
                    (out / "raw" / f"C_{c['cell']}_{i:03d}_{status}.log").write_text(raw)
                elif i == 1:
                    print(f"  repeat 1 sd: keygen={p['keygen_sd']} ({p['keygen_sd_rel']}%) "
                          f"encaps={p['encaps_sd']} ({p['encaps_sd_rel']}%) "
                          f"decaps={p['decaps_sd']} ({p['decaps_sd_rel']}%)", flush=True)
            p, raw, status = measure(args.uart, args.timeout)
            (out / "raw" / f"C_{c['cell']}_representative.log").write_text(raw)
        fh.close()
        print(f"\nTotal wall time: {(time.monotonic()-t_start)/60:.1f} min")
        print(f"Results: {csv_path}")
        return

    # ---- Tier A: reset repeats, one build and one flash per cell ----
    for c in cells:
        print(f"\n=== Tier A {c['cell']} : build+flash, then {args.n_reset} resets ===",
              flush=True)
        log, _ = build(c)
        if log is None:
            print(f"  BUILD FAILED {c['cell']}", flush=True)
            w.writerow(row_for("A", c, 0, "build_failed", None, "", "", "", 0)); fh.flush()
            continue
        md5 = hashlib.md5(ELF.read_bytes()).hexdigest()
        ram, fl = ib.parse_build_memory(log)
        if not flash():
            print(f"  FLASH FAILED {c['cell']}", flush=True)
            w.writerow(row_for("A", c, 0, "flash_failed", None, md5, ram, fl, 0)); fh.flush()
            continue
        for i in range(1, args.n_reset + 1):
            t0 = time.monotonic()
            p, raw, status = measure(args.uart, args.timeout)
            secs = time.monotonic() - t0
            w.writerow(row_for("A", c, i, status, p, md5, ram, fl, secs)); fh.flush()
            if status != "ok":
                (out / "raw" / f"A_{c['cell']}_{i:03d}_{status}.log").write_text(raw)
                print(f"  repeat {i}: {status}", flush=True)
            elif i == 1 or i % 25 == 0:
                print(f"  repeat {i}/{args.n_reset} ok "
                      f"keygen={p['keygen_avg']} encaps={p['encaps_avg']} "
                      f"decaps={p['decaps_avg']} ({secs:.1f}s)", flush=True)
        # keep one representative raw log per cell as evidence
        p, raw, status = measure(args.uart, args.timeout)
        (out / "raw" / f"A_{c['cell']}_representative.log").write_text(raw)

    # ---- Tier B: full clean-build + flash + measure repeats ----
    for name in TIER_B_CELLS:
        if args.only and name not in {s.strip() for s in args.only.split(",")}:
            continue
        c = next(x for x in CELLS if x["cell"] == name)
        print(f"\n=== Tier B {name} : {args.n_pipeline} full build+flash+measure ===",
              flush=True)
        for i in range(1, args.n_pipeline + 1):
            t0 = time.monotonic()
            subprocess.run(["rm", "-rf", str(ROOT / "build" / "Release")], check=False)
            log, _ = build(c)
            if log is None:
                w.writerow(row_for("B", c, i, "build_failed", None, "", "", "", 0)); fh.flush()
                continue
            md5 = hashlib.md5(ELF.read_bytes()).hexdigest()
            ram, fl = ib.parse_build_memory(log)
            if not flash():
                w.writerow(row_for("B", c, i, "flash_failed", None, md5, ram, fl, 0)); fh.flush()
                continue
            p, raw, status = measure(args.uart, args.timeout)
            secs = time.monotonic() - t0
            w.writerow(row_for("B", c, i, status, p, md5, ram, fl, secs)); fh.flush()
            if status != "ok":
                (out / "raw" / f"B_{name}_{i:03d}_{status}.log").write_text(raw)
            print(f"  pipeline {i}/{args.n_pipeline} {status} md5={md5[:8]} ({secs:.1f}s)",
                  flush=True)

    fh.close()
    print(f"\nTotal wall time: {(time.monotonic()-t_start)/60:.1f} min")
    print(f"Results: {csv_path}")


if __name__ == "__main__":
    sys.exit(main())
