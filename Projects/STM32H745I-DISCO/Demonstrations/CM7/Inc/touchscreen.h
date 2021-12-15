/**
  ******************************************************************************
  * @file    touchscreen.h
  * @author  MCD Application Team
  * @brief   This file contains play/pause buttons RGB888 images.
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


