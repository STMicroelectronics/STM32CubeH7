/**
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_FullDuplex_ComIT/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
/* LL drivers common to all LL examples */
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_utils.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/**
  * @brief LED1 
  */
#define LED1_PIN                           LL_GPIO_PIN_0
#define LED1_GPIO_PORT                     GPIOB
#define LED1_GPIO_CLK_ENABLE()             LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)

/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

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


/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         LL_GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                   GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC)   
#define USER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_13
#define USER_BUTTON_EXTI_IRQn                   EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)   
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE)   
#define USER_BUTTON_SYSCFG_SET_EXTI()           do {                                                                     \
                                                  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);                  \
                                                  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);  \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER                  EXTI15_10_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SPI1_Rx_Callback(void);
void SPI1_Tx_Callback(void);
void SPI1_EOT_Callback(void);
void SPI_TransferError_Callback(void);

void SPI6_Rx_Callback(void);
void SPI6_Tx_Callback(void);
void SPI6_EOT_Callback(void);
void UserButton_Callback(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
