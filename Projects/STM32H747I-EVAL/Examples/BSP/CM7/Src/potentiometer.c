/**
  ******************************************************************************
  * @file    BSP/Src/potentiometer.c
  * @author  MCD Application Team
  * @brief  This example code shows how to use the BSP  POTENTIOMETER
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
/* Private function prototypes -----------------------------------------------*/
static void POTENTIOMETER_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  POTENTIOMETER Demo
  * @param  None
  * @retval None
  */
void POTENTIOMETER_demo (void)
{
  uint32_t new_level = 0, old_level = 0;
  uint32_t v=0,mv=0;
  char text[50];

  ButtonState = 0;
  POTENTIOMETER_SetHint();

  /* POTENTIOMETER initialization */
  BSP_POT_Init(POT1);

  GUI_SetFont(&Font24);
  /* Set the LCD Back Color and Text Color*/
  GUI_SetBackColor(GUI_COLOR_WHITE);
  GUI_SetTextColor(GUI_COLOR_GREEN);

  while(1)
  {
    /* Get potentiometer voltage level */
    new_level = BSP_POT_GetLevel(POT1);
    old_level=(new_level*0xFFF) /100;
    v=((((old_level* 3300) / 0xFFF) / 1000));
    mv = ((((old_level* 3300) / 0xFFF)%1000)/100);
    sprintf((char*)text," Voltage Level  = %d,%d V",(int)v,(int)mv);
    GUI_DisplayStringAt(0, 480/2 + 45, (uint8_t *)text, CENTER_MODE);
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @brief  Display POTENTIOMETER Demo Hint
  * @param  None
  * @retval None
  */
static void POTENTIOMETER_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  GUI_Clear(GUI_COLOR_WHITE);

  /* Set LCD Demo description */
  GUI_SetTextColor(GUI_COLOR_BLUE);
  GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE);
  GUI_SetFont(&Font24);
  GUI_DisplayStringAt(0, 0, (uint8_t *)"POTENTIOMETER", CENTER_MODE);
  GUI_SetFont(&Font12);
  GUI_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to get", CENTER_MODE);
  GUI_DisplayStringAt(0, 45, (uint8_t *)"the potentiometer's voltage level ", CENTER_MODE);

  /* Set the LCD Text Color */
  GUI_SetTextColor(GUI_COLOR_BLUE);
  GUI_DrawRect(10, 90, x_size - 20, y_size- 100, GUI_COLOR_BLUE);
  GUI_DrawRect(11, 91, x_size - 22, y_size- 102, GUI_COLOR_BLUE);

  GUI_SetTextColor(GUI_COLOR_BLACK);
  GUI_SetBackColor(GUI_COLOR_WHITE);
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
