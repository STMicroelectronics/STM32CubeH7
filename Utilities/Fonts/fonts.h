/**
  ******************************************************************************
  * @file    fonts.h
  * @author  MCD Application Team
  * @brief   This file provide definition for fonts to be used for STM32xx-EVAL's
  *          LCD driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FONTS_H
#define FONTS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup FONTS
  * @{
  */

/** @defgroup FONTS_Exported_Types Exported Types
  * @{
  */
typedef struct _tFont
{
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
} sFONT;

extern sFONT Font24;
extern sFONT Font20;
extern sFONT Font16;
extern sFONT Font12;
extern sFONT Font8;
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* FONTS_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
