/**
  ******************************************************************************
  * @file    COMP/COMP_OutputBlanking/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP
  *          peripheral in mode output blanking feature.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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

/** @addtogroup COMP_OutputBlanking
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef   CompxHandle;
TIM_HandleTypeDef    PWM_Handle;

__IO uint32_t State = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void COMPx_Config(void);
static void TIM1_Config(void);
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

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /******* Initialize LEDs available on NUCLEO-H7A3ZI-Q board ******************/

  /* configure COMPx : output redirected to TIM1 BKIN and blanking source is TIM1 OC5 */
  COMPx_Config();

  /* TIM1 Configuration in PWM mode */
  TIM1_Config();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure COMPx
  * @param  None
  * @retval None
  */
static void COMPx_Config(void)
{

  /*##-1- Configure the COMPx peripheral ###################################*/
  /* COMPx init: COMPx Output is connected to GPIO only (not redirected     */
  /* to timers) and output blanking source is connected to                  */
  /* TIM1 OC5.                                              */
  CompxHandle.Instance = COMPx;

  CompxHandle.Init.InvertingInput    = COMP_INPUT_MINUS_VREFINT;
  CompxHandle.Init.NonInvertingInput = COMP_INPUT_PLUS_IO1;
  CompxHandle.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  CompxHandle.Init.BlankingSrce      = COMP_BLANKINGSRC_TIM1_OC5;
  CompxHandle.Init.Mode              = COMP_POWERMODE_HIGHSPEED;
  CompxHandle.Init.Hysteresis        = COMP_HYSTERESIS_NONE;
  CompxHandle.Init.WindowMode        = COMP_WINDOWMODE_DISABLE;
  CompxHandle.Init.TriggerMode       = COMP_TRIGGERMODE_NONE;
  HAL_COMP_Init(&CompxHandle);

  /*##-2- Start the comparator process #####################################*/
  /* Enable COMPx: the higher threshold is set to VREFINT ~ 1.22 V */
  HAL_COMP_Start(&CompxHandle);
}

/**
  * @brief  TIM1 Configuration 2 and channel 5 in PWM mode
  * @note   TIM1 configuration is based on APB1 frequency
  * @note   TIM1 Update event occurs each SystemCoreClock/FREQ
  * @param  None
  * @retval None
  */
void TIM1_Config(void)
{
  TIM_OC_InitTypeDef    TIMPWM_Config;

  /*##-1- Configure the TIM peripheral #######################################*/
  /* Configure TIM1 */
  /* PWM configuration */
  PWM_Handle.Instance = TIM1;

  /* Time Base configuration: Channel 2 and channel 5 frequency is
     (APB2 clock * 2) / period = 280000000 / 50000 = 5600 Hz */
  PWM_Handle.Init.Period = 50000;
  PWM_Handle.Init.Prescaler = 0;
  PWM_Handle.Init.ClockDivision = 0;
  PWM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  PWM_Handle.Init.RepetitionCounter = 0;
  HAL_TIM_PWM_Init(&PWM_Handle);

  /*##-2- Configure the PWM Output Capture ########################################*/
  /* PWM Output Capture configuration of TIM1 channel 2 */
  /* Duty cycle is pulse/period = 100 * (37500 / 50000) =  75% */
  TIMPWM_Config.OCMode  = TIM_OCMODE_PWM1;
  TIMPWM_Config.OCIdleState = TIM_OCIDLESTATE_RESET;
  TIMPWM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  TIMPWM_Config.Pulse = 37500;
  TIMPWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
  TIMPWM_Config.OCNPolarity = TIM_OCNPOLARITY_LOW;
  TIMPWM_Config.OCFastMode = TIM_OCFAST_DISABLE;
  if(HAL_TIM_PWM_ConfigChannel(&PWM_Handle, &TIMPWM_Config, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM Output Capture ########################################*/
  /* PWM Output Capture configuration of TIM1 channel 5 */
  /* Channel 5 is an internal channel (not available on GPIO): */
  /* TIM1 OC5 is high during 2000 / 280000000 = 7.1 micro second */
  TIMPWM_Config.OCMode  = TIM_OCMODE_PWM1;
  TIMPWM_Config.OCIdleState = TIM_OCIDLESTATE_RESET;
  TIMPWM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  TIMPWM_Config.Pulse = 2000;
  TIMPWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
  TIMPWM_Config.OCNPolarity = TIM_OCNPOLARITY_LOW;
  TIMPWM_Config.OCFastMode = TIM_OCFAST_DISABLE;
  if(HAL_TIM_PWM_ConfigChannel(&PWM_Handle, &TIMPWM_Config, TIM_CHANNEL_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Enable TIM peripheral counter ######################################*/
  /* Start the TIM1 Channel 2 PWM */
  if(HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start the TIM1 Channel 5 PWM */
  if(HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_5) != HAL_OK)
  {
    Error_Handler();
  }
}

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
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
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

