/**
  ******************************************************************************
  * @file    JPEG/MJPEG_VideoDecoding/Inc/display.h
  * @author  MCD Application Team
  * @brief   Header for display.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_MENU_H
#define __DISPLAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h745i_discovery_lcd.h"
#include "graphics.h"
#include "stm32_lcd.h"

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define LCD_FRAME_BUFFER         0xD0000000


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void InitMenuDisplay(void);
void DisplayGenericButtons(ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
void DisplayMenuImages(BackgroundWidgetTypeDef *pImage,uint32_t buttonsNumber);
void LCD_CopyImage(uint32_t *pSrc,uint32_t *pDst, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorMode, uint32_t InputOffset);
#endif /* __DISPLAY_H */

