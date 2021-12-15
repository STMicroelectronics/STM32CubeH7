/**
  ******************************************************************************
  * @file    DTS/DTS_GetTemperature/Src/stm32h7xx_hal_msp.c
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
  * @brief DTS MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hdts: DTS handle pointer
  * @retval None
  */
void HAL_DTS_MspInit(DTS_HandleTypeDef *hdts)
{
  /* DTS Peripheral clock enable */
  __HAL_RCC_DTS_CLK_ENABLE();
}

/**
  * @brief DTS MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hdts: DTS handle pointer
  * @retval None
  */
void HAL_DTS_MspDeInit(DTS_HandleTypeDef *hdts)
{
  /* Enable DTS reset state */
  __HAL_RCC_DTS_FORCE_RESET();

  /* Release DTS from reset state */
  __HAL_RCC_DTS_RELEASE_RESET();
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

