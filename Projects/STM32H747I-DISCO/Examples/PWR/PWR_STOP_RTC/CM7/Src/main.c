/**
  ******************************************************************************
  * @file    PWR/PWR_STOP_RTC/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32H7xx PWR HAL API to enter
  *          and exit the stop mode with RTC.
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

/** @addtogroup PWR_STOP_RTC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void SYSCLKConfig_STOP(void);

#if defined (WAKEUP_TARGET_CM7) || defined (WAKEUP_TARGET_ALL_CPU)
static void RTC_Config(void);
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7) 
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */

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

  /* When system initialization is finished, Cortex-M7 will release (wakeup) Cortex-M4  by means of 
     HSEM notification. Cortex-M4 release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).
  */

  /* HW semaphore Clock enable */
  __HAL_RCC_HSEM_CLK_ENABLE();

  /* Take HSEM */
  HAL_HSEM_FastTake(HSEM_ID_0);  
  
  /* Release HSEM in order to notify the CPU2(CM4) */     
  HAL_HSEM_Release(HSEM_ID_0,0);
  
  /* After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
     the access will not be safe until “clock ready flag” of such a domain is set (by hardware).  */
  while(!__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY));

  /* Initialize LED1 */
  BSP_LED_Init(LED1);

  /* Initialize LED3 for error handler */
  BSP_LED_Init(LED3);

#if defined (WAKEUP_TARGET_CM7)
  
  /* Configure the RTC */
  RTC_Config();
  
  /* Configure the EXTI lines */
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE19, EXTI_MODE_IT, ENABLE);
  HAL_EXTI_D2_EventInputConfig(EXTI_LINE19, EXTI_MODE_IT | EXTI_MODE_EVT, DISABLE);
  
#endif /* WAKEUP_TARGET_CM7 */

#if defined (WAKEUP_TARGET_ALL_CPU)

  /* Configure the RTC */
  RTC_Config();
  
  /* Configure the EXTI lines */
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE19, EXTI_MODE_IT, ENABLE);
  HAL_EXTI_D2_EventInputConfig(EXTI_LINE19, EXTI_MODE_IT, ENABLE);

#endif /* WAKEUP_TARGET_ALL_CPU */
  
  /* Allocate the Flash memory to the CM4 to be able to execute code when CM7 enter STOP mode */
  __HAL_RCC_FLASH_C2_ALLOCATE();

  while (1)
  {
    /* Turn LED1 on */
    BSP_LED_On(LED1);

    /* Insert 5 second delay */
    HAL_Delay(5000);

#if defined (WAKEUP_TARGET_CM7) || defined (WAKEUP_TARGET_ALL_CPU)

    /* The Following Wakeup sequence is highly recommended prior to each Standby mode entry
         mainly  when using more than one wakeup source this is to not miss any wakeup event.
          - Disable all used wakeup sources,
          - Clear all related wakeup flags,
          - Re-enable all used wakeup sources,
          - Enter the Standby mode.
    */
    /* Disable Wakeup Counter */
    HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);

    /* Set the Wake up time */
    /* RTC Wakeup Interrupt Generation:
       Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI))
       Wakeup Time = Wakeup Time Base * WakeUpCounter
       = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI)) * WakeUpCounter
        ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

      To configure the wake up timer to 20s the WakeUpCounter is set to 0x9C40:
      RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16
      Wakeup Time Base = 16 /(32.000KHz) = 0,5 ms
      Wakeup Time = 20s = 0,5ms  * WakeUpCounter
        ==> WakeUpCounter = 20s/0,5ms = 40000 = 0x9C40 */
    HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0x9C40, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

#endif /* WAKEUP_TARGET_CM7 || WAKEUP_TARGET_ALL_CPU */

    /* Clear STOP flag */
    __HAL_PWR_CLEAR_FLAG(PWR_CPU_FLAGS);

    /* Enter D2 and D3 domains to DStop mode */
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_STOP) == RESET)
    {
      /* Turn LED1 OFF  */
      BSP_LED_Off(LED1);

      /* Enter D1 to DStop Mode */
      HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D1_DOMAIN);
    }   

    /* Configures system clock after wake-up from STOP: enable HSE, PLL and select
    PLL as system clock source (HSE and PLL are disabled in STOP mode) */
    SYSCLKConfig_STOP();

  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Domain1 frequency)
  *            HCLK(Hz)                       = 20000000  (Domain2 frequency)
  *            D1AHB Prescaler                = 1
  *            APB3 Prescaler                 = 1
  *            D2APB1 Prescaler               = 1
  *            APB2 Prescaler                 = 1
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
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t pFLatency = 0;

 __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
 while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType     = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource  = RCC_SYSCLKSOURCE_PLLCLK;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
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

  while(1)
  {
  }
}

#if defined (WAKEUP_TARGET_CM7) || defined (WAKEUP_TARGET_ALL_CPU)

/**
  * @brief  RTC Configuration
  *         RTC Clocked by LSI (see HAL_RTC_MspInit)
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Configure RTC */
  RTCHandle.Instance = RTC;
  /* Configure RTC prescaler and RTC data registers as follow:
    - Hour Format = Format 24
    - Asynch Prediv = Value according to source clock
    - Synch Prediv = Value according to source clock
    - OutPut = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType = Open Drain */
  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
#endif


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
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

