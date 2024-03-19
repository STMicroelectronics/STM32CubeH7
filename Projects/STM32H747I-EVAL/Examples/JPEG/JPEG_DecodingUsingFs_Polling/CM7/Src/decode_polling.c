/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_Polling/CM7/Src/decode_polling.c
  * @author  MCD Application Team
  * @brief   This file provides routines for JPEG decoding with polling method.
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
#include "decode_polling.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup JPEG_DecodingUsingFs_Polling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t *DataBuffer;
  uint32_t DataBufferSize;

}JPEG_Data_BufferTypeDef;

/* Private define ------------------------------------------------------------*/

#define CHUNK_SIZE_IN  ((uint32_t)(4096)) 
#define CHUNK_SIZE_OUT ((uint32_t)(64 * 1024))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

FIL *pFile;     /* pointer to File object */

uint8_t MCU_Data_OutBuffer[CHUNK_SIZE_OUT];
uint8_t JPEG_Data_InBuffer[CHUNK_SIZE_IN];

JPEG_Data_BufferTypeDef JPEG_InBuffer = { JPEG_Data_InBuffer , 0};

uint32_t MCU_TotalNb = 0;
uint32_t MCU_BlockIndex = 0;
uint32_t Inputfile_Offset = 0;
uint32_t Jpeg_Decoding_End = 0;

uint32_t FrameBufferAddress;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Decode_Polling
  * @param hjpeg: JPEG handle pointer
  * @param  file   : pointer to file object
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_DecodePolling(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress)
{
  pFile = file;  
  FrameBufferAddress = DestAddress;

  /* Read from JPG file and fill the input buffer */
  if(f_read (pFile, JPEG_InBuffer.DataBuffer , CHUNK_SIZE_IN, (UINT*)(&JPEG_InBuffer.DataBufferSize)) != FR_OK)
  {
    Error_Handler();
  }
  
  /* Update the file Offset*/
  Inputfile_Offset = JPEG_InBuffer.DataBufferSize;
    
  /* Start JPEG decoding with polling (Blocking) method */
  HAL_JPEG_Decode(hjpeg ,JPEG_InBuffer.DataBuffer ,JPEG_InBuffer.DataBufferSize ,(uint8_t *)FrameBufferAddress ,CHUNK_SIZE_OUT,HAL_MAX_DELAY);
  
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
  if(NbDecodedData != JPEG_InBuffer.DataBufferSize)
  {
    Inputfile_Offset = Inputfile_Offset - JPEG_InBuffer.DataBufferSize + NbDecodedData;
    f_lseek(pFile,Inputfile_Offset);
  }
    
  if(f_read (pFile, JPEG_InBuffer.DataBuffer , CHUNK_SIZE_IN, (UINT*)(&JPEG_InBuffer.DataBufferSize)) == FR_OK)
  {
    Inputfile_Offset += JPEG_InBuffer.DataBufferSize;
    HAL_JPEG_ConfigInputBuffer(hjpeg, JPEG_InBuffer.DataBuffer, JPEG_InBuffer.DataBufferSize);  
  }
  else
  {
    Error_Handler();
  }
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
  Jpeg_Decoding_End = 1;
}

/**
  * @}
  */

/**
  * @}
  */

