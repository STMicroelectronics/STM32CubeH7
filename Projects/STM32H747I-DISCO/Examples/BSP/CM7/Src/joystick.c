/**
  ******************************************************************************
  * @file    BSP/CM7/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the
  *          stm32h747i_discovery driver
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HEADBAND_HEIGHT                 80
 __IO uint32_t JoyPinPressed = 0;
__IO uint32_t Joy_State ;
__IO uint32_t PreviousPinState=0;
  uint32_t x_size, y_size;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

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
  BSP_JOY_Init(JOY1, JOY_MODE_EXTI,JOY_ALL);

   JoyPinPressed = 0;
  while (1)
  {
          switch(JoyPinPressed)
      {
      case 0x01U:
        Joy_State = JOY_SEL;
        break;

      case 0x02U:
        Joy_State = JOY_DOWN;
        break;

      case 0x04U:
        Joy_State = JOY_LEFT;
        break;

      case 0x08U:
        Joy_State = JOY_RIGHT;
        break;

      case 0x10U:
        Joy_State = JOY_UP;
        break;
      default:
        Joy_State = JOY_NONE;
        break;
      }
    Joystick_SetCursorPosition();
    JoyPinPressed = 0;
    if(CheckForUserInput() > 0)
    {

      BSP_JOY_DeInit(JOY1, JOY_ALL);
      ButtonState = 0;
      JoyStickDemo = 0;
      return;
    }
     HAL_Delay(6);
  }
}

void BSP_JOY_Callback(JOY_TypeDef JOY, uint32_t JoyPin)
{
    JoyPinPressed = JoyPin;
}
/**
* @brief  Joystick cursor position
* @param  None
* @retval None
*/
static void Joystick_SetCursorPosition()
{
  static uint16_t xPtr = 12;
  static uint16_t yPtr = HEADBAND_HEIGHT + 12;
  static uint16_t old_xPtr = 12;
  static uint16_t old_yPtr = HEADBAND_HEIGHT + 12;


  switch(Joy_State)
  {
  case JOY_UP:
    if(yPtr > HEADBAND_HEIGHT + 12)
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
    if(xPtr < (y_size - 8 - 11))
    {
      xPtr++;
    }
    break;
  default:

    break;
  }

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);

  if(Joy_State == JOY_SEL)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayChar(xPtr, yPtr, 'X');

  }
  else if(Joy_State == JOY_NONE)
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

