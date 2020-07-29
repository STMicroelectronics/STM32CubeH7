/**
  ******************************************************************************
  * @file    stm32_lcd.c
  * @author  MCD Application Team
  * @brief   This file includes the basic functionalities to drive LCD
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

/* File Info: ------------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is a generic driver that provides basic display services. It should
     be used by any platform once LCD is initialized and following draw methods are
     registered:
         BSP_LCD_DrawBitmap
         BSP_LCD_DrawHLine
         BSP_LCD_DrawVLine
         BSP_LCD_FillRect
         BSP_LCD_ReadPixel
         BSP_LCD_WritePixel
         BSP_LCD_GetXSize
         BSP_LCD_GetYSize
         BSP_LCD_SetActiveLayer

   - At application level, once the LCD is initialized, user should call UTIL_LCD_SetFuncDriver()
     API to link board LCD drivers to BASIC GUI LCD drivers.
     User can then call the BASIC GUI services:
         UTIL_LCD_SetFuncDriver()
         UTIL_LCD_SetLayer()
         UTIL_LCD_SetDevice()
         UTIL_LCD_SetTextColor()
         UTIL_LCD_GetTextColor()
         UTIL_LCD_SetBackColor()
         UTIL_LCD_GetBackColor()
         UTIL_LCD_SetFont()
         UTIL_LCD_GetFont()
         UTIL_LCD_Clear)
         UTIL_LCD_ClearStringLine()
         UTIL_LCD_DisplayStringAtLine()
         UTIL_LCD_DisplayStringAt()
         UTIL_LCD_DisplayChar()
         UTIL_LCD_GetPixel()
         UTIL_LCD_SetPixel()
         UTIL_LCD_FillRGBRect()
         UTIL_LCD_DrawHLine()
         UTIL_LCD_DrawVLine()
         UTIL_LCD_DrawBitmap()
         UTIL_LCD_FillRect()
         UTIL_LCD_DrawLine()
         UTIL_LCD_DrawRect()
         UTIL_LCD_DrawCircle()
         UTIL_LCD_DrawPolygon()
         UTIL_LCD_DrawEllipse()
         UTIL_LCD_FillCircle()
         UTIL_LCD_FillPolygon()
         UTIL_LCD_FillEllipse()
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32_lcd.h"
#include "../Fonts/font24.c"
#include "../Fonts/font20.c"
#include "../Fonts/font16.c"
#include "../Fonts/font12.c"
#include "../Fonts/font8.c"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @defgroup STM32_LCD STM32 LCD Utility
  * @{
  */

#ifndef UTIL_LCD_MAX_LAYERS_NBR
  #define UTIL_LCD_MAX_LAYERS_NBR    2U
#endif

/** @defgroup UTIL_LCD_Private_Macros STM32 LCD Utility Private Macros
  * @{
  */
#define ABS(X)                 ((X) > 0 ? (X) : -(X))
#define POLY_X(Z)              ((int32_t)((Points + (Z))->X))
#define POLY_Y(Z)              ((int32_t)((Points + (Z))->Y))

#define CONVERTARGB88882RGB565(Color)((((Color & 0xFFU) >> 3) & 0x1FU) |\
                                     (((((Color & 0xFF00U) >> 8) >>2) & 0x3FU) << 5) |\
                                     (((((Color & 0xFF0000U) >> 16) >>3) & 0x1FU) << 11))

#define CONVERTRGB5652ARGB8888(Color)(((((((Color >> 11) & 0x1FU) * 527) + 23) >> 6) << 16) |\
                                     ((((((Color >> 5) & 0x3FU) * 259) + 33) >> 6) << 8) |\
                                     ((((Color & 0x1FU) * 527) + 23) >> 6) | 0xFF000000)

/**
  * @}
  */

/** @defgroup UTIL_LCD_Private_Types STM32 LCD Utility Private Types
  * @{
  */
typedef struct
{
  uint32_t x1;
  uint32_t y1;
  uint32_t x2;
  uint32_t y2;
  uint32_t x3;
  uint32_t y3;
}Triangle_Positions_t;

/**
  * @}
  */

/** @defgroup UTIL_LCD_Private_Variables STM32 LCD Utility Private Variables
  * @{
  */

/**
  * @brief  Current Drawing Layer properties variable
  */
static UTIL_LCD_Ctx_t DrawProp[UTIL_LCD_MAX_LAYERS_NBR];
static LCD_UTILS_Drv_t FuncDriver;

/**
  * @}
  */

/** @defgroup UTIL_LCD_Private_FunctionPrototypes STM32 LCD Utility Private FunctionPrototypes
  * @{
  */
static void DrawChar(uint32_t Xpos, uint32_t Ypos, const uint8_t *pData);
static void FillTriangle(Triangle_Positions_t *Positions, uint32_t Color);
/**
  * @}
  */

/** @defgroup UTIL_LCD_Exported_Functions STM32 LCD Utility Exported Functions
  * @{
  */

/**
  * @brief  Link board LCD drivers to STM32 LCD Utility drivers
  * @param  pDrv Structure of LCD functions
  */
void UTIL_LCD_SetFuncDriver(const LCD_UTILS_Drv_t *pDrv)
{
  FuncDriver.DrawBitmap     = pDrv->DrawBitmap;
  FuncDriver.FillRGBRect    = pDrv->FillRGBRect;
  FuncDriver.DrawHLine      = pDrv->DrawHLine;
  FuncDriver.DrawVLine      = pDrv->DrawVLine;
  FuncDriver.FillRect       = pDrv->FillRect;
  FuncDriver.GetPixel       = pDrv->GetPixel;
  FuncDriver.SetPixel       = pDrv->SetPixel;
  FuncDriver.GetXSize       = pDrv->GetXSize;
  FuncDriver.GetYSize       = pDrv->GetYSize;
  FuncDriver.SetLayer       = pDrv->SetLayer;
  FuncDriver.GetFormat      = pDrv->GetFormat;

  DrawProp->LcdLayer = 0;
  DrawProp->LcdDevice = 0;
  FuncDriver.GetXSize(0, &DrawProp->LcdXsize);
  FuncDriver.GetYSize(0, &DrawProp->LcdYsize);
  FuncDriver.GetFormat(0, &DrawProp->LcdPixelFormat);
}

/**
  * @brief  Set the LCD layer.
  * @param  Layer  LCD layer
  */
void UTIL_LCD_SetLayer(uint32_t Layer)
{
  if(FuncDriver.SetLayer != NULL)
  {
    if(FuncDriver.SetLayer(DrawProp->LcdDevice, Layer) == 0)
    {
      DrawProp->LcdLayer = Layer;
    }
  }
}

/**
  * @brief  Set the LCD instance to be used.
  * @param  Device  LCD instance
  */
void UTIL_LCD_SetDevice(uint32_t Device)
{
  DrawProp->LcdDevice = Device;
  FuncDriver.GetXSize(Device, &DrawProp->LcdXsize);
  FuncDriver.GetYSize(Device, &DrawProp->LcdYsize);
}

/**
  * @brief  Sets the LCD text color.
  * @param  Color  Text color code
  */
void UTIL_LCD_SetTextColor(uint32_t Color)
{
  DrawProp[DrawProp->LcdLayer].TextColor = Color;
}

/**
  * @brief  Gets the LCD text color.
  * @retval Used text color.
  */
uint32_t UTIL_LCD_GetTextColor(void)
{
  return DrawProp[DrawProp->LcdLayer].TextColor;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Color  Layer background color code
  */
void UTIL_LCD_SetBackColor(uint32_t Color)
{
  DrawProp[DrawProp->LcdLayer].BackColor = Color;
}

/**
  * @brief  Gets the LCD background color.
  * @retval Used background color
  */
uint32_t UTIL_LCD_GetBackColor(void)
{
  return DrawProp[DrawProp->LcdLayer].BackColor;
}

/**
  * @brief  Sets the LCD text font.
  * @param  fonts  Layer font to be used
  */
void UTIL_LCD_SetFont(sFONT *fonts)
{
  DrawProp[DrawProp->LcdLayer].pFont = fonts;
}

/**
  * @brief  Gets the LCD text font.
  * @retval Used layer font
  */
sFONT *UTIL_LCD_GetFont(void)
{
  return DrawProp[DrawProp->LcdLayer].pFont;
}

/**
  * @brief  Draws a RGB rectangle in currently active layer.
  * @param  pData   Pointer to RGB rectangle data
  * @param  Xpos    X position
  * @param  Ypos    Y position
  * @param  Length  Line length
  */
void UTIL_LCD_FillRGBRect(uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
  /* Write RGB rectangle data */
  FuncDriver.FillRGBRect(DrawProp->LcdDevice, Xpos, Ypos, pData, Width, Height);
}

/**
  * @brief  Draws an horizontal line in currently active layer.
  * @param  Xpos    X position
  * @param  Ypos    Y position
  * @param  Length  Line length
  * @param  Color   Draw color
  */
void UTIL_LCD_DrawHLine(uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  /* Write line */
  if(DrawProp->LcdPixelFormat == LCD_PIXEL_FORMAT_RGB565)
  {
    FuncDriver.DrawHLine(DrawProp->LcdDevice, Xpos, Ypos, Length, CONVERTARGB88882RGB565(Color));
  }
  else
  {
    FuncDriver.DrawHLine(DrawProp->LcdDevice, Xpos, Ypos, Length, Color);
  }
}

/**
  * @brief  Draws a vertical line in currently active layer.
  * @param  Xpos    X position
  * @param  Ypos    Y position
  * @param  Length  Line length
  * @param  Color   Draw color
  */
void UTIL_LCD_DrawVLine(uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  /* Write line */
  if(DrawProp->LcdPixelFormat == LCD_PIXEL_FORMAT_RGB565)
  {
    FuncDriver.DrawVLine(DrawProp->LcdDevice, Xpos, Ypos, Length, CONVERTARGB88882RGB565(Color));
  }
  else
  {
    FuncDriver.DrawVLine(DrawProp->LcdDevice, Xpos, Ypos, Length, Color);
  }
}

/**
  * @brief  Read an LCD pixel.
  * @param  Xpos     X position
  * @param  Ypos     Y position
  * @retval Color    pixel color
  */
void UTIL_LCD_GetPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color)
{
  /* Get Pixel */
  FuncDriver.GetPixel(DrawProp->LcdDevice, Xpos, Ypos, Color);
  if(DrawProp->LcdPixelFormat == LCD_PIXEL_FORMAT_RGB565)
  {
    *Color = CONVERTRGB5652ARGB8888(*Color);
  }
}

/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos     X position
  * @param  Ypos     Y position
  * @param  Color    Pixel color
  */
void UTIL_LCD_SetPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color)
{
  /* Set Pixel */
  if(DrawProp->LcdPixelFormat == LCD_PIXEL_FORMAT_RGB565)
  {
    FuncDriver.SetPixel(DrawProp->LcdDevice, Xpos, Ypos, CONVERTARGB88882RGB565(Color));
  }
  else
  {
    FuncDriver.SetPixel(DrawProp->LcdDevice, Xpos, Ypos, Color);
  }
}

/**
  * @brief  Clears the whole currently active layer of LTDC.
  * @param  Color  Color of the background
  */
void UTIL_LCD_Clear(uint32_t Color)
{
  /* Clear the LCD */
  UTIL_LCD_FillRect(0, 0, DrawProp->LcdXsize, DrawProp->LcdYsize, Color);
}

/**
  * @brief  Clears the selected line in currently active layer.
  * @param  Line  Line to be cleared
  */
void UTIL_LCD_ClearStringLine(uint32_t Line)
{
  /* Draw rectangle with background color */
  UTIL_LCD_FillRect(0, (Line * DrawProp[DrawProp->LcdLayer].pFont->Height), DrawProp->LcdXsize, DrawProp[DrawProp->LcdLayer].pFont->Height, DrawProp[DrawProp->LcdLayer].BackColor);
}

/**
  * @brief  Displays one character in currently active layer.
  * @param  Xpos Start column address
  * @param  Ypos Line where to display the character shape.
  * @param  Ascii Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
  */
void UTIL_LCD_DisplayChar(uint32_t Xpos, uint32_t Ypos, uint8_t Ascii)
{
  DrawChar(Xpos, Ypos, &DrawProp[DrawProp->LcdLayer].pFont->table[(Ascii-' ') *\
  DrawProp[DrawProp->LcdLayer].pFont->Height * ((DrawProp[DrawProp->LcdLayer].pFont->Width + 7) / 8)]);
}

/**
  * @brief  Displays characters in currently active layer.
  * @param  Xpos X position (in pixel)
  * @param  Ypos Y position (in pixel)
  * @param  Text Pointer to string to display on LCD
  * @param  Mode Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE
  */
void UTIL_LCD_DisplayStringAt(uint32_t Xpos, uint32_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
  uint32_t refcolumn = 1, i = 0;
  uint32_t size = 0, xsize = 0;
  uint8_t  *ptr = Text;

  /* Get the text size */
  while (*ptr++) size ++ ;

  /* Characters number per line */
  xsize = (DrawProp->LcdXsize/DrawProp[DrawProp->LcdLayer].pFont->Width);

  switch (Mode)
  {
  case CENTER_MODE:
    {
      refcolumn = Xpos + ((xsize - size)* DrawProp[DrawProp->LcdLayer].pFont->Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      refcolumn = Xpos;
      break;
    }
  case RIGHT_MODE:
    {
      refcolumn = - Xpos + ((xsize - size)*DrawProp[DrawProp->LcdLayer].pFont->Width);
      break;
    }
  default:
    {
      refcolumn = Xpos;
      break;
    }
  }

  /* Check that the Start column is located in the screen */
  if ((refcolumn < 1) || (refcolumn >= 0x8000))
  {
    refcolumn = 1;
  }

  /* Send the string character by character on LCD */
  while ((*Text != 0) & (((DrawProp->LcdXsize - (i*DrawProp[DrawProp->LcdLayer].pFont->Width)) & 0xFFFF) >= DrawProp[DrawProp->LcdLayer].pFont->Width))
  {
    /* Display one character on LCD */
    UTIL_LCD_DisplayChar(refcolumn, Ypos, *Text);
    /* Decrement the column position by 16 */
    refcolumn += DrawProp[DrawProp->LcdLayer].pFont->Width;

    /* Point on the next character */
    Text++;
    i++;
  }
}

/**
  * @brief  Displays a maximum of 60 characters on the LCD.
  * @param  Line: Line where to display the character shape
  * @param  ptr: Pointer to string to display on LCD
  */
void UTIL_LCD_DisplayStringAtLine(uint32_t Line, uint8_t *ptr)
{
  UTIL_LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

/**
  * @brief  Draws an uni-line (between two points) in currently active layer.
  * @param  Xpos1 Point 1 X position
  * @param  Ypos1 Point 1 Y position
  * @param  Xpos2 Point 2 X position
  * @param  Ypos2 Point 2 Y position
  * @param  Color Draw color
  */
void UTIL_LCD_DrawLine(uint32_t Xpos1, uint32_t Ypos1, uint32_t Xpos2, uint32_t Ypos2, uint32_t Color)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;
  int32_t x_diff, y_diff;

  x_diff = Xpos2 - Xpos1;
  y_diff = Ypos2 - Ypos1;

  deltax = ABS(x_diff);         /* The absolute difference between the x's */
  deltay = ABS(y_diff);         /* The absolute difference between the y's */
  x = Xpos1;                       /* Start x off at the first pixel */
  y = Ypos1;                       /* Start y off at the first pixel */

  if (Xpos2 >= Xpos1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (Ypos2 >= Ypos1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    UTIL_LCD_SetPixel(x, y, Color);   /* Draw the current pixel */
    num += numadd;                            /* Increase the numerator by the top of the fraction */
    if (num >= den)                           /* Check if numerator >= denominator */
    {
      num -= den;                             /* Calculate the new numerator value */
      x += xinc1;                             /* Change the x as appropriate */
      y += yinc1;                             /* Change the y as appropriate */
    }
    x += xinc2;                               /* Change the x as appropriate */
    y += yinc2;                               /* Change the y as appropriate */
  }
}

/**
  * @brief  Draws a rectangle in currently active layer.
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Width  Rectangle width
  * @param  Height Rectangle height
  * @param  Color  Draw color
  */
void UTIL_LCD_DrawRect(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color)
{
  /* Draw horizontal lines */
  UTIL_LCD_DrawHLine(Xpos, Ypos, Width, Color);
  UTIL_LCD_DrawHLine(Xpos, (Ypos+ Height - 1U), Width, Color);

  /* Draw vertical lines */
  UTIL_LCD_DrawVLine(Xpos, Ypos, Height, Color);
  UTIL_LCD_DrawVLine((Xpos + Width - 1U), Ypos, Height, Color);
}

/**
  * @brief  Draws a circle in currently active layer.
  * @param  Xpos    X position
  * @param  Ypos    Y position
  * @param  Radius  Circle radius
  * @param  Color   Draw color
  */
void UTIL_LCD_DrawCircle(uint32_t Xpos, uint32_t Ypos, uint32_t Radius, uint32_t Color)
{
  int32_t   decision;  /* Decision Variable */
  uint32_t  current_x; /* Current X Value */
  uint32_t  current_y; /* Current Y Value */

  decision = 3 - (Radius << 1);
  current_x = 0;
  current_y = Radius;

  while (current_x <= current_y)
  {
    if((Ypos - current_y) < DrawProp->LcdYsize)
    {
      if((Xpos + current_x) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos + current_x), (Ypos - current_y), Color);
      }
      if((Xpos - current_x) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos - current_x), (Ypos - current_y), Color);
      }
    }

    if((Ypos - current_x) < DrawProp->LcdYsize)
    {
      if((Xpos + current_y) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos + current_y), (Ypos - current_x), Color);
      }
      if((Xpos - current_y) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos - current_y), (Ypos - current_x), Color);
      }
    }

    if((Ypos + current_y) < DrawProp->LcdYsize)
    {
      if((Xpos + current_x) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos + current_x), (Ypos + current_y), Color);
      }
      if((Xpos - current_x) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos - current_x), (Ypos + current_y), Color);
      }
    }

    if((Ypos + current_x) < DrawProp->LcdYsize)
    {
      if((Xpos + current_y) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos + current_y), (Ypos + current_x), Color);
      }
      if((Xpos - current_y) < DrawProp->LcdXsize)
      {
        UTIL_LCD_SetPixel((Xpos - current_y), (Ypos + current_x), Color);
      }
    }

    if (decision < 0)
    {
      decision += (current_x << 2) + 6;
    }
    else
    {
      decision += ((current_x - current_y) << 2) + 10;
      current_y--;
    }
    current_x++;
  }
}

/**
  * @brief  Draws an poly-line (between many points) in currently active layer.
  * @param  Points      Pointer to the points array
  * @param  PointCount  Number of points
  * @param  Color       Draw color
  */
void UTIL_LCD_DrawPolygon(pPoint Points, uint32_t PointCount, uint32_t Color)
{
  int16_t x_pos = 0, y_pos = 0;

  if(PointCount < 2)
  {
    return;
  }

  UTIL_LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y, Color);

  while(--PointCount)
  {
    x_pos = Points->X;
    y_pos = Points->Y;
    Points++;
    UTIL_LCD_DrawLine(x_pos, y_pos, Points->X, Points->Y, Color);
  }
}

/**
  * @brief  Draws an ellipse on LCD in currently active layer.
  * @param  Xpos    X position
  * @param  Ypos    Y position
  * @param  XRadius Ellipse X radius
  * @param  YRadius Ellipse Y radius
  * @param  Color   Draw color
  */
void UTIL_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint32_t Color)
{
  int x_pos = 0, y_pos = -YRadius, err = 2-2*XRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  k = (float)(rad2/rad1);

  do
  {
    UTIL_LCD_SetPixel((Xpos-(uint32_t)(x_pos/k)), (Ypos + y_pos), Color);
    UTIL_LCD_SetPixel((Xpos+(uint32_t)(x_pos/k)), (Ypos + y_pos), Color);
    UTIL_LCD_SetPixel((Xpos+(uint32_t)(x_pos/k)), (Ypos - y_pos), Color);
    UTIL_LCD_SetPixel((Xpos-(uint32_t)(x_pos/k)), (Ypos - y_pos), Color);

    e2 = err;
    if (e2 <= x_pos)
    {
      err += ++x_pos*2+1;
      if (-y_pos == x_pos && e2 <= y_pos) e2 = 0;
    }
    if (e2 > y_pos)
    {
      err += ++y_pos*2+1;
    }
  }while (y_pos <= 0);
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash (32 bpp) in currently active layer.
  * @param  Xpos  Bmp X position in the LCD
  * @param  Ypos  Bmp Y position in the LCD
  * @param  pData Pointer to Bmp picture address in the internal Flash
  */
void UTIL_LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pData)
{
  FuncDriver.DrawBitmap(DrawProp->LcdDevice, Xpos, Ypos, pData);
}

/**
  * @brief  Draws a full rectangle in currently active layer.
  * @param  Xpos   X position
  * @param  Ypos   Y position
  * @param  Width  Rectangle width
  * @param  Height Rectangle height
  * @param  Color  Draw color
  */
void UTIL_LCD_FillRect(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color)
{
  /* Fill the rectangle */
  if(DrawProp->LcdPixelFormat == LCD_PIXEL_FORMAT_RGB565)
  {
    FuncDriver.FillRect(DrawProp->LcdDevice, Xpos, Ypos, Width, Height, CONVERTARGB88882RGB565(Color));
  }
  else
  {
    FuncDriver.FillRect(DrawProp->LcdDevice, Xpos, Ypos, Width, Height, Color);
  }
}

/**
  * @brief  Draws a full circle in currently active layer.
  * @param  Xpos   X position
  * @param  Ypos   Y position
  * @param  Radius Circle radius
  * @param  Color  Draw color
  */
void UTIL_LCD_FillCircle(uint32_t Xpos, uint32_t Ypos, uint32_t Radius, uint32_t Color)
{
  int32_t   decision;  /* Decision Variable */
  uint32_t  current_x; /* Current X Value */
  uint32_t  current_y; /* Current Y Value */

  decision = 3 - (Radius << 1);

  current_x = 0;
  current_y = Radius;

  while (current_x <= current_y)
  {
    if(current_y > 0)
    {
      if(current_y >= Xpos)
      {
        UTIL_LCD_DrawHLine(0, Ypos + current_x, 2*current_y - (current_y - Xpos), Color);
        UTIL_LCD_DrawHLine(0, Ypos - current_x, 2*current_y - (current_y - Xpos), Color);
      }
      else
      {
        UTIL_LCD_DrawHLine(Xpos - current_y, Ypos + current_x, 2*current_y, Color);
        UTIL_LCD_DrawHLine(Xpos - current_y, Ypos - current_x, 2*current_y, Color);
      }
    }

    if(current_x > 0)
    {
      if(current_x >= Xpos)
      {
        UTIL_LCD_DrawHLine(0, Ypos - current_y, 2*current_x - (current_x - Xpos), Color);
        UTIL_LCD_DrawHLine(0, Ypos + current_y, 2*current_x - (current_x - Xpos), Color);
      }
      else
      {
        UTIL_LCD_DrawHLine(Xpos - current_x, Ypos - current_y, 2*current_x, Color);
        UTIL_LCD_DrawHLine(Xpos - current_x, Ypos + current_y, 2*current_x, Color);
      }
    }
    if (decision < 0)
    {
      decision += (current_x << 2) + 6;
    }
    else
    {
      decision += ((current_x - current_y) << 2) + 10;
      current_y--;
    }
    current_x++;
  }

  UTIL_LCD_DrawCircle(Xpos, Ypos, Radius, Color);
}

/**
  * @brief  Draws a full poly-line (between many points) in currently active layer.
  * @param  Points     Pointer to the points array
  * @param  PointCount Number of points
  * @param  Color      Draw color
  */
void UTIL_LCD_FillPolygon(pPoint Points, uint32_t PointCount, uint32_t Color)
{
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, x_center = 0, y_center = 0, x_first = 0, y_first = 0, pixel_x = 0, pixel_y = 0, counter = 0;
  uint32_t  image_left = 0, image_right = 0, image_top = 0, image_bottom = 0;
  Triangle_Positions_t positions;

  image_left = image_right = Points->X;
  image_top= image_bottom = Points->Y;

  for(counter = 1; counter < PointCount; counter++)
  {
    pixel_x = POLY_X(counter);
    if(pixel_x < image_left)
    {
      image_left = pixel_x;
    }
    if(pixel_x > image_right)
    {
      image_right = pixel_x;
    }

    pixel_y = POLY_Y(counter);
    if(pixel_y < image_top)
    {
      image_top = pixel_y;
    }
    if(pixel_y > image_bottom)
    {
      image_bottom = pixel_y;
    }
  }

  if(PointCount < 2)
  {
    return;
  }

  x_center = (image_left + image_right)/2;
  y_center = (image_bottom + image_top)/2;

  x_first = Points->X;
  y_first = Points->Y;

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;
    positions.x1 = X;
    positions.y1 = Y;
    positions.x2 = X2;
    positions.y2 = Y2;
    positions.x3 = x_center;
    positions.y3 = y_center;
    FillTriangle(&positions, Color);

    positions.x2 = x_center;
    positions.y2 = y_center;
    positions.x3 = X2;
    positions.y3 = Y2;
    FillTriangle(&positions, Color);

    positions.x1 = x_center;
    positions.y1 = y_center;
    positions.x2 = X2;
    positions.y2 = Y2;
    positions.x3 = X;
    positions.y3 = Y;
    FillTriangle(&positions, Color);
  }

    positions.x1 = x_first;
    positions.y1 = y_first;
    positions.x2 = X2;
    positions.y2 = Y2;
    positions.x3 = x_center;
    positions.y3 = y_center;
    FillTriangle(&positions, Color);

    positions.x2 = x_center;
    positions.y2 = y_center;
    positions.x3 = X2;
    positions.y3 = Y2;
    FillTriangle(&positions, Color);

    positions.x1 = x_center;
    positions.y1 = y_center;
    positions.x2 = X2;
    positions.y2 = Y2;
    positions.x3 = x_first;
    positions.y3 = y_first;
    FillTriangle(&positions, Color);
}

/**
  * @brief  Draws a full ellipse in currently active layer.
  * @param  Xpos    X position
  * @param  Ypos    Y position
  * @param  XRadius Ellipse X radius
  * @param  YRadius Ellipse Y radius
  * @param  Color   Draw color
  */
void UTIL_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint32_t Color)
{
  int x_pos = 0, y_pos = -YRadius, err = 2-2*XRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  k = (float)(rad2/rad1);

  do
  {
    UTIL_LCD_DrawHLine((Xpos-(uint32_t)(x_pos/k)), (Ypos + y_pos), (2*(uint32_t)(x_pos/k) + 1), Color);
    UTIL_LCD_DrawHLine((Xpos-(uint32_t)(x_pos/k)), (Ypos - y_pos), (2*(uint32_t)(x_pos/k) + 1), Color);

    e2 = err;
    if (e2 <= x_pos)
    {
      err += ++x_pos*2+1;
      if (-y_pos == x_pos && e2 <= y_pos) e2 = 0;
    }
    if (e2 > y_pos) err += ++y_pos*2+1;
  }
  while (y_pos <= 0);
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos  Line where to display the character shape
  * @param  Ypos  Start column address
  * @param  pData Pointer to the character data
  */
static void DrawChar(uint32_t Xpos, uint32_t Ypos, const uint8_t *pData)
{
  uint32_t i = 0, j = 0, offset;
  uint32_t height, width;
  uint8_t  *pchar;
  uint32_t line;

  height = DrawProp[DrawProp->LcdLayer].pFont->Height;
  width  = DrawProp[DrawProp->LcdLayer].pFont->Width;
  uint16_t rgb565[24];
  uint32_t argb8888[24];

  offset =  8 *((width + 7)/8) -  width ;

  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)pData + (width + 7)/8 * i);

    switch(((width + 7)/8))
    {

    case 1:
      line =  pchar[0];
      break;

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;

    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;
    }

    if(DrawProp[DrawProp->LcdLayer].LcdPixelFormat == LCD_PIXEL_FORMAT_RGB565)
    {
      for (j = 0; j < width; j++)
      {
        if(line & (1 << (width- j + offset- 1)))
        {
          rgb565[j] = CONVERTARGB88882RGB565(DrawProp[DrawProp->LcdLayer].TextColor);
        }
        else
        {
          rgb565[j] = CONVERTARGB88882RGB565(DrawProp[DrawProp->LcdLayer].BackColor);
        }
      }
      UTIL_LCD_FillRGBRect(Xpos,  Ypos++, (uint8_t*)&rgb565[0], width, 1);
    }
    else
    {
      for (j = 0; j < width; j++)
      {
        if(line & (1 << (width- j + offset- 1)))
        {
          argb8888[j] = DrawProp[DrawProp->LcdLayer].TextColor;
        }
        else
        {
          argb8888[j] = DrawProp[DrawProp->LcdLayer].BackColor;
        }
      }
      UTIL_LCD_FillRGBRect(Xpos,  Ypos++, (uint8_t*)&argb8888[0], width, 1);
    }
  }
}

/**
  * @brief  Fills a triangle (between 3 points).
  * @param  Positions  pointer to riangle coordinates
  * @param  Color      Draw color
  */
static void FillTriangle(Triangle_Positions_t *Positions, uint32_t Color)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;
  int32_t x_diff, y_diff;

  x_diff = Positions->x2 - Positions->x1;
  y_diff = Positions->y2 - Positions->y1;

  deltax = ABS(x_diff);         /* The absolute difference between the x's */
  deltay = ABS(y_diff);         /* The absolute difference between the y's */
  x = Positions->x1;                       /* Start x off at the first pixel */
  y = Positions->y1;                       /* Start y off at the first pixel */

  if (Positions->x2 >= Positions->x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (Positions->y2 >= Positions->y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    UTIL_LCD_DrawLine(x, y, Positions->x3, Positions->y3, Color);

    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
