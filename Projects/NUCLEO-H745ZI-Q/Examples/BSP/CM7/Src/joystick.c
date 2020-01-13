/**
  ******************************************************************************
  * @file    BSP/CM7/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the
  *          stm32H745ZI_Nucleo_144 driver
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
uint32_t x_size, y_size;
/* Private function prototypes -----------------------------------------------*/
static void Joystick_SetHint(void);
void Joystick_SetCursorPosition(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Joystick Gpio demo
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
    GUI_SetBackColor(GUI_COLOR_WHITE); 
    GUI_SetTextColor(GUI_COLOR_RED);
    GUI_DisplayStringAt(0, y_size- 70, (uint8_t *)"ERROR", CENTER_MODE);
    GUI_DisplayStringAt(0, y_size- 55, (uint8_t *)"Joystick cannot", CENTER_MODE);
    GUI_DisplayStringAt(0, y_size- 40, (uint8_t *)"be initialized", CENTER_MODE);
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
  * @retval None
  */
static void Joystick_SetHint(void)
{
  /* Clear the LCD */ 
  GUI_Clear(GUI_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  GUI_FillRect(0, 0, x_size, 40, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE); 
  GUI_SetFont(&Font24);

  GUI_SetFont(&Font12);
  GUI_DisplayStringAt(0, 10, (uint8_t *)"Joystick Demo", CENTER_MODE);
  GUI_DisplayStringAt(0, 25, (uint8_t *)"Move the pointer", CENTER_MODE);

  
  /* Set the LCD Text Color */
  GUI_DrawRect(10, 50, x_size - 20, y_size- 60, GUI_COLOR_BLUE);
  GUI_DrawRect(11, 51, x_size - 22, y_size- 61, GUI_COLOR_BLUE);
}

/**
  * @brief  Joystick cursor position
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
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
