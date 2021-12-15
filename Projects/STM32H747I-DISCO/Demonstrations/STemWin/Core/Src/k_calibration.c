  /**
  ******************************************************************************
  * @file    k_calibration.c
  * @author  MCD Application Team
  * @brief   This example code shows how to calibrate the touchscreen.
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

/* Includes ------------------------------------------------------------------*/
#include "k_calibration.h"

/** @addtogroup Core
  * @{
  */

/** @addtogroup calibration
  * @{
  */

/* Imported variables */
#define CALIB_RES(val,pte1,ptm1,pte2,ptm2) ((((val-ptm2)*(pte1-pte2))/(ptm1-ptm2))+pte2)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO CALIBRATION_Data1Typedef data1;
static __IO CALIBRATION_Data2Typedef data2;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void TouchScreen_GetValue(void)
{
  data1.d32 = READ_REG(BKP_REG_CALIB_DR0);
  data2.d32 = READ_REG(BKP_REG_CALIB_DR1);
}

/**check if the TS is calibrated
  * @param  None
* @retval calibration state (1 : calibrated / 0: no)
  */
uint8_t TouchScreen_IsCalibrationDone(void)
{
  return (data2.b.IsCalibrated);
}

/**
  * @brief  Calibrate x position (to obtain X = calibrated(x))
  * @param  x : X position
  * @retval calibrated x
  */
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x)
{
  if(!TouchScreen_IsCalibrationDone())
    return x;

  int32_t ret=CALIB_RES(x,30,data1.b.A1,770,data1.b.B1);
  if (ret<0) ret=0;
  else if  (ret>800) ret=799;
  return ret;
}

/**
  * @brief  Calibrate Y position
  * @param  y : Y position
  * @retval calibrated y
  */
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y)
{
  if(!TouchScreen_IsCalibrationDone())
    return y;

  int32_t ret=CALIB_RES(y,30,data2.b.A2,450,data2.b.B2);
  if (ret<0) ret=0;
  else if  (ret>800) ret=799;
  return ret;
}

/**
  * @}
  */

/**
  * @}
  */

