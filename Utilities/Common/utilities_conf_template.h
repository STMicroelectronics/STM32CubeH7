/**
 ******************************************************************************
 * @file    utilities_conf.h
 * @author  MCD Application Team
 * @brief   Configuration file to utilities
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
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
/* Uncomment below line and replace 'boardname' with your board name, ex: stm324x9i_eval_lcd.h */
/* #include "boardname_lcd.h" */
#include "cmsis_compiler.h"
#include "stdio.h"
#include "string.h"

/******************************************************************************
 * common
 ******************************************************************************/
#define UTILS_ENTER_CRITICAL_SECTION( )         uint32_t primask_bit = __get_PRIMASK( );\
                                                __disable_irq( )

#define UTILS_EXIT_CRITICAL_SECTION( )          __set_PRIMASK( primask_bit )

#define UTILS_MEMSET8(dest, value, size)        memset((dest),(value),(size));

/******************************************************************************
 * tim_serv
 * (any macro that does not need to be modified can be removed)
 ******************************************************************************/
#define UTIL_TIMER_INIT_CRITICAL_SECTION( )
#define UTIL_TIMER_ENTER_CRITICAL_SECTION( )    UTILS_ENTER_CRITICAL_SECTION( )
#define UTIL_TIMER_EXIT_CRITICAL_SECTION( )     UTILS_EXIT_CRITICAL_SECTION( )

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
 * trace\advanced
 * the define option 
 *    UTIL_ADV_TRACE_CONDITIONNAL shall be defined if you want use conditional function
 *    UTIL_ADV_TRACE_UNCHUNK_MODE shall be defined if you want use the unchunk mode
 *    UTIL_ADV_TRACE_MEMLOCATION shall be defined if you want manage trace buffer location 
 ******************************************************************************/
#define UTIL_ADV_TRACE_CONDITIONNAL
#define UTIL_ADV_TRACE_UNCHUNK_MODE
#define UTIL_ADV_TRACE_DEBUG(...)
#define UTIL_ADV_TRACE_INIT_CRITICAL_SECTION( )    UTILS_INIT_CRITICAL_SECTION()
#define UTIL_ADV_TRACE_ENTER_CRITICAL_SECTION( )   UTILS_ENTER_CRITICAL_SECTION()
#define UTIL_ADV_TRACE_EXIT_CRITICAL_SECTION( )    UTILS_EXIT_CRITICAL_SECTION()
#define UTIL_ADV_TRACE_TMP_BUF_SIZE                (256U)
#define UTIL_ADV_TRACE_TMP_MAX_TIMESTMAP_SIZE      (15U)
#define UTIL_ADV_TRACE_FIFO_SIZE                   (512U)
#define UTIL_ADV_TRACE_MEMSET8( dest, value, size) UTIL_MEM_set_8((dest),(value),(size))
#define UTIL_ADV_TRACE_VSNPRINTF(...)              vsnprintf(__VA_ARGS__)
#define UTIL_ADV_TRACE_MEMLOCATION

/******************************************************************************
 * LCD trace Configuration
 ******************************************************************************/
#define USE_LCD_TRACE                            0U

/* Enable LCD scrolling */
#define     LCD_SCROLL_ENABLED                   1U

/* Define the LCD TRACE Fonts  */
#define     LCD_TRACE_HEADER_FONT                Font16
#define     LCD_TRACE_FOOTER_FONT                Font12
#define     LCD_TRACE_TEXT_FONT                  Font20

/* Define the LCD TRACE Colors  */
#define     LCD_TRACE_BACKGROUND_COLOR           UTILS_LCD_COLOR_WHITE
#define     LCD_TRACE_TEXT_COLOR                 UTILS_LCD_COLOR_DARKBLUE
#define     LCD_TRACE_SOLID_BACKGROUND_COLOR     UTILS_LCD_COLOR_BLUE
#define     LCD_TRACE_SOLID_TEXT_COLOR           UTILS_LCD_COLOR_WHITE
#define     LCD_TRACE_DEFAULT_COLOR              UTILS_LCD_COLOR_WHITE

/* Define the cache depth */
#define     CACHE_SIZE                           100U

/* The max value of YWINDOW_SIZE is calculated as follow:
   ((LCD Height - HEADER FONT height - FOOTER FONT height)/ TEXT FONT height) - 1 */
#define     YWINDOW_SIZE                         19U

/******************************************************************************
 * ITM trace Configuration
 ******************************************************************************/
#define USE_ITM_TRACE                            1U

#ifdef __cplusplus
}
#endif

#endif /*UTILITIES_CONF_H */
