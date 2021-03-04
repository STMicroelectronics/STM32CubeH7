/**
******************************************************************************
* @file    HDP_Boot/Inc/main.h
* @author  MCD Application Team
* @brief   Header for main.c module for Cortex-M7.
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32h7b3i_eval.h"
#include "stm32h7b3i_eval_lcd.h"
#include "stm32h7b3i_eval_sdram.h"
#include "stm32h7b3i_eval_io.h"
#include "stm32_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern __IO uint32_t ButtonState;

/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */
#define LCD_FRAME_BUFFER                  0xD0000000 /* LCD Frame buffer of size 800x480 in ARGB8888 */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
