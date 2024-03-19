/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_Interrupt/CM7/Src/decode_interrupt.c
  * @author  MCD Application Team
  * @brief   This file provides routines for JPEG decoding with IT method.
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
#include "decode_interrupt.h"
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup JPEG_DecodingUsingFs_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  __IO  uint8_t State;  
  uint8_t *DataBuffer;
  __IO  uint32_t DataBufferSize;

}JPEG_Data_BufferTypeDef;

/* Private define ------------------------------------------------------------*/

#define CHUNK_SIZE_IN  ((uint32_t)(4096)) 
#define CHUNK_SIZE_OUT ((uint32_t)(64 * 1024))

#define JPEG_BUFFER_EMPTY 0
#define JPEG_BUFFER_FULL  1

#define NB_INPUT_DATA_BUFFERS       2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

FIL *pFile;     /* pointer to File object */

uint8_t JPEG_Data_InBuffer0[CHUNK_SIZE_IN];
uint8_t JPEG_Data_InBuffer1[CHUNK_SIZE_IN];


JPEG_Data_BufferTypeDef Jpeg_IN_BufferTab[NB_INPUT_DATA_BUFFERS] =
{
  {JPEG_BUFFER_EMPTY , JPEG_Data_InBuffer0, 0},
  {JPEG_BUFFER_EMPTY , JPEG_Data_InBuffer1, 0}
};

uint32_t MCU_TotalNb = 0;
uint32_t MCU_BlockIndex = 0;
uint32_t Jpeg_HWDecodingEnd = 0;

uint32_t JPEG_IN_Read_BufferIndex = 0;
uint32_t JPEG_IN_Write_BufferIndex = 0;
__IO uint32_t Input_Is_Paused = 0;

uint32_t FrameBufferAddress;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  JPEG_Decode_IT
  * @param  hjpeg: JPEG handle pointer
  * @param  FileName    : jpg file path for decode.
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_Decode_IT(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress)
{
  uint32_t i;
  
  pFile = file;
  FrameBufferAddress = DestAddress;
          
  /* Read from JPG file and fill input buffers */
  for(i = 0; i < NB_INPUT_DATA_BUFFERS; i++)
  {
    if(f_read (pFile, Jpeg_IN_BufferTab[i].DataBuffer , CHUNK_SIZE_IN, (UINT*)(&Jpeg_IN_BufferTab[i].DataBufferSize)) == FR_OK)
    {
      Jpeg_IN_BufferTab[i].State = JPEG_BUFFER_FULL;
    }
    else
    {
      Error_Handler();
    }        
  } 
  /* Start JPEG decoding with IT method */
  HAL_JPEG_Decode_IT(hjpeg ,Jpeg_IN_BufferTab[0].DataBuffer ,Jpeg_IN_BufferTab[0].DataBufferSize ,(uint8_t *)FrameBufferAddress ,CHUNK_SIZE_OUT);
  
  return 0;
}

/**
  * @brief  JPEG Input Data BackGround processing .
  * @param hjpeg: JPEG handle pointer
  * @retval None
*/
uint32_t JPEG_InputHandler(JPEG_HandleTypeDef *hjpeg)
{
  if(Jpeg_HWDecodingEnd == 0)
  {
    if(Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].State == JPEG_BUFFER_EMPTY)
    {
      if(f_read (pFile, Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].DataBuffer , CHUNK_SIZE_IN, (UINT*)(&Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].DataBufferSize)) == FR_OK)
      {  
        Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].State = JPEG_BUFFER_FULL;
      }
      else
      {
        Error_Handler();
      }
      
      if((Input_Is_Paused == 1) && (JPEG_IN_Write_BufferIndex == JPEG_IN_Read_BufferIndex))
      {
        Input_Is_Paused = 0;
        HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBuffer, Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize);    
        
        HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT); 
      }
      
      JPEG_IN_Write_BufferIndex++;
      if(JPEG_IN_Write_BufferIndex >= NB_INPUT_DATA_BUFFERS)
      {
        JPEG_IN_Write_BufferIndex = 0;
      }            
    }
    return 0;
  }
  else
  {
    return 1;
  }
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
  if(NbDecodedData == Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize)
  {  
    Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].State = JPEG_BUFFER_EMPTY;
    Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize = 0;
  
    JPEG_IN_Read_BufferIndex++;
    if(JPEG_IN_Read_BufferIndex >= NB_INPUT_DATA_BUFFERS)
    {
      JPEG_IN_Read_BufferIndex = 0;        
    }
  
    if(Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].State == JPEG_BUFFER_EMPTY)
    {
      HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_INPUT);
      Input_Is_Paused = 1;
    }
    else
    {    
      HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBuffer, Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize);    
    }
  }
  else
  {
    HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBuffer + NbDecodedData, Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize - NbDecodedData);      
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
  Jpeg_HWDecodingEnd = 1; 
}

/**
  * @}
  */

/**
  * @}
  */

