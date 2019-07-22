/**
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/Inc/image_320_240_rgb.h
  * @author  MCD Application Team
  * @brief   Header for RGB 320x240 images definitions .
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
/*************************** End of file ****************************/
