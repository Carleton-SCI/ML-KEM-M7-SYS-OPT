#!/usr/bin/env python3
import argparse
import csv
import datetime as dt
import os
import re
import select
import shutil
import subprocess
import sys
import termios
import time
import zipfile
from dataclasses import dataclass
from pathlib import Path
from xml.sax.saxutils import escape

from tools.stm32f767_clock_profiles import PRESET_CLOCK_MHZ, PROFILES, parse_clock


ROOT = Path(__file__).resolve().parent
ELF = ROOT / "build" / "Release" / "STM_ML-KEM_V3.elf"
BIN = ROOT / "build" / "Release" / "STM_ML-KEM_V3.bin"
DEFAULT_UART = "/dev/ttyACM0"
DEFAULT_BAUD = 115200
DEFAULT_TIMEOUT_SECONDS = 60

KEY_SIZES = ("512", "768", "1024")
OPTS = ("speed", "stack")
CACHED_MODES = ("ON", "OFF")
RNG_MODES = ("ON", "OFF")
VALID_CLOCK_MHZ = tuple(sorted(PROFILES))
CLOCK_PROFILES = tuple(f"{mhz}mhz" for mhz in PRESET_CLOCK_MHZ)
ENDPOINT_CLOCK_PROFILES = ("24mhz", "216mhz")
FLASH_ACCEL_MODES = ("off", "art_prefetch")
DTCM_CONST_MODES = ("off", "ntt")
PROFILE_MODES = ("off", "ops")
BENCH_MODES = ("one_by_one", "sequence")
OPERATIONS = ("keygen", "encaps", "decaps")
STATS = ("min", "avg", "med", "max")
ITCM_GROUPS = (
    ("itcm_keccak", "Keccak permutation", "keccak"),
    ("itcm_keccak_helper", "Keccak helper routines", "keccak_helper"),
    ("itcm_ntt", "NTT and inverse NTT", "ntt"),
    ("itcm_matacc_cache", "matacc cache assembly kernels", "matacc_cache"),
    ("itcm_matacc_opt", "matacc opt assembly kernels", "matacc_opt"),
    ("itcm_matacc_capture", "cached-A capture matacc kernels", "matacc_capture"),
    ("itcm_frombytes", "frombytes/dot-product kernels", "frombytes"),
    ("itcm_basemul", "basemul dot-product kernels", "basemul"),
    ("itcm_compress", "compression/decompression kernels", "compress"),
    ("itcm_reduce", "reduction/pointwise kernels", "reduce"),
    ("itcm_cbd", "CBD/noise post-processing kernels", "cbd"),
)
# Kept in the dataclass/parser for old logs and manual debugging, but hidden
# from benchmark menus after repeated UART timeouts or invalid secret-match
# reports in paper_itcm_failed_sample_* and paper_itcm_oneoff*_sample_*.
UNSTABLE_ITCM_GROUP_NAMES = {"keccak_helper", "compress", "cbd"}
BENCHMARK_ITCM_GROUPS = tuple(
    item for item in ITCM_GROUPS if item[2] not in UNSTABLE_ITCM_GROUP_NAMES
)
ITCM_FIELDS = tuple(field for field, _, _ in ITCM_GROUPS)
ITCM_OFF_FLAGS = {field: "OFF" for field in ITCM_FIELDS}


def on_off(value):
    return "ON" if str(value).upper() == "ON" else "OFF"


def itcm_profile_from_flags(flags):
    names = [
        name
        for field, _, name in ITCM_GROUPS
        if on_off(flags.get(field, "OFF")) == "ON"
    ]
    return "+".join(names) if names else "off"


def itcm_flags_from_profile(profile):
    flags = dict(ITCM_OFF_FLAGS)
    profile = (profile or "off").strip().lower()
    if profile in ("", "off"):
        return flags
    if profile == "keccak":
        flags["itcm_keccak"] = "ON"
        return flags
    if profile == "ntt":
        flags["itcm_ntt"] = "ON"
        return flags
    # Legacy parser support for old benchmark CSV/log rows. New benchmark
    # menus and paper-matrix categories use the separate Keccak and NTT knobs.
    if profile == "keccak_ntt":
        flags["itcm_keccak"] = "ON"
        flags["itcm_ntt"] = "ON"
        return flags
    for item in profile.split("+"):
        for field, _, name in ITCM_GROUPS:
            if item == name:
                flags[field] = "ON"
                break
        else:
            raise ValueError(f"Unsupported ITCM profile item: {item}")
    return flags


def fill_itcm_fields(row):
    has_explicit = any(row.get(field) for field in ITCM_FIELDS)
    flags = {}
    if has_explicit:
        flags = {field: on_off(row.get(field, "OFF")) for field in ITCM_FIELDS}
    else:
        try:
            flags = itcm_flags_from_profile(row.get("itcm", "off"))
        except ValueError:
            flags = dict(ITCM_OFF_FLAGS)
    row.update(flags)
    row["itcm"] = itcm_profile_from_flags(flags)
    return row


def itcm_flag_sets_from_modes(itcm_modes):
    normalized = []
    for mode in itcm_modes:
        if isinstance(mode, dict):
            flags = dict(ITCM_OFF_FLAGS)
            for field in ITCM_FIELDS:
                flags[field] = on_off(mode.get(field, "OFF"))
        else:
            flags = itcm_flags_from_profile(mode)
        profile = itcm_profile_from_flags(flags)
        if profile not in {item[0] for item in normalized}:
            normalized.append((profile, flags))
    return [flags for _, flags in normalized]


def public_cache_mode(public_hpk, public_pk):
    hpk_on = public_hpk == "ON"
    pk_on = public_pk == "ON"
    if hpk_on and pk_on:
        return "hpk_pk"
    if hpk_on:
        return "hpk"
    if pk_on:
        return "pk"
    return "off"


def public_cache_flags(profile):
    if profile == "off":
        return "OFF", "OFF"
    if profile == "hpk":
        return "ON", "OFF"
    if profile == "pk":
        return "OFF", "ON"
    if profile == "hpk_pk":
        return "ON", "ON"
    raise ValueError(f"Unsupported public cache profile: {profile}")


def fill_public_cache_fields(row):
    hpk = row.get("public_hpk")
    pk = row.get("public_pk")
    profile = row.get("public_cache")

    if (not hpk or not pk) and profile:
        try:
            derived_hpk, derived_pk = public_cache_flags(profile)
        except ValueError:
            derived_hpk, derived_pk = "OFF", "OFF"
        hpk = hpk or derived_hpk
        pk = pk or derived_pk

    hpk = hpk or "OFF"
    pk = pk or "OFF"
    row["public_hpk"] = hpk
    row["public_pk"] = pk
    row["public_cache"] = public_cache_mode(hpk, pk)
    return row


@dataclass(frozen=True)
class BenchmarkCase:
    backend: str
    arch: str
    opt: str
    key_size: str
    variant: str
    keccak: str
    cached_a: str
    hw_rng: str
    clock: str
    flash_accel: str = "off"
    itcm_keccak: str = "OFF"
    itcm_keccak_helper: str = "OFF"
    itcm_ntt: str = "OFF"
    itcm_matacc_cache: str = "OFF"
    itcm_matacc_opt: str = "OFF"
    itcm_matacc_capture: str = "OFF"
    itcm_frombytes: str = "OFF"
    itcm_basemul: str = "OFF"
    itcm_compress: str = "OFF"
    itcm_reduce: str = "OFF"
    itcm_cbd: str = "OFF"
    dtcm_const: str = "off"
    profile: str = "off"
    public_hpk: str = "OFF"
    public_pk: str = "OFF"
    bench_mode: str = "one_by_one"

    @property
    def public_cache(self) -> str:
        return public_cache_mode(self.public_hpk, self.public_pk)

    @property
    def itcm_flags(self) -> dict:
        return {field: getattr(self, field) for field in ITCM_FIELDS}

    @property
    def itcm(self) -> str:
        return itcm_profile_from_flags(self.itcm_flags)

    def label(self) -> str:
        rng = "trng" if self.hw_rng == "ON" else "pseudo"
        cached = "cachedA" if self.cached_a == "ON" else "regenA"
        extras = (
            f"flash-{self.flash_accel}-itcm-{self.itcm}-"
            f"dtcm-{self.dtcm_const}-prof-{self.profile}-"
            f"pcache-{self.public_cache}-mode-{self.bench_mode}"
        )
        return (
            f"{self.backend}-mlkem{self.key_size}-{self.variant}-"
            f"{self.keccak}-{cached}-{rng}-{self.clock}-{extras}"
        )


FIELDNAMES = (
    "run_timestamp",
    "case_index",
    "status",
    "backend",
    "arch",
    "opt",
    "key_size",
    "variant",
    "keccak",
    "cached_a",
    "hw_rng",
    "clock",
    "flash_accel",
    "itcm",
    "itcm_keccak",
    "itcm_keccak_helper",
    "itcm_ntt",
    "itcm_matacc_cache",
    "itcm_matacc_opt",
    "itcm_matacc_capture",
    "itcm_frombytes",
    "itcm_basemul",
    "itcm_compress",
    "itcm_reduce",
    "itcm_cbd",
    "dtcm_const",
    "profile",
    "public_hpk",
    "public_pk",
    "public_cache",
    "bench_mode",
    "hclk_hz",
    "iterations",
    "keygen_min",
    "keygen_min_ms",
    "keygen_avg",
    "keygen_avg_ms",
    "keygen_med",
    "keygen_med_ms",
    "keygen_max",
    "keygen_max_ms",
    "encaps_min",
    "encaps_min_ms",
    "encaps_avg",
    "encaps_avg_ms",
    "encaps_med",
    "encaps_med_ms",
    "encaps_max",
    "encaps_max_ms",
    "decaps_min",
    "decaps_min_ms",
    "decaps_avg",
    "decaps_avg_ms",
    "decaps_med",
    "decaps_med_ms",
    "decaps_max",
    "decaps_max_ms",
    "shared_secret_match",
    "ram_used_bytes",
    "flash_used_bytes",
    "profile_log",
    "raw_log",
    "error",
)


CONFIG_RE = re.compile(
    r"Config:\s+ML-KEM-(?P<key_size>\S+)\s+"
    r"backend=(?P<backend>\S+)\s+"
    r"variant=(?P<variant>\S+)\s+"
    r"arch=(?P<arch>\S+)\s+"
    r"opt=(?P<opt>\S+)\s+"
    r"keccak=(?P<keccak>\S+)\s+"
    r"cached_a=(?P<cached_a>\S+)\s+"
    r"clock=(?P<clock>\S+)"
    r"(?:\s+flash_accel=(?P<flash_accel>\S+)\s+"
    r"itcm=(?P<itcm>\S+)\s+"
    r"(?:(?:itcm_keccak=(?P<itcm_keccak>\S+)\s+"
    r"itcm_keccak_helper=(?P<itcm_keccak_helper>\S+)\s+"
    r"itcm_ntt=(?P<itcm_ntt>\S+)\s+"
    r"itcm_matacc_cache=(?P<itcm_matacc_cache>\S+)\s+"
    r"itcm_matacc_opt=(?P<itcm_matacc_opt>\S+)\s+"
    r"itcm_matacc_capture=(?P<itcm_matacc_capture>\S+)\s+"
    r"itcm_frombytes=(?P<itcm_frombytes>\S+)\s+"
    r"itcm_basemul=(?P<itcm_basemul>\S+)\s+"
    r"itcm_compress=(?P<itcm_compress>\S+)\s+"
    r"itcm_reduce=(?P<itcm_reduce>\S+)\s+"
    r"itcm_cbd=(?P<itcm_cbd>\S+)\s+)?)"
    r"dtcm_const=(?P<dtcm_const>\S+)\s+"
    r"profile=(?P<profile>\S+)\s+"
    r"(?:(?:public_hpk=(?P<public_hpk>\S+)\s+"
    r"public_pk=(?P<public_pk>\S+)\s+)?)"
    r"public_cache=(?P<public_cache>\S+)"
    r"(?:\s+bench_mode=(?P<bench_mode>\S+))?)?"
)
HCLK_RE = re.compile(r"HCLK=(?P<hclk>\d+)\s+Hz")
ITER_RE = re.compile(r"=== Benchmark Results \((?P<iterations>\d+) iterations\) ===")
STAT_RE = re.compile(
    r"^(?P<name>Keygen|Encaps|Decaps)\s*:\s*"
    r"min=(?P<min>\d+)(?:\s+\((?P<min_ms>[0-9.]+)\s+ms\))?\s+"
    r"avg=(?P<avg>\d+)(?:\s+\((?P<avg_ms>[0-9.]+)\s+ms\))?\s+"
    r"med=(?P<med>\d+)(?:\s+\((?P<med_ms>[0-9.]+)\s+ms\))?\s+"
    r"max=(?P<max>\d+)(?:\s+\((?P<max_ms>[0-9.]+)\s+ms\))?",
    re.MULTILINE,
)
BENCH_MODE_ONE_BY_ONE_RE = re.compile(r"Running benchmarks one by one")
BENCH_MODE_SEQUENCE_RE = re.compile(r"Running benchmarks in one loop")
MATCH_RE = re.compile(r"Shared Secret Match:\s*(?P<match>\d+)")
RAM_RE = re.compile(r"\bRAM:\s+(?P<ram>\d+)\s+B")
FLASH_RE = re.compile(r"\bFLASH:\s+(?P<flash>\d+)\s+B")
PROFILE_LINE_RE = re.compile(r"^PROFILE\s+.+$", re.MULTILINE)


def prompt_choice(prompt, options, default=1):
    print()
    print(prompt)
    for idx, option in enumerate(options, 1):
        suffix = " [default]" if idx == default else ""
        print(f"  {idx}. {option}{suffix}")

    while True:
        value = input("> ").strip()
        if not value:
            return default
        if value.isdigit() and 1 <= int(value) <= len(options):
            return int(value)
        print(f"Enter a number from 1 to {len(options)}.")


def prompt_yes_no(prompt, default=True):
    suffix = "Y/n" if default else "y/N"
    while True:
        value = input(f"{prompt} [{suffix}] ").strip().lower()
        if not value:
            return default
        if value in ("y", "yes"):
            return True
        if value in ("n", "no"):
            return False
        print("Please answer yes or no.")


def normalize_clock_profile(value):
    mhz = parse_clock(str(value))
    if mhz not in PROFILES:
        valid = ", ".join(f"{item}mhz" for item in VALID_CLOCK_MHZ)
        raise ValueError(f"Unsupported clock {mhz}mhz. Valid choices: {valid}")
    return f"{mhz}mhz"


def prompt_clock_profile():
    custom_label = "Custom valid clock"
    options = CLOCK_PROFILES + (custom_label,)
    selected = prompt_choice("Select clock profile", options)
    if selected <= len(CLOCK_PROFILES):
        return CLOCK_PROFILES[selected - 1]

    while True:
        value = input("Clock in MHz or Hz, e.g. 42mhz or 42000000: ").strip()
        try:
            return normalize_clock_profile(value)
        except ValueError as exc:
            print(exc)


def prompt_clock_sweep():
    selected = prompt_choice(
        "Select clock sweep",
        (
            "Endpoint clocks only: 24mhz and 216mhz",
            "All predefined 16 clock stops",
            "Custom comma-separated valid clocks",
        ),
    )
    if selected == 1:
        return list(ENDPOINT_CLOCK_PROFILES)
    if selected == 2:
        return list(CLOCK_PROFILES)

    while True:
        value = input("Clock list, e.g. 24mhz,72mhz,216mhz: ").strip()
        try:
            clocks = [normalize_clock_profile(part) for part in value.split(",")]
        except ValueError as exc:
            print(exc)
            continue
        clocks = list(dict.fromkeys(clocks))
        if clocks:
            return clocks
        print("Enter at least one clock.")


def prompt_itcm_flags():
    flags = {}
    for field, label, _ in BENCHMARK_ITCM_GROUPS:
        flags[field] = "ON" if prompt_yes_no(f"Place {label} in ITCM?", False) else "OFF"
    return flags


def prompt_itcm_sweep_modes():
    choices = []
    for field, label, _ in BENCHMARK_ITCM_GROUPS:
        modes = []
        if prompt_yes_no(f"Benchmark {label} in ITCM?", False):
            modes.append("ON")
        if prompt_yes_no(f"Benchmark {label} not in ITCM?", True):
            modes.append("OFF")
        if not modes:
            modes = ["OFF"]
        choices.append((field, modes))

    modes = [dict(ITCM_OFF_FLAGS)]
    for field, values in choices:
        expanded = []
        for current in modes:
            for value in values:
                item = dict(current)
                item[field] = value
                expanded.append(item)
        modes = expanded
    return modes


def prompt_advanced_single():
    flash_accel = FLASH_ACCEL_MODES[
        prompt_choice("Select Flash accelerator", FLASH_ACCEL_MODES) - 1
    ]
    itcm_flags = prompt_itcm_flags()
    dtcm_const = DTCM_CONST_MODES[
        prompt_choice("Select DTCM public constants", DTCM_CONST_MODES) - 1
    ]
    profile = PROFILE_MODES[prompt_choice("Select profiling", PROFILE_MODES) - 1]
    public_hpk = "ON" if prompt_yes_no("Cache public H(pk)?", False) else "OFF"
    public_pk = "ON" if prompt_yes_no("Cache parsed public key?", False) else "OFF"
    bench_mode = BENCH_MODES[
        prompt_choice("Select benchmark loop mode", BENCH_MODES) - 1
    ]
    return flash_accel, itcm_flags, dtcm_const, profile, public_hpk, public_pk, bench_mode


def knob_combinations(cached_modes, rng_modes, clock_profiles,
                      flash_accel_modes=("off",), itcm_modes=("off",),
                      dtcm_const_modes=("off",), profile_modes=("off",),
                      public_hpk_modes=("OFF",), public_pk_modes=("OFF",),
                      bench_modes=("one_by_one",)):
    itcm_flag_sets = itcm_flag_sets_from_modes(itcm_modes)
    for cached in cached_modes:
        for rng in rng_modes:
            for clock in clock_profiles:
                for flash_accel in flash_accel_modes:
                    for itcm_flags in itcm_flag_sets:
                        for dtcm_const in dtcm_const_modes:
                            for profile in profile_modes:
                                for public_hpk in public_hpk_modes:
                                    for public_pk in public_pk_modes:
                                        for bench_mode in bench_modes:
                                            yield {
                                                "cached_a": cached,
                                                "hw_rng": rng,
                                                "clock": clock,
                                                "flash_accel": flash_accel,
                                                **itcm_flags,
                                                "dtcm_const": dtcm_const,
                                                "profile": profile,
                                                "public_hpk": public_hpk,
                                                "public_pk": public_pk,
                                                "bench_mode": bench_mode,
                                            }


def default_case():
    return BenchmarkCase(
        backend="pqm7",
        arch="m7",
        opt="speed",
        key_size="768",
        variant="m7fspeed",
        keccak="m4_opt_m7",
        cached_a="ON",
        hw_rng="ON",
        clock="24mhz",
        flash_accel="off",
        dtcm_const="off",
        profile="off",
        public_hpk="OFF",
        public_pk="OFF",
        bench_mode="one_by_one",
    )


def pqm4_cases(keys, opts, cached_modes, rng_modes, clock_profiles,
               flash_accel_modes=("off",), itcm_modes=("off",),
               dtcm_const_modes=("off",), profile_modes=("off",),
               public_hpk_modes=("OFF",), public_pk_modes=("OFF"),
               bench_modes=("one_by_one",)):
    cases = []
    for key in keys:
        for opt in opts:
            for knobs in knob_combinations(cached_modes, rng_modes,
                                           clock_profiles, flash_accel_modes,
                                           itcm_modes, dtcm_const_modes,
                                           profile_modes, public_hpk_modes,
                                           public_pk_modes, bench_modes):
                cases.append(
                    BenchmarkCase(
                        backend="pqm4",
                        arch="m4",
                        opt=opt,
                        key_size=key,
                        variant=f"m4f{opt}",
                        keccak="m4",
                        **knobs,
                    )
                )
    return cases


def pqm7_cases_from_bases(bases, cached_modes, rng_modes, clock_profiles,
                          flash_accel_modes=("off",), itcm_modes=("off",),
                          dtcm_const_modes=("off",), profile_modes=("off",),
                          public_hpk_modes=("OFF",), public_pk_modes=("OFF"),
                          bench_modes=("one_by_one",)):
    cases = []
    for arch, opt, variant, keccak, key in bases:
        for knobs in knob_combinations(cached_modes, rng_modes, clock_profiles,
                                       flash_accel_modes, itcm_modes,
                                       dtcm_const_modes, profile_modes,
                                       public_hpk_modes, public_pk_modes,
                                       bench_modes):
            cases.append(
                BenchmarkCase(
                    backend="pqm7",
                    arch=arch,
                    opt=opt,
                    key_size=key,
                    variant=variant,
                    keccak=keccak,
                    **knobs,
                )
            )
    return cases


def pqm7_sourceable_cases(keys, cached_modes, rng_modes, clock_profiles,
                          flash_accel_modes=("off",), itcm_modes=("off",),
                          dtcm_const_modes=("off",), profile_modes=("off",),
                          public_hpk_modes=("OFF",), public_pk_modes=("OFF"),
                          bench_modes=("one_by_one",)):
    bases = []
    for key in keys:
        for opt in OPTS:
            bases.append(("m4", opt, f"m4f{opt}", "m4", key))
            bases.append(("m4", opt, f"m4f{opt}-m7keccak", "m7", key))
            bases.append(("m7", opt, f"m7f{opt}", "m4_opt_m7", key))

    return pqm7_cases_from_bases(bases, cached_modes, rng_modes,
                                 clock_profiles, flash_accel_modes, itcm_modes,
                                 dtcm_const_modes, profile_modes,
                                 public_hpk_modes, public_pk_modes, bench_modes)


def single_case_interactive():
    backend_idx = prompt_choice("Select source tree", ("pqm7", "pqm4"))
    backend = ("pqm7", "pqm4")[backend_idx - 1]

    key_idx = prompt_choice("Select ML-KEM key size", KEY_SIZES, default=2)
    key = KEY_SIZES[key_idx - 1]

    clock = prompt_clock_profile()

    cached = "ON" if prompt_yes_no("Use cached_A?", True) else "OFF"
    rng = "ON" if prompt_yes_no("Use hardware TRNG?", True) else "OFF"
    flash_accel, itcm_flags, dtcm_const, profile, public_hpk, public_pk, bench_mode = prompt_advanced_single()

    if backend == "pqm4":
        opt_idx = prompt_choice("Select pqm4 optimization", OPTS)
        opt = OPTS[opt_idx - 1]
        return BenchmarkCase(
            backend="pqm4",
            arch="m4",
            opt=opt,
            key_size=key,
            variant=f"m4f{opt}",
            keccak="m4",
            cached_a=cached,
            hw_rng=rng,
            clock=clock,
            flash_accel=flash_accel,
            **itcm_flags,
            dtcm_const=dtcm_const,
            profile=profile,
            public_hpk=public_hpk,
            public_pk=public_pk,
            bench_mode=bench_mode,
        )

    arch_idx = prompt_choice("Select pqm7 implementation family", ("m7", "m4"))
    arch = ("m7", "m4")[arch_idx - 1]
    opt_idx = prompt_choice("Select optimization", OPTS)
    opt = OPTS[opt_idx - 1]

    if arch == "m7":
        keccak_idx = prompt_choice(
            "Select Keccak backend",
            ("m4_opt_m7", "m7", "m4"),
        )
        keccak = ("m4_opt_m7", "m7", "m4")[keccak_idx - 1]
        variant = f"m7f{opt}"
    else:
        keccak_idx = prompt_choice("Select Keccak backend", ("m4", "m7"))
        keccak = ("m4", "m7")[keccak_idx - 1]
        variant = f"m4f{opt}" if keccak == "m4" else f"m4f{opt}-m7keccak"

    return BenchmarkCase(
        backend="pqm7",
        arch=arch,
        opt=opt,
        key_size=key,
        variant=variant,
        keccak=keccak,
        cached_a=cached,
        hw_rng=rng,
        clock=clock,
        flash_accel=flash_accel,
        **itcm_flags,
        dtcm_const=dtcm_const,
        profile=profile,
        public_hpk=public_hpk,
        public_pk=public_pk,
        bench_mode=bench_mode,
    )


def complete_cases():
    cases = []
    cases.extend(pqm4_cases(KEY_SIZES, OPTS, CACHED_MODES, RNG_MODES,
                            CLOCK_PROFILES,
                            public_hpk_modes=CACHED_MODES,
                            public_pk_modes=CACHED_MODES))
    cases.extend(pqm7_sourceable_cases(KEY_SIZES, CACHED_MODES, RNG_MODES,
                                       CLOCK_PROFILES,
                                       public_hpk_modes=CACHED_MODES,
                                       public_pk_modes=CACHED_MODES))
    return cases


def extended_cases_interactive():
    keys = [key for key in KEY_SIZES if prompt_yes_no(f"Include ML-KEM-{key}?", True)]
    if not keys:
        keys = ["768"]

    cached_modes = []
    if prompt_yes_no("Benchmark cached_A enabled?", True):
        cached_modes.append("ON")
    if prompt_yes_no("Benchmark cached_A disabled?", True):
        cached_modes.append("OFF")
    if not cached_modes:
        cached_modes = ["ON"]

    rng_modes = []
    if prompt_yes_no("Benchmark hardware TRNG?", True):
        rng_modes.append("ON")
    if prompt_yes_no("Benchmark pseudo-random fallback?", False):
        rng_modes.append("OFF")
    if not rng_modes:
        rng_modes = ["ON"]

    clock_profiles = prompt_clock_sweep()

    flash_accel_modes = []
    if prompt_yes_no("Benchmark Flash accelerator disabled?", True):
        flash_accel_modes.append("off")
    if prompt_yes_no("Benchmark Flash ART+prefetch?", False):
        flash_accel_modes.append("art_prefetch")
    if not flash_accel_modes:
        flash_accel_modes = ["off"]

    itcm_modes = prompt_itcm_sweep_modes()

    dtcm_const_modes = []
    if prompt_yes_no("Benchmark DTCM public constants disabled?", True):
        dtcm_const_modes.append("off")
    if prompt_yes_no("Benchmark NTT constants in DTCM?", False):
        dtcm_const_modes.append("ntt")
    if not dtcm_const_modes:
        dtcm_const_modes = ["off"]

    profile_modes = []
    if prompt_yes_no("Benchmark profiling disabled?", True):
        profile_modes.append("off")
    if prompt_yes_no("Benchmark operation profile output?", False):
        profile_modes.append("ops")
    if not profile_modes:
        profile_modes = ["off"]

    public_hpk_modes = []
    if prompt_yes_no("Benchmark public H(pk) cache enabled?", False):
        public_hpk_modes.append("ON")
    if prompt_yes_no("Benchmark public H(pk) cache disabled?", True):
        public_hpk_modes.append("OFF")
    if not public_hpk_modes:
        public_hpk_modes = ["OFF"]

    public_pk_modes = []
    if prompt_yes_no("Benchmark parsed public-key cache enabled?", False):
        public_pk_modes.append("ON")
    if prompt_yes_no("Benchmark parsed public-key cache disabled?", True):
        public_pk_modes.append("OFF")
    if not public_pk_modes:
        public_pk_modes = ["OFF"]

    bench_modes = []
    if prompt_yes_no("Benchmark one-by-one mode?", True):
        bench_modes.append("one_by_one")
    if prompt_yes_no("Benchmark sequence mode?", False):
        bench_modes.append("sequence")
    if not bench_modes:
        bench_modes = ["one_by_one"]

    cases = []

    if prompt_yes_no("Enable pqm4?", True):
        pqm4_opts = [opt for opt in OPTS if prompt_yes_no(f"Enable pqm4 {opt}?", True)]
        if not pqm4_opts:
            pqm4_opts = ["speed"]
        cases.extend(pqm4_cases(keys, pqm4_opts, cached_modes, rng_modes,
                                clock_profiles, flash_accel_modes, itcm_modes,
                                dtcm_const_modes, profile_modes,
                                public_hpk_modes, public_pk_modes,
                                bench_modes))

    if prompt_yes_no("Enable pqm7?", True):
        if prompt_yes_no("Enable pqm7 m4 implementation variants?", True):
            m4_opts = [opt for opt in OPTS if prompt_yes_no(f"Enable pqm7 m4 {opt}?", True)]
            m4_keccaks = []
            if prompt_yes_no("Enable pqm7 m4 variants with M4 Keccak?", True):
                m4_keccaks.append("m4")
            if prompt_yes_no("Enable pqm7 m4 variants with M7 Keccak?", True):
                m4_keccaks.append("m7")
            m4_bases = []
            for key in keys:
                for opt in m4_opts:
                    for keccak in m4_keccaks:
                        variant = f"m4f{opt}" if keccak == "m4" else f"m4f{opt}-m7keccak"
                        m4_bases.append(("m4", opt, variant, keccak, key))
            cases.extend(pqm7_cases_from_bases(m4_bases, cached_modes,
                                               rng_modes, clock_profiles,
                                               flash_accel_modes, itcm_modes,
                                               dtcm_const_modes, profile_modes,
                                               public_hpk_modes,
                                               public_pk_modes, bench_modes))

        if prompt_yes_no("Enable pqm7 m7 implementation variants?", True):
            m7_opts = [opt for opt in OPTS if prompt_yes_no(f"Enable pqm7 m7 {opt}?", True)]
            m7_keccaks = [
                keccak
                for keccak in ("m4_opt_m7", "m7", "m4")
                if prompt_yes_no(f"Enable pqm7 m7 with {keccak} Keccak?", keccak == "m4_opt_m7")
            ]
            m7_bases = []
            for key in keys:
                for opt in m7_opts:
                    for keccak in m7_keccaks:
                        m7_bases.append(("m7", opt, f"m7f{opt}", keccak, key))
            cases.extend(pqm7_cases_from_bases(m7_bases, cached_modes,
                                               rng_modes, clock_profiles,
                                               flash_accel_modes, itcm_modes,
                                               dtcm_const_modes, profile_modes,
                                               public_hpk_modes,
                                               public_pk_modes, bench_modes))

    deduped = []
    seen = set()
    for case in cases:
        key = (
            case.backend,
            case.arch,
            case.opt,
            case.key_size,
            case.variant,
            case.keccak,
            case.cached_a,
            case.hw_rng,
            case.clock,
            case.flash_accel,
            case.itcm_keccak,
            case.itcm_keccak_helper,
            case.itcm_ntt,
            case.itcm_matacc_cache,
            case.itcm_matacc_opt,
            case.itcm_matacc_capture,
            case.itcm_frombytes,
            case.itcm_basemul,
            case.itcm_compress,
            case.itcm_reduce,
            case.itcm_cbd,
            case.dtcm_const,
            case.profile,
            case.public_hpk,
            case.public_pk,
            case.bench_mode,
        )
        if key not in seen:
            seen.add(key)
            deduped.append(case)

    if not deduped:
        print("No cases selected; using the default benchmark.")
        return [default_case()]
    return deduped


def choose_cases():
    option = prompt_choice(
        "Select benchmark mode",
        (
            "Default: pqm7 ML-KEM-768 m7fspeed, m4_opt_m7, cached_A=ON, TRNG=ON, clock=24mhz, flash=off, all ITCM knobs OFF, DTCM=off, profile=off, public_hpk=OFF, public_pk=OFF, bench_mode=one_by_one",
            "Single benchmark: choose one configuration interactively",
            "Complete benchmark: sourceable pqm4/pqm7 core variants x cached_A x RNG x 16 preset clocks x public_hpk x public_pk; Flash/all ITCM knobs/DTCM/profile off; bench_mode=one_by_one",
            "Extended benchmark: choose every enabled family and optimization dimension interactively",
            "Delete all previous benchmark results and exit",
        ),
    )

    if option == 1:
        return [default_case()]
    if option == 2:
        return [single_case_interactive()]
    if option == 3:
        return complete_cases()
    if option == 4:
        return extended_cases_interactive()
    return None


def delete_previous_results(output_dir, assume_yes=False):
    output_dir = Path(output_dir)
    if not output_dir.exists():
        print(f"No previous benchmark results found at {output_dir}.")
        return

    if not output_dir.is_dir():
        raise RuntimeError(f"Benchmark output path is not a directory: {output_dir}")

    children = list(output_dir.iterdir())
    if not children:
        print(f"No previous benchmark results found at {output_dir}.")
        return

    if not assume_yes:
        print()
        print(f"This will delete all files and folders under: {output_dir}")
        if not prompt_yes_no("Delete previous benchmark results now?", False):
            print("Previous benchmark results were kept.")
            return

    for child in children:
        if child.is_dir():
            shutil.rmtree(child)
        else:
            child.unlink()

    print(f"Deleted previous benchmark results from {output_dir}.")


def run_streamed(command, cwd=ROOT, timeout=None):
    print()
    print("+ " + " ".join(str(part) for part in command))
    process = subprocess.Popen(
        command,
        cwd=cwd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1,
    )
    output = []
    start = time.monotonic()

    assert process.stdout is not None
    for line in process.stdout:
        print(line, end="")
        output.append(line)
        if timeout is not None and (time.monotonic() - start) > timeout:
            process.kill()
            output.append(f"\nCommand timed out after {timeout} seconds\n")
            return 124, "".join(output)

    return process.wait(), "".join(output)


def build_case(case):
    configure_cmd = [
        "cmake",
        "--fresh",
        "--preset",
        "Release",
        f"-DUSE_HW_RNG={case.hw_rng}",
        f"-DMLKEM_K={case.key_size}",
        f"-DMLKEM_BACKEND={case.backend}",
        f"-DMLKEM_ARCH={case.arch}",
        f"-DMLKEM_OPT={case.opt}",
        f"-DMLKEM_KECCAK={case.keccak}",
        f"-DMLKEM_VARIANT={case.variant}",
        f"-DMLKEM_CLOCK={case.clock}",
        f"-DMLKEM_FLASH_ACCEL={case.flash_accel}",
        f"-DMLKEM_ITCM_KECCAK={case.itcm_keccak}",
        f"-DMLKEM_ITCM_KECCAK_HELPER={case.itcm_keccak_helper}",
        f"-DMLKEM_ITCM_NTT={case.itcm_ntt}",
        f"-DMLKEM_ITCM_MATACC_CACHE={case.itcm_matacc_cache}",
        f"-DMLKEM_ITCM_MATACC_OPT={case.itcm_matacc_opt}",
        f"-DMLKEM_ITCM_MATACC_CAPTURE={case.itcm_matacc_capture}",
        f"-DMLKEM_ITCM_FROMBYTES={case.itcm_frombytes}",
        f"-DMLKEM_ITCM_BASEMUL={case.itcm_basemul}",
        f"-DMLKEM_ITCM_COMPRESS={case.itcm_compress}",
        f"-DMLKEM_ITCM_REDUCE={case.itcm_reduce}",
        f"-DMLKEM_ITCM_CBD={case.itcm_cbd}",
        f"-DMLKEM_DTCM_CONST={case.dtcm_const}",
        f"-DMLKEM_PROFILE={case.profile}",
        f"-DMLKEM_PUBLIC_HPK={case.public_hpk}",
        f"-DMLKEM_PUBLIC_PK={case.public_pk}",
        f"-DMLKEM_BENCH_MODE={case.bench_mode}",
        f"-DMLKEM_USE_CACHED_A={case.cached_a}",
    ]
    build_cmd = ["cmake", "--build", "--preset", "Release", f"-j{os.cpu_count() or 1}"]

    code, configure_log = run_streamed(configure_cmd)
    if code != 0:
        return code, configure_log

    code, build_log = run_streamed(build_cmd)
    return code, configure_log + build_log


def flash_firmware():
    if not ELF.exists():
        raise RuntimeError(f"ELF does not exist: {ELF}")

    programmer = shutil.which("STM32_Programmer_CLI")
    if programmer:
        return run_streamed([programmer, "-c", "port=SWD", "-w", str(ELF), "-v", "-rst"])

    openocd = shutil.which("openocd")
    if openocd:
        return run_streamed(
            [
                openocd,
                "-f",
                "interface/stlink.cfg",
                "-f",
                "target/stm32f7x.cfg",
                "-c",
                f"program {ELF} verify reset exit",
            ]
        )

    st_flash = shutil.which("st-flash")
    if st_flash:
        objcopy = shutil.which("arm-none-eabi-objcopy")
        if not objcopy:
            raise RuntimeError("st-flash is available but arm-none-eabi-objcopy is missing.")
        code, objcopy_log = run_streamed([objcopy, "-O", "binary", str(ELF), str(BIN)])
        if code != 0:
            return code, objcopy_log
        code, flash_log = run_streamed([st_flash, "--reset", "write", str(BIN), "0x08000000"])
        return code, objcopy_log + flash_log

    raise RuntimeError("No flashing tool found: need STM32_Programmer_CLI, openocd, or st-flash.")


def baud_constant(baud):
    name = f"B{baud}"
    if not hasattr(termios, name):
        raise RuntimeError(f"Unsupported baud rate for termios: {baud}")
    return getattr(termios, name)


def configure_serial(fd, baud):
    attrs = termios.tcgetattr(fd)
    attrs[0] = 0
    attrs[1] = 0
    attrs[2] = termios.CS8 | termios.CREAD | termios.CLOCAL
    attrs[3] = 0
    attrs[4] = baud_constant(baud)
    attrs[5] = baud_constant(baud)
    attrs[6][termios.VMIN] = 0
    attrs[6][termios.VTIME] = 0
    termios.tcsetattr(fd, termios.TCSANOW, attrs)
    termios.tcflush(fd, termios.TCIOFLUSH)


def read_uart(device, baud, timeout_seconds):
    if not Path(device).exists():
        raise RuntimeError(f"UART device not found: {device}")

    print()
    print(f"Reading UART from {device} at {baud} baud...")
    fd = os.open(device, os.O_RDONLY | os.O_NOCTTY | os.O_NONBLOCK)
    try:
        configure_serial(fd, baud)
        deadline = time.monotonic() + timeout_seconds
        chunks = []

        while time.monotonic() < deadline:
            ready, _, _ = select.select([fd], [], [], 0.2)
            if not ready:
                continue

            chunk = os.read(fd, 4096)
            if not chunk:
                continue

            text = chunk.decode("utf-8", errors="replace")
            print(text, end="")
            chunks.append(text)
            joined = "".join(chunks)
            if "BENCHMARK_DONE" in joined or "RNG failed" in joined:
                return joined

        chunks.append(f"\nUART timeout after {timeout_seconds} seconds\n")
        return "".join(chunks)
    finally:
        os.close(fd)


def parse_build_memory(build_log):
    ram = ""
    flash = ""
    ram_match = RAM_RE.search(build_log)
    flash_match = FLASH_RE.search(build_log)
    if ram_match:
        ram = ram_match.group("ram")
    if flash_match:
        flash = flash_match.group("flash")
    return ram, flash


def parse_uart(uart_log):
    parsed = {}

    config = CONFIG_RE.search(uart_log)
    if config:
        parsed.update({key: value for key, value in config.groupdict().items() if value is not None})
        fill_itcm_fields(parsed)
        fill_public_cache_fields(parsed)

    hclk = HCLK_RE.search(uart_log)
    if hclk:
        parsed["hclk_hz"] = hclk.group("hclk")

    iterations = ITER_RE.search(uart_log)
    if iterations:
        parsed["iterations"] = iterations.group("iterations")

    for stat in STAT_RE.finditer(uart_log):
        prefix = stat.group("name").lower()
        for item in STATS:
            parsed[f"{prefix}_{item}"] = stat.group(item)
            ms_value = stat.group(f"{item}_ms")
            if ms_value is not None:
                parsed[f"{prefix}_{item}_ms"] = ms_value

    if "bench_mode" not in parsed:
        if BENCH_MODE_ONE_BY_ONE_RE.search(uart_log):
            parsed["bench_mode"] = "one_by_one"
        elif BENCH_MODE_SEQUENCE_RE.search(uart_log):
            parsed["bench_mode"] = "sequence"

    match = MATCH_RE.search(uart_log)
    if match:
        parsed["shared_secret_match"] = match.group("match")

    profile_lines = PROFILE_LINE_RE.findall(uart_log)
    if profile_lines:
        parsed["profile_log"] = " | ".join(profile_lines)

    return parsed


def cycle_ms(cycles, hclk_hz):
    if not cycles or not hclk_hz:
        return ""
    return f"{(float(cycles) * 1000.0 / float(hclk_hz)):.6f}"


def add_wall_time_fields(row):
    hclk = row.get("hclk_hz", "")
    for operation in OPERATIONS:
        for stat in STATS:
            cycle_key = f"{operation}_{stat}"
            ms_key = f"{operation}_{stat}_ms"
            if not row.get(ms_key):
                row[ms_key] = cycle_ms(row.get(cycle_key, ""), hclk)
    return row


def empty_row(run_timestamp, case_index, case, raw_log, status, error=""):
    row = {
        "run_timestamp": run_timestamp,
        "case_index": case_index,
        "status": status,
        "backend": case.backend,
        "arch": case.arch,
        "opt": case.opt,
        "key_size": case.key_size,
        "variant": case.variant,
        "keccak": case.keccak,
        "cached_a": case.cached_a,
        "hw_rng": case.hw_rng,
        "clock": case.clock,
        "flash_accel": case.flash_accel,
        "itcm": case.itcm,
        "itcm_keccak": case.itcm_keccak,
        "itcm_keccak_helper": case.itcm_keccak_helper,
        "itcm_ntt": case.itcm_ntt,
        "itcm_matacc_cache": case.itcm_matacc_cache,
        "itcm_matacc_opt": case.itcm_matacc_opt,
        "itcm_matacc_capture": case.itcm_matacc_capture,
        "itcm_frombytes": case.itcm_frombytes,
        "itcm_basemul": case.itcm_basemul,
        "itcm_compress": case.itcm_compress,
        "itcm_reduce": case.itcm_reduce,
        "itcm_cbd": case.itcm_cbd,
        "dtcm_const": case.dtcm_const,
        "profile": case.profile,
        "public_hpk": case.public_hpk,
        "public_pk": case.public_pk,
        "public_cache": case.public_cache,
        "bench_mode": case.bench_mode,
        "hclk_hz": "",
        "iterations": "",
        "keygen_min": "",
        "keygen_min_ms": "",
        "keygen_avg": "",
        "keygen_avg_ms": "",
        "keygen_med": "",
        "keygen_med_ms": "",
        "keygen_max": "",
        "keygen_max_ms": "",
        "encaps_min": "",
        "encaps_min_ms": "",
        "encaps_avg": "",
        "encaps_avg_ms": "",
        "encaps_med": "",
        "encaps_med_ms": "",
        "encaps_max": "",
        "encaps_max_ms": "",
        "decaps_min": "",
        "decaps_min_ms": "",
        "decaps_avg": "",
        "decaps_avg_ms": "",
        "decaps_med": "",
        "decaps_med_ms": "",
        "decaps_max": "",
        "decaps_max_ms": "",
        "shared_secret_match": "",
        "ram_used_bytes": "",
        "flash_used_bytes": "",
        "profile_log": "",
        "raw_log": raw_log,
        "error": error,
    }
    return add_wall_time_fields(row)


def benchmark_case(case, case_index, run_timestamp, raw_dir, args):
    raw_log_path = raw_dir / f"{case_index:03d}_{case.label()}.log"
    log_parts = []
    row = empty_row(run_timestamp, case_index, case, str(raw_log_path), "started")

    try:
        build_code, build_log = build_case(case)
        log_parts.append("===== BUILD LOG =====\n")
        log_parts.append(build_log)

        ram, flash = parse_build_memory(build_log)
        row["ram_used_bytes"] = ram
        row["flash_used_bytes"] = flash

        if build_code != 0:
            row["status"] = "build_failed"
            row["error"] = f"Build failed with exit code {build_code}"
            return row, "".join(log_parts)

        if args.build_only:
            row["status"] = "build_only"
            return row, "".join(log_parts)

        flash_code, flash_log = flash_firmware()
        log_parts.append("\n===== FLASH LOG =====\n")
        log_parts.append(flash_log)
        if flash_code != 0:
            row["status"] = "flash_failed"
            row["error"] = f"Flash failed with exit code {flash_code}"
            return row, "".join(log_parts)

        uart_log = read_uart(args.uart, args.baud, args.timeout)
        log_parts.append("\n===== UART LOG =====\n")
        log_parts.append(uart_log)

        parsed = parse_uart(uart_log)
        for key, value in parsed.items():
            if key in row:
                row[key] = value
        add_wall_time_fields(row)

        if "BENCHMARK_DONE" not in uart_log:
            row["status"] = "uart_timeout"
            row["error"] = "BENCHMARK_DONE marker was not observed"
        elif row["shared_secret_match"] != "1":
            row["status"] = "secret_mismatch"
            row["error"] = "Shared secret did not match"
        else:
            row["status"] = "ok"

        return row, "".join(log_parts)
    except Exception as exc:
        row["status"] = "error"
        row["error"] = str(exc)
        log_parts.append(f"\nERROR: {exc}\n")
        return row, "".join(log_parts)
    finally:
        raw_log_path.write_text("".join(log_parts), encoding="utf-8")


def column_name(index):
    name = ""
    while index:
        index, remainder = divmod(index - 1, 26)
        name = chr(ord("A") + remainder) + name
    return name


def xlsx_cell(ref, value):
    value = "" if value is None else str(value)
    if re.fullmatch(r"-?\d+(\.\d+)?", value):
        return f'<c r="{ref}"><v>{value}</v></c>'
    return f'<c r="{ref}" t="inlineStr"><is><t>{escape(value)}</t></is></c>'


def write_xlsx(path, rows):
    table = [FIELDNAMES] + [[row.get(field, "") for field in FIELDNAMES] for row in rows]
    sheet_rows = []
    for row_idx, row in enumerate(table, 1):
        cells = []
        for col_idx, value in enumerate(row, 1):
            cells.append(xlsx_cell(f"{column_name(col_idx)}{row_idx}", value))
        sheet_rows.append(f'<row r="{row_idx}">{"".join(cells)}</row>')

    sheet_xml = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
        '<worksheet xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">'
        '<sheetData>'
        + "".join(sheet_rows)
        + '</sheetData></worksheet>'
    )

    with zipfile.ZipFile(path, "w", compression=zipfile.ZIP_DEFLATED) as zf:
        zf.writestr(
            "[Content_Types].xml",
            '<?xml version="1.0" encoding="UTF-8"?>'
            '<Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">'
            '<Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/>'
            '<Default Extension="xml" ContentType="application/xml"/>'
            '<Override PartName="/xl/workbook.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"/>'
            '<Override PartName="/xl/worksheets/sheet1.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml"/>'
            "</Types>",
        )
        zf.writestr(
            "_rels/.rels",
            '<?xml version="1.0" encoding="UTF-8"?>'
            '<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">'
            '<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="xl/workbook.xml"/>'
            "</Relationships>",
        )
        zf.writestr(
            "xl/workbook.xml",
            '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
            '<workbook xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" '
            'xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">'
            '<sheets><sheet name="ML-KEM Benchmarks" sheetId="1" r:id="rId1"/></sheets>'
            "</workbook>",
        )
        zf.writestr(
            "xl/_rels/workbook.xml.rels",
            '<?xml version="1.0" encoding="UTF-8"?>'
            '<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">'
            '<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet" Target="worksheets/sheet1.xml"/>'
            "</Relationships>",
        )
        zf.writestr("xl/worksheets/sheet1.xml", sheet_xml)


def write_spreadsheets(output_base, rows):
    csv_path = output_base.with_suffix(".csv")
    xlsx_path = output_base.with_suffix(".xlsx")

    with csv_path.open("w", newline="", encoding="utf-8") as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=FIELDNAMES)
        writer.writeheader()
        writer.writerows(add_wall_time_fields(row) for row in rows)

    write_xlsx(xlsx_path, [add_wall_time_fields(row) for row in rows])
    return csv_path, xlsx_path


def parse_args():
    parser = argparse.ArgumentParser(description="Interactive ML-KEM benchmark runner")
    parser.add_argument("--uart", default=DEFAULT_UART, help=f"UART device, default {DEFAULT_UART}")
    parser.add_argument("--baud", type=int, default=DEFAULT_BAUD, help=f"UART baud, default {DEFAULT_BAUD}")
    parser.add_argument(
        "--timeout",
        type=int,
        default=DEFAULT_TIMEOUT_SECONDS,
        help=f"UART timeout per run, default {DEFAULT_TIMEOUT_SECONDS}s",
    )
    parser.add_argument(
        "--output-dir",
        default="benchmark_results",
        help="Directory for timestamped spreadsheets and raw logs",
    )
    parser.add_argument(
        "--build-only",
        action="store_true",
        help="Only build selected cases; do not flash or read UART",
    )
    parser.add_argument(
        "--yes",
        action="store_true",
        help="Skip the final confirmation prompt before running selected cases",
    )
    return parser.parse_args()


def main():
    args = parse_args()
    output_root = ROOT / args.output_dir
    cases = choose_cases()

    if cases is None:
        delete_previous_results(output_root, assume_yes=args.yes)
        return 0

    print()
    print(f"Selected {len(cases)} benchmark case(s).")
    for idx, case in enumerate(cases, 1):
        print(f"  {idx:3d}. {case.label()}")

    if not args.yes and prompt_yes_no("Delete previous benchmark results before this run?", False):
        delete_previous_results(output_root, assume_yes=True)

    if not args.yes and not prompt_yes_no("Start benchmark run?", True):
        print("Benchmark run cancelled.")
        return 0

    run_timestamp = dt.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    run_dir = output_root / f"mlkem_benchmark_{run_timestamp}"
    raw_dir = run_dir / "raw"
    run_dir.mkdir(parents=True, exist_ok=True)
    raw_dir.mkdir(parents=True, exist_ok=True)
    output_base = run_dir / "results"

    rows = []
    for idx, case in enumerate(cases, 1):
        print()
        print(f"===== Case {idx}/{len(cases)}: {case.label()} =====")
        row, _ = benchmark_case(case, idx, run_timestamp, raw_dir, args)
        rows.append(row)
        csv_path, xlsx_path = write_spreadsheets(output_base, rows)
        print(f"Case status: {row['status']}")
        if row["error"]:
            print(f"Error: {row['error']}")
        print(f"Updated spreadsheet: {xlsx_path}")

    csv_path, xlsx_path = write_spreadsheets(output_base, rows)
    print()
    print("Benchmark run finished.")
    print(f"Run folder: {run_dir}")
    print(f"CSV : {csv_path}")
    print(f"XLSX: {xlsx_path}")
    print(f"Raw logs: {raw_dir}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
