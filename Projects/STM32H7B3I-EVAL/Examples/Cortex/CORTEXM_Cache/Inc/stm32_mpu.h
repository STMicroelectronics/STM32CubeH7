/**
  ******************************************************************************
  * @file    Cortex/CORTEXM_Cache/Inc/stm32_mpu.h
  * @author  MCD Application Team
  * @brief   Header for stm32_mpu.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
