/**
  ******************************************************************************
  * @file    BSP/Src/lcd.c
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
#define LCD_FEATURES_NUM                5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Feature = 0;
/* Private function prototypes -----------------------------------------------*/
static void LCD_SetHint(void);
static void LCD_Show_Feature(uint8_t feature);
static void ReadPixelTest(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo (void)
{
  LCD_SetHint();
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
        ButtonState = 0;
        return;
      }
    }
  }
}

/**
  * @brief  Display LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
{
  uint32_t x_size, y_size;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  GUI_Clear(GUI_COLOR_WHITE);
  /* Set LCD Demo description */
  GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE);
  GUI_SetFont(&Font24);
  GUI_DisplayStringAt(0, 0, (uint8_t *)"LCD", CENTER_MODE);
  GUI_SetFont(&Font12);
  GUI_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  GUI_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, use TAMPER button to display", CENTER_MODE);
  GUI_DisplayStringAt(0, 60, (uint8_t *)"next page", CENTER_MODE);

   /* Set the LCD Text Color */
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
    HAL_Delay(2000);
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
    /* LCD Read Pixel */
    ReadPixelTest();
    break;

  case 4:
    BSP_SDRAM_DeInit(0);
    BSP_LCD_DeInit(0);
    BSP_LCD_InitEx(0, LCD_ORIENTATION_LANDSCAPE, LCD_PIXEL_FORMAT_RGB565, 640, 480);
    GUI_SetLayer(0);
    GUI_SetFuncDriver(&LCD_Driver);
    GUI_SetFont(&GUI_DEFAULT_FONT);
    /* Clear the LCD */
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_Clear(GUI_COLOR_BLUE);
    BSP_LCD_GetXSize(0, &x_size);
    GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_WHITE);

    /* Set the LCD Text Color */
    GUI_SetTextColor(GUI_COLOR_WHITE);
    GUI_DisplayStringAt(0, y_size/2, (uint8_t *)"Orientation: LANDSCAPE", CENTER_MODE);
    GUI_DisplayStringAt(0, y_size/2 + 45, (uint8_t *)"Pixel Format: RGB565", CENTER_MODE);
    HAL_Delay(2000);

    BSP_SDRAM_DeInit(0);
    BSP_LCD_DeInit(0);
    BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
    GUI_SetLayer(0);
    BSP_LCD_SetLayerVisible(0, 1, DISABLE);
    BSP_LCD_SetLayerVisible(0, 0, ENABLE);
    GUI_SetFuncDriver(&LCD_Driver);
    GUI_SetFont(&GUI_DEFAULT_FONT);
    /* Clear the LCD */
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_Clear(GUI_COLOR_BLUE);
    BSP_LCD_GetXSize(0, &x_size);
    GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_WHITE);
    /* Set the LCD Text Color */
    GUI_SetTextColor(GUI_COLOR_WHITE);
    GUI_DisplayStringAt(0, y_size/2, (uint8_t *)"Orientation: LANDSCAPE", CENTER_MODE);
    GUI_DisplayStringAt(0, y_size/2 + 45, (uint8_t *)"Pixel Format: RGB888", CENTER_MODE);
    HAL_Delay(2000);

    break;
  }
}

/**
  * @brief  LCD Read Pixel test
  * @param  None
  * @retval None
  */
static void ReadPixelTest(void)
{
  uint16_t i, j, k;
  uint32_t x_size, y_size, read_pixel;
  uint32_t colors[23] = {GUI_COLOR_BLUE, GUI_COLOR_GREEN, GUI_COLOR_RED, GUI_COLOR_CYAN, GUI_COLOR_MAGENTA, GUI_COLOR_YELLOW,
  GUI_COLOR_LIGHTBLUE, GUI_COLOR_LIGHTGREEN, GUI_COLOR_LIGHTRED, GUI_COLOR_LIGHTMAGENTA,
  GUI_COLOR_LIGHTYELLOW, GUI_COLOR_DARKBLUE, GUI_COLOR_DARKGREEN, GUI_COLOR_DARKRED, GUI_COLOR_DARKCYAN,
  GUI_COLOR_DARKMAGENTA, GUI_COLOR_DARKYELLOW, GUI_COLOR_LIGHTGRAY, GUI_COLOR_GRAY, GUI_COLOR_DARKGRAY,
  GUI_COLOR_BLACK, GUI_COLOR_BROWN, GUI_COLOR_ORANGE };


  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  for(k = 0; k < 23; k++)
  {
    GUI_Clear(colors[k]);
    for(j = 0; j < y_size; j++)
    {
      for(i = 0; i < x_size; i++)
      {
        GUI_GetPixel(i,j, &read_pixel);
        if(read_pixel != colors[k])
        {
          BSP_LED_On(LED_RED);
        }
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
