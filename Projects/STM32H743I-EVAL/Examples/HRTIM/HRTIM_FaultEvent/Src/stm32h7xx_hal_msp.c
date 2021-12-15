/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_FaultEvent/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void HRTIM_CHANNEL_GPIO_PORT(void);

/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */


/**
  * @brief HRTIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_HRTIM_MspInit(HRTIM_HandleTypeDef * hhrtim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* HRTIM1 Peripheral clock enable */
  HRTIM_CLK_ENABLE();

  /* Enable GPIO Channels Clock */
  HRTIM_CHANNEL_GPIO_PORT();

  /* Configure HRTIM1 pins as alternate function mode
  */
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Alternate = HRTIM_TIMA_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = HRTIM_TIMA_CHANNEL_PIN1;
  HAL_GPIO_Init(HRTIM_TIMA_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = HRTIM_TIMA_GPIO_AF_CHANNEL2;
  GPIO_InitStruct.Pin = HRTIM_TIMA_CHANNEL_PIN2;
  HAL_GPIO_Init(HRTIM_TIMA_CHANNEL2_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = GPIO_AF2_HRTIM1;
  GPIO_InitStruct.Pin = HRTIM_FAULT2_GPIO_PIN;
  HAL_GPIO_Init(HRTIM_FAULT2_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  HRTIM GPIO pins clock enable
  * @param  None
  * @retval None
  */
static void HRTIM_CHANNEL_GPIO_PORT(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();
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

