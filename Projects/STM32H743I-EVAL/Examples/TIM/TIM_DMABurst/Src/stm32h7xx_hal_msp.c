/**
  ******************************************************************************
  * @file    TIM/TIM_DMABurst/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */


/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  static DMA_HandleTypeDef  hdma_tim;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx clock enable */
  TIMx_CLK_ENABLE();

  /* Enable GPIO Channel3/3N Clocks */
  TIMx_CHANNEL1_GPIO_CLK_ENABLE();

  /* Enable DMA clock */
  DMAx_CLK_ENABLE();

  /* Configure TIM2_Channel1 in output, push-pull & alternate function mode */
  GPIO_InitStruct.Pin = GPIO_PIN_CHANNEL1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF_TIMx;
  HAL_GPIO_Init(TIMx_GPIO_CHANNEL1_PORT, &GPIO_InitStruct);


  /* Set the parameters to be configured */
  hdma_tim.Init.Request = DMA_REQUEST_TIM2_UP;
  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD ;
  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_WORD ;
  hdma_tim.Init.Mode = DMA_NORMAL;
  hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_tim.Init.PeriphBurst = DMA_PBURST_SINGLE;
  hdma_tim.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_tim.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;

    
  /* Set hdma_tim instance */
  hdma_tim.Instance = TIMx_UPDATE_DMA_INST;

  /* Link hdma_tim to hdma[TIM_DMA_ID_UPDATE] (update) */
  __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_UPDATE], hdma_tim);
  
  /* Initialize TIMx DMA handle */
  HAL_DMA_Init(htim->hdma[TIM_DMA_ID_UPDATE]);
  
  /*##-2- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(TIMx_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIMx_DMA_IRQn);
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

