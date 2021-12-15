/**
  ******************************************************************************
  * @file    SPI_FullDuplex_ComIT/Inc/main.h
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
/* LL drivers common to all LL examples */
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_utils.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition LED1-GREEN*/
#define LED1_PIN                           LL_GPIO_PIN_0
#define LED1_GPIO_PORT                     GPIOB
#define LED1_GPIO_CLK_ENABLE()             LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)

/* Definition LED3-RED*/
#define LED3_PIN                           LL_GPIO_PIN_14
#define LED3_GPIO_PORT                     GPIOB
#define LED3_GPIO_CLK_ENABLE()             LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SPI1_Rx_Callback(void);
void SPI1_Tx_Callback(void);
void SPI1_EOT_Callback(void);
void SPI_TransferError_Callback(void);

void SPI4_Rx_Callback(void);
void SPI4_Tx_Callback(void);
void SPI4_EOT_Callback(void);

#endif /* __MAIN_H */

