/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_LowPower/Src/freertos.c
  * @author  MCD Application Team
  * @brief   Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Pre/Post sleep processing prototypes */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);

/* USER CODE BEGIN PREPOSTSLEEP */
/**
  * @brief  Pre Sleep Processing
  * @param  ulExpectedIdleTime: Expected time in idle state
  * @retval None
  */
void PreSleepProcessing(uint32_t * ulExpectedIdleTime)
{
  /* Called by the kernel before it places the MCU into a sleep mode because
  configPRE_SLEEP_PROCESSING() is #defined to PreSleepProcessing().

  NOTE:  Additional actions can be taken here to get the power consumption
  even lower.  For example, peripherals can be turned off here, and then back
  on again in the post sleep processing function.  For maximum power saving
  ensure all unused pins are in their lowest power state. */

  /*
    (*ulExpectedIdleTime) is set to 0 to indicate that PreSleepProcessing contains
    its own wait for interrupt or wait for event instruction and so the kernel vPortSuppressTicksAndSleep
    function does not need to execute the wfi instruction
  */
  *ulExpectedIdleTime = 0;

  /*Enter to sleep Mode using the HAL function HAL_PWR_EnterSLEEPMode with WFI instruction*/
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/**
  * @brief  Post Sleep Processing
  * @param  ulExpectedIdleTime: Not used
  * @retval None
  */
void PostSleepProcessing(uint32_t * ulExpectedIdleTime)
{
  /* Called by the kernel when the MCU exits a sleep mode because
  configPOST_SLEEP_PROCESSING is #defined to PostSleepProcessing(). */

  /* Avoid compiler warnings about the unused parameter. */
  (void) ulExpectedIdleTime;
}
/* USER CODE END PREPOSTSLEEP */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

