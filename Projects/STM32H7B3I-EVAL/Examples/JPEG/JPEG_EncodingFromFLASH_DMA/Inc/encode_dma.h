/**
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/Inc/encode_dma.h
  * @author  MCD Application Team
  * @brief   Header for encode_dma.c module
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
#ifndef __ENCODE_DMA_H
#define __ENCODE_DMA_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t JPEG_Encode_DMA(JPEG_HandleTypeDef *hjpeg, uint32_t RGBImageBufferAddress, uint32_t RGBImageSize_Bytes, FIL *jpgfile);
uint32_t JPEG_EncodeOutputHandler(JPEG_HandleTypeDef *hjpeg);
void JPEG_EncodeInputHandler(JPEG_HandleTypeDef *hjpeg);

#endif /* __ENCODE_DMA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
