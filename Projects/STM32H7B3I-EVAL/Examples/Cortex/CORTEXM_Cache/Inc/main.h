/**
  ******************************************************************************
  * @file    Cortex/CORTEXM_Cache/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_eval.h"
#include "stm32_mpu.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the corresponding line to select the MPU configuration
   Note: Please refer to specific note about MPU_WRITE_THROUGH configuration
         in readme.txt */

/* #define MPU_WRITE_THROUGH                */
/* #define MPU_WRITE_BACK_WRITE_ALLOCATE    */
#define MPU_WRITE_BACK_NO_WRITE_ALLOCATE

#define DMA_INSTANCE               DMA1_Stream0
#define DMA_REQUEST                DMA_REQUEST_MEM2MEM
#define DMA_INSTANCE_IRQ           DMA1_Stream0_IRQn

#define BUFFER_SIZE                640   /* 80 Cache Lines */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
