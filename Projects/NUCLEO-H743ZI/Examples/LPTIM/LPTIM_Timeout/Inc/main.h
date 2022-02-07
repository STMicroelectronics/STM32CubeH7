/**
  ******************************************************************************
  * @file    LPTIM/LPTIM_Timeout/Inc/main.h
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
#include "stm32h7xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for LPTIMx clock resources */
#define LPTIMx                           LPTIM1
#define LPTIMx_CLK_ENABLE                __HAL_RCC_LPTIM1_CLK_ENABLE

/* Definition for LPTIMx Pins */
#define LPTIMx_GPIO_PORT_ENABLE         __HAL_RCC_GPIOG_CLK_ENABLE()
#define LPTIMx_ETR_GPIO_PORT            GPIOG
#define GPIO_PIN_ETR                    GPIO_PIN_14
#define GPIO_PUPD_ETR                   GPIO_PULLUP
#define GPIO_AF_LPTIMx_ETR              GPIO_AF1_LPTIM1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

