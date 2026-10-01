/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "api.h"
#include "randombytes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifndef MLKEM_K_STR
#define MLKEM_K_STR "unknown"
#endif
#ifndef MLKEM_BACKEND_STR
#define MLKEM_BACKEND_STR "unknown"
#endif
#ifndef MLKEM_ARCH_STR
#define MLKEM_ARCH_STR "unknown"
#endif
#ifndef MLKEM_OPT_STR
#define MLKEM_OPT_STR "unknown"
#endif
#ifndef MLKEM_KECCAK_STR
#define MLKEM_KECCAK_STR "unknown"
#endif
#ifndef MLKEM_VARIANT_STR
#define MLKEM_VARIANT_STR "unknown"
#endif
#ifndef MLKEM_CACHED_A_STR
#define MLKEM_CACHED_A_STR "unknown"
#endif
#ifndef MLKEM_CLOCK_STR
#define MLKEM_CLOCK_STR "unknown"
#endif
#ifndef MLKEM_FLASH_ACCEL_STR
#define MLKEM_FLASH_ACCEL_STR "off"
#endif
#ifndef MLKEM_ITCM_STR
#define MLKEM_ITCM_STR "off"
#endif
#ifndef MLKEM_ITCM_KECCAK_STR
#define MLKEM_ITCM_KECCAK_STR "OFF"
#endif
#ifndef MLKEM_ITCM_KECCAK_HELPER_STR
#define MLKEM_ITCM_KECCAK_HELPER_STR "OFF"
#endif
#ifndef MLKEM_ITCM_NTT_STR
#define MLKEM_ITCM_NTT_STR "OFF"
#endif
#ifndef MLKEM_ITCM_MATACC_CACHE_STR
#define MLKEM_ITCM_MATACC_CACHE_STR "OFF"
#endif
#ifndef MLKEM_ITCM_MATACC_OPT_STR
#define MLKEM_ITCM_MATACC_OPT_STR "OFF"
#endif
#ifndef MLKEM_ITCM_MATACC_CAPTURE_STR
#define MLKEM_ITCM_MATACC_CAPTURE_STR "OFF"
#endif
#ifndef MLKEM_ITCM_FROMBYTES_STR
#define MLKEM_ITCM_FROMBYTES_STR "OFF"
#endif
#ifndef MLKEM_ITCM_BASEMUL_STR
#define MLKEM_ITCM_BASEMUL_STR "OFF"
#endif
#ifndef MLKEM_ITCM_COMPRESS_STR
#define MLKEM_ITCM_COMPRESS_STR "OFF"
#endif
#ifndef MLKEM_ITCM_REDUCE_STR
#define MLKEM_ITCM_REDUCE_STR "OFF"
#endif
#ifndef MLKEM_ITCM_CBD_STR
#define MLKEM_ITCM_CBD_STR "OFF"
#endif
#ifndef MLKEM_DTCM_CONST_STR
#define MLKEM_DTCM_CONST_STR "off"
#endif
#ifndef MLKEM_PROFILE_STR
#define MLKEM_PROFILE_STR "off"
#endif
#ifndef MLKEM_PUBLIC_HPK_STR
#define MLKEM_PUBLIC_HPK_STR "OFF"
#endif
#ifndef MLKEM_PUBLIC_PK_STR
#define MLKEM_PUBLIC_PK_STR "OFF"
#endif
#ifndef MLKEM_PUBLIC_CACHE_STR
#define MLKEM_PUBLIC_CACHE_STR "off"
#endif
#ifndef MLKEM_BENCH_MODE_STR
#define MLKEM_BENCH_MODE_STR "one_by_one"
#endif
#ifndef MLKEM_CLOCK_MHZ
#define MLKEM_CLOCK_MHZ 24U
#endif

#define NUM_ITERATIONS 100

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma location=0x2007c000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x2007c0a0
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x2007c000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x2007c0a0))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __GNUC__ ) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection")));   /* Ethernet Tx DMA Descriptors */
#endif

ETH_TxPacketConfig TxConfig;

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

static uint8_t g_pk[CRYPTO_PUBLICKEYBYTES];
static uint8_t g_sk[CRYPTO_SECRETKEYBYTES];
#if defined(CRYPTO_HAS_CACHED_A) && defined(MLKEM_BENCH_CACHED_A)
static uint8_t g_a[KYBER_MATRIXBYTES];
#endif
#if defined(CRYPTO_HAS_PUBLIC_CACHE) && defined(MLKEM_PUBLIC_CACHE_ENABLED)
static uint32_t g_public_cache[(KYBER_PUBLICCACHEBYTES + 3U) / 4U];
#endif
static uint8_t g_ct[CRYPTO_CIPHERTEXTBYTES];
static uint8_t g_ss_enc[CRYPTO_BYTES];
static uint8_t g_ss_dec[CRYPTO_BYTES];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */
static void BenchmarkClock_Config(void);
static void uart_send_text(const char *text);
static void uart_send_hex_line(const char *label, const uint8_t *data, size_t len);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct {
  uint32_t target_mhz;
  uint32_t voltage_scale;
  uint32_t pllm;
  uint32_t plln;
  uint32_t pllp;
  uint32_t pllq;
  uint32_t pllr;
  uint32_t ahb_divider;
  uint32_t apb1_divider;
  uint32_t apb2_divider;
  uint32_t flash_latency;
  uint8_t overdrive;
} benchmark_clock_profile_t;

/*
 * NUCLEO-F767ZI provides an 8 MHz HSE bypass clock from ST-LINK MCO.
 * These profiles use PLLM=4, so the PLL input is 2 MHz. Every entry keeps
 * PLL48 at 48 MHz by selecting PLLQ such that (2 MHz * PLLN) / PLLQ = 48 MHz;
 * this preserves the RNG/USB-class peripheral clock while sweeping SYSCLK.
 */
static const benchmark_clock_profile_t g_benchmark_clocks[] = {
  { 24,  PWR_REGULATOR_VOLTAGE_SCALE3, 4,  96, RCC_PLLP_DIV8, 4, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_0, 0 },
  { 30,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 120, RCC_PLLP_DIV8, 5, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_0, 0 },
  { 32,  PWR_REGULATOR_VOLTAGE_SCALE1, 4,  96, RCC_PLLP_DIV6, 4, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 36,  PWR_REGULATOR_VOLTAGE_SCALE1, 4,  72, RCC_PLLP_DIV4, 3, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 40,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 120, RCC_PLLP_DIV6, 5, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 42,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 168, RCC_PLLP_DIV8, 7, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 48,  PWR_REGULATOR_VOLTAGE_SCALE1, 4,  96, RCC_PLLP_DIV4, 4, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 54,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 216, RCC_PLLP_DIV8, 9, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 56,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 168, RCC_PLLP_DIV6, 7, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 60,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 120, RCC_PLLP_DIV4, 5, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_1, 0 },
  { 64,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 192, RCC_PLLP_DIV6, 8, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_2, 0 },
  { 72,  PWR_REGULATOR_VOLTAGE_SCALE1, 4,  72, RCC_PLLP_DIV2, 3, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_2, 0 },
  { 84,  PWR_REGULATOR_VOLTAGE_SCALE1, 4, 168, RCC_PLLP_DIV4, 7, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_2, 0 },
  { 96,  PWR_REGULATOR_VOLTAGE_SCALE1, 4,  96, RCC_PLLP_DIV2, 4, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_3, 0 },
  { 108, PWR_REGULATOR_VOLTAGE_SCALE1, 4, 216, RCC_PLLP_DIV4, 9, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_3, 0 },
  { 120, PWR_REGULATOR_VOLTAGE_SCALE1, 4, 120, RCC_PLLP_DIV2, 5, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_3, 0 },
  { 144, PWR_REGULATOR_VOLTAGE_SCALE1, 4, 144, RCC_PLLP_DIV2, 6, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_4, 0 },
  { 168, PWR_REGULATOR_VOLTAGE_SCALE1, 4, 168, RCC_PLLP_DIV2, 7, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_5, 0 },
  { 192, PWR_REGULATOR_VOLTAGE_SCALE1, 4, 192, RCC_PLLP_DIV2, 8, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_6, 1 },
  { 216, PWR_REGULATOR_VOLTAGE_SCALE1, 4, 216, RCC_PLLP_DIV2, 9, 2, RCC_SYSCLK_DIV1, RCC_HCLK_DIV4, RCC_HCLK_DIV2, FLASH_LATENCY_7, 1 },
};

static const benchmark_clock_profile_t *BenchmarkClock_GetProfile(void)
{
  for (size_t i = 0; i < sizeof(g_benchmark_clocks) / sizeof(g_benchmark_clocks[0]); i++) {
    if (g_benchmark_clocks[i].target_mhz == (uint32_t)MLKEM_CLOCK_MHZ) {
      return &g_benchmark_clocks[i];
    }
  }

  return NULL;
}

static void BenchmarkClock_WaitVoltageReady(void)
{
  uint32_t tickstart = HAL_GetTick();

  while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY) == RESET) {
    if ((HAL_GetTick() - tickstart) > 1000U) {
      Error_Handler();
    }
  }
}

static void BenchmarkClock_EnableHsi(void)
{
  RCC_OscInitTypeDef osc = {0};

  osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  osc.HSIState = RCC_HSI_ON;
  osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  osc.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
    Error_Handler();
  }
}

static void BenchmarkClock_SwitchToHsi(void)
{
  RCC_ClkInitTypeDef clk = {0};

  clk.ClockType = RCC_CLOCKTYPE_SYSCLK;
  clk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

  if (HAL_RCC_ClockConfig(&clk, __HAL_FLASH_GET_LATENCY()) != HAL_OK) {
    Error_Handler();
  }
}

static void BenchmarkClock_Config(void)
{
  RCC_OscInitTypeDef osc = {0};
  RCC_ClkInitTypeDef clk = {0};
  const benchmark_clock_profile_t *clock = BenchmarkClock_GetProfile();

  if (clock == NULL) {
    Error_Handler();
  }

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(clock->voltage_scale);
  BenchmarkClock_WaitVoltageReady();

  BenchmarkClock_EnableHsi();
  BenchmarkClock_SwitchToHsi();

  if (clock->overdrive != 0U) {
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
      Error_Handler();
    }
  }

  osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  osc.HSEState = RCC_HSE_BYPASS;
  osc.PLL.PLLState = RCC_PLL_ON;
  osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  osc.PLL.PLLM = clock->pllm;
  osc.PLL.PLLN = clock->plln;
  osc.PLL.PLLP = clock->pllp;
  osc.PLL.PLLQ = clock->pllq;
  osc.PLL.PLLR = clock->pllr;

  if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
    Error_Handler();
  }

  clk.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clk.AHBCLKDivider = clock->ahb_divider;
  clk.APB1CLKDivider = clock->apb1_divider;
  clk.APB2CLKDivider = clock->apb2_divider;

  if (HAL_RCC_ClockConfig(&clk, clock->flash_latency) != HAL_OK) {
    Error_Handler();
  }

  SystemCoreClockUpdate();
}

#if defined(MLKEM_ITCM_COPY)
extern uint32_t _siitcm_text;
extern uint32_t _sitcm_text;
extern uint32_t _eitcm_text;
#endif

static void BenchmarkFlashAccel_Config(void)
{
#if defined(MLKEM_FLASH_ACCEL_ART_PREFETCH)
  FLASH->ACR |= FLASH_ACR_ARTEN | FLASH_ACR_PRFTEN;
#else
  FLASH->ACR &= ~(FLASH_ACR_ARTEN | FLASH_ACR_PRFTEN);
#endif
  __DSB();
  __ISB();
}

static void BenchmarkMemory_Config(void)
{
#if defined(MLKEM_ITCM_COPY)
  uint32_t *src = &_siitcm_text;
  uint32_t *dst = &_sitcm_text;

  SCB->VTOR = FLASH_BASE;
  __DSB();
  __ISB();

  while (dst < &_eitcm_text) {
    *dst++ = *src++;
  }

  SCB_InvalidateICache();
  __DSB();
  __ISB();
#endif
}

static void dwt_cycle_counter_init(void)
{
  volatile uint32_t *dwt_lar = (uint32_t *)0xE0001FB0U;

  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  *dwt_lar = 0xC5ACCE55U;
  DWT->CYCCNT = 0;
  DWT->CPICNT = 0;
  DWT->LSUCNT = 0;
  DWT->FOLDCNT = 0;
  DWT->EXCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk | DWT_CTRL_CPIEVTENA_Msk |
               DWT_CTRL_LSUEVTENA_Msk | DWT_CTRL_FOLDEVTENA_Msk |
               DWT_CTRL_EXCEVTENA_Msk;
}

static void benchmark_keypair(void)
{
#if defined(CRYPTO_HAS_PUBLIC_CACHE) && defined(MLKEM_PUBLIC_CACHE_ENABLED) && \
    defined(CRYPTO_HAS_CACHED_A) && defined(MLKEM_BENCH_CACHED_A)
  crypto_kem_keypair_cached_a_public(g_pk, g_sk, g_a,
                                     (unsigned char *)g_public_cache);
#elif defined(CRYPTO_HAS_PUBLIC_CACHE) && defined(MLKEM_PUBLIC_CACHE_ENABLED)
  crypto_kem_keypair_public(g_pk, g_sk, (unsigned char *)g_public_cache);
#elif defined(CRYPTO_HAS_CACHED_A) && defined(MLKEM_BENCH_CACHED_A)
  crypto_kem_keypair_cached_a(g_pk, g_sk, g_a);
#else
  crypto_kem_keypair(g_pk, g_sk);
#endif
}

static void benchmark_encaps(void)
{
#if defined(CRYPTO_HAS_PUBLIC_CACHE) && defined(MLKEM_PUBLIC_CACHE_ENABLED) && \
    defined(CRYPTO_HAS_CACHED_A) && defined(MLKEM_BENCH_CACHED_A)
  crypto_kem_enc_cached_a_public(g_ct, g_ss_enc, g_pk, g_a,
                                 (unsigned char *)g_public_cache);
#elif defined(CRYPTO_HAS_PUBLIC_CACHE) && defined(MLKEM_PUBLIC_CACHE_ENABLED)
  crypto_kem_enc_public(g_ct, g_ss_enc, g_pk,
                        (unsigned char *)g_public_cache);
#elif defined(CRYPTO_HAS_CACHED_A) && defined(MLKEM_BENCH_CACHED_A)
  crypto_kem_enc_cached_a(g_ct, g_ss_enc, g_pk, g_a);
#else
  crypto_kem_enc(g_ct, g_ss_enc, g_pk);
#endif
}

static void benchmark_decaps(void)
{
#if defined(CRYPTO_HAS_PUBLIC_CACHE) && defined(MLKEM_PUBLIC_CACHE_ENABLED) && \
    defined(CRYPTO_HAS_CACHED_A) && defined(MLKEM_BENCH_CACHED_A)
  crypto_kem_dec_cached_a_public(g_ss_dec, g_ct, g_sk, g_a,
                                 (unsigned char *)g_public_cache);
#elif defined(CRYPTO_HAS_PUBLIC_CACHE) && defined(MLKEM_PUBLIC_CACHE_ENABLED)
  crypto_kem_dec_public(g_ss_dec, g_ct, g_sk,
                        (unsigned char *)g_public_cache);
#elif defined(CRYPTO_HAS_CACHED_A) && defined(MLKEM_BENCH_CACHED_A)
  crypto_kem_dec_cached_a(g_ss_dec, g_ct, g_sk, g_a);
#else
  crypto_kem_dec(g_ss_dec, g_ct, g_sk);
#endif
}

static uint32_t benchmark_keygen_cycles(void)
{
  uint32_t start = DWT->CYCCNT;
  benchmark_keypair();
  return DWT->CYCCNT - start;
}

static uint32_t benchmark_encaps_cycles(void)
{
  uint32_t start = DWT->CYCCNT;
  benchmark_encaps();
  return DWT->CYCCNT - start;
}

static uint32_t benchmark_decaps_cycles(void)
{
  uint32_t start = DWT->CYCCNT;
  benchmark_decaps();
  return DWT->CYCCNT - start;
}

#if defined(MLKEM_PROFILE_OPS)
typedef struct {
  uint32_t cycles;
  uint8_t cpi;
  uint8_t lsu;
  uint8_t fold;
  uint8_t exc;
  uint8_t counters_supported;
  uint8_t overflow_suspected;
} dwt_profile_sample_t;

static void dwt_profile_reset(void)
{
  DWT->CPICNT = 0;
  DWT->LSUCNT = 0;
  DWT->FOLDCNT = 0;
  DWT->EXCCNT = 0;
  DWT->CYCCNT = 0;
}

static void dwt_profile_finish(dwt_profile_sample_t *sample)
{
  sample->cycles = DWT->CYCCNT;
  sample->cpi = (uint8_t)DWT->CPICNT;
  sample->lsu = (uint8_t)DWT->LSUCNT;
  sample->fold = (uint8_t)DWT->FOLDCNT;
  sample->exc = (uint8_t)DWT->EXCCNT;
  sample->counters_supported =
      ((DWT->CTRL & DWT_CTRL_NOPRFCNT_Msk) == 0U) ? 1U : 0U;
  sample->overflow_suspected = (sample->cycles > 255U) ? 1U : 0U;
}

static void profile_keygen(dwt_profile_sample_t *sample)
{
  dwt_profile_reset();
  benchmark_keypair();
  dwt_profile_finish(sample);
}

static void profile_encaps(dwt_profile_sample_t *sample)
{
  dwt_profile_reset();
  benchmark_encaps();
  dwt_profile_finish(sample);
}

static void profile_decaps(dwt_profile_sample_t *sample)
{
  dwt_profile_reset();
  benchmark_decaps();
  dwt_profile_finish(sample);
}

static void print_profile_sample(const char *name,
                                 const dwt_profile_sample_t *sample)
{
  char msg[160];

  snprintf(msg, sizeof(msg),
           "PROFILE %s cycles=%lu cpi=%u lsu=%u fold=%u exc=%u supported=%u overflow=%u\r\n",
           name, (unsigned long)sample->cycles, sample->cpi, sample->lsu,
           sample->fold, sample->exc, sample->counters_supported,
           sample->overflow_suspected);
  uart_send_text(msg);
}

static void print_profile_results(void)
{
  dwt_profile_sample_t sample;

  uart_send_text("=== DWT Profile Samples ===\r\n");
  profile_keygen(&sample);
  print_profile_sample("Keygen", &sample);
  profile_encaps(&sample);
  print_profile_sample("Encaps", &sample);
  profile_decaps(&sample);
  print_profile_sample("Decaps", &sample);
}
#endif

static void uart_send_text(const char *text)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)text, strlen(text), HAL_MAX_DELAY);
}

static void uart_send_hex_line(const char *label, const uint8_t *data, size_t len)
{
  char msg[128];
  size_t offset = 0U;

  offset += (size_t)snprintf(msg + offset, sizeof(msg) - offset, "%s", label);
  for (size_t i = 0; i < len && (offset + 2U) < sizeof(msg); i++) {
    offset += (size_t)snprintf(msg + offset, sizeof(msg) - offset, "%02X", data[i]);
  }
  snprintf(msg + offset, sizeof(msg) - offset, "\r\n");
  uart_send_text(msg);
}

static void sort_u32(uint32_t *values, size_t count)
{
  for (size_t i = 1; i < count; i++) {
    uint32_t key = values[i];
    size_t j = i;

    while (j > 0 && values[j - 1] > key) {
      values[j] = values[j - 1];
      j--;
    }
    values[j] = key;
  }
}

static uint32_t median_u32(uint32_t *values, size_t count)
{
  if (count == 0U) {
    return 0U;
  }

  sort_u32(values, count);

  size_t mid = count / 2U;
  if ((count % 2U) == 0U) {
    return (uint32_t)(((uint64_t)values[mid - 1U] + (uint64_t)values[mid]) / 2U);
  }
  return values[mid];
}

#if defined(MLKEM_REPORT_STDDEV)
static uint64_t isqrt_u64(uint64_t value)
{
  if (value < 2U) {
    return value;
  }

  uint64_t x = value;
  uint64_t y = (x + 1U) / 2U;
  while (y < x) {
    x = y;
    y = (x + value / x) / 2U;
  }
  return x;
}

/* Population standard deviation of the per-iteration cycle counts.
 *
 * Divides by n, not n-1: this is a descriptive statistic over the complete set
 * of timed iterations actually collected in this run, not an inferential
 * estimate of a wider population. For count = 100 the two differ by a factor
 * sqrt(100/99) = 1.005, so the choice is immaterial numerically and is made for
 * consistency with how the value is reported.
 *
 * Operates only on the values array passed in, that is, on the inner timed
 * loop of one run. No cross-run or repeated-boot data enters this result.
 *
 * Uses the exact integer form n*sum(x^2) - (sum x)^2 so that the truncated
 * integer mean cannot bias the result. For NUM_ITERATIONS = 100 and cycle
 * counts below 2^21 both terms stay well inside the uint64 range.
 *
 * Order independent, so it may be called before or after median_u32 sorts the
 * array in place. */
static uint32_t stddev_u32(const uint32_t *values, size_t count)
{
  if (count < 2U) {
    return 0U;
  }

  uint64_t sum = 0U;
  uint64_t sum_sq = 0U;

  for (size_t i = 0; i < count; i++) {
    uint64_t v = (uint64_t)values[i];
    sum += v;
    sum_sq += v * v;
  }

  uint64_t n = (uint64_t)count;
  uint64_t variance = (n * sum_sq - sum * sum) / (n * n);

  return (uint32_t)isqrt_u64(variance);
}

/* Reported as an integer cycle count plus a relative value in percent with
 * four decimals, matching the integer-only formatting used elsewhere in this
 * file. Emitted on its own line so existing log parsers, which key on the
 * "min=" stat lines, ignore it. */
static void print_stddev_line(const char *name,
                              const uint32_t *values,
                              size_t count,
                              uint32_t avg_cycles)
{
  char msg[128];
  uint32_t sd = stddev_u32(values, count);
  uint32_t rel_e6 = 0U;

  if (avg_cycles != 0U) {
    rel_e6 = (uint32_t)(((uint64_t)sd * 1000000U) / (uint64_t)avg_cycles);
  }

  snprintf(msg, sizeof(msg),
           "STDDEV %s: sd=%lu cycles rel=%lu.%04lu %% n=%lu\r\n",
           name,
           (unsigned long)sd,
           (unsigned long)(rel_e6 / 10000U),
           (unsigned long)(rel_e6 % 10000U),
           (unsigned long)count);
  uart_send_text(msg);
}
#endif /* MLKEM_REPORT_STDDEV */

static uint32_t cycles_to_us(uint32_t cycles, uint32_t hclk)
{
  if (hclk == 0U) {
    return 0U;
  }
  return (uint32_t)((((uint64_t)cycles * 1000000ULL) + ((uint64_t)hclk / 2ULL)) /
                    (uint64_t)hclk);
}

static void format_cycles_ms(char *buf, size_t len, uint32_t cycles, uint32_t hclk)
{
  uint32_t us = cycles_to_us(cycles, hclk);

  snprintf(buf, len, "%lu (%lu.%03lu ms)",
           (unsigned long)cycles,
           (unsigned long)(us / 1000U),
           (unsigned long)(us % 1000U));
}

static void print_stat_line(const char *name,
                            uint32_t min_cycles,
                            uint32_t avg_cycles,
                            uint32_t med_cycles,
                            uint32_t max_cycles,
                            uint32_t hclk)
{
  char min_text[32];
  char avg_text[32];
  char med_text[32];
  char max_text[32];
  char msg[192];

  format_cycles_ms(min_text, sizeof(min_text), min_cycles, hclk);
  format_cycles_ms(avg_text, sizeof(avg_text), avg_cycles, hclk);
  format_cycles_ms(med_text, sizeof(med_text), med_cycles, hclk);
  format_cycles_ms(max_text, sizeof(max_text), max_cycles, hclk);

  snprintf(msg, sizeof(msg), "%s : min=%s avg=%s med=%s max=%s\r\n",
           name, min_text, avg_text, med_text, max_text);
  uart_send_text(msg);
}

static void print_benchmark_config(void)
{
  char msg[768];
  uint32_t hclk = HAL_RCC_GetHCLKFreq();

  snprintf(msg, sizeof(msg),
           "Config: ML-KEM-%s backend=%s variant=%s arch=%s opt=%s keccak=%s cached_a=%s clock=%s flash_accel=%s itcm=%s itcm_keccak=%s itcm_keccak_helper=%s itcm_ntt=%s itcm_matacc_cache=%s itcm_matacc_opt=%s itcm_matacc_capture=%s itcm_frombytes=%s itcm_basemul=%s itcm_compress=%s itcm_reduce=%s itcm_cbd=%s dtcm_const=%s profile=%s public_hpk=%s public_pk=%s public_cache=%s bench_mode=%s\r\n",
           MLKEM_K_STR, MLKEM_BACKEND_STR, MLKEM_VARIANT_STR,
           MLKEM_ARCH_STR, MLKEM_OPT_STR, MLKEM_KECCAK_STR,
           MLKEM_CACHED_A_STR, MLKEM_CLOCK_STR, MLKEM_FLASH_ACCEL_STR,
           MLKEM_ITCM_STR, MLKEM_ITCM_KECCAK_STR,
           MLKEM_ITCM_KECCAK_HELPER_STR, MLKEM_ITCM_NTT_STR,
           MLKEM_ITCM_MATACC_CACHE_STR, MLKEM_ITCM_MATACC_OPT_STR,
           MLKEM_ITCM_MATACC_CAPTURE_STR, MLKEM_ITCM_FROMBYTES_STR,
           MLKEM_ITCM_BASEMUL_STR, MLKEM_ITCM_COMPRESS_STR,
           MLKEM_ITCM_REDUCE_STR, MLKEM_ITCM_CBD_STR,
           MLKEM_DTCM_CONST_STR, MLKEM_PROFILE_STR,
           MLKEM_PUBLIC_HPK_STR, MLKEM_PUBLIC_PK_STR,
           MLKEM_PUBLIC_CACHE_STR, MLKEM_BENCH_MODE_STR);
  uart_send_text(msg);
  snprintf(msg, sizeof(msg), "HCLK=%lu Hz\r\n", (unsigned long)hclk);
  uart_send_text(msg);
}

static int rng_self_test(void)
{
#ifdef USE_HW_RNG
  uint8_t sample[16];
  char msg[96];
  size_t pos = 0;
  int all_zero = 1;
  int all_same = 1;

  randombytes(sample, sizeof(sample));

  for (size_t i = 0; i < sizeof(sample); i++) {
    if (sample[i] != 0) {
      all_zero = 0;
    }
    if (sample[i] != sample[0]) {
      all_same = 0;
    }
  }

  pos += snprintf(msg + pos, sizeof(msg) - pos, "RNG sample: ");
  for (size_t i = 0; i < sizeof(sample) && (pos + 2) < sizeof(msg); i++) {
    pos += snprintf(msg + pos, sizeof(msg) - pos, "%02X", sample[i]);
  }
  pos += snprintf(msg + pos, sizeof(msg) - pos, "\r\n");
  uart_send_text(msg);

  if ((RNG->SR & (RNG_SR_SECS | RNG_SR_CECS)) != 0) {
    return 0;
  }
  if (all_zero || all_same) {
    return 0;
  }
  return 1;
#else
  uart_send_text("RNG sample: hardware RNG disabled\r\n");
  return 1;
#endif
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  BenchmarkClock_Config();
  BenchmarkFlashAccel_Config();
  BenchmarkMemory_Config();

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  char msg[256];
  uint32_t keygen_cycles[NUM_ITERATIONS];
  uint32_t encaps_cycles[NUM_ITERATIONS];
  uint32_t decaps_cycles[NUM_ITERATIONS];
  int shared_secret_match;
  uint32_t keygen_min, keygen_max, keygen_avg, keygen_sum;
  uint32_t encaps_min, encaps_max, encaps_avg, encaps_sum;
  uint32_t decaps_min, decaps_max, decaps_avg, decaps_sum;
  uint32_t keygen_median, encaps_median, decaps_median;

  HAL_Delay(1500);

  dwt_cycle_counter_init();
  uart_send_text("ML-KEM benchmark start\r\n");
  print_benchmark_config();
  if (!rng_self_test()) {
    uart_send_text("RNG failed, aborting benchmark\r\n");
    while (1) {
      HAL_Delay(1000);
    }
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    shared_secret_match = 1;
    keygen_sum = encaps_sum = decaps_sum = 0;
    keygen_min = encaps_min = decaps_min = 0xFFFFFFFF;
    keygen_max = encaps_max = decaps_max = 0;

    HAL_SuspendTick();
    benchmark_keygen_cycles();
    benchmark_encaps_cycles();
    benchmark_decaps_cycles();


#if defined(MLKEM_BENCH_ONE_BY_ONE)
    snprintf(msg, sizeof(msg), "Running benchmarks one by one...\r\n");
    uart_send_text(msg);
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
      keygen_cycles[i] = benchmark_keygen_cycles();
      if (keygen_cycles[i] < keygen_min) keygen_min = keygen_cycles[i];
      if (keygen_cycles[i] > keygen_max) keygen_max = keygen_cycles[i];
      keygen_sum += keygen_cycles[i];
    }
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
      encaps_cycles[i] = benchmark_encaps_cycles();
      if (encaps_cycles[i] < encaps_min) encaps_min = encaps_cycles[i];
      if (encaps_cycles[i] > encaps_max) encaps_max = encaps_cycles[i];
      encaps_sum += encaps_cycles[i];
    }
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
      decaps_cycles[i] = benchmark_decaps_cycles();
      if (decaps_cycles[i] < decaps_min) decaps_min = decaps_cycles[i];
      if (decaps_cycles[i] > decaps_max) decaps_max = decaps_cycles[i];
      decaps_sum += decaps_cycles[i];
    }

#else
    snprintf(msg, sizeof(msg), "Running benchmarks in one loop...\r\n");
    uart_send_text(msg);

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
      keygen_cycles[i] = benchmark_keygen_cycles();
      encaps_cycles[i] = benchmark_encaps_cycles();
      decaps_cycles[i] = benchmark_decaps_cycles();
      
      if (memcmp(g_ss_enc, g_ss_dec, CRYPTO_BYTES) != 0) {
        shared_secret_match = 0;
      }

      if (keygen_cycles[i] < keygen_min) keygen_min = keygen_cycles[i];
      if (keygen_cycles[i] > keygen_max) keygen_max = keygen_cycles[i];
      keygen_sum += keygen_cycles[i];

      if (encaps_cycles[i] < encaps_min) encaps_min = encaps_cycles[i];
      if (encaps_cycles[i] > encaps_max) encaps_max = encaps_cycles[i];
      encaps_sum += encaps_cycles[i];

      if (decaps_cycles[i] < decaps_min) decaps_min = decaps_cycles[i];
      if (decaps_cycles[i] > decaps_max) decaps_max = decaps_cycles[i];
      decaps_sum += decaps_cycles[i];
    }
#endif

    if (memcmp(g_ss_enc, g_ss_dec, CRYPTO_BYTES) != 0) {
      shared_secret_match = 0;
    }

    keygen_avg = keygen_sum / NUM_ITERATIONS;
    encaps_avg = encaps_sum / NUM_ITERATIONS;
    decaps_avg = decaps_sum / NUM_ITERATIONS;

    keygen_median = median_u32(keygen_cycles, NUM_ITERATIONS);
    encaps_median = median_u32(encaps_cycles, NUM_ITERATIONS);
    decaps_median = median_u32(decaps_cycles, NUM_ITERATIONS);

    snprintf(msg, sizeof(msg), "=== Benchmark Results (%d iterations) ===\r\n", NUM_ITERATIONS);
    uart_send_text(msg);
    print_stat_line("Keygen", keygen_min, keygen_avg, keygen_median,
                    keygen_max, HAL_RCC_GetHCLKFreq());
    print_stat_line("Encaps", encaps_min, encaps_avg, encaps_median,
                    encaps_max, HAL_RCC_GetHCLKFreq());
    print_stat_line("Decaps", decaps_min, decaps_avg, decaps_median,
                    decaps_max, HAL_RCC_GetHCLKFreq());
#if defined(MLKEM_REPORT_STDDEV)
    print_stddev_line("Keygen", keygen_cycles, NUM_ITERATIONS, keygen_avg);
    print_stddev_line("Encaps", encaps_cycles, NUM_ITERATIONS, encaps_avg);
    print_stddev_line("Decaps", decaps_cycles, NUM_ITERATIONS, decaps_avg);
#endif
    uart_send_hex_line("Encapsulated Shared Secret: ", g_ss_enc, CRYPTO_BYTES);
    uart_send_hex_line("Decapsulated Shared Secret: ", g_ss_dec, CRYPTO_BYTES);
    snprintf(msg, sizeof(msg), "Shared Secret Match: %d\r\n\r\n", shared_secret_match);
    uart_send_text(msg);

#if defined(MLKEM_PROFILE_OPS)
    print_profile_results();
#endif

    HAL_ResumeTick();

    uart_send_text("BENCHMARK_DONE\r\n");
    while (1) {
      HAL_Delay(1000);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV8;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
