/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module (CM7)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h745i_discovery.h"
#include "ff_gen_drv.h"
#include "mmc_diskio.h"


/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
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

/* Exported macro ------------------------------------------------------------*/
#define LOCK_HSEM(__sem__)   while(HAL_HSEM_FastTake(__sem__) != HAL_OK) {}
#define UNLOCK_HSEM(__sem__)  HAL_HSEM_Release(__sem__, 0)


/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
