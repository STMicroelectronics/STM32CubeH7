/**
  ******************************************************************************
  * @file    hwjpeg_decode.c
  * @author  MCD Application Team
  * @brief   HW JPEG decoding module
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


#include <stm32h7xx_hal.h>

#include <cmsis_os.h>

#include "ff.h"
#include "hwjpeg_decode.h"

#define MAX_CHUNK_SIZE  8192

static osMessageQId DecodeEvent;

void HwJPEG_MspInit(struct HwJPEG_Context_s *context)
{
    /* Enable JPEG clock */
    __HAL_RCC_JPGDECEN_CLK_ENABLE();

    /* Enable MDMA clock */
    __HAL_RCC_MDMA_CLK_ENABLE();

    HAL_NVIC_SetPriority(JPEG_IRQn, 0x5, 0xF);
    HAL_NVIC_EnableIRQ(JPEG_IRQn);

    /* Input MDMA */
    context->hmdmaIn.Init.Priority           = MDMA_PRIORITY_HIGH;
    context->hmdmaIn.Init.Endianness         = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    context->hmdmaIn.Init.SourceInc          = MDMA_SRC_INC_BYTE;
    context->hmdmaIn.Init.DestinationInc     = MDMA_DEST_INC_DISABLE;
    context->hmdmaIn.Init.SourceDataSize     = MDMA_SRC_DATASIZE_BYTE;
    context->hmdmaIn.Init.DestDataSize       = MDMA_DEST_DATASIZE_WORD;
    context->hmdmaIn.Init.DataAlignment      = MDMA_DATAALIGN_PACKENABLE;
    context->hmdmaIn.Init.SourceBurst        = MDMA_SOURCE_BURST_32BEATS;
    context->hmdmaIn.Init.DestBurst          = MDMA_DEST_BURST_16BEATS;
    context->hmdmaIn.Init.SourceBlockAddressOffset = 0;
    context->hmdmaIn.Init.DestBlockAddressOffset = 0;

    /*Using JPEG Input FIFO Threshold as a trigger for the MDMA*/
    context->hmdmaIn.Init.Request = MDMA_REQUEST_JPEG_INFIFO_TH; /* Set the MDMA HW trigger to JPEG Input FIFO Threshold flag*/
    context->hmdmaIn.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
    context->hmdmaIn.Init.BufferTransferLength = 32;

    context->hmdmaIn.Instance = MDMA_Channel1;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&context->hjpeg, hdmain, context->hmdmaIn);

    /* DeInitialize the DMA Stream */
    HAL_MDMA_DeInit(&context->hmdmaIn);
    /* Initialize the DMA stream */
    HAL_MDMA_Init(&context->hmdmaIn);

    /* Output MDMA */
    context->hmdmaOut.Init.Priority        = MDMA_PRIORITY_VERY_HIGH;
    context->hmdmaIn.Init.Endianness       = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    context->hmdmaOut.Init.SourceInc       = MDMA_SRC_INC_DISABLE;
    context->hmdmaOut.Init.DestinationInc  = MDMA_DEST_INC_BYTE;
    context->hmdmaOut.Init.SourceDataSize  = MDMA_SRC_DATASIZE_WORD;
    context->hmdmaOut.Init.DestDataSize    = MDMA_DEST_DATASIZE_BYTE;
    context->hmdmaOut.Init.DataAlignment   = MDMA_DATAALIGN_PACKENABLE;
    context->hmdmaOut.Init.SourceBurst     = MDMA_SOURCE_BURST_32BEATS;
    context->hmdmaOut.Init.DestBurst       = MDMA_DEST_BURST_32BEATS;
    context->hmdmaOut.Init.SourceBlockAddressOffset = 0;
    context->hmdmaOut.Init.DestBlockAddressOffset = 0;

    /*Using JPEG Output FIFO Threshold as a trigger for the MDMA*/
    context->hmdmaOut.Init.Request              = MDMA_REQUEST_JPEG_OUTFIFO_TH; /* Set the MDMA HW trigger to JPEG Output FIFO Threshold flag*/
    context->hmdmaOut.Init.TransferTriggerMode  = MDMA_BUFFER_TRANSFER;
    context->hmdmaOut.Init.BufferTransferLength = 32;

    context->hmdmaOut.Instance = MDMA_Channel0;
    /* DeInitialize the DMA Stream */
    HAL_MDMA_DeInit(&context->hmdmaOut);
    /* Initialize the DMA stream */
    HAL_MDMA_Init(&context->hmdmaOut);

    /* Associate the DMA handle */
    __HAL_LINKDMA(&context->hjpeg, hdmaout, context->hmdmaOut);

    HAL_NVIC_SetPriority(MDMA_IRQn, 0x5, 0xF);
    HAL_NVIC_EnableIRQ(MDMA_IRQn);
}

uint32_t JPEG_InitDecode_HW(struct HwJPEG_Context_s *context)
{
    JPEG_InitColorTables();

    HwJPEG_MspInit(context);

    context->hjpeg.Instance = JPEG;
    HAL_JPEG_Init(&context->hjpeg);

    osMessageQDef(decode_queue, 1, uint32_t);
    DecodeEvent = osMessageCreate(osMessageQ(decode_queue), NULL);

    return 0;
}

void JPEG_StopDecode_HW(struct HwJPEG_Context_s *context)
{
    vQueueDelete(DecodeEvent);

    HAL_JPEG_Abort(&context->hjpeg);
}

static void DMA2D_YCbCr_To_RGB_2(uint32_t *pSrc, uint32_t chroma,
                                 uint8_t *destination, uint16_t width,
                                 uint16_t height, uint16_t pitch)
{
    uint32_t inputLineOffset = 0;
    uint32_t regValue, regMask;
    uint32_t chromasubsampling;

    switch (chroma) {
    case JPEG_420_SUBSAMPLING:
        chromasubsampling = DMA2D_CSS_420;
        break;
    case JPEG_422_SUBSAMPLING:
        chromasubsampling = DMA2D_CSS_422;
        break;
    case JPEG_444_SUBSAMPLING:
        chromasubsampling = DMA2D_NO_CSS;
        break;
    default:
        configASSERT(0);
    };

    inputLineOffset = width % 16;
    if(inputLineOffset != 0)
        inputLineOffset = 16 - inputLineOffset;

    regValue = DMA2D_INPUT_YCBCR
               | (DMA2D_REPLACE_ALPHA << 16)
               | (DMA2D_REGULAR_ALPHA << 20)
               | (DMA2D_RB_REGULAR << 21)
               | (0xFFU << 24)
               | (chromasubsampling << 18);

    regMask = DMA2D_BGPFCCR_CM
              | DMA2D_BGPFCCR_AM
              | DMA2D_BGPFCCR_ALPHA
              | DMA2D_BGPFCCR_AI
              | DMA2D_BGPFCCR_RBS
              | DMA2D_FGPFCCR_CSS;

    /* Setup DMA2D Configuration */
    DMA2D->CR      = 0x00010000UL | (1 << 9);
    DMA2D->OPFCCR  = DMA2D_OUTPUT_RGB888;
    DMA2D->OOR     = pitch - width;
    DMA2D->OPFCCR  |= (DMA2D_REGULAR_ALPHA << 20);
    DMA2D->OPFCCR  |= (DMA2D_RB_REGULAR << 21);
    DMA2D->FGPFCCR |= (regMask & regValue);
    DMA2D->FGOR    = inputLineOffset;
    DMA2D->NLR     = (uint32_t)(width << 16) | (uint16_t)height;
    DMA2D->OMAR    = (uint32_t)destination;
    DMA2D->FGMAR   = (uint32_t)pSrc;

    /* Start the transfer, no interrupts */
    DMA2D->CR |= 1;

    /* Poll until transfer is done */
    while (DMA2D->CR & DMA2D_CR_START)
    {
    }
}

int JPEG_Decode_DMA(struct HwJPEG_Context_s *context, uint8_t *buffer,
                    uint32_t length, uint32_t MCUBufferAddress, uint32_t DestAddress)
{
    /* Initialize decode context */
    context->input_buffer = buffer;
    context->input_buffer_size = length;
    context->output_buffer = (uint8_t*)MCUBufferAddress;
    context->output_buffer_size = MAX_CHUNK_SIZE;
    context->input_offset = 0;
    context->output_offset = 0;
    context->chroma_subsampling = (uint32_t)-1; /* invalid */
    context->width = 0;
    context->height = 0;

    /* MDMA can only fetch up-to 65535 32bit words per transfer */
    int size = length > MAX_CHUNK_SIZE ? MAX_CHUNK_SIZE : length;

    HAL_StatusTypeDef ret =
        HAL_JPEG_Decode_DMA(&context->hjpeg, context->input_buffer,
                            size, context->output_buffer, MAX_CHUNK_SIZE);

    if (ret == HAL_OK) {
        osMessageGet(DecodeEvent, 0); /* make sure the queue is empty */
        osEvent event = osMessageGet(DecodeEvent, osWaitForever);

        if (event.status == osEventMessage) {
            if (event.value.v == DECODE_DECODE_COMPLETE_MESSAGE) {
                if (context->chroma_subsampling != JPEG_420_SUBSAMPLING
                    && context->chroma_subsampling != JPEG_422_SUBSAMPLING
                    && context->chroma_subsampling != JPEG_444_SUBSAMPLING)
                    return -1; /* no HAL_JPEG_InfoReadyCallback? */

                DMA2D_YCbCr_To_RGB_2((uint32_t*)MCUBufferAddress,
                                     context->chroma_subsampling,
                                     (uint8_t*)DestAddress,
                                     context->width, context->height, 800);
                return 0;
            } else
                if (event.value.v == DECODE_ERROR_MESSAGE)
                    return -1;
        }

        return -1;
    }

    return -1;
}

void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
    struct HwJPEG_Context_s *context =
        container_of(hjpeg, struct HwJPEG_Context_s, hjpeg);

    /* Assert against the only supported colorspace in this application */
    configASSERT(pInfo->ColorSpace == JPEG_YCBCR_COLORSPACE);

    context->width = pInfo->ImageWidth;
    context->height = pInfo->ImageHeight;
    context->chroma_subsampling = pInfo->ChromaSubsampling;
}

void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
    struct HwJPEG_Context_s *context =
        container_of(hjpeg, struct HwJPEG_Context_s, hjpeg);

    context->input_offset += NbDecodedData;

    int length = context->input_buffer_size - context->input_offset;

    if (length > 0) {
        int size = (length + 4) > MAX_CHUNK_SIZE ? MAX_CHUNK_SIZE : length;

        HAL_JPEG_ConfigInputBuffer(hjpeg,
                                   context->input_buffer
                                       + context->input_offset, size);
    } else
        HAL_JPEG_ConfigInputBuffer(hjpeg,
                                   context->input_buffer
                                       + context->input_offset, 0);
}

void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
    struct HwJPEG_Context_s *context =
        container_of(hjpeg, struct HwJPEG_Context_s, hjpeg);

    context->output_offset += OutDataLength;

    HAL_JPEG_ConfigOutputBuffer(hjpeg,
                                context->output_buffer
                                    + context->output_offset, MAX_CHUNK_SIZE);
}

void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
    osMessagePut(DecodeEvent, DECODE_ERROR_MESSAGE, 0);
}

void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
    osMessagePut(DecodeEvent, DECODE_DECODE_COMPLETE_MESSAGE, 0);
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
#ifdef USE_STM32H747I_EVAL
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
#endif // USE_STM32H747I_EVAL
