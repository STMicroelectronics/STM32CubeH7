/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_RegToMemWithLCD/Inc/main.h 
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
#include "stm32h7b3i_discovery.h"
#include "stm32h7b3i_discovery_lcd.h"
#include "stm32h7b3i_discovery_sdram.h"

#define LCD_FRAME_BUFFER  0xD0000000
#define QVGA_RES_X  320
#define QVGA_RES_Y  240

#define LAYER_SIZE_X    QVGA_RES_X
#define LAYER_SIZE_Y    QVGA_RES_Y



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

