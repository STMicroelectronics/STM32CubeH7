/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_MultiplePWM/Src/main.c 
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

/** @addtogroup HRTIM_MultiplePWM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HRTIM_MAX_PERIOD  100
#define HRTIM_TIMA_PULSE1 13
#define HRTIM_TIMB_PULSE1 26
#define HRTIM_TIMC_PULSE1 38
#define HRTIM_TIMD_PULSE1 49
#define HRTIM_TIME_PULSE1 63

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
HRTIM_HandleTypeDef                 HrtimHandle;

HRTIM_TimeBaseCfgTypeDef            sConfig_time_base;
HRTIM_SimplePWMChannelCfgTypeDef    sConfig_Channel;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
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

  /* Configure the MPU attributes */
  MPU_Config();

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

  /*##-2- Configure the HRTIM TIMA PWM channels 1 #############################################*/
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  sConfig_time_base.Period = HRTIM_MAX_PERIOD;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.RepetitionCounter = 0;
          
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, &sConfig_time_base);         
          
  sConfig_Channel.Polarity = HRTIM_OUTPUTPOLARITY_LOW;
  sConfig_Channel.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_Channel.Pulse = HRTIM_TIMA_PULSE1;
          
  HAL_HRTIM_SimplePWMChannelConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                    HRTIM_OUTPUT_TA1, &sConfig_Channel);

  /*##-3- Configure the HRTIM TIMB PWM channels 1 #############################################*/
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  sConfig_time_base.Period = HRTIM_MAX_PERIOD;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.RepetitionCounter = 0;
          
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, &sConfig_time_base);         
          
  sConfig_Channel.Polarity = HRTIM_OUTPUTPOLARITY_LOW;
  sConfig_Channel.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_Channel.Pulse = HRTIM_TIMB_PULSE1;
          
  HAL_HRTIM_SimplePWMChannelConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                    HRTIM_OUTPUT_TB1, &sConfig_Channel);

  /*##-4- Configure the HRTIM TIMC PWM channels 1 #############################################*/
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  sConfig_time_base.Period = HRTIM_MAX_PERIOD;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.RepetitionCounter = 0;
          
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, &sConfig_time_base);         
          
  sConfig_Channel.Polarity = HRTIM_OUTPUTPOLARITY_LOW;
  sConfig_Channel.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_Channel.Pulse = HRTIM_TIMC_PULSE1;
          
  HAL_HRTIM_SimplePWMChannelConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, 
                                    HRTIM_OUTPUT_TC1, &sConfig_Channel);

  /*##-5- Configure the HRTIM TIMD PWM channels 1 #############################################*/
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  sConfig_time_base.Period = HRTIM_MAX_PERIOD;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.RepetitionCounter = 0;
          
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, &sConfig_time_base);         

  sConfig_Channel.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  sConfig_Channel.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_Channel.Pulse = HRTIM_TIMD_PULSE1;
          
  HAL_HRTIM_SimplePWMChannelConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, 
                                    HRTIM_OUTPUT_TD1, &sConfig_Channel);

  /*##-6- Configure the HRTIM TIME PWM channels 1 #############################################*/
  sConfig_time_base.Mode = HRTIM_MODE_CONTINUOUS;
  sConfig_time_base.Period = HRTIM_MAX_PERIOD;
  sConfig_time_base.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
  sConfig_time_base.RepetitionCounter = 0;
          
  HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_E, &sConfig_time_base);         
          
  sConfig_Channel.Polarity = HRTIM_OUTPUTPOLARITY_LOW;
  sConfig_Channel.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  sConfig_Channel.Pulse = HRTIM_TIME_PULSE1;
          
  HAL_HRTIM_SimplePWMChannelConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_E, 
                                    HRTIM_OUTPUT_TE1, &sConfig_Channel);

  /*##-7- Start PWM signals generation #########################################################*/
  if (HAL_HRTIM_SimplePWMStart(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }     
  if (HAL_HRTIM_SimplePWMStart(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  if (HAL_HRTIM_SimplePWMStart(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, HRTIM_OUTPUT_TC1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  if (HAL_HRTIM_SimplePWMStart(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  if (HAL_HRTIM_SimplePWMStart(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_E, HRTIM_OUTPUT_TE1) != HAL_OK)
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
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
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

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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

