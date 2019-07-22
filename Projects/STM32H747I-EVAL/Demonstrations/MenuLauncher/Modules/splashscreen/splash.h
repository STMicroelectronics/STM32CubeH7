/**
  ******************************************************************************
  * @file    splash.h
  * @author  MCD Application Team
  * @brief   Header for splashscreen module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
