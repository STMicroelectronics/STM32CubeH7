/**
  ******************************************************************************
  * @file    BSP/CM7/Src/potentiometer.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the BSP  POTENTIOMETER
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void POTENTIOMETER_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SRAM Demo
  * @param  None
  * @retval None
  */
void POTENTIOMETER_demo (void)
{
  uint32_t potentiometer_val = 0;
  uint32_t v=0,mv=0;
  uint8_t text[50];

  POTENTIOMETER_SetHint();

  /* POTENTIOMETER initialization */
  BSP_POTENTIOMETER_Init();

  BSP_LCD_SetFont(&Font24);
  /* Set the LCD Back Color and Text Color*/
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);

  while(1)
  {
    /* Get potentiometer voltage level */
    potentiometer_val = BSP_POTENTIOMETER_GetLevel();

    /* Display voltage level */
    v=((potentiometer_val* 3300) / 0xFFF) / 1000;
    mv = (((potentiometer_val* 3300) / 0xFFF)%1000)/100;
    sprintf((char*)text," Voltage Level  = %d,%d V",(int)v,(int)mv);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)text, CENTER_MODE);

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
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"POTENTIOMETER", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to get", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"the potentiometer's voltage level ", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
