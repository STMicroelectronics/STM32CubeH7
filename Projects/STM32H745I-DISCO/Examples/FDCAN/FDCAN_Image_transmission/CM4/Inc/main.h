/**
  ******************************************************************************
  * @file    FDCAN_Image_transmission/CM4/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M4.
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
#define ImageSize 79120

/* User can use this section to tailor FDCANx instance used and associated
   resources */
/* Definition for FDCANx clock resources */
#define FDCANx                      FDCAN1
#define FDCANx_CLK_ENABLE()         __HAL_RCC_FDCAN_CLK_ENABLE()
#define FDCANx_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define FDCANx_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()

#define FDCANx_FORCE_RESET()   __HAL_RCC_FDCAN_FORCE_RESET()
#define FDCANx_RELEASE_RESET() __HAL_RCC_FDCAN_RELEASE_RESET()

/* Definition for FDCANx Pins */    
#define FDCANx_TX_PIN       GPIO_PIN_13
#define FDCANx_TX_GPIO_PORT GPIOH
#define FDCANx_TX_AF        GPIO_AF9_FDCAN1
#define FDCANx_RX_PIN       GPIO_PIN_14
#define FDCANx_RX_GPIO_PORT GPIOH
#define FDCANx_RX_AF        GPIO_AF9_FDCAN1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

