/**
  ******************************************************************************
  * @file    CORDIC/CORDIC_Sin_DMA/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_cordic_write;
extern DMA_HandleTypeDef hdma_cordic_read;
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/


/**
* @brief CORDIC MSP Initialization
* This function configures the hardware resources used in this example
* @param hcordic: CORDIC handle pointer
* @retval None
*/
void HAL_CORDIC_MspInit(CORDIC_HandleTypeDef* hcordic)
{
  if(hcordic->Instance==CORDIC)
  {
    /* Peripheral clock enable */
    __HAL_RCC_CORDIC_CLK_ENABLE();
  
    /* CORDIC DMA Init */
    /* CORDIC_WRITE Init */
    hdma_cordic_write.Instance = DMA1_Stream0;
    hdma_cordic_write.Init.Request = DMA_REQUEST_CORDIC_WRITE;
    hdma_cordic_write.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_cordic_write.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_cordic_write.Init.MemInc = DMA_MINC_ENABLE;
    hdma_cordic_write.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_cordic_write.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_cordic_write.Init.Mode = DMA_NORMAL;
    hdma_cordic_write.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_cordic_write) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hcordic,hdmaIn,hdma_cordic_write);

    /* CORDIC_READ Init */
    hdma_cordic_read.Instance = DMA1_Stream1;
    hdma_cordic_read.Init.Request = DMA_REQUEST_CORDIC_READ;
    hdma_cordic_read.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_cordic_read.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_cordic_read.Init.MemInc = DMA_MINC_ENABLE;
    hdma_cordic_read.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_cordic_read.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_cordic_read.Init.Mode = DMA_NORMAL;
    hdma_cordic_read.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_cordic_read) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hcordic,hdmaOut,hdma_cordic_read);

  }

}

/**
* @brief CORDIC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcordic: CORDIC handle pointer
* @retval None
*/
void HAL_CORDIC_MspDeInit(CORDIC_HandleTypeDef* hcordic)
{
  if(hcordic->Instance==CORDIC)
  {
  /* Reset CORDIC peripheral */
  __HAL_RCC_CORDIC_FORCE_RESET();
  __HAL_RCC_CORDIC_RELEASE_RESET();

    /* Peripheral clock disable */
    __HAL_RCC_CORDIC_CLK_DISABLE();

    /* CORDIC DMA DeInit */
    HAL_DMA_DeInit(hcordic->hdmaIn);
    HAL_DMA_DeInit(hcordic->hdmaOut);
  }

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
