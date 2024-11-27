/**
  ******************************************************************************
  * @file    TIM/TIM_Asymetric/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32H7xx TIM HAL API to generate
  *          an asymmetric signal.
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

/** @addtogroup TIM_Asymetric
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define INITIAL_PHASE         (uint32_t) 15000
#define INITIAL_LENGTH        (uint32_t) 5000
#define PWM_FREQUENCY         (uint32_t) 22000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle8;
TIM_HandleTypeDef    TimHandle1;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Master and slave configurations */
TIM_SlaveConfigTypeDef  sSlaveConfig;
TIM_MasterConfigTypeDef sMasterConfig;

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

  /*##-1- Configure the TIM peripheral #######################################*/ 
  /* --------------------------------------------------------------------------- 
     TIM8 is configured to generate an Asymmetric signal with a programmable 
   Phase-Shifted signal on TIM8_CH2:
   - TIM8 Channel 1 is configured in PWM2 mode
   - TIM8 Channel 2 is configured in Asymmetric PWM2 mode
   - The counter mode is center aligned mode
   - The pulse length and the phase shift are programmed consecutively in TIM8_CCR2 and TIM8_CCR1.
    
   TIM1 is configured to generating the reference signal on Channel1 used by TIM8:
    - TIM1 is generating a PWM signal with frequency equal to 4.54 KHz
    - TIM1 is used as master for TIM8, the update event of TIM1 genarates the Reset counter
      of TIM8 to synchronize the two signals: the reference signal (TIM1_CH1) and 
            the shifted signal (TIM8_CH2). 
          
    In this example TIM1 and TIM8 input clock (TIM18CLK) is set to APB2 clock (2*PCLK2)    
    TIM1 and TIM8 signals are at frequency of  ((SystemCoreClock/2) / (PWM_FREQUENCY + 1))  
       
    TIM8 is gerating a signal with the following caracteristics:
    - Pulse length = (TIM8_CCR1 + TIM8_CCR2) / TIM8_CLK
    - Phase shift = TIM8_CCR1/TIM8_CLK
      with TIM8_CLK = ((SystemCoreClock/2) / (Period + 1)), as the prescaler is equal to zero.
  
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32h7xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency     
  --------------------------------------------------------------------------- */
  /* Initialize Timers: TIM1 & TIM8 */  
  TimHandle1.Instance = TIM1;
  TimHandle1.Init.Prescaler         = 0;
  TimHandle1.Init.Period            = 2 * PWM_FREQUENCY;
  TimHandle1.Init.ClockDivision     = 0;
  TimHandle1.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle1.Init.RepetitionCounter = 0;
  if(HAL_TIM_PWM_Init(&TimHandle1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  TimHandle8.Instance = TIM8;
  TimHandle8.Init.Prescaler         = 0;
  TimHandle8.Init.Period            = PWM_FREQUENCY;
  TimHandle8.Init.ClockDivision     = 0;
  TimHandle8.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED1;
  TimHandle8.Init.RepetitionCounter = 0;
  if(HAL_TIM_PWM_Init(&TimHandle8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channels #########################################*/ 
  /* Channels 1&2 configuration on TIM8 */
  sConfig.OCMode = TIM_OCMODE_PWM2;
  sConfig.Pulse = INITIAL_PHASE;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;
  sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle8, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  sConfig.OCMode = TIM_OCMODE_ASSYMETRIC_PWM2;
  sConfig.Pulse = INITIAL_LENGTH;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle8, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Channel1 configuration on TIM1 */
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.Pulse = PWM_FREQUENCY;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle1, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*##-3- Configure master/slave mode and trigger ############################*/ 
  /* Synchronization between TIM1 and TIM8
  The aim is to generate a reference signal on TIM1_CH1
  The Phase-Shifted signal generated on TIM8_CH2 is compared to the reference
  signal */
  /* Configure TIM8 in slave mode: an active edge on  trigger input generates a 
  reset on TIM8 */
  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger     = TIM_TS_ITR0;
  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;
  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
  sSlaveConfig.TriggerFilter    = 0;
  if(HAL_TIM_SlaveConfigSynchronization(&TimHandle8, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  /* Configure TIM1 in master mode */
  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
  if(HAL_TIMEx_MasterConfigSynchronization(&TimHandle1, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /*##-4- Start PWM signals generation #######################################*/ 
  /* Start TIM1 channel 1 */
  if(HAL_TIM_PWM_Start(&TimHandle1, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* Start TIM8 channel 1 */
  if(HAL_TIM_PWM_Start(&TimHandle8, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* Start TIM8 channel 2 */
  if(HAL_TIM_PWM_Start(&TimHandle8, TIM_CHANNEL_2) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }

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

