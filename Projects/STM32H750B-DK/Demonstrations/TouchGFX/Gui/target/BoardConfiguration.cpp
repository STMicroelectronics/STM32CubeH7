
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

#include <STM32H7DMA.hpp>
#include <STM32H7HAL.hpp>

#include <STM32H750TouchController.hpp>
#include <STM32H7Instrumentation.hpp>

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
#include "stm32h7xx_hal_dsi.h"

#include "stm32h750b_discovery.h"
#include "stm32h750b_discovery_sdram.h"
#include "stm32h750b_discovery_qspi.h"
#include "stm32h750b_discovery_lcd.h"

#include "../Components/rk043fn48h/rk043fn48h.h"
#include "../Components/ft5336/ft5336.h"

    LTDC_HandleTypeDef hltdc;
    DMA2D_HandleTypeDef hdma2d;
}

#if !defined(USE_BPP) || USE_BPP==16
#define FRAMEBUFFER_SIZE  (RK043FN48H_WIDTH * RK043FN48H_HEIGHT * 2)
#elif USE_BPP==24
#define FRAMEBUFFER_SIZE  (RK043FN48H_WIDTH * RK043FN48H_HEIGHT * 3)
#else
#error Unknown USE_BPP
#endif

#define FB_BUFFER_NBR     3
#define BMP_CACHE_SIZE    (4 * 1024 * 1024)

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

extern "C" uint32_t LCD_GetXSize()
{
    return RK043FN48H_WIDTH;
}

extern "C" uint32_t LCD_GetYSize()
{
    return RK043FN48H_HEIGHT;
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
  * @brief  Initialize the BSP LCD Msp.
  * Application can surcharge if needed this function implementation
  */
static void LCD_MspInit(void)
{
    GPIO_InitTypeDef gpio_init_structure;

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
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*** LTDC Pins configuration ***/
    /* GPIOI configuration */
    gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_9 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull      = GPIO_NOPULL;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &gpio_init_structure);

    /* GPIOJ configuration */
    gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_3 | \
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull      = GPIO_NOPULL;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

    /* GPIOK configuration */
    gpio_init_structure.Pin       = GPIO_PIN_2 | GPIO_PIN_3 | \
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 ;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull      = GPIO_NOPULL;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &gpio_init_structure);

    /* GPIOH configuration */
    gpio_init_structure.Pin       =  GPIO_PIN_9 | GPIO_PIN_1;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull      = GPIO_NOPULL;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOH, &gpio_init_structure);

    gpio_init_structure.Pin       = GPIO_PIN_7;     /* LCD_DISP pin has to be manually controlled */
    gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOD, &gpio_init_structure);
    /* Assert display enable LCD_DISP pin */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
}

static uint8_t LCD_Init(void)
{
    /* LTDC Config */
    /* Timing and polarity */
    hltdc.Init.HorizontalSync = (RK043FN48H_HSYNC - 1);
    hltdc.Init.VerticalSync = (RK043FN48H_VSYNC - 1);
    hltdc.Init.AccumulatedHBP = (RK043FN48H_HSYNC + (RK043FN48H_HBP - 11) - 1);
    hltdc.Init.AccumulatedVBP = (RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
    hltdc.Init.AccumulatedActiveH = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
    hltdc.Init.AccumulatedActiveW = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + (RK043FN48H_HBP - 11) - 1);
    hltdc.Init.TotalHeigh = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP + RK043FN48H_VFP - 1);
    hltdc.Init.TotalWidth = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + (RK043FN48H_HBP - 11) + RK043FN48H_HFP - 1);

    /* Initialize the LCD pixel width and pixel height */
    hltdc.LayerCfg->ImageWidth  = RK043FN48H_WIDTH;
    hltdc.LayerCfg->ImageHeight = RK043FN48H_HEIGHT;

    /* background value */
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;

    /* Polarity */
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Instance = LTDC;

    if (HAL_LTDC_GetState(&hltdc) == HAL_LTDC_STATE_RESET)
    {
        LCD_MspInit();
    }

    HAL_LTDC_Init(&hltdc);

    /* Configure LTDC layer framebuffer address */
    //LCD_LayerDefaultInit(0, frameBuf0);

    return 0;
}

namespace touchgfx
{
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
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
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY); /* PWR set to LDO for the STM32H750B-DISCO board */

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 160;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK)
    {
        while (1)
        {
        }
    }

    /* LCD clock configuration */
    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 288 Mhz */
    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 288/32 = 9 Mhz */
    /* PLLUSBCLK = PLL3_VCO Output/PLL3Q = 288/6 = 48 Mhz */
    /* LTDC clock frequency = PLLLCDCLK = 9 Mhz */
    /* USB clock frequency = 48 Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 25;
    PeriphClkInitStruct.PLL3.PLL3N = 288;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 6;
    PeriphClkInitStruct.PLL3.PLL3R = 32;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        while (1)
        {
        }
    }

    /* PLL3 for USB Clock */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
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
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
    if (ret != HAL_OK)
    {
        while (1)
        {
        }
    }
}

extern "C" uint8_t BSP_QSPI_Init_NoReset(void);

void hw_init()
{
    SCB_InvalidateICache();
    SCB_EnableICache();

    SCB_InvalidateDCache();
    SCB_EnableDCache();

    HAL_Init();
    
    SystemClock_Config();

    BSP_SDRAM_Init(0);


    __HAL_RCC_CRC_CLK_ENABLE();

    /* Disable FMC Bank1 to avoid speculative/cache accesses */
    FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;

    GPIO::init();

    /* Initialize display */
    LCD_Init();
}

STM32H7DMA dma;
STM32H750TouchController tc;
STM32H7Instrumentation mcuInstr;

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
    uint16_t dispWidth  = LCD_GetXSize();
    uint16_t dispHeight = LCD_GetYSize();

    HAL& hal = touchgfx_generic_init<STM32H7HAL>(dma, display, tc, dispWidth, dispHeight,
                                                 (uint16_t*)bmpCache, BMP_CACHE_SIZE, 128);

    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitDepth, true, true);

    hal.setTouchSampleRate(1);
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    mcuInstr.init();

    //Set MCU instrumentation and Load calculation
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);
}
}
