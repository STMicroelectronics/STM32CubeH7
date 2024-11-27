/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_DAC_ADC_Interconnect/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup HRTIM_DAC_ADC_Interconnect
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HRTIM_MASTER_PERIOD  0x7
#define HRTIM_TIMA_PERIOD    0xFFF 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t HRTIM_TIMA_PULSE = 0;
uint32_t uwConvertedValue = 0;

/* Timer handler declaration */
HRTIM_HandleTypeDef                 HrtimHandle;
DAC_HandleTypeDef                   DacHandle;
ADC_HandleTypeDef                   AdcHandle;

HRTIM_TimeBaseCfgTypeDef            sConfig_time_base;
HRTIM_TimerCfgTypeDef               sConfig_timerA;
HRTIM_TimerCfgTypeDef               sConfig_master;
HRTIM_OutputCfgTypeDef              sConfig_output_config;
HRTIM_CompareCfgTypeDef             sConfig_compare;
DAC_ChannelConfTypeDef              sConfig_dac;
ADC_ChannelConfTypeDef              sConfig_adc;
HRTIM_ADCTriggerCfgTypeDef          sConfig_adctrig;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void DAC_Configuration(void);
static void ADC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
  */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /*##-1- Configure the HRTIM peripheral ######################################################*/
  /* Initialize the HRTIM structure */
  HrtimHandle.Instance = HRTIM1;
  HrtimHandle.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
  HrtimHandle.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;

  HAL_HRTIM_Init(&HrtimHandle);

  /*##-2- Configure the HRTIM MASTER ########################################################*/
  /* Initialize the HRTIM Master timing structure */
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  sConfig_time_base.Period = HRTIM_MASTER_PERIOD;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.RepetitionCounter = 0;
          
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_MASTER, &sConfig_time_base);

  sConfig_master.DMARequests = HRTIM_TIM_DMA_NONE;
  sConfig_master.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  sConfig_master.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  sConfig_master.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  sConfig_master.DACSynchro = HRTIM_DACSYNC_DACTRIGOUT_1;
  sConfig_master.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  sConfig_master.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  sConfig_master.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  sConfig_master.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  sConfig_master.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  sConfig_master.InterruptRequests = HRTIM_TIM_IT_NONE;

  HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_MASTER,&sConfig_master);

 /* Configure the HRTIM Master to trigger the ADC */
 sConfig_adctrig.Trigger = HRTIM_ADCTRIGGEREVENT13_MASTER_PERIOD;
 sConfig_adctrig.UpdateSource = HRTIM_ADCTRIGGERUPDATE_MASTER;

 HAL_HRTIM_ADCTriggerConfig(&HrtimHandle, HRTIM_ADCTRIGGER_1, &sConfig_adctrig);

 /*##-3- Configure the HRTIM TIMA ########################################################*/
 /* Initialize the HRTIM TIMA timing structure */
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  sConfig_time_base.Period = HRTIM_TIMA_PERIOD;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.RepetitionCounter = 0;
          
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, &sConfig_time_base);

  sConfig_timerA.DMARequests = HRTIM_TIM_DMA_NONE;
  sConfig_timerA.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  sConfig_timerA.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  sConfig_timerA.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  sConfig_timerA.DACSynchro = HRTIM_DACSYNC_NONE;
  sConfig_timerA.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  sConfig_timerA.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  sConfig_timerA.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  sConfig_timerA.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  sConfig_timerA.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  sConfig_timerA.InterruptRequests = HRTIM_TIM_IT_NONE;
  sConfig_timerA.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  sConfig_timerA.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  sConfig_timerA.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  sConfig_timerA.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  sConfig_timerA.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  sConfig_timerA.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  sConfig_timerA.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;

  HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, &sConfig_timerA);

  /* Configure the HRTIM TIMA compare unit */
  sConfig_compare.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  sConfig_compare.AutoDelayedTimeout = 0;
  sConfig_compare.CompareValue = HRTIM_TIMA_PULSE;

  HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &sConfig_compare);

  sConfig_output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  sConfig_output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  sConfig_output_config.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1;
  sConfig_output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  sConfig_output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  sConfig_output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  sConfig_output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &sConfig_output_config);

  /*##-3- DAC Configuration ##################################################################*/
  DAC_Configuration();

  /*##-4- ADC Configuration ##################################################################*/
  ADC_Configuration();

  /*##-5- Enable DAC Channel1 ################################################################*/
  if (HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  /* ### -6 - Start ADC conversion ############################################################ */
  if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-7- Start PWM signals generation ########################################################*/
  if (HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TA1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }

  /*##-8- Start HRTIM counter ###################################################################*/
  if (HAL_HRTIM_WaveformCounterStart(&HrtimHandle, HRTIM_TIMERID_MASTER + HRTIM_TIMERID_TIMER_A) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* ### -9 - Start DAC conversion ############################################################ */
  if (HAL_DACEx_TriangleWaveGenerate(&DacHandle, DAC_CHANNEL_1, DAC_TRIANGLEAMPLITUDE_4095) != HAL_OK)
  {
    Error_Handler();
  }
  /* Infinite loop */
  while (1)
  {
  }
}


/**
  * @brief  End of conversion callback in non blocking mode 
  * @param  hadc : hadc handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Read the converted value */
  uwConvertedValue = HAL_ADC_GetValue(hadc);
  
  __HAL_HRTIM_SetCompare(&HrtimHandle, 0, HRTIM_COMPAREUNIT_1, uwConvertedValue);
}
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
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  DAC Configuration
  * @param  None
  * @retval None
  */
void DAC_Configuration(void)
{
  DacHandle.Instance          = DAC1;

  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  sConfig_dac.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  sConfig_dac.DAC_Trigger = DAC_TRIGGER_HR1_TRGO1;
  sConfig_dac.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;


  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig_dac, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
}

/**
  * @brief  ADC Configuration
  * @param  None
  * @retval None
  */
void ADC_Configuration(void)
{
  AdcHandle.Instance          = ADC2;

  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC de-initialization Error */
    Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV2;          /* Asynchronous clock mode, input ADC clock divided by 2 */
  AdcHandle.Init.Resolution               = ADC_RESOLUTION_12B;            /* 16-bit resolution for converted data */
  AdcHandle.Init.ScanConvMode             = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoWait         = DISABLE;                       /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode       = ENABLE;                        /* Continuous mode enabled (automatic conversion restart after each conversion) */
  AdcHandle.Init.NbrOfConversion          = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode    = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion      = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_HR1_ADCTRG1;  /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_FALLING; /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;         /* DR register used as output (DMA mode disabled) */
  AdcHandle.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;         /* Left shift of final results */
  AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.OversamplingMode         = DISABLE;                       /* Oversampling disable */

  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig_adc.Channel          = ADC_CHANNEL_DAC1CH1_ADC2;        /* Select DAC1 internal channel */
  sConfig_adc.Rank             = ADC_REGULAR_RANK_1;              /* Rank of sampled channel number ADCx_CHANNEL */
  sConfig_adc.SamplingTime     = ADC_SAMPLETIME_810CYCLES_5;      /* Sampling time (number of clock cycles unit) */
  sConfig_adc.SingleDiff       = ADC_SINGLE_ENDED;                /* Single input channel */
  sConfig_adc.OffsetNumber     = ADC_OFFSET_NONE;                 /* No offset subtraction */ 
  sConfig_adc.Offset           = 0;                               /* Parameter discarded because offset correction is disabled */
  sConfig_adc.OffsetRightShift = DISABLE;                         /* No Right Offset Shift */
  sConfig_adc.OffsetSignedSaturation  = DISABLE;                  /* No Signed Saturation */

  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig_adc) != HAL_OK)
  {
    Error_Handler();
  }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

