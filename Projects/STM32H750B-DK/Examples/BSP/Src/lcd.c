/**
  ******************************************************************************
  * @file    BSP/Src/lcd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"LCD", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, use Tamper push-button to display", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"next page", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size - 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size - 102, UTIL_LCD_COLOR_BLUE);
 }

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{250, 160}, {270, 140}, {290, 140}, {310, 160}, {310, 180}, {290, 200}, {270, 200}, {250, 180}};
  Point Points2[3];
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  Points2[0].X = x_size - 80;
  Points2[0].Y = y_size - 50;
  Points2[1].X = x_size - 20;
  Points2[1].Y = y_size - 50;
  Points2[2].X = x_size - 20;
  Points2[2].Y = y_size - 100;

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(12, 92, x_size - 24, y_size- 104, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);

  switch (feature)
  {
  case 0:
    /* Text Feature */
    UTIL_LCD_DisplayStringAt(14, 100, (uint8_t *)"Left aligned Text", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(0, 115, (uint8_t *)"Center aligned Text", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(14, 130, (uint8_t*)"Right aligned Text", RIGHT_MODE);
    UTIL_LCD_SetFont(&Font24);
    UTIL_LCD_DisplayStringAt(14, 180, (uint8_t *)"Font24", LEFT_MODE);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(x_size/2 -20, 180, (uint8_t *)"Font20", LEFT_MODE);
    UTIL_LCD_SetFont(&Font16);
    UTIL_LCD_DisplayStringAt(x_size - 80, 184, (uint8_t *)"Font16", LEFT_MODE);
    HAL_Delay(2000);
    break;

  case 1:

    /* Draw misc. Shapes */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(20, 100, 60 , 40, UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_FillRect(100, 100, 60 , 40, UTIL_LCD_COLOR_BLACK);

    UTIL_LCD_DrawCircle(x_size - 120, 120, 20, UTIL_LCD_COLOR_GRAY);
    UTIL_LCD_FillCircle(x_size - 40, 120, 20, UTIL_LCD_COLOR_GRAY);

    UTIL_LCD_FillPolygon(Points, 8, UTIL_LCD_COLOR_GREEN);

    UTIL_LCD_DrawEllipse(130, 170, 30, 20, UTIL_LCD_COLOR_RED);
    UTIL_LCD_FillEllipse(200, 170, 30, 20, UTIL_LCD_COLOR_RED);

    UTIL_LCD_DrawHLine(20, y_size - 30, x_size / 5, UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawLine (100, y_size - 20, 230, y_size- 50, UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawLine (100, y_size- 50, 230, y_size- 20, UTIL_LCD_COLOR_BLACK);

    UTIL_LCD_DrawPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);
    HAL_Delay(2000);
    break;

  case 2:
    /* Draw Bitmap */
    UTIL_LCD_DrawBitmap(20, 100, (uint8_t *)stlogo);
    HAL_Delay(500);

    UTIL_LCD_DrawBitmap(x_size/2 - 40, 100, (uint8_t *)stlogo);
    HAL_Delay(500);

    UTIL_LCD_DrawBitmap(x_size-100, 100, (uint8_t *)stlogo);
    HAL_Delay(500);

    UTIL_LCD_DrawBitmap(20, y_size- 80, (uint8_t *)stlogo);
    HAL_Delay(500);

    UTIL_LCD_DrawBitmap(x_size/2 - 40, y_size- 80, (uint8_t *)stlogo);
    HAL_Delay(500);

    UTIL_LCD_DrawBitmap(x_size-100, y_size- 80, (uint8_t *)stlogo);
    HAL_Delay(500);
    break;

  case 3:
    BSP_SDRAM_DeInit(0);
    BSP_LCD_DeInit(0);
    BSP_LCD_InitEx(0, LCD_ORIENTATION_LANDSCAPE, LCD_PIXEL_FORMAT_RGB565, 480,272);
    UTIL_LCD_SetLayer(0);
    UTIL_LCD_SetFuncDriver(&LCD_Driver);
    UTIL_LCD_SetFont(&UTIL_LCD_DEFAULT_FONT);
    /* Clear the LCD */
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLUE);
    BSP_LCD_GetXSize(0, &x_size);
    UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_WHITE);

    /* Set the LCD Text Color */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DisplayStringAt(0, y_size/2, (uint8_t *)"Orientation: LANDSCAPE", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size/2 + 45, (uint8_t *)"Pixel Format: RGB565", CENTER_MODE);
    HAL_Delay(2000);

    BSP_SDRAM_DeInit(0);
    BSP_LCD_DeInit(0);
    BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
    UTIL_LCD_SetLayer(0);
    BSP_LCD_SetLayerVisible(0, 1, DISABLE);
    BSP_LCD_SetLayerVisible(0, 0, ENABLE);
    UTIL_LCD_SetFuncDriver(&LCD_Driver);
    UTIL_LCD_SetFont(&UTIL_LCD_DEFAULT_FONT);
    /* Clear the LCD */
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLUE);
    BSP_LCD_GetXSize(0, &x_size);
    UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_WHITE);
    /* Set the LCD Text Color */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DisplayStringAt(0, y_size/2, (uint8_t *)"Orientation: LANDSCAPE", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size/2 + 45, (uint8_t *)"Pixel Format: RGB888", CENTER_MODE);
    HAL_Delay(2000);

    break;
  }
}

/**
  * @}
  */

/**
  * @}
  */
