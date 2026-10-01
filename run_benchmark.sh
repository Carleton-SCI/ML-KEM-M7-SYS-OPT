#!/bin/bash

# Default values
COMPILE=1
UPLOAD=1
READ=1
USE_HW_RNG="OFF"
MLKEM_K="768"
MLKEM_BACKEND="auto"
MLKEM_ARCH="m7"
MLKEM_OPT="speed"
MLKEM_KECCAK="m4_opt_m7"
MLKEM_VARIANT=""
MLKEM_CLOCK="24mhz"
MLKEM_FLASH_ACCEL="off"
MLKEM_ITCM_KECCAK="OFF"
MLKEM_ITCM_KECCAK_HELPER="OFF"
MLKEM_ITCM_NTT="OFF"
MLKEM_ITCM_MATACC_CACHE="OFF"
MLKEM_ITCM_MATACC_OPT="OFF"
MLKEM_ITCM_MATACC_CAPTURE="OFF"
MLKEM_ITCM_FROMBYTES="OFF"
MLKEM_ITCM_BASEMUL="OFF"
MLKEM_ITCM_COMPRESS="OFF"
MLKEM_ITCM_REDUCE="OFF"
MLKEM_ITCM_CBD="OFF"
MLKEM_DTCM_CONST="off"
MLKEM_PROFILE="off"
MLKEM_PUBLIC_HPK="OFF"
MLKEM_PUBLIC_PK="OFF"
MLKEM_BENCH_MODE="one_by_one"
USE_CACHED_A="ON"
CMAKE_FRESH=""
ARCH_SET=0
LIST_VARIANTS=0
LIST_CLOCKS=0

PRESET_CLOCKS=(
    24mhz 30mhz 32mhz 36mhz 40mhz 48mhz 60mhz 72mhz
    84mhz 96mhz 108mhz 120mhz 144mhz 168mhz 192mhz 216mhz
)
VALID_CLOCK_MHZ=(
    24 30 32 36 40 42 48 54 56 60 64 72 84 96 108 120
    144 168 192 216
)

usage() {
    echo "Usage: $0 [options]"
    echo "  --no-compile          Skip CMake configure/build"
    echo "  --no-upload           Skip flashing"
    echo "  --no-read             Skip UART read"
    echo "  --hw-rng              Enable STM32 hardware RNG"
    echo "  --k VALUE             ML-KEM key size: 512, 768, or 1024"
    echo "  --backend VALUE       Source tree: auto, pqm4, or pqm7"
    echo "  --arch VALUE          Architecture: m4 or m7"
    echo "  --opt VALUE           Optimization: speed or stack"
    echo "  --keccak VALUE        Keccak backend for pqm7: m4_opt_m7, m7, or m4"
    echo "  --variant VALUE       Exact implementation directory override, e.g. m7fspeed"
    echo "  --clock VALUE         Clock profile, e.g. 24mhz, 96mhz, 216mhz, or integer Hz"
    echo "  --flash-accel VALUE   Flash accelerator: off or art_prefetch"
    echo "  --itcm-keccak         Place selected Keccak permutation in ITCM"
    echo "  --itcm-ntt            Place NTT/inverse-NTT code in ITCM"
    echo "  --itcm-matacc-cache   Place matacc cache assembly kernels in ITCM"
    echo "  --itcm-matacc-opt     Place matacc opt assembly kernels in ITCM"
    echo "  --itcm-matacc-capture Place cached-A capture matacc kernels in ITCM"
    echo "  --itcm-frombytes      Place frombytes/dot-product kernels in ITCM"
    echo "  --itcm-basemul        Place basemul dot-product kernels in ITCM"
    echo "  --itcm-reduce         Place reduction/pointwise kernels in ITCM"
    echo "  --dtcm-const VALUE    DTCM public constants: off or ntt"
    echo "  --profile VALUE       Profiling: off or ops"
    echo "  --public-hpk          Enable public H(pk) cache"
    echo "  --no-public-hpk       Disable public H(pk) cache"
    echo "  --public-pk           Enable parsed public-key cache"
    echo "  --no-public-pk        Disable parsed public-key cache"
    echo "  --bench-mode VALUE    Benchmark loop mode: one_by_one or sequence"
    echo "  --list-clocks         List predefined and valid custom clock profiles and exit"
    echo "  --list-variants       List sourceable ML-KEM variants and exit"
    echo "  --cached-a            Enable cached matrix-A benchmark path when supported"
    echo "  --no-cached-a         Disable cached matrix-A benchmark path"
    echo "  --fresh               Recreate CMake cache before building"
    echo "  -h, --help            Show this help"
}

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --no-compile) COMPILE=0 ;;
        --no-upload) UPLOAD=0 ;;
        --no-read) READ=0 ;;
        --hw-rng) USE_HW_RNG="ON" ;;
        --k) MLKEM_K="$2"; shift ;;
        --backend) MLKEM_BACKEND="$2"; shift ;;
        --arch) MLKEM_ARCH="$2"; ARCH_SET=1; shift ;;
        --opt) MLKEM_OPT="$2"; shift ;;
        --keccak) MLKEM_KECCAK="$2"; shift ;;
        --variant) MLKEM_VARIANT="$2"; shift ;;
        --clock) MLKEM_CLOCK="$2"; shift ;;
        --flash-accel) MLKEM_FLASH_ACCEL="$2"; shift ;;
        --itcm-keccak) MLKEM_ITCM_KECCAK="ON" ;;
        --no-itcm-keccak) MLKEM_ITCM_KECCAK="OFF" ;;
        --itcm-keccak-helper) echo "Error: --itcm-keccak-helper is disabled for benchmark runs pending audit after repeated timeouts."; exit 1 ;;
        --no-itcm-keccak-helper) MLKEM_ITCM_KECCAK_HELPER="OFF" ;;
        --itcm-ntt) MLKEM_ITCM_NTT="ON" ;;
        --no-itcm-ntt) MLKEM_ITCM_NTT="OFF" ;;
        --itcm-matacc-cache) MLKEM_ITCM_MATACC_CACHE="ON" ;;
        --no-itcm-matacc-cache) MLKEM_ITCM_MATACC_CACHE="OFF" ;;
        --itcm-matacc-opt) MLKEM_ITCM_MATACC_OPT="ON" ;;
        --no-itcm-matacc-opt) MLKEM_ITCM_MATACC_OPT="OFF" ;;
        --itcm-matacc-capture) MLKEM_ITCM_MATACC_CAPTURE="ON" ;;
        --no-itcm-matacc-capture) MLKEM_ITCM_MATACC_CAPTURE="OFF" ;;
        --itcm-frombytes) MLKEM_ITCM_FROMBYTES="ON" ;;
        --no-itcm-frombytes) MLKEM_ITCM_FROMBYTES="OFF" ;;
        --itcm-basemul) MLKEM_ITCM_BASEMUL="ON" ;;
        --no-itcm-basemul) MLKEM_ITCM_BASEMUL="OFF" ;;
        --itcm-compress) echo "Error: --itcm-compress is disabled for benchmark runs pending audit after a shared-secret mismatch."; exit 1 ;;
        --no-itcm-compress) MLKEM_ITCM_COMPRESS="OFF" ;;
        --itcm-reduce) MLKEM_ITCM_REDUCE="ON" ;;
        --no-itcm-reduce) MLKEM_ITCM_REDUCE="OFF" ;;
        --itcm-cbd) echo "Error: --itcm-cbd is disabled for benchmark runs pending audit after repeated timeouts."; exit 1 ;;
        --no-itcm-cbd) MLKEM_ITCM_CBD="OFF" ;;
        --dtcm-const) MLKEM_DTCM_CONST="$2"; shift ;;
        --profile) MLKEM_PROFILE="$2"; shift ;;
        --public-hpk) MLKEM_PUBLIC_HPK="ON" ;;
        --no-public-hpk) MLKEM_PUBLIC_HPK="OFF" ;;
        --public-pk) MLKEM_PUBLIC_PK="ON" ;;
        --no-public-pk) MLKEM_PUBLIC_PK="OFF" ;;
        --bench-mode) MLKEM_BENCH_MODE="$2"; shift ;;
        --list-clocks) LIST_CLOCKS=1 ;;
        --list-variants) LIST_VARIANTS=1 ;;
        --cached-a) USE_CACHED_A="ON" ;;
        --no-cached-a) USE_CACHED_A="OFF" ;;
        --fresh) CMAKE_FRESH="--fresh" ;;
        -h|--help) usage; exit 0 ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

set -e

join_by_comma() {
    local separator=""
    local item=""
    for item in "$@"; do
        printf "%s%s" "$separator" "$item"
        separator=", "
    done
    printf "\n"
}

valid_clock_list() {
    local items=()
    for mhz in "${VALID_CLOCK_MHZ[@]}"; do
        items+=("${mhz}mhz")
    done
    join_by_comma "${items[@]}"
}

public_cache_display() {
    if [ "$MLKEM_PUBLIC_HPK" = "ON" ] && [ "$MLKEM_PUBLIC_PK" = "ON" ]; then
        printf "hpk_pk"
    elif [ "$MLKEM_PUBLIC_HPK" = "ON" ]; then
        printf "hpk"
    elif [ "$MLKEM_PUBLIC_PK" = "ON" ]; then
        printf "pk"
    else
        printf "off"
    fi
}

itcm_display() {
    local items=()
    [ "$MLKEM_ITCM_KECCAK" = "ON" ] && items+=("keccak")
    [ "$MLKEM_ITCM_KECCAK_HELPER" = "ON" ] && items+=("keccak_helper")
    [ "$MLKEM_ITCM_NTT" = "ON" ] && items+=("ntt")
    [ "$MLKEM_ITCM_MATACC_CACHE" = "ON" ] && items+=("matacc_cache")
    [ "$MLKEM_ITCM_MATACC_OPT" = "ON" ] && items+=("matacc_opt")
    [ "$MLKEM_ITCM_MATACC_CAPTURE" = "ON" ] && items+=("matacc_capture")
    [ "$MLKEM_ITCM_FROMBYTES" = "ON" ] && items+=("frombytes")
    [ "$MLKEM_ITCM_BASEMUL" = "ON" ] && items+=("basemul")
    [ "$MLKEM_ITCM_COMPRESS" = "ON" ] && items+=("compress")
    [ "$MLKEM_ITCM_REDUCE" = "ON" ] && items+=("reduce")
    [ "$MLKEM_ITCM_CBD" = "ON" ] && items+=("cbd")
    if [ ${#items[@]} -eq 0 ]; then
        printf "off"
    else
        local IFS=+
        printf "%s" "${items[*]}"
    fi
}

normalize_clock_input() {
    local raw="${1,,}"
    local mhz=""
    local hz=""
    raw="${raw//[[:space:]]/}"

    if [[ "$raw" =~ ^([0-9]+)mhz$ ]]; then
        mhz="${BASH_REMATCH[1]}"
    elif [[ "$raw" =~ ^([0-9]+)hz$ ]]; then
        hz="${BASH_REMATCH[1]}"
        if (( hz % 1000000 != 0 )); then
            echo "Error: --clock Hz values must be integer MHz values" >&2
            return 1
        fi
        mhz=$(( hz / 1000000 ))
    elif [[ "$raw" =~ ^([0-9]+)$ ]]; then
        if (( raw >= 1000000 )); then
            if (( raw % 1000000 != 0 )); then
                echo "Error: --clock Hz values must be integer MHz values" >&2
                return 1
            fi
            mhz=$(( raw / 1000000 ))
        else
            mhz="$raw"
        fi
    else
        echo "Error: --clock must look like 24mhz, 96mhz, or 216000000" >&2
        return 1
    fi

    for valid in "${VALID_CLOCK_MHZ[@]}"; do
        if (( mhz == valid )); then
            echo "${mhz}mhz"
            return 0
        fi
    done

    echo "Error: unsupported --clock ${mhz}mhz. Valid clocks: $(valid_clock_list)" >&2
    return 1
}

if [ $LIST_CLOCKS -eq 1 ]; then
    echo "Predefined interactive clock stops:"
    join_by_comma "${PRESET_CLOCKS[@]}"
    echo
    echo "All valid clock profiles, including custom stops:"
    valid_clock_list
    exit 0
fi

if [ $LIST_VARIANTS -eq 1 ]; then
    for backend in pqm4 pqm7; do
        echo "$backend:"
        find "third_party/$backend/crypto_kem" -mindepth 2 -maxdepth 2 -type d -path '*/ml-kem-*/*' \
            | sed "s#third_party/$backend/crypto_kem/##" \
            | sort
    done
    exit 0
fi

case "$MLKEM_K" in
    512|768|1024) ;;
    *) echo "Error: --k must be one of: 512, 768, 1024"; exit 1 ;;
esac

case "$MLKEM_BACKEND" in
    auto|pqm4|pqm7) ;;
    *) echo "Error: --backend must be one of: auto, pqm4, pqm7"; exit 1 ;;
esac

case "$MLKEM_ARCH" in
    m4|m7) ;;
    *) echo "Error: --arch must be one of: m4, m7"; exit 1 ;;
esac

case "$MLKEM_OPT" in
    speed|stack) ;;
    clean) echo "Error: --opt clean is not sourceable in this tree. Use speed or stack."; exit 1 ;;
    *) echo "Error: --opt must be one of: speed, stack"; exit 1 ;;
esac

case "$MLKEM_KECCAK" in
    m4_opt_m7|m7|m4) ;;
    *) echo "Error: --keccak must be one of: m4_opt_m7, m7, m4"; exit 1 ;;
esac

MLKEM_CLOCK="$(normalize_clock_input "$MLKEM_CLOCK")"
if ! python3 tools/stm32f767_clock_profiles.py --validate "$MLKEM_CLOCK" >/dev/null; then
    echo "Error: clock profile validation failed for $MLKEM_CLOCK"
    exit 1
fi

case "$MLKEM_FLASH_ACCEL" in
    off|art_prefetch) ;;
    *) echo "Error: --flash-accel must be one of: off, art_prefetch"; exit 1 ;;
esac

for itcm_flag in \
    MLKEM_ITCM_KECCAK \
    MLKEM_ITCM_KECCAK_HELPER \
    MLKEM_ITCM_NTT \
    MLKEM_ITCM_MATACC_CACHE \
    MLKEM_ITCM_MATACC_OPT \
    MLKEM_ITCM_MATACC_CAPTURE \
    MLKEM_ITCM_FROMBYTES \
    MLKEM_ITCM_BASEMUL \
    MLKEM_ITCM_COMPRESS \
    MLKEM_ITCM_REDUCE \
    MLKEM_ITCM_CBD; do
    case "${!itcm_flag}" in
        ON|OFF) ;;
        *) echo "Error: $itcm_flag must be ON or OFF"; exit 1 ;;
    esac
done

case "$MLKEM_DTCM_CONST" in
    off|ntt) ;;
    *) echo "Error: --dtcm-const must be one of: off, ntt"; exit 1 ;;
esac

case "$MLKEM_PROFILE" in
    off|ops) ;;
    *) echo "Error: --profile must be one of: off, ops"; exit 1 ;;
esac

case "$MLKEM_PUBLIC_HPK" in
    ON|OFF) ;;
    *) echo "Error: public H(pk) cache must be ON or OFF"; exit 1 ;;
esac

case "$MLKEM_PUBLIC_PK" in
    ON|OFF) ;;
    *) echo "Error: parsed public-key cache must be ON or OFF"; exit 1 ;;
esac

case "$MLKEM_BENCH_MODE" in
    one_by_one|sequence) ;;
    *) echo "Error: --bench-mode must be one of: one_by_one, sequence"; exit 1 ;;
esac

if [ "$MLKEM_BACKEND" = "pqm4" ] && [ $ARCH_SET -eq 0 ]; then
    MLKEM_ARCH="m4"
fi

if [ "$MLKEM_BACKEND" = "pqm4" ] && [ "$MLKEM_ARCH" != "m4" ]; then
    echo "Error: pqm4 only provides m4f variants. Use --arch m4 or --backend pqm7."
    exit 1
fi

if [ $COMPILE -eq 1 ]; then
    variant_display="$MLKEM_VARIANT"
    if [ -z "$variant_display" ]; then
        variant_display="auto"
    fi
    echo "Compiling ML-KEM-$MLKEM_K (backend=$MLKEM_BACKEND, variant=$variant_display, arch=$MLKEM_ARCH, opt=$MLKEM_OPT, keccak=$MLKEM_KECCAK, cached_a=$USE_CACHED_A, clock=$MLKEM_CLOCK, flash_accel=$MLKEM_FLASH_ACCEL, itcm=$(itcm_display), dtcm_const=$MLKEM_DTCM_CONST, profile=$MLKEM_PROFILE, public_hpk=$MLKEM_PUBLIC_HPK, public_pk=$MLKEM_PUBLIC_PK, public_cache=$(public_cache_display), bench_mode=$MLKEM_BENCH_MODE)..."
    cmake_args=(
        --preset Release
        -DUSE_HW_RNG=$USE_HW_RNG
        -DMLKEM_K=$MLKEM_K
        -DMLKEM_BACKEND=$MLKEM_BACKEND
        -DMLKEM_ARCH=$MLKEM_ARCH
        -DMLKEM_OPT=$MLKEM_OPT
        -DMLKEM_KECCAK=$MLKEM_KECCAK
        -DMLKEM_VARIANT=$MLKEM_VARIANT
        -DMLKEM_CLOCK=$MLKEM_CLOCK
        -DMLKEM_FLASH_ACCEL=$MLKEM_FLASH_ACCEL
        -DMLKEM_ITCM_KECCAK=$MLKEM_ITCM_KECCAK
        -DMLKEM_ITCM_KECCAK_HELPER=$MLKEM_ITCM_KECCAK_HELPER
        -DMLKEM_ITCM_NTT=$MLKEM_ITCM_NTT
        -DMLKEM_ITCM_MATACC_CACHE=$MLKEM_ITCM_MATACC_CACHE
        -DMLKEM_ITCM_MATACC_OPT=$MLKEM_ITCM_MATACC_OPT
        -DMLKEM_ITCM_MATACC_CAPTURE=$MLKEM_ITCM_MATACC_CAPTURE
        -DMLKEM_ITCM_FROMBYTES=$MLKEM_ITCM_FROMBYTES
        -DMLKEM_ITCM_BASEMUL=$MLKEM_ITCM_BASEMUL
        -DMLKEM_ITCM_COMPRESS=$MLKEM_ITCM_COMPRESS
        -DMLKEM_ITCM_REDUCE=$MLKEM_ITCM_REDUCE
        -DMLKEM_ITCM_CBD=$MLKEM_ITCM_CBD
        -DMLKEM_DTCM_CONST=$MLKEM_DTCM_CONST
        -DMLKEM_PROFILE=$MLKEM_PROFILE
        -DMLKEM_PUBLIC_HPK=$MLKEM_PUBLIC_HPK
        -DMLKEM_PUBLIC_PK=$MLKEM_PUBLIC_PK
        -DMLKEM_BENCH_MODE=$MLKEM_BENCH_MODE
        -DMLKEM_USE_CACHED_A=$USE_CACHED_A
    )
    if [ -n "$CMAKE_FRESH" ]; then
        cmake_args=("$CMAKE_FRESH" "${cmake_args[@]}")
    fi
    cmake "${cmake_args[@]}"
    cmake --build --preset Release -j$(nproc)
fi

if [ $UPLOAD -eq 1 ]; then
    echo "Uploading firmware..."
    if command -v STM32_Programmer_CLI &> /dev/null; then
        STM32_Programmer_CLI -c port=SWD -w build/Release/STM_ML-KEM_V3.elf -v -rst
    elif command -v openocd &> /dev/null; then
        openocd -f interface/stlink.cfg -f target/stm32f7x.cfg -c "program build/Release/STM_ML-KEM_V3.elf verify reset exit"
    elif command -v st-flash &> /dev/null; then
        if [ ! -f "build/Release/STM_ML-KEM_V3.bin" ]; then
            arm-none-eabi-objcopy -O binary build/Release/STM_ML-KEM_V3.elf build/Release/STM_ML-KEM_V3.bin
        fi
        st-flash --reset write build/Release/STM_ML-KEM_V3.bin 0x08000000
    else
        echo "Warning: No flashing tool found. Skipping upload."
    fi
fi

if [ $READ -eq 1 ]; then
    echo "Reading from UART (/dev/ttyACM0 at 115200 baud)..."
    #sleep 1
    if [ -c /dev/ttyACM0 ]; then
        stty -F /dev/ttyACM0 115200 cs8 -cstopb -parenb raw -echo
        timeout 0.5 cat /dev/ttyACM0 > /dev/null 2>&1 || true
        timeout 20 awk '{ print; fflush(); if ($0 ~ /BENCHMARK_DONE/) exit }' /dev/ttyACM0 || true
    else
        echo "Error: /dev/ttyACM0 not found. Check your connection."
    fi
fi
