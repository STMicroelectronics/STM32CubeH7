/**
  ******************************************************************************
  * @file    splash.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-March-2017
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

/* Includes ------------------------------------------------------------------*/
#include "GUI.h"

void SplashScreen_Start(WM_HWIN hWin);
void SplashScreen_Stop(void);
uint8_t SplashScreen_IsRunning(void);

/* EOF */
