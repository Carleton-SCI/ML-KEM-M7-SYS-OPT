cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM_ML-KEM_V3)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

set(MLKEM_K "768" CACHE STRING "ML-KEM key size (512, 768, 1024)")
set(MLKEM_BACKEND "auto" CACHE STRING "ML-KEM source tree (auto, pqm4, pqm7)")
set(MLKEM_ARCH "m7" CACHE STRING "Implementation architecture (m4, m7)")
set(MLKEM_OPT "speed" CACHE STRING "Optimization variant (speed, stack)")
set(MLKEM_KECCAK "m4_opt_m7" CACHE STRING "Keccak permutation for pqm7 builds (m4_opt_m7, m7, m4)")
set(MLKEM_VARIANT "" CACHE STRING "Exact implementation directory override, for example m7fspeed")
set(MLKEM_CLOCK "24mhz" CACHE STRING "Benchmark clock profile, for example 24mhz, 96mhz, or 216mhz")
set(MLKEM_FLASH_ACCEL "off" CACHE STRING "Flash accelerator profile (off, art_prefetch)")
set(MLKEM_ITCM_KECCAK "OFF" CACHE STRING "Place selected Keccak permutation in ITCM (ON, OFF)")
set(MLKEM_ITCM_KECCAK_HELPER "OFF" CACHE STRING "Place generic Keccak helper routines in ITCM (ON, OFF)")
set(MLKEM_ITCM_NTT "OFF" CACHE STRING "Place NTT and inverse NTT code in ITCM (ON, OFF)")
set(MLKEM_ITCM_MATACC_CACHE "OFF" CACHE STRING "Place matacc cache assembly kernels in ITCM (ON, OFF)")
set(MLKEM_ITCM_MATACC_OPT "OFF" CACHE STRING "Place matacc opt assembly kernels in ITCM (ON, OFF)")
set(MLKEM_ITCM_MATACC_CAPTURE "OFF" CACHE STRING "Place cached-A capture matacc kernels in ITCM (ON, OFF)")
set(MLKEM_ITCM_FROMBYTES "OFF" CACHE STRING "Place frombytes/dot-product kernels in ITCM (ON, OFF)")
set(MLKEM_ITCM_BASEMUL "OFF" CACHE STRING "Place basemul dot-product kernels in ITCM (ON, OFF)")
set(MLKEM_ITCM_COMPRESS "OFF" CACHE STRING "Place compression/decompression C kernels in ITCM (ON, OFF)")
set(MLKEM_ITCM_REDUCE "OFF" CACHE STRING "Place reduction/pointwise arithmetic kernels in ITCM (ON, OFF)")
set(MLKEM_ITCM_CBD "OFF" CACHE STRING "Place CBD/noise post-processing kernels in ITCM (ON, OFF)")
set(MLKEM_DTCM_CONST "off" CACHE STRING "DTCM public-constant placement profile (off, ntt)")
set(MLKEM_PROFILE "off" CACHE STRING "Benchmark profiling profile (off, ops)")
set(MLKEM_PUBLIC_HPK "OFF" CACHE STRING "Cache public H(pk) (ON, OFF)")
set(MLKEM_PUBLIC_PK "OFF" CACHE STRING "Cache parsed public-key material (ON, OFF)")
set(MLKEM_BENCH_MODE "one_by_one" CACHE STRING "Benchmark loop mode (one_by_one, sequence)")
set(MLKEM_STATS "off" CACHE STRING "Report per-iteration cycle dispersion over the timed loop (off, stddev)")
option(MLKEM_USE_CACHED_A "Use cached matrix-A benchmark path when available" ON)

set_property(CACHE MLKEM_K PROPERTY STRINGS 512 768 1024)
set_property(CACHE MLKEM_BACKEND PROPERTY STRINGS auto pqm4 pqm7)
set_property(CACHE MLKEM_ARCH PROPERTY STRINGS m4 m7)
set_property(CACHE MLKEM_OPT PROPERTY STRINGS speed stack)
set_property(CACHE MLKEM_KECCAK PROPERTY STRINGS m4_opt_m7 m7 m4)
set(MLKEM_CLOCK_PRESETS
    24mhz 30mhz 32mhz 36mhz 40mhz 48mhz 60mhz 72mhz
    84mhz 96mhz 108mhz 120mhz 144mhz 168mhz 192mhz 216mhz)
set_property(CACHE MLKEM_CLOCK PROPERTY STRINGS ${MLKEM_CLOCK_PRESETS})
set_property(CACHE MLKEM_FLASH_ACCEL PROPERTY STRINGS off art_prefetch)
foreach(_itcm_cache_flag
        MLKEM_ITCM_KECCAK
        MLKEM_ITCM_KECCAK_HELPER
        MLKEM_ITCM_NTT
        MLKEM_ITCM_MATACC_CACHE
        MLKEM_ITCM_MATACC_OPT
        MLKEM_ITCM_MATACC_CAPTURE
        MLKEM_ITCM_FROMBYTES
        MLKEM_ITCM_BASEMUL
        MLKEM_ITCM_COMPRESS
        MLKEM_ITCM_REDUCE
        MLKEM_ITCM_CBD)
    set_property(CACHE ${_itcm_cache_flag} PROPERTY STRINGS OFF ON)
endforeach()
set_property(CACHE MLKEM_DTCM_CONST PROPERTY STRINGS off ntt)
set_property(CACHE MLKEM_PROFILE PROPERTY STRINGS off ops)
set_property(CACHE MLKEM_PUBLIC_HPK PROPERTY STRINGS OFF ON)
set_property(CACHE MLKEM_PUBLIC_PK PROPERTY STRINGS OFF ON)
set_property(CACHE MLKEM_BENCH_MODE PROPERTY STRINGS one_by_one sequence)
set_property(CACHE MLKEM_STATS PROPERTY STRINGS off stddev)

set(MLKEM_VALID_K 512 768 1024)
if(NOT MLKEM_K IN_LIST MLKEM_VALID_K)
    message(FATAL_ERROR "Unsupported MLKEM_K value: ${MLKEM_K}. Use 512, 768, or 1024.")
endif()

set(MLKEM_VALID_BACKENDS auto pqm4 pqm7)
if(NOT MLKEM_BACKEND IN_LIST MLKEM_VALID_BACKENDS)
    message(FATAL_ERROR "Unsupported MLKEM_BACKEND value: ${MLKEM_BACKEND}. Use auto, pqm4, or pqm7.")
endif()

set(MLKEM_VALID_ARCHES m4 m7)
if(NOT MLKEM_ARCH IN_LIST MLKEM_VALID_ARCHES)
    message(FATAL_ERROR "Unsupported MLKEM_ARCH value: ${MLKEM_ARCH}. Use m4 or m7.")
endif()

set(MLKEM_VALID_OPTS speed stack)
if(NOT MLKEM_OPT IN_LIST MLKEM_VALID_OPTS)
    message(FATAL_ERROR "Unsupported MLKEM_OPT value: ${MLKEM_OPT}. Sourceable variants are speed and stack; clean is not present in this tree.")
endif()

set(MLKEM_VALID_KECCAK m4_opt_m7 m7 m4)
if(NOT MLKEM_KECCAK IN_LIST MLKEM_VALID_KECCAK)
    message(FATAL_ERROR "Unsupported MLKEM_KECCAK value: ${MLKEM_KECCAK}. Use m4_opt_m7, m7, or m4.")
endif()

set(MLKEM_VALID_CLOCK_MHZ 24 30 32 36 40 42 48 54 56 60 64 72 84 96 108 120 144 168 192 216)
string(TOLOWER "${MLKEM_CLOCK}" MLKEM_CLOCK_LOWER)
string(REPLACE " " "" MLKEM_CLOCK_LOWER "${MLKEM_CLOCK_LOWER}")
if(MLKEM_CLOCK_LOWER MATCHES "^([0-9]+)mhz$")
    set(MLKEM_CLOCK_MHZ "${CMAKE_MATCH_1}")
elseif(MLKEM_CLOCK_LOWER MATCHES "^([0-9]+)hz$")
    set(_MLKEM_CLOCK_HZ "${CMAKE_MATCH_1}")
    math(EXPR _MLKEM_CLOCK_REM "${_MLKEM_CLOCK_HZ} % 1000000")
    if(NOT _MLKEM_CLOCK_REM EQUAL 0)
        message(FATAL_ERROR "Unsupported MLKEM_CLOCK value: ${MLKEM_CLOCK}. Hz values must be an integer MHz value.")
    endif()
    math(EXPR MLKEM_CLOCK_MHZ "${_MLKEM_CLOCK_HZ} / 1000000")
elseif(MLKEM_CLOCK_LOWER MATCHES "^([0-9]+)$")
    set(_MLKEM_CLOCK_NUM "${CMAKE_MATCH_1}")
    if(_MLKEM_CLOCK_NUM GREATER 1000000)
        math(EXPR _MLKEM_CLOCK_REM "${_MLKEM_CLOCK_NUM} % 1000000")
        if(NOT _MLKEM_CLOCK_REM EQUAL 0)
            message(FATAL_ERROR "Unsupported MLKEM_CLOCK value: ${MLKEM_CLOCK}. Hz values must be an integer MHz value.")
        endif()
        math(EXPR MLKEM_CLOCK_MHZ "${_MLKEM_CLOCK_NUM} / 1000000")
    else()
        set(MLKEM_CLOCK_MHZ "${_MLKEM_CLOCK_NUM}")
    endif()
else()
    message(FATAL_ERROR "Unsupported MLKEM_CLOCK value: ${MLKEM_CLOCK}. Use a value like 24mhz, 96mhz, or 216mhz.")
endif()

if(NOT MLKEM_CLOCK_MHZ IN_LIST MLKEM_VALID_CLOCK_MHZ)
    message(FATAL_ERROR
        "Unsupported MLKEM_CLOCK value: ${MLKEM_CLOCK}. Valid MHz values are: ${MLKEM_VALID_CLOCK_MHZ}. "
        "The interactive presets are: ${MLKEM_CLOCK_PRESETS}.")
endif()
set(MLKEM_CLOCK_CANONICAL "${MLKEM_CLOCK_MHZ}mhz")

set(MLKEM_VALID_FLASH_ACCEL off art_prefetch)
if(NOT MLKEM_FLASH_ACCEL IN_LIST MLKEM_VALID_FLASH_ACCEL)
    message(FATAL_ERROR "Unsupported MLKEM_FLASH_ACCEL value: ${MLKEM_FLASH_ACCEL}. Use off or art_prefetch.")
endif()

string(TOUPPER "${MLKEM_ITCM_KECCAK}" MLKEM_ITCM_KECCAK)
string(TOUPPER "${MLKEM_ITCM_KECCAK_HELPER}" MLKEM_ITCM_KECCAK_HELPER)
string(TOUPPER "${MLKEM_ITCM_NTT}" MLKEM_ITCM_NTT)
string(TOUPPER "${MLKEM_ITCM_MATACC_CACHE}" MLKEM_ITCM_MATACC_CACHE)
string(TOUPPER "${MLKEM_ITCM_MATACC_OPT}" MLKEM_ITCM_MATACC_OPT)
string(TOUPPER "${MLKEM_ITCM_MATACC_CAPTURE}" MLKEM_ITCM_MATACC_CAPTURE)
string(TOUPPER "${MLKEM_ITCM_FROMBYTES}" MLKEM_ITCM_FROMBYTES)
string(TOUPPER "${MLKEM_ITCM_BASEMUL}" MLKEM_ITCM_BASEMUL)
string(TOUPPER "${MLKEM_ITCM_COMPRESS}" MLKEM_ITCM_COMPRESS)
string(TOUPPER "${MLKEM_ITCM_REDUCE}" MLKEM_ITCM_REDUCE)
string(TOUPPER "${MLKEM_ITCM_CBD}" MLKEM_ITCM_CBD)

foreach(_itcm_flag
        MLKEM_ITCM_KECCAK
        MLKEM_ITCM_KECCAK_HELPER
        MLKEM_ITCM_NTT
        MLKEM_ITCM_MATACC_CACHE
        MLKEM_ITCM_MATACC_OPT
        MLKEM_ITCM_MATACC_CAPTURE
        MLKEM_ITCM_FROMBYTES
        MLKEM_ITCM_BASEMUL
        MLKEM_ITCM_COMPRESS
        MLKEM_ITCM_REDUCE
        MLKEM_ITCM_CBD)
    if(NOT ${_itcm_flag} STREQUAL "OFF" AND NOT ${_itcm_flag} STREQUAL "ON")
        message(FATAL_ERROR "Unsupported ${_itcm_flag} value: ${${_itcm_flag}}. Use ON or OFF.")
    endif()
endforeach()

set(MLKEM_VALID_DTCM_CONST off ntt)
if(NOT MLKEM_DTCM_CONST IN_LIST MLKEM_VALID_DTCM_CONST)
    message(FATAL_ERROR "Unsupported MLKEM_DTCM_CONST value: ${MLKEM_DTCM_CONST}. Use off or ntt.")
endif()

set(MLKEM_VALID_PROFILE off ops)
if(NOT MLKEM_PROFILE IN_LIST MLKEM_VALID_PROFILE)
    message(FATAL_ERROR "Unsupported MLKEM_PROFILE value: ${MLKEM_PROFILE}. Use off or ops.")
endif()

set(MLKEM_VALID_STATS off stddev)
if(NOT MLKEM_STATS IN_LIST MLKEM_VALID_STATS)
    message(FATAL_ERROR "Unsupported MLKEM_STATS value: ${MLKEM_STATS}. Use off or stddev.")
endif()

string(TOUPPER "${MLKEM_PUBLIC_HPK}" MLKEM_PUBLIC_HPK)
string(TOUPPER "${MLKEM_PUBLIC_PK}" MLKEM_PUBLIC_PK)

foreach(_public_flag MLKEM_PUBLIC_HPK MLKEM_PUBLIC_PK)
    if(NOT ${_public_flag} STREQUAL "OFF" AND NOT ${_public_flag} STREQUAL "ON")
        message(FATAL_ERROR "Unsupported ${_public_flag} value: ${${_public_flag}}. Use ON or OFF.")
    endif()
endforeach()

if(MLKEM_PUBLIC_HPK STREQUAL "ON" AND MLKEM_PUBLIC_PK STREQUAL "ON")
    set(MLKEM_PUBLIC_CACHE_STR_VALUE "hpk_pk")
elseif(MLKEM_PUBLIC_HPK STREQUAL "ON")
    set(MLKEM_PUBLIC_CACHE_STR_VALUE "hpk")
elseif(MLKEM_PUBLIC_PK STREQUAL "ON")
    set(MLKEM_PUBLIC_CACHE_STR_VALUE "pk")
else()
    set(MLKEM_PUBLIC_CACHE_STR_VALUE "off")
endif()

set(MLKEM_VALID_BENCH_MODES one_by_one sequence)
if(NOT MLKEM_BENCH_MODE IN_LIST MLKEM_VALID_BENCH_MODES)
    message(FATAL_ERROR "Unsupported MLKEM_BENCH_MODE value: ${MLKEM_BENCH_MODE}. Use one_by_one or sequence.")
endif()
set(MLKEM_BENCH_MODE_DEFS "")
if(MLKEM_BENCH_MODE STREQUAL "one_by_one")
    list(APPEND MLKEM_BENCH_MODE_DEFS MLKEM_BENCH_ONE_BY_ONE)
endif()

set(MLKEM_FLASH_ACCEL_DEFS "")
if(MLKEM_FLASH_ACCEL STREQUAL "art_prefetch")
    list(APPEND MLKEM_FLASH_ACCEL_DEFS MLKEM_FLASH_ACCEL_ART_PREFETCH)
endif()

set(MLKEM_ITCM_DEFS "")
set(MLKEM_ITCM_ENABLED_NAMES "")
foreach(_itcm_pair
        "MLKEM_ITCM_KECCAK;keccak"
        "MLKEM_ITCM_KECCAK_HELPER;keccak_helper"
        "MLKEM_ITCM_NTT;ntt"
        "MLKEM_ITCM_MATACC_CACHE;matacc_cache"
        "MLKEM_ITCM_MATACC_OPT;matacc_opt"
        "MLKEM_ITCM_MATACC_CAPTURE;matacc_capture"
        "MLKEM_ITCM_FROMBYTES;frombytes"
        "MLKEM_ITCM_BASEMUL;basemul"
        "MLKEM_ITCM_COMPRESS;compress"
        "MLKEM_ITCM_REDUCE;reduce"
        "MLKEM_ITCM_CBD;cbd")
    list(GET _itcm_pair 0 _itcm_flag)
    list(GET _itcm_pair 1 _itcm_name)
    if(${_itcm_flag} STREQUAL "ON")
        list(APPEND MLKEM_ITCM_DEFS MLKEM_ITCM_COPY ${_itcm_flag})
        list(APPEND MLKEM_ITCM_ENABLED_NAMES ${_itcm_name})
    endif()
endforeach()
list(REMOVE_DUPLICATES MLKEM_ITCM_DEFS)
if(MLKEM_ITCM_ENABLED_NAMES)
    list(JOIN MLKEM_ITCM_ENABLED_NAMES "+" MLKEM_ITCM_STR_VALUE)
else()
    set(MLKEM_ITCM_STR_VALUE "off")
endif()

set(MLKEM_DTCM_CONST_DEFS "")
if(MLKEM_DTCM_CONST STREQUAL "ntt")
    list(APPEND MLKEM_DTCM_CONST_DEFS MLKEM_DTCM_CONST_NTT)
endif()

set(MLKEM_PROFILE_DEFS "")
if(MLKEM_PROFILE STREQUAL "ops")
    list(APPEND MLKEM_PROFILE_DEFS MLKEM_PROFILE_OPS)
endif()

# Off by default: with MLKEM_STATS=off no dispersion code is compiled in, so
# existing benchmark builds and their firmware images are unchanged.
set(MLKEM_STATS_DEFS "")
if(MLKEM_STATS STREQUAL "stddev")
    list(APPEND MLKEM_STATS_DEFS MLKEM_REPORT_STDDEV)
endif()

set(MLKEM_PUBLIC_CACHE_DEFS "")
if(MLKEM_PUBLIC_HPK STREQUAL "ON" OR MLKEM_PUBLIC_PK STREQUAL "ON")
    list(APPEND MLKEM_PUBLIC_CACHE_DEFS MLKEM_PUBLIC_CACHE_ENABLED)
endif()
if(MLKEM_PUBLIC_HPK STREQUAL "ON")
    list(APPEND MLKEM_PUBLIC_CACHE_DEFS MLKEM_PUBLIC_CACHE_HPK)
endif()
if(MLKEM_PUBLIC_PK STREQUAL "ON")
    list(APPEND MLKEM_PUBLIC_CACHE_DEFS MLKEM_PUBLIC_CACHE_PK)
endif()

set(MLKEM_EFFECTIVE_BACKEND ${MLKEM_BACKEND})
if(MLKEM_EFFECTIVE_BACKEND STREQUAL "auto")
    if(MLKEM_ARCH STREQUAL "m4")
        set(MLKEM_EFFECTIVE_BACKEND pqm4)
    else()
        set(MLKEM_EFFECTIVE_BACKEND pqm7)
    endif()
endif()

if(MLKEM_EFFECTIVE_BACKEND STREQUAL "pqm4")
    if(NOT MLKEM_ARCH STREQUAL "m4")
        message(FATAL_ERROR "pqm4 only provides m4f variants. Use -DMLKEM_ARCH=m4 or MLKEM_BACKEND=pqm7.")
    endif()
    set(PQM_LIB_DIR ${CMAKE_SOURCE_DIR}/third_party/pqm4)
    if(MLKEM_VARIANT STREQUAL "")
        set(MLKEM_IMPL_DIR_NAME m4f${MLKEM_OPT})
    else()
        set(MLKEM_IMPL_DIR_NAME ${MLKEM_VARIANT})
    endif()
    set(KECCAK_SRC ${PQM_LIB_DIR}/common/keccakf1600.S)
    set(KECCAK_DEFS "")
elseif(MLKEM_EFFECTIVE_BACKEND STREQUAL "pqm7")
    set(PQM_LIB_DIR ${CMAKE_SOURCE_DIR}/third_party/pqm7)
    if(MLKEM_VARIANT STREQUAL "")
        if(MLKEM_ARCH STREQUAL "m7")
            set(MLKEM_IMPL_DIR_NAME m7f${MLKEM_OPT})
        elseif(MLKEM_KECCAK STREQUAL "m7")
            set(MLKEM_IMPL_DIR_NAME m4f${MLKEM_OPT}-m7keccak)
        else()
            set(MLKEM_IMPL_DIR_NAME m4f${MLKEM_OPT})
        endif()
    else()
        set(MLKEM_IMPL_DIR_NAME ${MLKEM_VARIANT})
    endif()
    set(KECCAK_HELPER_SRC ${PQM_LIB_DIR}/common/keccakf1600.S)
    if(MLKEM_KECCAK STREQUAL "m7")
        set(KECCAK_PERMUTE_SRC ${PQM_LIB_DIR}/common/keccakf1600_adomnicai_m7.s)
        set(KECCAK_DEFS MUPQ PQM7_USE_M7_KECCAK)
    elseif(MLKEM_KECCAK STREQUAL "m4")
        set(KECCAK_PERMUTE_SRC ${PQM_LIB_DIR}/common/keccakf1600_adomnicai_m4.S)
        set(KECCAK_DEFS MUPQ PQM7_USE_M4_KECCAK)
    elseif(MLKEM_KECCAK STREQUAL "m4_opt_m7")
        set(KECCAK_PERMUTE_SRC ${PQM_LIB_DIR}/common/keccakf1600_adomnicai_m4_opt_m7.s)
        set(KECCAK_DEFS MUPQ)
    else()
        message(FATAL_ERROR "Unsupported MLKEM_KECCAK value: ${MLKEM_KECCAK}")
    endif()
    set(KECCAK_SRC ${KECCAK_HELPER_SRC} ${KECCAK_PERMUTE_SRC})
else()
    message(FATAL_ERROR "Unsupported MLKEM_BACKEND value after auto resolution: ${MLKEM_EFFECTIVE_BACKEND}.")
endif()

set(MLKEM_DIR ${PQM_LIB_DIR}/crypto_kem/ml-kem-${MLKEM_K}/${MLKEM_IMPL_DIR_NAME})
if(NOT IS_DIRECTORY ${MLKEM_DIR})
    file(GLOB MLKEM_AVAILABLE_IMPLS LIST_DIRECTORIES true
         RELATIVE ${PQM_LIB_DIR}/crypto_kem/ml-kem-${MLKEM_K}
         ${PQM_LIB_DIR}/crypto_kem/ml-kem-${MLKEM_K}/*)
    message(FATAL_ERROR
        "ML-KEM implementation directory does not exist: ${MLKEM_DIR}\n"
        "Available ${MLKEM_EFFECTIVE_BACKEND} ML-KEM-${MLKEM_K} variants: ${MLKEM_AVAILABLE_IMPLS}")
endif()

if(MLKEM_USE_CACHED_A)
    set(MLKEM_CACHED_A_STR_VALUE "ON")
else()
    set(MLKEM_CACHED_A_STR_VALUE "OFF")
endif()

message(STATUS "ML-KEM backend: ${MLKEM_EFFECTIVE_BACKEND}")
message(STATUS "ML-KEM variant: ${MLKEM_IMPL_DIR_NAME}")
message(STATUS "ML-KEM clock: ${MLKEM_CLOCK_CANONICAL}")
message(STATUS "ML-KEM flash accelerator: ${MLKEM_FLASH_ACCEL}")
message(STATUS "ML-KEM ITCM profile: ${MLKEM_ITCM_STR_VALUE}")
message(STATUS "ML-KEM ITCM Keccak: ${MLKEM_ITCM_KECCAK}")
message(STATUS "ML-KEM ITCM Keccak helpers: ${MLKEM_ITCM_KECCAK_HELPER}")
message(STATUS "ML-KEM ITCM NTT: ${MLKEM_ITCM_NTT}")
message(STATUS "ML-KEM ITCM matacc cache: ${MLKEM_ITCM_MATACC_CACHE}")
message(STATUS "ML-KEM ITCM matacc opt: ${MLKEM_ITCM_MATACC_OPT}")
message(STATUS "ML-KEM ITCM matacc capture: ${MLKEM_ITCM_MATACC_CAPTURE}")
message(STATUS "ML-KEM ITCM frombytes: ${MLKEM_ITCM_FROMBYTES}")
message(STATUS "ML-KEM ITCM basemul: ${MLKEM_ITCM_BASEMUL}")
message(STATUS "ML-KEM ITCM compression: ${MLKEM_ITCM_COMPRESS}")
message(STATUS "ML-KEM ITCM reduction: ${MLKEM_ITCM_REDUCE}")
message(STATUS "ML-KEM ITCM CBD: ${MLKEM_ITCM_CBD}")
message(STATUS "ML-KEM DTCM constants: ${MLKEM_DTCM_CONST}")
message(STATUS "ML-KEM profile: ${MLKEM_PROFILE}")
message(STATUS "ML-KEM public H(pk) cache: ${MLKEM_PUBLIC_HPK}")
message(STATUS "ML-KEM parsed public-key cache: ${MLKEM_PUBLIC_PK}")
message(STATUS "ML-KEM public cache profile: ${MLKEM_PUBLIC_CACHE_STR_VALUE}")
message(STATUS "ML-KEM benchmark mode: ${MLKEM_BENCH_MODE}")

set(MLKEM_STACK_DEFS "")
if(MLKEM_IMPL_DIR_NAME MATCHES "stack")
    list(APPEND MLKEM_STACK_DEFS MLKEM_CACHED_A_STACK)
endif()

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    MLKEM_K_STR=\"${MLKEM_K}\"
    MLKEM_BACKEND_STR=\"${MLKEM_EFFECTIVE_BACKEND}\"
    MLKEM_ARCH_STR=\"${MLKEM_ARCH}\"
    MLKEM_OPT_STR=\"${MLKEM_OPT}\"
    MLKEM_KECCAK_STR=\"${MLKEM_KECCAK}\"
    MLKEM_VARIANT_STR=\"${MLKEM_IMPL_DIR_NAME}\"
    MLKEM_CACHED_A_STR=\"${MLKEM_CACHED_A_STR_VALUE}\"
    MLKEM_CLOCK_STR=\"${MLKEM_CLOCK_CANONICAL}\"
    MLKEM_CLOCK_MHZ=${MLKEM_CLOCK_MHZ}
    MLKEM_FLASH_ACCEL_STR=\"${MLKEM_FLASH_ACCEL}\"
    MLKEM_ITCM_STR=\"${MLKEM_ITCM_STR_VALUE}\"
    MLKEM_ITCM_KECCAK_STR=\"${MLKEM_ITCM_KECCAK}\"
    MLKEM_ITCM_KECCAK_HELPER_STR=\"${MLKEM_ITCM_KECCAK_HELPER}\"
    MLKEM_ITCM_NTT_STR=\"${MLKEM_ITCM_NTT}\"
    MLKEM_ITCM_MATACC_CACHE_STR=\"${MLKEM_ITCM_MATACC_CACHE}\"
    MLKEM_ITCM_MATACC_OPT_STR=\"${MLKEM_ITCM_MATACC_OPT}\"
    MLKEM_ITCM_MATACC_CAPTURE_STR=\"${MLKEM_ITCM_MATACC_CAPTURE}\"
    MLKEM_ITCM_FROMBYTES_STR=\"${MLKEM_ITCM_FROMBYTES}\"
    MLKEM_ITCM_BASEMUL_STR=\"${MLKEM_ITCM_BASEMUL}\"
    MLKEM_ITCM_COMPRESS_STR=\"${MLKEM_ITCM_COMPRESS}\"
    MLKEM_ITCM_REDUCE_STR=\"${MLKEM_ITCM_REDUCE}\"
    MLKEM_ITCM_CBD_STR=\"${MLKEM_ITCM_CBD}\"
    MLKEM_DTCM_CONST_STR=\"${MLKEM_DTCM_CONST}\"
    MLKEM_PROFILE_STR=\"${MLKEM_PROFILE}\"
    MLKEM_PUBLIC_HPK_STR=\"${MLKEM_PUBLIC_HPK}\"
    MLKEM_PUBLIC_PK_STR=\"${MLKEM_PUBLIC_PK}\"
    MLKEM_PUBLIC_CACHE_STR=\"${MLKEM_PUBLIC_CACHE_STR_VALUE}\"
    MLKEM_BENCH_MODE_STR=\"${MLKEM_BENCH_MODE}\"
    MLKEM_STATS_STR=\"${MLKEM_STATS}\"
    ${MLKEM_BENCH_MODE_DEFS}
    ${MLKEM_FLASH_ACCEL_DEFS}
    ${MLKEM_ITCM_DEFS}
    ${MLKEM_PROFILE_DEFS}
    ${MLKEM_STATS_DEFS}
    ${MLKEM_PUBLIC_CACHE_DEFS}
    ${MLKEM_STACK_DEFS}
)
if(MLKEM_USE_CACHED_A)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE MLKEM_BENCH_CACHED_A)
endif()

set(MUPQ_COMMON_DIR ${PQM_LIB_DIR}/mupq/common)

file(GLOB MLKEM_C_SRCS "${MLKEM_DIR}/*.c")
file(GLOB MLKEM_S_SRCS "${MLKEM_DIR}/*.s" "${MLKEM_DIR}/*.S")
list(FILTER MLKEM_C_SRCS EXCLUDE REGEX "/cached_a\\.c$")

if(MLKEM_EFFECTIVE_BACKEND STREQUAL "pqm7" AND MLKEM_IMPL_DIR_NAME STREQUAL "m7fstack")
    list(APPEND MLKEM_S_SRCS
        ${PQM_LIB_DIR}/slothy/examples/opt/armv7m/frombytes_mul_kyber_opt_m7.s
        ${PQM_LIB_DIR}/slothy/examples/opt/armv7m/frombytes_mul_acc_kyber_opt_m7.s
    )
endif()

if(MLKEM_PUBLIC_PK STREQUAL "ON" AND NOT MLKEM_IMPL_DIR_NAME MATCHES "stack")
    list(APPEND MLKEM_S_SRCS
        ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/public_cache_prime_asm.S
    )
    list(APPEND MLKEM_PUBLIC_CACHE_DEFS MLKEM_PUBLIC_CACHE_PRIME_ASM)
endif()

if(MLKEM_USE_CACHED_A AND MLKEM_EFFECTIVE_BACKEND STREQUAL "pqm7" AND
   MLKEM_IMPL_DIR_NAME STREQUAL "m7fspeed")
    list(APPEND MLKEM_S_SRCS
        ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/matacc_capture_16_32_opt_m7.s
        ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/matacc_capture_32_32_opt_m7.s
        ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/matacc_capture_32_16_opt_m7.s
        ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/matacc_capture_opt_16_32_opt_m7.s
        ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/matacc_capture_opt_32_32_opt_m7.s
        ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/matacc_capture_opt_32_16_opt_m7.s
    )
    list(APPEND MLKEM_PUBLIC_CACHE_DEFS MLKEM_CACHED_A_CAPTURE_M7)
endif()

if(MLKEM_USE_CACHED_A OR MLKEM_PUBLIC_HPK STREQUAL "ON" OR MLKEM_PUBLIC_PK STREQUAL "ON")
    list(APPEND MLKEM_C_SRCS ${CMAKE_SOURCE_DIR}/third_party/mlkem_cached_a/cached_a.c)
endif()

set(MLKEM_ADVANCED_ITCM_FLAGS
    MLKEM_ITCM_MATACC_CACHE
    MLKEM_ITCM_MATACC_OPT
    MLKEM_ITCM_MATACC_CAPTURE
    MLKEM_ITCM_FROMBYTES
    MLKEM_ITCM_BASEMUL
    MLKEM_ITCM_COMPRESS
    MLKEM_ITCM_REDUCE
    MLKEM_ITCM_CBD)
set(MLKEM_ADVANCED_ITCM_REQUESTED FALSE)
foreach(_advanced_itcm_flag IN LISTS MLKEM_ADVANCED_ITCM_FLAGS)
    if(${_advanced_itcm_flag} STREQUAL "ON")
        set(MLKEM_ADVANCED_ITCM_REQUESTED TRUE)
    endif()
endforeach()
if(MLKEM_ADVANCED_ITCM_REQUESTED AND
   (NOT MLKEM_EFFECTIVE_BACKEND STREQUAL "pqm7" OR
    NOT MLKEM_IMPL_DIR_NAME STREQUAL "m7fspeed"))
    message(FATAL_ERROR
        "Advanced ITCM kernel placement knobs currently apply to pqm7 m7fspeed only. "
        "Requested variant: backend=${MLKEM_EFFECTIVE_BACKEND}, variant=${MLKEM_IMPL_DIR_NAME}.")
endif()
if(MLKEM_ITCM_KECCAK_HELPER STREQUAL "ON" AND NOT MLKEM_EFFECTIVE_BACKEND STREQUAL "pqm7")
    message(FATAL_ERROR
        "MLKEM_ITCM_KECCAK_HELPER currently applies to pqm7 builds only; "
        "pqm4 uses MLKEM_ITCM_KECCAK for its Keccak assembly source.")
endif()
if(MLKEM_ITCM_NTT STREQUAL "ON" AND
   (NOT MLKEM_EFFECTIVE_BACKEND STREQUAL "pqm7" OR
    NOT MLKEM_IMPL_DIR_NAME MATCHES "^m7f"))
    message(FATAL_ERROR
        "MLKEM_ITCM_NTT currently applies to pqm7 m7f variants only. "
        "Requested variant: backend=${MLKEM_EFFECTIVE_BACKEND}, variant=${MLKEM_IMPL_DIR_NAME}.")
endif()
if(MLKEM_ITCM_MATACC_CAPTURE STREQUAL "ON" AND NOT MLKEM_USE_CACHED_A)
    message(FATAL_ERROR "MLKEM_ITCM_MATACC_CAPTURE requires MLKEM_USE_CACHED_A=ON because the capture assembly is only built for cached-A key generation.")
endif()

function(mlkem_make_itcm_asm OUT_VAR SRC SECTION_TAG)
    file(READ "${SRC}" _mlkem_itcm_content)
    string(REGEX REPLACE
        "(\n|^)([ \t]*\\.thumb[^\n]*\n)"
        "\\1\\2.section .itcm_text.${SECTION_TAG},\"ax\",%progbits\n"
        _mlkem_itcm_content
        "${_mlkem_itcm_content}")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/mlkem_itcm_sources")
    get_filename_component(_mlkem_itcm_name "${SRC}" NAME)
    set(_mlkem_itcm_out
        "${CMAKE_BINARY_DIR}/mlkem_itcm_sources/${SECTION_TAG}_${_mlkem_itcm_name}")
    file(WRITE "${_mlkem_itcm_out}" "${_mlkem_itcm_content}")
    set(${OUT_VAR} "${_mlkem_itcm_out}" PARENT_SCOPE)
endfunction()

function(mlkem_replace_asm_with_itcm BASENAME SECTION_TAG)
    set(_mlkem_updated_sources "")
    set(_mlkem_replaced FALSE)
    foreach(_mlkem_src IN LISTS MLKEM_S_SRCS)
        get_filename_component(_mlkem_src_name "${_mlkem_src}" NAME)
        if(_mlkem_src_name STREQUAL "${BASENAME}")
            mlkem_make_itcm_asm(_mlkem_generated "${_mlkem_src}" "${SECTION_TAG}")
            list(APPEND _mlkem_updated_sources "${_mlkem_generated}")
            set(_mlkem_replaced TRUE)
        else()
            list(APPEND _mlkem_updated_sources "${_mlkem_src}")
        endif()
    endforeach()
    if(NOT _mlkem_replaced)
        message(STATUS "ITCM source group ${SECTION_TAG}: ${BASENAME} not present in ${MLKEM_IMPL_DIR_NAME}")
    endif()
    set(MLKEM_S_SRCS "${_mlkem_updated_sources}" PARENT_SCOPE)
endfunction()

if(MLKEM_ITCM_MATACC_CACHE STREQUAL "ON")
    mlkem_replace_asm_with_itcm(matacc_asm_cache_16_32_kyber_opt_m7.s matacc_cache)
    mlkem_replace_asm_with_itcm(matacc_asm_cache_32_32_kyber_opt_m7.s matacc_cache)
    mlkem_replace_asm_with_itcm(matacc_asm_cache_32_16_kyber_opt_m7.s matacc_cache)
endif()
if(MLKEM_ITCM_MATACC_OPT STREQUAL "ON")
    mlkem_replace_asm_with_itcm(matacc_asm_opt_16_32_kyber_opt_m7.s matacc_opt)
    mlkem_replace_asm_with_itcm(matacc_asm_opt_32_32_kyber_opt_m7.s matacc_opt)
    mlkem_replace_asm_with_itcm(matacc_asm_opt_32_16_kyber_opt_m7.s matacc_opt)
endif()
if(MLKEM_ITCM_MATACC_CAPTURE STREQUAL "ON")
    mlkem_replace_asm_with_itcm(matacc_capture_16_32_opt_m7.s matacc_capture)
    mlkem_replace_asm_with_itcm(matacc_capture_32_32_opt_m7.s matacc_capture)
    mlkem_replace_asm_with_itcm(matacc_capture_32_16_opt_m7.s matacc_capture)
    mlkem_replace_asm_with_itcm(matacc_capture_opt_16_32_opt_m7.s matacc_capture)
    mlkem_replace_asm_with_itcm(matacc_capture_opt_32_32_opt_m7.s matacc_capture)
    mlkem_replace_asm_with_itcm(matacc_capture_opt_32_16_opt_m7.s matacc_capture)
endif()
if(MLKEM_ITCM_FROMBYTES STREQUAL "ON")
    mlkem_replace_asm_with_itcm(frombytes_mul_16_32_kyber_opt_m7.s frombytes)
    mlkem_replace_asm_with_itcm(frombytes_mul_acc_32_32_kyber_opt_m7.s frombytes)
    mlkem_replace_asm_with_itcm(frombytes_mul_acc_32_16_kyber_opt_m7.s frombytes)
endif()
if(MLKEM_ITCM_BASEMUL STREQUAL "ON")
    mlkem_replace_asm_with_itcm(basemul_16_32_kyber_opt_m7.s basemul)
    mlkem_replace_asm_with_itcm(basemul_acc_32_32_kyber_opt_m7.s basemul)
    mlkem_replace_asm_with_itcm(basemul_acc_32_16_kyber_opt_m7.s basemul)
endif()
if(MLKEM_ITCM_REDUCE STREQUAL "ON")
    mlkem_replace_asm_with_itcm(barrett_reduce_kyber_opt_m7.s reduce)
    mlkem_replace_asm_with_itcm(add_kyber_opt_m7.s reduce)
    mlkem_replace_asm_with_itcm(sub_kyber_opt_m7.s reduce)
    mlkem_replace_asm_with_itcm(fromplant_kyber_opt_m7.s reduce)
endif()

set(MLKEM_IMPL_SOURCES
    ${MLKEM_C_SRCS}
    ${MLKEM_S_SRCS}
    ${PQM_LIB_DIR}/common/randombytes.c
    ${MUPQ_COMMON_DIR}/fips202.c
    ${KECCAK_SRC}
)

add_library(mlkem_impl STATIC ${MLKEM_IMPL_SOURCES})
if(KECCAK_DEFS)
    target_compile_definitions(mlkem_impl PRIVATE ${KECCAK_DEFS})
endif()
target_compile_definitions(mlkem_impl PRIVATE
    ${MLKEM_ITCM_DEFS}
    ${MLKEM_DTCM_CONST_DEFS}
    ${MLKEM_PUBLIC_CACHE_DEFS}
    ${MLKEM_STACK_DEFS}
)
target_include_directories(mlkem_impl PUBLIC
    ${MLKEM_DIR}
    ${MUPQ_COMMON_DIR}
)
target_compile_options(mlkem_impl PRIVATE
    $<$<CONFIG:Release>:-O3>
)

target_link_libraries(mlkem_impl PUBLIC stm32cubemx)
target_link_libraries(${CMAKE_PROJECT_NAME} mlkem_impl)

option(USE_HW_RNG "Use STM32 Hardware RNG" OFF)
if(USE_HW_RNG)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE USE_HW_RNG)
target_compile_definitions(mlkem_impl PRIVATE USE_HW_RNG)
endif()
