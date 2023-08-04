/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_CmdMode_TearingEffect_ExtPin/CM7/Inc/main.h
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
#include "stm32h7xx_hal.h"

#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_lcd.h"
#include "stm32h747i_discovery_sdram.h"
#include "stm32h747i_discovery_bus.h"
#include "stm32_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LCD_FRAME_BUFFER        0xD0000000

#if (USE_LCD_CTRL_NT35510 > 0)
#define LCD_CMD_CASET                           NT35510_CMD_CASET
#define LCD_CMD_PASET                           NT35510_CMD_RASET
#define LCD_CMD_WRTESCN                         NT35510_CMD_STESL
#define LCD_CMD_DISPON                          NT35510_CMD_DISPON
#define LCD_CMD_TEOFF                           NT35510_CMD_TEOFF
#define LCD_COMP_IO                             NT35510_IO_t
#define LCD_COMP_OBJ                            NT35510_Object_t
#define LCD_COMP_RegisterBusIO                  NT35510_RegisterBusIO
#define LCD_COMP_Init                           NT35510_Init
#define LCD_COMP_COLMOD_RGB888                  NT35510_COLMOD_RGB888
#else
#define LCD_CMD_CASET                           OTM8009A_CMD_CASET
#define LCD_CMD_PASET                           OTM8009A_CMD_PASET
#define LCD_CMD_WRTESCN                         OTM8009A_CMD_WRTESCN
#define LCD_CMD_DISPON                          OTM8009A_CMD_DISPON
#define LCD_CMD_TEOFF                           OTM8009A_CMD_TEOFF
#define LCD_COMP_IO                             OTM8009A_IO_t
#define LCD_COMP_OBJ                            OTM8009A_Object_t
#define LCD_COMP_RegisterBusIO                  OTM8009A_RegisterBusIO
#define LCD_COMP_Init                           OTM8009A_Init
#define LCD_COMP_COLMOD_RGB888                  OTM8009A_COLMOD_RGB888
#endif /* USE_LCD_CTRL_NT35510 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

