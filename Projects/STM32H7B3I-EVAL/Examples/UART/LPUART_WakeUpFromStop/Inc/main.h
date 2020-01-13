/**
  ******************************************************************************
  * @file    UART/LPUART_WakeUpFromStop/Inc/main.h
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
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor LPUART instance used and associated
   resources */
/* Definition for LPUART clock resources */
#define LPUARTx                           LPUART1
#define LPUARTx_CLK_ENABLE()              __HAL_RCC_LPUART1_CLK_ENABLE();
#define LPUARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define LPUARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define LPUARTx_FORCE_RESET()             __HAL_RCC_LPUART1_FORCE_RESET()
#define LPUARTx_RELEASE_RESET()           __HAL_RCC_LPUART1_RELEASE_RESET()

/* Definition for LPUART Pins */
#define LPUARTx_TX_PIN                    GPIO_PIN_9
#define LPUARTx_TX_GPIO_PORT              GPIOA
#define LPUARTx_TX_AF                     GPIO_AF3_LPUART
#define LPUARTx_RX_PIN                    GPIO_PIN_10
#define LPUARTx_RX_GPIO_PORT              GPIOA
#define LPUARTx_RX_AF                     GPIO_AF3_LPUART

/* Definition for LPUART's NVIC */
#define LPUARTx_IRQn                      LPUART1_IRQn
#define LPUARTx_IRQHandler                LPUART1_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported functions ------------------------------------------------------- */
void LPUARTx_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
