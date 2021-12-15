/**
  ******************************************************************************
  * @file    ResourcesManager/ResourcesManager_SharedResources/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the main.c
  *          file.
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

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h745i_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

  /* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF7_USART3

/* Definition for USARTx's NVIC IRQ and IRQ Handlers */
#define USARTx_IRQn                      USART3_IRQn
#define USARTx_IRQHandler                USART3_IRQHandler

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


