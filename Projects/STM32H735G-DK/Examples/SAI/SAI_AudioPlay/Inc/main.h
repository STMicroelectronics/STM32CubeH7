/**
  ******************************************************************************
  * @file    SAI/SAI_AudioPlay/Inc/main.h
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
#include "stm32h735g_discovery.h"
#include "stm32h735g_discovery_bus.h"
#include "audio.h"
/* Include audio component Driver */
#include "../Components/wm8994/wm8994.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

  
/* SAI peripheral configuration defines */
#define AUDIO_SAIx                           SAI1_Block_B
#define AUDIO_SAIx_CLK_ENABLE()              __HAL_RCC_SAI1_CLK_ENABLE()

#define AUDIO_SAIx_FS_GPIO_PORT              GPIOF
#define AUDIO_SAIx_FS_AF                     GPIO_AF6_SAI1
#define AUDIO_SAIx_FS_PIN                    GPIO_PIN_9
#define AUDIO_SAIx_SCK_GPIO_PORT             GPIOF
#define AUDIO_SAIx_SCK_AF                    GPIO_AF6_SAI1
#define AUDIO_SAIx_SCK_PIN                   GPIO_PIN_8
#define AUDIO_SAIx_SD_GPIO_PORT              GPIOF
#define AUDIO_SAIx_SD_AF                     GPIO_AF6_SAI1
#define AUDIO_SAIx_SD_PIN                    GPIO_PIN_6
#define AUDIO_SAIx_MCLK_GPIO_PORT            GPIOF
#define AUDIO_SAIx_MCLK_AF                   GPIO_AF6_SAI1
#define AUDIO_SAIx_MCLK_PIN                  GPIO_PIN_7
   
#define AUDIO_SAIx_MCLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAIx_SCK_ENABLE()              __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAIx_FS_ENABLE()               __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAIx_SD_ENABLE()               __HAL_RCC_GPIOF_CLK_ENABLE()
   

#endif /* __MAIN_H */

