/**
  ******************************************************************************
  * @file    jpeg_utils_conf_template.h
  * @author  MCD Application Team
  * @brief   jpeg_utils configuration template file.
  *          This file should be copied to the application folder and modified 
  *          as follows:
  *            - Rename it to 'jpeg_utils_conf.h'.
  *            - Update the name of the JPEG driver's header file, depending on
  *               the EVAL board you are using, see lines 51 and 52 below (by default this  
  *               file will generate compile error unless you do this modification).
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
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
/*#include "stm32fXxx_hal.h" */
/* #include "stm32fXxx_hal_jpeg.h" */

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
#define USE_JPEG_DECODER     1  /* Enable Decoding Post-Processing functions (YCbCr to RGB conversion) */
#define USE_JPEG_ENCODER     1  /* Enable Encoding Pre-Processing functions (RGB to YCbCr conversion)*/

#define JPEG_RGB_FORMAT      JPEG_ARGB8888  /* Select RGB format: ARGB8888, RGB888, RBG_565 */
#define JPEG_SWAP_RB         0  /* Change color order to BGR */

/**
* @}
*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __JPEG_UTILS_CONF_H__ */
