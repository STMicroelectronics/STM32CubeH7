/**
  ******************************************************************************
  * @file    BSP/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32h7xx_hal.h"
#include "stm32h735g_discovery.h"
#include "stm32h735g_discovery_lcd.h"
#include "stm32h735g_discovery_ospi.h"
#include "stm32_lcd.h"

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern __IO uint32_t ButtonState;

/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */
#define LCD_FRAME_BUFFER                  0xD0000000 /* LCD Frame buffer of size 480x272 in ARGB8888 */

/* OSPI write address */
#define OSPI_RAM_WRITE_READ_ADDR             0x70400000
#define OSPI_RAM_WRITE_READ_ADDR_OFFSET ((uint32_t)0x0800)


/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
