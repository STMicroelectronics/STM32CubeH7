/**
  ******************************************************************************
  * @file    rtc_app.h
  * @author  MCD Application Team
  * @brief   Header for rtc_app.c file
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
#ifndef __RTC_RTC_H
#define __RTC_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef STM32H747xx
#include "stm32h7xx_hal.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
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

