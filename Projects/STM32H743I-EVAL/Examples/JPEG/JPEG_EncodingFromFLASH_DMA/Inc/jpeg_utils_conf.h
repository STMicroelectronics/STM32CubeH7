/**
  ******************************************************************************
  * @file    jpeg_utils_conf.h
  * @author  MCD Application Team
  * @brief   jpeg_utils configuration file.
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

#define JPEG_RGB_FORMAT      JPEG_RGB565  /* Select RGB format: ARGB8888, RGB888, RBG565 */
#define JPEG_SWAP_RB         0  /* Change color order to BGR */

/**
* @}
*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __JPEG_UTILS_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
