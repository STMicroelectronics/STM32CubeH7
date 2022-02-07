/**
 ******************************************************************************
 * @file    utilities_conf.h
 * @author  MCD Application Team
 * @brief   Configuration file to utilities
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UTILITIES_CONF_H
#define UTILITIES_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h750b_discovery_lcd.h"
#include "cmsis_compiler.h"
#include "string.h"

/******************************************************************************
 * common
 ******************************************************************************/
#define UTILS_ENTER_CRITICAL_SECTION( )         uint32_t primask_bit = __get_PRIMASK( );\
                                                __disable_irq( )

#define UTILS_EXIT_CRITICAL_SECTION( )          __set_PRIMASK( primask_bit )

#define UTILS_MEMSET8(dest, value, size)        memset((dest),(value),(size));

/******************************************************************************
 * tiny low power manager
 * (any macro that does not need to be modified can be removed)
 ******************************************************************************/
#define UTIL_LPM_INIT_CRITICAL_SECTION( )
#define UTIL_LPM_ENTER_CRITICAL_SECTION( )      UTILS_ENTER_CRITICAL_SECTION( )
#define UTIL_LPM_EXIT_CRITICAL_SECTION( )       UTILS_EXIT_CRITICAL_SECTION( )


/******************************************************************************
 * sequencer
 * (any macro that does not need to be modified can be removed)
 ******************************************************************************/
#define UTIL_SEQ_INIT_CRITICAL_SECTION( )
#define UTIL_SEQ_ENTER_CRITICAL_SECTION( )      UTILS_ENTER_CRITICAL_SECTION( )
#define UTIL_SEQ_EXIT_CRITICAL_SECTION( )       UTILS_EXIT_CRITICAL_SECTION( )
#define UTIL_SEQ_CONF_TASK_NBR                  (32U)
#define UTIL_SEQ_CONF_PRIO_NBR                  (2U)
#define UTIL_SEQ_MEMSET8( dest, value, size )   UTILS_MEMSET8((dest),(value),(size))

/******************************************************************************
 * LCD trace Configuration
 ******************************************************************************/
#define USE_LCD_TRACE                            1U

/* Enable LCD scrolling */
#define     LCD_SCROLL_ENABLED                   1U

/* Define the LCD TRACE Fonts  */
#define     LCD_TRACE_HEADER_FONT                Font16
#define     LCD_TRACE_FOOTER_FONT                Font12
#define     LCD_TRACE_TEXT_FONT                  Font12

/* Define the LCD TRACE Colors  */
#define     LCD_TRACE_BACKGROUND_COLOR           UTIL_LCD_COLOR_BLACK
#define     LCD_TRACE_TEXT_COLOR                 UTIL_LCD_COLOR_WHITE
#define     LCD_TRACE_SOLID_BACKGROUND_COLOR     UTIL_LCD_COLOR_BLUE
#define     LCD_TRACE_SOLID_TEXT_COLOR           UTIL_LCD_COLOR_WHITE
#define     LCD_TRACE_DEFAULT_COLOR              UTIL_LCD_COLOR_WHITE

/* Define the cache depth */
#define     CACHE_SIZE                           100U

/* The max value of YWINDOW_SIZE is calculated as follow:
   ((LCD Height - HEADER FONT height - FOOTER FONT height)/ TEXT FONT height) - 1 */
#define     YWINDOW_SIZE                         13U

/******************************************************************************
 * ITM trace Configuration
 ******************************************************************************/
#define USE_ITM_TRACE                            1U

#ifdef __cplusplus
}
#endif

#endif /*UTILITIES_CONF_H */

