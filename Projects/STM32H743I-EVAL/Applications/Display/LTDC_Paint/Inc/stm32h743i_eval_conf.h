/**
  ******************************************************************************
  * @file    stm32h743i_eval_config.h
  * @author  MCD Application Team
  * @brief   STM32h743i_EVAL board configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32h743i_EVAL_CONFIG_H
#define STM32h743i_EVAL_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* COM usage define */
#define USE_COM_LOG                         1U
#define USE_BSP_COM_FEATURE                 1U

/* POT usage define */
#define USE_BSP_POT_FEATURE                 1U

/* JOY usage define */
#define USE_BSP_JOY_FEATURE                 1U

/* IO class usage define */
#define USE_BSP_IO_CLASS                    1U

/* LCD controllers defines */
#define LCD_LAYER_0_ADDRESS               0xD0000000U  
#define LCD_LAYER_1_ADDRESS               0xD0130000U

/* Audio codecs defines */
#define USE_AUDIO_CODEC_WM8994              1U
#define DEFAULT_AUDIO_IN_BUFFER_SIZE        2048U
#define USE_BSP_CPU_CACHE_MAINTENANCE       0U

/* TS supported features defines */
#define TS_TOUCH_NBR                        1U
#define USE_TS3510_TS_CTRL                  1U
#define USE_EXC7200_TS_CTRL                 1U

/* IRQ priorities */
#define BSP_SRAM_IT_PRIORITY                15U
#define BSP_SDRAM_IT_PRIORITY               15U
#define BSP_IOEXPANDER_IT_PRIORITY          15U
#define BSP_BUTTON_USER_IT_PRIORITY         15U
#define BSP_BUTTON_WAKEUP_IT_PRIORITY       15U
#define BSP_BUTTON_TAMPER_IT_PRIORITY       15U
#define BSP_AUDIO_OUT_IT_PRIORITY           14U
#define BSP_AUDIO_IN_IT_PRIORITY            15U
#define BSP_SD_IT_PRIORITY                  14U
#define BSP_SD_RX_IT_PRIORITY               14U
#define BSP_SD_TX_IT_PRIORITY               15U
#define BSP_TS_IT_PRIORITY                  15U

#ifdef __cplusplus
}
#endif

#endif /* STM32h743i_EVAL_CONFIG_H */

