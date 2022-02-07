/**
  ******************************************************************************
  * @file    hal_octospi_utility.c
  * @author  MCD Application Team
  * @brief   This file provides helper to initialize and OCTOSPI/externalFlash.
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
#ifndef __HAL_OCTOSPI_UTILITY_H
#define __HAL_OCTOSPI_UTILITY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OSPI_PAGE_SIZE    256
#define OSPI_SECTOR_SIZE  4096
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t OSPI_Init(uint32_t NomberOfLine, uint32_t InstanceNumber);
uint32_t OSPI_DeInit(uint32_t Idx);
uint32_t OSPI_Read (uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size, uint32_t Idx);
uint32_t OSPI_Write(uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size, uint32_t Idx);
uint32_t OSPI_MemoryMap(uint32_t  Idx);
uint32_t OSPI_EnableEIP(void);
uint32_t OSPI_Erase( uint32_t Idx);
uint32_t OSPI_Abort(void);

#endif /* __HAL_OCTOSPI_UTILITY_H */

