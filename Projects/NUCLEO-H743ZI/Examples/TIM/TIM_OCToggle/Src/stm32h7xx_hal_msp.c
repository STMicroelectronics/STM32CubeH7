/**
  ******************************************************************************
  * @file    TIM/TIM_OCToggle/Src/stm32h7xx_hal_msp.c
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
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */


/**
  * @brief TIM MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIM1 Peripheral clock enable */
  TIMx_CLK_ENABLE();
    
  /* Enable GPIO Channels Clock */
  TIMx_CHANNEL_GPIO_PORT;
  
  /*##-2- Configure I/Os #####################################################*/
  /* Configure PA.08 (TIM1_Channel1), PA.09 (TIM1_Channel2), PA.10 (TIM1_Channel3),
     PE.14 (TIM1_Channel4) in output, pull-up, alternate function mode
  */
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL1;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);
  
  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL2;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL2;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);
  
  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL3;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL3;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL3, &GPIO_InitStruct);
  
  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL4;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL4;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL4, &GPIO_InitStruct);

  /*##-3- Enable the TIM2 global Interrupt & set priority ####################*/
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
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

