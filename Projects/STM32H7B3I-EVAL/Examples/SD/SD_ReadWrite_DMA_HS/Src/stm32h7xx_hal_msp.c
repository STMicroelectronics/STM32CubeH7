/**
  ******************************************************************************
  * @file    SD/SD_ReadWrite_DMA_HS/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup SD_ReadWrite_DMA_HS
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
  BSP_IO_Init_t bsp_init;
  /* Enable SDIO clock */
  __HAL_RCC_SDMMC2_CLK_ENABLE();


  __HAL_RCC_SDMMC2_FORCE_RESET();
  __HAL_RCC_SDMMC2_RELEASE_RESET();
  /*  SDMMC Clk: (((24/24) * 360) / 2) = 180Mhz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
  RCC_PeriphCLKInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2M = 24;
  RCC_PeriphCLKInitStruct.PLL2.PLL2N = 360;
  RCC_PeriphCLKInitStruct.PLL2.PLL2FRACN = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2P = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2R = 2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2Q = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF11_SDMMC2;
  /* GPIOC configuration, D0, D1*/
  gpio_init_structure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOC configuration,  D2, D3*/
   gpio_init_structure.Alternate = GPIO_AF10_SDMMC2;
  gpio_init_structure.Pin =  GPIO_PIN_11 | GPIO_PIN_12;

  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOD configuration, CK ,CMD line */
  gpio_init_structure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  gpio_init_structure.Alternate = GPIO_AF11_SDMMC2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* IO Expander Init */
  bsp_init.Pin = IO_PIN_0;
  bsp_init.Pull=IO_PULLUP;
  bsp_init.Mode = IO_MODE_OUTPUT_PP;
  BSP_IO_Init(0,&bsp_init);
  bsp_init.Pin = IO_PIN_8;
  BSP_IO_Init(0,&bsp_init);

  /* Activate 3V3_SW and 1V8_SW */
  BSP_IO_WritePin(0, IO_PIN_0, IO_PIN_RESET);  /* 3.3V sw : Off */
  BSP_IO_WritePin(0, IO_PIN_8, IO_PIN_RESET);  /* 1.8V sw : Off */
  HAL_Delay(200);
  BSP_IO_WritePin(0, IO_PIN_0, IO_PIN_SET);    /* 3.3V sw : On */
  BSP_IO_WritePin(0, IO_PIN_8, IO_PIN_SET);    /* 1.8V sw : On */
  /* SD_LDO_SEL ; 3.3V/1.8V switch */
  gpio_init_structure.Mode    = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Speed   = GPIO_SPEED_FREQ_LOW;
  gpio_init_structure.Pin     = SD_LDO_SEL_PIN;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SDMMC2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDMMC2_IRQn);

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

