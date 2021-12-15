/**
  ******************************************************************************
  * @file    TIM/TIM_Asymetric/Src/stm32h7xx_hal_msp.c
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
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM1_CLK_ENABLE();
  __HAL_RCC_TIM8_CLK_ENABLE();
  
  /* Enable GPIO Channels Clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();  

  /* Configure (PA.08, TIM1_CH1), (PC.06, TIM8_CH1) and (PC.07, TIM8_CH2)
  in output, push-pull, alternate function mode  */
  
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  /* GPIO TIM1_Channel1 configuration : PA.08 in AF1 */
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* GPIO TIM8_Channel1 configuration : PC.06 in AF3 */
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* GPIO TIM8_Channel2 configuration : PC.07 in AF3 */
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
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

