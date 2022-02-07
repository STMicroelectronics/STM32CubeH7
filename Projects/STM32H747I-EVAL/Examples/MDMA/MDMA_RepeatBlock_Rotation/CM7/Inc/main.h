/**
  ******************************************************************************
  * @file    MDMA/MDMA_RepeatBlock_Rotation/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7
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
#include "stm32h7xx_hal.h"
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_lcd.h"
#include "stm32_lcd.h"
#include "stm32h747i_eval_sdram.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MDMA_INSTANCE MDMA_Channel0

#define LCD_FRAME_BUFFER        0xD0000000 /* LCD display buffer */
#define MDMA_DEST_FRAME_BUFFER  0xD0200000 /* MDMA destination buffer (drawing buffer) */

#define IMAGE_WIDTH             (480U)
#define IMAGE_HEIGHT            (272U)

#define RGB565_BYTES_PER_PIXEL    (2U)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

