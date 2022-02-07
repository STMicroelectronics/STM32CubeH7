/**
  ******************************************************************************
  * @file    Demonstration/Oscilloscope_SignalsGenerator/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "sig_gen.h"
#include "oscillo.h"
/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
extern     DMA_HandleTypeDef             DmaHandle;
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


/*####### 1- TIM Initialization   #################################*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM6) /*dac*/
  {
    TIM_DAC_CLK_ENABLE();
  }
  if(htim->Instance == TIM3) /*adc*/
  {
    TIM_ADC_CLK_ENABLE();
  }  
}

/*####### 2- TIM De-Initialization   ##############################*/
/**
  * @brief TIM MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htim: TIM handle pointer
  * @retval None
  */

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  
  if(htim->Instance == TIM6) /*dac*/
  {
    TIM_DAC_FORCE_RESET();
    TIM_DAC_RELEASE_RESET();
  }
  if(htim->Instance == TIM3) /*adc*/
  {
    TIM_ADC_FORCE_RESET();
    TIM_ADC_RELEASE_RESET();
  } 

}


/**
  * @brief DAC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  static DMA_HandleTypeDef  hdma_dac1;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  DACx_CHANNEL_GPIO_CLK_ENABLE();
  /* DAC Periph clock enable */
  DACx_CLK_ENABLE();
  /* DMA1 clock enable */
  DMAx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* DAC Channel1 GPIO pin configuration */
  GPIO_InitStruct.Pin = DACx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DACx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##########################################*/
  /* Set the parameters to be configured for DACx_DMA_STREAM */
  hdma_dac1.Instance = DACx_DMA_INSTANCE;

  hdma_dac1.Init.Request  = DMA_REQUEST_DAC2;

  hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_dac1.Init.Mode = DMA_CIRCULAR;
  hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH;

  HAL_DMA_Init(&hdma_dac1);

  /* Associate the initialized DMA handle to the DAC handle */
  __HAL_LINKDMA(hdac, DMA_Handle2, hdma_dac1);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* Enable the DMA1_Stream5 IRQ Channel */
  HAL_NVIC_SetPriority(DACx_DMA_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DACx_DMA_IRQn);

}

/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /*##-1- Reset peripherals ##################################################*/
  DACx_FORCE_RESET();
  DACx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the DAC Channel1 GPIO pin */
  HAL_GPIO_DeInit(DACx_CHANNEL_GPIO_PORT, DACx_CHANNEL_PIN);

  /*##-3- Disable the DMA Stream ############################################*/
  /* De-Initialize the DMA Stream associate to DAC_Channel1 */
  HAL_DMA_DeInit(hdac->DMA_Handle2);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(DACx_DMA_IRQn);
}

/**
* @brief  ADC MSP Init
* @param  hadc : ADC handle
* @retval None
*/
/*####### 1- ADC Initialization   #################################*/

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable clock of GPIO associated to the peripheral channels */
  ADCx_CHANNEL_GPIO_CLK_ENABLE();
  
  /* Enable clock of ADCx peripheral */
  ADCx_CLK_ENABLE();
  
  /* ADC Periph interface clock configuration */
  __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_PLL2);
  
 /*##-2- Configure peripheral GPIO ##########################################*/ 
 
  /* Configure GPIO pins of the selected ADC channels */
    GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);  
 
}

/*####### 2- ADC De-Initialization   ##############################*/
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
  /* De-initialize GPIO pin of the selected ADC channel */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);

  /*##-3- Disable the DMA ####################################################*/
  /* De-Initialize the DMA associated to the peripheral */
  if(hadc->DMA_Handle != NULL)
  {
    HAL_DMA_DeInit(hadc->DMA_Handle);
  }

  /*##-4- Disable the NVIC ###################################################*/
  /* Disable the NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);
  
  /* Disable the NVIC configuration for ADC interrupt */
  HAL_NVIC_DisableIRQ(ADCx_IRQn);
}



/*####### 5- UART Initialization   ################################*/

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART1CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /* NVIC for USARTx */
  HAL_NVIC_SetPriority(USARTx_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/*####### 6- UART De-Initialization   #################################*/

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
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

