/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComDMA/CM4/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module for Cortex-M4.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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

/** @addtogroup SPI_FullDuplex_ComDMA
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  if (hspi->Instance == SPI4)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPI4_SCK_GPIO_CLK_ENABLE();
    SPI4_MISO_GPIO_CLK_ENABLE();
    SPI4_MOSI_GPIO_CLK_ENABLE();
    /* Enable SPI4 clock */
    SPI4_CLK_ENABLE();
    /* Enable DMA clock */
    DMA2_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPI4_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPI4_SCK_AF;
    HAL_GPIO_Init(SPI4_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPI4_MISO_PIN;
    GPIO_InitStruct.Alternate = SPI4_MISO_AF;
    HAL_GPIO_Init(SPI4_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPI4_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPI4_MOSI_AF;
    HAL_GPIO_Init(SPI4_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the DMA ##################################################*/
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance                 = SPI4_TX_DMA_STREAM;
    hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
    hdma_tx.Init.Request             = SPI4_TX_DMA_REQUEST;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;

    HAL_DMA_Init(&hdma_tx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(hspi, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */
    hdma_rx.Instance                 = SPI4_RX_DMA_STREAM;
    hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
    hdma_rx.Init.Request             = SPI4_RX_DMA_REQUEST;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_rx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(hspi, hdmarx, hdma_rx);

    /*##-4- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (SPI4_TX) */
    HAL_NVIC_SetPriority(SPI4_DMA_TX_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(SPI4_DMA_TX_IRQn);

    /* NVIC configuration for DMA transfer complete interrupt (SPI4_RX) */
    HAL_NVIC_SetPriority(SPI4_DMA_RX_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SPI4_DMA_RX_IRQn);

    /*##-5- Configure the NVIC for SPI #########################################*/
    /* NVIC configuration for SPI transfer complete interrupt (SPI4) */
    HAL_NVIC_SetPriority(SPI4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SPI4_IRQn);
  }
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPI4)
  {
    /*##-1- Reset peripherals ##################################################*/
    SPI4_FORCE_RESET();
    SPI4_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Deconfigure SPI SCK */
    HAL_GPIO_DeInit(SPI4_SCK_GPIO_PORT, SPI4_SCK_PIN);
    /* Deconfigure SPI MISO */
    HAL_GPIO_DeInit(SPI4_MISO_GPIO_PORT, SPI4_MISO_PIN);
    /* Deconfigure SPI MOSI */
    HAL_GPIO_DeInit(SPI4_MOSI_GPIO_PORT, SPI4_MOSI_PIN);

    /*##-3- Disable the DMA ####################################################*/
    /* De-Initialize the DMA associated to transmission process */
    HAL_DMA_DeInit(&hdma_tx);
    /* De-Initialize the DMA associated to reception process */
    HAL_DMA_DeInit(&hdma_rx);

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(SPI4_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(SPI4_DMA_RX_IRQn);

    /*##-5- Disable the NVIC for SPI ###########################################*/
    HAL_NVIC_EnableIRQ(SPI4_IRQn);
  }
}

/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspInit(void)
{*/
/*}*/

/**
  * @brief  DeInitializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspDeInit(void)
{*/
/*}*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

