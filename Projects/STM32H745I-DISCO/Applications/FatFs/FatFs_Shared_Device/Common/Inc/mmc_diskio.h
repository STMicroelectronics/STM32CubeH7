/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/Common/Inc/mmc_diskio.h
  * @author  MCD Application Team
  * @brief   Header for mmc_diskio.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

