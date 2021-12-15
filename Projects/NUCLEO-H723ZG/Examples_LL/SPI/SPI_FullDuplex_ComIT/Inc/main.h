/**
  ******************************************************************************
  * @file    SPI_FullDuplex_ComIT/Inc/main.h
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

/* Definition LED1*/
#define LED_OK_PIN                              LL_GPIO_PIN_0
#define LED_OK_GPIO_PORT                        GPIOB
#define LED_OK_GPIO_CLK_ENABLE()                LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)

/* Definition LED3*/
#define LED_ERROR_PIN                           LL_GPIO_PIN_14
#define LED_ERROR_GPIO_PORT                     GPIOB
#define LED_ERROR_GPIO_CLK_ENABLE()             LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)

/*SPIx_Master*/

#define SPIx_MASTER_SCK_PIN                  LL_GPIO_PIN_3
#define SPIx_MASTER_SCK_GPIO_PORT            GPIOB
#define SPIx_MASTER_SCK_AF                   LL_GPIO_AF_5

#define SPIx_MASTER_MISO_PIN                 LL_GPIO_PIN_4
#define SPIx_MASTER_MISO_GPIO_PORT           GPIOB
#define SPIx_MASTER_MISO_AF                  LL_GPIO_AF_5

#define SPIx_MASTER_MOSI_PIN                 LL_GPIO_PIN_5
#define SPIx_MASTER_MOSI_GPIO_PORT           GPIOB
#define SPIx_MASTER_MOSI_AF                  LL_GPIO_AF_5

/*SPIx_Slave*/

#define SPIx_SLAVE_SCK_PIN                  LL_GPIO_PIN_5
#define SPIx_SLAVE_SCK_GPIO_PORT            GPIOA
#define SPIx_SLAVE_SCK_AF                   LL_GPIO_AF_8

#define SPIx_SLAVE_MISO_PIN                 LL_GPIO_PIN_6
#define SPIx_SLAVE_MISO_GPIO_PORT           GPIOA
#define SPIx_SLAVE_MISO_AF                  LL_GPIO_AF_8

#define SPIx_SLAVE_MOSI_PIN                 LL_GPIO_PIN_7
#define SPIx_SLAVE_MOSI_GPIO_PORT           GPIOA
#define SPIx_SLAVE_MOSI_AF                  LL_GPIO_AF_8


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SPI1_Rx_Callback(void);
void SPI1_Tx_Callback(void);
void SPI1_EOT_Callback(void);
void SPI_TransferError_Callback(void);

void SPI6_Rx_Callback(void);
void SPI6_Tx_Callback(void);
void SPI6_EOT_Callback(void);

#endif /* __MAIN_H */

