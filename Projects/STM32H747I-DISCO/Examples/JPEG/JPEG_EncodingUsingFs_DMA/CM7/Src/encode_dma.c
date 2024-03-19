/**
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingUsingFs_DMA/CM7/Src/encode_dma.c
  * @author  MCD Application Team
  * @brief   This file provides routines for JPEG Encoding with DMA method.
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
#include "encode_dma.h"
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup JPEG_EncodingUsingFs_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  __IO  uint8_t State;  
  uint8_t *DataBuffer;
  __IO uint32_t DataBufferSize;

}JPEG_Data_BufferTypeDef;

/* Private define ------------------------------------------------------------*/
#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
#define BYTES_PER_PIXEL    4
#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
#define BYTES_PER_PIXEL    3
#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
#define BYTES_PER_PIXEL    2
#endif

#define CHUNK_SIZE_IN   ((uint32_t)(MAX_INPUT_WIDTH * BYTES_PER_PIXEL * MAX_INPUT_LINES)) 
#define CHUNK_SIZE_OUT  ((uint32_t) (4096))

#define JPEG_BUFFER_EMPTY       0
#define JPEG_BUFFER_FULL        1

#define BMP_HEADER_SIZE  ((uint32_t)(54)) 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
JPEG_RGBToYCbCr_Convert_Function pRGBToYCbCr_Convert_Function;

uint8_t Input_Data_Buffer[CHUNK_SIZE_IN];

uint8_t MCU_Data_IntBuffer0[CHUNK_SIZE_IN];
uint8_t MCU_Data_InBuffer1[CHUNK_SIZE_IN];

uint8_t JPEG_Data_OutBuffer0[CHUNK_SIZE_OUT];
uint8_t JPEG_Data_OutBuffer1[CHUNK_SIZE_OUT];

JPEG_Data_BufferTypeDef Jpeg_OUT_BufferTab = {JPEG_BUFFER_EMPTY , JPEG_Data_OutBuffer0 , 0};

JPEG_Data_BufferTypeDef Jpeg_IN_BufferTab = {JPEG_BUFFER_EMPTY , MCU_Data_IntBuffer0, 0};

uint32_t MCU_TotalNb                = 0;
uint32_t MCU_BlockIndex             = 0;
__IO uint32_t Jpeg_HWEncodingEnd    = 0;


__IO uint32_t Output_Is_Paused      = 0;
__IO uint32_t Input_Is_Paused       = 0;

uint32_t CurrentLine                = 1;
JPEG_ConfTypeDef Conf;

FIL *pBmpFile; 
FIL *pJpegFile;


/* Private function prototypes -----------------------------------------------*/
static void ReadBmpRgbLines(FIL *file, JPEG_ConfTypeDef Conf, uint8_t * pDataBuffer, uint32_t *BufferSize);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Encode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  bmpfile    : bmp file path for encode.
  * @param  jpgfile    : jpg file path for encode.
  * @retval None
  */
uint32_t JPEG_Encode_DMA(JPEG_HandleTypeDef *hjpeg, FIL *bmpfile, FIL *jpgfile)
{
  pBmpFile = bmpfile;
  pJpegFile = jpgfile;
  uint32_t dataBufferSize = 0;

  /* Reset all Global variables */
  MCU_TotalNb                = 0;
  MCU_BlockIndex             = 0;
  Jpeg_HWEncodingEnd         = 0;
  Output_Is_Paused           = 0;
  Input_Is_Paused            = 0;
  CurrentLine                = 1;

  /* BMP Header Parsing */
  BMP_GetInfo(pBmpFile, &Conf);
  JPEG_GetEncodeColorConvertFunc(&Conf, &pRGBToYCbCr_Convert_Function, &MCU_TotalNb);

  /* Clear Output Buffer */
  Jpeg_OUT_BufferTab.DataBufferSize = 0;
  Jpeg_OUT_BufferTab.State = JPEG_BUFFER_EMPTY; 


  /* Fill input Buffer */
  /* Read and reorder MAX_INPUT_LINES lines from BMP file and fill data buffer */
  ReadBmpRgbLines(pBmpFile, Conf, Input_Data_Buffer ,&dataBufferSize);
  
  /* RGB to YCbCr Pre-Processing */
  MCU_BlockIndex += pRGBToYCbCr_Convert_Function(Input_Data_Buffer, Jpeg_IN_BufferTab.DataBuffer, 0, dataBufferSize,(uint32_t*)(&Jpeg_IN_BufferTab.DataBufferSize));
  Jpeg_IN_BufferTab.State = JPEG_BUFFER_FULL;

  /* Fill Encoding Params */
  HAL_JPEG_ConfigEncoding(hjpeg, &Conf);

  /* Start JPEG encoding with DMA method */
  HAL_JPEG_Encode_DMA(hjpeg ,Jpeg_IN_BufferTab.DataBuffer ,Jpeg_IN_BufferTab.DataBufferSize ,Jpeg_OUT_BufferTab.DataBuffer ,CHUNK_SIZE_OUT);

  return 0;
}

/**
  * @brief JPEG Output Data BackGround processing .
  * @param hjpeg: JPEG handle pointer
  * @retval 1 : if JPEG processing has finiched, 0 : if JPEG processing still ongoing
  */
uint32_t JPEG_EncodeOutputHandler(JPEG_HandleTypeDef *hjpeg)
{
  uint32_t bytesWritefile = 0;

  if(Jpeg_OUT_BufferTab.State == JPEG_BUFFER_FULL)
  {  
    f_write (pJpegFile, Jpeg_OUT_BufferTab.DataBuffer ,Jpeg_OUT_BufferTab.DataBufferSize, (UINT*)(&bytesWritefile)) ;
    
    Jpeg_OUT_BufferTab.State = JPEG_BUFFER_EMPTY;
    Jpeg_OUT_BufferTab.DataBufferSize = 0;
    
    if(Jpeg_HWEncodingEnd != 0)
    {
      return 1;
    }
    else if(Output_Is_Paused == 1)
    {
      Output_Is_Paused = 0;
      HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);            
    }    
  }
 
  return 0;  
}

/**
  * @brief JPEG Input Data BackGround Preprocessing .
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void JPEG_EncodeInputHandler(JPEG_HandleTypeDef *hjpeg)
{
  uint32_t dataBufferSize = 0;

  if((Jpeg_IN_BufferTab.State == JPEG_BUFFER_EMPTY) && (MCU_BlockIndex <= MCU_TotalNb))  
  {
    /* Read and reorder 16 lines from BMP file and fill data buffer */
    ReadBmpRgbLines(pBmpFile, Conf, Input_Data_Buffer ,&dataBufferSize);
    if(dataBufferSize != 0)
    {
      /* Pre-Processing */
      MCU_BlockIndex += pRGBToYCbCr_Convert_Function(Input_Data_Buffer, Jpeg_IN_BufferTab.DataBuffer, 0, dataBufferSize, (uint32_t*)(&Jpeg_IN_BufferTab.DataBufferSize));
      Jpeg_IN_BufferTab.State = JPEG_BUFFER_FULL;

      if(Input_Is_Paused == 1)
      {
        Input_Is_Paused = 0;
        HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab.DataBuffer, Jpeg_IN_BufferTab.DataBufferSize);    
  
        HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT); 
      }
    }
    else
    {
      MCU_BlockIndex++;
    }
  }
}

/**
  * @brief JPEG Get Data callback
  * @param hjpeg: JPEG handle pointer
  * @param NbEncodedData: Number of encoded (consumed) bytes from input buffer
  * @retval None
  */
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbEncodedData)
{
  if(NbEncodedData == Jpeg_IN_BufferTab.DataBufferSize)
  {  
    Jpeg_IN_BufferTab.State = JPEG_BUFFER_EMPTY;
    Jpeg_IN_BufferTab.DataBufferSize = 0;

    HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_INPUT);
    Input_Is_Paused = 1;
  }
  else
  {
    HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab.DataBuffer + NbEncodedData, Jpeg_IN_BufferTab.DataBufferSize - NbEncodedData);      
  }
}

/**
  * @brief JPEG Data Ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pDataOut: pointer to the output data buffer
  * @param OutDataLength: length of output buffer in bytes
  * @retval None
  */
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
  Jpeg_OUT_BufferTab.State = JPEG_BUFFER_FULL;
  Jpeg_OUT_BufferTab.DataBufferSize = OutDataLength;

  HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
  Output_Is_Paused = 1;

  HAL_JPEG_ConfigOutputBuffer(hjpeg, Jpeg_OUT_BufferTab.DataBuffer, CHUNK_SIZE_OUT); 
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

/*
  * @brief JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_EncodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{    
  Jpeg_HWEncodingEnd = 1; 
}

/*
  * @brief Read and reorder MAX_INPUT_LINES lines from BMP file and fill data buffer
  * @param file: pointer to the source file name
  * @param file: pointer to the Data Buffer Address
  * @param file: pointer to the Data Buffer Size
  * @retval None
  */
static void ReadBmpRgbLines(FIL *file, JPEG_ConfTypeDef Conf, uint8_t * pDataBuffer, uint32_t *BufferSize)
{
  uint32_t bytesReadfile    = 1;
  uint32_t CurrentBlockLine = 1;
  *BufferSize = 0;

  while((CurrentLine <= Conf.ImageHeight) && (CurrentBlockLine <= MAX_INPUT_LINES))
  {
    f_lseek (file,BMP_HEADER_SIZE + Conf.ImageWidth *(Conf.ImageHeight - CurrentLine)*3);
    f_read (file, pDataBuffer , Conf.ImageWidth*3 , (UINT*)(&bytesReadfile));

    pDataBuffer += bytesReadfile;
    *BufferSize += bytesReadfile;
    CurrentLine +=1 ;
    CurrentBlockLine += 1;
  }
}

/**
  * @}
  */

