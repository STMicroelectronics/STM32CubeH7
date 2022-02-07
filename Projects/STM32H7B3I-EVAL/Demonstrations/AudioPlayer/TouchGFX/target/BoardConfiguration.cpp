
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

#include <STM32H7DMA.hpp>
#include <STM32H7HAL.hpp>

#include <STM32H7TouchController.hpp>
#include <STM32H7Instrumentation.hpp>

#include "cmsis_os.h"
#include <FreeRTOS.h>
#include <queue.h>

/***********************************************************
 ******         24 Bits Per Pixel Support            *******
 ***********************************************************
 *
 * The default bit depth of the framebuffer is 16bpp. If you want 24bpp support, define the symbol "USE_BPP" with a value
 * of "24", e.g. "USE_BPP=24". This symbol affects the following:
 *
 * 1. Type of TouchGFX LCD (16bpp vs 24bpp)
 * 2. Bit depth of the framebuffer(s)
 * 3. TFT controller configuration.
 *
 * WARNING: Remember to modify your image formats accordingly in app/config/. Please see the following knowledgebase article
 * for further details on how to choose and configure the appropriate image formats for your application:
 *
 * https://touchgfx.zendesk.com/hc/en-us/articles/206725849
 */

extern "C" {
#include "stm32h7xx_hal.h"

#include "stm32h7b3i_eval.h"
#include "stm32h7b3i_eval_sdram.h"
#include "stm32h7b3i_eval_ospi.h"

#include "../Components/rk070er9427/rk070er9427.h"
#include "../Components/ft5336/ft5336.h"

LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;

}

/* External variables --------------------------------------------------------*/
#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __GNUC__ ) || defined( __CC_ARM )
__attribute__((section(".heap_mem"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="heap_mem"
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

//LCD Configuration Defines
#define LCD_HSYNC       RK070ER9427_HSYNC
#define LCD_VSYNC       RK070ER9427_VSYNC
#define LCD_HBP         RK070ER9427_HBP
#define LCD_HFP         RK070ER9427_HFP
#define LCD_VBP         RK070ER9427_VBP
#define LCD_VFP         RK070ER9427_VFP
#define LCD_WIDTH       RK070ER9427_WIDTH
#define LCD_HEIGHT      RK070ER9427_HEIGHT

#if !defined(USE_BPP) || USE_BPP==16
#define FRAMEBUFFER_SIZE  (LCD_WIDTH * LCD_HEIGHT * 2)
#elif USE_BPP==24
#define FRAMEBUFFER_SIZE  (LCD_WIDTH * LCD_HEIGHT * 3)
#else
#error Unknown USE_BPP
#endif

#define FB_BUFFER_NBR     2
#define BMP_CACHE_SIZE    (128 * 1024)

/**
  * @brief LCD special pins
  */
/* LCD Display control pin */
#define LCD_DISP_CTRL_PIN                     GPIO_PIN_2
#define LCD_DISP_CTRL_PULL                    GPIO_NOPULL
#define LCD_DISP_CTRL_GPIO_PORT               GPIOA
#define LCD_DISP_CTRL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_DISP_CTRL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()

/* LCD Display enable pin */
#define LCD_DISP_EN_PIN                      GPIO_PIN_7
#define LCD_DISP_EN_PULL                     GPIO_NOPULL
#define LCD_DISP_EN_GPIO_PORT                GPIOK
#define LCD_DISP_EN_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOK_CLK_ENABLE()
#define LCD_DISP_EN_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOK_CLK_DISABLE()

/* Back-light control pin */
#define LCD_BL_CTRL_PIN                       GPIO_PIN_1
#define LCD_BL_CTRL_PULL                      GPIO_NOPULL
#define LCD_BL_CTRL_GPIO_PORT                 GPIOA
#define LCD_BL_CTRL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_BL_CTRL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

#if defined ( __GNUC__ )
__attribute__((section(".FBSection"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="FBSection"
#elif defined( __CC_ARM )
__attribute__((section(".FBSection"), zero_init)) __attribute__ ((aligned (32)))
#endif
static uint8_t framebuffer[FB_BUFFER_NBR][FRAMEBUFFER_SIZE];

#if defined ( __GNUC__ )
__attribute__((section(".BmpCacheSection"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="BmpCacheSection"
#elif defined( __CC_ARM )
__attribute__((section(".BmpCacheSection"), zero_init)) __attribute__ ((aligned (32)))
#endif
static uint8_t BmpCache[BMP_CACHE_SIZE];

static uint32_t frameBuf0 = (uint32_t)(&framebuffer);
static uint8_t* bmpCache  = BmpCache;

uint32_t      user_button_press = BUTTONn;
QueueHandle_t ButtonEventQueue  = 0;

extern "C" uint32_t LCD_GetXSize()
{
    return LCD_WIDTH;
}

extern "C" uint32_t LCD_GetYSize()
{
    return LCD_HEIGHT;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED_RED);

  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t Address)
{
    LTDC_LayerCfgTypeDef Layercfg;

    /* Layer Init */
    Layercfg.WindowX0 = 0;
    Layercfg.WindowY0 = 0;
    Layercfg.WindowX1 = LCD_GetXSize();
    Layercfg.WindowY1 = LCD_GetYSize();
    Layercfg.FBStartAdress = Address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.ImageWidth = LCD_GetXSize();
    Layercfg.ImageHeight = LCD_GetYSize();
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
#if !defined(USE_BPP) || USE_BPP==16
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
#elif USE_BPP==24
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
#else
#error Unknown USE_BPP
#endif

    HAL_LTDC_ConfigLayer(&hltdc, &Layercfg, LayerIndex);
}

extern "C" void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc)
{
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();
}

void OneTimeInitializeDefaultLayer()
{
    static bool intialized = false;
    if (!intialized)
    {
        intialized = true;
        LCD_LayerDefaultInit(0, frameBuf0);
    }
}

/**
  * @brief  LCD Reset
  *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
  *         and deactivating it later.
  * @param  None
  */
static void LCD_Reset(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  LCD_DISP_CTRL_GPIO_CLK_ENABLE();

  /* Configure the GPIO Reset pin */
  gpio_init_structure.Pin   = LCD_DISP_CTRL_PIN;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = LCD_DISP_CTRL_PULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_DISP_CTRL_GPIO_PORT , &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT , LCD_DISP_CTRL_PIN, GPIO_PIN_RESET);
  HAL_Delay(20);/* wait 20 ms */
  HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT , LCD_DISP_CTRL_PIN, GPIO_PIN_SET);/* Deactivate XRES */
  HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */
}

/**
  * @brief  Configure LCD control pins (Back-light, Display Enable and TE)
  * @retval None
  */
static void LCD_InitSequence(void)
{
  GPIO_InitTypeDef  gpio_init_structure;
  /* LCD_DISP GPIO configuration */
  LCD_DISP_EN_GPIO_CLK_ENABLE();

  gpio_init_structure.Pin       = LCD_DISP_EN_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_DISP_EN_GPIO_PORT, &gpio_init_structure);
  /* Assert LCD_DISP  pin */
  HAL_GPIO_WritePin(LCD_DISP_EN_GPIO_PORT, LCD_DISP_EN_PIN, GPIO_PIN_SET);

  /* LCD_BL_CTRL GPIO configuration */
  LCD_BL_CTRL_GPIO_CLK_ENABLE();

  gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &gpio_init_structure);
  /* Assert back-light LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);
}

/**
  * @brief  Initialize the BSP LCD Msp.
  * Application can surcharge if needed this function implementation
  */
static void LCD_MspInit(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  if(hltdc.Instance == LTDC)
  {
    /* Enable the LTDC and DMA2D clocks */
    __HAL_RCC_LTDC_CLK_ENABLE();
    __HAL_RCC_DMA2D_CLK_ENABLE();

    /** @brief Toggle Sw reset of LTDC IP */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();

    /** @brief Toggle Sw reset of DMA2D IP */
    __HAL_RCC_DMA2D_FORCE_RESET();
    __HAL_RCC_DMA2D_RELEASE_RESET();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();

    /*** LTDC Pins configuration ***/
    /* GPIOI configuration */
    gpio_init_structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull      = GPIO_NOPULL;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &gpio_init_structure);

    /* GPIOJ configuration */
    gpio_init_structure.Pin       = GPIO_PIN_All;
    gpio_init_structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

    /* GPIOK configuration */
    gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    gpio_init_structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &gpio_init_structure);

    /** Toggle Sw reset of LTDC IP */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();
  }
}

static uint8_t LCD_Init(void)
{
    /* LTDC Config */
    /* Timing and polarity */
    hltdc.Init.HorizontalSync     = LCD_HSYNC - 1U;
    hltdc.Init.AccumulatedHBP     = LCD_HSYNC + LCD_HBP - 1U;
    hltdc.Init.AccumulatedActiveW = LCD_HSYNC + LCD_WIDTH + LCD_HBP - 1U;
    hltdc.Init.TotalWidth         = LCD_HSYNC + LCD_WIDTH + LCD_HBP + LCD_HFP - 1U;
    hltdc.Init.VerticalSync       = LCD_VSYNC - 1U;
    hltdc.Init.AccumulatedVBP     = LCD_VSYNC + LCD_VBP - 1U;
    hltdc.Init.AccumulatedActiveH = LCD_VSYNC + LCD_HEIGHT + LCD_VBP - 1U;
    hltdc.Init.TotalHeigh         = LCD_VSYNC + LCD_HEIGHT + LCD_VBP + LCD_VFP - 1U;

    /* Initialize the LCD pixel width and pixel height */
    hltdc.LayerCfg->ImageWidth    = LCD_WIDTH;
    hltdc.LayerCfg->ImageHeight   = LCD_HEIGHT;

    /* background value */
    hltdc.Init.Backcolor.Blue     = 0;
    hltdc.Init.Backcolor.Green    = 0;
    hltdc.Init.Backcolor.Red      = 0;

    /* Polarity */
    hltdc.Init.HSPolarity         = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity         = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity         = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;

    hltdc.Instance = LTDC;
    if (HAL_LTDC_GetState(&hltdc) == HAL_LTDC_STATE_RESET)
    {
        /* Toggle Hardware Reset of the LCD using its XRES signal (active low) */
        LCD_Reset();
        /* Initialize LCD special pins GPIOs */
        LCD_InitSequence();
        LCD_MspInit();
    }

    HAL_LTDC_Init(&hltdc);

    return 0;
}

/**
  * @brief  Initializes the OSPI interface.
  * @param  hospi          OSPI handle
  * @param  Config         OSPI config structure
  * @retval BSP status
  */
extern "C" HAL_StatusTypeDef MX_OSPI_NOR_Init(OSPI_HandleTypeDef *hospi, MX_OSPI_Config *Config)
{
  /* Fix the Clock Prescaler for DTR Mode */
  Config->ClockPrescaler = 4;

    /* OctoSPI initialization */
  hospi->Instance = OCTOSPI1;

  hospi->Init.FifoThreshold      = 4;
  hospi->Init.DualQuad           = HAL_OSPI_DUALQUAD_DISABLE;
  hospi->Init.DeviceSize         = Config->MemorySize; /* 512 MBits */
  hospi->Init.ChipSelectHighTime = 2;
  hospi->Init.FreeRunningClock   = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi->Init.ClockMode          = HAL_OSPI_CLOCK_MODE_0;
  hospi->Init.WrapSize           = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi->Init.ClockPrescaler     = Config->ClockPrescaler;
  hospi->Init.SampleShifting     = Config->SampleShifting;
  hospi->Init.ChipSelectBoundary = 0;

  if (Config->TransferRate == (uint32_t)BSP_OSPI_NOR_DTR_TRANSFER)
  {
    hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_MACRONIX;
    hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  }
  else
  {
    hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_MICRON;
    hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  }

  return HAL_OSPI_Init(hospi);
}

namespace touchgfx
{
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 24000000
  *            PLL_M                          = 12
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    /*!< Supply configuration update enable */
    HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLM = 12;
    RCC_OscInitStruct.PLL.PLLN = 280;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK)
    {
        while (1)
        {
        }
    }

    /* RK070ER9427 LCD clock configuration */
    /* LCD clock configuration */
    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 288 Mhz */
    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 288/9 = 32 Mhz */
    /* PLLUSBCLK = PLL3_VCO Output/PLL3Q = 288/6 = 48 Mhz */
    /* LTDC clock frequency = 24 Mhz */
    /* USB clock frequency = 48 Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 24;
    PeriphClkInitStruct.PLL3.PLL3N = 288;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 6;
    PeriphClkInitStruct.PLL3.PLL3R = 9;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        while (1)
        {
        }
    }


    /* Select PLL as system clock source and configure  bus clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                   RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
    if (ret != HAL_OK)
    {
        while (1)
        {
        }
    }

    /*
    Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
            (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
            The I/O Compensation Cell activation  procedure requires :
          - The activation of the CSI clock
          - The activation of the SYSCFG clock
          - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
    */
    __HAL_RCC_CSI_ENABLE() ;

    __HAL_RCC_SYSCFG_CLK_ENABLE() ;

    HAL_EnableCompensationCell();
}


/**
  * @brief  Configure the MPU attributes as Write Through for Internal D1SRAM.
  * @note   The Base Address is 0x24000000 since this memory interface is the AXI.
  *         The Configured Region Size is 512KB because same as D1SRAM size.
  * @param  None
  * @retval None
  */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for Quad-SPI area to strongly ordered
     This setting is essentially needed to avoid MCU blockings!
     See also STM Application Note AN4861 */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = OCTOSPI1_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for the QSPI 64MB to Cacheable WT */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = OCTOSPI1_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in SO */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER3;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in Write-through (Buffers) */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER4;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup AXI SRAM, SRAM1 and SRAM2 in Write-through */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D1_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_1MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER5;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
/* Setup SRD SRAM in Write-through */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SRD_SRAM_BASE;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER6;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);


  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}


static void ConfigureExternalRom(void)
{
  BSP_OSPI_NOR_Init_t Flash;

  BSP_OSPI_NOR_DeInit(0);

  Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  if(BSP_OSPI_NOR_Init(0, &Flash) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  if(BSP_OSPI_NOR_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}

static void ConfigureExternalRam(void)
{
  /* Initialize the SDRAM memory */
  if (BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None
  * @retval RetVal
  */
static void BspInit(void)
{
  /* Initialize the External Ram */
  ConfigureExternalRam();

  /* Initialize the External Rom */
  ConfigureExternalRom();
}


void hw_init()
{
    SCB_EnableICache();
    SCB_EnableDCache();

    MPU_Config();

    HAL_Init();

    SystemClock_Config();

    __HAL_RCC_CRC_CLK_ENABLE();

    GPIO::init();

    /* For error reporting */
    BSP_LED_Init(LED_RED);
    BSP_LED_Off(LED_RED);

    BspInit();
    /* Disable FMC Bank1 to avoid speculative/cache accesses */
    FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;
    
     /* Initialize display */
    LCD_Init();

    /* Initialize Tamper button */
    BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_EXTI);

}

#if !defined(USE_BPP) || USE_BPP==16
static LCD16bpp display;
static uint16_t bitDepth = 16;
#elif USE_BPP==24
static LCD24bpp display;
static uint16_t bitDepth = 24;
#else
#error Unknown USE_BPP
#endif

void touchgfx_init()
{
    STM32H7DMA *dma = new STM32H7DMA;
    STM32H7TouchController *tc = new STM32H7TouchController;
    STM32H7Instrumentation *mcuInstr = new STM32H7Instrumentation;

    uint16_t dispWidth  = LCD_GetXSize();
    uint16_t dispHeight = LCD_GetYSize();

    HAL& hal = touchgfx_generic_init<STM32H7HAL>(*dma, display, *tc, dispWidth, dispHeight,
                                                 (uint16_t*)bmpCache, BMP_CACHE_SIZE, 16);

    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitDepth, true, true);

    hal.setTouchSampleRate(1);
    hal.setFingerSize(10);
    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    mcuInstr->init();

    //Set MCU instrumentation and Load calculation
    hal.setMCUInstrumentation(mcuInstr);
    hal.enableMCULoadCalculation(true);
}
}
