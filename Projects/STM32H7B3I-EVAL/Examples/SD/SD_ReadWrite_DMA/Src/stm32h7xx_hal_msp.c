/**
  ******************************************************************************
  * @file    SD/SD_ReadWrite_DMA/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/** @addtogroup SD_ReadWrite_DMA
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
  GPIO_InitTypeDef gpio_init_structure;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;  
  /* Enable SDIO clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  /* D0(PC8), D1(PC9), D2(PC10), D3(PC11), CK(PC12), CMD(PD2) */
  /* Common GPIO configuration */
  gpio_init_structure.Alternate = GPIO_AF12_SDMMC1;
  
  /* GPIOC configuration */
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /*  SDMMC Clk: (((24/24) * 400) / 2) = 50Mhz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
  RCC_PeriphCLKInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2RGE = RCC_PLL1VCIRANGE_2;
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
  
  /* DeInit GPIO pins can be done in the application 
  (by surcharging this __weak function) */
  
    /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  
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
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
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

