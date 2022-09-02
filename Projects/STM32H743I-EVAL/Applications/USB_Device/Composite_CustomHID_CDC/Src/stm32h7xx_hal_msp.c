/**
  ******************************************************************************
  * @file    stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Applications
  * @{
  */

/** @defgroup Composite_CustomHID_CDC
  * @{
  */

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
* @brief  ADC MSP Init
* @param  hadc : ADC handle
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef * hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  static DMA_HandleTypeDef DmaHandle;

  /* ##-1- Enable peripherals and GPIO Clocks #################################
   */
  /* Enable GPIO clock *************************************** */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* ADC Periph clock enable */
  ADCx_CLK_ENABLE();
  /* ADC Periph interface clock configuration */
  __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
  /* Enable DMA clock */
  DMAx_CHANNELx_CLK_ENABLE();

  /* ##- 2- Configure peripheral GPIO #########################################
   */
  /* ADC Channel GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
  /* ##- 3- Configure DMA #####################################################
   */

  /*********************** Configure DMA parameters ***************************/
  DmaHandle.Instance = DMA1_Stream1;
  DmaHandle.Init.Request = DMA_REQUEST_ADC1;
  DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
  DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
  DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DmaHandle.Init.Mode = DMA_CIRCULAR;
  DmaHandle.Init.Priority = DMA_PRIORITY_MEDIUM;

  HAL_DMA_Init(&DmaHandle);

  /* Associate the DMA handle */
  __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);

  /* NVIC configuration for DMA Input data interrupt */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
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
void HAL_ADC_MspDeInit(ADC_HandleTypeDef * hadc)
{

  /* ##-1- Reset peripherals ##################################################
   */
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();
  /* ADC Periph clock disable (automatically reset all ADC instances of the ADC
   * common group) */
  __HAL_RCC_ADC12_CLK_DISABLE();

  /* ##-2- Disable peripherals and GPIO Clocks ################################
   */
  /* De-initialize the ADC Channel GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
}
/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this application:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef * huart)
{
  static DMA_HandleTypeDef hdma_tx;
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /* ##-1- Enable peripherals and GPIO Clocks #################################
   */
  /* Enable GPIO clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /* Enable DMAx clock */
  DMAx_CLK_ENABLE();

  /* ##-2- Configure peripheral GPIO ##########################################
   */
  /* UART TX GPIO pin configuration */
  GPIO_InitStruct.Pin = USARTx_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /* ##-4- Configure the DMA streams ##########################################
   */
  /* Configure the DMA handler for Transmission process */
  hdma_tx.Instance = USARTx_TX_DMA_STREAM;
  hdma_tx.Init.Request = USARTx_TX_DMA_CHANNEL;
  hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode = DMA_NORMAL;
  hdma_tx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_tx.Init.MemBurst = DMA_MBURST_INC4;
  hdma_tx.Init.PeriphBurst = DMA_PBURST_INC4;


  HAL_DMA_Init(&hdma_tx);

  /* Associate the initialized DMA handle to the UART handle */
  __HAL_LINKDMA(huart, hdmatx, hdma_tx);

  /* ##-5- Configure the NVIC for DMA #########################################
   */
  /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);

  /* ##-6- Configure the NVIC for UART ########################################
   */
  HAL_NVIC_SetPriority(USARTx_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);


  /* ##-7- Enable TIM peripherals Clock #######################################
   */
  TIMx_CLK_ENABLE();

  /* ##-8- Configure the NVIC for TIMx ########################################
   */
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(TIMx_IRQn, 6, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this application:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef * huart)
{
  /* ##-1- Reset peripherals ##################################################
   */
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /* ##-2- Disable peripherals and GPIO Clocks ################################
   */
  /* Configure UART Tx as alternate function */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

  /* ##-3- Disable the NVIC for UART ##########################################
   */
  HAL_NVIC_DisableIRQ(USARTx_IRQn);

  /* ##-4- Reset TIM peripheral ###############################################
   */
  TIMx_FORCE_RESET();
  TIMx_RELEASE_RESET();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
