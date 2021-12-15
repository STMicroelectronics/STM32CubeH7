/**
  ******************************************************************************
  * @file    OTFDEC_EncryptionDecryption/Src/stm32h7xx_hal_msp.c
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
  * @brief OSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral
  *           - NVIC configuration for DMA and OSPI interrupts
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the OctoSPI memory interface clocks */
  OSPI_CLK_ENABLE();
  /* Reset the OctoSPI memory interface */
  OSPI_FORCE_RESET();
  OSPI_RELEASE_RESET();

  OSPI_CS_GPIO_CLK_ENABLE();
  OSPI_CLK_GPIO_CLK_ENABLE();
  OSPI_D0_GPIO_CLK_ENABLE();
  OSPI_D1_GPIO_CLK_ENABLE();
  OSPI_D2_GPIO_CLK_ENABLE();
  OSPI_D3_GPIO_CLK_ENABLE();
  OSPI_D4_GPIO_CLK_ENABLE();
  OSPI_D5_GPIO_CLK_ENABLE();
  OSPI_D6_GPIO_CLK_ENABLE();
  OSPI_D7_GPIO_CLK_ENABLE();
  OSPI_DQS_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* OSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = OSPI_CS_AF;
  HAL_GPIO_Init(OSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Alternate = OSPI_CLK_AF ;
  HAL_GPIO_Init(OSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D0_PIN;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = OSPI_D0_AF ;
  HAL_GPIO_Init(OSPI_D0_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D1_PIN;
  GPIO_InitStruct.Alternate = OSPI_D1_AF ;
  HAL_GPIO_Init(OSPI_D1_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D2_PIN;
  GPIO_InitStruct.Alternate = OSPI_D2_AF ;
  HAL_GPIO_Init(OSPI_D2_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D3_PIN;
  GPIO_InitStruct.Alternate = OSPI_D3_AF ;
  HAL_GPIO_Init(OSPI_D3_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D4_PIN;
  GPIO_InitStruct.Alternate = OSPI_D4_AF ;
  HAL_GPIO_Init(OSPI_D4_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D5_PIN;
  GPIO_InitStruct.Alternate = OSPI_D5_AF ;
  HAL_GPIO_Init(OSPI_D5_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D6_PIN;
  GPIO_InitStruct.Alternate = OSPI_D6_AF ;
  HAL_GPIO_Init(OSPI_D6_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D7_PIN;
  GPIO_InitStruct.Alternate = OSPI_D7_AF ;
  HAL_GPIO_Init(OSPI_D7_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI DQS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_DQS_PIN;
  GPIO_InitStruct.Alternate = OSPI_DQS_AF;
  HAL_GPIO_Init(OSPI_DQS_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for OSPI #########################################*/
  /* NVIC configuration for OSPI interrupt */
  HAL_NVIC_SetPriority(OSPI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(OSPI_IRQn);
}

/**
  * @brief OSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef *hospi)
{
  /*##-1- Disable the NVIC for OSPI and DMA ##################################*/
  HAL_NVIC_DisableIRQ(OSPI_IRQn);

  /*##-2- Disable peripherals ################################################*/
  /* De-Configure OSPI pins */
  HAL_GPIO_DeInit(OSPI_CS_GPIO_PORT, OSPI_CS_PIN);
  HAL_GPIO_DeInit(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN);
  HAL_GPIO_DeInit(OSPI_D0_GPIO_PORT, OSPI_D0_PIN);
  HAL_GPIO_DeInit(OSPI_D1_GPIO_PORT, OSPI_D1_PIN);
  HAL_GPIO_DeInit(OSPI_D2_GPIO_PORT, OSPI_D2_PIN);
  HAL_GPIO_DeInit(OSPI_D3_GPIO_PORT, OSPI_D3_PIN);
  HAL_GPIO_DeInit(OSPI_D4_GPIO_PORT, OSPI_D4_PIN);
  HAL_GPIO_DeInit(OSPI_D5_GPIO_PORT, OSPI_D5_PIN);
  HAL_GPIO_DeInit(OSPI_D6_GPIO_PORT, OSPI_D6_PIN);
  HAL_GPIO_DeInit(OSPI_D7_GPIO_PORT, OSPI_D7_PIN);
  HAL_GPIO_DeInit(OSPI_DQS_GPIO_PORT, OSPI_DQS_PIN);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the OctoSPI memory interface */
  OSPI_FORCE_RESET();
  OSPI_RELEASE_RESET();

  /* Disable the OctoSPI memory interface clocks */
  OSPI_CLK_DISABLE();
}

/**
* @brief  CRYP BSP Init
* @param  hcryp : CRYP handle
* @retval None
*/
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
  /* Enable CRYP clock */
  __HAL_RCC_CRYP_CLK_ENABLE();

  /* Force the CRYP Periheral Clock Reset */
  __HAL_RCC_CRYP_FORCE_RESET();

  /* Release the CRYP Periheral Clock Reset */
  __HAL_RCC_CRYP_RELEASE_RESET();
}

/**
* @brief  CRYP BSP DeInit
* @param  hcryp : CRYP handle
* @retval None
*/
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Enable CRYP clock */
  __HAL_RCC_CRYP_CLK_DISABLE();
}

/**
* @brief  OTDF BSP Init
* @param  hotfdec_region : OTDF REGION handle
* @retval None
*/
void HAL_OTFDEC_MspInit(OTFDEC_HandleTypeDef *hotfdec_region)
{
  if ((hotfdec_region->Instance == OTFDEC1))
  {
    /* Enable OTFDEC1 clock */
    __HAL_RCC_OTFDEC1_CLK_ENABLE();

    /* Force the OTFDEC1 Periheral Reset */
    __HAL_RCC_OTFDEC1_FORCE_RESET();
    __HAL_RCC_OTFDEC1_RELEASE_RESET();

    /*Enable and set OTFDEC1 Interrupt to the highest priority*/
    HAL_NVIC_SetPriority(OTFDEC1_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(OTFDEC1_IRQn);
  }
  else
  {
    /* Enable OTFDEC2 clock */
    __HAL_RCC_OTFDEC2_CLK_ENABLE();

    /* Force the OTFDEC2 Periheral Reset */
    __HAL_RCC_OTFDEC2_FORCE_RESET();
    __HAL_RCC_OTFDEC2_RELEASE_RESET();

    /*Enable and set OTFDEC2 Interrupt to the highest priority*/
    HAL_NVIC_SetPriority(OTFDEC2_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(OTFDEC2_IRQn);
  }
}

/**
* @brief  OTDF BSP DeInit
* @param  hotfdec_region : OTDF REGION handle
* @retval None
*/
void HAL_OTFDEC_MspDeInit(OTFDEC_HandleTypeDef *hotfdec_region)
{
  if ((hotfdec_region->Instance == OTFDEC1) )
  {
    /* Enable OTFDEC1 clock */
    __HAL_RCC_OTFDEC1_CLK_DISABLE();
  }
  else
  {
    /* Enable OTFDEC2 clock */
    __HAL_RCC_OTFDEC2_CLK_DISABLE();
  }
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
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

