/**
  ******************************************************************************
  * @file    Demonstrations/Inc/main.h
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
#include "stm32h7xx_nucleo.h"
#include "adafruit_802.h"
#include "adafruit_802_sd.h"
#include "adafruit_802_lcd.h"
#include "stm32_lcd.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "fatfs_storage.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define  MAX_BMP_FILES  25
#define  MAX_BMP_FILE_NAME 11
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

