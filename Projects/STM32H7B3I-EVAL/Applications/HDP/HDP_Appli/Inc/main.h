/**
  ******************************************************************************
  * @file    HDP_Appli/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
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
#include "stdio.h"
#include "string.h"
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_eval.h"
#include "stm32h7b3i_eval_lcd.h"
#include "stm32h7b3i_eval_sdram.h"
#include "stm32h7b3i_eval_io.h"
#include "stm32_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern __IO uint32_t ButtonState;

/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */
#define LCD_FRAME_BUFFER                  0xD0000000 /* LCD Frame buffer of size 800x480 in ARGB8888 */

#endif /* __MAIN_H */

