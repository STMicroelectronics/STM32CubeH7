/**
  ******************************************************************************
  * @file    lcd_trace.h
  * @author  MCD Application Team
  * @brief   header for the lcd_trace.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  LCD_TRACE_H
#define  LCD_TRACE_H

/* Includes ------------------------------------------------------------------*/
#include "utilities_conf.h"
#include "stm32_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup LCD_TRACE
  * @{
  */

/** @defgroup LCD_TRACE_Exported_Defines Exported Defines
  * @{
  */
#if (LCD_SCROLL_ENABLED == 1)
 #define     LCD_CACHE_DEPTH     (YWINDOW_SIZE + CACHE_SIZE)
#else
 #define     LCD_CACHE_DEPTH     YWINDOW_SIZE
#endif
/**
  * @}
  */

/** @defgroup LCD_TRACE_Exported_Types Exported Types
  * @{
  */
typedef struct _LCD_TRACE_line
{
  uint8_t  Line[128];
  uint32_t Color;
}UTIL_LCD_TRACE_Line_t;

typedef struct
{
  uint8_t  CacheBufferYptrInvert;
  uint8_t  ScrollActive;
  uint8_t  Lock;
  uint8_t  Scrolled;
  uint16_t CacheBufferXptr;
  uint16_t CacheBufferYptrTop;
  uint16_t CacheBufferYptrBottom;
  uint16_t CacheBufferYptrTopBack;
  uint16_t CacheBufferYptrBottomBack;
  uint16_t ScrollBackStep;
  uint32_t LineColor;
  uint32_t YWindowSize;
}UTIL_LCD_TRACE_Ctx_t;

typedef enum {
  UTIL_LCD_TRACE_OK              = 0,   /* Operation terminated successfully  */
  UTIL_LCD_TRACE_ERROR           = 1,   /* Operation terminated with error    */
  UTIL_LCD_TRACE_UNKNOWN_ERROR   = 2,   /* Unknown Error                      */
} UTIL_LCD_TRACE_Status_t;

/**
  * @}
  */

/** @addtogroup LCD_TRACE_Exported_Variables
  * @{
  */
extern UTIL_LCD_TRACE_Ctx_t LcdTrace_Ctx;
/**
  * @}
  */

/** @defgroup LCD_TRACE_Exported_Macros Exported Macros
  * @{
  */
#define  LCD_ErrTrace(...)    do { \
                                 LcdTrace_Ctx.LineColor = UTIL_LCD_COLOR_RED;\
                                 printf("ERROR: ") ;\
                                 printf(__VA_ARGS__);\
                                 LcdTrace_Ctx.LineColor = LCD_TRACE_DEFAULT_COLOR;\
                               }while (0)

#define  LCD_UsrTrace(...)    do { \
	                             LcdTrace_Ctx.LineColor = LCD_TRACE_TEXT_COLOR;\
                                 printf(__VA_ARGS__);\
                               } while (0)


#define  LCD_DbgTrace(...)    do { \
                                 LcdTrace_Ctx.LineColor = UTIL_LCD_COLOR_CYAN;\
                                 printf(__VA_ARGS__);\
                                 LcdTrace_Ctx.LineColor = LCD_TRACE_DEFAULT_COLOR;\
                               }while (0)
/**
  * @}
  */

/** @defgroup LCD_TRACE_Exported_FunctionsPrototype Exported Functions Prototype
  * @{
  */
UTIL_LCD_TRACE_Status_t UTIL_LCD_TRACE_Init(void);
void UTIL_LCD_TRACE_DeInit(void);
void UTIL_LCD_TRACE_SetHeader (uint8_t *Header);
void UTIL_LCD_TRACE_SetFooter(uint8_t *Footer);
void UTIL_LCD_TRACE_ClearTextZone(void);
void UTIL_LCD_TRACE_UpdateDisplay (void);

#if (LCD_SCROLL_ENABLED > 0)
 UTIL_LCD_TRACE_Status_t UTIL_LCD_TRACE_ScrollBack(void);
 UTIL_LCD_TRACE_Status_t UTIL_LCD_TRACE_ScrollForward(void);
#endif
/**
  * @}
  */

#endif /* LCD_TRACE_H */

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
