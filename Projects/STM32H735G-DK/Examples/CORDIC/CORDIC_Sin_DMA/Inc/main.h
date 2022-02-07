/**
  ******************************************************************************
  * @file    CORDIC/CORDIC_Sin_DMA/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
#include "stm32h735g_discovery.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


/* Private defines -----------------------------------------------------------*/
/* Pass/Fail Status */
#define PASS              0
#define FAIL              1

/* Size of data array */
#define ARRAY_SIZE        64U

/* Reference values in Q1.31 format */
#define DELTA             (int32_t)0x00001000       /* Max residual error for sines, with 6 cycle precision:
                                                       2^-19 max residual error, ie 31-19=12 LSB, ie <0x1000 */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

