/**
  ******************************************************************************
  * @file    hwjpeg_decode.h
  * @author  MCD Application Team
  * @brief   header of hwjpeg_decode.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __DECODE_DMA_H
#define __DECODE_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "jpeg_utils.h"

#define DECODE_ERROR_MESSAGE              0xFFFF
#define DECODE_DECODE_COMPLETE_MESSAGE    0xFFD8

#define JPEG_SOI_MARKER                 (0xFFD8) /* JPEG Start Of Image marker*/
#define JPEG_SOI_MARKER_BYTE0           (JPEG_SOI_MARKER & 0xFF)
#define JPEG_SOI_MARKER_BYTE1           ((JPEG_SOI_MARKER >> 8) & 0xFF)

struct HwJPEG_Context_s {
    MDMA_HandleTypeDef hmdmaIn;
    MDMA_HandleTypeDef hmdmaOut;
    JPEG_HandleTypeDef hjpeg;
    uint8_t *input_buffer;
    uint32_t input_buffer_size;
    uint8_t *output_buffer;
    uint32_t output_buffer_size;
    uint32_t input_offset;
    uint32_t output_offset;
    uint32_t chroma_subsampling; /* JPEG_420_SUBSAMPLING, JPEG_422_SUBSAMPLING or JPEG_444_SUBSAMPLING */
    uint32_t width;
    uint32_t height;
};

#define container_of(ptr, type, member) \
        (struct HwJPEG_Context_s *)( (char *)ptr - offsetof(type,member) )

uint32_t JPEG_InitDecode_HW(struct HwJPEG_Context_s *context);
int JPEG_Decode_DMA(struct HwJPEG_Context_s *context, uint8_t *buffer, uint32_t length, uint32_t MCUBufferAddress, uint32_t DestAddress, uint32_t DestPitch);
void JPEG_StopDecode_HW(struct HwJPEG_Context_s *context);

#ifdef __cplusplus
}
#endif

#endif /* __DECODE_DMA_H */
