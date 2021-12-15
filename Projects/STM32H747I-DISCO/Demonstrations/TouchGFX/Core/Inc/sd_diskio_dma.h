/**
  ******************************************************************************
  * @file    sd_diskio_dma.h
  * @author  MCD Application Team
  * @brief   Header for sd_diskio_dma.c module
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
#ifndef __SD_DISKIO_H
#define __SD_DISKIO_H

/* Includes ------------------------------------------------------------------*/
#ifdef USE_STM32H747I_DISCO
#include "stm32h747i_discovery_sd.h"
#else
#include "stm32h747i_eval_sd.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern const Diskio_drvTypeDef  SD_Driver;

#endif /* __SD_DISKIO_H */


