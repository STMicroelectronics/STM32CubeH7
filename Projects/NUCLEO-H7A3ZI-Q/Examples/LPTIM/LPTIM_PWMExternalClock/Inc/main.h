/**
  ******************************************************************************
  * @file    LPTIM/LPTIM_PWMExternalClock/Inc/main.h
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
#include "stm32h7xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for LPTIMx clock resources */
#define LPTIMx                           LPTIM1
#define LPTIMx_CLK_ENABLE                __HAL_RCC_LPTIM1_CLK_ENABLE

/* Definition for LPTIMx Pins */
#define LPTIMx_GPIO_PORT_ENABLE         __HAL_RCC_GPIOD_CLK_ENABLE();
#define LPTIMx_OUT_GPIO_PORT            GPIOD
#define LPTIMx_IN1_GPIO_PORT            GPIOD
#define GPIO_PIN_OUT                    GPIO_PIN_12
#define GPIO_PIN_IN1                    GPIO_PIN_13
#define GPIO_PUPD_OUT                   GPIO_PULLUP
#define GPIO_PUPD_IN1                   GPIO_PULLUP
#define GPIO_AF_LPTIMx_OUT              GPIO_AF1_LPTIM1
#define GPIO_AF_LPTIMx_IN1              GPIO_AF1_LPTIM1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

