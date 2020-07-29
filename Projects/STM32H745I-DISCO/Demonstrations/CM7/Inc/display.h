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
#ifndef __DISPLAY_H
#define __DISPLAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h745i_discovery_lcd.h"
#include "graphics.h"
#include "stm32_lcd.h"

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
void SG_DisplayGenericButtons(ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
void SG_DisplayBackgroundImage(BackgroundWidgetTypeDef *pImage,uint32_t buttonsNumber);
void SG_DisplayUpDownButtons(ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
void SG_DisplayFrequencyIcon(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint8_t *Frequency);
void SG_DisplayAmplitudeIcon(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint8_t *Amplitude);
void LCD_CopyImage(uint32_t *pSrc,uint32_t *pDst, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorMode, uint32_t InputOffset);
#endif /* __DISPLAY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
