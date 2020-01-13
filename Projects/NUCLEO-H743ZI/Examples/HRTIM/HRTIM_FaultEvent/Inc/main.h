/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_FaultEvent/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#include "stm32h7xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define HRTIMx                            HRTIM11

#define HRTIM_CLK_ENABLE()               __HAL_RCC_HRTIM1_CLK_ENABLE()
#define HRTIM_FORCE_RESET()              __HAL_RCC_HRTIM1_FORCE_RESET()
#define HRTIM_RELEASE_RESET()            __HAL_RCC_HRTIM1_RELEASE_RESET()

/* Definition for HRTIM Channel Pins */
#define HRTIM_TIMA_CHANNEL1_GPIO_PORT    GPIOC
#define HRTIM_TIMA_CHANNEL_PIN1          GPIO_PIN_6
#define HRTIM_TIMA_GPIO_AF_CHANNEL1      GPIO_AF1_HRTIM1

#define HRTIM_TIMA_CHANNEL2_GPIO_PORT    GPIOC
#define HRTIM_TIMA_CHANNEL_PIN2          GPIO_PIN_7
#define HRTIM_TIMA_GPIO_AF_CHANNEL2      GPIO_AF1_HRTIM1

#define HRTIM_FAULT2_GPIO_PORT           GPIOC
#define HRTIM_FAULT2_GPIO_PIN            GPIO_PIN_11
#define HRTIM_FAULT2_AF                  GPIO_AF2_HRTIM1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
