/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_Arbitary_Waveform/Inc/main.h 
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
#include "stm32h743i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define HRTIMx                            HRTIM11

#define HRTIM_CLK_ENABLE()               __HAL_RCC_HRTIM1_CLK_ENABLE()
#define HRTIM_FORCE_RESET()              __HAL_RCC_HRTIM1_FORCE_RESET()
#define HRTIM_RELEASE_RESET()            __HAL_RCC_HRTIM1_RELEASE_RESET()

/* Definition for HRTIM Channel Pins */
#define HRTIM_TIMD_CHANNEL1_GPIO_PORT    GPIOA
#define HRTIM_TIMD_CHANNEL_PIN1          GPIO_PIN_11
#define HRTIM_TIMD_GPIO_AF_CHANNEL1      GPIO_AF2_HRTIM1

#define HRTIM_TIMD_CHANNEL2_GPIO_PORT    GPIOA
#define HRTIM_TIMD_CHANNEL_PIN2          GPIO_PIN_12
#define HRTIM_TIMD_GPIO_AF_CHANNEL2      GPIO_AF2_HRTIM1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

