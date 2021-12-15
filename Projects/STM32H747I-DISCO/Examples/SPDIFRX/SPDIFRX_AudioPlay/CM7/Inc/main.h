/**
  ******************************************************************************
  * @file    SPDIFRX/SPDIFRX_AudioPlay/CM7/Inc/main.h
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
#include "stm32h7xx_hal.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_bus.h"
#include "audio.h"
#include "../Components/wm8994/wm8994.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* SAI peripheral configuration defines */
#define AUDIO_SAIx                          SAI1_Block_A
#define AUDIO_SAIx_CLK_ENABLE()             __HAL_RCC_SAI1_CLK_ENABLE()

#define AUDIO_SAIx_FS_GPIO_PORT             GPIOE
#define AUDIO_SAIx_FS_AF                    GPIO_AF6_SAI1
#define AUDIO_SAIx_FS_PIN                   GPIO_PIN_4
#define AUDIO_SAIx_SCK_GPIO_PORT            GPIOE
#define AUDIO_SAIx_SCK_AF                   GPIO_AF6_SAI1
#define AUDIO_SAIx_SCK_PIN                  GPIO_PIN_5
#define AUDIO_SAIx_SD_GPIO_PORT             GPIOE
#define AUDIO_SAIx_SD_AF                    GPIO_AF6_SAI1
#define AUDIO_SAIx_SD_PIN                   GPIO_PIN_6
#define AUDIO_SAIx_MCLK_GPIO_PORT           GPIOG
#define AUDIO_SAIx_MCLK_AF                  GPIO_AF6_SAI1
#define AUDIO_SAIx_MCLK_PIN                 GPIO_PIN_7

#define AUDIO_SAIx_MCLK_ENABLE()            __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_SAIx_SCK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_SAIx_FS_ENABLE()              __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_SAIx_SD_ENABLE()              __HAL_RCC_GPIOE_CLK_ENABLE()

/* SPDIFRX peripheral configuration defines */
#define AUDIO_SPDIFRX_CLK_ENABLE()          __HAL_RCC_SPDIFRX_CLK_ENABLE()
#define AUDIO_SPDIFRX_GPIO_PORT             GPIOD
#define AUDIO_SPDIFRX_AF                    GPIO_AF9_SPDIF
#define AUDIO_SPDIFRX_PIN                   GPIO_PIN_7
#define AUDIO_SPDIFRX_IN_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
/* Audio codec I2C address */
#define AUDIO_I2C_ADDRESS                  0x34U
#define VOLUME_OUT_CONVERT(Volume)    (((Volume) > 100)? 63:((uint8_t)(((Volume) * 63) / 100)))
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

