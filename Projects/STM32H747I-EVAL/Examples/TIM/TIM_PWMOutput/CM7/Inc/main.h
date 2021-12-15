/**
  ******************************************************************************
  * @file    TIM/TIM_PWMOutput/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM1
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE(); \
                                       __HAL_RCC_GPIOE_CLK_ENABLE();
#define TIMx_GPIO_PORT_CHANNEL1        GPIOE
#define TIMx_GPIO_PORT_CHANNEL2        GPIOE
#define TIMx_GPIO_PORT_CHANNEL3        GPIOE
#define TIMx_GPIO_PORT_CHANNEL4        GPIOA
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_9
#define TIMx_GPIO_PIN_CHANNEL2         GPIO_PIN_11
#define TIMx_GPIO_PIN_CHANNEL3         GPIO_PIN_13
#define TIMx_GPIO_PIN_CHANNEL4         GPIO_PIN_11
#define TIMx_GPIO_AF_CHANNEL1          GPIO_AF1_TIM1
#define TIMx_GPIO_AF_CHANNEL2          GPIO_AF1_TIM1
#define TIMx_GPIO_AF_CHANNEL3          GPIO_AF1_TIM1
#define TIMx_GPIO_AF_CHANNEL4          GPIO_AF1_TIM1
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

