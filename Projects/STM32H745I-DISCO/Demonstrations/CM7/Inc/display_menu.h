/**
  ******************************************************************************
  * @file    JPEG/MJPEG_VideoDecoding/Inc/display.h
  * @author  MCD Application Team
  * @brief   Header for display.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
