/**
  ******************************************************************************
  * @file    Cortex/CORTEXM_Cache/Inc/stm32_mpu.h
  * @author  MCD Application Team
  * @brief   Header for stm32_mpu.c module
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
#ifndef __STM32_MPU_H
#define __STM32_MPU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MPU_Config_WT(void);
void MPU_Config_WB_WA(void);
void MPU_Config_WB_nWA(void);
void MPU_Config_nCacheable(void);

#endif /* __STM32_MPU_H */

