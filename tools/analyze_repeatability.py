#!/usr/bin/env python3
"""Audit R2.2 evidence without pooling within-run and between-run dispersion.

Reads the preserved repeatability CSV, checks each (tier, configuration) group,
and writes descriptive summaries. C_n1 is reported separately as superseded
evidence. No confidence intervals, significance tests, or hardware runs.
"""
import argparse
import csv
import hashlib
import statistics
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT = ROOT / "benchmark_results/repeatability_2026-09-05/results.csv"
OUTPUT = ROOT / "benchmark_results/repeatability_2026-09-05"
OPS = ("keygen", "encaps", "decaps")
REPEATS = {"A": 100, "B": 20, "C": 20, "C_n1": 20}
METADATA = (
    "key_size", "clock", "itcm", "dtcm_const", "flash_accel", "hw_rng",
    "elf_md5", "ram_used_bytes", "flash_used_bytes", "hclk_hz", "iterations",
)


def load_groups(path):
    with path.open(newline="") as stream:
        rows = list(csv.DictReader(stream))
    groups = defaultdict(list)
    for row in rows:
        if row["status"] != "ok" or row["shared_secret_match"] != "1":
            raise ValueError(f"Unsuccessful row, not silently excluded: {row}")
        if row["tier"] not in REPEATS or row["iterations"] != "100":
            raise ValueError(f"Unexpected tier or iteration count: {row}")
        for op in OPS:
            low, mean, med, high = (int(row[f"{op}_{s}"])
                                   for s in ("min", "avg", "med", "max"))
            if not (0 < low <= mean <= high and low <= med <= high):
                raise ValueError(f"Invalid timing summaries: {row}")
            if row["tier"] in ("C", "C_n1"):
                # Match firmware's integer-cycle SD and four-decimal truncation.
                relative = (int(row[f"{op}_sd"]) * 1000000 // mean) / 10000
                if float(row[f"{op}_sd_rel"]) != relative:
                    raise ValueError(f"Inconsistent relative SD: {row}")
            elif row[f"{op}_sd"] or row[f"{op}_sd_rel"]:
                raise ValueError(f"Unexpected within-run SD in timing tier: {row}")
        groups[row["tier"], row["cell"]].append(row)
    for (tier, cell), group in groups.items():
        expected = REPEATS[tier]
        if (len(group) != expected or
                {int(r["repeat"]) for r in group} != set(range(1, expected + 1))):
            raise ValueError(f"Missing or duplicate repetitions: {tier}/{cell}")
        for field in METADATA:
            if len({r[field] for r in group}) != 1:
                raise ValueError(f"Mixed {field}: {tier}/{cell}")
    if Counter(tier for tier, _ in groups) != {"A": 11, "B": 2, "C": 11, "C_n1": 11}:
        raise ValueError("Unexpected configuration coverage for this evidence set")
    return rows, groups


def summarize(groups, provenance):
    summaries = []
    for (tier, cell), group in sorted(groups.items()):
        for op in OPS:
            means = [int(r[f"{op}_avg"]) for r in group]
            mean = statistics.fmean(means)
            sd = statistics.stdev(means)
            inner = [float(r[f"{op}_sd_rel"]) for r in group
                     if r[f"{op}_sd_rel"]]
            summaries.append({
                "tier": tier, "cell": cell, "operation": op,
                "repeats": len(group),
                **{field: group[0][field] for field in METADATA},
                "run_mean_cycles_mean": mean,
                "run_mean_cycles_min": min(means),
                "run_mean_cycles_max": max(means),
                "run_mean_cycles_sd_n_minus_1": sd,
                "run_mean_relative_sd_pct": 100 * sd / mean,
                "run_mean_relative_range_pct": 100 * (max(means) - min(means)) / mean,
                "within_run_sd_denominator": {"C": "n", "C_n1": "n-1"}.get(tier, ""),
                "within_run_relative_sd_pct_min": min(inner) if inner else "",
                "within_run_relative_sd_pct_median": statistics.median(inner) if inner else "",
                "within_run_relative_sd_pct_max": max(inner) if inner else "",
                "evidence_use": "superseded" if tier == "C_n1" else "current",
                **provenance,
            })
    return summaries


def comparisons(groups, provenance):
    comparisons = []
    for knob in ("art", "dtcm", "itcm"):
        for clock in (24, 216):
            base, cell = f"base_768_{clock}", f"{knob}_768_{clock}"
            for op in OPS:
                bv = [int(r[f"{op}_avg"]) for r in groups["A", base]]
                kv = [int(r[f"{op}_avg"]) for r in groups["A", cell]]
                bm, km = statistics.fmean(bv), statistics.fmean(kv)
                comparisons.append({
                    "tier": "A", "baseline_cell": base, "profile_cell": cell,
                    "operation": op, "repeats_per_configuration": len(bv),
                    "iterations_per_run": 100,
                    "baseline_grand_mean_cycles": bm, "profile_grand_mean_cycles": km,
                    "cycle_reduction_pct": 100 * (bm - km) / bm,
                    "effect_statistic": "difference_of_arithmetic_means_of_run_means",
                    "ranges_disjoint": max(kv) < min(bv) or max(bv) < min(kv),
                    "itcm_faster_gap_cycles": min(bv) - max(kv) if knob == "itcm" else "",
                    **provenance,
                })
    return comparisons


def write_csv(path, rows):
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]))
        writer.writeheader()
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("results", type=Path, nargs="?", default=DEFAULT)
    parser.add_argument("--output-dir", type=Path, default=OUTPUT)
    args = parser.parse_args()
    path = args.results.resolve()
    rows, groups = load_groups(path)
    provenance = {
        "source_csv": str(path.relative_to(ROOT)) if path.is_relative_to(ROOT) else str(path),
        "source_csv_sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
        "analysis_script": str(Path(__file__).resolve().relative_to(ROOT)),
        "analysis_script_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
    }
    summaries = summarize(groups, provenance)
    effects = comparisons(groups, provenance)
    args.output_dir.mkdir(parents=True, exist_ok=True)
    write_csv(args.output_dir / "dispersion_by_configuration.csv", summaries)
    write_csv(args.output_dir / "effects_vs_baseline.csv", effects)
    print(f"Validated {len(rows)} rows: {dict(Counter(r['tier'] for r in rows))}")
    print("All statuses/matches/counts/relative-SD fields valid; metadata and ELF MD5 stable per tier/cell.")
    for rng, label in (("OFF", "Fixed seed"), ("ON", "TRNG")):
        within = [s for s in summaries if s["tier"] == "C" and s["hw_rng"] == rng]
        across = [s["run_mean_relative_sd_pct"] for s in summaries
                  if s["tier"] == "A" and s["hw_rng"] == rng]
        print(f"{label}: within-run SD (C) "
              f"{min(s['within_run_relative_sd_pct_min'] for s in within):.4f}--"
              f"{max(s['within_run_relative_sd_pct_max'] for s in within):.4f}%; "
              f"SD of run means (A) {min(across):.6f}--{max(across):.6f}%")
    for row in effects:
        if row["profile_cell"].startswith("itcm_"):
            print(f"{row['profile_cell']} {row['operation']}: "
                  f"reduction {row['cycle_reduction_pct']:.6f}%, "
                  f"non-overlap gap {row['itcm_faster_gap_cycles']} cycles")
    print(f"Wrote {len(summaries)} dispersion and {len(effects)} comparison rows to {args.output_dir}")


if __name__ == "__main__":
    main()
