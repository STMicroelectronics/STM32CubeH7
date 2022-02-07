/**
  ******************************************************************************
  * @file    JPEG/MJPEG_VideoDecodingFromQSPI/Src/main.c
  * @author  MCD Application Team
  * @version V0.5.0
  * @date    18-February-2017  
  * @brief   This sample code shows how to use the HW JPEG to Decode an MJPEG video file 
  *          using the STM32H7xx HAL API.
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
#include "main.h"
#include "AVI_parser.h"
#include "decode_dma.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defroup MJPEG_VideoDecodingFromQSPI
  * @{
  */

extern void DMA2D_CopyBuffer_YCbCr_To_RGB(int LayerIndex, const void * pSrc, const void * pDst, int xSize, int ySize, int aCssMode);

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define RGB_OUTPUT_DATA_BUFFER0         ((uint8_t const *)&VIDEO_RGBBuffer[0])
#define RGB_OUTPUT_DATA_BUFFER1         ((uint8_t const *)&VIDEO_RGBBuffer[1])
#define AVI_FILE_ADDRESS                ((uint32_t)&splash_st) /*Address of the MJPEG AVI file located on the QSPI Flash*/

#define SPLASHSCREEN_THREAD_STACK_SIZE  (configMINIMAL_STACK_SIZE * 16)
#define SPLASHSCREEN_THREAD_PRIORITY    osPriorityBelowNormal

enum
{
  DISPLAY_COMPLETE_EVENT     = 0x20
, DISPLAY_EXIT_EVENT
};

JPEG_HandleTypeDef            JPEG_Handle;
extern osMessageQId           DecodeEvent;
extern const unsigned char    splash_st __attribute__ ((section ("splash_st_section")));

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AVI_CONTEXT            AVI_Handle;
static JPEG_ConfTypeDef       JPEG_Info;

static volatile osMessageQId  SplashScreenEvent = 0;
static volatile osMessageQId  DisplayEvent = 0;
static osThreadId             hDisplayTask = NULL;

/* Buffers for right and left samples */
#if defined ( __GNUC__ )
__attribute__((section(".jpeg_decoder_buffers"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="jpeg_decoder_buffers"
#elif defined( __CC_ARM )
__attribute__((section(".jpeg_decoder_buffers"), zero_init)) __attribute__ ((aligned (32)))
#endif
uint8_t VIDEO_JPEGBuffer[MAX_CHUNK_SIZE];

#if defined ( __GNUC__ )
__attribute__((section(".rgb_video_buffers"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="rgb_video_buffers"
#elif defined( __CC_ARM )
__attribute__((section(".rgb_video_buffers"), zero_init)) __attribute__ ((aligned (32)))
#endif
static uint32_t VIDEO_RGBBuffer[2][480 * 272];

/* Private function prototypes -----------------------------------------------*/
static void DMA2D_CopyBuffer(void *pSrc, void *pDst, JPEG_ConfTypeDef *JPEG_Info);

/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

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
  hmdmaOut.Init.Endianness      = MDMA_LITTLE_ENDIANNESS_PRESERVE;
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
  
}

void HAL_JPEG_MspDeInit(JPEG_HandleTypeDef *hjpeg)
{
  HAL_NVIC_DisableIRQ(JPEG_IRQn);
  HAL_NVIC_DisableIRQ(MDMA_IRQn);
  
  /* DeInitialize the MDMA Stream */
  HAL_MDMA_DeInit(hjpeg->hdmain);
  
  /* DeInitialize the MDMA Stream */
  HAL_MDMA_DeInit(hjpeg->hdmaout);
}

/**
  * @brief  This function handles JPEG interrupt request.
  * @param  None
  * @retval None
  */
void JPEG_IRQHandler(void)
{
  HAL_JPEG_IRQHandler(&JPEG_Handle);
}

/**
  * @brief  This function handles MDMA interrupt request.
  * @param  None
  * @retval None
  */
void MDMA_IRQHandler()
{
  /* Check the interrupt and clear flag */
  HAL_MDMA_IRQHandler(JPEG_Handle.hdmain);
  HAL_MDMA_IRQHandler(JPEG_Handle.hdmaout);  
}

/**
  * @brief  callback for Video Frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbVideoFrame(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_POST_PAINT:
      if(DisplayEvent)
      {
        osMessagePut (DisplayEvent, DISPLAY_COMPLETE_EVENT, 0);
      }
      break;
    default:
      /* The original callback */
      IMAGE_Callback(pMsg);
      break;
  }
}

static void SplashScreenTask(void const *pvParameters)
{
  WM_HWIN                 hVideoFrameWin;
  uint32_t                StartTime, EndTime;
  int32_t                 delay_ms;
  uint32_t                FrameType = 0;
  osEvent                 event;
  GUI_BITMAP              BitmapTarget;

  hVideoFrameWin = (WM_HWIN )pvParameters;

  /*## Initialize the AVI Parser ##########################################*/
  if(AVI_ParserInit(&AVI_Handle, AVI_FILE_ADDRESS) == 0)
  {
    BitmapTarget.XSize        = WM_GetWindowSizeX(hVideoFrameWin);
    BitmapTarget.YSize        = WM_GetWindowSizeY(hVideoFrameWin);
#if defined(FB_BYTES_PER_PIXEL)
    BitmapTarget.BitsPerPixel = (8 * FB_BYTES_PER_PIXEL);
#else
    BitmapTarget.BitsPerPixel = 32;
#endif /* FB_BYTES_PER_PIXEL */
    BitmapTarget.BytesPerLine = (BitmapTarget.XSize * (BitmapTarget.BitsPerPixel/8));
    BitmapTarget.pData        = (const U8*)RGB_OUTPUT_DATA_BUFFER0;
    BitmapTarget.pPal         = NULL;
    switch(BitmapTarget.BitsPerPixel)
    {
      case 16:
        BitmapTarget.pMethods = GUI_DRAW_BMPM565;
      break;
      case 24:
        BitmapTarget.pMethods = GUI_DRAW_BMPM888;
      break;
      default :
      case 32:
#if (GUI_USE_ARGB == 1)
        BitmapTarget.pMethods = GUI_DRAW_BMPM8888I;
#else
        BitmapTarget.pMethods = GUI_DRAW_BMP8888;
#endif
      break;
    }

    /* Create Display Event */
    osMessageQDef(osDisplayQueue, 2, uint32_t);
    DisplayEvent = osMessageCreate (osMessageQ(osDisplayQueue), NULL);
    configASSERT(DisplayEvent);

    /* Associate renderTarget bitmap to the IMAGE widget */
    IMAGE_SetBitmap(hVideoFrameWin, &BitmapTarget);
    WM_SetCallback(hVideoFrameWin, _cbVideoFrame);

    do
    {
      /*## Get a Frame from the AVI file ##################################*/
      FrameType = AVI_GetFrame(&AVI_Handle);
      
      if(FrameType == AVI_VIDEO_FRAME)
      {
        StartTime = osKernelSysTick();
        AVI_Handle.CurrentImage++;
        
        /*## Start decoding the current JPEG frame with DMA (Not Blocking ) Method ################*/
        if(JPEG_Decode_DMA(&JPEG_Handle,(uint32_t)AVI_Handle.pVideoBuffer ,AVI_Handle.FrameSize, (uint32_t)VIDEO_JPEGBuffer))
        {
          break;
        }

        /*## Wait till end of JPEG decoding ###############################*/
        event = osMessageGet(DecodeEvent, osWaitForever);
        if( (event.status != osEventMessage) || (event.value.v != DECODE_COMPLETE_EVENT) )
        {
          break;
        }

        if(AVI_Handle.CurrentImage == 1)
        {
          /*## Get JPEG Info  #############################################*/
          HAL_JPEG_GetInfo(&JPEG_Handle, &JPEG_Info);
        }

        /*## Copy the Decoded frame to the display frame buffer using the DMA2D #*/
        DMA2D_CopyBuffer((void *)VIDEO_JPEGBuffer, &BitmapTarget, &JPEG_Info);

        /* Refresh the screen */
        WM_InvalidateWindow(hVideoFrameWin);
        
        event = osMessageGet( DisplayEvent, osWaitForever );
        if( (event.status != osEventMessage) || (event.value.v != DISPLAY_COMPLETE_EVENT) )
        {
          break;
        }

        BitmapTarget.pData = (BitmapTarget.pData == RGB_OUTPUT_DATA_BUFFER0) ? RGB_OUTPUT_DATA_BUFFER1 : RGB_OUTPUT_DATA_BUFFER0;

        EndTime = osKernelSysTick();

        /* Presentation period */
        delay_ms = ((AVI_Handle.aviInfo.SecPerFrame/1000) - (EndTime - StartTime));
        if(delay_ms > 0)
        {
          osDelay(delay_ms); /* Presentation period */
        }
      }
    } while(AVI_Handle.CurrentImage < AVI_Handle.aviInfo.TotalFrame);
  }

  osMessageDelete(DisplayEvent);
  DisplayEvent = 0;

  if(SplashScreenEvent)
  {
    osMessagePut (SplashScreenEvent, 0, 0);
  }

  hDisplayTask = NULL;
  vTaskDelete( NULL );
}

static uint32_t SplashScreen_WaitForCompletion(void)
{
  osEvent event;

  /* Now block until the splash screen is finished */
  for(;;)
  {
    GUI_Delay(10);
    event = osMessageGet(SplashScreenEvent, 10);
    if( event.status == osEventMessage )
    {
      return event.value.v;
    }
  }
}

/**
  * @brief  Copy the Decoded image to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  ImageWidth: image width
  * @param  ImageHeight: image Height 
  * @retval None
  */
static void DMA2D_CopyBuffer(void *pSrc, void *pDst, JPEG_ConfTypeDef *JPEG_Info)
{
  uint32_t cssMode = JPEG_420_SUBSAMPLING;  
  
  if(JPEG_Info->ChromaSubsampling == JPEG_420_SUBSAMPLING)
  {
    cssMode = DMA2D_CSS_420;  
  }
  else if(JPEG_Info->ChromaSubsampling == JPEG_444_SUBSAMPLING)
  {
    cssMode = DMA2D_NO_CSS;   
  }
  else if(JPEG_Info->ChromaSubsampling == JPEG_422_SUBSAMPLING)
  {
    cssMode = DMA2D_CSS_422;    
  }

  DMA2D_CopyBuffer_YCbCr_To_RGB( 0, pSrc, pDst, JPEG_Info->ImageWidth, JPEG_Info->ImageHeight, cssMode );
}

void SplashScreen_Start(WM_HWIN hVideoFrameWin)
{
  if(JPEG_Decode_Init(&JPEG_Handle) == 0)
  {
    /* Create Display Event */
    osMessageQDef(osSplashScreenQueue, 2, uint32_t);
    SplashScreenEvent = osMessageCreate (osMessageQ(osSplashScreenQueue), NULL);
    configASSERT(SplashScreenEvent);

    /* Measurement Task */
    osThreadDef(SplashScreen, SplashScreenTask, SPLASHSCREEN_THREAD_PRIORITY, 0, SPLASHSCREEN_THREAD_STACK_SIZE);
    hDisplayTask = osThreadCreate (osThread(SplashScreen), (void *)hVideoFrameWin);
    configASSERT(hDisplayTask);

    /* Now block until the splash screen is finished */
    SplashScreen_WaitForCompletion();

    osMessageDelete(SplashScreenEvent);
    SplashScreenEvent = 0;
  }
}

void SplashScreen_Stop(void)
{
  if(SplashScreenEvent)
  {
    AVI_Handle.CurrentImage = (U32 )-1;
  }
}

uint8_t SplashScreen_IsRunning(void)
{
  return (SplashScreenEvent != 0);
}

/**
  * @}
  */

/**
  * @}
  */

