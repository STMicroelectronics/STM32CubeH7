/**
  ******************************************************************************
  * @file    bsp.h
  * @author  MCD Application Team
  * @brief   Header for bsp.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h735g_discovery.h"
#include "stm32h735g_discovery_bus.h"
#include "stm32h735g_discovery_ospi.h"
#include "stm32h735g_discovery_ts.h"

/* Standard includes components */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define AUTO_DEMO_TIMEOUT_0       20
#define AUTO_DEMO_TIMEOUT_1        5
#define BKP_REG_DEMO_LOC          (RTC->BKP29R)
#define BKP_REG_PWR_CFG           (RTC->BKP28R)
#define BKP_REG_SW_CFG            (RTC->BKP27R)

   /**
  * @brief LCD special pins
  */
/* LCD Display control pin */
#define LCD_DISP_CTRL_PIN                     GPIO_PIN_10
#define LCD_DISP_CTRL_PULL                    GPIO_PULLUP
#define LCD_DISP_CTRL_GPIO_PORT               GPIOD
#define LCD_DISP_CTRL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define LCD_DISP_CTRL_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOD_CLK_DISABLE()

/* LCD Display enable pin */
#define LCD_DISP_EN_PIN                      GPIO_PIN_13
#define LCD_DISP_EN_PULL                     GPIO_NOPULL
#define LCD_DISP_EN_GPIO_PORT                GPIOE
#define LCD_DISP_EN_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOE_CLK_ENABLE()
#define LCD_DISP_EN_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOK_CLK_DISABLE()

/* Back-light control pin */
#define LCD_BL_CTRL_PIN                       GPIO_PIN_15
#define LCD_BL_CTRL_PULL                      GPIO_NOPULL
#define LCD_BL_CTRL_GPIO_PORT                 GPIOG
#define LCD_BL_CTRL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOG_CLK_ENABLE()
#define LCD_BL_CTRL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOG_CLK_DISABLE()

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MPU_Config(void);
uint8_t BspInit(const uint8_t minimal);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

