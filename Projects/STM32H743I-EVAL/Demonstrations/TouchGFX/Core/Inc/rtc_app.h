/**
  ******************************************************************************
  * @file    rtc_app.h
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    13-November-2015
  * @brief   Header for rtc_app.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_RTC_H
#define __RTC_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef STM32H743xx
#include "stm32h7xx_hal.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/**
 * RTC cloc divider
 */
#define RTC_CLOCK_SOURCE_LSE

#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV  (0x7F)
#define RTC_SYNCH_PREDIV   (0xF9)
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  (0x7F)
#define RTC_SYNCH_PREDIV   (0x00FF)
#endif

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void     RTC_CalendarBkupInit(void);
void     RTC_BkupSaveParameter(uint32_t address, uint32_t data);
uint32_t RTC_BkupRestoreParameter(uint32_t address);

void RTC_SetTime  (RTC_TimeTypeDef *Time);
void RTC_GetTime  (RTC_TimeTypeDef *Time);
void RTC_SetDate  (RTC_DateTypeDef *Date);
void RTC_GetDate  (RTC_DateTypeDef *Date);

#ifdef __cplusplus
}
#endif

#endif /*__RTC_RTC_H */

