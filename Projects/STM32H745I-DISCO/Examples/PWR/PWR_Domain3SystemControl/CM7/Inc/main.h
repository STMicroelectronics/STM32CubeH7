/**
  ******************************************************************************
  * @file    PWR/PWR_Domain3SystemControl/CM7/Inc/main.h
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
#include "stm32h745i_discovery.h"
#include "common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx clock resources */
#define I2Cx                             I2C4
#define I2Cx_CLK_ENABLE()                __HAL_RCC_I2C4_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()

#define I2Cx_FORCE_RESET()               __HAL_RCC_I2C4_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __HAL_RCC_I2C4_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_12
#define I2Cx_SCL_GPIO_PORT              GPIOD
#define I2Cx_SCL_AF                     GPIO_AF4_I2C4
#define I2Cx_SDA_PIN                    GPIO_PIN_13
#define I2Cx_SDA_GPIO_PORT              GPIOD
#define I2Cx_SDA_AF                     GPIO_AF4_I2C4

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

