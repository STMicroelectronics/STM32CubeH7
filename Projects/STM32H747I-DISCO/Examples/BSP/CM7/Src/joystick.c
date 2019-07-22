/**
  ******************************************************************************
  * @file    BSP/CM7/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the 
  *          stm32h747i_discovery driver
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static JOYState_TypeDef JoyState = JOY_NONE;
  
/* Private function prototypes -----------------------------------------------*/
static void Joystick_SetHint(uint32_t Index);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Joystick Gpio demo
  * @param  None
  * @retval None
  */
void Joystick_demo (void)
{ 

  uint8_t status = 0;

  Joystick_SetHint(0);
  
  status = BSP_JOY_Init(JOY_MODE_GPIO);
  
  if (status != HAL_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }
  
  while (1)
  {
    if (status == HAL_OK)
    {
      /* Get the Joystick State */
      JoyState = BSP_JOY_GetState();
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
  * @brief  Joystick EXTI demo
  * @param  None
  * @retval None
  */
void Joystick_exti_demo (void)
{
  uint8_t status = 0;
  uint32_t joy_pin;
  Joystick_SetHint(1);
  
  status = BSP_JOY_Init(JOY_MODE_EXTI);
  
  if (status != HAL_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }
  
  JoyState = JOY_NONE;
  Joystick_SetCursorPosition();
  while (1)
  {
    if((JoystickStates & 0x0F00) == 0x0100)
    {
      joy_pin = JoystickStates & 0x000F;
      switch(joy_pin)
      {
      case 0:
        JoyState = JOY_SEL;
        break;
        
      case 1:
        JoyState = JOY_DOWN;
        break;
        
      case 2:
        JoyState = JOY_LEFT;
        break; 
        
      case 3:
        JoyState = JOY_RIGHT;
        break; 
        
      case 4:
        JoyState = JOY_UP;
        break;
      default:
        JoyState = JOY_NONE;
        break;
      }
      
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
  * @brief  Joystick cursor position
  * @param  None
  * @retval None
  */
void Joystick_SetCursorPosition(void)
{
  static uint16_t xPtr = 12;
  static uint16_t yPtr = HEADBAND_HEIGHT + 12;
  static uint16_t old_xPtr = 12;
  static uint16_t old_yPtr = HEADBAND_HEIGHT + 12;

      
  switch(JoyState)
  {
  case JOY_UP:
    if(yPtr > HEADBAND_HEIGHT + 12)
    {
      yPtr--;
    }
    break;
  case JOY_DOWN:
    if(yPtr < (BSP_LCD_GetYSize() - 12 - 11))
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
    if(xPtr < (BSP_LCD_GetXSize() - 8 - 11))
    {
      xPtr++;
    }
    break;
  default:
    break;
  }

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

  if(JoyState == JOY_SEL)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayChar(xPtr, yPtr, 'X');

  }
  else if(JoyState == JOY_NONE)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayChar(xPtr, yPtr, 'X');
  }
  else
  {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayChar(old_xPtr, old_yPtr, 'X');
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayChar(xPtr, yPtr, 'X');

    old_xPtr = xPtr;
    old_yPtr = yPtr;
  }
}

/**
  * @brief  Display joystick demo hint
  * @param  None
  * @retval None
  */
static void Joystick_SetHint(uint32_t Index)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  if(Index == 0)
  {
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Joystick in POLLING mode", CENTER_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Joystick in EXTI mode", CENTER_MODE);
  }
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the joystick to move", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"the pointer inside the rectangle", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press Tamper push-button to switch to next menu", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
