/**
  ******************************************************************************
  * @file    videoplayer_app.c
  * @author  MCD Application Team
  * @brief   This file provides routines for JPEG decoding with DMA method.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "video_player_app.h"
#include "jpeg_utils.h"
#include "LCDConf.h"
#include "GUI_AVI.h"

/** @addtogroup VIDEO_PLAYER
  * @{
  */

/** @addtogroup VIDEO_PLAYER
  * @{
  */

extern uint8_t toggle_chromART;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DECODE_CONVERT_OUTPUT_BUFF    0x11

#define FRAME_BUFFER_WIDTH              640
#define FRAME_BUFFER_HEIGHT             480
#define RGB565_BYTES_PER_PIXEL          (uint32_t) (2U)

#define RGB_VIDEO_BUFFER_SIZE           ((uint32_t)(FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * RGB565_BYTES_PER_PIXEL)) /* 750 Kb */
#define RGB_VIDEO_BUFFER_NBR            2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
JPEG_YCbCrToRGB_Convert_Function pConvert_Function;
JPEG_HandleTypeDef    JPEG_Handle;
static uint32_t DecodeBufferAddress;
static uint32_t DisplayBufferAddress;
static uint8_t EnabledChromART = 1;

/* Buffers for right and left samples */
JPEG_ConfTypeDef       JPEG_Info;

static uint32_t MCU_BlockIndex = 0;
osThreadId hOutputThread;
static osMessageQId OutputEvent = 0;
uint32_t MCU_TotalNb = 0, IsFirstTime = 0;
osSemaphoreId osVidSemph;
osSemaphoreId osVidSemph_output;

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="rgb_video_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".rgb_video_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".rgb_video_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t VIDEO_RGBBuffer[RGB_VIDEO_BUFFER_NBR][RGB_VIDEO_BUFFER_SIZE] ;

/* Private function prototypes -----------------------------------------------*/
static void OutputThread(void const *argument);
void TransferComplete_CallBack(void);
void HalfTransfer_CallBack(void);
extern void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine);
extern void DMA2D_CopyBuffer_YCbCr_To_RGB(uint32_t *pSrc, uint16_t xsize, uint16_t ysize);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init HArdware Decoder
  * @param  None
  * @retval None
  */
void HW_JPEG_Init(void)
{
  JPEG_Handle.Instance = JPEG;
  
  HAL_JPEG_Init(&JPEG_Handle);

  DecodeBufferAddress  = (uint32_t )&VIDEO_RGBBuffer[0];
  DisplayBufferAddress = (uint32_t )&VIDEO_RGBBuffer[0];
  
  /* Output FRAME Queue */
  osMessageQDef(OUTPUT_Queue, 6, uint16_t);
  OutputEvent = osMessageCreate (osMessageQ(OUTPUT_Queue), NULL); 
  
  /* Output Thread  definition */
  osThreadDef(OUTPUT_THREAD, OutputThread, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE);
  hOutputThread = osThreadCreate(osThread(OUTPUT_THREAD), NULL);
  
  /* Create the Semaphore used by the two threads */
  /* Create Semaphore lock */
  osSemaphoreDef(Semaphore);
  osVidSemph = osSemaphoreCreate(osSemaphore(Semaphore), 1);
  osSemaphoreWait(osVidSemph , osWaitForever); 
}


/**
  * @brief  Stop HArdware Decoder
  * @param  None
  * @retval None
  */
void HW_JPEG_DeInit(void)
{ 
  HAL_NVIC_DisableIRQ(MDMA_IRQn);
  
  /* DeInitialize the MDMA Stream */
  HAL_MDMA_DeInit(JPEG_Handle.hdmain);
  
  /* DeInitialize the MDMA Stream */
  HAL_MDMA_DeInit(JPEG_Handle.hdmaout);
  
  HAL_JPEG_DeInit(&JPEG_Handle);
  
  if(hOutputThread)
  {
    osThreadTerminate(hOutputThread);
    hOutputThread = 0;
  }
  
  if(osVidSemph)
  {
    osSemaphoreDelete(osVidSemph);
    osVidSemph = 0;
  }
  
  if(OutputEvent)
  {
    vQueueDelete(OutputEvent); 
    OutputEvent = 0;  
  }
}

/**
  * @brief  Output Jpeg task
  * @param  None
  * @retval None
  */
static void OutputThread(void const *argument)
{
  uint32_t ConvertedDataCount;
  osEvent event;
  
  for(;;)
  {
    event = osMessageGet(OutputEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
        case DECODE_CONVERT_OUTPUT_BUFF:
          if(EnabledChromART == 1)
          {
            DMA2D_CopyBuffer_YCbCr_To_RGB((uint32_t *)DisplayBufferAddress, 640, 480);
            DisplayBufferAddress = (DisplayBufferAddress == (uint32_t )&VIDEO_RGBBuffer[1] ? (uint32_t )&VIDEO_RGBBuffer[0] : (uint32_t )&VIDEO_RGBBuffer[1]);
          }
          else
          {
            MCU_BlockIndex += pConvert_Function( (uint8_t *)DecodeBufferAddress, (uint8_t *)DisplayBufferAddress, MCU_BlockIndex, CHUNK_SIZE_OUT, &ConvertedDataCount);
            if((MCU_BlockIndex == MCU_TotalNb) && (MCU_TotalNb != 0))
            {
              _LCD_DrawBitmap16bpp(0, 0, 0, (uint16_t *)DisplayBufferAddress, 640, 480, 1280);
              DisplayBufferAddress = (DisplayBufferAddress == (uint32_t )&VIDEO_RGBBuffer[1] ? (uint32_t )&VIDEO_RGBBuffer[0] : (uint32_t )&VIDEO_RGBBuffer[1]);
            }
            else
            {
              HAL_JPEG_Resume(&JPEG_Handle, JPEG_PAUSE_RESUME_OUTPUT); 
            }
          }
          break;
      }
    }
  }
}

/**
  * @brief  Decode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  FileName    : jpg file path for decode.
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
U32 HW_JPEG_Draw (const void * pFileData, U32 DataSize, U32 x0, U32 y0)
{
  MCU_BlockIndex = 0;
  MCU_TotalNb = 0;

  /* Set the current ChromART state */
  EnabledChromART = toggle_chromART;

  /* Set the Decoder buffer */
  DecodeBufferAddress = (DecodeBufferAddress == (uint32_t )&VIDEO_RGBBuffer[1]) ? (uint32_t )&VIDEO_RGBBuffer[0] : (uint32_t )&VIDEO_RGBBuffer[1];

  /* Start the HW Decoder */
  HAL_JPEG_Decode_DMA(&JPEG_Handle , (uint8_t *)pFileData ,DataSize ,(uint8_t *)DecodeBufferAddress ,CHUNK_SIZE_OUT);

  if(osSemaphoreWait(osVidSemph , 1000) == osErrorOS)
  {
    return 1;
  }
  return 0;
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
  if(EnabledChromART == 1)
  {
    MCU_BlockIndex += CHUNK_SIZE_OUT;
    HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)(DecodeBufferAddress + MCU_BlockIndex), CHUNK_SIZE_OUT);    
  }
  else
  {
    SCB_InvalidateDCache_by_Addr((uint32_t *)pDataOut, CHUNK_SIZE_OUT);
    HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
    HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)DecodeBufferAddress, CHUNK_SIZE_OUT); 
    osMessagePut ( OutputEvent, DECODE_CONVERT_OUTPUT_BUFF, 0);    
  }
}

/**
  * @brief  JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
  if(EnabledChromART == 1)
  {
    osMessagePut ( OutputEvent, DECODE_CONVERT_OUTPUT_BUFF, 0);
  }
  osSemaphoreRelease(osVidSemph);
}

/**
  * @brief  JPEG Info ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pInfo: JPEG Info Struct pointer
  * @retval None
  */
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
  if(EnabledChromART == 0)
  {
    if(JPEG_GetDecodeColorConvertFunc(pInfo, &pConvert_Function, &MCU_TotalNb) == HAL_OK)
    {
    }
  }
}


/**
  * @brief JPEG MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - NVIC configuration for JPEG interrupt request enable
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
  static MDMA_HandleTypeDef   hmdmaIn;
  static MDMA_HandleTypeDef   hmdmaOut;  
  
  /* Enable JPEG clock */
  __HAL_RCC_JPGDECEN_CLK_ENABLE();
  
  /* Enable MDMA clock */
  __HAL_RCC_MDMA_CLK_ENABLE();
  
  HAL_NVIC_SetPriority(JPEG_IRQn, 0x07, 0x0F);
  HAL_NVIC_EnableIRQ(JPEG_IRQn);  
  
  /* Input MDMA */
  /* Set the parameters to be configured */ 
  hmdmaIn.Init.Priority           = MDMA_PRIORITY_HIGH;
  hmdmaIn.Init.Endianness         = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdmaIn.Init.SourceInc          = MDMA_SRC_INC_BYTE;
  hmdmaIn.Init.DestinationInc     = MDMA_DEST_INC_DISABLE;
  hmdmaIn.Init.SourceDataSize     = MDMA_SRC_DATASIZE_BYTE;
  hmdmaIn.Init.DestDataSize       = MDMA_DEST_DATASIZE_WORD;
  hmdmaIn.Init.DataAlignment      = MDMA_DATAALIGN_PACKENABLE;   
  hmdmaIn.Init.SourceBurst        = MDMA_SOURCE_BURST_32BEATS;
  hmdmaIn.Init.DestBurst          = MDMA_DEST_BURST_16BEATS; 
  hmdmaIn.Init.SourceBlockAddressOffset = 0;
  hmdmaIn.Init.DestBlockAddressOffset  = 0;
  
  /*Using JPEG Input FIFO Threshold as a trigger for the MDMA*/
  hmdmaIn.Init.Request = MDMA_REQUEST_JPEG_INFIFO_TH; /* Set the MDMA HW trigger to JPEG Input FIFO Threshold flag*/  
  hmdmaIn.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;  
  hmdmaIn.Init.BufferTransferLength = 32; /*Set the MDMA buffer size to the JPEG FIFO threshold size i.e 32 bytes (8 words)*/
  
  hmdmaIn.Instance = MDMA_Channel1;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hjpeg, hdmain, hmdmaIn);
  
  /* DeInitialize the DMA Stream */
  HAL_MDMA_DeInit(&hmdmaIn);  
  /* Initialize the DMA stream */
  HAL_MDMA_Init(&hmdmaIn);  
  
  /* Output MDMA */
  /* Set the parameters to be configured */ 
  hmdmaOut.Init.Priority        = MDMA_PRIORITY_VERY_HIGH;
  hmdmaIn.Init.Endianness       = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdmaOut.Init.SourceInc       = MDMA_SRC_INC_DISABLE;
  hmdmaOut.Init.DestinationInc  = MDMA_DEST_INC_BYTE;
  hmdmaOut.Init.SourceDataSize  = MDMA_SRC_DATASIZE_WORD;
  hmdmaOut.Init.DestDataSize    = MDMA_DEST_DATASIZE_BYTE;
  hmdmaOut.Init.DataAlignment   = MDMA_DATAALIGN_PACKENABLE;
  hmdmaOut.Init.SourceBurst     = MDMA_SOURCE_BURST_32BEATS;
  hmdmaOut.Init.DestBurst       = MDMA_DEST_BURST_32BEATS;  
  hmdmaOut.Init.SourceBlockAddressOffset = 0;
  hmdmaOut.Init.DestBlockAddressOffset  = 0;  
  
  /*Using JPEG Output FIFO Threshold as a trigger for the MDMA*/
  hmdmaOut.Init.Request              = MDMA_REQUEST_JPEG_OUTFIFO_TH; /* Set the MDMA HW trigger to JPEG Output FIFO Threshold flag*/ 
  hmdmaOut.Init.TransferTriggerMode  = MDMA_BUFFER_TRANSFER;    
  hmdmaOut.Init.BufferTransferLength = 32; /*Set the MDMA buffer size to the JPEG FIFO threshold size i.e 32 bytes (8 words)*/
  
  hmdmaOut.Instance = MDMA_Channel0;
  /* DeInitialize the DMA Stream */
  HAL_MDMA_DeInit(&hmdmaOut);  
  /* Initialize the DMA stream */
  HAL_MDMA_Init(&hmdmaOut);
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hjpeg, hdmaout, hmdmaOut);

  HAL_NVIC_SetPriority(MDMA_IRQn, 0x08, 0x0F);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);
  
  JPEG_InitColorTables();
}

/**
  * @}
  */

/**
  * @}
  */

