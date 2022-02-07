/**
  ******************************************************************************
  * @file    ADC/ADC_InternalChannelConversion/Src/stm32h7xx_hal_msp.c
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

/** @defgroup ADC_InternalChannelConversion
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
 
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC Periph clock enable */
  __HAL_RCC_ADC3_CLK_ENABLE();
  
  /* ADC Periph interface clock configuration */
  __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
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
  __HAL_RCC_ADC3_FORCE_RESET();
  __HAL_RCC_ADC3_RELEASE_RESET();
  /* ADC Periph clock disable
   (automatically reset all ADC's) */
  __HAL_RCC_ADC3_CLK_DISABLE();
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

