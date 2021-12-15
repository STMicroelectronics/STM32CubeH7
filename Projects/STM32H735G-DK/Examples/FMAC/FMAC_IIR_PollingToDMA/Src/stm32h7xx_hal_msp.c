/**
  ******************************************************************************
  * @file    FMAC/FMAC_IIR_PollingToDMA/Src/stm32h7xx_hal_msp.c
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

/** @defgroup FMAC_IIR_PollingToDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_fmac_read;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
* @brief FMAC MSP Initialization
* This function configures the hardware resources used in this example
* @param hfmac: FMAC handle pointer
* @retval None
*/
void HAL_FMAC_MspInit(FMAC_HandleTypeDef* hfmac)
{
  if(hfmac->Instance==FMAC)
  {
    __HAL_RCC_FMAC_CLK_ENABLE();
  
    /* FMAC DMA Init */
    /* FMAC_READ Init */
    hdma_fmac_read.Instance = DMA1_Stream0;
    hdma_fmac_read.Init.Request = DMA_REQUEST_FMAC_READ;
    hdma_fmac_read.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_fmac_read.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_fmac_read.Init.MemInc = DMA_MINC_ENABLE;
    hdma_fmac_read.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_fmac_read.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_fmac_read.Init.Mode = DMA_NORMAL;
    hdma_fmac_read.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_fmac_read) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hfmac,hdmaOut,hdma_fmac_read);
  }

}

/**
* @brief FMAC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hfmac: FMAC handle pointer
* @retval None
*/
void HAL_FMAC_MspDeInit(FMAC_HandleTypeDef* hfmac)
{
  if(hfmac->Instance==FMAC)
  {
    /* Peripheral clock disable */
    __HAL_RCC_FMAC_CLK_DISABLE();

    /* FMAC DMA DeInit */
    HAL_DMA_DeInit(hfmac->hdmaOut);
  }

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

