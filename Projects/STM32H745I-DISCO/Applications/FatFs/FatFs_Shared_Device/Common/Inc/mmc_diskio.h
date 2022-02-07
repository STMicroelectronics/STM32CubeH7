/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/Common/Inc/mmc_diskio.h
  * @author  MCD Application Team
  * @brief   Header for mmc_diskio.c module
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
#ifndef __MMC_DISKIO_H
#define __MMC_DISKIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h745i_discovery_mmc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern const Diskio_drvTypeDef  MMC_Driver;

#endif /* __MMC_DISKIO_H */


