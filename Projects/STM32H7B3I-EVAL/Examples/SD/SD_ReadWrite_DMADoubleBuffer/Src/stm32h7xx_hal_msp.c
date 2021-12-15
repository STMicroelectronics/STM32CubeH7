/**
  ******************************************************************************
  * @file    SD/SD_ReadWrite_DMADoubleBuffer/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup SD_ReadWrite_DMADoubleBuffer
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
  * @brief SD MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral
  *           - NVIC configuration for DMA and SD interrupts
  * @param hsd: SD handle pointer
  * @retval None
  */

void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  

  /* Enable SDIO clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();

  /* Enable GPIOs clock */
  SDMMC_D0_GPIO_CLK_ENABLE();
  SDMMC_D1_GPIO_CLK_ENABLE();
  SDMMC_D2_GPIO_CLK_ENABLE();
  SDMMC_D3_GPIO_CLK_ENABLE();
  SDMMC_CK_GPIO_CLK_ENABLE();
  SDMMC_CMD_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  /* SDMMC D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SDMMC_D0_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(SDMMC_D0_GPIO_PORT, &GPIO_InitStruct);

  /* SDMMC D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SDMMC_D1_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(SDMMC_D1_GPIO_PORT, &GPIO_InitStruct);

  /* SDMMC D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SDMMC_D2_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(SDMMC_D2_GPIO_PORT, &GPIO_InitStruct);

  /* SDMMC D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SDMMC_D3_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(SDMMC_D3_GPIO_PORT, &GPIO_InitStruct);

  /* SDMMC CK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SDMMC_CK_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(SDMMC_CK_GPIO_PORT, &GPIO_InitStruct);

  /* SDMMC CMD GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SDMMC_CMD_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(SDMMC_CMD_GPIO_PORT, &GPIO_InitStruct);


  /*  SDMMC Kernel Clock: (((24/24) * 400) / 2) = 200Mhz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
  RCC_PeriphCLKInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2RGE = RCC_PLL1VCIRANGE_0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2M = 24;
  RCC_PeriphCLKInitStruct.PLL2.PLL2N = 400;
  RCC_PeriphCLKInitStruct.PLL2.PLL2FRACN = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2P = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2R = 2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2Q = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  __HAL_RCC_SDMMC1_FORCE_RESET();
  __HAL_RCC_SDMMC1_RELEASE_RESET();

  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SDMMC1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDMMC1_IRQn);

}

/**
  * @brief SD MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hsd: SD handle pointer
  * @retval None
  */
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{

  /* Disable NVIC for SDIO interrupts */
  HAL_NVIC_DisableIRQ(SDMMC1_IRQn);

  /* DeInit GPIO pins can be done in the application
  (by surcharging this __weak function) */

  /* Disable GPIOs clock */
  SDMMC_D0_GPIO_CLK_DISABLE();
  SDMMC_D1_GPIO_CLK_DISABLE();
  SDMMC_D2_GPIO_CLK_DISABLE();
  SDMMC_D3_GPIO_CLK_DISABLE();
  SDMMC_CK_GPIO_CLK_DISABLE();
  SDMMC_CMD_GPIO_CLK_DISABLE();

  /* Disable SDMMC1 clock */
  __HAL_RCC_SDMMC1_CLK_DISABLE();
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
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16910CLKSOURCE_CDPCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
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

