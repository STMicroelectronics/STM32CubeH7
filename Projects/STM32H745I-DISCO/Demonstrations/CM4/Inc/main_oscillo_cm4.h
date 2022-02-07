/**
  ******************************************************************************
  * @file    Demonstration/STM32H745-Discovery_Demo/CM7/Inc/main.h
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
#include "stm32h7xx_hal.h"
#include "stm32h745i_discovery.h"
#include <stdio.h>
#include <string.h>


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ################## ADCs Tools Definetions ######################## */

#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC12_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNEL                   ADC_CHANNEL_0

/* Definition of ADCx channels pins */
#define ADCx_CHANNEL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_PORT         GPIOA
#define ADCx_CHANNEL_PIN               GPIO_PIN_0

/* Definition of ADCx DMA resources */
#define ADCx_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx_DMA                        DMA1_Stream1

#define ADCx_DMA_IRQn                   DMA1_Stream1_IRQn
#define ADCx_DMA_IRQHandler             DMA1_Stream1_IRQHandler

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                       ADC_IRQn
#define ADCx_IRQHandler                 ADC_IRQHandler


/* ################## TIM Tools Definetions ######################## */   


#define TIMx                           TIM3
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()
#define TIMx_IRQn                      TIM3_IRQn
#define TIMx_IRQHandler                TIM3_IRQHandler

/* ################## TIM Tools Definetions ######################## */   

/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF7_USART3

/* Definition for USARTx's NVIC IRQ and IRQ Handlers */
#define USARTx_IRQn                      USART3_IRQn
#define USARTx_IRQHandler                USART3_IRQHandler

/* Size of Transmission buffer */
#define TXSTARTMESSAGESIZE               (COUNTOF(aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                 (COUNTOF(aTxEndMessage) - 1)

/* Size of Reception buffer */
#define Received_Command_Size                     10

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))



     
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

