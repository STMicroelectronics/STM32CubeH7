/**
  ******************************************************************************
  * @file    BSP/CM7/Src/lcd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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

/** @addtogroup STM32FH7xx_HAL_Examples
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
  
  ADAFRUIT_802_LCD_GetXSize(0, &x_size);
  
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 55, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE); 
  UTIL_LCD_SetFont(&Font24);

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"LCD Demo", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 25, (uint8_t *)"Use User BUTTON to", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"display next page", CENTER_MODE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
}

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
   Point Points[]= {{20, 70}, {60, 70}, {60, 100}};
  Point Points2[]= {{80, 70}, {120, 70}, {120, 100}};
  uint32_t x_size, y_size;
  
  ADAFRUIT_802_LCD_GetXSize(0, &x_size);
  ADAFRUIT_802_LCD_GetXSize(0, &y_size);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);    
  UTIL_LCD_FillRect(0, 60, x_size, y_size - 40, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);

  switch (feature)
  {
  case 0:
    /* Text Feature */
    UTIL_LCD_SetFont(&Font24);
    UTIL_LCD_DisplayStringAt(14, 80, (uint8_t *)"Font24", LEFT_MODE); 
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(0, 105, (uint8_t *)"Font20", CENTER_MODE); 
    UTIL_LCD_SetFont(&Font16);
    UTIL_LCD_DisplayStringAt(14, 130, (uint8_t *)"Font16", RIGHT_MODE); 
    break;
    
  case 1:
    /* Draw misc. Shapes part 1*/
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK); 
    UTIL_LCD_DrawRect(20, 70, 40 , 20, UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_FillRect(70, 70, 40 , 20, UTIL_LCD_COLOR_BLACK); 
    
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE); 
    UTIL_LCD_DrawCircle(40, 120, 20, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_FillCircle(90, 120, 20, UTIL_LCD_COLOR_BLUE);
     
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED); 
    UTIL_LCD_DrawLine (20, 70, 20+40 , 70+20, UTIL_LCD_COLOR_RED);
    UTIL_LCD_DrawLine (20, 70+20, 20+40 , 70, UTIL_LCD_COLOR_RED);
    UTIL_LCD_DrawHLine(20, 120, 40, UTIL_LCD_COLOR_RED);
    UTIL_LCD_DrawVLine(40, 100, 40, UTIL_LCD_COLOR_RED);
    break;
 
  case 2:
    /* Draw misc. Shapes part 2*/
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN); 
    UTIL_LCD_DrawPolygon(Points, 3, UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);
    
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED); 
    UTIL_LCD_DrawEllipse(x_size - 100, 110, 20, 30, UTIL_LCD_COLOR_RED);
    UTIL_LCD_FillEllipse(x_size - 40, 110, 20, 30, UTIL_LCD_COLOR_RED);
    break;
  
  case 3:
    /* Set Orientation and Draw Bitmap */
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);
    
    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE); 
    UTIL_LCD_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);    
    
    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_PORTRAIT_ROT180); 
    UTIL_LCD_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);     
    
    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE_ROT180);
    UTIL_LCD_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);    

    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_PORTRAIT); 
    UTIL_LCD_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);
    break;
  }
}

/**
  * @}
  */

/**
  * @}
  */

