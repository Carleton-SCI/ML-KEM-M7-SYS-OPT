# Strict Single-Knob Paper Benchmark Matrix

Run folder: `benchmark_results/paper_matrix_single_knob_sequence_2026-06-23_19-11-45`

Produced: `2026-06-23_19-11-45`

Command: `python3 -`

All deltas are cycle reductions versus the basic `baseline` for the same key size and clock.

Wall-clock times in parentheses are derived from measured cycles and firmware-reported HCLK.

## Category Definitions

- `baseline`: Baseline previous-best pqm7 path: m7fspeed, m4_opt_m7 Keccak, cached_A OFF, TRNG OFF, Flash ART/prefetch OFF, ITCM OFF, DTCM constants OFF, public cache OFF.
- `single_cached_a`: Only cached public matrix A is enabled.
- `single_trng`: Only STM32 hardware TRNG is enabled.
- `single_flash_art_prefetch`: Only Flash ART/prefetch acceleration is enabled.
- `single_itcm_opt`: Endpoint-selected best regenerated-A ITCM placement only. ML-KEM-512 uses keccak+ntt; ML-KEM-768 uses keccak+matacc_cache+matacc_opt+frombytes+basemul; ML-KEM-1024 uses keccak+matacc_cache+matacc_opt.
- `single_dtcm_ntt`: Only NTT public constants placement in DTCM/RAM is enabled.
- `single_public_hpk`: Only cached public H(pk) is enabled.
- `single_public_pk`: Only parsed public-key cache is enabled.

## ML-KEM-512, 24mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 229021 (9.543 ms) | 0.00% | 229131 (9.547 ms) | 0.00% | 246613 (10.276 ms) | 0.00% | 704765 | 0.00% | 7320 | paper_matrix_sequence_2026-06-05_19-15-58:1 |
| single_cached_a | reused | off |  | 235812 (9.826 ms) | -2.97% | 135053 (5.627 ms) | 41.06% | 153267 (6.386 ms) | 37.85% | 524132 | 25.63% | 8856 | paper_matrix_sequence_2026-06-05_19-15-58:2 |
| single_trng | reused | off |  | 225091 (9.379 ms) | 1.72% | 227175 (9.466 ms) | 0.85% | 246698 (10.279 ms) | -0.03% | 698964 | 0.82% | 7208 | paper_matrix_sequence_2026-06-05_19-15-58:3 |
| single_flash_art_prefetch | reused | off |  | 229091 (9.545 ms) | -0.03% | 229148 (9.548 ms) | -0.01% | 246716 (10.280 ms) | -0.04% | 704955 | -0.03% | 7320 | paper_matrix_sequence_2026-06-05_19-15-58:4 |
| single_itcm_opt | reused | keccak+ntt |  | 227958 (9.498 ms) | 0.46% | 227304 (9.471 ms) | 0.80% | 245137 (10.214 ms) | 0.60% | 700399 | 0.62% | 7320 | paper_itcm_a_path_sequence_2026-06-21_13-56-08:26 |
| single_dtcm_ntt | reused | off |  | 229076 (9.545 ms) | -0.02% | 229145 (9.548 ms) | -0.01% | 246705 (10.279 ms) | -0.04% | 704926 | -0.02% | 9112 | paper_matrix_sequence_2026-06-05_19-15-58:7 |
| single_public_hpk | reused | off |  | 229224 (9.551 ms) | -0.09% | 191666 (7.986 ms) | 16.35% | 246866 (10.286 ms) | -0.10% | 667756 | 5.25% | 7352 | paper_matrix_sequence_2026-06-05_19-15-58:8 |
| single_public_pk | reused | off |  | 234121 (9.755 ms) | -2.23% | 226155 (9.423 ms) | 1.30% | 244605 (10.192 ms) | 0.81% | 704881 | -0.02% | 9368 | paper_matrix_sequence_2026-06-05_19-15-58:79 |

## ML-KEM-512, 216mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 232267 (1.075 ms) | 0.00% | 231997 (1.074 ms) | 0.00% | 250388 (1.159 ms) | 0.00% | 714652 | 0.00% | 7320 | paper_matrix_sequence_2026-06-05_19-15-58:14 |
| single_cached_a | reused | off |  | 238026 (1.102 ms) | -2.48% | 136789 (0.633 ms) | 41.04% | 155844 (0.722 ms) | 37.76% | 530659 | 25.75% | 8856 | paper_matrix_sequence_2026-06-05_19-15-58:15 |
| single_trng | reused | off |  | 230101 (1.065 ms) | 0.93% | 230402 (1.067 ms) | 0.69% | 250114 (1.158 ms) | 0.11% | 710617 | 0.56% | 7208 | paper_matrix_sequence_2026-06-05_19-15-58:16 |
| single_flash_art_prefetch | reused | off |  | 232319 (1.076 ms) | -0.02% | 232031 (1.074 ms) | -0.01% | 250558 (1.160 ms) | -0.07% | 714908 | -0.04% | 7320 | paper_matrix_sequence_2026-06-05_19-15-58:17 |
| single_itcm_opt | reused | keccak+ntt |  | 228458 (1.058 ms) | 1.64% | 228025 (1.056 ms) | 1.71% | 246074 (1.139 ms) | 1.72% | 702557 | 1.69% | 7320 | paper_itcm_a_path_sequence_2026-06-21_13-56-08:66 |
| single_dtcm_ntt | reused | off |  | 232264 (1.075 ms) | 0.00% | 232043 (1.074 ms) | -0.02% | 250540 (1.160 ms) | -0.06% | 714847 | -0.03% | 9112 | paper_matrix_sequence_2026-06-05_19-15-58:20 |
| single_public_hpk | reused | off |  | 232187 (1.075 ms) | 0.03% | 194581 (0.901 ms) | 16.13% | 250411 (1.159 ms) | -0.01% | 677179 | 5.24% | 7352 | paper_matrix_sequence_2026-06-05_19-15-58:21 |
| single_public_pk | reused | off |  | 237463 (1.099 ms) | -2.24% | 229161 (1.061 ms) | 1.22% | 248858 (1.152 ms) | 0.61% | 715482 | -0.12% | 9368 | paper_matrix_sequence_2026-06-05_19-15-58:80 |

## ML-KEM-768, 24mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 373931 (15.580 ms) | 0.00% | 382991 (15.958 ms) | 0.00% | 407582 (16.983 ms) | 0.00% | 1164504 | 0.00% | 8792 | paper_matrix_sequence_2026-06-05_19-15-58:27 |
| single_cached_a | reused | off |  | 389809 (16.242 ms) | -4.25% | 174840 (7.285 ms) | 54.35% | 199136 (8.297 ms) | 51.14% | 763785 | 34.41% | 12248 | paper_matrix_sequence_2026-06-05_19-15-58:28 |
| single_trng | reused | off |  | 370779 (15.449 ms) | 0.84% | 381911 (15.913 ms) | 0.28% | 408873 (17.036 ms) | -0.32% | 1161563 | 0.25% | 8680 | paper_matrix_sequence_2026-06-05_19-15-58:29 |
| single_flash_art_prefetch | reused | off |  | 374052 (15.586 ms) | -0.03% | 383202 (15.967 ms) | -0.06% | 407732 (16.989 ms) | -0.04% | 1164986 | -0.04% | 8792 | paper_matrix_sequence_2026-06-05_19-15-58:30 |
| single_itcm_opt | reused | keccak+matacc_cache+matacc_opt+frombytes+basemul |  | 372344 (15.514 ms) | 0.42% | 381329 (15.889 ms) | 0.43% | 406016 (16.917 ms) | 0.38% | 1159689 | 0.41% | 8792 | paper_itcm_a_path_sequence_2026-06-21_13-56-08:109 |
| single_dtcm_ntt | reused | off |  | 374074 (15.586 ms) | -0.04% | 383190 (15.966 ms) | -0.05% | 407731 (16.989 ms) | -0.04% | 1164995 | -0.04% | 10584 | paper_matrix_sequence_2026-06-05_19-15-58:33 |
| single_public_hpk | reused | off |  | 374031 (15.585 ms) | -0.03% | 327678 (13.653 ms) | 14.44% | 408045 (17.002 ms) | -0.11% | 1109754 | 4.70% | 8824 | paper_matrix_sequence_2026-06-05_19-15-58:34 |
| single_public_pk | reused | off |  | 381578 (15.899 ms) | -2.05% | 380304 (15.846 ms) | 0.70% | 405011 (16.875 ms) | 0.63% | 1166893 | -0.21% | 11864 | paper_matrix_sequence_2026-06-05_19-15-58:81 |

## ML-KEM-768, 216mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 378284 (1.751 ms) | 0.00% | 387205 (1.793 ms) | 0.00% | 412828 (1.911 ms) | 0.00% | 1178317 | 0.00% | 8792 | paper_matrix_sequence_2026-06-05_19-15-58:40 |
| single_cached_a | reused | off |  | 393822 (1.823 ms) | -4.11% | 177363 (0.821 ms) | 54.19% | 202370 (0.937 ms) | 50.98% | 773555 | 34.35% | 12248 | paper_matrix_sequence_2026-06-05_19-15-58:41 |
| single_trng | reused | off |  | 377205 (1.746 ms) | 0.29% | 386986 (1.792 ms) | 0.06% | 414091 (1.917 ms) | -0.31% | 1178282 | 0.00% | 8680 | paper_matrix_sequence_2026-06-05_19-15-58:42 |
| single_flash_art_prefetch | reused | off |  | 378359 (1.752 ms) | -0.02% | 387299 (1.793 ms) | -0.02% | 413025 (1.912 ms) | -0.05% | 1178683 | -0.03% | 8792 | paper_matrix_sequence_2026-06-05_19-15-58:43 |
| single_itcm_opt | reused | keccak+matacc_cache+matacc_opt+frombytes+basemul |  | 373160 (1.728 ms) | 1.35% | 382214 (1.770 ms) | 1.29% | 406961 (1.884 ms) | 1.42% | 1162335 | 1.36% | 8792 | paper_itcm_a_path_sequence_2026-06-21_13-56-08:149 |
| single_dtcm_ntt | reused | off |  | 378314 (1.751 ms) | -0.01% | 387268 (1.793 ms) | -0.02% | 413000 (1.912 ms) | -0.04% | 1178582 | -0.02% | 10584 | paper_matrix_sequence_2026-06-05_19-15-58:46 |
| single_public_hpk | reused | off |  | 378198 (1.751 ms) | 0.02% | 331458 (1.535 ms) | 14.40% | 412969 (1.912 ms) | -0.03% | 1122625 | 4.73% | 8824 | paper_matrix_sequence_2026-06-05_19-15-58:47 |
| single_public_pk | reused | off |  | 386039 (1.787 ms) | -2.05% | 384361 (1.779 ms) | 0.73% | 410416 (1.900 ms) | 0.58% | 1180816 | -0.21% | 11864 | paper_matrix_sequence_2026-06-05_19-15-58:82 |

## ML-KEM-1024, 24mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 590538 (24.606 ms) | 0.00% | 597755 (24.906 ms) | 0.00% | 631860 (26.328 ms) | 0.00% | 1820153 | 0.00% | 10424 | paper_matrix_sequence_2026-06-05_19-15-58:53 |
| single_cached_a | reused | off |  | 620483 (25.853 ms) | -5.07% | 228984 (9.541 ms) | 61.69% | 262807 (10.950 ms) | 58.41% | 1112274 | 38.89% | 16568 | paper_matrix_sequence_2026-06-05_19-15-58:54 |
| single_trng | reused | off |  | 587704 (24.488 ms) | 0.48% | 596551 (24.856 ms) | 0.20% | 632129 (26.339 ms) | -0.04% | 1816384 | 0.21% | 10312 | paper_matrix_sequence_2026-06-05_19-15-58:55 |
| single_flash_art_prefetch | reused | off |  | 590892 (24.621 ms) | -0.06% | 597840 (24.910 ms) | -0.01% | 632169 (26.340 ms) | -0.05% | 1820901 | -0.04% | 10424 | paper_matrix_sequence_2026-06-05_19-15-58:56 |
| single_itcm_opt | reused | keccak+matacc_cache+matacc_opt |  | 589458 (24.561 ms) | 0.18% | 596844 (24.869 ms) | 0.15% | 630178 (26.257 ms) | 0.27% | 1816480 | 0.20% | 10424 | paper_itcm_a_path_sequence_2026-06-21_13-56-08:187 |
| single_dtcm_ntt | reused | off |  | 590926 (24.622 ms) | -0.07% | 597901 (24.913 ms) | -0.02% | 632207 (26.342 ms) | -0.05% | 1821034 | -0.05% | 12216 | paper_matrix_sequence_2026-06-05_19-15-58:59 |
| single_public_hpk | reused | off |  | 590023 (24.584 ms) | 0.09% | 522314 (21.763 ms) | 12.62% | 630093 (26.254 ms) | 0.28% | 1742430 | 4.27% | 10456 | paper_matrix_sequence_2026-06-05_19-15-58:60 |
| single_public_pk | reused | off |  | 600738 (25.031 ms) | -1.73% | 593089 (24.712 ms) | 0.78% | 626785 (26.116 ms) | 0.80% | 1820612 | -0.03% | 14520 | paper_matrix_sequence_2026-06-05_19-15-58:83 |

## ML-KEM-1024, 216mhz

| category | origin | ITCM | selected overlay | keygen avg cycles (ms) | keygen delta | encaps avg cycles (ms) | encaps delta | decaps avg cycles (ms) | decaps delta | total avg cycles | total delta | RAM | source |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | reused | off |  | 595065 (2.755 ms) | 0.00% | 601529 (2.785 ms) | 0.00% | 637113 (2.950 ms) | 0.00% | 1833707 | 0.00% | 10424 | paper_matrix_sequence_2026-06-05_19-15-58:66 |
| single_cached_a | reused | off |  | 626162 (2.899 ms) | -5.23% | 231939 (1.074 ms) | 61.44% | 266798 (1.235 ms) | 58.12% | 1124899 | 38.65% | 16568 | paper_matrix_sequence_2026-06-05_19-15-58:67 |
| single_trng | reused | off |  | 594056 (2.750 ms) | 0.17% | 600857 (2.782 ms) | 0.11% | 636259 (2.946 ms) | 0.13% | 1831172 | 0.14% | 10312 | paper_matrix_sequence_2026-06-05_19-15-58:68 |
| single_flash_art_prefetch | reused | off |  | 595091 (2.755 ms) | -0.00% | 601224 (2.783 ms) | 0.05% | 637107 (2.950 ms) | 0.00% | 1833422 | 0.02% | 10424 | paper_matrix_sequence_2026-06-05_19-15-58:69 |
| single_itcm_opt | reused | keccak+matacc_cache+matacc_opt |  | 590329 (2.733 ms) | 0.80% | 597800 (2.768 ms) | 0.62% | 631146 (2.922 ms) | 0.94% | 1819275 | 0.79% | 10424 | paper_itcm_a_path_sequence_2026-06-21_13-56-08:227 |
| single_dtcm_ntt | reused | off |  | 595128 (2.755 ms) | -0.01% | 601252 (2.784 ms) | 0.05% | 637199 (2.950 ms) | -0.01% | 1833579 | 0.01% | 12216 | paper_matrix_sequence_2026-06-05_19-15-58:72 |
| single_public_hpk | reused | off |  | 594791 (2.754 ms) | 0.05% | 526424 (2.437 ms) | 12.49% | 635040 (2.940 ms) | 0.33% | 1756255 | 4.22% | 10456 | paper_matrix_sequence_2026-06-05_19-15-58:73 |
| single_public_pk | reused | off |  | 605016 (2.801 ms) | -1.67% | 596601 (2.762 ms) | 0.82% | 631555 (2.924 ms) | 0.87% | 1833172 | 0.03% | 14520 | paper_matrix_sequence_2026-06-05_19-15-58:84 |
