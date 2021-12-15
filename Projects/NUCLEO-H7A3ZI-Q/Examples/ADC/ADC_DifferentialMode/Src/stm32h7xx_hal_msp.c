/**
  ******************************************************************************
  * @file    ADC/ADC_DifferentialMode/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defgroup ADC_DifferentialMode
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
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  ADCx_CHANNEL_GPIO_CLK_ENABLE();
  /* ADC1 Periph clock enable */
  ADCx_CLK_ENABLE();

  /* ADC Periph interface clock configuration */
  __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
  
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure GPIO pin of the selected ADC channel */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_DIFF_HIGH;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_DIFF_HIGH_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = ADCx_CHANNEL_DIFF_LOW;
  HAL_GPIO_Init(ADCx_CHANNEL_DIFF_LOW_GPIO_PORT, &GPIO_InitStruct);
  
  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for ADC interrupt */
  /* Priority: high-priority */
  HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADC_IRQn);
  
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{

  /*##-1- Reset peripherals ##################################################*/
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC Channel GPIO pin */
  HAL_GPIO_DeInit(GPIOB, ADCx_CHANNEL_DIFF_HIGH);
  HAL_GPIO_DeInit(GPIOB, ADCx_CHANNEL_DIFF_LOW);

  /*##-3- Disable the DMA Channels ###########################################*/
  /* De-Initialize the DMA Channel associated to transmission process */
  HAL_DMA_DeInit(hadc->DMA_Handle);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(DMA1_Stream1_IRQn);
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

