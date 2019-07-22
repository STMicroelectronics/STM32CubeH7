/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Inc/main.h
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
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_lcd.h"
#include "stm32h747i_eval_sdram.h"
#include "stm32h747i_eval_io.h"


#define LCD_FRAME_BUFFER  0xD0000000

#define IMAGE_SIZE_X      240
#define IMAGE_SIZE_Y      130

#define ARGB8888_BYTE_PER_PIXEL  4
#define RGB565_BYTE_PER_PIXEL    2
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
