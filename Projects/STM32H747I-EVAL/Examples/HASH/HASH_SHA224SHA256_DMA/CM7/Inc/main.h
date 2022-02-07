/**
  ******************************************************************************
  * @file    HASH/HASH_SHA224SHA256_DMA/CM7/Inc/main.h
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
#include "stm32h747i_eval_io.h"
#include "string.h"

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define DMA_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()

/* Definition for HASH's DMA */
#define HASH_DMA_CHANNEL                DMA_REQUEST_HASH_IN
#define HASH_DMA_STREAM                 DMA2_Stream7

/* Definition for DMA's NVIC */
#define HASH_DMA_IRQn                   DMA2_Stream7_IRQn
#define HASH_DMA_IRQHandler             DMA2_Stream7_IRQHandler
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

