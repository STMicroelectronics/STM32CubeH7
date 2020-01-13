/**
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecoding/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "stm32h747i_eval.h"
#include "stm32h747i_eval_lcd.h"
#include "basic_gui.h"
#include "stm32h747i_eval_sdram.h"
#include "stm32h747i_eval_sd.h"

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
