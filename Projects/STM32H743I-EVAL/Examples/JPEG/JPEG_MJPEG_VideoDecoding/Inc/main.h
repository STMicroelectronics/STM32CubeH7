/**
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecoding/Inc/main.h
  * @author  MCD Application Team  
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "stm32h743i_eval.h"
#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval_sdram.h"
#include "stm32h743i_eval_sd.h"
#include "stm32_lcd.h"

/* Jpeg includes component */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

#include "AVI_parser.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MDMA_INSTANCE MDMA_Channel0

#define LCD_FRAME_BUFFER        0xD0000000
#define JPEG_OUTPUT_DATA_BUFFER0 0xD0600000
#define JPEG_OUTPUT_DATA_BUFFER1 0xD0800000

/*uncomment this line to regulate the decoding frame rate to the native video frame rate */
#define USE_FRAMERATE_REGULATION


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

