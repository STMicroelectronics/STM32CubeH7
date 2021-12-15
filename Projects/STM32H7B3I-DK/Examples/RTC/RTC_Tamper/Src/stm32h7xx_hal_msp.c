/**
  ******************************************************************************
  * @file    RTC/RTC_Tamper/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/** @addtogroup stm32h7xx_HAL_Examples
  * @{
  */

/** @defgroup RTC_Tamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t RtcClockSource
#ifdef RTC_CLOCK_SOURCE_LSE
  = RCC_RTCCLKSOURCE_LSE;
#elif defined (RTC_CLOCK_SOURCE_LSI)
  = RCC_RTCCLKSOURCE_LSI;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  *
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

  /*##-1- Enables access to the backup domain ################################*/
  /* To enable access on RTC registers */
  HAL_PWR_EnableBkUpAccess();

  /*##-2- Get RTC clock configuration ########################################*/
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  /*##-3- Configure LSE/LSI as RTC clock source ##############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	
#elif defined (RTC_CLOCK_SOURCE_LSI)
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
#else
#error Please select the RTC Clock source inside the main.h file
#endif /*RTC_CLOCK_SOURCE_LSE*/

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    PeriphClkInitStruct.RTCClockSelection = RtcClockSource;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /*##-4- Configure the NVIC for RTC Tamper ##################################*/
  HAL_NVIC_SetPriority(RTC_TAMP_STAMP_CSS_LSE_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(RTC_TAMP_STAMP_CSS_LSE_IRQn);
}

/**
  * @brief RTC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

