# Consolidated Clock Sweep With Endpoint-Selected ITCM

Run folder: `benchmark_results/paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45`

Produced: `2026-06-23_19-11-45`

Command: `python3 paper/scripts/refresh_paper_benchmarks.py --run-missing`

All deltas are cycle reductions versus the basic `baseline` for the same key size and clock.

Wall-clock times in parentheses are derived from measured cycles and firmware-reported HCLK.

## Category Definitions

- `baseline`: Baseline previous-best pqm7 path: m7fspeed, m4_opt_m7 Keccak, cached_A OFF, TRNG OFF, Flash ART/prefetch OFF, ITCM OFF, DTCM constants OFF, public cache OFF.
- `all_caching`: Caching composite with cached_A, public_hpk, and public_pk enabled, plus the endpoint-selected captured-A ITCM overlay for this key size and clock. TRNG, Flash ART/prefetch, and DTCM constants are OFF.
- `all_non_caching`: Non-caching composite with TRNG, Flash ART/prefetch, and DTCM NTT constants enabled, plus the endpoint-selected regenerated-A ITCM overlay for this key size and clock. cached_A, public_hpk, and public_pk are OFF.
- `all_caching_no_public_pk`: Caching composite without parsed public-key cache: cached_A and public_hpk are enabled, public_pk is OFF, plus the endpoint-selected captured-A ITCM overlay for this key size and clock. TRNG, Flash ART/prefetch, and DTCM constants are OFF.
- `all_non_caching_no_dtcm`: Non-caching composite without DTCM constants: TRNG and Flash ART/prefetch are enabled, DTCM constants are OFF, plus the endpoint-selected regenerated-A ITCM overlay for this key size and clock. cached_A, public_hpk, and public_pk are OFF.
- `all_enabled_no_dtcm_no_public_pk`: Combined project optimizations except DTCM constants and parsed public-key cache: cached_A, public_hpk, TRNG, and Flash ART/prefetch are enabled, public_pk and DTCM constants are OFF, plus the endpoint-selected captured-A ITCM overlay for this key size and clock.
- `all_enabled`: Combined project optimizations with cached_A, public_hpk, public_pk, TRNG, Flash ART/prefetch, and DTCM NTT constants enabled, plus the endpoint-selected captured-A ITCM overlay for this key size and clock.

## ML-KEM-512, 24mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229048 (9.544 ms) | 0.00% | 229226 (9.551 ms) | 0.00% | 246586 (10.274 ms) | 0.00% | 704860 | 0.00% | 7320 | paper_itcm_composite_sequence_2026-06-21_17-45-49:1 |
| all_caching | reused | keccak+matacc_capture | keccak_capture | 240064 (10.003 ms) | -4.81% | 95808 (3.992 ms) | 58.20% | 150463 (6.269 ms) | 38.98% | 486335 | 31.00% | 10936 | paper_itcm_composite_sequence_2026-06-21_17-45-49:8 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 223813 (9.326 ms) | 2.29% | 225263 (9.386 ms) | 1.73% | 244466 (10.186 ms) | 0.86% | 693542 | 1.61% | 9000 | paper_itcm_composite_sequence_2026-06-21_17-45-49:3 |
| all_caching_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 235572 (9.816 ms) | -2.85% | 97330 (4.055 ms) | 57.54% | 152085 (6.337 ms) | 38.32% | 484987 | 31.19% | 8888 | paper_itcm_composite_sequence_2026-06-21_17-45-49:13 |
| all_non_caching_no_dtcm | reused | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 223896 (9.329 ms) | 2.25% | 225749 (9.406 ms) | 1.52% | 244828 (10.201 ms) | 0.71% | 694473 | 1.47% | 7208 | paper_itcm_composite_sequence_2026-06-21_17-45-49:17 |
| all_enabled_no_dtcm_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 231533 (9.647 ms) | -1.08% | 94948 (3.956 ms) | 58.58% | 151693 (6.321 ms) | 38.48% | 478174 | 32.16% | 8776 | paper_itcm_composite_sequence_2026-06-21_17-45-49:18 |
| all_enabled | reused | keccak+matacc_capture | keccak_capture | 236359 (9.848 ms) | -3.19% | 93657 (3.902 ms) | 59.14% | 150326 (6.264 ms) | 39.04% | 480342 | 31.85% | 12616 | paper_itcm_composite_sequence_2026-06-21_17-45-49:20 |

## ML-KEM-512, 30mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229123 (7.637 ms) | 0.00% | 229111 (7.637 ms) | 0.00% | 246549 (8.218 ms) | 0.00% | 704783 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:3 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240027 (8.001 ms) | -4.76% | 95874 (3.196 ms) | 58.15% | 150508 (5.017 ms) | 38.95% | 486409 | 30.98% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:9 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 223980 (7.466 ms) | 2.24% | 225533 (7.518 ms) | 1.56% | 244635 (8.155 ms) | 0.78% | 694148 | 1.51% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:8 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235584 (7.853 ms) | -2.82% | 96970 (3.232 ms) | 57.68% | 152108 (5.070 ms) | 38.31% | 484662 | 31.23% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:11 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224308 (7.477 ms) | 2.10% | 226126 (7.538 ms) | 1.30% | 245222 (8.174 ms) | 0.54% | 695656 | 1.30% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:12 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 231642 (7.721 ms) | -1.10% | 94575 (3.153 ms) | 58.72% | 151693 (5.056 ms) | 38.47% | 477910 | 32.19% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:13 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 236732 (7.891 ms) | -3.32% | 93649 (3.122 ms) | 59.13% | 150373 (5.012 ms) | 39.01% | 480754 | 31.79% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:14 |

## ML-KEM-512, 32mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229564 (7.174 ms) | 0.00% | 229385 (7.168 ms) | 0.00% | 247091 (7.722 ms) | 0.00% | 706040 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:5 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240141 (7.504 ms) | -4.61% | 95950 (2.998 ms) | 58.17% | 150621 (4.707 ms) | 39.04% | 486712 | 31.06% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:16 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 224221 (7.007 ms) | 2.33% | 226005 (7.063 ms) | 1.47% | 244877 (7.652 ms) | 0.90% | 695103 | 1.55% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:13 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235691 (7.365 ms) | -2.67% | 97108 (3.035 ms) | 57.67% | 152232 (4.757 ms) | 38.39% | 485031 | 31.30% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:18 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224233 (7.007 ms) | 2.32% | 226074 (7.065 ms) | 1.44% | 245136 (7.661 ms) | 0.79% | 695443 | 1.50% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:19 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 231842 (7.245 ms) | -0.99% | 94671 (2.958 ms) | 58.73% | 151745 (4.742 ms) | 38.59% | 478258 | 32.26% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:20 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 236420 (7.388 ms) | -2.99% | 93738 (2.929 ms) | 59.14% | 150468 (4.702 ms) | 39.10% | 480626 | 31.93% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:21 |

## ML-KEM-512, 36mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229526 (6.376 ms) | 0.00% | 229337 (6.370 ms) | 0.00% | 247124 (6.865 ms) | 0.00% | 705987 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:7 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240136 (6.670 ms) | -4.62% | 95978 (2.666 ms) | 58.15% | 150619 (4.184 ms) | 39.05% | 486733 | 31.06% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:23 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 224473 (6.235 ms) | 2.20% | 226230 (6.284 ms) | 1.35% | 245084 (6.808 ms) | 0.83% | 695787 | 1.44% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:18 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235690 (6.547 ms) | -2.69% | 97101 (2.697 ms) | 57.66% | 152241 (4.229 ms) | 38.39% | 485032 | 31.30% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:25 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224276 (6.230 ms) | 2.29% | 226059 (6.279 ms) | 1.43% | 245113 (6.809 ms) | 0.81% | 695448 | 1.49% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:26 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 232233 (6.451 ms) | -1.18% | 94675 (2.630 ms) | 58.72% | 151751 (4.215 ms) | 38.59% | 478659 | 32.20% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:27 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 236606 (6.572 ms) | -3.08% | 93755 (2.604 ms) | 59.12% | 150464 (4.180 ms) | 39.11% | 480825 | 31.89% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:28 |

## ML-KEM-512, 40mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229543 (5.739 ms) | 0.00% | 229311 (5.733 ms) | 0.00% | 247083 (6.177 ms) | 0.00% | 705937 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:9 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240138 (6.003 ms) | -4.62% | 95988 (2.400 ms) | 58.14% | 150627 (3.766 ms) | 39.04% | 486753 | 31.05% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:30 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 224112 (5.603 ms) | 2.37% | 225773 (5.644 ms) | 1.54% | 244684 (6.117 ms) | 0.97% | 694569 | 1.61% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:23 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235687 (5.892 ms) | -2.68% | 97105 (2.428 ms) | 57.65% | 152240 (3.806 ms) | 38.39% | 485032 | 31.29% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:32 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224131 (5.603 ms) | 2.36% | 225874 (5.647 ms) | 1.50% | 244909 (6.123 ms) | 0.88% | 694914 | 1.56% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:33 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 232223 (5.806 ms) | -1.17% | 94702 (2.368 ms) | 58.70% | 151757 (3.794 ms) | 38.58% | 478682 | 32.19% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:34 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 236632 (5.916 ms) | -3.09% | 93779 (2.344 ms) | 59.10% | 150465 (3.762 ms) | 39.10% | 480876 | 31.88% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:35 |

## ML-KEM-512, 48mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229492 (4.781 ms) | 0.00% | 229305 (4.777 ms) | 0.00% | 247100 (5.148 ms) | 0.00% | 705897 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:11 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240128 (5.003 ms) | -4.63% | 95983 (2.000 ms) | 58.14% | 150599 (3.137 ms) | 39.05% | 486710 | 31.05% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:37 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 224140 (4.670 ms) | 2.33% | 225813 (4.704 ms) | 1.52% | 244655 (5.097 ms) | 0.99% | 694608 | 1.60% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:28 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235681 (4.910 ms) | -2.70% | 97149 (2.024 ms) | 57.63% | 152215 (3.171 ms) | 38.40% | 485045 | 31.29% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:39 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224231 (4.671 ms) | 2.29% | 225934 (4.707 ms) | 1.47% | 244921 (5.103 ms) | 0.88% | 695086 | 1.53% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:40 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 231977 (4.833 ms) | -1.08% | 94745 (1.974 ms) | 58.68% | 151751 (3.161 ms) | 38.59% | 478473 | 32.22% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:41 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 236941 (4.936 ms) | -3.25% | 93833 (1.955 ms) | 59.08% | 150456 (3.135 ms) | 39.11% | 481230 | 31.83% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:42 |

## ML-KEM-512, 60mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229514 (3.825 ms) | 0.00% | 229329 (3.822 ms) | 0.00% | 247101 (4.118 ms) | 0.00% | 705944 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:13 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240140 (4.002 ms) | -4.63% | 95937 (1.599 ms) | 58.17% | 150606 (2.510 ms) | 39.05% | 486683 | 31.06% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:44 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 224277 (3.738 ms) | 2.28% | 225813 (3.764 ms) | 1.53% | 244607 (4.077 ms) | 1.01% | 694697 | 1.59% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:33 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235693 (3.928 ms) | -2.69% | 97107 (1.618 ms) | 57.66% | 152236 (2.537 ms) | 38.39% | 485036 | 31.29% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:46 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224399 (3.740 ms) | 2.23% | 225994 (3.767 ms) | 1.45% | 244933 (4.082 ms) | 0.88% | 695326 | 1.50% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:47 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 232080 (3.868 ms) | -1.12% | 94795 (1.580 ms) | 58.66% | 151757 (2.529 ms) | 38.59% | 478632 | 32.20% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:48 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 236702 (3.945 ms) | -3.13% | 93880 (1.565 ms) | 59.06% | 150458 (2.508 ms) | 39.11% | 481040 | 31.86% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:49 |

## ML-KEM-512, 72mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 230028 (3.195 ms) | 0.00% | 229920 (3.193 ms) | 0.00% | 247631 (3.439 ms) | 0.00% | 707579 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:15 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240254 (3.337 ms) | -4.45% | 96073 (1.334 ms) | 58.21% | 150719 (2.093 ms) | 39.14% | 487046 | 31.17% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:51 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 224700 (3.121 ms) | 2.32% | 226018 (3.139 ms) | 1.70% | 244868 (3.401 ms) | 1.12% | 695586 | 1.69% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:38 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235775 (3.275 ms) | -2.50% | 97263 (1.351 ms) | 57.70% | 152390 (2.117 ms) | 38.46% | 485428 | 31.40% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:53 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224812 (3.122 ms) | 2.27% | 226301 (3.143 ms) | 1.57% | 245195 (3.405 ms) | 0.98% | 696308 | 1.59% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:54 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 232581 (3.230 ms) | -1.11% | 94933 (1.319 ms) | 58.71% | 151816 (2.109 ms) | 38.69% | 479330 | 32.26% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:55 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 236990 (3.292 ms) | -3.03% | 94050 (1.306 ms) | 59.09% | 150553 (2.091 ms) | 39.20% | 481593 | 31.94% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:56 |

## ML-KEM-512, 84mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229989 (2.738 ms) | 0.00% | 229906 (2.737 ms) | 0.00% | 247618 (2.948 ms) | 0.00% | 707513 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:17 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240253 (2.860 ms) | -4.46% | 96094 (1.144 ms) | 58.20% | 150732 (1.794 ms) | 39.13% | 487079 | 31.16% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:58 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 224674 (2.675 ms) | 2.31% | 225912 (2.689 ms) | 1.74% | 244700 (2.913 ms) | 1.18% | 695286 | 1.73% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:43 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235777 (2.807 ms) | -2.52% | 97254 (1.158 ms) | 57.70% | 152383 (1.814 ms) | 38.46% | 485414 | 31.39% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:60 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 224753 (2.676 ms) | 2.28% | 226168 (2.692 ms) | 1.63% | 244976 (2.916 ms) | 1.07% | 695897 | 1.64% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:61 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 232948 (2.773 ms) | -1.29% | 94993 (1.131 ms) | 58.68% | 151808 (1.807 ms) | 38.69% | 479749 | 32.19% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:62 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 237303 (2.825 ms) | -3.18% | 94110 (1.120 ms) | 59.07% | 150550 (1.792 ms) | 39.20% | 481963 | 31.88% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:63 |

## ML-KEM-512, 96mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 230459 (2.401 ms) | 0.00% | 230236 (2.398 ms) | 0.00% | 248182 (2.585 ms) | 0.00% | 708877 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:19 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240375 (2.504 ms) | -4.30% | 96186 (1.002 ms) | 58.22% | 150858 (1.571 ms) | 39.21% | 487419 | 31.24% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:65 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 225095 (2.345 ms) | 2.33% | 226525 (2.360 ms) | 1.61% | 244958 (2.552 ms) | 1.30% | 696578 | 1.73% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:48 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235902 (2.457 ms) | -2.36% | 97408 (1.015 ms) | 57.69% | 152513 (1.589 ms) | 38.55% | 485823 | 31.47% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:67 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 225055 (2.344 ms) | 2.34% | 226389 (2.358 ms) | 1.67% | 245126 (2.553 ms) | 1.23% | 696570 | 1.74% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:68 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 232862 (2.426 ms) | -1.04% | 95133 (0.991 ms) | 58.68% | 151876 (1.582 ms) | 38.80% | 479871 | 32.31% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:69 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 237422 (2.473 ms) | -3.02% | 94243 (0.982 ms) | 59.07% | 150667 (1.569 ms) | 39.29% | 482332 | 31.96% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:70 |

## ML-KEM-512, 108mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 230424 (2.134 ms) | 0.00% | 230200 (2.131 ms) | 0.00% | 248200 (2.298 ms) | 0.00% | 708824 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:21 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240366 (2.226 ms) | -4.31% | 96194 (0.891 ms) | 58.21% | 150865 (1.397 ms) | 39.22% | 487425 | 31.23% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:72 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 225107 (2.084 ms) | 2.31% | 226371 (2.096 ms) | 1.66% | 244842 (2.267 ms) | 1.35% | 696320 | 1.76% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:53 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235885 (2.184 ms) | -2.37% | 97394 (0.902 ms) | 57.69% | 152522 (1.412 ms) | 38.55% | 485801 | 31.46% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:74 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 225240 (2.086 ms) | 2.25% | 226489 (2.097 ms) | 1.61% | 245185 (2.270 ms) | 1.21% | 696914 | 1.68% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:75 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 233158 (2.159 ms) | -1.19% | 95188 (0.881 ms) | 58.65% | 151877 (1.406 ms) | 38.81% | 480223 | 32.25% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:76 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 237822 (2.202 ms) | -3.21% | 94310 (0.873 ms) | 59.03% | 150661 (1.395 ms) | 39.30% | 482793 | 31.89% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:77 |

## ML-KEM-512, 120mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 230435 (1.920 ms) | 0.00% | 230166 (1.918 ms) | 0.00% | 248178 (2.068 ms) | 0.00% | 708779 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:23 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240383 (2.003 ms) | -4.32% | 96202 (0.802 ms) | 58.20% | 150859 (1.257 ms) | 39.21% | 487444 | 31.23% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:79 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 225278 (1.877 ms) | 2.24% | 226442 (1.887 ms) | 1.62% | 244852 (2.040 ms) | 1.34% | 696572 | 1.72% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:58 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 235900 (1.966 ms) | -2.37% | 97398 (0.812 ms) | 57.68% | 152509 (1.271 ms) | 38.55% | 485807 | 31.46% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:81 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 225551 (1.880 ms) | 2.12% | 226723 (1.889 ms) | 1.50% | 245351 (2.045 ms) | 1.14% | 697625 | 1.57% | 7208 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:82 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 233244 (1.944 ms) | -1.22% | 95246 (0.794 ms) | 58.62% | 151871 (1.266 ms) | 38.81% | 480361 | 32.23% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:83 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 237716 (1.981 ms) | -3.16% | 94364 (0.786 ms) | 59.00% | 150670 (1.256 ms) | 39.29% | 482750 | 31.89% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:84 |

## ML-KEM-512, 144mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 230868 (1.603 ms) | 0.00% | 230769 (1.603 ms) | 0.00% | 248755 (1.727 ms) | 0.00% | 710392 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:25 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240483 (1.670 ms) | -4.16% | 96305 (0.669 ms) | 58.27% | 150978 (1.048 ms) | 39.31% | 487766 | 31.34% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:86 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 225755 (1.568 ms) | 2.21% | 226586 (1.574 ms) | 1.81% | 245046 (1.702 ms) | 1.49% | 697387 | 1.83% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:63 |
| all_caching_no_public_pk | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 236274 (1.641 ms) | -2.34% | 97172 (0.675 ms) | 57.89% | 152803 (1.061 ms) | 38.57% | 486249 | 31.55% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:88 |
| all_non_caching_no_dtcm | reused | keccak+ntt | keccak_ntt | 225806 (1.568 ms) | 2.19% | 226555 (1.573 ms) | 1.83% | 245210 (1.703 ms) | 1.43% | 697571 | 1.80% | 7208 | paper_non_caching_no_dtcm_sequence_2026-06-11_12-51-53:26 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 233547 (1.622 ms) | -1.16% | 95437 (0.663 ms) | 58.64% | 151926 (1.055 ms) | 38.93% | 480910 | 32.30% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:90 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 238322 (1.655 ms) | -3.23% | 94585 (0.657 ms) | 59.01% | 150721 (1.047 ms) | 39.41% | 483628 | 31.92% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:91 |

## ML-KEM-512, 168mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 231391 (1.377 ms) | 0.00% | 231127 (1.376 ms) | 0.00% | 249301 (1.484 ms) | 0.00% | 711819 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:27 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240587 (1.432 ms) | -3.97% | 96434 (0.574 ms) | 58.28% | 151113 (0.899 ms) | 39.39% | 488134 | 31.42% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:93 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 226132 (1.346 ms) | 2.27% | 227064 (1.352 ms) | 1.76% | 245130 (1.459 ms) | 1.67% | 698326 | 1.90% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:68 |
| all_caching_no_public_pk | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 236317 (1.407 ms) | -2.13% | 97219 (0.579 ms) | 57.94% | 152828 (0.910 ms) | 38.70% | 486364 | 31.67% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:95 |
| all_non_caching_no_dtcm | reused | keccak+ntt | keccak_ntt | 226380 (1.348 ms) | 2.17% | 227171 (1.352 ms) | 1.71% | 245468 (1.461 ms) | 1.54% | 699019 | 1.80% | 7208 | paper_non_caching_no_dtcm_sequence_2026-06-11_12-51-53:28 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 234125 (1.394 ms) | -1.18% | 95643 (0.569 ms) | 58.62% | 152000 (0.905 ms) | 39.03% | 481768 | 32.32% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:97 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 238622 (1.420 ms) | -3.13% | 94793 (0.564 ms) | 58.99% | 150833 (0.898 ms) | 39.50% | 484248 | 31.97% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:98 |

## ML-KEM-512, 192mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 231838 (1.207 ms) | 0.00% | 231728 (1.207 ms) | 0.00% | 249863 (1.301 ms) | 0.00% | 713429 | 0.00% | 7320 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:29 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 240697 (1.254 ms) | -3.82% | 96557 (0.503 ms) | 58.33% | 151222 (0.788 ms) | 39.48% | 488476 | 31.53% | 10936 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:100 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 226534 (1.180 ms) | 2.29% | 227027 (1.182 ms) | 2.03% | 245214 (1.277 ms) | 1.86% | 698775 | 2.05% | 9000 | paper_clock_sweep_sequence_2026-06-05_19-28-12:73 |
| all_caching_no_public_pk | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 236350 (1.231 ms) | -1.95% | 97236 (0.506 ms) | 58.04% | 152882 (0.796 ms) | 38.81% | 486468 | 31.81% | 8888 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:102 |
| all_non_caching_no_dtcm | reused | keccak+ntt | keccak_ntt | 226830 (1.181 ms) | 2.16% | 227240 (1.184 ms) | 1.94% | 245637 (1.279 ms) | 1.69% | 699707 | 1.92% | 7208 | paper_non_caching_no_dtcm_sequence_2026-06-11_12-51-53:30 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 234210 (1.220 ms) | -1.02% | 95825 (0.499 ms) | 58.65% | 152060 (0.792 ms) | 39.14% | 482095 | 32.43% | 8776 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:104 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 238925 (1.244 ms) | -3.06% | 95025 (0.495 ms) | 58.99% | 150933 (0.786 ms) | 39.59% | 484883 | 32.03% | 12616 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:105 |

## ML-KEM-512, 216mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 232198 (1.075 ms) | 0.00% | 231992 (1.074 ms) | 0.00% | 250440 (1.159 ms) | 0.00% | 714630 | 0.00% | 7320 | paper_itcm_composite_sequence_2026-06-21_17-45-49:22 |
| all_caching | reused | keccak+matacc_capture | keccak_capture | 240823 (1.115 ms) | -3.71% | 96658 (0.447 ms) | 58.34% | 151371 (0.701 ms) | 39.56% | 488852 | 31.59% | 10936 | paper_itcm_composite_sequence_2026-06-21_17-45-49:29 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 226484 (1.049 ms) | 2.46% | 226757 (1.050 ms) | 2.26% | 244847 (1.134 ms) | 2.23% | 698088 | 2.31% | 9000 | paper_itcm_composite_sequence_2026-06-21_17-45-49:24 |
| all_caching_no_public_pk | reused | keccak+ntt+matacc_capture | keccak_capture_ntt | 236381 (1.094 ms) | -1.80% | 97321 (0.451 ms) | 58.05% | 152923 (0.708 ms) | 38.94% | 486625 | 31.91% | 8888 | paper_itcm_composite_sequence_2026-06-21_17-45-49:35 |
| all_non_caching_no_dtcm | reused | keccak+ntt | keccak_ntt | 227013 (1.051 ms) | 2.23% | 227134 (1.052 ms) | 2.09% | 245504 (1.137 ms) | 1.97% | 699651 | 2.10% | 7208 | paper_itcm_composite_sequence_2026-06-21_17-45-49:26 |
| all_enabled_no_dtcm_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 234723 (1.087 ms) | -1.09% | 96046 (0.445 ms) | 58.60% | 152113 (0.704 ms) | 39.26% | 482882 | 32.43% | 8776 | paper_itcm_composite_sequence_2026-06-21_17-45-49:39 |
| all_enabled | reused | keccak+matacc_capture | keccak_capture | 239259 (1.108 ms) | -3.04% | 95215 (0.441 ms) | 58.96% | 151008 (0.699 ms) | 39.70% | 485482 | 32.07% | 12616 | paper_itcm_composite_sequence_2026-06-21_17-45-49:41 |

## ML-KEM-768, 24mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 373958 (15.582 ms) | 0.00% | 383058 (15.961 ms) | 0.00% | 407616 (16.984 ms) | 0.00% | 1164632 | 0.00% | 8792 | paper_itcm_composite_sequence_2026-06-21_17-45-49:43 |
| all_caching | reused | keccak+matacc_capture | keccak_capture | 395360 (16.473 ms) | -5.72% | 115375 (4.807 ms) | 69.88% | 195411 (8.142 ms) | 52.06% | 706146 | 39.37% | 15352 | paper_itcm_composite_sequence_2026-06-21_17-45-49:50 |
| all_non_caching | reused | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368716 (15.363 ms) | 1.40% | 379592 (15.816 ms) | 0.90% | 405909 (16.913 ms) | 0.42% | 1154217 | 0.89% | 10472 | paper_itcm_composite_sequence_2026-06-21_17-45-49:54 |
| all_caching_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 388783 (16.199 ms) | -3.96% | 118119 (4.922 ms) | 69.16% | 197391 (8.225 ms) | 51.57% | 704293 | 39.53% | 12280 | paper_itcm_composite_sequence_2026-06-21_17-45-49:55 |
| all_non_caching_no_dtcm | reused | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368305 (15.346 ms) | 1.51% | 379147 (15.798 ms) | 1.02% | 405493 (16.896 ms) | 0.52% | 1152945 | 1.00% | 8680 | paper_itcm_composite_sequence_2026-06-21_17-45-49:59 |
| all_enabled_no_dtcm_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 384507 (16.021 ms) | -2.82% | 115392 (4.808 ms) | 69.88% | 196812 (8.201 ms) | 51.72% | 696711 | 40.18% | 12168 | paper_itcm_composite_sequence_2026-06-21_17-45-49:60 |
| all_enabled | reused | keccak+matacc_capture | keccak_capture | 392193 (16.341 ms) | -4.88% | 113669 (4.736 ms) | 70.33% | 195775 (8.157 ms) | 51.97% | 701637 | 39.75% | 17032 | paper_itcm_composite_sequence_2026-06-21_17-45-49:62 |

## ML-KEM-768, 30mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 373869 (12.462 ms) | 0.00% | 382851 (12.762 ms) | 0.00% | 407425 (13.581 ms) | 0.00% | 1164145 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:35 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395166 (13.172 ms) | -5.70% | 115360 (3.845 ms) | 69.87% | 195442 (6.515 ms) | 52.03% | 705968 | 39.36% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:121 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368871 (12.296 ms) | 1.34% | 379707 (12.657 ms) | 0.82% | 406032 (13.534 ms) | 0.34% | 1154610 | 0.82% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:122 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 388772 (12.959 ms) | -3.99% | 118137 (3.938 ms) | 69.14% | 197435 (6.581 ms) | 51.54% | 704344 | 39.50% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:123 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368535 (12.285 ms) | 1.43% | 379376 (12.646 ms) | 0.91% | 405757 (13.525 ms) | 0.41% | 1153668 | 0.90% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:124 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 384705 (12.824 ms) | -2.90% | 115422 (3.847 ms) | 69.85% | 196824 (6.561 ms) | 51.69% | 696951 | 40.13% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:125 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 392513 (13.084 ms) | -4.99% | 113658 (3.789 ms) | 70.31% | 195801 (6.527 ms) | 51.94% | 701972 | 39.70% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:126 |

## ML-KEM-768, 32mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 374457 (11.702 ms) | 0.00% | 383455 (11.983 ms) | 0.00% | 408157 (12.755 ms) | 0.00% | 1166069 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:37 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395280 (12.353 ms) | -5.56% | 115458 (3.608 ms) | 69.89% | 195558 (6.111 ms) | 52.09% | 706296 | 39.43% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:128 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368297 (11.509 ms) | 1.65% | 379132 (11.848 ms) | 1.13% | 405483 (12.671 ms) | 0.66% | 1152912 | 1.13% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:129 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 388958 (12.155 ms) | -3.87% | 118106 (3.691 ms) | 69.20% | 197616 (6.176 ms) | 51.58% | 704680 | 39.57% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:130 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368692 (11.522 ms) | 1.54% | 379477 (11.859 ms) | 1.04% | 405918 (12.685 ms) | 0.55% | 1154087 | 1.03% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:131 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 384610 (12.019 ms) | -2.71% | 115518 (3.610 ms) | 69.87% | 197097 (6.159 ms) | 51.71% | 697225 | 40.21% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:132 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 392566 (12.268 ms) | -4.84% | 113797 (3.556 ms) | 70.32% | 195940 (6.123 ms) | 51.99% | 702303 | 39.77% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:133 |

## ML-KEM-768, 36mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 374481 (10.402 ms) | 0.00% | 383448 (10.651 ms) | 0.00% | 408135 (11.337 ms) | 0.00% | 1166064 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:39 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395282 (10.980 ms) | -5.55% | 115456 (3.207 ms) | 69.89% | 195545 (5.432 ms) | 52.09% | 706283 | 39.43% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:135 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368850 (10.246 ms) | 1.50% | 379641 (10.546 ms) | 0.99% | 405983 (11.277 ms) | 0.53% | 1154474 | 0.99% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:136 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 388958 (10.804 ms) | -3.87% | 118105 (3.281 ms) | 69.20% | 197628 (5.490 ms) | 51.58% | 704691 | 39.57% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:137 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368768 (10.244 ms) | 1.53% | 379513 (10.542 ms) | 1.03% | 405937 (11.276 ms) | 0.54% | 1154218 | 1.02% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:138 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 384503 (10.681 ms) | -2.68% | 115525 (3.209 ms) | 69.87% | 197079 (5.474 ms) | 51.71% | 697107 | 40.22% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:139 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 393150 (10.921 ms) | -4.99% | 113819 (3.162 ms) | 70.32% | 195952 (5.443 ms) | 51.99% | 702921 | 39.72% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:140 |

## ML-KEM-768, 40mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 374488 (9.362 ms) | 0.00% | 383470 (9.587 ms) | 0.00% | 408135 (10.203 ms) | 0.00% | 1166093 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:41 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395282 (9.882 ms) | -5.55% | 115461 (2.887 ms) | 69.89% | 195554 (4.889 ms) | 52.09% | 706297 | 39.43% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:142 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368845 (9.221 ms) | 1.51% | 379615 (9.490 ms) | 1.01% | 405920 (10.148 ms) | 0.54% | 1154380 | 1.00% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:143 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 388961 (9.724 ms) | -3.86% | 118115 (2.953 ms) | 69.20% | 197618 (4.940 ms) | 51.58% | 704694 | 39.57% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:144 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368738 (9.218 ms) | 1.54% | 379462 (9.487 ms) | 1.05% | 405876 (10.147 ms) | 0.55% | 1154076 | 1.03% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:145 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 384967 (9.624 ms) | -2.80% | 115556 (2.889 ms) | 69.87% | 197102 (4.928 ms) | 51.71% | 697625 | 40.17% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:146 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 392543 (9.814 ms) | -4.82% | 113844 (2.846 ms) | 70.31% | 195971 (4.899 ms) | 51.98% | 702358 | 39.77% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:147 |

## ML-KEM-768, 48mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 374520 (7.803 ms) | 0.00% | 383481 (7.989 ms) | 0.00% | 408169 (8.504 ms) | 0.00% | 1166170 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:43 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395298 (8.235 ms) | -5.55% | 115480 (2.406 ms) | 69.89% | 195550 (4.074 ms) | 52.09% | 706328 | 39.43% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:149 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368980 (7.687 ms) | 1.48% | 379672 (7.910 ms) | 0.99% | 405981 (8.458 ms) | 0.54% | 1154633 | 0.99% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:150 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 388972 (8.104 ms) | -3.86% | 118156 (2.462 ms) | 69.19% | 197645 (4.118 ms) | 51.58% | 704773 | 39.57% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:151 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 368713 (7.682 ms) | 1.55% | 379488 (7.906 ms) | 1.04% | 405787 (8.454 ms) | 0.58% | 1153988 | 1.04% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:152 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 384656 (8.014 ms) | -2.71% | 115561 (2.408 ms) | 69.87% | 197031 (4.105 ms) | 51.73% | 697248 | 40.21% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:153 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 392622 (8.180 ms) | -4.83% | 113878 (2.372 ms) | 70.30% | 195996 (4.083 ms) | 51.98% | 702496 | 39.76% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:154 |

## ML-KEM-768, 60mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 374470 (6.241 ms) | 0.00% | 383444 (6.391 ms) | 0.00% | 408126 (6.802 ms) | 0.00% | 1166040 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:45 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395290 (6.588 ms) | -5.56% | 115452 (1.924 ms) | 69.89% | 195550 (3.259 ms) | 52.09% | 706292 | 39.43% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:156 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369070 (6.151 ms) | 1.44% | 379697 (6.328 ms) | 0.98% | 405897 (6.765 ms) | 0.55% | 1154664 | 0.98% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:157 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 388957 (6.483 ms) | -3.87% | 118113 (1.969 ms) | 69.20% | 197619 (3.294 ms) | 51.58% | 704689 | 39.57% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:158 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369026 (6.150 ms) | 1.45% | 379606 (6.327 ms) | 1.00% | 405900 (6.765 ms) | 0.55% | 1154532 | 0.99% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:159 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 384981 (6.416 ms) | -2.81% | 115658 (1.928 ms) | 69.84% | 197061 (3.284 ms) | 51.72% | 697700 | 40.17% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:160 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 392741 (6.546 ms) | -4.88% | 113945 (1.899 ms) | 70.28% | 195941 (3.266 ms) | 51.99% | 702627 | 39.74% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:161 |

## ML-KEM-768, 72mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 375072 (5.209 ms) | 0.00% | 384087 (5.335 ms) | 0.00% | 408896 (5.679 ms) | 0.00% | 1168055 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:47 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395380 (5.491 ms) | -5.41% | 115561 (1.605 ms) | 69.91% | 195670 (2.718 ms) | 52.15% | 706611 | 39.51% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:163 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369768 (5.136 ms) | 1.41% | 380338 (5.282 ms) | 0.98% | 406472 (5.645 ms) | 0.59% | 1156578 | 0.98% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:164 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 389127 (5.405 ms) | -3.75% | 118448 (1.645 ms) | 69.16% | 197830 (2.748 ms) | 51.62% | 705405 | 39.61% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:165 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369410 (5.131 ms) | 1.51% | 379927 (5.277 ms) | 1.08% | 406143 (5.641 ms) | 0.67% | 1155480 | 1.08% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:166 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 385529 (5.355 ms) | -2.79% | 115789 (1.608 ms) | 69.85% | 196979 (2.736 ms) | 51.83% | 698297 | 40.22% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:167 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 393068 (5.459 ms) | -4.80% | 114155 (1.585 ms) | 70.28% | 196127 (2.724 ms) | 52.03% | 703350 | 39.78% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:168 |

## ML-KEM-768, 84mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 375077 (4.465 ms) | 0.00% | 384017 (4.572 ms) | 0.00% | 408944 (4.868 ms) | 0.00% | 1168038 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:49 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395385 (4.707 ms) | -5.41% | 115557 (1.376 ms) | 69.91% | 195651 (2.329 ms) | 52.16% | 706593 | 39.51% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:170 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369844 (4.403 ms) | 1.40% | 380322 (4.528 ms) | 0.96% | 406407 (4.838 ms) | 0.62% | 1156573 | 0.98% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:171 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 389117 (4.632 ms) | -3.74% | 118428 (1.410 ms) | 69.16% | 197843 (2.355 ms) | 51.62% | 705388 | 39.61% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:172 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369439 (4.398 ms) | 1.50% | 379866 (4.522 ms) | 1.08% | 406042 (4.834 ms) | 0.71% | 1155347 | 1.09% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:173 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 385182 (4.586 ms) | -2.69% | 115849 (1.379 ms) | 69.83% | 196979 (2.345 ms) | 51.83% | 698010 | 40.24% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:174 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 393396 (4.683 ms) | -4.88% | 114225 (1.360 ms) | 70.26% | 196109 (2.335 ms) | 52.05% | 703730 | 39.75% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:175 |

## ML-KEM-768, 96mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 375744 (3.914 ms) | 0.00% | 384636 (4.007 ms) | 0.00% | 409614 (4.267 ms) | 0.00% | 1169994 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:51 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395514 (4.120 ms) | -5.26% | 115658 (1.205 ms) | 69.93% | 195808 (2.040 ms) | 52.20% | 706980 | 39.57% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:177 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369849 (3.853 ms) | 1.57% | 380223 (3.961 ms) | 1.15% | 406226 (4.232 ms) | 0.83% | 1156298 | 1.17% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:178 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 389303 (4.055 ms) | -3.61% | 118474 (1.234 ms) | 69.20% | 198032 (2.063 ms) | 51.65% | 705809 | 39.67% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:179 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369354 (3.847 ms) | 1.70% | 379679 (3.955 ms) | 1.29% | 405798 (4.227 ms) | 0.93% | 1154831 | 1.30% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:180 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 385815 (4.019 ms) | -2.68% | 116008 (1.208 ms) | 69.84% | 197097 (2.053 ms) | 51.88% | 698920 | 40.26% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:181 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 393952 (4.104 ms) | -4.85% | 114399 (1.192 ms) | 70.26% | 196267 (2.044 ms) | 52.08% | 704618 | 39.78% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:182 |

## ML-KEM-768, 108mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 375733 (3.479 ms) | 0.00% | 384634 (3.561 ms) | 0.00% | 409565 (3.792 ms) | 0.00% | 1169932 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:53 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395519 (3.662 ms) | -5.27% | 115656 (1.071 ms) | 69.93% | 195800 (1.813 ms) | 52.19% | 706975 | 39.57% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:184 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369711 (3.423 ms) | 1.60% | 380015 (3.519 ms) | 1.20% | 405965 (3.759 ms) | 0.88% | 1155691 | 1.22% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:185 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 389302 (3.605 ms) | -3.61% | 118450 (1.097 ms) | 69.20% | 198053 (1.834 ms) | 51.64% | 705805 | 39.67% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:186 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369717 (3.423 ms) | 1.60% | 379944 (3.518 ms) | 1.22% | 405994 (3.759 ms) | 0.87% | 1155655 | 1.22% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:187 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 385649 (3.571 ms) | -2.64% | 116051 (1.075 ms) | 69.83% | 197089 (1.825 ms) | 51.88% | 698789 | 40.27% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:188 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 393940 (3.648 ms) | -4.85% | 114451 (1.060 ms) | 70.24% | 196263 (1.817 ms) | 52.08% | 704654 | 39.77% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:189 |

## ML-KEM-768, 120mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 375727 (3.131 ms) | 0.00% | 384652 (3.205 ms) | 0.00% | 409633 (3.414 ms) | 0.00% | 1170012 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:55 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395516 (3.296 ms) | -5.27% | 115658 (0.964 ms) | 69.93% | 195783 (1.632 ms) | 52.21% | 706957 | 39.58% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:191 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 370047 (3.084 ms) | 1.51% | 380251 (3.169 ms) | 1.14% | 406138 (3.384 ms) | 0.85% | 1156436 | 1.16% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:192 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 389306 (3.244 ms) | -3.61% | 118497 (0.987 ms) | 69.19% | 198031 (1.650 ms) | 51.66% | 705834 | 39.67% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:193 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 369874 (3.082 ms) | 1.56% | 380014 (3.167 ms) | 1.21% | 405995 (3.383 ms) | 0.89% | 1155883 | 1.21% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:194 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 386008 (3.217 ms) | -2.74% | 116125 (0.968 ms) | 69.81% | 197089 (1.642 ms) | 51.89% | 699222 | 40.24% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:195 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 393594 (3.280 ms) | -4.76% | 114536 (0.954 ms) | 70.22% | 196284 (1.636 ms) | 52.08% | 704414 | 39.79% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:196 |

## ML-KEM-768, 144mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 376327 (2.613 ms) | 0.00% | 385318 (2.676 ms) | 0.00% | 410357 (2.850 ms) | 0.00% | 1172002 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:57 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395651 (2.748 ms) | -5.13% | 115762 (0.804 ms) | 69.96% | 195918 (1.361 ms) | 52.26% | 707331 | 39.65% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:198 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 370239 (2.571 ms) | 1.62% | 380280 (2.641 ms) | 1.31% | 406075 (2.820 ms) | 1.04% | 1156594 | 1.31% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:199 |
| all_caching_no_public_pk | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 389967 (2.708 ms) | -3.62% | 118797 (0.825 ms) | 69.17% | 198248 (1.377 ms) | 51.69% | 707012 | 39.67% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:200 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 370521 (2.573 ms) | 1.54% | 380541 (2.643 ms) | 1.24% | 406399 (2.822 ms) | 0.96% | 1157461 | 1.24% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:201 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 386093 (2.681 ms) | -2.60% | 116313 (0.808 ms) | 69.81% | 197161 (1.369 ms) | 51.95% | 699567 | 40.31% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:202 |
| all_enabled | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 394168 (2.737 ms) | -4.74% | 114454 (0.795 ms) | 70.30% | 196099 (1.362 ms) | 52.21% | 704721 | 39.87% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:203 |

## ML-KEM-768, 168mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 377011 (2.244 ms) | 0.00% | 385881 (2.297 ms) | 0.00% | 411389 (2.449 ms) | 0.00% | 1174281 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:59 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395770 (2.356 ms) | -4.98% | 115870 (0.690 ms) | 69.97% | 196049 (1.167 ms) | 52.34% | 707689 | 39.73% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:205 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 370784 (2.207 ms) | 1.65% | 380664 (2.266 ms) | 1.35% | 406322 (2.419 ms) | 1.23% | 1157770 | 1.41% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:206 |
| all_caching_no_public_pk | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 389978 (2.321 ms) | -3.44% | 118933 (0.708 ms) | 69.18% | 198310 (1.180 ms) | 51.80% | 707221 | 39.77% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:207 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 370414 (2.205 ms) | 1.75% | 380221 (2.263 ms) | 1.47% | 406035 (2.417 ms) | 1.30% | 1156670 | 1.50% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:208 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 386824 (2.303 ms) | -2.60% | 116541 (0.694 ms) | 69.80% | 197271 (1.174 ms) | 52.05% | 700636 | 40.33% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:209 |
| all_enabled | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 394604 (2.349 ms) | -4.67% | 114598 (0.682 ms) | 70.30% | 196149 (1.168 ms) | 52.32% | 705351 | 39.93% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:210 |

## ML-KEM-768, 192mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 377714 (1.967 ms) | 0.00% | 386586 (2.013 ms) | 0.00% | 411897 (2.145 ms) | 0.00% | 1176197 | 0.00% | 8792 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:61 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 395889 (2.062 ms) | -4.81% | 115983 (0.604 ms) | 70.00% | 196163 (1.022 ms) | 52.38% | 708035 | 39.80% | 15352 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:212 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 371136 (1.933 ms) | 1.74% | 380845 (1.984 ms) | 1.49% | 406407 (2.117 ms) | 1.33% | 1158388 | 1.51% | 10472 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:213 |
| all_caching_no_public_pk | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 390047 (2.031 ms) | -3.27% | 119007 (0.620 ms) | 69.22% | 198354 (1.033 ms) | 51.84% | 707408 | 39.86% | 12280 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:214 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 371148 (1.933 ms) | 1.74% | 380794 (1.983 ms) | 1.50% | 406522 (2.117 ms) | 1.30% | 1158464 | 1.51% | 8680 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:215 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 387400 (2.018 ms) | -2.56% | 116701 (0.608 ms) | 69.81% | 197337 (1.028 ms) | 52.09% | 701438 | 40.36% | 12168 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:216 |
| all_enabled | new | keccak+ntt+matacc_capture | keccak_capture_ntt | 394556 (2.055 ms) | -4.46% | 114869 (0.598 ms) | 70.29% | 196185 (1.022 ms) | 52.37% | 705610 | 40.01% | 17032 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:217 |

## ML-KEM-768, 216mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 378278 (1.751 ms) | 0.00% | 387155 (1.792 ms) | 0.00% | 412878 (1.911 ms) | 0.00% | 1178311 | 0.00% | 8792 | paper_itcm_composite_sequence_2026-06-21_17-45-49:64 |
| all_caching | reused | keccak+matacc_capture | keccak_capture | 396034 (1.833 ms) | -4.69% | 116076 (0.537 ms) | 70.02% | 196276 (0.909 ms) | 52.46% | 708386 | 39.88% | 15352 | paper_itcm_composite_sequence_2026-06-21_17-45-49:71 |
| all_non_caching | reused | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 371631 (1.721 ms) | 1.76% | 381131 (1.764 ms) | 1.56% | 406602 (1.882 ms) | 1.52% | 1159364 | 1.61% | 10472 | paper_itcm_composite_sequence_2026-06-21_17-45-49:75 |
| all_caching_no_public_pk | reused | keccak+ntt+matacc_capture | keccak_capture_ntt | 390065 (1.806 ms) | -3.12% | 119130 (0.552 ms) | 69.23% | 198407 (0.919 ms) | 51.95% | 707602 | 39.95% | 12280 | paper_itcm_composite_sequence_2026-06-21_17-45-49:77 |
| all_non_caching_no_dtcm | reused | keccak+matacc_cache+matacc_opt+frombytes+basemul | keccak_matacc_dotproduct | 371513 (1.720 ms) | 1.79% | 380942 (1.764 ms) | 1.60% | 406596 (1.882 ms) | 1.52% | 1159051 | 1.63% | 8680 | paper_itcm_composite_failed_sample_2026-06-21_18-04-42:1 |
| all_enabled_no_dtcm_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 387648 (1.795 ms) | -2.48% | 116972 (0.542 ms) | 69.79% | 197437 (0.914 ms) | 52.18% | 702057 | 40.42% | 12168 | paper_itcm_composite_sequence_2026-06-21_17-45-49:81 |
| all_enabled | reused | keccak+ntt+matacc_capture | keccak_capture_ntt | 395499 (1.831 ms) | -4.55% | 114981 (0.532 ms) | 70.30% | 196203 (0.908 ms) | 52.48% | 706683 | 40.03% | 17032 | paper_itcm_composite_sequence_2026-06-21_17-45-49:84 |

## ML-KEM-1024, 24mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 590374 (24.599 ms) | 0.00% | 597560 (24.898 ms) | 0.00% | 631679 (26.320 ms) | 0.00% | 1819613 | 0.00% | 10424 | paper_itcm_composite_sequence_2026-06-21_17-45-49:85 |
| all_caching | reused | keccak+matacc_capture | keccak_capture | 627410 (26.142 ms) | -6.27% | 150712 (6.280 ms) | 74.78% | 257816 (10.742 ms) | 59.19% | 1035938 | 43.07% | 20696 | paper_itcm_composite_sequence_2026-06-21_17-45-49:92 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 586559 (24.440 ms) | 0.65% | 596082 (24.837 ms) | 0.25% | 631382 (26.308 ms) | 0.05% | 1814023 | 0.31% | 12104 | paper_itcm_composite_sequence_2026-06-21_17-45-49:87 |
| all_caching_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 618523 (25.772 ms) | -4.77% | 153520 (6.397 ms) | 74.31% | 260738 (10.864 ms) | 58.72% | 1032781 | 43.24% | 16600 | paper_itcm_composite_sequence_2026-06-21_17-45-49:97 |
| all_non_caching_no_dtcm | reused | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 585993 (24.416 ms) | 0.74% | 595097 (24.796 ms) | 0.41% | 630545 (26.273 ms) | 0.18% | 1811635 | 0.44% | 10312 | paper_itcm_composite_sequence_2026-06-21_17-45-49:100 |
| all_enabled_no_dtcm_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 615311 (25.638 ms) | -4.22% | 151781 (6.324 ms) | 74.60% | 260949 (10.873 ms) | 58.69% | 1028041 | 43.50% | 16488 | paper_itcm_composite_sequence_2026-06-21_17-45-49:102 |
| all_enabled | reused | keccak+matacc_capture | keccak_capture | 623765 (25.990 ms) | -5.66% | 148679 (6.195 ms) | 75.12% | 257834 (10.743 ms) | 59.18% | 1030278 | 43.38% | 22376 | paper_itcm_composite_sequence_2026-06-21_17-45-49:104 |

## ML-KEM-1024, 30mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 590615 (19.687 ms) | 0.00% | 597450 (19.915 ms) | 0.00% | 631598 (21.053 ms) | 0.00% | 1819663 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:67 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627460 (20.915 ms) | -6.24% | 150745 (5.025 ms) | 74.77% | 257798 (8.593 ms) | 59.18% | 1036003 | 43.07% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:233 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 586345 (19.545 ms) | 0.72% | 595718 (19.857 ms) | 0.29% | 630959 (21.032 ms) | 0.10% | 1813022 | 0.36% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:168 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618530 (20.618 ms) | -4.73% | 153528 (5.118 ms) | 74.30% | 260683 (8.689 ms) | 58.73% | 1032741 | 43.25% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:235 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 585544 (19.518 ms) | 0.86% | 594912 (19.830 ms) | 0.42% | 630351 (21.012 ms) | 0.20% | 1810807 | 0.49% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:236 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 615166 (20.506 ms) | -4.16% | 151777 (5.059 ms) | 74.60% | 260924 (8.697 ms) | 58.69% | 1027867 | 43.51% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:237 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624206 (20.807 ms) | -5.69% | 148646 (4.955 ms) | 75.12% | 257845 (8.595 ms) | 59.18% | 1030697 | 43.36% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:238 |

## ML-KEM-1024, 32mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 591222 (18.476 ms) | 0.00% | 598018 (18.688 ms) | 0.00% | 632367 (19.761 ms) | 0.00% | 1821607 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:69 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627612 (19.613 ms) | -6.16% | 150853 (4.714 ms) | 74.77% | 257930 (8.060 ms) | 59.21% | 1036395 | 43.11% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:240 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 586351 (18.323 ms) | 0.82% | 595742 (18.617 ms) | 0.38% | 630965 (19.718 ms) | 0.22% | 1813058 | 0.47% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:173 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618700 (19.334 ms) | -4.65% | 153874 (4.809 ms) | 74.27% | 260833 (8.151 ms) | 58.75% | 1033407 | 43.27% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:242 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 585350 (18.292 ms) | 0.99% | 594711 (18.585 ms) | 0.55% | 630228 (19.695 ms) | 0.34% | 1810289 | 0.62% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:243 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 615443 (19.233 ms) | -4.10% | 152142 (4.754 ms) | 74.56% | 261174 (8.162 ms) | 58.70% | 1028759 | 43.52% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:244 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624469 (19.515 ms) | -5.62% | 148763 (4.649 ms) | 75.12% | 257970 (8.062 ms) | 59.21% | 1031202 | 43.39% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:245 |

## ML-KEM-1024, 36mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 591208 (16.422 ms) | 0.00% | 598077 (16.613 ms) | 0.00% | 632366 (17.566 ms) | 0.00% | 1821651 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:71 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627611 (17.434 ms) | -6.16% | 150856 (4.190 ms) | 74.78% | 257916 (7.164 ms) | 59.21% | 1036383 | 43.11% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:247 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 586698 (16.297 ms) | 0.76% | 596070 (16.558 ms) | 0.34% | 631266 (17.535 ms) | 0.17% | 1814034 | 0.42% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:178 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618684 (17.186 ms) | -4.65% | 153822 (4.273 ms) | 74.28% | 260839 (7.246 ms) | 58.75% | 1033345 | 43.27% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:249 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 585576 (16.266 ms) | 0.95% | 594906 (16.525 ms) | 0.53% | 630390 (17.511 ms) | 0.31% | 1810872 | 0.59% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:250 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 615234 (17.090 ms) | -4.06% | 152233 (4.229 ms) | 74.55% | 261166 (7.255 ms) | 58.70% | 1028633 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:251 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624567 (17.349 ms) | -5.64% | 148786 (4.133 ms) | 75.12% | 257976 (7.166 ms) | 59.20% | 1031329 | 43.38% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:252 |

## ML-KEM-1024, 40mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 591204 (14.780 ms) | 0.00% | 597995 (14.950 ms) | 0.00% | 632358 (15.809 ms) | 0.00% | 1821557 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:73 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627609 (15.690 ms) | -6.16% | 150854 (3.771 ms) | 74.77% | 257919 (6.448 ms) | 59.21% | 1036382 | 43.10% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:254 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 586662 (14.667 ms) | 0.77% | 596002 (14.900 ms) | 0.33% | 631188 (15.780 ms) | 0.19% | 1813852 | 0.42% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:183 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618705 (15.468 ms) | -4.65% | 153845 (3.846 ms) | 74.27% | 260827 (6.521 ms) | 58.75% | 1033377 | 43.27% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:256 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 585742 (14.644 ms) | 0.92% | 595050 (14.876 ms) | 0.49% | 630519 (15.763 ms) | 0.29% | 1811311 | 0.56% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:257 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 615259 (15.381 ms) | -4.07% | 152244 (3.806 ms) | 74.54% | 261179 (6.529 ms) | 58.70% | 1028682 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:258 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624615 (15.615 ms) | -5.65% | 148805 (3.720 ms) | 75.12% | 257971 (6.449 ms) | 59.20% | 1031391 | 43.38% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:259 |

## ML-KEM-1024, 48mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 591077 (12.314 ms) | 0.00% | 597863 (12.455 ms) | 0.00% | 632288 (13.173 ms) | 0.00% | 1821228 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:75 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627666 (13.076 ms) | -6.19% | 150865 (3.143 ms) | 74.77% | 257958 (5.374 ms) | 59.20% | 1036489 | 43.09% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:261 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 586864 (12.226 ms) | 0.71% | 596184 (12.421 ms) | 0.28% | 631347 (13.153 ms) | 0.15% | 1814395 | 0.38% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:188 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618657 (12.889 ms) | -4.67% | 153895 (3.206 ms) | 74.26% | 260835 (5.434 ms) | 58.75% | 1033387 | 43.26% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:263 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 585805 (12.204 ms) | 0.89% | 595046 (12.397 ms) | 0.47% | 630473 (13.135 ms) | 0.29% | 1811324 | 0.54% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:264 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 615165 (12.816 ms) | -4.08% | 152099 (3.169 ms) | 74.56% | 261170 (5.441 ms) | 58.69% | 1028434 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:265 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624459 (13.010 ms) | -5.65% | 148912 (3.102 ms) | 75.09% | 257998 (5.375 ms) | 59.20% | 1031369 | 43.37% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:266 |

## ML-KEM-1024, 60mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 591186 (9.853 ms) | 0.00% | 598034 (9.967 ms) | 0.00% | 632379 (10.540 ms) | 0.00% | 1821599 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:77 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627609 (10.460 ms) | -6.16% | 150839 (2.514 ms) | 74.78% | 257920 (4.299 ms) | 59.21% | 1036368 | 43.11% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:268 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 587163 (9.786 ms) | 0.68% | 596345 (9.939 ms) | 0.28% | 631436 (10.524 ms) | 0.15% | 1814944 | 0.37% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:193 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618681 (10.311 ms) | -4.65% | 153864 (2.564 ms) | 74.27% | 260834 (4.347 ms) | 58.75% | 1033379 | 43.27% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:270 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 586114 (9.769 ms) | 0.86% | 595274 (9.921 ms) | 0.46% | 630615 (10.510 ms) | 0.28% | 1812003 | 0.53% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:271 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 615984 (10.266 ms) | -4.19% | 152385 (2.540 ms) | 74.52% | 261201 (4.353 ms) | 58.70% | 1029570 | 43.48% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:272 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624768 (10.413 ms) | -5.68% | 148911 (2.482 ms) | 75.10% | 257964 (4.299 ms) | 59.21% | 1031643 | 43.37% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:273 |

## ML-KEM-1024, 72mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 591770 (8.219 ms) | 0.00% | 598553 (8.313 ms) | 0.00% | 633216 (8.795 ms) | 0.00% | 1823539 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:79 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627729 (8.718 ms) | -6.08% | 151026 (2.098 ms) | 74.77% | 258047 (3.584 ms) | 59.25% | 1036802 | 43.14% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:275 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 587218 (8.156 ms) | 0.77% | 596346 (8.283 ms) | 0.37% | 631416 (8.770 ms) | 0.28% | 1814980 | 0.47% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:198 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618897 (8.596 ms) | -4.58% | 153924 (2.138 ms) | 74.28% | 261048 (3.626 ms) | 58.77% | 1033869 | 43.30% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:277 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 586335 (8.144 ms) | 0.92% | 595407 (8.270 ms) | 0.53% | 630732 (8.760 ms) | 0.39% | 1812474 | 0.61% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:278 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 616000 (8.556 ms) | -4.09% | 152434 (2.117 ms) | 74.53% | 261409 (3.631 ms) | 58.72% | 1029843 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:279 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624903 (8.679 ms) | -5.60% | 149232 (2.073 ms) | 75.07% | 258105 (3.585 ms) | 59.24% | 1032240 | 43.39% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:280 |

## ML-KEM-1024, 84mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 591805 (7.045 ms) | 0.00% | 598587 (7.126 ms) | 0.00% | 633227 (7.538 ms) | 0.00% | 1823619 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:81 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627730 (7.473 ms) | -6.07% | 151028 (1.798 ms) | 74.77% | 258038 (3.072 ms) | 59.25% | 1036796 | 43.15% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:282 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 587409 (6.993 ms) | 0.74% | 596432 (7.100 ms) | 0.36% | 631433 (7.517 ms) | 0.28% | 1815274 | 0.46% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:203 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618866 (7.367 ms) | -4.57% | 153889 (1.832 ms) | 74.29% | 261060 (3.108 ms) | 58.77% | 1033815 | 43.31% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:284 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 586352 (6.980 ms) | 0.92% | 595334 (7.087 ms) | 0.54% | 630591 (7.507 ms) | 0.42% | 1812277 | 0.62% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:285 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 616077 (7.334 ms) | -4.10% | 152502 (1.816 ms) | 74.52% | 261389 (3.112 ms) | 58.72% | 1029968 | 43.52% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:286 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 624702 (7.437 ms) | -5.56% | 149323 (1.778 ms) | 75.05% | 258105 (3.073 ms) | 59.24% | 1032130 | 43.40% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:287 |

## ML-KEM-1024, 96mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 592580 (6.173 ms) | 0.00% | 599274 (6.242 ms) | 0.00% | 634084 (6.605 ms) | 0.00% | 1825938 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:83 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627857 (6.540 ms) | -5.95% | 151094 (1.574 ms) | 74.79% | 258159 (2.689 ms) | 59.29% | 1037110 | 43.20% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:289 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 587639 (6.121 ms) | 0.83% | 596595 (6.215 ms) | 0.45% | 631568 (6.579 ms) | 0.40% | 1815802 | 0.56% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:208 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 619146 (6.449 ms) | -4.48% | 154276 (1.607 ms) | 74.26% | 261209 (2.721 ms) | 58.81% | 1034631 | 43.34% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:291 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 586512 (6.110 ms) | 1.02% | 595414 (6.202 ms) | 0.64% | 630815 (6.571 ms) | 0.52% | 1812741 | 0.72% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:292 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 616692 (6.424 ms) | -4.07% | 153006 (1.594 ms) | 74.47% | 261607 (2.725 ms) | 58.74% | 1031305 | 43.52% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:293 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 625422 (6.515 ms) | -5.54% | 149305 (1.555 ms) | 75.09% | 258264 (2.690 ms) | 59.27% | 1032991 | 43.43% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:294 |

## ML-KEM-1024, 108mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 592614 (5.487 ms) | 0.00% | 599320 (5.549 ms) | 0.00% | 634095 (5.871 ms) | 0.00% | 1826029 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:85 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627858 (5.814 ms) | -5.95% | 151097 (1.399 ms) | 74.79% | 258171 (2.390 ms) | 59.29% | 1037126 | 43.20% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:296 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 587987 (5.444 ms) | 0.78% | 596833 (5.526 ms) | 0.41% | 631729 (5.849 ms) | 0.37% | 1816549 | 0.52% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:213 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 619150 (5.733 ms) | -4.48% | 154240 (1.428 ms) | 74.26% | 261198 (2.419 ms) | 58.81% | 1034588 | 43.34% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:298 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 586891 (5.434 ms) | 0.97% | 595709 (5.516 ms) | 0.60% | 630948 (5.842 ms) | 0.50% | 1813548 | 0.68% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:299 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 616607 (5.709 ms) | -4.05% | 153011 (1.417 ms) | 74.47% | 261630 (2.423 ms) | 58.74% | 1031248 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:300 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 625533 (5.792 ms) | -5.55% | 149366 (1.383 ms) | 75.08% | 258269 (2.391 ms) | 59.27% | 1033168 | 43.42% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:301 |

## ML-KEM-1024, 120mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 592560 (4.938 ms) | 0.00% | 599357 (4.995 ms) | 0.00% | 634024 (5.284 ms) | 0.00% | 1825941 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:87 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627846 (5.232 ms) | -5.95% | 151099 (1.259 ms) | 74.79% | 258157 (2.151 ms) | 59.28% | 1037102 | 43.20% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:303 |
| all_non_caching | reused | keccak+ntt | keccak_ntt | 587918 (4.899 ms) | 0.78% | 596683 (4.972 ms) | 0.45% | 631530 (5.263 ms) | 0.39% | 1816131 | 0.54% | 12104 | paper_clock_sweep_sequence_2026-06-05_19-28-12:218 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 619163 (5.160 ms) | -4.49% | 154222 (1.285 ms) | 74.27% | 261203 (2.177 ms) | 58.80% | 1034588 | 43.34% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:305 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 587354 (4.895 ms) | 0.88% | 596080 (4.967 ms) | 0.55% | 631315 (5.261 ms) | 0.43% | 1814749 | 0.61% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:306 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 616638 (5.139 ms) | -4.06% | 153111 (1.276 ms) | 74.45% | 261611 (2.180 ms) | 58.74% | 1031360 | 43.52% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:307 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 626010 (5.217 ms) | -5.64% | 149442 (1.245 ms) | 75.07% | 258271 (2.152 ms) | 59.26% | 1033723 | 43.39% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:308 |

## ML-KEM-1024, 144mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 593232 (4.120 ms) | 0.00% | 599808 (4.165 ms) | 0.00% | 634912 (4.409 ms) | 0.00% | 1827952 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:89 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 627963 (4.361 ms) | -5.85% | 151275 (1.051 ms) | 74.78% | 258325 (1.794 ms) | 59.31% | 1037563 | 43.24% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:310 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 587749 (4.082 ms) | 0.92% | 596189 (4.140 ms) | 0.60% | 631070 (4.382 ms) | 0.61% | 1815008 | 0.71% | 12104 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:311 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 619331 (4.301 ms) | -4.40% | 154325 (1.072 ms) | 74.27% | 261502 (1.816 ms) | 58.81% | 1035158 | 43.37% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:312 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 587309 (4.079 ms) | 1.00% | 595931 (4.138 ms) | 0.65% | 630911 (4.381 ms) | 0.63% | 1814151 | 0.75% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:313 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 617098 (4.285 ms) | -4.02% | 153208 (1.064 ms) | 74.46% | 261859 (1.818 ms) | 58.76% | 1032165 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:314 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 626825 (4.353 ms) | -5.66% | 149886 (1.041 ms) | 75.01% | 258383 (1.794 ms) | 59.30% | 1035094 | 43.37% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:315 |

## ML-KEM-1024, 168mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 593899 (3.535 ms) | 0.00% | 600584 (3.575 ms) | 0.00% | 635666 (3.784 ms) | 0.00% | 1830149 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:91 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 628079 (3.739 ms) | -5.76% | 151344 (0.901 ms) | 74.80% | 258431 (1.538 ms) | 59.34% | 1037854 | 43.29% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:317 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 588167 (3.501 ms) | 0.97% | 596444 (3.550 ms) | 0.69% | 631450 (3.759 ms) | 0.66% | 1816061 | 0.77% | 12104 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:318 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 619634 (3.688 ms) | -4.33% | 154706 (0.921 ms) | 74.24% | 261637 (1.557 ms) | 58.84% | 1035977 | 43.39% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:319 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 587689 (3.498 ms) | 1.05% | 596077 (3.548 ms) | 0.75% | 631231 (3.757 ms) | 0.70% | 1814997 | 0.83% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:320 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 617595 (3.676 ms) | -3.99% | 153760 (0.915 ms) | 74.40% | 262134 (1.560 ms) | 58.76% | 1033489 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:321 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 626796 (3.731 ms) | -5.54% | 149931 (0.892 ms) | 75.04% | 258580 (1.539 ms) | 59.32% | 1035307 | 43.43% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:322 |

## ML-KEM-1024, 192mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 594578 (3.097 ms) | 0.00% | 601071 (3.131 ms) | 0.00% | 636541 (3.315 ms) | 0.00% | 1832190 | 0.00% | 10424 | paper_enabled_no_dtcm_no_public_pk_sequence_2026-06-20_20-18-13:93 |
| all_caching | new | keccak+matacc_capture | keccak_capture | 628171 (3.272 ms) | -5.65% | 151528 (0.789 ms) | 74.79% | 258594 (1.347 ms) | 59.38% | 1038293 | 43.33% | 20696 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:324 |
| all_non_caching | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 588749 (3.066 ms) | 0.98% | 596828 (3.108 ms) | 0.71% | 631571 (3.289 ms) | 0.78% | 1817148 | 0.82% | 12104 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:325 |
| all_caching_no_public_pk | new | keccak+matacc_capture | keccak_capture | 619847 (3.228 ms) | -4.25% | 154640 (0.805 ms) | 74.27% | 262040 (1.365 ms) | 58.83% | 1036527 | 43.43% | 16600 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:326 |
| all_non_caching_no_dtcm | new | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 588245 (3.064 ms) | 1.07% | 596506 (3.107 ms) | 0.76% | 631337 (3.288 ms) | 0.82% | 1816088 | 0.88% | 10312 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:327 |
| all_enabled_no_dtcm_no_public_pk | new | keccak+matacc_capture | keccak_capture | 618365 (3.221 ms) | -4.00% | 153863 (0.801 ms) | 74.40% | 262336 (1.366 ms) | 58.79% | 1034564 | 43.53% | 16488 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:328 |
| all_enabled | new | keccak+matacc_capture | keccak_capture | 627027 (3.266 ms) | -5.46% | 150277 (0.783 ms) | 75.00% | 258697 (1.347 ms) | 59.36% | 1036001 | 43.46% | 22376 | paper_clock_sweep_composite_itcm_opt_sequence_2026-06-23_19-11-45:329 |

## ML-KEM-1024, 216mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 595056 (2.755 ms) | 0.00% | 601497 (2.785 ms) | 0.00% | 637144 (2.950 ms) | 0.00% | 1833697 | 0.00% | 10424 | paper_itcm_composite_sequence_2026-06-21_17-45-49:106 |
| all_caching | reused | keccak+matacc_capture | keccak_capture | 628234 (2.908 ms) | -5.58% | 151585 (0.702 ms) | 74.80% | 258686 (1.198 ms) | 59.40% | 1038505 | 43.37% | 20696 | paper_itcm_composite_sequence_2026-06-21_17-45-49:113 |
| all_non_caching | reused | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 589425 (2.729 ms) | 0.95% | 597370 (2.766 ms) | 0.69% | 632226 (2.927 ms) | 0.77% | 1819021 | 0.80% | 12104 | paper_itcm_composite_sequence_2026-06-21_17-45-49:116 |
| all_caching_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 620073 (2.871 ms) | -4.20% | 155077 (0.718 ms) | 74.22% | 262020 (1.213 ms) | 58.88% | 1037170 | 43.44% | 16600 | paper_itcm_composite_sequence_2026-06-21_17-45-49:118 |
| all_non_caching_no_dtcm | reused | keccak+matacc_cache+matacc_opt | keccak_matacc_pair | 588714 (2.726 ms) | 1.07% | 596784 (2.763 ms) | 0.78% | 631797 (2.925 ms) | 0.84% | 1817295 | 0.89% | 10312 | paper_itcm_composite_sequence_2026-06-21_17-45-49:121 |
| all_enabled_no_dtcm_no_public_pk | reused | keccak+matacc_capture | keccak_capture | 618624 (2.864 ms) | -3.96% | 154431 (0.715 ms) | 74.33% | 262601 (1.216 ms) | 58.78% | 1035656 | 43.52% | 16488 | paper_itcm_composite_sequence_2026-06-21_17-45-49:123 |
| all_enabled | reused | keccak+matacc_capture | keccak_capture | 627372 (2.905 ms) | -5.43% | 150387 (0.696 ms) | 75.00% | 258862 (1.198 ms) | 59.37% | 1036621 | 43.47% | 22376 | paper_itcm_composite_sequence_2026-06-21_17-45-49:125 |
