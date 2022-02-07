/**
  ******************************************************************************
  * @file    OTFDEC/OTFDEC_ExecutingCryptedInstruction/stm32h7xx_hal_msp.c
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

  if(hospi->Instance == OCTOSPI1)
  {
    /*Enable peripherals and GPIO Clocks*/

    /* Enable the OctoSPI memory interface clocks */
    __HAL_RCC_OSPI1_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Configure peripheral GPIO */
    /* OSPI CS GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* OSPI CLK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* OSPI D0 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_8;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D1 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D2 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D3 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D4 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_1;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* OSPI D5 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_3;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* OSPI D6 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* OSPI D7 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_14;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* OSPI DQS GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_5;
    GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*##-3- Configure the NVIC for OSPI */
    /* NVIC configuration for OSPI interrupt */
    HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);
  }
  else
  {
    /*##-1- Enable peripherals and GPIO Clocks*/
    /* Enable the OctoSPI memory interface clocks */
    __HAL_RCC_OSPI2_CLK_ENABLE();
    /* Reset the OctoSPI memory interface */

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO */
    /* OSPI CS GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPIM_P2;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* OSPI CLK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_4;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D0 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_0;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D1 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_1;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D2 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D3 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_3;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OSPI D4 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_0;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* OSPI D5 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_1;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* OSPI D6 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_10;
    GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPIM_P1 ;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* OSPI D7 GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_11;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2 ;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* OSPI DQS GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*##-3- Configure the NVIC for OSPI */
    /* NVIC configuration for OSPI interrupt */
    HAL_NVIC_SetPriority(OCTOSPI2_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(OCTOSPI2_IRQn);
  }
}

void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef *hospi)
{

  /* Disable the OctoSPI memory interface clock */
  __HAL_RCC_OSPI1_CLK_DISABLE();
  __HAL_RCC_OSPI2_CLK_DISABLE();

  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
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

  /*##-1- Enable peripherals and GPIO Clocks*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO*/
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
  /*##-1- Reset peripherals*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
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
  * @}
  */


