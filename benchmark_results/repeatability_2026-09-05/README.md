# Measurement Repeatability Evidence

Experiment dated 2026-09-05; interpretation audited against
[results.csv](results.csv), the firmware, the analysis script, and the 22
representative logs in `raw/`. The audit changes analysis and reporting only,
not firmware or measurements.

## Design and Counts

Every CSV row summarizes 100 timed iterations **per operation**. A run is a
software reset, boot, and benchmark, not a power cycle.

| Tier | Configurations | Runs per configuration | Rows | Purpose |
|---|---:|---:|---:|---|
| A | 11 | 100 | 1100 | Repeatability of run means; one fixed, uninstrumented binary per configuration |
| B | 2 | 20 | 40 | Clean rebuild, flash, and reset check |
| C | 11 | 20 | 220 | Firmware-reported within-run SD, denominator n |
| C_n1 | 11 | 20 | 220 | Superseded SD experiment, denominator n-1; preserved, not pooled with C |

Total: **1580 rows**, all status `ok` and shared-secret match 1.
A+B+C contribute 1360 current rows; C_n1 is the additional historical 220.
The 20 C runs supply 20 separate within-run SDs, not one SD over 2000 samples.

All configurations use pqm7, m7fspeed, m4_opt_m7, sequence measurements,
and no public-data caches. This is a targeted subset, not a random or
exhaustive sample of the paper's 384 configurations:

| Configurations | Count | Coverage rationale |
|---|---:|---|
| ML-KEM-768 baseline, Flash ART/prefetch, DTCM NTT constants, and ITCM at both 24 and 216 MHz | 8 | Compare small placement effects against matched baselines at both clock endpoints, using the middle parameter set |
| ML-KEM-512 and ML-KEM-1024 baselines at 216 MHz | 2 | Check baseline dispersion at the smaller and larger module ranks; not their optimized placements |
| ML-KEM-768 baseline with hardware TRNG at 216 MHz | 1 | Check fresh-input variability separately from fixed-seed replay |

The ITCM profile is `keccak+matacc_cache+matacc_opt+frombytes+basemul`.
Only the TRNG configuration uses fresh hardware randomness. The fixed-seed
software generator restarts on each boot, replaying the same sequence of
inputs. This controls input selection; it does not prove that all
input-dependent timing differences cancel between configurations.
Tier B covers only the 768/216 baseline and ITCM configurations.

The subset supports the descriptive repeatability check in manuscript
Section 5.5; it does not assign uncertainty to every table entry. Additional
repeats of these same binaries would not cover the missing cached paths,
composite profiles, other parameter sets' ITCM selections, or intermediate
clocks. No further experiment is required for the narrowly stated check.
For broader claims, prioritize the parsed-key cache, NC-noD, and selected
cached profile, plus the 512/1024 ITCM selections. Such a follow-up needs
contemporaneous baselines, interleaved comparison runs, retained per-iteration
samples and complete per-run logs, and source/binary provenance. It has not
been run; no existing precision figure is assigned to those profiles.

## Statistics and Results

For each C row, firmware calculates
`SD = sqrt(sum((x - mean(x))^2) / 100)`
from that run's own 100 cycle samples. It truncates SD to integer cycles and
reports `100 * SD / run_mean` to four decimal places. No outer-run term
enters this calculation.

For A and B, the analyzer computes sample SD (denominator R-1) of the R
reported run means, divided by their arithmetic average. This is a different
quantity, not within-run SD divided by ten.

| Quantity | Fixed seed | Hardware TRNG |
|---|---:|---:|
| C: within-run relative SD, range over runs/configurations/operations | 0.2897-1.0076% | 0.3088-0.6661% |
| A: relative SD of run means | maximum 0.006034% | 0.050130-0.051525% |
| B: relative SD of run means | maximum 0.003087% | not measured |

B's recorded ELF MD5 is constant within each configuration and agrees with A.
This checks those rebuilds, not all builds or toolchains. Tier C uses different
binaries: its maximum fixed-seed SD of run means is 0.012953%, so A's bound
must not be assigned to C.

The ITCM comparison below uses **arithmetic averages of the 100 run means**,
consistent with the manuscript's metric, rather than the previous analyzer's
median of run means. Positive savings mean fewer cycles.

| Clock | KeyGen savings | Encaps savings | Decaps savings | Smallest separation of baseline/ITCM run-mean ranges |
|---|---:|---:|---:|---:|
| 24 MHz | 0.440354% | 0.445468% | 0.396396% | 1583 cycles |
| 216 MHz | 1.355455% | 1.277613% | 1.445196% | 4912 cycles |

All six baseline/ITCM comparisons have non-overlapping observed ranges.
Rounding the arithmetic-mean effects gives 0.40-0.45% and 1.28-1.45%.
These supplementary measurements do not replace the manuscript's original
benchmark tables.

## Interpretation and Evidence Limits

- Keep both statistics: C describes individual-iteration dispersion; A
  directly tests stability of the 100-iteration averages used in comparisons.
  The earlier 0.03-0.10% was a square-root-of-100 standard-error calculation,
  not a measured accuracy bound. A standard error is not itself a confidence
  interval; its interpretation requires sampling assumptions.
  See [NIST's mean-interval definition](https://www.itl.nist.gov/div898/handbook/eda/section3/eda352.htm).
- The ITCM separations support repeatability for these tested configurations.
  They do not establish "70 sigma" significance or independently validate
  every sub-percent result, cached profile, parameter set, or clock.
  Configurations were consecutive batches, not randomized/interleaved pairs.
  One board and campaign do not establish across-board or across-day
  reproducibility. See [NIST on short- and longer-term variability](https://www.itl.nist.gov/div898/handbook/mpc/section1/mpc114.htm).
- Within-run variation can include input-dependent work and execution effects;
  these summaries do not isolate their contributions. Post-loop statistics
  avoid timing the SD calculation but can change the instrumented binary's
  layout. Differences between A and C do not isolate layout as the cause or
  prove that C's SD transfers unchanged to A.
- Individual cycle samples are not retained. Firmware's SD formula and printed
  relative values can be checked, but SD cannot be recomputed independently
  from the saved min/mean/median/max. Successful repetitions have parsed CSV
  summaries, not individual raw UART logs. The 22 representative A/C logs are
  extra resets after their batches, not one-to-one evidence for the 1580 rows.
  No B or C_n1 raw logs are present. Per-repeat ELF digests identify reported
  builds; they are not retained binaries or a complete source-commit record.

## Reproduce the Analysis

From the repository root; this command does **not** run the board:

```sh
python3 tools/analyze_repeatability.py
```

The generated outputs are written to this directory:

- `dispersion_by_configuration.csv`: 105 rows, one per tier/configuration/
  operation, with metadata, separate SD definitions, and source CSV/script
  SHA-256 digests. C_n1 rows are explicitly marked superseded.
- `effects_vs_baseline.csv`: 18 Tier-A operation comparisons for
  Flash, DTCM, and ITCM, with the effect statistic and observed range separation.

The source CSV SHA-256 is
`a27d55433daf2db8a686d81a0ce3b904174f564fd05c65008d154a459991e7cc`.
Analysis rejects unsuccessful rows, missing/duplicate repeats, mixed metadata,
and inconsistent firmware-relative SD fields rather than silently dropping data.
New hardware campaigns must use a new output directory, not this evidence set.
