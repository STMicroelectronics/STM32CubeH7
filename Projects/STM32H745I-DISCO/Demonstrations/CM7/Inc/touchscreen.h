/**
  ******************************************************************************
  * @file    touchscreen.h
  * @author  MCD Application Team
  * @brief   This file contains play/pause buttons RGB888 images.
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
#ifndef __TOUCH_SCREEN_H
#define __TOUCH_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h745i_discovery_ts.h"
#include "stm32h745i_discovery_lcd.h"
#include "graphics.h"
#include "main_oscillo_cm7.h"

/* Exported types ------------------------------------------------------------*/


/* Exported Variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
SG_WaveButtonIndex_e SG_GetWaveButtonIndex(SG_SwitchButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
SG_UpDownButtonIndex_e SG_GetUpDownButtonIndex(ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
void SG_UpdateGenericButtons(ButtonWidgetTypeDef *pButton);

uint32_t SG_InitTouchScreen(void);

#endif /* __TOUCH_SCREEN_H */

/*************************** End of file ****************************/

