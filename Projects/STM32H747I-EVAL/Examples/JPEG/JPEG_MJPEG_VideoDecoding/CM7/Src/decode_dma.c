/**
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecoding/CM7/Src/decode_dma.c
  * @author  MCD Application Team
 * @brief   This file provides routines for JPEG decoding with DMA method.
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

/* Includes ------------------------------------------------------------------*/
#include "decode_dma.h"
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup JPEG_MJPEG_VideoDecoding
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define CHUNK_SIZE_IN  ((uint32_t)(64 * 1024)) 
#define CHUNK_SIZE_OUT ((uint32_t)(64 * 1024))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


__IO uint32_t Jpeg_HWDecodingEnd = 0;



uint32_t FrameBufferAddress;
uint32_t JPEGSourceAddress;
uint32_t Input_frameSize;
uint32_t Input_frameIndex;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Decode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  FrameSourceAddress    : video buffer address.
  * @param  DestAddress : YCbCr destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress)
{
  JPEGSourceAddress =  FrameSourceAddress ;
  FrameBufferAddress = DestAddress;
  Input_frameIndex=0;
  Input_frameSize = FrameSize;
  
  Jpeg_HWDecodingEnd = 0;
  
  /* Start JPEG decoding with DMA method */
  HAL_JPEG_Decode_DMA(hjpeg ,(uint8_t *)JPEGSourceAddress ,CHUNK_SIZE_IN ,(uint8_t *)FrameBufferAddress ,CHUNK_SIZE_OUT);
  
  
  return 0;
  
}

/**
  * @brief  JPEG Info ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pInfo: JPEG Info Struct pointer
  * @retval None
  */
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{  
}

/**
  * @brief  JPEG Get Data callback
  * @param hjpeg: JPEG handle pointer
  * @param NbDecodedData: Number of decoded (consumed) bytes from input buffer
  * @retval None
  */
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
  uint32_t inDataLength; 
  Input_frameIndex += NbDecodedData;
  if( Input_frameIndex < Input_frameSize)
  {
   JPEGSourceAddress = JPEGSourceAddress + NbDecodedData;
    
    if((Input_frameSize - Input_frameIndex) >= CHUNK_SIZE_IN)
    {
      inDataLength = CHUNK_SIZE_IN;
    }
    else
    {
      inDataLength = Input_frameSize - Input_frameIndex;
    }    
  }
  else
  {
    inDataLength = 0; 
  }
  HAL_JPEG_ConfigInputBuffer(hjpeg,(uint8_t *)JPEGSourceAddress, inDataLength);    
  
}

/**
  * @brief  JPEG Data Ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pDataOut: pointer to the output data buffer
  * @param OutDataLength: length of output buffer in bytes
  * @retval None
  */
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
  /* Update JPEG encoder output buffer address*/  
  FrameBufferAddress += OutDataLength;

  HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)FrameBufferAddress, CHUNK_SIZE_OUT); 
}

/**
  * @brief  JPEG Error callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
  Error_Handler();
}

/**
  * @brief  JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{    
  Jpeg_HWDecodingEnd = 1; 
}

/**
  * @}
  */

/**
  * @}
  */

