/**
  ******************************************************************************
  * @file    FMAC/FMAC_FIR_DMAToIT/Src/stm32h7xx_hal_msp.c
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

/** @defgroup FMAC_FIR_DMAToIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_fmac_preload;

extern DMA_HandleTypeDef hdma_fmac_write;
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
    /* Peripheral clock enable */
    __HAL_RCC_FMAC_CLK_ENABLE();
  
    /* FMAC DMA Init */
    /* FMAC_PRELOAD Init */
    hdma_fmac_preload.Instance = DMA1_Stream0;
    hdma_fmac_preload.Init.Request = DMA_REQUEST_MEM2MEM;
    hdma_fmac_preload.Init.Direction = DMA_MEMORY_TO_MEMORY;
    hdma_fmac_preload.Init.PeriphInc = DMA_PINC_ENABLE;
    hdma_fmac_preload.Init.MemInc = DMA_MINC_DISABLE;
    hdma_fmac_preload.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_fmac_preload.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_fmac_preload.Init.Mode = DMA_NORMAL;
    hdma_fmac_preload.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_fmac_preload) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hfmac,hdmaPreload,hdma_fmac_preload);

    /* FMAC_WRITE Init */
    hdma_fmac_write.Instance = DMA1_Stream1;
    hdma_fmac_write.Init.Request = DMA_REQUEST_FMAC_WRITE;
    hdma_fmac_write.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_fmac_write.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_fmac_write.Init.MemInc = DMA_MINC_ENABLE;
    hdma_fmac_write.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_fmac_write.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_fmac_write.Init.Mode = DMA_NORMAL;
    hdma_fmac_write.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_fmac_write) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hfmac,hdmaIn,hdma_fmac_write);

    /* FMAC interrupt Init */
    HAL_NVIC_SetPriority(FMAC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FMAC_IRQn);
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
    HAL_DMA_DeInit(hfmac->hdmaPreload);
    HAL_DMA_DeInit(hfmac->hdmaIn);

    /* FMAC interrupt DeInit */
    HAL_NVIC_DisableIRQ(FMAC_IRQn);
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

