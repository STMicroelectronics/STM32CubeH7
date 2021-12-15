
#include <touchgfx/hal/GPIO.hpp>
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>

#include <stm32h7xx_hal.h>

#include <STM32H7HAL.hpp>
#include <STM32H7DMA.hpp>

#include <EXC7200TouchController.hpp>
#include <STM32H7Instrumentation.hpp>

#define USE_DOUBLE_BUFFERING

extern "C"
{
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_dma.h>
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_rcc_ex.h>
#include <stm32h7xx_hal_tim.h>

#include <stm32h743i_eval.h>
#include <stm32h743i_eval_lcd.h>
#include <stm32h743i_eval_io.h>
#include <stm32h743i_eval_qspi.h>
#include <stm32h743i_eval_sdram.h>
  
#include "rtc_app.h"
#include "mfx.h"

extern LTDC_HandleTypeDef hlcd_ltdc;

#define LAYER0_ADDRESS  (LCD_FB_START_ADDRESS)

static uint32_t frameBuf0 = (uint32_t)(LAYER0_ADDRESS); //Beginning of SDRAM

#ifdef LCD_CLK_TUNING
#define FRACN3_MAX (1 << 13)
#define DELTA ((1<<13) >> 9)
#define TUNING_PERIOD  1000U
TIM_HandleTypeDef TimHandleLcd;
HAL_StatusTypeDef LCD_CLK_TuningInit(TIM_HandleTypeDef *TimHandle, uint32_t TimPriority);
void LCD_CLK_Tuning(void);
#endif /* LCD_CLK_TUNING */

static void Error_Handler(void)
{
  BSP_LED_On(LED3);

  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

#ifdef LCD_CLK_TUNING
/**
  * @brief  Initializes the LTDC MSP.
  * @param  hltdc: LTDC handle
  * @param  Params: Pointer to void
  * @retval None
  */
void BSP_LCD_MspInit(LTDC_HandleTypeDef *hltdc, void *Params)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable the LTDC and DMA2D clocks */
  __HAL_RCC_LTDC_CLK_ENABLE();
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  /*** LTDC Pins configuration ***/
  /* GPIOI configuration */
  gpio_init_structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* GPIOJ configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

  /* GPIOK configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);
}

/**
  * @brief  Clock Config.
  * @param  hltdc: LTDC handle
  * @param  Params: Pointer to void
  * @note   This API is called by BSP_LCD_Init()
  *         Being __weak it can be overwritten by the application
  * @retval None
  */
HAL_StatusTypeDef MX_LTDC_ClockConfig(LTDC_HandleTypeDef *hltdc)
{
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;

  if(LCD_CLK_TuningInit(&TimHandleLcd, 1) != HAL_OK)
  {
    while (1);
  }
  periph_clk_init_struct.PLL3.PLL3R = 8; // 25M~25.625M clock out
  periph_clk_init_struct.PLL3.PLL3N = 40;// 200M~205M VCO
  periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  periph_clk_init_struct.PLL3.PLL3M = 5;
  periph_clk_init_struct.PLL3.PLL3P = 2;
  periph_clk_init_struct.PLL3.PLL3Q = 2;
  periph_clk_init_struct.PLL3.PLL3FRACN = 0;
  periph_clk_init_struct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  periph_clk_init_struct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  return(HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct));

   
}

/*
 * TUNING_PERIOD may be 1k(1000U),10k(10000U),20k(20000U),50k,100k
 */
HAL_StatusTypeDef LCD_CLK_TuningInit(TIM_HandleTypeDef *TimHandle, uint32_t TimPriority)
{
  RCC_ClkInitTypeDef    clkconfig;
  uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
  uint32_t              uwPrescalerValue = 0U;
  uint32_t              pFLatency;

  /* Configure the TIM7 IRQ priority */
  HAL_NVIC_SetPriority(TIM7_IRQn, TimPriority ,0U);

  /* Enable the TIM7 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM7_IRQn);

  /* Enable TIM7 clock */
  __HAL_RCC_TIM7_CLK_ENABLE();

  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

  /* Get APB1 prescaler */
  uwAPB1Prescaler = clkconfig.APB1CLKDivider;

  /* Compute TIM6 clock */
  if (uwAPB1Prescaler == RCC_HCLK_DIV1)
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
  }

  /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

  /* Initialize TIM7 */
  TimHandle->Instance = TIM7;

  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM7CLK/100000) - 1]. to have a (1/100000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  TimHandle->Init.Period = (1000000U / TUNING_PERIOD) - 1U;
  TimHandle->Init.Prescaler = uwPrescalerValue;
  TimHandle->Init.ClockDivision = 0;
  TimHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(TimHandle) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    return HAL_TIM_Base_Start_IT(TimHandle);
  }

  /* Return function status */
  return HAL_ERROR;
}

void LCD_CLK_Tuning(void)
{
  static uint16_t t=0;
  static int16_t delta=DELTA;

  /* Disable PLL3FRACN */
  __HAL_RCC_PLL3FRACN_DISABLE();

  /* Configures PLL3 clock Fractional Part Of The Multiplication Factor */
  __HAL_RCC_PLL3FRACN_CONFIG(t);
  if(t == FRACN3_MAX)
    __HAL_RCC_PLL3FRACN_CONFIG(FRACN3_MAX-1);

  /* Enable PLL3FRACN */
  __HAL_RCC_PLL3FRACN_ENABLE();
  t += delta;
  if(t >= FRACN3_MAX)
    delta = -DELTA;
  else if(t == 0)
    delta = DELTA;
}
#endif /* LCD_CLK_TUNING */

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
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    /*!< Supply configuration update enable */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

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
    if(ret != HAL_OK)
    {
        Error_Handler();
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
    if(ret != HAL_OK)
    {
        Error_Handler();
    }

    /* Activate CSI clock mondatory for I/O Compensation Cell*/
    __HAL_RCC_CSI_ENABLE() ;

    /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;

    /* Enables the I/O Compensation Cell */ 
    HAL_EnableCompensationCell();
}

HAL_StatusTypeDef MX_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, uint32_t LayerIndex, MX_LTDC_LayerConfig_t *Config)
{
  LTDC_LayerCfgTypeDef pLayerCfg;

  pLayerCfg.WindowX0 = Config->X0;
  pLayerCfg.WindowX1 = Config->X1;
  pLayerCfg.WindowY0 = Config->Y0;
  pLayerCfg.WindowY1 = Config->Y1;
  pLayerCfg.PixelFormat = Config->PixelFormat;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = LAYER0_ADDRESS;
  pLayerCfg.ImageWidth = (Config->X1 - Config->X0);
  pLayerCfg.ImageHeight = (Config->Y1 - Config->Y0);
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  return HAL_LTDC_ConfigLayer(hltdc, &pLayerCfg, LayerIndex);
}

/**
  * @brief  Initializes the AMPIRE 640x480 LCD.
  * @param  None
  * @retval LCD state
  */
uint8_t Display_Init(void)
{
  BSP_LCD_InitEx(0, LCD_ORIENTATION_LANDSCAPE, LCD_PIXEL_FORMAT_RGB565, LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGHT);

  return 0;
}

/**
  * @brief  SD detect callback
  * @param  None
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
    case MFX_IRQOUT_PIN :
    {
      MfxDetectInt();
    }
    break;

    default:
      break;
  }
}
}

namespace touchgfx
{
void hw_init()
{
    *((uint32_t*)0x51046108) = 1; /* DMA2D issue in cut 1.1 */
    *((uint32_t*)0x51008108) = 1; /* AXI SRAM issue in cut 1.2 */

    HAL_Init();
    SystemClock_Config();

    __HAL_RCC_CRC_CLK_ENABLE();

    /* Enable SRAM1/SRAM2/SRAM3 clocks on D2 domain */
    __HAL_RCC_D2SRAM1_CLK_ENABLE();
    __HAL_RCC_D2SRAM2_CLK_ENABLE();
    __HAL_RCC_D2SRAM3_CLK_ENABLE();

    BSP_SDRAM_Init(0);

    /* Disable FMC Bank1 to avoid speculative/cache accesses */
    FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;

    /* Initialize the QSPI */
  BSP_QSPI_Init_t init ;
  init.InterfaceMode=MT25TL01G_QPI_MODE;
  init.TransferRate= MT25TL01G_DTR_TRANSFER ;
  init.DualFlashMode= MT25TL01G_DUALFLASH_ENABLE;
  
  BSP_QSPI_Init(0,&init);
  BSP_QSPI_EnableMemoryMappedMode(0);
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

  GPIO::init();

    /* Configure unused area of QSPI region as strongly ordered.
     * This is *important* to avoid unintentional fetches from illegal
     * addresses due to cache/speculation which would halt the MCU.
     */
    HAL_MPU_Disable();

    MPU_Region_InitTypeDef MPU_InitStruct;

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

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER3;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as WT for QSPI */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER4;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup Dcache in Write-through for SDRAM (framebuffer) */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0xD0000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup Dcache in Write-through for AXI SRAM, SRAM1 and SRAM2 */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x24000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x10000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER5;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup Dcache in Write-through for SRAM3 */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x10040000;
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

    /* Enable I&D caches */
    SCB_EnableICache();
    SCB_EnableDCache();

    /* Initialize the display */
    Display_Init();

#ifdef USE_STM32H747I_DISCO
    /* Init TEMP Sensor */
    TEMP_SENSOR_Init();
#else
    /* Init Potentiometer Sensor */
    BSP_POT_Init(POT1);
#endif

    /* Initialized RTC */
    RTC_CalendarBkupInit();
    
    /* Initialize MFX */
    MfxInit();
}

STM32H7DMA dma;
EXC7200TouchController tc;
STM32H7Instrumentation mcuInstr;

#if !defined(USE_BPP) || USE_BPP==16
LCD16bpp display;
uint16_t bitDepth = 16;
#else
#error Unknown USE_BPP
#endif

void touchgfx_init()
{
    uint32_t dispWidth;
    uint32_t dispHeight;
    BSP_LCD_GetXSize(0, &dispWidth);
    BSP_LCD_GetYSize(0, &dispHeight);
 
    HAL& hal = touchgfx_generic_init<STM32H7HAL>(dma, display, tc, dispWidth, dispHeight,
                                                 (uint16_t *)(frameBuf0 + dispWidth * dispHeight * 2 * 3),
                                                 4 * 1024 * 1024, 64);

#ifdef USE_DOUBLE_BUFFERING
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitDepth);
#else
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitDepth, false);
#endif

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
