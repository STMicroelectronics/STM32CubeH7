/**
  ******************************************************************************
  * @file    FMC/FMC_SRAM/Src/stm32h7xx_hal_msp.c
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

/** @defgroup FMC_SRAM
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
  * @brief SRAM MSP Initialization
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hsram: SRAM handle pointer
  * @retval None
  */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
  static MDMA_HandleTypeDef mdma_handle;
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();
  
  /* Enable chosen DMAx clock */
  __HAL_RCC_MDMA_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  
  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |\
                              GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |\
                              GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3| GPIO_PIN_4 | GPIO_PIN_7     |\
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |\
                              GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |\
                              GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);  

  /* GPIOH configuration */
  gpio_init_structure.Pin   = /*GPIO_PIN_3 | GPIO_PIN_6 |*/ GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |\
                              GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
   
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);
	
  /* GPIOI configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | 
	                            GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                              GPIO_PIN_9 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure); 
	
  /* GPIOB configuration */
  gpio_init_structure.Pin   = GPIO_PIN_7;   
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  
  /* Configure common MDMA parameters */
  mdma_handle.Init.Request = MDMA_REQUEST_SW;
  mdma_handle.Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
  mdma_handle.Init.Priority = MDMA_PRIORITY_HIGH;
  mdma_handle.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  mdma_handle.Init.SourceInc = MDMA_SRC_INC_WORD;
  mdma_handle.Init.DestinationInc = MDMA_DEST_INC_WORD;
  mdma_handle.Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
  mdma_handle.Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
  mdma_handle.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;                            
  mdma_handle.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
  mdma_handle.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
  mdma_handle.Init.BufferTransferLength = 128;
  mdma_handle.Init.SourceBlockAddressOffset = 0;
  mdma_handle.Init.DestBlockAddressOffset = 0;  
  
  mdma_handle.Instance = MDMA_Channel1;
  
   /* Associate the DMA handle */
  __HAL_LINKDMA(hsram, hmdma, mdma_handle);
  
  /* Deinitialize the Stream for new transfer */
  HAL_MDMA_DeInit(&mdma_handle);
  
  /* Configure the DMA Stream */
  HAL_MDMA_Init(&mdma_handle);
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);   
}


/**
  * @brief SRAM MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param hsram: SRAM handle pointer
  * @retval None
  */
void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef  *hsram)
{  
  /*## Disable peripherals and GPIO Clocks ###################################*/
    static MDMA_HandleTypeDef mdma_handle;
  
    /* Disable NVIC configuration for DMA interrupt */
    HAL_NVIC_DisableIRQ(MDMA_IRQn);

    /* Deinitialize the stream for new transfer */
    mdma_handle.Instance = MDMA_Channel1;
    HAL_MDMA_DeInit(&mdma_handle);

    /* GPIO pins clock, FMC clock and DMA clock can be shut down in the applications
       by surcharging this __weak function */ 
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

