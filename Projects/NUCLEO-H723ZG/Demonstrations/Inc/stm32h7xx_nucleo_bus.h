/**
  ******************************************************************************
  * @file    BSP/Inc/stm32h7xx_nucleo_bus.h
  * @author  MCD Application Team
  * @brief   This file is the header of stm32h7xx_nucleo_bus.c
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
#ifndef STM32H7XX_NUCLEO_BUS_H
#define STM32H7XX_NUCLEO_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_nucleo_conf.h"
#include "stm32h7xx_nucleo_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32H7XX_NUCLEO STM32H7XX_NUCLEO
  * @{
  */

/** @addtogroup STM32H7XX_NUCLEO_BUS BUS
  * @{
  */

/**
  * @brief  SPI Interface pins
  *         used to communicate with LCD and SD available on Adafruit 1.8" TFT shield
  */
#define BUS_SPI1_INSTANCE                            SPI1
#define BUS_SPI1_CLK_ENABLE()                        __HAL_RCC_SPI1_CLK_ENABLE()

#define BUS_SPI1_SCK_AF                              GPIO_AF5_SPI1
#define BUS_SPI1_SCK_GPIO_PORT                       GPIOA
#define BUS_SPI1_SCK_PIN                             GPIO_PIN_5
#define BUS_SPI1_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE()

#define BUS_SPI1_MISO_AF                             GPIO_AF5_SPI1
#define BUS_SPI1_MISO_GPIO_PORT                      GPIOA
#define BUS_SPI1_MISO_PIN                            GPIO_PIN_6
#define BUS_SPI1_MISO_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MISO_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOA_CLK_DISABLE()

#define BUS_SPI1_MOSI_AF                             GPIO_AF5_SPI1
#define BUS_SPI1_MOSI_GPIO_PORT                      GPIOB
#define BUS_SPI1_MOSI_PIN                            GPIO_PIN_5
#define BUS_SPI1_MOSI_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI1_MOSI_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOB_CLK_DISABLE()


/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define BUS_SPI1_TIMEOUT_MAX                        1000U
#ifndef BUS_SPI1_BAUDRATE
 #define BUS_SPI1_BAUDRATE                              12500000 /* baud rate of SPI1 = 12.5 Mbps*/
#endif

/** @defgroup STM32H7XX_NUCLEO_BUS_Exported_Types BUS Exported Types
  * @{
  */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pSPI_CallbackTypeDef  pMspSpiInitCb;
  pSPI_CallbackTypeDef  pMspSpiDeInitCb;
}BSP_SPI1_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */


/** @defgroup STM32H7XX_NUCLEO_BUS_Exported_Functions BUS Exported Functions
  * @{
  */
int32_t BSP_SPI1_Init(void);
int32_t BSP_SPI1_DeInit(void);
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI1_RegisterMspCallbacks(uint32_t Instance, BSP_SPI1_Cb_t *CallBacks);
int32_t BSP_SPI1_RegisterDefaultMspCallbacks(uint32_t Instance);
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */
int32_t BSP_SPI1_Send(uint8_t *pTxData, uint32_t Length);
int32_t BSP_SPI1_Recv(uint8_t *pRxData, uint32_t Length);
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint32_t Length);
HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef *phspi, uint32_t baudrate_presc);
int32_t BSP_GetTick(void);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H7XX_NUCLEO_BUS_H */

