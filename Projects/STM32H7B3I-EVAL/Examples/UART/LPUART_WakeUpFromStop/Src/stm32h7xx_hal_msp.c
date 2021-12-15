/**
  ******************************************************************************
  * @file    UART/LPUART_WakeUpFromStop/Src/stm32h7xx_hal_msp.c
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

/** @defgroup LPUART_WakeUpFromStop
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
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef          GPIO_InitStruct;
  RCC_OscInitTypeDef        RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  LPUARTx_TX_GPIO_CLK_ENABLE();
  LPUARTx_RX_GPIO_CLK_ENABLE();
  
  /* Enable LPUART clock */
  LPUARTx_CLK_ENABLE(); 
  
  /*##-2- Enable the HSI clock and select HSI as LPUART source clock #*/
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Error */
    while(1); 
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_HSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    while(1);
  }  
  
  /*##-3- Configure peripheral GPIO ##########################################*/
  /* LPUART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = LPUARTx_TX_AF;
  
  HAL_GPIO_Init(LPUARTx_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* LPUART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUARTx_RX_PIN;
  GPIO_InitStruct.Alternate = LPUARTx_RX_AF;
  
  HAL_GPIO_Init(LPUARTx_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /*##-4- Configure the NVIC for LPUART ######################################*/
  /* NVIC for LPUART */
  HAL_NVIC_SetPriority(LPUARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPUARTx_IRQn);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  LPUARTx_FORCE_RESET();
  LPUARTx_RELEASE_RESET();
  
  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  HAL_GPIO_DeInit(LPUARTx_TX_GPIO_PORT, LPUARTx_TX_PIN);
  HAL_GPIO_DeInit(LPUARTx_RX_GPIO_PORT, LPUARTx_RX_PIN);
}

/**
  * @brief UART wakeup from Stop mode callback
  * @param huart: uart handle
  * @retval None
  */
void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef *huart)
{
  /* Turn back on LED1 */
  BSP_LED_On(LED1);
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

