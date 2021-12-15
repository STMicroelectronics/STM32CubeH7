/**
  ******************************************************************************
  * @file    BSP/Src/potentiometer.c
  * @author  MCD Application Team
  * @brief  This example code shows how to use the BSP  POTENTIOMETER
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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

  UTIL_LCD_SetFont(&Font24);
  /* Set the LCD Back Color and Text Color*/
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);

  while(1)
  {
    /* Get potentiometer voltage level */
    new_level = BSP_POT_GetLevel(POT1);
    old_level=(new_level*0xFFF) /100;
    v=((((old_level* 3300) / 0xFFF) / 1000));
    mv = ((((old_level* 3300) / 0xFFF)%1000)/100);
    sprintf((char*)text," Voltage Level  = %d,%d V",(int)v,(int)mv);
    UTIL_LCD_DisplayStringAt(0, 480/2 + 45, (uint8_t *)text, CENTER_MODE);
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
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"POTENTIOMETER", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to get", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"the potentiometer's voltage level ", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
}

/**
  * @}
  */

/**
  * @}
  */
