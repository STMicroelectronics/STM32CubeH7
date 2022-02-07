/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_MultiplePWM/Src/stm32h7xx_hal_msp.c
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

  /* Configure HRTIMA TIMA TA1/A2, TIMB TB1/2, TIMC TC1/2, TIMD TD1/2 and TIME TE1.2
   channels as alternate function mode */
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Alternate = HRTIM_TIMA_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = HRTIM_TIMA_CHANNEL_PIN1;
  HAL_GPIO_Init(HRTIM_TIMA_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = HRTIM_TIMB_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = HRTIM_TIMB_CHANNEL_PIN1;
  HAL_GPIO_Init(HRTIM_TIMB_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = HRTIM_TIMC_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = HRTIM_TIMC_CHANNEL_PIN1;
  HAL_GPIO_Init(HRTIM_TIMC_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = HRTIM_TIMD_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = HRTIM_TIMD_CHANNEL_PIN1;
  HAL_GPIO_Init(HRTIM_TIMD_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = HRTIM_TIME_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = HRTIM_TIME_CHANNEL_PIN1;
  HAL_GPIO_Init(HRTIM_TIME_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  HRTIM GPIO pins clock enable
  * @param  None
  * @retval None
  */
static void HRTIM_CHANNEL_GPIO_PORT(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
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

