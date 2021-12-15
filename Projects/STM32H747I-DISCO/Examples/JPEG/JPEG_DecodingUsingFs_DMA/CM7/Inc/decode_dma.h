/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/decode_dma.h
  * @author  MCD Application Team
  * @brief   Header for decode_dma.c module
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
#ifndef __DECODE_DMA_H
#define __DECODE_DMA_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress);
uint32_t JPEG_InputHandler(JPEG_HandleTypeDef *hjpeg);

#endif /* __DECODE_DMA_H */

