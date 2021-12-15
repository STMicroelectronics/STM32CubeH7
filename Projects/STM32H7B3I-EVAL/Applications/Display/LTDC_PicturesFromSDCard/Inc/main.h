/**
  ******************************************************************************
  * @file    Display/LTDC_PicturesFromSDCard/Inc/main.h
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

#include "stm32h7b3i_eval.h"
#include "stm32h7b3i_eval_lcd.h"
#include "stm32h7b3i_eval_sdram.h"
#include "stm32h7b3i_eval_ts.h"
#include "stm32_lcd.h"
#include <stdlib.h>
#include "fatfs_storage.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ARGB8888_BYTE_PER_PIXEL       4

/* LTDC foreground layer address 800x480 in ARGB8888 */
#define LCD_FG_LAYER_ADDRESS          LCD_LAYER_0_ADDRESS

/* LTDC background layer address 800x480 in ARGB8888 following the foreground layer */
#define LCD_BG_LAYER_ADDRESS          LCD_FG_LAYER_ADDRESS + (LCD_DEFAULT_WIDTH * LCD_DEFAULT_HEIGHT * ARGB8888_BYTE_PER_PIXEL)

#define INTERNAL_BUFFER_START_ADDRESS LCD_BG_LAYER_ADDRESS + (LCD_DEFAULT_WIDTH * LCD_DEFAULT_HEIGHT * ARGB8888_BYTE_PER_PIXEL)
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BSP_SD_INSTANCE 0U
/** @brief LTDC Background layer index
 */
#define LTDC_ACTIVE_LAYER_BACKGROUND      ((uint32_t) 0)

/** @brief LTDC Foreground layer index
 */
#define LTDC_ACTIVE_LAYER_FOREGROUND      ((uint32_t) 1)
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

