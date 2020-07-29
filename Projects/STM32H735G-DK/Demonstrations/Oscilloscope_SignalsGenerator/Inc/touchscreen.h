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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32h735g_discovery_ts.h"
#include "stm32h735g_discovery_lcd.h"
#include "graphics.h"
#include "sig_gen.h"

/* Exported types ------------------------------------------------------------*/


/* Exported Variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
SG_WaveButtonIndex_e SG_GetWaveButtonIndex(SG_SwitchButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
SG_UpDownButtonIndex_e SG_GetUpDownButtonIndex(SG_ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber);
uint32_t SG_InitTouchScreen(void);

#endif /* __TOUCH_SCREEN_H */

/*************************** End of file ****************************/

