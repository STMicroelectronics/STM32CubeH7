/**
  ******************************************************************************
  * @file    TIM/TIM_DMA/CM7/Inc/main.h
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
#include "stm32h745i_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of TIM instance */
#define TIMx                             TIM3

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM3_CLK_ENABLE
#define DMAx_CLK_ENABLE                  __HAL_RCC_DMA1_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_CHANNEL1_GPIO_CLK_ENABLE    __HAL_RCC_GPIOA_CLK_ENABLE

#define TIMx_GPIO_CHANNEL1_PORT          GPIOA
#define GPIO_PIN_CHANNEL1                GPIO_PIN_6


#define GPIO_AF_TIMx                     GPIO_AF2_TIM3

/* Definition for TIMx's DMA */
#define TIMx_CC1_DMA_REQUEST             DMA_REQUEST_TIM3_CH1
#define TIMx_CC1_DMA_INST                DMA1_Stream7

/* Definition for ADCx's NVIC */
#define TIMx_DMA_IRQn                    DMA1_Stream7_IRQn
#define TIMx_DMA_IRQHandler              DMA1_Stream7_IRQHandler
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

