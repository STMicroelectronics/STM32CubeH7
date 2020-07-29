/**
  ******************************************************************************
  * @file    stm32_lcd.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32_lcd.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_LCD_H
#define STM32_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Fonts/fonts.h"
#include "lcd.h"
#include <stddef.h>

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup STM32 LCD Utility
  * @{
  */

/** @defgroup UTIL_LCD_Exported_Constants STM32 LCD Utility Exported Constants
  * @{
  */
/**
  * @brief  LCD Utility color definitions values
  */
#define UTIL_LCD_COLOR_BLUE          0xFF0000FFUL
#define UTIL_LCD_COLOR_GREEN         0xFF00FF00UL
#define UTIL_LCD_COLOR_RED           0xFFFF0000UL
#define UTIL_LCD_COLOR_CYAN          0xFF00FFFFUL
#define UTIL_LCD_COLOR_MAGENTA       0xFFFF00FFUL
#define UTIL_LCD_COLOR_YELLOW        0xFFFFFF00UL
#define UTIL_LCD_COLOR_LIGHTBLUE     0xFF8080FFUL
#define UTIL_LCD_COLOR_LIGHTGREEN    0xFF80FF80UL
#define UTIL_LCD_COLOR_LIGHTRED      0xFFFF8080UL
#define UTIL_LCD_COLOR_LIGHTCYAN     0xFF80FFFFUL
#define UTIL_LCD_COLOR_LIGHTMAGENTA  0xFFFF80FFUL
#define UTIL_LCD_COLOR_LIGHTYELLOW   0xFFFFFF80UL
#define UTIL_LCD_COLOR_DARKBLUE      0xFF000080UL
#define UTIL_LCD_COLOR_DARKGREEN     0xFF008000UL
#define UTIL_LCD_COLOR_DARKRED       0xFF800000UL
#define UTIL_LCD_COLOR_DARKCYAN      0xFF008080UL
#define UTIL_LCD_COLOR_DARKMAGENTA   0xFF800080UL
#define UTIL_LCD_COLOR_DARKYELLOW    0xFF808000UL
#define UTIL_LCD_COLOR_WHITE         0xFFFFFFFFUL
#define UTIL_LCD_COLOR_LIGHTGRAY     0xFFD3D3D3UL
#define UTIL_LCD_COLOR_GRAY          0xFF808080UL
#define UTIL_LCD_COLOR_DARKGRAY      0xFF404040UL
#define UTIL_LCD_COLOR_BLACK         0xFF000000UL
#define UTIL_LCD_COLOR_BROWN         0xFFA52A2AUL
#define UTIL_LCD_COLOR_ORANGE        0xFFFFA500UL

/* Defintion of Official ST Colors */
#define UTIL_LCD_COLOR_ST_BLUE_DARK   0xFF002052UL
#define UTIL_LCD_COLOR_ST_BLUE        0xFF39A9DCUL
#define UTIL_LCD_COLOR_ST_BLUE_LIGHT  0xFFD1E4F3UL
#define UTIL_LCD_COLOR_ST_GREEN_LIGHT 0xFFBBCC01UL
#define UTIL_LCD_COLOR_ST_GREEN_DARK  0xFF003D14UL
#define UTIL_LCD_COLOR_ST_YELLOW      0xFFFFD300UL
#define UTIL_LCD_COLOR_ST_BROWN       0xFF5C0915UL
#define UTIL_LCD_COLOR_ST_PINK        0xFFD4007AUL
#define UTIL_LCD_COLOR_ST_PURPLE      0xFF590D58UL
#define UTIL_LCD_COLOR_ST_GRAY_DARK   0xFF4F5251UL
#define UTIL_LCD_COLOR_ST_GRAY        0xFF90989EUL
#define UTIL_LCD_COLOR_ST_GRAY_LIGHT  0xFFB9C4CAUL

/**
  * @brief LCD Utility default font
  */
#define UTIL_LCD_DEFAULT_FONT        Font24

/**
  * @}
  */

/** @defgroup UTIL_LCD_Exported_Types STM32 LCD Utility Exported Types
  * @{
  */
#define LINE(x) ((x) * (((sFONT *)UTIL_LCD_GetFont())->Height))
/**
  * @}
  */

/** @defgroup UTIL_LCD_Exported_Types STM32 LCD Utility Exported Types
  * @{
  */

/**
  * @brief  LCD Utility Drawing main properties
  */
typedef struct
{
  uint32_t  TextColor; /*!< Specifies the color of text */
  uint32_t  BackColor; /*!< Specifies the background color below the text */
  sFONT    *pFont;     /*!< Specifies the font used for the text */
  uint32_t  LcdLayer;
  uint32_t  LcdDevice;
  uint32_t  LcdXsize;
  uint32_t  LcdYsize;
  uint32_t  LcdPixelFormat;
} UTIL_LCD_Ctx_t;

/**
  * @brief  LCD Utility Drawing point (pixel) geometric definition
  */
typedef struct
{
  int16_t X; /*!< geometric X position of drawing */
  int16_t Y; /*!< geometric Y position of drawing */
} Point;

/**
  * @brief  Pointer on LCD Utility Drawing point (pixel) geometric definition
  */
typedef Point * pPoint;

/**
  * @brief  LCD Utility drawing Line alignment mode definitions
  */
typedef enum
{
  CENTER_MODE             = 0x01,    /*!< Center mode */
  RIGHT_MODE              = 0x02,    /*!< Right mode  */
  LEFT_MODE               = 0x03     /*!< Left mode   */
} Text_AlignModeTypdef;

/**
  * @}
  */

/** @addtogroup UTIL_LCD_Exported_Functions
  * @{
  */
void     UTIL_LCD_SetFuncDriver(const LCD_UTILS_Drv_t *pDrv);

void     UTIL_LCD_SetLayer(uint32_t Layer);
void     UTIL_LCD_SetDevice(uint32_t Device);

void     UTIL_LCD_SetTextColor(uint32_t Color);
uint32_t UTIL_LCD_GetTextColor(void);
void     UTIL_LCD_SetBackColor(uint32_t Color);
uint32_t UTIL_LCD_GetBackColor(void);
void     UTIL_LCD_SetFont(sFONT *fonts);
sFONT    *UTIL_LCD_GetFont(void);

void     UTIL_LCD_Clear(uint32_t Color);
void     UTIL_LCD_ClearStringLine(uint32_t Line);
void     UTIL_LCD_DisplayStringAtLine(uint32_t Line, uint8_t *ptr);
void     UTIL_LCD_DisplayStringAt(uint32_t Xpos, uint32_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
void     UTIL_LCD_DisplayChar(uint32_t Xpos, uint32_t Ypos, uint8_t Ascii);
void     UTIL_LCD_GetPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color);
void     UTIL_LCD_SetPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color);
void     UTIL_LCD_FillRGBRect(uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height);
void     UTIL_LCD_DrawHLine(uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
void     UTIL_LCD_DrawVLine(uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
void     UTIL_LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pData);
void     UTIL_LCD_FillRect(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color);
void     UTIL_LCD_DrawLine(uint32_t Xpos1, uint32_t Ypos1, uint32_t Xpos2, uint32_t Ypos2, uint32_t Color);
void     UTIL_LCD_DrawRect(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color);
void     UTIL_LCD_DrawCircle(uint32_t Xpos, uint32_t Ypos, uint32_t Radius, uint32_t Color);
void     UTIL_LCD_DrawPolygon(pPoint Points, uint32_t PointCount, uint32_t Color);
void     UTIL_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint32_t Color);
void     UTIL_LCD_FillCircle(uint32_t Xpos, uint32_t Ypos, uint32_t Radius, uint32_t Color);
void     UTIL_LCD_FillPolygon(pPoint Points, uint32_t PointCount, uint32_t Color);
void     UTIL_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint32_t Color);

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
