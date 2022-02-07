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
#ifndef __DISPLAY_H
#define __DISPLAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h735g_discovery.h"
#include "stm32h735g_discovery_lcd.h"
#include "stm32_lcd.h"
#include "graphics.h"
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BUTTON_STATE_OFF            0U 
#define BUTTON_STATE_ON             1U
#define LCD_FRAME_BUFFER         0xD0000000

#define SG_WAVE_BUTTON_XPOSITION       6
#define SG_WAVE_BUTTON_WIDTH           64
#define SG_WAVE_BUTTON_HEIGHT          32
#define SG_TOTAL_WAVE_BUTTON_NBR       6
/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void SG_InitDisplay(void);
void SG_DisplaySwitchButton(SG_SwitchButtonWidgetTypeDef *pButton,uint32_t *pDst, uint8_t ButtonState);
void SG_DisplayWaveButtons(SG_SwitchButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
void SG_SwitchWaveButtonState(SG_SwitchButtonWidgetTypeDef *pButton, uint32_t buttonIndex, uint32_t buttonsNumber);
void SG_DisplayBackgroundImage(SG_BackgroundWidgetTypeDef *pImage);
void SG_DisplayUpDownButtons(SG_ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
void SG_DisplayFrequencyIcon(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint8_t *Frequency);
void SG_DisplayAmplitudeIcon(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint8_t *Amplitude);
void LCD_CopyImage(uint32_t *pSrc,uint32_t *pDst, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorMode, uint32_t InputOffset);
#endif /* __DISPLAY_H */

