/**
  ******************************************************************************
  * @file    MenuLauncher/Modules/splashscreen/decode_dma.c
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
#include "main.h"
#include "AVI_parser.h"
#include "decode_dma.h"
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup MJPEG_VideoDecodingFromQSPI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t State;  
  uint8_t *DataBuffer;
  uint32_t DataBufferSize;

}JPEG_Data_BufferTypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


osMessageQId DecodeEvent = 0;

static osMutexId  osDeviceMutex = 0;

static uint32_t   input_buffer;
static uint32_t   input_buffer_size;
static uint32_t   input_offset;
static uint32_t   output_buffer;
static uint32_t   output_offset;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint32_t JPEG_Decode_Init(JPEG_HandleTypeDef *hjpeg)
{
  uint32_t ret = 0;

  if(!osDeviceMutex)
  {
    ret = 1;

    /* Decode FRAME Queue */
    osMessageQDef(osDecodeQueue, 2, uint32_t);
    DecodeEvent = osMessageCreate (osMessageQ(osDecodeQueue), NULL);
    if(DecodeEvent)
    {
      osMutexDef(DeviceMutex);
      osDeviceMutex = osMutexCreate(osMutex(DeviceMutex));
      if(osDeviceMutex)
      {
        osMutexWait(osDeviceMutex, osWaitForever);

        hjpeg->Instance = JPEG;
        ret = HAL_JPEG_Init(hjpeg);

        osMutexRelease(osDeviceMutex);
      }
    }

    /* Clean up in case of error */
    if (ret != HAL_OK)
    {
      if(DecodeEvent)
      {
        osMessageDelete(DecodeEvent);
        DecodeEvent = 0;
      }
      if(osDeviceMutex)
      {
        osMutexDelete(osDeviceMutex);
        osDeviceMutex = 0;
      }
    }
  }

  return ret;
}

void JPEG_Decode_DeInit(JPEG_HandleTypeDef *hjpeg)
{
  if(DecodeEvent)
  {
    osMessageDelete(DecodeEvent);
    DecodeEvent = 0;
  }

  osMutexWait(osDeviceMutex, osWaitForever);
  HAL_JPEG_DeInit(hjpeg);
  osMutexRelease(osDeviceMutex);

  if(osDeviceMutex)
  {
    osMutexDelete(osDeviceMutex);
    osDeviceMutex = 0;
  }
}

/**
  * @brief  Decode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  FrameSourceAddress    : video buffer address.
  * @param  DestAddress : YCbCr destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress)
{
  uint32_t ret = 0;
  osMutexWait(osDeviceMutex, osWaitForever);

  input_buffer      = FrameSourceAddress ;
  output_buffer     = DestAddress;
  input_offset      = 0;
  output_offset     = 0;
  input_buffer_size = (FrameSize + 31) &~31;

  /* Start JPEG decoding with DMA method */
  if(HAL_JPEG_Decode_DMA(hjpeg, (uint8_t *)input_buffer, MAX_CHUNK_SIZE, (uint8_t *)output_buffer, MAX_CHUNK_SIZE) != HAL_OK)
  {
    ret = 1;
  }

  osMutexRelease(osDeviceMutex);
  return ret;
}

/**
  * @brief  JPEG Get Data callback
  * @param hjpeg: JPEG handle pointer
  * @param NbDecodedData: Number of decoded (consummed) bytes from input buffer
  * @retval None
  */
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
  int size = 0;
  osMutexWait(osDeviceMutex, osWaitForever);

  input_buffer_size -= NbDecodedData;

  if (input_buffer_size > 0)
  {
    input_offset += NbDecodedData;
    size = (input_buffer_size + 4) > MAX_CHUNK_SIZE ? MAX_CHUNK_SIZE : input_buffer_size;
  }

  HAL_JPEG_ConfigInputBuffer(hjpeg,(uint8_t *)(input_buffer + input_offset), size);

  osMutexRelease(osDeviceMutex);
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
  osMutexWait(osDeviceMutex, osWaitForever);

  /* Update JPEG encoder output buffer address*/  
  output_offset += OutDataLength;

  HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)(output_buffer + output_offset), MAX_CHUNK_SIZE);

  osMutexRelease(osDeviceMutex);
}

/**
  * @brief  JPEG Error callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
  if (DecodeEvent)
  {
    osMessagePut(DecodeEvent, DECODE_ERROR_EVENT, 0);
  }
}

/**
  * @brief  JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{    
  if (DecodeEvent)
  {
    osMessagePut(DecodeEvent, DECODE_COMPLETE_EVENT, 0);
  }
}

/**
  * @}
  */

/**
  * @}
  */

