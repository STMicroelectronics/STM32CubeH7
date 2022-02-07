/**
  ******************************************************************************
  * @file    PWR/PWR_STOP_RTC/CM4/Src/main.c
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
#ifdef WAKEUP_TARGET_CM4
static void RTC_Config(void);
static void Error_Handler(void);
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* HW semaphore Clock enable */
  __HAL_RCC_HSEM_CLK_ENABLE();
  
  /* Configure the NVIC HSEM notification interrupt for CM4 */
  HAL_NVIC_SetPriority(HSEM2_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(HSEM2_IRQn);
  
  /* Activate HSEM notification for Cortex-M4 */
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  
  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Deactivate HSEM notification for Cortex-M4 */
  HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

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

  /* Initialize LED2 */
  BSP_LED_Init(LED2);

#ifdef WAKEUP_TARGET_CM4

  /* Enable CSI clock */
  __HAL_RCC_CSI_ENABLE();

  /* Set CSI as wakeup clock source for CM4 */
  HAL_RCCEx_WakeUpStopCLKConfig(RCC_STOP_WAKEUPCLOCK_CSI);

  /* Configure the RTC */
  RTC_Config();

  /* Configure the EXTI lines */
  HAL_EXTI_D2_EventInputConfig(EXTI_LINE19, EXTI_MODE_IT, ENABLE);
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE19, EXTI_MODE_IT | EXTI_MODE_EVT, DISABLE);

#endif /* WAKEUP_TARGET_CM4 */

  while(1)
  {
    /* Turn on LED2 */
    BSP_LED_On(LED2);

    /* Insert 3 seconds delay */
    HAL_Delay(3000);

#ifdef WAKEUP_TARGET_CM4

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
#endif /* WAKEUP_TARGET_CM4 */

    /* Clear STOP flag */
    __HAL_PWR_CLEAR_FLAG(PWR_CPU_FLAGS);

    /* Clear CM4 EXTI RTC wakeup timer interrupt before entering STOP mode */
    __HAL_RTC_WAKEUPTIMER_EXTID2_CLEAR_FLAG();

    /* Enter D2 and D3 domains to DStop mode */
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_STOP) == RESET)
    {
      /* Turn Off LED2 */
      BSP_LED_Off(LED2);

      /* Enter D3 domain to DStandby mode */
      HAL_PWREx_EnterSTANDBYMode(PWR_D3_DOMAIN);

      /* Enter D2 domain to DStop mode */
      HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D2_DOMAIN);

      /* Call HAL_InitTick function after wakeup from STOP mode to re-configure the source
         of the time base for CM4 since the system clock is now HSI or CSI if selected */
      HAL_InitTick(TICK_INT_PRIORITY);
    }
  }
}

#ifdef WAKEUP_TARGET_CM4
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

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);

  while(1)
  {
  }
}
#endif




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

