/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_FaultEvent/Src/main.c 
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

/** @addtogroup HRTIM_FaultEvent
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HRTIM_TIMA_PERIOD  0xFFFF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
HRTIM_HandleTypeDef                 HrtimHandle;

HRTIM_TimeBaseCfgTypeDef            sConfig_time_base;
HRTIM_TimerCfgTypeDef               sConfig_timerA;
HRTIM_OutputCfgTypeDef              sConfig_output_config;
HRTIM_CompareCfgTypeDef             sConfig_compare;
HRTIM_FaultCfgTypeDef               sConfig_fault2;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

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

  /*##-2- Configure the HRTIM TIMA PWM channels 1 & 2 #########################################*/
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
  sConfig_timerA.FaultEnable = HRTIM_TIMFAULTENABLE_FAULT2;
  sConfig_timerA.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  sConfig_timerA.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  sConfig_timerA.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  sConfig_timerA.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  sConfig_timerA.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;

  HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A,&sConfig_timerA);

  sConfig_compare.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  sConfig_compare.AutoDelayedTimeout = 0;
  sConfig_compare.CompareValue = HRTIM_TIMA_PERIOD/2;

  HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &sConfig_compare);

  sConfig_compare.CompareValue = HRTIM_TIMA_PERIOD/4;
  HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, &sConfig_compare);

  sConfig_output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  sConfig_output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  sConfig_output_config.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1;
  sConfig_output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  sConfig_output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  sConfig_output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  sConfig_output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &sConfig_output_config);

  sConfig_output_config.ResetSource = HRTIM_OUTPUTRESET_TIMPER;
  sConfig_output_config.SetSource = HRTIM_OUTPUTSET_TIMCMP2;

  HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &sConfig_output_config);

  /*##-3- Fault Event configuration   #########################################################*/
/* Fault conditionning configuration */
  sConfig_fault2.Source = HRTIM_FAULTSOURCE_DIGITALINPUT;
  sConfig_fault2.Polarity = HRTIM_FAULTPOLARITY_HIGH;
  sConfig_fault2.Filter = HRTIM_FAULTFILTER_NONE;
  sConfig_fault2.Lock = HRTIM_FAULTLOCK_READWRITE;
  
  HAL_HRTIM_FaultPrescalerConfig(&HrtimHandle, HRTIM_FAULTPRESCALER_DIV1);
  
  HAL_HRTIM_FaultConfig(&HrtimHandle, HRTIM_FAULT_2, &sConfig_fault2);
  
  /* Enable Fault2 input */
  HAL_HRTIM_FaultModeCtl(&HrtimHandle, HRTIM_FAULT_2, HRTIM_FAULTMODECTL_ENABLED);

  /*##-4- Start PWM signals generation ########################################################*/
  if (HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }

/*##-5- Start HRTIM counter ###################################################################*/
  if (HAL_HRTIM_WaveformCounterStart(&HrtimHandle, HRTIM_TIMERID_TIMER_A) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* Infinite loop */
  while (1)
  {
  }
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

