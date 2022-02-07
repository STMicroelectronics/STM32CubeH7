/**
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
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
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_lcd.h"
#include "stm32h747i_discovery_sdram.h"
#include "stm32h747i_discovery_camera.h"
#include "stm32h747i_discovery_bus.h"
#include "otm8009a.h"
#include "stm32_lcd.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */

#define LCD_FRAME_BUFFER                  0xD0000000 /* LCD Frame buffer of size 800x480 in ARGB8888 */

#define CAMERA_FRAME_BUFFER               0xD0177000


#define ARGB8888_BYTE_PER_PIXEL  4

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

