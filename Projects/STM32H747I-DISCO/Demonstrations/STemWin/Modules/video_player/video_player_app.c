/**
  ******************************************************************************
  * @file    videoplayer_app.c
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


/* Private typedef -----------------------------------------------------------*/
typedef struct hwjpeg_context_s {
    JPEG_HandleTypeDef hjpeg;
    uint8_t *input_buffer;
    uint32_t input_buffer_size;
    uint8_t *output_buffer;
    uint32_t output_buffer_size;
    uint32_t input_offset;
    uint32_t output_offset;
} hwjpeg_context_t;

/* Private define ------------------------------------------------------------*/
#define OUTPUT_THREAD_STACK_SIZE        (8 * configMINIMAL_STACK_SIZE)
#define OUTPUT_THREAD_PRIORITY          osPriorityNormal

#define DECODE_CONVERT_OUTPUT_BUFF      0x11

#define FRAME_BUFFER_WIDTH              800
#define FRAME_BUFFER_HEIGHT             480
#define RGB565_BYTES_PER_PIXEL          (uint32_t) (2U)

#define RGB_VIDEO_BUFFER_SIZE           ((uint32_t)(FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * RGB565_BYTES_PER_PIXEL)) /* 750 Kb */
#define RGB_VIDEO_BUFFER_NBR            2

/* Private macro -------------------------------------------------------------*/
#define container_of(ptr, type, member) \
        (struct hwjpeg_context_s *)( (char *)ptr - offsetof(type, member) )

/* Private variables ---------------------------------------------------------*/
static JPEG_YCbCrToRGB_Convert_Function pConvert_Function;
static hwjpeg_context_t hwjpeg_context;
static uint8_t DecodeBufferIndex = 0;
static uint32_t DecodeBufferAddress , DisplayFrameAddress;

/* Buffers for right and left samples */
#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="jpeg_decoder_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".jpeg_decoder_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".jpeg_decoder_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t VIDEO_JPEGBuffer[MAX_CHUNK_SIZE];

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="rgb_video_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".rgb_video_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".rgb_video_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t VIDEO_RGBBuffer[RGB_VIDEO_BUFFER_NBR][RGB_VIDEO_BUFFER_SIZE] ;

static uint32_t MCU_BlockIndex = 0;
static osThreadId hOutputThread = 0;
static osMessageQId OutputEvent = 0;
static uint32_t MCU_TotalNb = 0;
static osSemaphoreId osVidSemph = 0;

JPEG_HandleTypeDef *JPEG_Handle = NULL;

extern uint8_t toggle_chromART;

/* Private function prototypes -----------------------------------------------*/
static void OutputThread(void const *argument);
extern void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine);
extern void DMA2D_CopyBuffer_YCbCr_To_RGB(uint32_t *pSrc, uint16_t xsize, uint16_t ysize);

/* Private functions ---------------------------------------------------------*/

static void JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg);
static void JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo);
static void JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg);
static void JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength);
static void JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData);

/**
  * @brief  Init HArdware Decoder
  * @param  None
  * @retval None
  */
void HW_JPEG_Init(void)
{
  hwjpeg_context.hjpeg.Instance = JPEG;
  hwjpeg_context.hjpeg.MspInitCallback = NULL;
  hwjpeg_context.hjpeg.MspDeInitCallback = NULL;

  HAL_JPEG_Init(&hwjpeg_context.hjpeg);

  /* Setup custom callbacks */
  hwjpeg_context.hjpeg.InfoReadyCallback  = JPEG_InfoReadyCallback;
  hwjpeg_context.hjpeg.DecodeCpltCallback = JPEG_DecodeCpltCallback;
  hwjpeg_context.hjpeg.ErrorCallback      = JPEG_ErrorCallback;
  hwjpeg_context.hjpeg.GetDataCallback    = JPEG_GetDataCallback;
  hwjpeg_context.hjpeg.DataReadyCallback  = JPEG_DataReadyCallback;

  if (toggle_chromART == 1)
  {
    DecodeBufferAddress = (uint32_t )&VIDEO_RGBBuffer[0];
    DisplayFrameAddress = (uint32_t )&VIDEO_RGBBuffer[0];
  }
  
  /* Output FRAME Queue */
  osMessageQDef(OUTPUT_Queue, 6, uint16_t);
  OutputEvent = osMessageCreate (osMessageQ(OUTPUT_Queue), NULL); 
 
  /* Create the Semaphore used by the two threads */
  /* Create Semaphore lock */
//  osSemaphoreDef(Semaphore);
//  osVidSemph = osSemaphoreCreate(osSemaphore(Semaphore), 1);

  /* Output Thread  definition */
  osThreadDef(OUTPUT_THREAD, OutputThread, OUTPUT_THREAD_PRIORITY, 0, OUTPUT_THREAD_STACK_SIZE);
  hOutputThread = osThreadCreate(osThread(OUTPUT_THREAD), NULL);

  /* Public handle referenced in stm32h7xx_it.c */
  JPEG_Handle = &hwjpeg_context.hjpeg;

  JPEG_InitColorTables();

#if defined (DEBUG_HW_JPEG)
  BSP_LED_Off(LED_ORANGE);
  BSP_LED_Off(LED_BLUE);
#endif /* DEBUG_HW_JPEG */
}


/**
  * @brief  Stop HArdware Decoder
  * @param  None
  * @retval None
  */
void HW_JPEG_DeInit(void)
{ 
  HAL_JPEG_DeInit(&hwjpeg_context.hjpeg);
  
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
#if defined (DEBUG_HW_JPEG)
    BSP_LED_Off(LED_ORANGE);
    BSP_LED_Off(LED_BLUE);
#endif /* DEBUG_HW_JPEG */
}

/**
  * @brief  Output Jpeg task
  * @param  None
  * @retval None
  */
static void OutputThread(void const *argument)
{
  osEvent event;
  
  for(;;)
  {
    event = osMessageGet(OutputEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
        case DECODE_CONVERT_OUTPUT_BUFF:
          if(toggle_chromART == 1)
          {
            DMA2D_CopyBuffer_YCbCr_To_RGB((uint32_t *)DisplayFrameAddress, 800, 480);
            if(DisplayFrameAddress == (uint32_t )&VIDEO_RGBBuffer[1])
            {
              DisplayFrameAddress = (uint32_t )&VIDEO_RGBBuffer[0];
            }
            else
            {
              DisplayFrameAddress = (uint32_t )&VIDEO_RGBBuffer[1];
            }  
          }
          else
          {
            MCU_BlockIndex += pConvert_Function( (uint8_t *)VIDEO_JPEGBuffer, (uint8_t *)&VIDEO_RGBBuffer[1], MCU_BlockIndex, MAX_CHUNK_SIZE, NULL);
            if((MCU_BlockIndex == MCU_TotalNb) && (MCU_TotalNb != 0))
            {
              _LCD_DrawBitmap16bpp(0, 0, 0, (uint16_t *)&VIDEO_RGBBuffer[1], 800, 480, 1600);
            }
            else
            {
              HAL_JPEG_Resume(&hwjpeg_context.hjpeg, JPEG_PAUSE_RESUME_OUTPUT); 
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
  HAL_StatusTypeDef ret = HAL_TIMEOUT;

  if (osVidSemph)
  {
    if (osSemaphoreWait(osVidSemph, osWaitForever) == osErrorOS)
      return ret;
  }

  /* Initialize decode context */
  hwjpeg_context.input_buffer = (uint8_t*)pFileData;
  hwjpeg_context.input_buffer_size = (DataSize + 31) &~31;
  hwjpeg_context.output_buffer = (uint8_t*)DecodeBufferAddress;
  hwjpeg_context.output_buffer_size = MAX_CHUNK_SIZE;
  hwjpeg_context.input_offset = 0;
  hwjpeg_context.output_offset = 0;

  ret = HAL_JPEG_Decode_DMA(&hwjpeg_context.hjpeg,
                             hwjpeg_context.input_buffer, MAX_CHUNK_SIZE,
                             hwjpeg_context.output_buffer, MAX_CHUNK_SIZE);

#if defined (DEBUG_HW_JPEG)
  if(ret == HAL_OK)
    BSP_LED_Off(LED_ORANGE);
  else
    BSP_LED_On(LED_ORANGE);
#endif /* DEBUG_HW_JPEG */

  return ret;
}

static void JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
  struct hwjpeg_context_s *context =
    container_of(hjpeg, struct hwjpeg_context_s, hjpeg);

  context->input_offset += NbDecodedData;

  int length = context->input_buffer_size - context->input_offset;

  if (length > 0)
  {
    int size = (length + 4) > MAX_CHUNK_SIZE ? MAX_CHUNK_SIZE : length;

    HAL_JPEG_ConfigInputBuffer(hjpeg,
                               context->input_buffer
                                   + context->input_offset, size);
  }
  else
  {
    HAL_JPEG_ConfigInputBuffer(hjpeg,
                               context->input_buffer
                                   + context->input_offset, 0);
  }
}

/**
  * @brief  JPEG Data Ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pDataOut: pointer to the output data buffer
  * @param OutDataLength: length of output buffer in bytes
  * @retval None
  */
static void JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
  if (toggle_chromART == 1)
  {
    struct hwjpeg_context_s *context = container_of(hjpeg, struct hwjpeg_context_s, hjpeg);
  
    context->output_offset += OutDataLength;
  
    HAL_JPEG_ConfigOutputBuffer(hjpeg,
                                context->output_buffer
                                    + context->output_offset,
                                MAX_CHUNK_SIZE);
  }
  else
  {
    SCB_InvalidateDCache_by_Addr((uint32_t*)pDataOut, MAX_CHUNK_SIZE);
    HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
    HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t*)VIDEO_JPEGBuffer, MAX_CHUNK_SIZE);
  
    if (OutputEvent)
        osMessagePut(OutputEvent, DECODE_CONVERT_OUTPUT_BUFF, 0); /* For software YCbCr/RGB conversion */
  }
}

/**
  * @brief  JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
static void JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
  if(toggle_chromART == 1)
  {
    DecodeBufferIndex = (DecodeBufferIndex == 0? 1 : 0);
    DecodeBufferAddress = (uint32_t )(&VIDEO_RGBBuffer[DecodeBufferIndex]);
    if (OutputEvent)
      osMessagePut(OutputEvent, DECODE_CONVERT_OUTPUT_BUFF, 0);
  }

  if (osVidSemph)
  {
    osSemaphoreRelease(osVidSemph);
  }
#if defined (DEBUG_HW_JPEG)
  BSP_LED_Toggle(LED_BLUE);
#endif /* DEBUG_HW_JPEG */
}

/**
  * @brief  JPEG Info ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pInfo: JPEG Info Struct pointer
  * @retval None
  */
static void JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
  if(toggle_chromART == 0)
  {
    if(JPEG_GetDecodeColorConvertFunc(pInfo, &pConvert_Function, &MCU_TotalNb) == HAL_OK)
    {
    }
  }
}

static void JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
  BSP_ErrorHandler();
}


/**
  * @}
  */

/**
  * @}
  */

