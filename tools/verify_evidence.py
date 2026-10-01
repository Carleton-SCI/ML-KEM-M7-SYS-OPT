#!/usr/bin/env python3
"""Verify the integrity and documented shape of the retained evidence sets."""

import csv
import hashlib
from collections import Counter
from pathlib import Path

from analyze_repeatability import load_groups


ROOT = Path(__file__).resolve().parents[1]
RESULTS = ROOT / "benchmark_results"
PRIMARY = (
    ("single-knob", RESULTS / "paper_matrix_single_knob_sequence_2026-06-23_19-11-45", 48),
    ("composite-clock-sweep", RESULTS / "paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45", 336),
)


class EvidenceError(RuntimeError):
    pass


def require(condition, message):
    if not condition:
        raise EvidenceError(message)


def read_csv(path):
    require(path.is_file(), f"missing file: {path.relative_to(ROOT)}")
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream))


def repo_file(value):
    require(value, "empty raw-log path")
    path = (ROOT / value).resolve()
    try:
        path.relative_to(ROOT)
    except ValueError as exc:
        raise EvidenceError(f"path escapes repository: {value}") from exc
    require(path.is_file(), f"missing referenced file: {value}")
    return path


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def relative_logs(directory):
    return {path.resolve().relative_to(ROOT).as_posix()
            for path in directory.rglob("*.log")}


def verify_primary(label, directory, expected_rows):
    results = read_csv(directory / "results.csv")
    manifest = read_csv(directory / "source_manifest.csv")
    require(len(results) == expected_rows,
            f"{label}: expected {expected_rows} result rows, found {len(results)}")
    require(len(manifest) == expected_rows,
            f"{label}: expected {expected_rows} manifest rows, found {len(manifest)}")
    require((directory / "results.xlsx").is_file(), f"{label}: missing results.xlsx")

    require(all(row["status"] == "ok" for row in results),
            f"{label}: non-ok result row")
    require(all(row["shared_secret_match"] == "1" for row in results),
            f"{label}: shared-secret mismatch")
    require(all(row["iterations"] == "100" for row in results),
            f"{label}: unexpected iteration count")

    result_by_index = {row["case_index"]: row for row in results}
    manifest_by_index = {row["case_index"]: row for row in manifest}
    require(len(result_by_index) == expected_rows, f"{label}: duplicate result index")
    require(len(manifest_by_index) == expected_rows, f"{label}: duplicate manifest index")
    require(set(result_by_index) == set(manifest_by_index),
            f"{label}: result/manifest case mismatch")
    for index, result in result_by_index.items():
        source = manifest_by_index[index]
        require((result["key_size"], result["clock"]) ==
                (source["key_size"], source["clock"]),
                f"{label}: metadata mismatch for case {index}")

    referenced = set()
    for row in manifest:
        path = repo_file(row["raw_log"])
        actual = sha256(path)
        require(actual == row["raw_sha256"],
                f"{label}: digest mismatch for {row['raw_log']}")
        referenced.add(path.relative_to(ROOT).as_posix())
    logs = relative_logs(directory / "raw")
    require(referenced == logs,
            f"{label}: manifest/raw-log set mismatch "
            f"({len(referenced)} referenced, {len(logs)} present)")
    print(f"ok: {label}: {expected_rows} rows and SHA-256-backed raw logs")


def verify_itcm():
    directory = RESULTS / "itcm_selection_2026-06-21"
    specs = (
        ("a_path", 240),
        ("composite", 126),
    )
    all_rows = []
    all_referenced = set()
    for stem, expected in specs:
        rows = read_csv(directory / f"{stem}_results.csv")
        cases = read_csv(directory / f"{stem}_case_manifest.csv")
        require(len(rows) == expected and len(cases) == expected,
                f"ITCM {stem}: expected {expected} result/manifest rows")
        require({row["case_index"] for row in rows} ==
                {row["case_index"] for row in cases},
                f"ITCM {stem}: result/manifest case mismatch")
        for row in rows:
            path = repo_file(row["raw_log"])
            all_referenced.add(path.relative_to(ROOT).as_posix())
            if row["status"] == "ok":
                require(row["shared_secret_match"] == "1",
                        f"ITCM {stem}: successful row without shared-secret match")
        all_rows.extend(rows)

    statuses = Counter(row["status"] for row in all_rows)
    expected_statuses = Counter({"ok": 324, "uart_timeout": 41, "secret_mismatch": 1})
    require(statuses == expected_statuses,
            f"ITCM: unexpected statuses {dict(statuses)}")
    logs = relative_logs(directory / "raw")
    require(all_referenced == logs,
            f"ITCM: result/raw-log set mismatch "
            f"({len(all_referenced)} referenced, {len(logs)} present)")
    print("ok: ITCM selection: 366 attempted cases "
          "(324 ok, 41 UART timeouts, 1 secret mismatch)")


def verify_repeatability():
    directory = RESULTS / "repeatability_2026-09-05"
    results_path = directory / "results.csv"
    rows, groups = load_groups(results_path)
    require(len(rows) == 1580,
            f"repeatability: expected 1580 rows, found {len(rows)}")
    require(Counter(row["tier"] for row in rows) ==
            Counter({"A": 1100, "B": 40, "C": 220, "C_n1": 220}),
            "repeatability: unexpected tier counts")
    require(len(groups) == 35, "repeatability: unexpected configuration count")
    require(len(relative_logs(directory / "raw")) == 22,
            "repeatability: expected 22 representative raw logs")

    source_digest = sha256(results_path)
    analysis_digest = sha256(ROOT / "tools/analyze_repeatability.py")
    for filename, expected in (
            ("dispersion_by_configuration.csv", 105),
            ("effects_vs_baseline.csv", 18)):
        processed = read_csv(directory / filename)
        require(len(processed) == expected,
                f"repeatability: {filename} has {len(processed)} rows")
        require({row["source_csv_sha256"] for row in processed} == {source_digest},
                f"repeatability: stale source digest in {filename}")
        require({row["analysis_script_sha256"] for row in processed} ==
                {analysis_digest},
                f"repeatability: stale analysis-script digest in {filename}")
    print("ok: repeatability: 1580 validated rows and processed provenance")


def main():
    for label, directory, count in PRIMARY:
        verify_primary(label, directory, count)
    verify_itcm()
    verify_repeatability()
    print("All retained evidence checks passed.")


if __name__ == "__main__":
    try:
        main()
    except (EvidenceError, ValueError, KeyError) as exc:
        raise SystemExit(f"evidence verification failed: {exc}") from exc
