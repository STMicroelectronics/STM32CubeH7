/**
  ******************************************************************************
  * @file    DAC/DAC_DualConversion/Src/stm32h7xx_hal_msp.c
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

/** @defgroup DAC_DualConversion
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
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  DACx_CHANNEL_GPIO_CLK_ENABLE();
  /* DAC Periph clock enable */
  DACx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* DAC Channel1 and Channel2 GPIO pin configuration */
  GPIO_InitStruct.Pin = (DACx_CHANNEL_PIN | DACx_CHANNEL_PIN2);
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DACx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
	
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
  DACx_FORCE_RESET();

  /* Release DAC from reset state */
  DACx_RELEASE_RESET();
}
/**
  * @brief HRTIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hhrtim: HRTIM handle pointer
  * @retval None
  */
void HAL_HRTIM_MspInit(HRTIM_HandleTypeDef *hhrtim)
{
  /* TIM6 Periph clock enable */
  __HAL_RCC_HRTIM1_CLK_ENABLE();
}

/**
  * @brief HRTIM MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hhrtim: HRTIM handle pointer
  * @retval None
  */
void HAL_HRTIM_MspDeInit(HRTIM_HandleTypeDef *hhrtim)
{

  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_HRTIM1_CLK_DISABLE();

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

