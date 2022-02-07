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

#ifndef  __JPEG_UTILS_CONF_H__
#define  __JPEG_UTILS_CONF_H__


/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_jpeg.h"

/* Exported types ------------------------------------------------------------*/
/** @addtogroup JPEG_Exported_Defines
  * @{
  */
/* Exported constants --------------------------------------------------------*/

/* RGB Color format definition for JPEG encoding/Decoding : Should not be modified*/
#define JPEG_ARGB8888            0  /* ARGB8888 Color Format */
#define JPEG_RGB888              1  /* RGB888 Color Format   */
#define JPEG_RGB565              2  /* RGB565 Color Format   */

#define USE_JPEG_DECODER 1

#define JPEG_RGB_FORMAT JPEG_RGB888

/**
* @}
*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __JPEG_UTILS_CONF_H__ */
