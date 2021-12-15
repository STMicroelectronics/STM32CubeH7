/**
  ******************************************************************************
  * @file    RAMECC/RAMECC_ErrorCount/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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
#include "stm32h7xx_nucleo.h"
#include "common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RAMECC_MONITOR         RAMECC1_Monitor1      /* AXI-SRAM Monitoring   */

#define MEMORY_BASE              0x24000000UL        /* AXI-SRAM base address */
#define MEMORY_SIZE              0x00080000UL        /* AXI-SRAM size         */
#define INC_OFFSET               0x00000004UL        /* AXI-SRAM access width */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

