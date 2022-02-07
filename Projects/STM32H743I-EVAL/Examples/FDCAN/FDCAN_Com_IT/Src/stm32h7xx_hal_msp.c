/**
  ******************************************************************************
  * @file    FDCAN_Com_IT/Src/stm32h7xx_hal_msp.c
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

/** @defgroup FDCAN_Com_IT
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
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{
}

/**
  * @brief  Initializes the FDCAN MSP.
  * @param  hfdcan: pointer to an FDCAN_HandleTypeDef structure that contains
  *         the configuration information for the specified FDCAN.
  * @retval None
  */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  FDCANx_TX_GPIO_CLK_ENABLE();
  FDCANx_RX_GPIO_CLK_ENABLE();
  
  /* Select PLL1Q as source of FDCANx clock */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
  RCC_PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
  
  /* Enable FDCANx clock */
  FDCANx_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* FDCANx TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = FDCANx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = FDCANx_TX_AF;
  HAL_GPIO_Init(FDCANx_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* FDCANx RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = FDCANx_RX_PIN;
  GPIO_InitStruct.Alternate = FDCANx_RX_AF;
  HAL_GPIO_Init(FDCANx_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /*##-3- Configure the NVIC #################################################*/   
  /* NVIC for FDCANx */
  HAL_NVIC_SetPriority(FDCANx_IT0_IRQn, 0, 1);
  HAL_NVIC_SetPriority(FDCANx_IT1_IRQn, 0, 1);
  HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(FDCANx_IT0_IRQn);
  HAL_NVIC_EnableIRQ(FDCANx_IT1_IRQn);
  HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);
}

/**
  * @brief  DeInitializes the FDCAN MSP.
  * @param  hfdcan: pointer to an FDCAN_HandleTypeDef structure that contains
  *         the configuration information for the specified FDCAN.
  * @retval None
  */
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan)
{
  /*##-1- Reset peripherals ##################################################*/
  FDCANx_FORCE_RESET();
  FDCANx_RELEASE_RESET();
  
  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure FDCANx Tx as alternate function  */
  HAL_GPIO_DeInit(FDCANx_TX_GPIO_PORT, FDCANx_TX_PIN);
  
  /* Configure FDCANx Rx as alternate function  */
  HAL_GPIO_DeInit(FDCANx_RX_GPIO_PORT, FDCANx_RX_PIN);
  
  /*##-3- Disable the NVIC for FDCANx ########################################*/
  HAL_NVIC_DisableIRQ(FDCANx_IT0_IRQn);
  HAL_NVIC_DisableIRQ(FDCANx_IT1_IRQn);
  HAL_NVIC_DisableIRQ(FDCAN_CAL_IRQn);
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

