/**
  ******************************************************************************
  * @file    ADC/ADC_DAC_Interconnect/Src/main.c
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

/** @addtogroup ADC_DAC_Interconnect
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handle declaration */
ADC_HandleTypeDef        AdcHandle;
/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef   sAdcConfig;
/* ADC Analog WatchDog configuration structure declaration */
ADC_AnalogWDGConfTypeDef sAnalogWDGConf;

/* DAC handle declaration */
DAC_HandleTypeDef        DacHandle;
/* DAC channel configuration structure declaration */
DAC_ChannelConfTypeDef   sDacConfig;

/* Converted value declaration */
uint32_t                 uwConvertedValue;
/* Input voltage declaration */
uint32_t                 uwInputVoltage;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void DAC_Config(void);
static void HRTIM_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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

  /* This sample code shows how to use STM32H7xx ADC HAL API to connect DAC to ADC
     and setup Analog Watchdog on signal level.
     To proceed, 5 steps are required. */

  /* Turn LED1/LED2/LED3 on */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* ### - 0 - Configure DAC peripheral #################################### */
  DAC_Config();

  /* ### - 1 - Initialize ADC peripheral #################################### */
  AdcHandle.Instance          = ADC2;

  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC de-initialization Error */
    Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV2;          /* Asynchronous clock mode, input ADC clock divided by 2*/
  AdcHandle.Init.Resolution               = ADC_RESOLUTION_16B;            /* 16-bit resolution for converted data */
  AdcHandle.Init.ScanConvMode             = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoWait         = DISABLE;                       /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode       = ENABLE;                        /* Continuous mode enabled (automatic conversion restart after each conversion) */
  AdcHandle.Init.NbrOfConversion          = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode    = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion      = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv         = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;         /* DR register used as output (DMA mode disabled) */
  AdcHandle.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;         /* Left shift of final results */
  AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.OversamplingMode         = DISABLE;                       /* Oversampling disable */

  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 3 - Channel configuration ######################################## */
  sAdcConfig.Channel                = ADC_CHANNEL_DAC1CH1_ADC2;   /* Select DAC1 internal channel */
  sAdcConfig.Rank                   = ADC_REGULAR_RANK_1;         /* Rank of sampled channel number ADCx_CHANNEL */
  sAdcConfig.SamplingTime           = ADC_SAMPLETIME_810CYCLES_5; /* Sampling time (number of clock cycles unit) */
  sAdcConfig.SingleDiff             = ADC_SINGLE_ENDED;           /* Single input channel */
  sAdcConfig.OffsetNumber           = ADC_OFFSET_NONE;            /* No offset subtraction */ 
  sAdcConfig.Offset                 = 0;                          /* Parameter discarded because offset correction is disabled */
  sAdcConfig.OffsetRightShift       = DISABLE;                    /* No Right Offset Shift */
  sAdcConfig.OffsetSignedSaturation = DISABLE;                    /* No Signed Saturation */
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sAdcConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 4 - Analog Watchdog configuration ################################ */
  sAnalogWDGConf.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
  sAnalogWDGConf.WatchdogMode   = ADC_ANALOGWATCHDOG_SINGLE_REG;
  sAnalogWDGConf.Channel        = ADC_CHANNEL_DAC1CH1_ADC2;
  sAnalogWDGConf.ITMode         = ENABLE;
  sAnalogWDGConf.HighThreshold  = 0x5000;
  sAnalogWDGConf.LowThreshold   = 0x0000;
  
  HAL_ADC_AnalogWDGConfig(&AdcHandle, &sAnalogWDGConf);
  
  sAnalogWDGConf.WatchdogNumber = ADC_ANALOGWATCHDOG_2;
  sAnalogWDGConf.WatchdogMode   = ADC_ANALOGWATCHDOG_SINGLE_REG;
  sAnalogWDGConf.Channel        = ADC_CHANNEL_DAC1CH1_ADC2;
  sAnalogWDGConf.ITMode         = ENABLE;
  sAnalogWDGConf.HighThreshold  = 0xFFFF;
  sAnalogWDGConf.LowThreshold   = 0x5000;
  
  HAL_ADC_AnalogWDGConfig(&AdcHandle, &sAnalogWDGConf);

  /* ### - 5 - Start conversion ############################################# */
  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Infinite Loop */
  while (1)
  {    
  }
}

/**
  * @brief  Analog watchdog callback in non blocking mode 
  * @param  hadc : hadc handle
  * @retval None
  */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  /* Read the converted value */
  uwConvertedValue = HAL_ADC_GetValue(&AdcHandle);

  /* Convert the result from 16 bit value to the voltage dimension (mV unit) */
  /* Vref = 3.3 V */
  uwInputVoltage = (uwConvertedValue * 3300) / 0xFFFF;

  BSP_LED_On(LED2);
  BSP_LED_Off(LED1);
}


/**
  * @brief  Analog watchdog callback in non blocking mode 
  * @param  hadc : hadc handle
  * @retval None
  */
void HAL_ADCEx_LevelOutOfWindow2Callback(ADC_HandleTypeDef* hadc)
{
  /* Read the converted value */
  uwConvertedValue = HAL_ADC_GetValue(&AdcHandle);

  /* Convert the result from 16 bit value to the voltage dimension (mV unit) */
  /* Vref = 3.3 V */
  uwInputVoltage = (uwConvertedValue * 3300) / 0xFFFF;

  BSP_LED_On(LED1);
  BSP_LED_Off(LED2);
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

static void DAC_Config(void)
{
  DacHandle.Instance = DAC1;
 
  /*##-0- DeInit the DAC peripheral ##########################################*/
  if (HAL_DAC_DeInit(&DacHandle) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }
  
  /*##-1- Init the DAC peripheral #######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the HRTIM peripheral #######################################*/
  HRTIM_Config();
   
  /*##-3- Configure DAC channel1 #############################################*/
  sDacConfig.DAC_Trigger                 = DAC_TRIGGER_HR1_TRGO1;
  sDacConfig.DAC_OutputBuffer            = DAC_OUTPUTBUFFER_DISABLE;
  sDacConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  
  if (HAL_DAC_ConfigChannel(&DacHandle, &sDacConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-4- Set DAC Channels DHR register ######################################*/
  if (HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0xFF) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }

  /* Triangle wave generation */
  if (HAL_DACEx_TriangleWaveGenerate(&DacHandle, DAC_CHANNEL_1, DAC_TRIANGLEAMPLITUDE_2047) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-5- Enable DAC Channel1 ################################################*/
  if (HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
}

/**
  * @brief  HRTIM Configuration
  * @param  None
  * @retval None
  */
void HRTIM_Config(void)
{
  HRTIM_HandleTypeDef hhrtim;
  HRTIM_TimeBaseCfgTypeDef time_base_config_master;
  HRTIM_TimerCfgTypeDef timer_config_master;
  /*##-1- Configure the HRTIM peripheral #######################################*/
  
  /* HRTIM Initialization */
  hhrtim.Instance = (HRTIM_TypeDef *)NULL;
  
  hhrtim.hdmaMaster = (DMA_HandleTypeDef *)NULL;    
  hhrtim.hdmaTimerA = (DMA_HandleTypeDef *)NULL;     
  hhrtim.hdmaTimerB = (DMA_HandleTypeDef *)NULL;  
  hhrtim.hdmaTimerC = (DMA_HandleTypeDef *)NULL;  
  hhrtim.hdmaTimerD = (DMA_HandleTypeDef *)NULL;  
  hhrtim.hdmaTimerE = (DMA_HandleTypeDef *)NULL;
    
  hhrtim.Instance = HRTIM1;
  hhrtim.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
  HAL_HRTIM_Init(&hhrtim);

  /* Time base master configuration */
  time_base_config_master.Mode = HRTIM_MODE_CONTINUOUS;
  time_base_config_master.Period = 0xFFFF;
  time_base_config_master.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  time_base_config_master.RepetitionCounter = 0xF;
  
  HAL_HRTIM_TimeBaseConfig(&hhrtim,HRTIM_TIMERINDEX_MASTER,&time_base_config_master);
                           

  /* Master Time configuration */
  timer_config_master.InterruptRequests = HRTIM_MASTER_IT_MUPD;
  timer_config_master.DMARequests = HRTIM_TIM_DMA_NONE;
  timer_config_master.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  timer_config_master.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config_master.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config_master.DACSynchro = HRTIM_DACSYNC_DACTRIGOUT_1;
  timer_config_master.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  timer_config_master.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config_master.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config_master.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  timer_config_master.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config_master.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  timer_config_master.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config_master.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config_master.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config_master.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config_master.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  timer_config_master.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  
  HAL_HRTIM_WaveformTimerConfig(&hhrtim,HRTIM_TIMERINDEX_MASTER,&timer_config_master);
  

  /*##-2- Enable HRTIM peripheral counter ######################################*/
  HAL_HRTIM_WaveformCounterStart(&hhrtim, HRTIM_TIMERID_MASTER);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
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

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
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

