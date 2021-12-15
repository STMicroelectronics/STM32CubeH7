/**
  ******************************************************************************
  * @file    BSP/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the
  *          STM32H743I-EVAL driver
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
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t JoyState = JOY_NONE;
uint32_t JoyStickDemo = 0;
/* Private function prototypes -----------------------------------------------*/
static void Joystick_SetHint(void);
static void Joystick_SetCursorPosition(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Joystick demo
  * @param  None
  * @retval None
  */
void Joystick_demo (void)
{
  JoyStickDemo=1;
  Joystick_SetHint();

  BSP_JOY_Init(JOY1, JOY_MODE_EXTI, JOY_ALL);

  Joystick_SetCursorPosition();

  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      JoyStickDemo=0;
      return;
    }
    HAL_Delay(5);
  }
}

void BSP_JOY_Callback(JOY_TypeDef JOY, JOYPin_TypeDef JoyPin)
{
  Joystick_SetCursorPosition();
}

/**
  * @brief  Joystick cursor position
  * @param  None
  * @retval None
  */
static void Joystick_SetCursorPosition(void)
{
  static uint16_t xPtr = 12;
  static uint16_t yPtr = 92;
  static uint16_t old_xPtr = 12;
  static uint16_t old_yPtr = 92;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  JoyState = BSP_JOY_GetState(JOY1);

  switch(JoyState)
  {
  case JOY_UP:
    if(yPtr > 92)
    {
      yPtr--;
    }

    break;
  case JOY_DOWN:
    if(yPtr < (y_size - 12 - 12))
    {
      yPtr++;
    }
    break;
  case JOY_LEFT:
    if(xPtr > 12)
    {
      xPtr--;
    }
    break;
  case JOY_RIGHT:
    if(xPtr < (x_size - 8 - 11))
    {
      xPtr++;
    }
    break;
  default:
    break;
  }

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);

  if(JoyState == JOY_SEL)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayChar(xPtr, yPtr, 'X');
  }
  else if(JoyState == JOY_NONE)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayChar(xPtr, yPtr, 'X');
  }
  else
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DisplayChar(old_xPtr, old_yPtr, 'X');
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayChar(xPtr, yPtr, 'X');

    old_xPtr = xPtr;
    old_yPtr = yPtr;
  }
}

/**
  * @brief  Display joystick demo hint
  * @param  None
  * @retval None
  */
static void Joystick_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set Joystick Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Joystick", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the joystick to move", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"the pointer inside the rectangle", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press Tamper push-button to switch to next menu", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);
}
/**
  * @}
  */

/**
  * @}
  */

