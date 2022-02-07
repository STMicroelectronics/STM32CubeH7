/**
  ******************************************************************************
  * @file    FDCAN_Com_polling/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor FDCANx instance used and associated
   resources */
/* Definition for FDCANx clock resources */
#define FDCANx                      FDCAN1
#define FDCANx_CLK_ENABLE()         __HAL_RCC_FDCAN_CLK_ENABLE()
#define FDCANx_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define FDCANx_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define FDCANx_FORCE_RESET()   __HAL_RCC_FDCAN_FORCE_RESET()
#define FDCANx_RELEASE_RESET() __HAL_RCC_FDCAN_RELEASE_RESET()

/* Definition for FDCANx Pins */    
#define FDCANx_TX_PIN       GPIO_PIN_12
#define FDCANx_TX_GPIO_PORT GPIOA
#define FDCANx_TX_AF        GPIO_AF9_FDCAN1
#define FDCANx_RX_PIN       GPIO_PIN_11
#define FDCANx_RX_GPIO_PORT GPIOA
#define FDCANx_RX_AF        GPIO_AF9_FDCAN1

/* Definition for FDCANx's NVIC IRQ and IRQ Handlers */
#define FDCANx_IT0_IRQn   FDCAN1_IT0_IRQn
#define FDCANx_IT1_IRQn   FDCAN1_IT1_IRQn
#define FDCANx_IRQHandler FDCAN1_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

