/**
  ******************************************************************************
  * @file    lcd_trace.c
  * @author  MCD Application Team
  * @brief   This file provides all the LCD Trace firmware functions.
  *
  *          The LCD Trace module allows to automatically set a header and footer
  *          on any application using the LCD display and allows to dump user,
  *          debug and error messages by using the following macros: LCD_ErrTrace(),
  *          LCD_UsrTrace() and LCD_DbgTrace().
  *
  *          It supports also the scroll feature by embedding an internal software
  *          cache for display. This feature allows to dump message sequentially
  *          on the display even if the number of displayed lines is bigger than
  *          the total number of line allowed by the display.
  *
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
#include  "lcd_trace.h"
#include  <stdio.h>

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @defgroup LCD_TRACE LCD_TRACE
  * @brief LCD Trace LCD_Application module
  * @{
  */

/** @defgroup UTIL_LCD_TRACE_Private_Defines Private Defines
  * @{
  */
/* Define the display window settings */
#define     YWINDOW_MIN         4U
/**
  * @}
  */

/** @defgroup UTIL_LCD_TRACE_Private_Macros Private Macros
  * @{
  */
#if (USE_LCD_TRACE > 0)
/* Redirect the printf to the LCD */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define UTIL_LCD_TRACE_PUTCHAR int __io_putchar(int ch)
#else
#define UTIL_LCD_TRACE_PUTCHAR int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif
/**
  * @}
  */

/** @defgroup UTIL_LCD_TRACE_Private_Variables Private Variables
  * @{
  */
UTIL_LCD_TRACE_Line_t LCD_CacheBuffer [LCD_CACHE_DEPTH];
/**
  * @}
  */

/** @defgroup UTIL_LCD_TRACE_Exported_Variables Exported Variables
  * @{
  */
UTIL_LCD_TRACE_Ctx_t LcdTrace_Ctx;
/**
  * @}
  */


/** @defgroup UTIL_LCD_TRACE_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  Initializes the LCD Trace module
  * @retval None
  */
UTIL_LCD_TRACE_Status_t UTIL_LCD_TRACE_Init (void)
{
  uint32_t y_size;

  /* Deinit LCD cache */
  UTIL_LCD_TRACE_DeInit();

  /* Set background color */
  UTIL_LCD_SetBackColor(LCD_TRACE_BACKGROUND_COLOR);

  /* Clear the LCD */
  UTIL_LCD_Clear(LCD_TRACE_BACKGROUND_COLOR);

  BSP_LCD_GetYSize(0, &y_size);
  LcdTrace_Ctx.YWindowSize = ((y_size - (LCD_TRACE_HEADER_FONT.Height * 3U) - \
    LCD_TRACE_FOOTER_FONT.Height - 4U)/LCD_TRACE_TEXT_FONT.Height);

  if (YWINDOW_SIZE > LcdTrace_Ctx.YWindowSize)
  {
    return UTIL_LCD_TRACE_ERROR;
  }

  return UTIL_LCD_TRACE_OK;
}

/**
  * @brief DeInitializes the LCD Trace module.
  * @retval None
  */
void UTIL_LCD_TRACE_DeInit(void)
{
  LcdTrace_Ctx.LineColor                 = LCD_TRACE_TEXT_COLOR;
  LcdTrace_Ctx.CacheBufferXptr           = 0U;
  LcdTrace_Ctx.CacheBufferYptrTop        = 0U;
  LcdTrace_Ctx.CacheBufferYptrBottom     = 0U;
  LcdTrace_Ctx.CacheBufferYptrTopBack    = 0U;
  LcdTrace_Ctx.CacheBufferYptrBottomBack = 0U;
  LcdTrace_Ctx.CacheBufferYptrInvert     = ENABLE;
  LcdTrace_Ctx.ScrollActive              = DISABLE;
  LcdTrace_Ctx.Lock                      = DISABLE;
  LcdTrace_Ctx.Scrolled                  = DISABLE;
  LcdTrace_Ctx.ScrollBackStep            = 0U;
  LcdTrace_Ctx.YWindowSize               = 0U;
}

/**
  * @brief  Display the application header on the LCD screen
  * @param  Header  pointer to the string to be displayed
  * @retval None
  */
void UTIL_LCD_TRACE_SetHeader (uint8_t *Header)
{
  uint32_t x_size;

  BSP_LCD_GetXSize(0, &x_size);

  /* Set the LCD Font */
  UTIL_LCD_SetFont (&LCD_TRACE_HEADER_FONT);
  UTIL_LCD_FillRect(0, 0, x_size, LCD_TRACE_HEADER_FONT.Height * 3, LCD_TRACE_SOLID_BACKGROUND_COLOR);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(LCD_TRACE_SOLID_TEXT_COLOR);
  UTIL_LCD_SetBackColor(LCD_TRACE_SOLID_BACKGROUND_COLOR);

  UTIL_LCD_DisplayStringAt(0, LCD_TRACE_HEADER_FONT.Height, Header, CENTER_MODE);

  UTIL_LCD_SetBackColor(LCD_TRACE_BACKGROUND_COLOR);
  UTIL_LCD_SetTextColor(LCD_TRACE_TEXT_COLOR);
  UTIL_LCD_SetFont (&LCD_TRACE_TEXT_FONT);
}

/**
  * @brief  Display the application footer on the LCD screen
  * @param  Footer  pointer to the string to be displayed
  * @retval None
  */
void UTIL_LCD_TRACE_SetFooter(uint8_t *Footer)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Set the LCD Font */
  UTIL_LCD_SetFont (&LCD_TRACE_FOOTER_FONT);

  UTIL_LCD_FillRect(0, y_size - LCD_TRACE_FOOTER_FONT.Height - 4, x_size, LCD_TRACE_FOOTER_FONT.Height + 4, LCD_TRACE_SOLID_BACKGROUND_COLOR);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(LCD_TRACE_SOLID_TEXT_COLOR);
  UTIL_LCD_SetBackColor(LCD_TRACE_SOLID_BACKGROUND_COLOR);

  UTIL_LCD_DisplayStringAt(0, y_size - LCD_TRACE_FOOTER_FONT.Height, Footer, CENTER_MODE);

  UTIL_LCD_SetBackColor(LCD_TRACE_BACKGROUND_COLOR);
  UTIL_LCD_SetTextColor(LCD_TRACE_TEXT_COLOR);
  UTIL_LCD_SetFont (&LCD_TRACE_TEXT_FONT);
}

/**
  * @brief  Clear the Text Zone
  * @retval None
  */
void UTIL_LCD_TRACE_ClearTextZone(void)
{
  uint32_t i;

  for (i= 0 ; i < YWINDOW_SIZE; i++)
  {
    UTIL_LCD_ClearStringLine(i + YWINDOW_MIN);
  }

  UTIL_LCD_TRACE_DeInit();
}

#if (USE_LCD_TRACE > 0)
/**
  * @brief  Redirect the printf to the LCD
  * @retval None
 */
UTIL_LCD_TRACE_PUTCHAR
{
  uint32_t x_size;

  BSP_LCD_GetXSize(0, &x_size);

  sFONT *cFont = UTIL_LCD_GetFont();
  uint32_t idx;

  if(LcdTrace_Ctx.Lock == DISABLE)
  {
    if(LcdTrace_Ctx.ScrollActive == ENABLE)
    {
      LcdTrace_Ctx.CacheBufferYptrBottom = LcdTrace_Ctx.CacheBufferYptrBottomBack;
      LcdTrace_Ctx.CacheBufferYptrTop    = LcdTrace_Ctx.CacheBufferYptrTopBack;
      LcdTrace_Ctx.ScrollActive = DISABLE;
      LcdTrace_Ctx.Scrolled = DISABLE;
      LcdTrace_Ctx.ScrollBackStep = 0;
    }

    if(( LcdTrace_Ctx.CacheBufferXptr < (x_size) /cFont->Width ) &&  ( ch != '\n'))
    {
      LCD_CacheBuffer[LcdTrace_Ctx.CacheBufferYptrBottom].Line[LcdTrace_Ctx.CacheBufferXptr++] = (uint16_t)ch;
    }
    else
    {
      if(LcdTrace_Ctx.CacheBufferYptrTop >= LcdTrace_Ctx.CacheBufferYptrBottom)
      {

        if(LcdTrace_Ctx.CacheBufferYptrInvert == DISABLE)
        {
          LcdTrace_Ctx.CacheBufferYptrTop++;

          if(LcdTrace_Ctx.CacheBufferYptrTop == LCD_CACHE_DEPTH)
          {
            LcdTrace_Ctx.CacheBufferYptrTop = 0;
          }
        }
        else
        {
          LcdTrace_Ctx.CacheBufferYptrInvert = DISABLE;
        }
      }

      for(idx = LcdTrace_Ctx.CacheBufferXptr ; idx < (x_size) /cFont->Width; idx++)
      {
        LCD_CacheBuffer[LcdTrace_Ctx.CacheBufferYptrBottom].Line[LcdTrace_Ctx.CacheBufferXptr++] = ' ';
      }
      LCD_CacheBuffer[LcdTrace_Ctx.CacheBufferYptrBottom].Color = LcdTrace_Ctx.LineColor;

      LcdTrace_Ctx.CacheBufferXptr = 0;

      UTIL_LCD_TRACE_UpdateDisplay ();

      LcdTrace_Ctx.CacheBufferYptrBottom ++;

      if (LcdTrace_Ctx.CacheBufferYptrBottom == LCD_CACHE_DEPTH)
      {
        LcdTrace_Ctx.CacheBufferYptrBottom = 0;
        LcdTrace_Ctx.CacheBufferYptrTop = 1;
        LcdTrace_Ctx.CacheBufferYptrInvert = ENABLE;
      }

      if( ch != '\n')
      {
        LCD_CacheBuffer[LcdTrace_Ctx.CacheBufferYptrBottom].Line[LcdTrace_Ctx.CacheBufferXptr++] = (uint16_t)ch;
      }

    }
  }
  return ch;
}
#endif

/**
  * @brief  Update the text area display
  * @retval None
  */
void UTIL_LCD_TRACE_UpdateDisplay (void)
{
  uint32_t cnt = 0U, length, ptr, index;

  if((LcdTrace_Ctx.CacheBufferYptrBottom  < (YWINDOW_SIZE -1)) &&
     (LcdTrace_Ctx.CacheBufferYptrBottom  >= LcdTrace_Ctx.CacheBufferYptrTop))
  {
    UTIL_LCD_SetTextColor(LCD_CacheBuffer[cnt + LcdTrace_Ctx.CacheBufferYptrBottom].Color);
    UTIL_LCD_DisplayStringAtLine ((YWINDOW_MIN + LcdTrace_Ctx.CacheBufferYptrBottom),
                           (uint8_t *)(LCD_CacheBuffer[cnt + LcdTrace_Ctx.CacheBufferYptrBottom].Line));
  }
  else
  {
    if(LcdTrace_Ctx.CacheBufferYptrBottom < LcdTrace_Ctx.CacheBufferYptrTop)
    {
      /* Virtual length for rolling */
      length = LCD_CACHE_DEPTH + LcdTrace_Ctx.CacheBufferYptrBottom ;
    }
    else
    {
      length = LcdTrace_Ctx.CacheBufferYptrBottom;
    }

    ptr = length - YWINDOW_SIZE + 1;

    for (cnt = 0 ; cnt < YWINDOW_SIZE ; cnt ++)
    {
      index = (cnt + ptr )% LCD_CACHE_DEPTH ;

      UTIL_LCD_SetTextColor(LCD_CacheBuffer[index].Color);
      UTIL_LCD_DisplayStringAtLine ((cnt + YWINDOW_MIN),
                             (uint8_t *)(LCD_CacheBuffer[index].Line));
    }
  }
}

#if (LCD_SCROLL_ENABLED > 0)
/**
  * @brief  Display previous text frame
  * @retval Status
  */
UTIL_LCD_TRACE_Status_t UTIL_LCD_TRACE_ScrollBack(void)
{
  if(LcdTrace_Ctx.ScrollActive == DISABLE)
  {
    LcdTrace_Ctx.CacheBufferYptrBottomBack = LcdTrace_Ctx.CacheBufferYptrBottom;
    LcdTrace_Ctx.CacheBufferYptrTopBack    = LcdTrace_Ctx.CacheBufferYptrTop;

    if(LcdTrace_Ctx.CacheBufferYptrBottom > LcdTrace_Ctx.CacheBufferYptrTop)
    {
      if ((LcdTrace_Ctx.CacheBufferYptrBottom - LcdTrace_Ctx.CacheBufferYptrTop) <=  YWINDOW_SIZE)
      {
        LcdTrace_Ctx.Lock = DISABLE;
        return UTIL_LCD_TRACE_ERROR;
      }
    }
    LcdTrace_Ctx.ScrollActive = ENABLE;

    if((LcdTrace_Ctx.CacheBufferYptrBottom  > LcdTrace_Ctx.CacheBufferYptrTop)&&
       (LcdTrace_Ctx.Scrolled == DISABLE ))
    {
      LcdTrace_Ctx.CacheBufferYptrBottom--;
      LcdTrace_Ctx.Scrolled = ENABLE;
    }
  }

  if(LcdTrace_Ctx.ScrollActive == ENABLE)
  {
    LcdTrace_Ctx.Lock = ENABLE;

    if(LcdTrace_Ctx.CacheBufferYptrBottom > LcdTrace_Ctx.CacheBufferYptrTop)
    {
      if((LcdTrace_Ctx.CacheBufferYptrBottom  - LcdTrace_Ctx.CacheBufferYptrTop) <  YWINDOW_SIZE )
      {
        LcdTrace_Ctx.Lock = DISABLE;
        return UTIL_LCD_TRACE_ERROR;
      }

      LcdTrace_Ctx.CacheBufferYptrBottom --;
    }
    else if(LcdTrace_Ctx.CacheBufferYptrBottom <= LcdTrace_Ctx.CacheBufferYptrTop)
    {
      if((LCD_CACHE_DEPTH  - LcdTrace_Ctx.CacheBufferYptrTop + LcdTrace_Ctx.CacheBufferYptrBottom) < YWINDOW_SIZE)
      {
        LcdTrace_Ctx.Lock = DISABLE;
        return UTIL_LCD_TRACE_ERROR;
      }
      LcdTrace_Ctx.CacheBufferYptrBottom --;

      if(LcdTrace_Ctx.CacheBufferYptrBottom == 0xFFFF)
      {
        LcdTrace_Ctx.CacheBufferYptrBottom = LCD_CACHE_DEPTH - 2;
      }
    }
    LcdTrace_Ctx.ScrollBackStep++;
    UTIL_LCD_TRACE_UpdateDisplay();
    LcdTrace_Ctx.Lock = DISABLE;
  }
  return UTIL_LCD_TRACE_OK;
}

/**
  * @brief  Display next text frame
  * @retval Status
  */
UTIL_LCD_TRACE_Status_t UTIL_LCD_TRACE_ScrollForward(void)
{

  if(LcdTrace_Ctx.ScrollBackStep != 0)
  {
    if(LcdTrace_Ctx.ScrollActive == DISABLE)
    {

      LcdTrace_Ctx.CacheBufferYptrBottomBack = LcdTrace_Ctx.CacheBufferYptrBottom;
      LcdTrace_Ctx.CacheBufferYptrTopBack    = LcdTrace_Ctx.CacheBufferYptrTop;

      if(LcdTrace_Ctx.CacheBufferYptrBottom > LcdTrace_Ctx.CacheBufferYptrTop)
      {

        if ((LcdTrace_Ctx.CacheBufferYptrBottom - LcdTrace_Ctx.CacheBufferYptrTop) <=  YWINDOW_SIZE)
        {
          LcdTrace_Ctx.Lock = DISABLE;
          return UTIL_LCD_TRACE_ERROR;
        }
      }
      LcdTrace_Ctx.ScrollActive = ENABLE;

      if((LcdTrace_Ctx.CacheBufferYptrBottom  > LcdTrace_Ctx.CacheBufferYptrTop)&&
         (LcdTrace_Ctx.Scrolled == DISABLE ))
      {
        LcdTrace_Ctx.CacheBufferYptrBottom--;
        LcdTrace_Ctx.Scrolled = ENABLE;
      }
    }

    if(LcdTrace_Ctx.ScrollActive == ENABLE)
    {
      LcdTrace_Ctx.Lock = ENABLE;
      LcdTrace_Ctx.ScrollBackStep--;

      if(++LcdTrace_Ctx.CacheBufferYptrBottom == LCD_CACHE_DEPTH)
      {
        LcdTrace_Ctx.CacheBufferYptrBottom = 0;
      }

      UTIL_LCD_TRACE_UpdateDisplay();
      LcdTrace_Ctx.Lock = DISABLE;
    }
    return UTIL_LCD_TRACE_OK;
  }
  else /* ScrollBackStep == 0 */
  {
    LcdTrace_Ctx.Lock = DISABLE;
    return UTIL_LCD_TRACE_ERROR;
  }
}
#endif /* LCD_SCROLL_ENABLED */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
