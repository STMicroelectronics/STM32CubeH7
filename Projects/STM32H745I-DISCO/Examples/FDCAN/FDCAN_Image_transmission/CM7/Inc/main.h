/**
  ******************************************************************************
  * @file    FDCAN_Image_transmission/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
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
#include "stm32h745i_discovery_sdram.h"
#include "stm32h745i_discovery_lcd.h"
#include "stm32_lcd.h"

#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ImageSize 79120
#define LCD_FRAME_BUFFER 0xD0000000
#define FIRST_RUN  0
#define SECOND_RUN 1
#define END_TEST   2

/* User can use this section to tailor FDCANx instance used and associated
   resources */
/* Definition for FDCANx clock resources */
#define FDCANx                      FDCAN2
#define FDCANx_CLK_ENABLE()         __HAL_RCC_FDCAN_CLK_ENABLE()
#define FDCANx_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define FDCANx_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define FDCANx_FORCE_RESET()   __HAL_RCC_FDCAN_FORCE_RESET()
#define FDCANx_RELEASE_RESET() __HAL_RCC_FDCAN_RELEASE_RESET()

/* Definition for FDCANx Pins */    
#define FDCANx_TX_PIN       GPIO_PIN_13
#define FDCANx_TX_GPIO_PORT GPIOB
#define FDCANx_TX_AF        GPIO_AF9_FDCAN1
#define FDCANx_RX_PIN       GPIO_PIN_5
#define FDCANx_RX_GPIO_PORT GPIOB
#define FDCANx_RX_AF        GPIO_AF9_FDCAN1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

