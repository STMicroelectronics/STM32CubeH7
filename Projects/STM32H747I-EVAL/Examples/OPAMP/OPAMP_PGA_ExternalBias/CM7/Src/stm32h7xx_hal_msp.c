/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_PGA_ExternalBias/CM7/Src/stm32h7xx_hal_msp.c
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

/** @defgroup OPAMP_PGA_ExternalBias
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
  * @brief DAC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
 
  /*## Enable peripherals and  Clocks ####################################*/

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
  /*## Reset peripherals ##################################################*/
  __HAL_RCC_DAC12_FORCE_RESET();
  __HAL_RCC_DAC12_RELEASE_RESET();

}


/**
  * @brief OPAMP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripherals clock enable
  *           - Peripherals GPIO Configuration  
  * @param hopamp: OPAMP handle pointer
  * @retval None
  */
void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef *hopamp)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/* GPIOx Periph clock enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/* Configure GPIO on PC5 in analog mode */ 
	GPIO_InitStruct.Pin   = GPIO_PIN_5;
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/* Configure GPIO on PC4 in analog mode */
	GPIO_InitStruct.Pin   = GPIO_PIN_4;
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_ENABLE();  
}

/**
  * @brief  DeInitializes the OPAMP MSP.
  * @param  hopamp: pointer to a OPAMP_HandleTypeDef structure that contains
  *         the configuration information for the specified OPAMP.  
  * @retval None
  */
void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef *hopamp)
{
	/* De-initialize the GPIO pin */
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_5);
	
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_DISABLE();  
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

