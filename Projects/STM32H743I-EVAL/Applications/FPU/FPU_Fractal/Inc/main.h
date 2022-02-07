/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/Inc/main.h 
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
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval_ts.h"
#include "stm32h743i_eval_sdram.h"
#include "stm32_lcd.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* Set this define to 1 to OverClocking the system clock to 480MHz*/
#define USE_VOS0_480MHZ_OVERCLOCK 0

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ITERATION       ((uint32_t)256)
#define FRACTAL_ZOOM    ((uint16_t)200)
#define  REAL_CONSTANT  (0.285f)
#define  IMG_CONSTANT  (0.01f)

#define ARGB8888_BYTES_PER_PIXEL 4

#define LCD_FRAME_BUFFER  ((uint32_t)0xD0000000)

#define FRACTAL_FRAME_BUFFER  ((uint32_t)0xD0800000)

#define LCD_X_SIZE ((uint32_t) 640)
#define LCD_Y_SIZE ((uint32_t) 480)

#if (__FPU_USED == 1)
#define SCORE_FPU_MODE                "FPU On"
#else
#define SCORE_FPU_MODE                "FPU Off"
#endif  /* __FPU_USED */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

