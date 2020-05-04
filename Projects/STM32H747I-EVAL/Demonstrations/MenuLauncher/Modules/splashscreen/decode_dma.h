/**
  ******************************************************************************
  * @file    MenuLauncher\Modules\splashscreen\decode_dma.h
  * @author  MCD Application Team
  * @brief   Header for decode_dma.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
enum
{
  DECODE_COMPLETE_EVENT     = 0x10
, DECODE_ERROR_EVENT
};

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t JPEG_Decode_Init(JPEG_HandleTypeDef *hjpeg);
void JPEG_Decode_DeInit(JPEG_HandleTypeDef *hjpeg);
uint32_t JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress);

#endif /* __DECODE_DMA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
