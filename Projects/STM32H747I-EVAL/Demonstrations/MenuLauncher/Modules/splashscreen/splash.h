/**
  ******************************************************************************
  * @file    splash.h
  * @author  MCD Application Team
  * @brief   Header for splashscreen module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPLASH_H
#define __SPLASH_H

/* Includes ------------------------------------------------------------------*/
#include "GUI.h"

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SplashScreen_Start(WM_HWIN hWin);
void SplashScreen_Stop(void);
uint8_t SplashScreen_IsRunning(void);

#endif /* __SPLASH_H */

