/**
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/Inc/main.h
  * @author  MCD Application Team  
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "stm32h7b3i_discovery.h"
#include "stm32h7b3i_discovery_sd.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* JPEG utility include */
#include "jpeg_utils.h"

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define JPEG_CHROMA_SAMPLING     JPEG_420_SUBSAMPLING   /* Select Chroma Sampling: JPEG_420_SUBSAMPLING, JPEG_422_SUBSAMPLING, JPEG_444_SUBSAMPLING   */
#define JPEG_COLOR_SPACE         JPEG_YCBCR_COLORSPACE  /* Select Color Space: JPEG_YCBCR_COLORSPACE, JPEG_GRAYSCALE_COLORSPACE, JPEG_CMYK_COLORSPACE */
#define JPEG_IMAGE_QUALITY       75                     /* Set Image Quality for Jpeg Encoding */
#define MAX_INPUT_WIDTH          800                    /* Set the Maximum of BMP images Width to be tested */
#define MAX_INPUT_LINES          16                     /* Set Input buffer lines to 16 for YCbCr420, and 8 for YCbCr422 and YCbCr444 (to save RAM space) */


#define LCD_FRAME_BUFFER         0xD0000000

#define BSP_SD_INSTANCE         0
  /* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void RGB_GetInfo(JPEG_ConfTypeDef *pInfo);

#endif /* __MAIN_H */

