/**
  ******************************************************************************
  * @file    PSSI_Transmit_Receive_DMA/Inc/main.h 
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
#include "stm32h7xx_nucleo.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* PSSI TX */
#define PSSI_IRQHandler            DCMI_IRQHandler
#define PSSI_DMA_TX_IRQHandler        DMA2_Stream3_IRQHandler
#define PSSI_DMA_TX_CLK_ENABLE         __HAL_RCC_DMA2_CLK_ENABLE
#define PSSI_DMA_TX_STREAM             DMA2_Stream3
#define PSSI_DMA_TX_IRQ                DMA2_Stream3_IRQn

/* PSSI RX */
#define PSSI_DMA_RX_IRQHandler        DMA2_Stream4_IRQHandler
#define PSSI_DMA_RX_CLK_ENABLE         __HAL_RCC_DMA2_CLK_ENABLE
#define PSSI_DMA_RX_STREAM             DMA2_Stream4
#define PSSI_DMA_RX_IRQ                DMA2_Stream4_IRQn  

/* Definition for TIMx clock resources */
#define TIMx                           TIM1
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOE_CLK_ENABLE();
#define TIMx_GPIO_PORT_CHANNEL1        GPIOE
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_9
#define TIMx_GPIO_AF_CHANNEL1          GPIO_AF1_TIM1


#endif /* __MAIN_H */

