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
/* Standard includes components */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BKP_REG_SUBDEMO_ADDRESS   (TAMP->BKP26R)

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void BSP_SetSubDemoAddress(uint32_t JumpAddress);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

