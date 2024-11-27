/**
  ******************************************************************************
  * @file    Templates_LL/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_utils.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Set this define to 1 to OverClocking the system clock to 480MHz*/
#define USE_VOS0_480MHZ_OVERCLOCK    1

/**
  * @brief LED1 
  */
#define LED1_PIN                           LL_GPIO_PIN_0
#define LED1_GPIO_PORT                     GPIOB
#define LED1_GPIO_CLK_ENABLE()             LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)
#define LED1_GPIO_CLK_DISABLE()            LL_AHB4_GRP1_DisableClock(LL_AHB4_GRP1_PERIPH_GPIOB)

/**
  * @brief LED2 
  */
#if defined (USE_NUCLEO_H743ZI2)
#define LED2_PIN                                LL_GPIO_PIN_1
#define LED2_GPIO_PORT                          GPIOE
#define LED2_GPIO_CLK_ENABLE()                  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE)
#define LED2_GPIO_CLK_DISABLE()                 LL_AHB4_GRP1_DisableClock(LL_AHB4_GRP1_PERIPH_GPIOE)
#else /* USE_NUCLEO_H743ZI */
#define LED2_PIN                                LL_GPIO_PIN_7
#define LED2_GPIO_PORT                          GPIOB
#define LED2_GPIO_CLK_ENABLE()                  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)
#define LED2_GPIO_CLK_DISABLE()                 LL_AHB4_GRP1_DisableClock(LL_AHB4_GRP1_PERIPH_GPIOB)
#endif

/**
  * @brief LED3 
  */
#define LED3_PIN                           LL_GPIO_PIN_14
#define LED3_GPIO_PORT                     GPIOB
#define LED3_GPIO_CLK_ENABLE()             LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)
#define LED3_GPIO_CLK_DISABLE()            LL_AHB4_GRP1_DisableClock(LL_AHB4_GRP1_PERIPH_GPIOB)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

