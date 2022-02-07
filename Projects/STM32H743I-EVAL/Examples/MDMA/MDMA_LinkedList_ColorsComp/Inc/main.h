/**
  ******************************************************************************
  * @file    MDMA/MDMA_LinkedList_ColorsComp/Inc/main.h
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
#include "stm32_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MDMA_INSTANCE MDMA_Channel0

#define IMAGE_WIDTH  (uint32_t) (320)
#define IMAGE_HEIGHT (uint32_t) (240)

#define ARGB8888_BYTES_PER_PIXEL  (4U)

#define LCD_FRAME_BUFFER            0xD0000000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

