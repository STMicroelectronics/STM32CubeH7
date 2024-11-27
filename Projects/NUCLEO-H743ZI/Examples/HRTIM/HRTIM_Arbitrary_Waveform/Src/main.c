/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_Arbitary_Waveform/Src/main.c 
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

/** @addtogroup HRTIM_Arbitary_Waveform
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HRTIM_MAX_PERIOD  0xFFFF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
HRTIM_HandleTypeDef                 HrtimHandle;

HRTIM_TimeBaseCfgTypeDef            sConfig_time_base;
HRTIM_TimerCfgTypeDef               sConfig_timerD;
HRTIM_OutputCfgTypeDef              sConfig_output_config;
HRTIM_CompareCfgTypeDef             sConfig_compare;
	
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
  /* Initialize the HRTIM structure (minimal configuration) */
  HrtimHandle.Instance = HRTIM1;
  HrtimHandle.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
  HrtimHandle.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;

  /* Initialize HRTIM */
  HAL_HRTIM_Init(&HrtimHandle);

  /*##-2- Configure the HRTIM TIMA PWM channels 1 & 2 #########################################*/
  sConfig_time_base.Period = HRTIM_MAX_PERIOD;
  sConfig_time_base.RepetitionCounter = 0;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, &sConfig_time_base);
 
  /* TIMERD global configuration: preload enabled on REP event */
  sConfig_timerD.DMARequests = HRTIM_TIM_DMA_NONE;
  sConfig_timerD.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  sConfig_timerD.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  sConfig_timerD.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  sConfig_timerD.DACSynchro = HRTIM_DACSYNC_NONE;
  sConfig_timerD.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  sConfig_timerD.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  sConfig_timerD.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  sConfig_timerD.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  sConfig_timerD.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  sConfig_timerD.InterruptRequests = HRTIM_TIM_IT_NONE;
  sConfig_timerD.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  sConfig_timerD.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  sConfig_timerD.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  sConfig_timerD.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  sConfig_timerD.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  sConfig_timerD.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  sConfig_timerD.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	
  HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D,&sConfig_timerD);
 
  /* TD1 waveform description: toggles on TIMD period, CMP1 and CMP2 event */
  sConfig_output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  sConfig_output_config.SetSource = HRTIM_OUTPUTSET_TIMPER + HRTIM_OUTPUTSET_TIMCMP1 + HRTIM_OUTPUTSET_TIMCMP2;
  sConfig_output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMPER + HRTIM_OUTPUTRESET_TIMCMP1 + HRTIM_OUTPUTRESET_TIMCMP2;
  sConfig_output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  sConfig_output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  sConfig_output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  sConfig_output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	
  HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, &sConfig_output_config);

  /* TD2 waveform description: set on TIMD PER and CMP2 and */
  /* reset on TIMD CMP1 and CMP3 event                      */
  sConfig_output_config.SetSource = HRTIM_OUTPUTSET_TIMPER + HRTIM_OUTPUTSET_TIMCMP2;
  sConfig_output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1 + HRTIM_OUTPUTRESET_TIMCMP3;
	
  HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD2, &sConfig_output_config);

  /* Set compare register 1 in Timer D */
  sConfig_compare.CompareValue = HRTIM_MAX_PERIOD/4;     /* 25% of the period */
  HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1, &sConfig_compare);

  /* Set compare register 2 in Timer D */
  sConfig_compare.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  sConfig_compare.AutoDelayedTimeout = 0;
  sConfig_compare.CompareValue = (3*HRTIM_MAX_PERIOD)/8;     /* 37.5% of the period */
  HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_2, &sConfig_compare);

  /* Set compare register 3 in Timer D */
  sConfig_compare.CompareValue = HRTIM_MAX_PERIOD/2;     /* 50% of the period */
  HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_3, &sConfig_compare);
			
  /*##-7- Start Waveform signals generation ########################################################*/
  if (HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2) != HAL_OK)
  {
    /* Waveform Generation Error */
    Error_Handler();
  }
	if (HAL_HRTIM_WaveformCounterStart(&HrtimHandle, HRTIM_TIMERID_TIMER_D) != HAL_OK)
  {
    /* Waveform Generation Error */
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
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
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
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
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

