/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESGCM/CM7/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup CRYP_AESGCM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern CRYP_HandleTypeDef hcryp;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
