/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_MultiplePWM/CM7/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module for Cortex-M7.       
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defgroup HRTIM_MultiplePWM
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

  /* Configure HRTIM TIMB TB2
   channels as alternate function mode */
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Alternate = HRTIM_TIMB_GPIO_AF_CHANNEL2;
  GPIO_InitStruct.Pin = HRTIM_TIMB_CHANNEL_PIN2;
  HAL_GPIO_Init(HRTIM_TIMB_CHANNEL2_GPIO_PORT, &GPIO_InitStruct);

}

/**
  * @brief  HRTIM GPIO pins clock enable
  * @param  None
  * @retval None
  */
static void HRTIM_CHANNEL_GPIO_PORT(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
