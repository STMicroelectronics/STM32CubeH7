/**
  ******************************************************************************
  * @file    BSP/CM7/Src/lcd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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
  GUI_Clear(GUI_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  GUI_FillRect(0, 0, x_size, 55, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE); 
  GUI_SetFont(&Font24);

  GUI_SetFont(&Font12);
  GUI_DisplayStringAt(0, 10, (uint8_t *)"LCD Demo", CENTER_MODE);
  GUI_DisplayStringAt(0, 25, (uint8_t *)"Use User BUTTON to", CENTER_MODE);
  GUI_DisplayStringAt(0, 40, (uint8_t *)"display next page", CENTER_MODE);

  GUI_SetTextColor(GUI_COLOR_BLUE);
  GUI_SetBackColor(GUI_COLOR_WHITE);
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
  
  GUI_SetBackColor(GUI_COLOR_WHITE);  
  GUI_SetTextColor(GUI_COLOR_WHITE);    
  GUI_FillRect(0, 60, x_size, y_size - 40, GUI_COLOR_WHITE);
  GUI_SetTextColor(GUI_COLOR_BLACK);

  switch (feature)
  {
  case 0:
    /* Text Feature */
    GUI_SetFont(&Font24);
    GUI_DisplayStringAt(14, 80, (uint8_t *)"Font24", LEFT_MODE); 
    GUI_SetFont(&Font20);
    GUI_DisplayStringAt(0, 105, (uint8_t *)"Font20", CENTER_MODE); 
    GUI_SetFont(&Font16);
    GUI_DisplayStringAt(14, 130, (uint8_t *)"Font16", RIGHT_MODE); 
    break;
    
  case 1:
    /* Draw misc. Shapes part 1*/
    GUI_SetTextColor(GUI_COLOR_BLACK); 
    GUI_DrawRect(20, 70, 40 , 20, GUI_COLOR_BLACK);
    GUI_FillRect(70, 70, 40 , 20, GUI_COLOR_BLACK); 
    
    GUI_SetTextColor(GUI_COLOR_BLUE); 
    GUI_DrawCircle(40, 120, 20, GUI_COLOR_BLUE);
    GUI_FillCircle(90, 120, 20, GUI_COLOR_BLUE);
     
    GUI_SetTextColor(GUI_COLOR_RED); 
    GUI_DrawLine (20, 70, 20+40 , 70+20, GUI_COLOR_RED);
    GUI_DrawLine (20, 70+20, 20+40 , 70, GUI_COLOR_RED);
    GUI_DrawHLine(20, 120, 40, GUI_COLOR_RED);
    GUI_DrawVLine(40, 100, 40, GUI_COLOR_RED);
    break;
 
  case 2:
    /* Draw misc. Shapes part 2*/
    GUI_SetTextColor(GUI_COLOR_GREEN); 
    GUI_DrawPolygon(Points, 3, GUI_COLOR_GREEN);
    GUI_FillPolygon(Points2, 3, GUI_COLOR_GREEN);
    
    GUI_SetTextColor(GUI_COLOR_RED); 
    GUI_DrawEllipse(x_size - 100, 110, 20, 30, GUI_COLOR_RED);
    GUI_FillEllipse(x_size - 40, 110, 20, 30, GUI_COLOR_RED);
    break;
  
  case 3:
    /* Set Orientation and Draw Bitmap */
    GUI_Clear(GUI_COLOR_WHITE);
    GUI_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);
    
    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE); 
    GUI_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);    
    
    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_PORTRAIT_ROT180); 
    GUI_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);     
    
    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE_ROT180);
    GUI_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(1000);    

    ADAFRUIT_802_LCD_DeInit(0);
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_PORTRAIT); 
    GUI_DrawBitmap(20, 70, (uint8_t *)stlogo);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
