/**
  ******************************************************************************
  * @file    USB_Device/AUDIO_Standalone/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio.h"
#include "usbd_audio_if.h"
#include "stm32h743i_eval_audio.h"

/* Exported types ------------------------------------------------------------*/
#define LED_GREEN       LED1
#define LED_ORANGE      LED2
#define LED_RED         LED3
#define LED_BLUE        LED4

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SystemClock_Config(void);
void AUDIO_OUT_ClockConfig(SAI_HandleTypeDef * hsai, uint32_t AudioFreq, void *Params);
#endif /* __MAIN_H */

