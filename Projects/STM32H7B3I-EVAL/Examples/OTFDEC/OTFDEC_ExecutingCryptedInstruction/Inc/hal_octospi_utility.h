/**
  ******************************************************************************
  * @file    hal_octospi_utility.c
  * @author  MCD Application Team
  * @brief   This file provides helper to initialize and OCTOSPI/externalFlash.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
