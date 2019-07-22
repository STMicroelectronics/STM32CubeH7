/**
  ******************************************************************************
  * @file    ts_calibration.h
  * @author  MCD Application Team
  * @brief   Header for ts_calibration file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  

#ifndef TS_CALIBRATION_H
#define TS_CALIBRATION_H

#include "main.h"

void  TouchScreen_GetValue(void);
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x);
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y);

#endif /* TS_CALIBRATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
