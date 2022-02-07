/**
  ******************************************************************************
  * @file    USART/USART_SlaveMode/Src/stm32h7xx_hal_msp.c
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

/** @defgroup USART_SlaveMode
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
  * @brief USART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param husart: USART handle pointer
  * @retval None
  */
void HAL_USART_MspInit(USART_HandleTypeDef *husart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  if(husart->Instance == USARTx)
  {
    /*##-1- Enable peripherals and GPIO Clocks ###############################*/
    /* Enable GPIO TX/RX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();
    
    /* Enable USARTx clock */
    USARTx_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ########################################*/  
    /* USART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;
    
    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* USART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
    
    /* USART CK GPIO pin configuration  */
    GPIO_InitStruct.Pin = USARTx_CK_PIN;
    GPIO_InitStruct.Alternate = USARTx_CK_AF;
    
    HAL_GPIO_Init(USARTx_CK_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * @brief USART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param husart: USART handle pointer
  * @retval None
  */
void HAL_USART_MspDeInit(USART_HandleTypeDef *husart)
{
  if(husart->Instance == USARTx)
  {
    /*##-1- Reset peripherals ################################################*/
    USARTx_FORCE_RESET();
    USARTx_RELEASE_RESET();
    
    /*##-2- Disable peripherals and GPIO Clocks ##############################*/
    /* Configure USARTx Tx as alternate function  */
    HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
    /* Configure USARTx Rx as alternate function  */
    HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
    /* Configure USARTx Ck as alternate function  */
    HAL_GPIO_DeInit(USARTx_CK_GPIO_PORT, USARTx_CK_PIN);
  }
}

/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  if(hspi->Instance == SPIx)
  {     
    /*##-1- Enable peripherals and GPIO Clocks ###############################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    SPIx_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ########################################*/  
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
    
    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
    
    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * @brief SPI MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPIx)
  {   
    /*##-1- Reset peripherals ################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();
    
    /*##-2- Disable peripherals and GPIO Clocks ##############################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
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

