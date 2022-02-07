/**
  ******************************************************************************
  * @file    Core/Common/Src/stm32h7xx_hal_msp.c
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
#include "stm32h7xx_hal.h"
#include "stm32h750b_discovery.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USARTx                           USART3
#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();

#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF7_USART3

#define RTC_CLOCK_SOURCE_LSI

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup HAL_MSP_Private_Functions
  * @{
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

  /* Enable USART1 clock */
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
  GPIO_InitStruct.Pin       = USARTx_RX_PIN;
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

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

}

/**
  * @brief  Initializes SDRAM MSP.
  * @param  hsdram: SDRAM handle
  * @param  Params: Pointer to void
  * @retval None
  */
void BSP_SDRAM_MspInit(SDRAM_HandleTypeDef  *hsdram, void *Params)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  
  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9 | GPIO_PIN_10 |\
                              GPIO_PIN_14 | GPIO_PIN_15;
 
   
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
      
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
    
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);
}

/**
  * @brief  DeInitializes SDRAM MSP.
  * @param  hsdram: SDRAM handle
  * @param  Params: Pointer to void
  * @retval None
  */
void BSP_SDRAM_MspDeInit(SDRAM_HandleTypeDef  *hsdram, void *Params)
{
  /* GPIOD : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOD, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9 | GPIO_PIN_10 |\
                          GPIO_PIN_14 | GPIO_PIN_15));

  /* GPIOE : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                          GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                          GPIO_PIN_15));

  /* GPIOF : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOF, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                          GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                          GPIO_PIN_15));

  /* GPIOG : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOG, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15));

  /* GPIOH : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOH, (GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7));
}

/**
  * @brief DMA2D MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{  
  /* Enable peripheral */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA2D_IRQn, 0x5, 0x0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @brief DMA2D MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)
{
  /* Disable periphiral interrupts */
  HAL_NVIC_DisableIRQ(DMA2D_IRQn);

  /* Enable DMA2D reset state */
  __HAL_RCC_DMA2D_FORCE_RESET();
  
  /* Release DMA2D from reset state */ 
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /* Disable peripheral */
  __HAL_RCC_DMA2D_CLK_DISABLE();
}

/**
  * @brief LTDC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{  
  GPIO_InitTypeDef  gpio_init_structure;
  
/* LTDC Initialization -------------------------------------------------------*/
  
 /* Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  /*** LTDC Pins configuration ***/

  /* GPIOI configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_9 | \
                                  GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* GPIOJ configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | \
                                  GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
                                  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | \
                                  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

  /* GPIOK configuration */
  gpio_init_structure.Pin       = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | \
                                  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);
  
  /* GPIOH configuration */
  gpio_init_structure.Pin       = GPIO_PIN_9;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);  

  /* Set LTDC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0x7, 0);
  
  /* Enable LTDC Interrupt */
  HAL_NVIC_EnableIRQ(LTDC_IRQn);
}

/**
  * @brief LTDC MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)
{
  /* Disable LTDC Interrupt */
  HAL_NVIC_DisableIRQ(LTDC_IRQn);

  /* GPIOI : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOI, ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_9 | \
                           GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15) );

  /* GPIOI : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOI, ( GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_9 | \
                           GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15) );

  /* GPIOJ : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOJ, ( GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_3 | \
                           GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | \
                           GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
                           GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | \
                           GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15) );

  /* GPIOK : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOK, ( GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | \
                           GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | \
                           GPIO_PIN_7) );

  /* GPIOH : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_9);

  /* GPIOD : Set Default Configuration */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_7);
  
  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();

  /* Release LTDC from reset state */ 
  __HAL_RCC_LTDC_RELEASE_RESET();

 /* Disable the LTDC clock */
  __HAL_RCC_LTDC_CLK_DISABLE();
}

