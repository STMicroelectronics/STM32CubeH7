/**
  ******************************************************************************
  * @file    CRYP/CRYP_TDESmodes/Src/stm32h7xx_hal_msp.c
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
  * @brief CRYP MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripherals clock enable
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
  /* Enable CRYP clock */
  __HAL_RCC_CRYP_CLK_ENABLE();
  
  /* Force the CRYP Peripheral Clock Reset */  
  __HAL_RCC_CRYP_FORCE_RESET(); 
  
  /* Release the CRYP Peripheral Clock Reset */  
  __HAL_RCC_CRYP_RELEASE_RESET();
}

/**
  * @brief CRYP MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripherals clock
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Disable CRYP clock */
  __HAL_RCC_CRYP_CLK_DISABLE();
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

