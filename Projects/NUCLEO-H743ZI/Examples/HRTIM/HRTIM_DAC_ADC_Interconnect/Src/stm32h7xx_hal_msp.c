/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_DAC_ADC_Interconnect/Src/stm32h7xx_hal_msp.c
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
static void HRTIM_CHANNEL_GPIO_PORT(void);

/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */


/**
  * @brief HRTIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_HRTIM_MspInit(HRTIM_HandleTypeDef * hhrtim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* HRTIM1 Peripheral clock enable */
  HRTIM_CLK_ENABLE();

  /* Enable GPIO Channels Clock */
  HRTIM_CHANNEL_GPIO_PORT();

  /* Configure PC.06 (HRTIM1_TIMA_TA1) in Pull-up, alternate function mode */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Alternate = HRTIM_TIMA_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = HRTIM_TIMA_CHANNEL_PIN1;
  HAL_GPIO_Init(HRTIM_TIMA_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);
}

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
 
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC Periph clock enable */
  __HAL_RCC_ADC12_CLK_ENABLE();
  
  /* ADC Periph interface clock configuration */
  __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
  
  HAL_NVIC_SetPriority(ADC_IRQn, 0x0F, 0); 
  NVIC_EnableIRQ(ADC_IRQn);
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
  __HAL_RCC_ADC12_FORCE_RESET();
  __HAL_RCC_ADC12_RELEASE_RESET();
	
  /* ADC Periph clock disable
   (automatically reset all ADC's) */
  __HAL_RCC_ADC12_CLK_DISABLE();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC Channel GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);   
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
  /* DAC Periph clock enable */
  __HAL_RCC_DAC12_CLK_ENABLE();
}

/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /* Enable DAC reset state */
  __HAL_RCC_DAC12_FORCE_RESET();

  /* Release DAC from reset state */
  __HAL_RCC_DAC12_RELEASE_RESET();
}
/**
  * @brief  HRTIM GPIO pins clock enable
  * @param  None
  * @retval None
  */
static void HRTIM_CHANNEL_GPIO_PORT(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
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

