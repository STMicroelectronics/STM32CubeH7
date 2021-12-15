/**
  ******************************************************************************
  * @file    QSPI/QSPI_MemoryMappedDual/CM7/Src/stm32h7xx_hal_msp.c
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

/** @defgroup QSPI_MemoryMappedDual
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
  * @brief QSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for QSPI interrupt
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the QuadSPI memory interface clock */
  QSPI_CLK_ENABLE();
  /* Reset the QuadSPI memory interface */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();
  /* Enable GPIO clocks */
  QSPI_CS_GPIO_CLK_ENABLE();
  QSPI_CLK_GPIO_CLK_ENABLE();
  QSPI_BK1_D0_GPIO_CLK_ENABLE();
  QSPI_BK1_D1_GPIO_CLK_ENABLE();
  QSPI_BK1_D2_GPIO_CLK_ENABLE();
  QSPI_BK1_D3_GPIO_CLK_ENABLE();
  
  QSPI_BK2_D0_GPIO_CLK_ENABLE();
  QSPI_BK2_D1_GPIO_CLK_ENABLE();
  QSPI_BK2_D2_GPIO_CLK_ENABLE();
  QSPI_BK2_D3_GPIO_CLK_ENABLE();
  /* Enable DMA clock */
  QSPI_MDMA_CLK_ENABLE();   

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* QSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  /* QSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_CLK_GPIO_PORT, &GPIO_InitStruct);
  
  /* - Configure QSPI Bank1 */
  /* QSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK1_D0_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_D0_GPIO_PORT, &GPIO_InitStruct);

  /* QSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK1_D1_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_D1_GPIO_PORT, &GPIO_InitStruct);

  /* QSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK1_D2_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_D2_GPIO_PORT, &GPIO_InitStruct);

  /* QSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK1_D3_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_D3_GPIO_PORT, &GPIO_InitStruct);


  /* - Configure QSPI Bank2 */
  /* QSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK2_D0_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_D0_GPIO_PORT, &GPIO_InitStruct);

  /* QSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK2_D1_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_D1_GPIO_PORT, &GPIO_InitStruct);

  /* QSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK2_D2_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_D2_GPIO_PORT, &GPIO_InitStruct);

  /* QSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_BK2_D3_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_D3_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for QSPI #########################################*/
  /* NVIC configuration for QSPI interrupt */
  HAL_NVIC_SetPriority(QUADSPI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(QUADSPI_IRQn);

}

/**
  * @brief QSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
  /*##-1- Disable the NVIC for QSPI ###########################################*/
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

  /*##-1- Disable peripherals ################################################*/
  /* De-Configure QSPI pins */
  HAL_GPIO_DeInit(QSPI_CS_GPIO_PORT, QSPI_CS_PIN);
  HAL_GPIO_DeInit(QSPI_CLK_GPIO_PORT, QSPI_CLK_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D0_GPIO_PORT, QSPI_BK1_D0_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D1_GPIO_PORT, QSPI_BK1_D1_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D2_GPIO_PORT, QSPI_BK1_D2_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D3_GPIO_PORT, QSPI_BK1_D3_PIN);

  
  HAL_GPIO_DeInit(QSPI_BK2_D0_GPIO_PORT, QSPI_BK2_D0_PIN);
  HAL_GPIO_DeInit(QSPI_BK2_D1_GPIO_PORT, QSPI_BK2_D1_PIN);
  HAL_GPIO_DeInit(QSPI_BK2_D2_GPIO_PORT, QSPI_BK2_D2_PIN);
  HAL_GPIO_DeInit(QSPI_BK2_D3_GPIO_PORT, QSPI_BK2_D3_PIN);
  
  /*##-3- Reset peripherals ##################################################*/
  /* Reset the QuadSPI memory interface */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();

  /* Disable the QuadSPI memory interface clock */
  QSPI_CLK_DISABLE();
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

