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

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void BSP_SetBrightness(uint32_t Brightness);
void BSP_PWM_Init(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

