# ITCM Selection Evidence

This directory preserves the successful raw evidence used to choose the
endpoint ITCM profiles reported in the paper. The `a_path_*` files compare
regenerated-A and captured-A paths; the `composite_*` files record candidate
placements applied to combined profiles. The `raw/` directories retain all
366 UART logs, while the CSV files preserve case metadata and cycle summaries.
Of the attempted cases, 324 completed successfully, 41 timed out during UART
collection, and one exploratory case reported a secret mismatch. Failed cases
are retained but are not used for endpoint selection.

`best_itcm_vs_basic_baseline.csv` records the selected composite placements and
their comparison with the basic baseline. `itcm_fit_matrix_2026-06-21.csv`
records measured ITCM sizes and rejected overflow combinations. The endpoint
selection table is reproduced in the repository's main `README.md`.
