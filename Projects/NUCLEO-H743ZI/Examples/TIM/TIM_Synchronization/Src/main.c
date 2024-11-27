/**
  ******************************************************************************
  * @file    TIM/TIM_Synchronization/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to command 2 Timers as slaves (TIM3 & TIM4)
  *          using a Timer as master (TIM1)
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

/** @addtogroup TIM_Synchronization
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer1 handler declaration: Master */
TIM_HandleTypeDef         TimMasterHandle;

/* Timer3 handler declaration: Slave1 */
TIM_HandleTypeDef         TimSlave1Handle;

/* Timer4 handler declaration: Slave2 */
TIM_HandleTypeDef         TimSlave2Handle;

/* Output compare structure */
TIM_OC_InitTypeDef        sOCConfig;

/* Master configuration structure */
TIM_MasterConfigTypeDef   sMasterConfig;

/* Slave configuration structure */
TIM_SlaveConfigTypeDef   sSlaveConfig;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Timers Configuration */
  /* ---------------------------------------------------------------------------
    TIM1 and Timers(TIM3 and TIM4) synchronisation in parallel mode.
     1/TIM1 is configured as Master Timer:
         - PWM Mode is used
         - The TIM1 Update event is used as Trigger Output

     2/TIM3 and TIM4 are slaves for TIM1,
         - PWM Mode is used
         - The ITR0(TIM1) is used as input trigger for both slaves
         - Gated mode is used, so starts and stops of slaves counters
           are controlled by the Master trigger output signal(update event).

    In this example TIM1 input clock (TIM1CLK) is set to APB2 clock DIV2 (PCLK2),
    since APB2 prescaler is different from 1.
      TIM1CLK = 2 * APB2 clock
      => TIM1CLK = 2*PCLK2

    The TIM1 counter clock is equal to 2*PCLK2 = 200 MHz.

    The Master Timer TIM1 is running at:
    TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 781.250 KHz
    TIM1_Period = (TIM1 counter clock / TIM1 frequency) - 1 = 182
    and the duty cycle is equal to: TIM1_CCR1/(TIM1_ARR + 1) = 50%

    The TIM3 is running at:
    (TIM1 frequency)/ ((TIM3 period +1)* (Repetition_Counter+1)) = 52.083 KHz and
    a duty cycle equal to TIM3_CCR1/(TIM3_ARR + 1) = 33.3%

    The TIM4 is running at:
    (TIM1 frequency)/ ((TIM4 period +1)* (Repetition_Counter+1)) = 78.125 KHz and
    a duty cycle equal to TIM4_CCR1/(TIM4_ARR + 1) = 50%

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in SystemClock_Config().
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  --------------------------------------------------------------------------- */

  /* Set Timers instance */
  TimMasterHandle.Instance = TIM1;
  TimSlave1Handle.Instance = TIM3;
  TimSlave2Handle.Instance = TIM4;

  /*====================== Master configuration : TIM1 =======================*/
  /* Initialize TIM1 peripheral in PWM mode*/
  TimMasterHandle.Init.Period            = 255;
  TimMasterHandle.Init.Prescaler         = 0;
  TimMasterHandle.Init.ClockDivision     = 0;
  TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimMasterHandle.Init.RepetitionCounter = 4;
  if (HAL_TIM_PWM_Init(&TimMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the PWM_channel_1  */
  sOCConfig.OCMode       = TIM_OCMODE_PWM1;
  sOCConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sOCConfig.Pulse        = 128;
  sOCConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sOCConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sOCConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sOCConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&TimMasterHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure TIM1 as master & use the update event as Trigger Output (TRGO) */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&TimMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*================== End of Master configuration : TIM1 ====================*/


  /*====================== Slave1 configuration : TIM3 =======================*/
  /* Initialize TIM3 peripheral in PWM mode*/
  TimSlave1Handle.Init.Period            = 2;
  TimSlave1Handle.Init.Prescaler         = 0;
  TimSlave1Handle.Init.ClockDivision     = 0;
  TimSlave1Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimSlave1Handle.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&TimSlave1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the PWM_channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_PWM1;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sOCConfig.Pulse = 1;
  if (HAL_TIM_PWM_ConfigChannel(&TimSlave1Handle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure TIM3 in Gated slave mode &
  use the Internal Trigger 0 (ITR0) as trigger source */
  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger     = TIM_TS_ITR0;
  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;
  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
  sSlaveConfig.TriggerFilter    = 0;
  if (HAL_TIM_SlaveConfigSynchronization(&TimSlave1Handle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*================== End of Slave1 configuration : TIM3 ====================*/


  /*====================== Slave2 configuration : TIM4 =======================*/
  /* Initialize TIM4 peripheral in PWM mode*/
  TimSlave2Handle.Init.Period            = 1;
  TimSlave2Handle.Init.Prescaler         = 0;
  TimSlave2Handle.Init.ClockDivision     = 0;
  TimSlave2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimSlave2Handle.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&TimSlave2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the PWM_channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_PWM1;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sOCConfig.Pulse = 1;
  if (HAL_TIM_PWM_ConfigChannel(&TimSlave2Handle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure TIM3 in Gated slave mode &
  use the Internal Trigger 0 (ITR0) as trigger source */
  sSlaveConfig.SlaveMode     = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger  = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchronization(&TimSlave2Handle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*================== End of Slave2 configuration : TIM4 ====================*/


  /* Start Master PWM generation */
  if (HAL_TIM_PWM_Start(&TimMasterHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM generation Error */
    Error_Handler();
  }

  /* Start Slave1 PWM generation */
  if (HAL_TIM_PWM_Start(&TimSlave1Handle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM generation Error */
    Error_Handler();
  }
  /* Start Slave2 PWM generation */
  if (HAL_TIM_PWM_Start(&TimSlave2Handle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM generation Error */
    Error_Handler();
  }

  while (1)
  {
  }

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

