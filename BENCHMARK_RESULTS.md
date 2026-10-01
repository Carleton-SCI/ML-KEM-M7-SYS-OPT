# Retained Benchmark Evidence

This repository retains the benchmark datasets used to support the
accompanying paper evaluation. The paper-facing tables and interpretation are
reproduced in the [Benchmarking Results](README.md#benchmarking-results)
section of the main README.

| Dataset | Rows | Scope |
| --- | ---: | --- |
| `paper_matrix_single_knob_sequence_2026-06-23_19-11-45` | 48 | Single-optimization measurements across ML-KEM parameter sets and endpoint clocks. |
| `paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45` | 336 | Composite optimization profiles across the supported clock sweep. |
| `itcm_selection_2026-06-21` | 366 | Candidate ITCM placements with raw UART evidence. |
| `repeatability_2026-09-05` | 1580 | Targeted dispersion and repeatability measurements. |

The two primary datasets contain `results.csv`, `results.xlsx`, summaries, raw
UART logs, and a `source_manifest.csv`. Their manifests map every retained row
to its raw log and record a SHA-256 digest. The supporting ITCM and
repeatability directories document their own evidence layouts and limitations.

For portability and local-path privacy, absolute build paths in the copied raw
UART logs were replaced with `<REPOSITORY_ROOT>`. The manifest digests in this
artifact were recalculated after that path-only sanitization. Measurement
values, firmware output, and benchmark configuration fields are otherwise
retained. Historical `bundle://` values identify archived development sources;
the repository-relative `raw_log` and `raw_sha256` fields are sufficient to
verify this public snapshot.

The scripts in `interactive_benchmark.py`, `run_benchmark.sh`, and `tools/`
can reproduce new measurements on the supported target and toolchain.
Run `python3 tools/verify_evidence.py` for an offline integrity check.
