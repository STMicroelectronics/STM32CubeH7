/**
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/CM7/Inc/image_320_240_rgb.h
  * @author  MCD Application Team
  * @brief   Header for RGB 320x240 images definitions .
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
#ifndef __IMAGE_320x240_RGB_H
#define __IMAGE_320x240_RGB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "jpeg_utils.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RGB_IMAGE_WIDTH  ((uint32_t)320)  
#define RGB_IMAGE_HEIGHT ((uint32_t)240)


#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
  #define RGB_IMAGE_SIZE  ((uint32_t)RGB_IMAGE_HEIGHT * RGB_IMAGE_WIDTH * 4)
  extern const uint32_t Image_ARGB8888[RGB_IMAGE_SIZE];

#elif(JPEG_RGB_FORMAT == JPEG_RGB888)
  #define RGB_IMAGE_SIZE  ((uint32_t)RGB_IMAGE_HEIGHT * RGB_IMAGE_WIDTH * 3)
  extern const uint32_t Image_RGB888[RGB_IMAGE_SIZE];

#elif(JPEG_RGB_FORMAT == JPEG_RGB565)
  #define RGB_IMAGE_SIZE  ((uint32_t)RGB_IMAGE_HEIGHT * RGB_IMAGE_WIDTH * 2)
  extern const uint32_t Image_RGB565[RGB_IMAGE_SIZE];

#else
  #error "unknown JPEG_RGB_FORMAT "
#endif /* JPEG_RGB_FORMAT */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* __IMAGE_320x240_RGB_H */
