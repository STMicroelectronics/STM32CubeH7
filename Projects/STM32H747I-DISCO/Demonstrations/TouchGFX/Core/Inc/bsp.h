/**
  ******************************************************************************
  * @file    bsp.h
  * @author  MCD Application Team
  * @brief   header of bsp.c module
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
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_discovery_ts.h"
#include "stm32h747i_discovery_sd.h"
/* Standard includes components */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define AUTO_DEMO_TIMEOUT_0       20
#define AUTO_DEMO_TIMEOUT_1        5
#define BKP_REG_PWR_CFG           (RTC->BKP28R)
#define BKP_REG_SW_CFG            (RTC->BKP27R)

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
uint8_t BspInit(void);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
