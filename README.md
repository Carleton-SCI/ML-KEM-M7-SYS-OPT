# ML-KEM Cortex-M7 Benchmark Suite

This repository contains the firmware, build tooling, benchmark runners, and
retained measurement results for system-level optimization of ML-KEM on the
STM32F767ZI (Arm Cortex-M7). It supports ML-KEM-512, ML-KEM-768, and
ML-KEM-1024 through sourceable pqm4 and pqm7 speed/stack variants.

## Accompanying Publication

This is the software and evidence artifact for:

> **System-Level Optimization Beyond Cryptographic Kernels: An ML-KEM Case
> Study on Arm Cortex-M7**

Authors:

- Mahmoud Abdelhafeez Sayed, Systems and Computer Engineering, Carleton
  University (`Mahmoud.Sayed3@Carleton.ca`)
- Mostafa Taha, Systems and Computer Engineering, Carleton University
  (`Mostafa.Taha@Carleton.ca`)
- Gurp Nijjer, Quantegra Technologies Inc., White Rock, BC, Canada
  (`gurp@quantegra.ca`)

in the 19th International Symposium on Foundations & Practice of Security held in Gatineau, Canada. DOI metadata will be added when publicly available.
Until then, use [`CITATION.cff`](CITATION.cff) for software citation metadata.

## Requirements

- CMake and GNU Make
- Arm GNU Toolchain (`arm-none-eabi-gcc`)
- Python 3
- A supported STM32 flashing utility (`STM32_Programmer_CLI`, OpenOCD, or
  `st-flash`) for hardware execution
- A serial terminal device for UART capture (default: `/dev/ttyACM0`)

The retained primary campaigns used a NUCLEO-F767ZI (STM32F767ZI, revision Z),
GNU Arm Embedded GCC 13.2.1, and STM32CubeProgrammer 2.22.0. These versions
are recorded in the raw logs; later compatible versions may also build and run
the artifact but are not the environment from which the reported data came.

## Build and Run

The non-interactive shell default is pqm7 `m7fspeed`, ML-KEM-768, the
`m4_opt_m7` Keccak permutation, cached A, a 24 MHz clock, the deterministic
benchmark generator, and `one_by_one` mode. The interactive menu default uses
the same core configuration but enables the hardware TRNG. The deterministic
generator exists only for reproducible measurements and must not be used for
operational key generation.

```sh
./run_benchmark.sh
./interactive_benchmark.py
```

`run_benchmark.sh --help` lists non-interactive configuration flags. The
individual public-data controls are `--cached-a`, `--public-hpk`, and
`--public-pk`; individual ITCM controls begin with `--itcm-`. Aggregate legacy
profiles are intentionally not supported.

The interactive runner builds, flashes, collects UART output through
`BENCHMARK_DONE`, and writes new local runs beneath `benchmark_results/`.
Generated local runs are ignored by Git unless explicitly added as retained
evidence.

The targeted repeatability campaign uses optional, default-off firmware support
for reporting dispersion across the 100 timed iterations:

```sh
python3 tools/run_repeatability_experiment.py --help
python3 tools/analyze_repeatability.py
```

The runner selects `MLKEM_STATS=stddev` only for its within-run-dispersion tier;
ordinary builds keep `MLKEM_STATS=off`.

The primary evaluation matrices can be reproduced with:

```sh
python3 tools/benchmark_paper_matrix.py --matrix single_knob
python3 tools/benchmark_paper_matrix.py --matrix composite_clock_sweep
```

Use `--help` to select individual clocks, categories, UART devices, and output
directories. New output directories are timestamped and do not overwrite the
retained evidence.

## Hardware and Clocks

The target is the NUCLEO-F767ZI using the ST-LINK 8 MHz MCO as HSE bypass.
The interactive presets are 24, 30, 32, 36, 40, 48, 60, 72, 84, 96, 108,
120, 144, 168, 192, and 216 MHz. Additional valid firmware profiles are
validated by:

```sh
python3 tools/stm32f767_clock_profiles.py --list-valid
python3 tools/stm32f767_clock_profiles.py --validate 216mhz
```

The CMake target and flashed artifacts are named `STM_ML-KEM_V3`.

The STM32F767ZI provides 16 KiB of ITCM in hardware. The measured firmware
uses a 15 KiB application linker window beginning at `0x00000400`; retained
build logs therefore report a 15 KiB ITCM region. This is a linker-window
choice in the measured configuration, not a statement that the MCU has only
15 KiB of ITCM.

## Benchmarking Results

### Evidence and measurement protocol

The paper evaluation comprises 384 primary cases: a 48-case matrix isolating
eight configurations across the three ML-KEM parameter sets and the 24 and
216 MHz endpoints, and a 336-case matrix containing the baseline plus six
composite profiles across three parameter sets and 16 clocks. The following
directories retain the supporting data:

| Evidence set | Scope |
| --- | --- |
| [`paper_matrix_single_knob_sequence_2026-06-23_19-11-45`](benchmark_results/paper_matrix_single_knob_sequence_2026-06-23_19-11-45/) | 48 single-knob cases, including the parsed-public-key cache. |
| [`paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45`](benchmark_results/paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45/) | 336 baseline and composite-profile cases over all 16 clocks. |
| [`itcm_selection_2026-06-21`](benchmark_results/itcm_selection_2026-06-21/) | 366 attempted candidate-placement cases and raw UART logs used to select the endpoint ITCM groups. |
| [`repeatability_2026-09-05`](benchmark_results/repeatability_2026-09-05/) | 1580 repeatability rows, processed summaries, and representative UART logs. |

Validate the retained row counts, statuses, raw-log references, and primary
manifest hashes without accessing the board:

```sh
python3 tools/verify_evidence.py
```

The ITCM campaign contains 324 successful cases, 41 UART timeouts, and one
failed exploratory case; all are retained, and only successful measurements
support the selected placements. Each primary case measures key generation, encapsulation, and decapsulation
over 100 timed iterations with the Arm DWT cycle counter after one warm-up call
per operation. Results report arithmetic-mean cycles and wall-clock time from
the measured HCLK. All retained primary rows have `status=ok` and report a
matching encapsulated and decapsulated shared secret. The baseline is pqm7
`m7fspeed` with `m4_opt_m7` Keccak and all added optimization knobs disabled.

Unless stated otherwise, a positive percentage is a cycle reduction relative
to the baseline with the same parameter set and clock:

```text
100 * (baseline_cycles - profile_cycles) / baseline_cycles
```

Thus, negative values denote regressions. Data-RAM values are static linker
allocations, not peak-stack measurements. Auxiliary-state values identify the
caller-managed public or public-key-derived cache and do not change the
standard ML-KEM wire formats.

### Baseline

Times are milliseconds in parentheses; RAM and standardized public-key (PK)
sizes are bytes. These rows are in the single-knob
[`summary_by_category.csv`](benchmark_results/paper_matrix_single_knob_sequence_2026-06-23_19-11-45/summary_by_category.csv).

| Set | MHz | KeyGen | Encaps | Decaps | Data RAM | PK |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| ML-KEM-512 | 24 | 229021 (9.54) | 229131 (9.55) | 246613 (10.28) | 7320 | 800 |
| ML-KEM-512 | 216 | 232267 (1.08) | 231997 (1.07) | 250388 (1.16) | 7320 | 800 |
| ML-KEM-768 | 24 | 373931 (15.58) | 382991 (15.96) | 407582 (16.98) | 8792 | 1184 |
| ML-KEM-768 | 216 | 378284 (1.75) | 387205 (1.79) | 412828 (1.91) | 8792 | 1184 |
| ML-KEM-1024 | 24 | 590538 (24.61) | 597755 (24.91) | 631860 (26.33) | 10424 | 1568 |
| ML-KEM-1024 | 216 | 595065 (2.76) | 601529 (2.79) | 637113 (2.95) | 10424 | 1568 |

Cycle counts rise slightly at the higher endpoint, so high-frequency latency
cannot be obtained reliably by scaling only the 24 MHz cycle counts.

### Individual optimization directions

The ranges below cover all parameter sets and both endpoint clocks. Positive
values are cycle reductions; RAM and auxiliary-state deltas are bytes.

| Optimization | KeyGen (%) | Encaps (%) | Decaps (%) | Data RAM delta | Aux. state delta |
| --- | ---: | ---: | ---: | ---: | ---: |
| Hardware TRNG | 0.17 to 1.72 | 0.06 to 0.85 | -0.32 to 0.13 | -112 | 0 |
| Flash ART/prefetch | -0.06 to -0.00 | -0.06 to 0.05 | -0.07 to 0.00 | 0 | 0 |
| Endpoint-selected ITCM | 0.18 to 1.64 | 0.15 to 1.71 | 0.27 to 1.72 | 0 | 0 |
| DTCM NTT constants | -0.07 to 0.00 | -0.05 to 0.05 | -0.06 to -0.01 | +1792 | 0 |
| Cached A | -5.23 to -2.48 | 41.04 to 61.69 | 37.76 to 58.41 | +1536 to +6144 | +1536 to +6144 |
| H(pk) cache | -0.09 to 0.09 | 12.49 to 16.35 | -0.11 to 0.33 | +32 | +32 |
| Parsed PK cache | -2.24 to -1.67 | 0.70 to 1.30 | 0.58 to 0.87 | +2048 to +4096 | +2048 to +4096 |

For the paper's representative ML-KEM-768, 216 MHz memory/performance
comparison, the exact single-knob values are:

| Optimization | KeyGen (%) | Encaps (%) | Decaps (%) | Data RAM delta | Aux. state delta |
| --- | ---: | ---: | ---: | ---: | ---: |
| Cached A | -4.11 | 54.19 | 50.98 | +3456 | +3456 |
| Hardware TRNG | 0.29 | 0.06 | -0.31 | -112 | 0 |
| Flash ART/prefetch | -0.02 | -0.02 | -0.05 | 0 | 0 |
| Endpoint-selected ITCM | 1.35 | 1.29 | 1.42 | 0 | 0 |
| DTCM NTT constants | -0.01 | -0.02 | -0.04 | +1792 | 0 |
| H(pk) cache | 0.02 | 14.40 | -0.03 | +32 | +32 |
| Parsed PK cache | -2.05 | 0.73 | 0.58 | +3072 | +3072 |

Cached A supplies the largest isolated encapsulation and decapsulation gains.
The H(pk) cache gives a smaller but favorable encapsulation trade-off for 32
bytes. Parsed-PK caching is comparatively expensive for its measured benefit;
DTCM NTT placement consumes 1792 bytes without a corresponding cycle gain.

### Endpoint ITCM selection

ITCM placement is path- and profile-dependent. The abbreviations below are
`K` (Keccak), `NTT` (forward/inverse NTT), `MA` (matrix-row accumulation),
`DP` (polynomial-vector dot products), and `CMA` (matrix accumulation while
capturing A). The full candidate measurements are in
[`a_path_results.csv`](benchmark_results/itcm_selection_2026-06-21/a_path_results.csv)
and
[`composite_results.csv`](benchmark_results/itcm_selection_2026-06-21/composite_results.csv).

| Set | MHz | Isolated regen-A | NC | NC-noD | AC | AC-noPK | AE-noD-noPK | AE |
| --- | ---: | --- | --- | --- | --- | --- | --- | --- |
| ML-KEM-512 | 24 | K+NTT | K+NTT | K+MA+DP | K+CMA | K+CMA | K+CMA | K+CMA |
| ML-KEM-512 | 216 | K+NTT | K+NTT | K+NTT | K+CMA | K+NTT+CMA | K+CMA | K+CMA |
| ML-KEM-768 | 24 | K+MA+DP | K+MA+DP | K+MA+DP | K+CMA | K+CMA | K+CMA | K+CMA |
| ML-KEM-768 | 216 | K+MA+DP | K+MA+DP | K+MA+DP | K+CMA | K+NTT+CMA | K+CMA | K+NTT+CMA |
| ML-KEM-1024 | 24 | K+MA | K+NTT | K+MA | K+CMA | K+CMA | K+CMA | K+CMA |
| ML-KEM-1024 | 216 | K+MA | K+MA | K+MA | K+CMA | K+CMA | K+CMA | K+CMA |

For the 16-clock composite sweep, 24 through 120 MHz use the 24 MHz
selection, and 144 through 216 MHz use the 216 MHz selection. Interior
placements are therefore an endpoint approximation, not a separate placement
search at every clock.

### Combined optimization profiles

Here caching means caller-managed public-data reuse, not the Cortex-M7 caches.
`AC` enables cached A, cached H(pk), and parsed-PK state; `AC-noPK` omits the
parsed representation. `NC` combines hardware TRNG, Flash ART/prefetch, DTCM
NTT constants, and profile-specific ITCM; `NC-noD` omits DTCM constants. `AE`
combines AC and NC, while `AE-noD-noPK` combines cached A and H(pk) with
NC-noD.

The following ranges cover all parameter sets and both endpoint clocks. Their
source is the composite-sweep
[`summary_by_category.csv`](benchmark_results/paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45/summary_by_category.csv).

| Profile | KeyGen (%) | Encaps (%) | Decaps (%) | Data RAM delta | Aux. state delta |
| --- | ---: | ---: | ---: | ---: | ---: |
| NC | 0.65 to 2.46 | 0.25 to 2.26 | 0.05 to 2.23 | +1680 | 0 |
| NC-noD | 0.74 to 2.25 | 0.41 to 2.09 | 0.18 to 1.97 | -112 | 0 |
| AC | -6.27 to -3.71 | 58.20 to 74.80 | 38.98 to 59.40 | +3616 to +10272 | +3616 to +10272 |
| AC-noPK | -4.77 to -1.80 | 57.54 to 74.31 | 38.32 to 58.88 | +1568 to +6176 | +1568 to +6176 |
| AE-noD-noPK | -4.22 to -1.08 | 58.58 to 74.60 | 38.48 to 58.78 | +1456 to +6064 | +1568 to +6176 |
| AE | -5.66 to -3.04 | 58.96 to 75.12 | 39.04 to 59.37 | +5296 to +11952 | +3616 to +10272 |

Profiles without public-data reuse reduce cycles by at most about 2.5% and add
no auxiliary public state. Public-data-reuse profiles provide much larger
encapsulation and decapsulation gains, with key-generation and storage costs.

### Selected balanced profile

`AE-noD-noPK` retains cached A, H(pk), hardware TRNG, Flash configuration, and
profile-specific ITCM while omitting DTCM constants and parsed-PK state. Delta
is the cycle reduction relative to the matching baseline; cycles are followed
by milliseconds in parentheses.

| Set | MHz | KeyGen delta, cycles (ms) | Encaps delta, cycles (ms) | Decaps delta, cycles (ms) | Data RAM | Aux. state |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| ML-KEM-512 | 24 | -1.08%, 231533 (9.65) | 58.58%, 94948 (3.96) | 38.48%, 151693 (6.32) | 8776 | 1568 |
| ML-KEM-512 | 216 | -1.09%, 234723 (1.09) | 58.60%, 96046 (0.45) | 39.26%, 152113 (0.70) | 8776 | 1568 |
| ML-KEM-768 | 24 | -2.82%, 384507 (16.02) | 69.88%, 115392 (4.81) | 51.72%, 196812 (8.20) | 12168 | 3488 |
| ML-KEM-768 | 216 | -2.48%, 387648 (1.80) | 69.79%, 116972 (0.54) | 52.18%, 197437 (0.91) | 12168 | 3488 |
| ML-KEM-1024 | 24 | -4.22%, 615311 (25.64) | 74.60%, 151781 (6.32) | 58.69%, 260949 (10.87) | 16488 | 6176 |
| ML-KEM-1024 | 216 | -3.96%, 618624 (2.86) | 74.33%, 154431 (0.72) | 58.78%, 262601 (1.22) | 16488 | 6176 |

For every parameter set and endpoint, the added key-generation cycles are
smaller than those saved by one subsequent encapsulation or decapsulation,
excluding external distribution and persistent-storage costs. At 216 MHz,
encapsulation is below 1 ms for all three parameter sets.

### Clock scaling

The complete 16-clock data for all parameter sets and seven configurations are
in [`results.csv`](benchmark_results/paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45/results.csv)
and [`results.xlsx`](benchmark_results/paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45/results.xlsx).
Representative ML-KEM-768 `AE-noD-noPK` values are:

| MHz | KeyGen cycles (ms) | Encaps cycles (ms) | Decaps cycles (ms) |
| ---: | ---: | ---: | ---: |
| 24 | 384507 (16.02) | 115392 (4.81) | 196812 (8.20) |
| 32 | 384610 (12.02) | 115518 (3.61) | 197097 (6.16) |
| 48 | 384656 (8.01) | 115561 (2.41) | 197031 (4.11) |
| 60 | 384981 (6.42) | 115658 (1.93) | 197061 (3.28) |
| 72 | 385529 (5.36) | 115789 (1.61) | 196979 (2.74) |
| 96 | 385815 (4.02) | 116008 (1.21) | 197097 (2.05) |
| 120 | 386008 (3.22) | 116125 (0.97) | 197089 (1.64) |
| 144 | 386093 (2.68) | 116313 (0.81) | 197161 (1.37) |
| 168 | 386824 (2.30) | 116541 (0.69) | 197271 (1.17) |
| 216 | 387648 (1.80) | 116972 (0.54) | 197437 (0.91) |

Cycles increase slightly with frequency, consistent with memory-system effects,
but wall-clock latency falls by approximately a factor of nine from 24 to 216
MHz across the measured parameter sets and operations.

### Dispersion and repeatability

Eleven targeted configurations cover ML-KEM-768 baseline, Flash, DTCM, and
ITCM cases at both endpoints; ML-KEM-512 and ML-KEM-1024 baselines at 216 MHz;
and one ML-KEM-768 hardware-TRNG case. This is a targeted check, not an
uncertainty estimate for every primary-matrix row.

Across 20 runs per configuration, the within-run standard deviation over 100
timed iterations was 0.29-1.01% of the run mean for fixed-seed configurations
and 0.31-0.67% for the TRNG configuration. Across 100 resets with each binary
held fixed, the relative standard deviation of the resulting run means was at
most 0.006% for fixed-seed cases and 0.050-0.052% with the TRNG. The ML-KEM-768
ITCM reductions were 0.40-0.45% at 24 MHz and 1.28-1.45% at 216 MHz; every
observed ITCM run mean was below the corresponding baseline range for all
three operations at both endpoints.

The complete rows and processed statistics are in
[`results.csv`](benchmark_results/repeatability_2026-09-05/results.csv),
[`dispersion_by_configuration.csv`](benchmark_results/repeatability_2026-09-05/dispersion_by_configuration.csv),
and
[`effects_vs_baseline.csv`](benchmark_results/repeatability_2026-09-05/effects_vs_baseline.csv).
Re-run the evidence checks without accessing the board using:

```sh
python3 tools/analyze_repeatability.py
```

These results characterize one board and the stated campaigns. They do not
establish energy optimality, across-board reproducibility, constant-time
behavior, or side-channel resistance.

The primary logs predate the visible project rename and therefore contain the
historical build target `STM_ML-KEM_V2`; the current source and generated
artifacts use `STM_ML-KEM_V3`. Absolute local paths in retained primary logs
were replaced by the portable marker `<REPOSITORY_ROOT>`, and the manifest
hashes were computed after that path-only sanitization. A
`bundle://STM_ML-KEM_V3_pre_cleanup_...` value in `source_raw_log` records
historical provenance from the development archive. Verification of this
public artifact uses the repository-relative `raw_log` and `raw_sha256`
fields; the external development bundle is not required.

## Repository Layout

- `Core/`, `Drivers/`, linker script, and startup assembly: STM32 firmware.
- `third_party/`: vendored pqm4/pqm7 ML-KEM and SLOTHY source snapshots.
- `third_party/mlkem_cached_a/`: project-specific cached public-data support.
- `tools/`: target clock validation.
- `tools/benchmark_paper_matrix.py`: exact single-knob, composite clock-sweep,
  and ITCM-selection campaign definitions.
- `tools/run_repeatability_experiment.py` and `tools/analyze_repeatability.py`:
  targeted dispersion collection and analysis.
- `tools/verify_evidence.py`: offline integrity checks for all retained
  evidence sets.
- `benchmark_results/`: retained raw logs, CSV/XLSX results, ITCM-selection
  evidence, and repeatability summaries.

CubeMX metadata (`STM_ML-KEM_V3.ioc`, `.mxproject`, `.settings/`) is retained
for firmware regeneration. After CubeMX regeneration, verify the benchmark
clock override, DWT measurement setup, linker memory regions, and CMake source
selection before collecting new measurements.

## License and Provenance

Project-specific code is released under GPL-3.0; see [`LICENSE`](LICENSE).
The repository also contains third-party components under their own licenses
and notices. See [`THIRD_PARTY.md`](THIRD_PARTY.md) before redistributing or
modifying vendored code.

The pqm4/pqm7 trees are source snapshots rather than live Git submodules.
Their nested Git metadata was intentionally excluded from this publication
repository. The release commit therefore fixes the exact source snapshot used
here; upstream project links and the provenance limitation are documented in
`THIRD_PARTY.md`.
