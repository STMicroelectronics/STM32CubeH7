/**
  ******************************************************************************
  * @file    SPDIFRX/SPDIFRX_AudioPlay/CM7/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup SPDIFRX_AudioPlay
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA_HandleTypeDef   SaiOutputDmaHandle;
DMA_HandleTypeDef   SpdifrxDmaHandle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  SAI MSP Init.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  GPIO_Init;

  /* Enable SAI1 clock */
  AUDIO_SAIx_CLK_ENABLE();

  /* Configure GPIOs used for SAI1 */
  AUDIO_SAIx_MCLK_ENABLE();
  AUDIO_SAIx_SCK_ENABLE();
  AUDIO_SAIx_FS_ENABLE();
  AUDIO_SAIx_SD_ENABLE();

  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_Init.Alternate = AUDIO_SAIx_FS_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_FS_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_FS_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SCK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SCK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SCK_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SD_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SD_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SD_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_MCLK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_MCLK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_MCLK_GPIO_PORT, &GPIO_Init);

  /* Configure DMA2 used for SAI1 */
  __HAL_RCC_DMA2_CLK_ENABLE();

  SaiOutputDmaHandle.Init.Request             = DMA_REQUEST_SAI1_A;
  SaiOutputDmaHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  SaiOutputDmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  SaiOutputDmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  SaiOutputDmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  SaiOutputDmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  SaiOutputDmaHandle.Init.Mode                = DMA_CIRCULAR;
  SaiOutputDmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
  SaiOutputDmaHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  SaiOutputDmaHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  SaiOutputDmaHandle.Init.MemBurst            = DMA_MBURST_SINGLE;
  SaiOutputDmaHandle.Init.PeriphBurst         = DMA_PBURST_SINGLE;

  /* Select the DMA instance to be used for the transfer : DMA2_Stream6 */
  SaiOutputDmaHandle.Instance                 = DMA2_Stream6;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsai, hdmatx, SaiOutputDmaHandle);

  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&SaiOutputDmaHandle);

  /* Configure the DMA Stream */
  if (HAL_OK != HAL_DMA_Init(&SaiOutputDmaHandle))
  {
    Error_Handler();
  }
    
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

/**
* @brief SPDIFRX MSP Init
* @param hspdif: SPDIFRX handle
* @retval None
*/

void HAL_SPDIFRX_MspInit(SPDIFRX_HandleTypeDef *hspdif)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  
  /* Enable SPDIFRX clock */
  AUDIO_SPDIFRX_CLK_ENABLE();
  
	PeriphClkInitStruct.SpdifrxClockSelection = RCC_SPDIFRXCLKSOURCE_PLL3;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 1;
	PeriphClkInitStruct.PLL3.PLL3R = 1;
	PeriphClkInitStruct.PLL3.PLL3N = 200;
	PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
	PeriphClkInitStruct.PLL3.PLL3M = 25;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
	
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /* Configure SPDIFRX_IN pin */
  AUDIO_SPDIFRX_IN_ENABLE();
  
  GPIO_InitStructure.Pin       = AUDIO_SPDIFRX_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Alternate = AUDIO_SPDIFRX_AF; 
  HAL_GPIO_Init(AUDIO_SPDIFRX_GPIO_PORT, &GPIO_InitStructure); 
  
  /* Enable DMA1 clock */
  __HAL_RCC_DMA1_CLK_ENABLE();
 
  /* Configure the SpdifrxDmaHandle handle parameters */
  SpdifrxDmaHandle.Init.Request             = DMA_REQUEST_SPDIF_RX_DT;
  SpdifrxDmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  SpdifrxDmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  SpdifrxDmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  SpdifrxDmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  SpdifrxDmaHandle.Init.MemDataAlignment    = DMA_PDATAALIGN_WORD;
  SpdifrxDmaHandle.Init.Mode                = DMA_CIRCULAR;
  SpdifrxDmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
  SpdifrxDmaHandle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
  SpdifrxDmaHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  SpdifrxDmaHandle.Init.MemBurst            = DMA_MBURST_SINGLE;
  SpdifrxDmaHandle.Init.PeriphBurst         = DMA_MBURST_SINGLE; 
  
  SpdifrxDmaHandle.Instance = DMA1_Stream1;
  
  /* Associate the initialized hdma_rx handle to the the hspdif handle*/
  __HAL_LINKDMA(hspdif, hdmaDrRx, SpdifrxDmaHandle);
  
  /* Configure the DMA Stream */
  if (HAL_OK != HAL_DMA_Init(&SpdifrxDmaHandle))
  {
    Error_Handler();
  }	
  
  /* NVIC configuration for DMA transfer complete interrupt (SPDIFRX) */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn,   0x01, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
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

