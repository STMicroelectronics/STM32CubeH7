/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7
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
#include "stm32h7xx_hal.h"

#include "stm32h747i_eval.h"
#include "stm32h747i_eval_io.h"
#include "stm32h747i_eval_lcd.h"
#include "stm32h747i_eval_sdram.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LCD_FRAME_BUFFER         0xD0000000
#define JPEG_OUTPUT_DATA_BUFFER  0xD0200000 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
