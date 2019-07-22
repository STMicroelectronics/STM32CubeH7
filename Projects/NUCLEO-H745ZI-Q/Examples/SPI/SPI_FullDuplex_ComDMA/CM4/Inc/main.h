/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComDMA/CM4/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M4.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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
#include "stm32h7xx_nucleo_144.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor SPIx instance used and associated
   resources */
/* Definition for SPIx clock resources */
/* define SPI4 */
#define SPI4_CLK_ENABLE()                __HAL_RCC_SPI4_CLK_ENABLE()
#define DMA2_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI4_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOE_CLK_ENABLE()

#define SPI4_FORCE_RESET()               __HAL_RCC_SPI4_FORCE_RESET()
#define SPI4_RELEASE_RESET()             __HAL_RCC_SPI4_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPI4_SCK_PIN                     GPIO_PIN_12
#define SPI4_SCK_GPIO_PORT               GPIOE
#define SPI4_SCK_AF                      GPIO_AF5_SPI4
#define SPI4_MISO_PIN                    GPIO_PIN_13
#define SPI4_MISO_GPIO_PORT              GPIOE
#define SPI4_MISO_AF                     GPIO_AF5_SPI4
#define SPI4_MOSI_PIN                    GPIO_PIN_14
#define SPI4_MOSI_GPIO_PORT              GPIOE
#define SPI4_MOSI_AF                     GPIO_AF5_SPI4

/* Definition for SPIx's DMA */
#define SPI4_TX_DMA_STREAM               DMA2_Stream3
#define SPI4_RX_DMA_STREAM               DMA2_Stream2

#define SPI4_TX_DMA_REQUEST              DMA_REQUEST_SPI4_TX
#define SPI4_RX_DMA_REQUEST              DMA_REQUEST_SPI4_RX

/* Definition for SPIx's NVIC */
#define SPI4_DMA_TX_IRQn                 DMA2_Stream3_IRQn
#define SPI4_DMA_RX_IRQn                 DMA2_Stream2_IRQn

#define SPI4_DMA_TX_IRQHandler           DMA2_Stream3_IRQHandler
#define SPI4_DMA_RX_IRQHandler           DMA2_Stream2_IRQHandler

#define SPI4_IRQn                        SPI4_IRQn
#define SPI4_IRQHandler                  SPI4_IRQHandler

/* Size of buffer */
#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
