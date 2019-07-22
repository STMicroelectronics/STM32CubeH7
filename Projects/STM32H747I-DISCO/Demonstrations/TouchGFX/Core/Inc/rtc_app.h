/**
  ******************************************************************************
  * @file    rtc_app.h
  * @author  MCD Application Team
  * @brief   Header for rtc_app.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
