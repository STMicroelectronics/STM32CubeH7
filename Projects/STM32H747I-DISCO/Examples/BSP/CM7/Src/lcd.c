/**
  ******************************************************************************
  * @file    BSP/CM7/Src/lcd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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
#define LCD_FEATURES_NUM                4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Feature = 0;
/* Private function prototypes -----------------------------------------------*/
static void LCD_SetHint(void);
static void LCD_Show_Feature(uint8_t feature);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo (void)
{
  LCD_SetHint();
  LCD_Feature = 0;
  LCD_Show_Feature (LCD_Feature);

  while (1)
  {

    if(CheckForUserInput() > 0)
    {
      if(++LCD_Feature < LCD_FEATURES_NUM)
      {
       LCD_Show_Feature (LCD_Feature);
      }
      else
      {
        return;
      }
    }
    HAL_Delay(100);
  }
}

/**
  * @brief  Display LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
{
  uint32_t x_size;
  uint32_t y_size;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  /* Clear the LCD */
  GUI_Clear(GUI_COLOR_WHITE);

  /*  Set the LCD Text Color */
  GUI_SetTextColor(GUI_COLOR_DARKBLUE);
  GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE);
  GUI_SetFont(&Font24);
  GUI_DisplayStringAt(0, 0, (uint8_t *)"LCD", CENTER_MODE);
  GUI_SetFont(&Font12);
  GUI_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  GUI_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, use Tamper push-button to display", CENTER_MODE);
  GUI_DisplayStringAt(0, 60, (uint8_t *)"next page", CENTER_MODE);

  GUI_DrawRect(10, 90, x_size - 20, y_size- 100, GUI_COLOR_BLUE);
  GUI_DrawRect(11, 91, x_size - 22, y_size- 102, GUI_COLOR_BLUE);
 }

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{220, 220}, {280, 180}, {320, 180}, {370, 220}, {370, 260}, {320, 310}, {280, 310}, {220, 260}};
  Point Points2[3];
  uint32_t x_size, y_size;
  int32_t i = 0;
  uint8_t text[50];
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  Points2[0].X = x_size - 80;
  Points2[0].Y = 150;
  Points2[1].X = x_size - 20;
  Points2[1].Y = 150;
  Points2[2].X = x_size - 20;
  Points2[2].Y = 200;

  GUI_SetBackColor(GUI_COLOR_WHITE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_FillRect(12, 92, x_size - 24, y_size- 104, GUI_COLOR_WHITE);
  GUI_SetTextColor(GUI_COLOR_BLACK);

  switch (feature)
  {
  case 0:
    /* Text Feature */

    GUI_DisplayStringAt(14, 100, (uint8_t *)"Left aligned Text", LEFT_MODE);
    GUI_DisplayStringAt(0, 115, (uint8_t *)"Center aligned Text", CENTER_MODE);
    GUI_DisplayStringAt(14, 130, (uint8_t*)"Right aligned Text", RIGHT_MODE);
    GUI_SetFont(&Font24);
    GUI_DisplayStringAt(14, 180, (uint8_t *)"Font24", LEFT_MODE);
    GUI_SetFont(&Font20);
    GUI_DisplayStringAt(x_size/2 -20, 180, (uint8_t *)"Font20", LEFT_MODE);
    GUI_SetFont(&Font16);
    GUI_DisplayStringAt(x_size - 80, 184, (uint8_t *)"Font16", LEFT_MODE);
    break;

  case 1:


    /* Draw misc. Shapes */
    GUI_SetTextColor(GUI_COLOR_BLACK);
    GUI_DrawRect(20, 100, 60 , 40, GUI_COLOR_BLACK);
    GUI_FillRect(100, 100, 60 , 40, GUI_COLOR_BLACK);

    GUI_DrawCircle(x_size - 120, 120, 20, GUI_COLOR_GRAY);
    GUI_FillCircle(x_size - 40, 120, 20, GUI_COLOR_GRAY);

    GUI_FillPolygon(Points, 8, GUI_COLOR_GREEN);

    GUI_DrawEllipse(130, 170, 30, 20, GUI_COLOR_RED);
    GUI_FillEllipse(200, 170, 30, 20, GUI_COLOR_RED);

    GUI_DrawHLine(20, y_size - 30, x_size / 5, GUI_COLOR_BLACK);
    GUI_DrawLine (100, y_size - 20, 230, y_size- 50, GUI_COLOR_BLACK);
    GUI_DrawLine (100, y_size- 50, 230, y_size- 20, GUI_COLOR_BLACK);

    GUI_DrawPolygon(Points2, 3, GUI_COLOR_GREEN);
    HAL_Delay(2000);
    break;

  case 2:
    /* Draw Bitmap */
    /* Draw Bitmap */
    GUI_DrawBitmap(20, 100, (uint8_t *)stlogo);
    HAL_Delay(500);

    GUI_DrawBitmap(x_size/2 - 40, 100, (uint8_t *)stlogo);
    HAL_Delay(500);

    GUI_DrawBitmap(x_size-100, 100, (uint8_t *)stlogo);
    HAL_Delay(500);

    GUI_DrawBitmap(20, y_size- 80, (uint8_t *)stlogo);
    HAL_Delay(500);

    GUI_DrawBitmap(x_size/2 - 40, y_size- 80, (uint8_t *)stlogo);
    HAL_Delay(500);

    GUI_DrawBitmap(x_size-100, y_size- 80, (uint8_t *)stlogo);
    HAL_Delay(500);
    break;

  case 3:
    GUI_SetFont(&Font24);
    /* Set the LCD Back Color and Text Color*/
    GUI_SetBackColor(GUI_COLOR_WHITE);
    GUI_SetTextColor(GUI_COLOR_GREEN);

    /*  Set the brightness */
    for( i = 100; i >= 0; i-=10)
    {
      sprintf((char*)text," Brightness  =  %d ",(int)i);
      GUI_DisplayStringAt(0, x_size/2 + 45, (uint8_t *)text, CENTER_MODE);
      BSP_LCD_SetBrightness(0,i);
      HAL_Delay(300);
    }

    for( i = 0; i <= 100; i+=10)
    {
      sprintf((char*)text," Brightness  =  %d ",(int)i);
      GUI_DisplayStringAt(0, x_size/2 + 45, (uint8_t *)text, CENTER_MODE);
      BSP_LCD_SetBrightness(0,i);
      HAL_Delay(300);
    }
    break;
  }
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
