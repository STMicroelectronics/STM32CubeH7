/**
  ******************************************************************************
  * @file    BSP/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the
  *          stm32746ZG_Nucleo_144 driver
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
uint32_t x_size, y_size;
/* Private function prototypes -----------------------------------------------*/

static void Joystick_SetHint(void);
void Joystick_SetCursorPosition(void);

/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Joystick Gpio demo
  * @param  None
  * @retval None
  */
void Joystick_demo (void)
{

  int32_t status = 0;
  ButtonState = 0;

  ADAFRUIT_802_LCD_GetXSize(0, &x_size);
  ADAFRUIT_802_LCD_GetYSize(0, &y_size);

  Joystick_SetHint();

  status = ADAFRUIT_802_JOY_Init(JOY1,  JOY_MODE_EXTI, JOY_ALL);

  if (status != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size- 70, (uint8_t *)"ERROR", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size- 55, (uint8_t *)"Joystick cannot", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size- 40, (uint8_t *)"be initialized", CENTER_MODE);
  }

  while (1)
  {
    if (status == BSP_ERROR_NONE)
    {
      /* Get the Joystick State */
      JoyState = ADAFRUIT_802_JOY_GetState(JOY1);
      Joystick_SetCursorPosition();
    }
    if(CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay(6);
  }
}

/**
  * @brief  Display joystick demo hint
  * @param  None
  * @retval None
  */
static void Joystick_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set Joystick Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 40, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"Joystick Demo", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 25, (uint8_t *)"Move the pointer", CENTER_MODE);


  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 50, x_size - 20, y_size- 59, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 51, x_size - 22, y_size- 61, UTIL_LCD_COLOR_BLUE);
}

/**
  * @brief  Joystick cursor position
  * @param  None
  * @retval None
  */
void Joystick_SetCursorPosition(void)
{
  static uint16_t xPtr = 12;
  static uint16_t yPtr = 52;
  static uint16_t old_xPtr = 12;
  static uint16_t old_yPtr = 52;


  switch(JoyState)
  {
  case JOY_UP:
    if(yPtr > 52)
    {
      yPtr--;
    }
    break;
  case JOY_DOWN:
    if(yPtr < (y_size - 12 - 11))
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
  * @}
  */

/**
  * @}
  */

