/**
  ******************************************************************************
  * @file    Display/LTDC_Paint/Src/ts_calibration.c 
  * @author  MCD Application Team
  * @brief   This application code shows how to calibrate the touchscreen.
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
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Applications
  * @{
  */

/** @addtogroup LTDC_Paint
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TS_State_t  TS_State;
static TS_Init_t TS_Init;
static int16_t A1, A2, B1, B2;
static int16_t aPhysX[2], aPhysY[2], aLogX[2], aLogY[2];
static uint8_t Calibration_Done = 0;

/* Private function prototypes -----------------------------------------------*/
static void TouchscreenCalibration_SetHint(void);
static void GetPhysValues(int16_t LogX, int16_t LogY, int16_t * pPhysX, int16_t * pPhysY);
static void WaitForPressedState(uint8_t Pressed);
uint8_t IsCalibrationDone(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Performs the TS calibration
  * @param  None
  * @retval None
  */
void Touchscreen_Calibration(void)
{ 
  uint8_t status = 0;
  uint8_t i = 0;

  TouchscreenCalibration_SetHint();
  
  uint32_t XSize = 0, YSize = 0;
  BSP_LCD_GetXSize(0, &XSize);
  BSP_LCD_GetYSize(0, &YSize);
    
  status = BSP_TS_Init(0, &TS_Init);
  
  if (status != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE); 
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, XSize - 95, (uint8_t *)"ERROR", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0,YSize - 80, (uint8_t *)"Touchscreen cannot be initialized", CENTER_MODE);
  }
  
  while (1)
  {
    if (status == BSP_ERROR_NONE)
    {
      aLogX[0] = 15;
      aLogY[0] = 15;
      aLogX[1] = XSize - 15;
      aLogY[1] = YSize - 15;
      
      for (i = 0; i < 2; i++) 
      {
        GetPhysValues(aLogX[i], aLogY[i], &aPhysX[i], &aPhysY[i]);
      }
      A1 = (1000 * ( aLogX[1] - aLogX[0]))/ ( aPhysX[1] - aPhysX[0]); 
      B1 = (1000 * aLogX[0]) - A1 * aPhysX[0]; 
      
      A2 = (1000 * ( aLogY[1] - aLogY[0]))/ ( aPhysY[1] - aPhysY[0]); 
      B2 = (1000 * aLogY[0]) - A2 * aPhysY[0]; 
      
      Calibration_Done = 1;
      return;
    }
   
    HAL_Delay(5);
  }
}

/**
  * @brief  Displays calibration hint.
  * @param  None
  * @retval None
  */
static void TouchscreenCalibration_SetHint(void)
{
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set Touchscreen Demo description */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);

  uint32_t XSize = 0, YSize = 0;
  BSP_LCD_GetXSize(0, &XSize);
  BSP_LCD_GetYSize(0, &YSize);
    
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, YSize/2 - 27, (uint8_t *)"Before using the Touchscreen", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, YSize/2 - 12, (uint8_t *)"you need to calibrate it.", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, YSize/2 + 3, (uint8_t *)"Press on the black circles", CENTER_MODE);
}

/**
  * @brief  Gets Physical position
  * @param  LogX: Logical X position
  * @param  LogY: Logical Y position
  * @param  pPhysX: Physical X position
  * @param  pPhysY: Physical Y position
  * @retval None
  */
static void GetPhysValues(int16_t LogX, int16_t LogY, int16_t *pPhysX, int16_t *pPhysY) 
{
  /* Draw the ring */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillCircle(LogX, LogY, 5, UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillCircle(LogX, LogY, 2, UTIL_LCD_COLOR_WHITE);
  
  /* Wait until touch is pressed */
  WaitForPressedState(1);
  
  BSP_TS_GetState(0, &TS_State);
  *pPhysX = TS_State.TouchX;
  *pPhysY = TS_State.TouchY; 
  
  /* Wait until touch is released */
  WaitForPressedState(0);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillCircle(LogX, LogY, 5, UTIL_LCD_COLOR_WHITE);
}

/**
  * @brief  Wait for touch detected.
  * @param  Pressed: Touch pressed
  * @retval None
  */
static void WaitForPressedState(uint8_t Pressed) 
{
  TS_State_t  State;
  
  do 
  {
    BSP_TS_GetState(0, &State);
    HAL_Delay(10);
    if (State.TouchDetected == Pressed) 
    {
      uint16_t TimeStart = HAL_GetTick();
      do {
        BSP_TS_GetState(0, &State);      
        HAL_Delay(10);
        if (State.TouchDetected != Pressed) 
        {
          break;
        } else if ((HAL_GetTick() - 100) > TimeStart) 
        {
          return;
        }
      } while (1);
    }
  } while (1);
}

/**
  * @brief  Calibrates X position.
  * @param  x: X position
  * @retval Calibrated x
  */
uint16_t Calibration_GetX(uint16_t x)
{
  return (((A1 * x) + B1)/1000);
}

/**
  * @brief  Calibrates Y position.
  * @param  y: Y position
  * @retval Calibrated y
  */
uint16_t Calibration_GetY(uint16_t y)
{
  return (((A2 * y) + B2)/1000);
}

/**
  * @brief  Checks if the TS is calibrated.
  * @param  None
  * @retval Calibration state (1: Calibrated / 0: Not)
  */
uint8_t IsCalibrationDone(void)
{
  return (Calibration_Done);
}

/**
  * @}
  */ 
  
/**
  * @}
  */
    
