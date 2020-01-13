/**
  ******************************************************************************
  * @file    BSP/CM7/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the
  *          SRM32H747I_EVAL driver
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
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
  JoyStickDemo = 1;
  Joystick_SetHint();
  BSP_JOY_Init(JOY1, JOY_MODE_EXTI, JOY_ALL);

  Joystick_SetCursorPosition();

  while (1)
  {
    if(CheckForUserInput() > 0)
    {

      BSP_JOY_DeInit(JOY1, JOY_ALL);
      ButtonState = 0;
      JoyStickDemo = 0;
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

  GUI_SetBackColor(GUI_COLOR_WHITE);
  GUI_SetTextColor(GUI_COLOR_BLUE);

  if(JoyState == JOY_SEL)
  {
    GUI_SetTextColor(GUI_COLOR_RED);
    GUI_DisplayChar(xPtr, yPtr, 'X');
  }
  else if(JoyState == JOY_NONE)
  {
    GUI_SetTextColor(GUI_COLOR_BLUE);
    GUI_DisplayChar(xPtr, yPtr, 'X');
  }
  else
  {
    GUI_SetTextColor(GUI_COLOR_WHITE);
    GUI_DisplayChar(old_xPtr, old_yPtr, 'X');
    GUI_SetTextColor(GUI_COLOR_BLUE);
    GUI_DisplayChar(xPtr, yPtr, 'X');

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
  GUI_Clear(GUI_COLOR_WHITE);

  /* Set Joystick Demo description */
  GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE);
  GUI_SetFont(&Font24);
  GUI_DisplayStringAt(0, 0, (uint8_t *)"Joystick", CENTER_MODE);
  GUI_SetFont(&Font12);
  GUI_DisplayStringAt(0, 30, (uint8_t *)"Please use the joystick to move", CENTER_MODE);
  GUI_DisplayStringAt(0, 45, (uint8_t *)"the pointer inside the rectangle", CENTER_MODE);
  GUI_DisplayStringAt(0, 60, (uint8_t *)"Press Tamper push-button to switch to next menu", CENTER_MODE);

  /* Set the LCD Text Color */
  GUI_DrawRect(10, 90, x_size - 20, y_size- 100, GUI_COLOR_BLUE);
  GUI_DrawRect(11, 91, x_size - 22, y_size- 102, GUI_COLOR_BLUE);
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
