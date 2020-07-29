
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

#include <STM32H7DMA.hpp>
#include <STM32H7HAL.hpp>

#include <STM32H7TouchController.hpp>
#include <STM32H7Instrumentation.hpp>

#include <KeySampler.hpp>

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

#include "stm32h7b3i_discovery.h"
#include "stm32h7b3i_discovery_sdram.h"
#include "stm32h7b3i_discovery_ospi.h"

#include "../Components/rk043fn48h/rk043fn48h.h"
#include "../Components/ft5336/ft5336.h"

#include "bsp.h"

LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;
}

//LCD Configuration Defines
#define LCD_HSYNC       RK043FN48H_HSYNC
#define LCD_VSYNC       RK043FN48H_VSYNC
#define LCD_HBP         (RK043FN48H_HBP - 11)
#define LCD_HFP         RK043FN48H_HFP
#define LCD_VBP         RK043FN48H_VBP
#define LCD_VFP         RK043FN48H_VFP
#define LCD_WIDTH       RK043FN48H_WIDTH
#define LCD_HEIGHT      RK043FN48H_HEIGHT

#if !defined(USE_BPP) || USE_BPP==16
#define FRAMEBUFFER_SIZE  (LCD_WIDTH * LCD_HEIGHT * 2)
#elif USE_BPP==24
#define FRAMEBUFFER_SIZE  (LCD_WIDTH * LCD_HEIGHT * 3)
#else
#error Unknown USE_BPP
#endif

#define FB_BUFFER_NBR     3
#define BMP_CACHE_SIZE    (4 * 1024 * 1024)

/**
  * @brief LCD special pins
  */
/* LCD Display control pin */
#define LCD_DISP_CTRL_PIN                     GPIO_PIN_2
#define LCD_DISP_CTRL_PULL                    GPIO_PULLUP
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

#if defined ( __GNUC__ ) || defined( __CC_ARM )
__attribute__((section(".FBSection"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="FBSection"
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
        /* De-assert display enable LCD_DISP_EN pin */
        HAL_GPIO_WritePin(LCD_DISP_EN_GPIO_PORT, LCD_DISP_EN_PIN, GPIO_PIN_RESET);

        /* Assert display enable LCD_DISP_CTRL pin */
        HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT, LCD_DISP_CTRL_PIN, GPIO_PIN_SET);

        /* Assert backlight LCD_BL_CTRL pin */
        HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);
    }
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

    gpio_init_structure.Pin = GPIO_PIN_2;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOH, &gpio_init_structure);


    /** Toggle Sw reset of LTDC IP */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();
  }

  LCD_DISP_CTRL_GPIO_CLK_ENABLE();
  LCD_BL_CTRL_GPIO_CLK_ENABLE();
  LCD_DISP_EN_GPIO_CLK_ENABLE();

  /* LCD_DISP_EN GPIO configuration */
  gpio_init_structure.Pin       = LCD_DISP_EN_PIN;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull      = LCD_DISP_EN_PULL;
  HAL_GPIO_Init(LCD_DISP_EN_GPIO_PORT, &gpio_init_structure);

  /* LCD_DISP_CTRL GPIO configuration */
  gpio_init_structure.Pin       = LCD_DISP_CTRL_PIN;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull      = LCD_DISP_CTRL_PULL;
  HAL_GPIO_Init(LCD_DISP_CTRL_GPIO_PORT, &gpio_init_structure);

  /* LCD_BL_CTRL GPIO configuration */
  gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;  /* LCD_BL_CTRL pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull      = LCD_BL_CTRL_PULL;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &gpio_init_structure);
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
        LCD_MspInit();
    }

    HAL_LTDC_Init(&hltdc);

    /* Configure LTDC layer framebuffer address */
    LCD_LayerDefaultInit(0, frameBuf0);

    return 0;
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

    /* RK043FN48H LCD clock configuration */
    /* LCD clock configuration */
    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 288 Mhz */
    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 288/30 = 9.6 Mhz */
    /* PLLUSBCLK = PLL3_VCO Output/PLL3Q = 288/6 = 48 Mhz */
    /* LTDC clock frequency = 9.6 Mhz */
    /* USB clock frequency = 48 Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 24;
    PeriphClkInitStruct.PLL3.PLL3N = 288;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 6;
    PeriphClkInitStruct.PLL3.PLL3R = 30;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
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

void hw_init()
{
    SCB_EnableICache();
    SCB_EnableDCache();

#if !defined (CONFIG_ROMLESS)
    MPU_Config();

    HAL_Init();
#endif /* ! CONFIG_ROMLESS */

    SystemClock_Config();

    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);

    __HAL_RCC_CRC_CLK_ENABLE();

    GPIO::init();

    if(BspInit() == 0)
    {
      configASSERT(0);
    }

    /* Create Queue with Button push events */
    ButtonEventQueue = xQueueCreate( 2, sizeof( uint32_t ) );
    if (ButtonEventQueue == 0)
    {
      configASSERT(0);
    }

    /* Disable FMC Bank1 to avoid speculative/cache accesses */
    FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;

    /* Initialize display */
    LCD_Init();
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
KeySampler btnCtrl;

void touchgfx_init()
{
    STM32H7DMA *dma = new STM32H7DMA;
    STM32H7TouchController *tc = new STM32H7TouchController;
    STM32H7Instrumentation *mcuInstr = new STM32H7Instrumentation;

    uint16_t dispWidth  = LCD_GetXSize();
    uint16_t dispHeight = LCD_GetYSize();

    HAL& hal = touchgfx_generic_init<STM32H7HAL>(*dma, display, *tc, dispWidth, dispHeight,
                                                 (uint16_t*)bmpCache, BMP_CACHE_SIZE, 128);

    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitDepth, true, true);

    hal.setTouchSampleRate(1);
    hal.setFingerSize(1);
    hal.setButtonController(&btnCtrl);
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

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
    user_button_press = BUTTON_USER;
    BaseType_t xHigherPriorityTaskWoken;

    // We have not woken a task at the start of the ISR.
    xHigherPriorityTaskWoken = pdFALSE;

    xQueueSendToBackFromISR( ButtonEventQueue, ( void * ) &user_button_press, &xHigherPriorityTaskWoken );

    // Switch context if necessary.
    if( xHigherPriorityTaskWoken )
    {
        taskYIELD ();
    }
}
