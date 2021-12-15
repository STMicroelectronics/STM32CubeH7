/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FatFs/FatFs_RAMDisk/FatFs/App/Inc/app_fatfs.h
  * @author  MCD Application Team
  * @brief   Header for app_fatfs.c file
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_FATFS_H
#define __APP_FATFS_H

/* Includes ------------------------------------------------------------------*/
#include "ff.h"
#include "ff_gen_drv.h"
#include "sram_diskio.h" /* defines SRAMDISK_Driver as external */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
int32_t MX_FATFS_Init(void);
int32_t MX_FATFS_Process(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#endif /*__APP_FATFS_H */

