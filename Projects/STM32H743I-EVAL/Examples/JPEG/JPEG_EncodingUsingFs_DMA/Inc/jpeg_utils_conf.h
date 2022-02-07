/**
  ******************************************************************************
  * @file    jpeg_utils_conf.h
  * @author  MCD Application Team
  * @brief   jpeg_utils configuration file.
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef  __JPEG_UTILS_CONF_H__
#define  __JPEG_UTILS_CONF_H__


/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_jpeg.h"

/* Private define ------------------------------------------------------------*/
/** @addtogroup JPEG_Private_Defines
  * @{
  */

/* RGB Color format definition for JPEG encoding/Decoding : Should not be modified*/
#define JPEG_ARGB8888            0  /* ARGB8888 Color Format */
#define JPEG_RGB888              1  /* RGB888 Color Format   */
#define JPEG_RGB565              2  /* RGB565 Color Format   */
    
/**
* @}
*/
    
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @addtogroup JPEG_Exported_Defines
  * @{
  */    
#define USE_JPEG_DECODER     0  /* Enable Decoding Post-Processing functions (YCbCr to RGB conversion) */
#define USE_JPEG_ENCODER     1  /* Enable Encoding Pre-Processing functions (RGB to YCbCr conversion)*/

#define JPEG_RGB_FORMAT      JPEG_RGB888  /* Select RGB format: ARGB8888, RGB888, RBG565 */
#define JPEG_SWAP_RB         0  /* Change color order to BGR */

/**
* @}
*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __JPEG_UTILS_CONF_H__ */

