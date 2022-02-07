/**
  ******************************************************************************
  * @file    bsp.h
  * @author  MCD Application Team
  * @brief   Header for bsp.c file
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
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_discovery.h"
#include "stm32h7b3i_discovery_bus.h"
#include "stm32h7b3i_discovery_sdram.h"
#include "stm32h7b3i_discovery_ospi.h"

/* Standard includes components */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define AUTO_DEMO_TIMEOUT_0       20
#define AUTO_DEMO_TIMEOUT_1        5
#define BKP_REG_PWR_CFG           (TAMP->BKP28R)
#define BKP_REG_SW_CFG            (TAMP->BKP27R)
#define BKP_REG_SUBDEMO_ADDRESS   (TAMP->BKP26R)

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MPU_Config(void);
uint8_t BspInit(void);
void BSP_SetSubDemoAddress(uint32_t JumpAddress);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

