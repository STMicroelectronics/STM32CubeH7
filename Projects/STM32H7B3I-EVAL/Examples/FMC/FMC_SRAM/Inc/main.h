/**
  ******************************************************************************
  * @file    FMC/FMC_SRAM/Inc/main.h
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_eval.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Exported constants --------------------------------------------------------*/
#define SRAM_BANK_ADDR  ((uint32_t)0x68000000)
  
/* #define SRAM_MEMORY_WIDTH            FMC_NORSRAM_MEM_BUS_WIDTH_8  */
#define SRAM_MEMORY_WIDTH               FMC_NORSRAM_MEM_BUS_WIDTH_16

#define SRAM_TIMEOUT     ((uint32_t)0xFFFF)

#endif /* __MAIN_H */

