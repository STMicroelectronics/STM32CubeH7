/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_Polling/CM7/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module for Cortex-M7.
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

/** @addtogroup JPEG_DecodingUsingFs_Polling
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
  * @brief JPEG MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - NVIC configuration for JPEG interrupt request enable
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
  /* Enable JPEG clock */
  __HAL_RCC_JPGDECEN_CLK_ENABLE();
  
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

