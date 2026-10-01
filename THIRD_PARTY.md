# Third-Party Software

This repository includes source snapshots from several upstream projects.
Project-specific code is covered by the root `LICENSE`; vendored components
remain subject to their own licenses, notices, and source-file headers.

| Component | Location | Upstream | License/provenance note |
| --- | --- | --- | --- |
| pqm4 | `third_party/pqm4/` | <https://github.com/mupq/pqm4> | pqm4 states that implementation licenses vary; its remaining framework is dual Apache-2.0/CC0. Consult its README and individual source notices. |
| pqm7 | `third_party/pqm7/` | <https://github.com/mupq/pqm7> | Experimental Cortex-M7 framework. Consult the included README and source-file notices. |
| mupq/PQClean subsets | `third_party/pqm4/mupq/`, `third_party/pqm7/mupq/` | <https://github.com/mupq/mupq>, <https://github.com/PQClean/PQClean> | Licenses vary by implementation; consult the included READMEs and source notices. |
| SLOTHY | `third_party/pqm7/slothy/` | <https://github.com/slothy-optimizer/slothy> | MIT; see `third_party/pqm7/slothy/LICENSE`. |
| STM32F7 HAL | `Drivers/STM32F7xx_HAL_Driver/` | <https://github.com/STMicroelectronics/STM32CubeF7> | BSD-3-Clause fallback terms are recorded in the included `LICENSE.txt`. |
| CMSIS and STM32F7 device support | `Drivers/CMSIS/` | <https://github.com/ARM-software/CMSIS_5>, <https://github.com/STMicroelectronics/cmsis-device-f7> | CMSIS is Apache-2.0; STM32F7 device terms are in the included license files. |

The publication snapshot intentionally omits nested vendor `.git/`
directories. Exact nested upstream commit identifiers were not retained when
that metadata was removed, so the table does not claim revisions that cannot
be verified. The public release commit fixes the exact vendored contents used
by this artifact; the preserved `.gitmodules`, READMEs, licenses, and source
headers provide upstream and licensing context.
