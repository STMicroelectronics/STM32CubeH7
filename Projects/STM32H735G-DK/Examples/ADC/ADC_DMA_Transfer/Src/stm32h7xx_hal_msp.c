/**
  ******************************************************************************
  * @file    ADC/ADC_DMA_Transfer/Src/stm32h7xx_hal_msp.c
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

/** @defgroup ADC_DMA_Transfer
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
* @brief  ADC MSP Init
* @param  hadc : ADC handle
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  static DMA_HandleTypeDef         DmaHandle;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  ADCx_CHANNEL_GPIO_CLK_ENABLE();
  /* ADC Periph clock enable */
  ADCx_CLK_ENABLE();  
  /* ADC Periph interface clock configuration */
  __HAL_RCC_CLKP_CONFIG(RCC_CLKPSOURCE_HSE);
  __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
  /* Enable DMA clock */
  DMAx_CHANNELx_CLK_ENABLE();
  
  /*##- 2- Configure peripheral GPIO #########################################*/
  /* ADC Channel GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
  /*##- 3- Configure DMA #####################################################*/ 

  /*********************** Configure DMA parameters ***************************/
  DmaHandle.Instance                 = DMA1_Stream1;
  DmaHandle.Init.Request             = DMA_REQUEST_ADC3;
  DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  DmaHandle.Init.Mode                = DMA_CIRCULAR;
  DmaHandle.Init.Priority            = DMA_PRIORITY_MEDIUM;
  /* Deinitialize  & Initialize the DMA for new transfer */
  HAL_DMA_DeInit(&DmaHandle);
  HAL_DMA_Init(&DmaHandle);
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);

  /* NVIC configuration for DMA Input data interrupt */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);  
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
  /* ADC Periph clock disable
   (automatically reset all ADC instances of the ADC common group) */
  __HAL_RCC_ADC12_CLK_DISABLE();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC Channel GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
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

