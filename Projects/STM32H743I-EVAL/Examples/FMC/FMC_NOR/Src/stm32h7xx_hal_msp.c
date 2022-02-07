/**
  ******************************************************************************
  * @file    FMC/FMC_NOR/Src/stm32h7xx_hal_msp.c
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

/** @defgroup FMC_NOR_Basic
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
  * @brief NOR MSP Initialization
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hnor: NOR handle pointer
  * @retval None
  */
void HAL_NOR_MspInit(NOR_HandleTypeDef *hnor)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();  
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
    /* GPIOC configuration */
  gpio_init_structure.Alternate = GPIO_AF9_FMC;
  gpio_init_structure.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);
  
  /* GPIOD configuration */
  gpio_init_structure.Alternate = GPIO_AF12_FMC;  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7 |\
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                              GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 ;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
    
  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6   |\
                              GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                              GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
}

/**
  * @brief NOR MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param hnor: NOR handle pointer
  * @retval None
  */
void HAL_NOR_MspDeInit(NOR_HandleTypeDef *hnor)
{
  /*## Disable peripherals and GPIO Clocks ###################################*/
  /* Configure FMC as alternate function  */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6   |\
                         GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                         GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
  
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6   |\
                         GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                         GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                         GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                         GPIO_PIN_5);
}


/**
  * @brief  NOR BSP Wait for Ready/Busy signal.
  * @param  norHandle: Pointer to NOR handle
  * @param  Timeout: Timeout duration  
  * @retval None
  */
void HAL_NOR_MspWait(NOR_HandleTypeDef *norHandle, uint32_t Timeout)
{
  uint32_t timeout = Timeout;
  
  /* Polling on Ready/Busy signal */
  while((HAL_GPIO_ReadPin(NOR_READY_BUSY_GPIO, NOR_READY_BUSY_PIN) != NOR_BUSY_STATE) && (timeout > 0)) 
  {
    timeout--;
  }
  
  timeout = Timeout;
  
  /* Polling on Ready/Busy signal */
  while((HAL_GPIO_ReadPin(NOR_READY_BUSY_GPIO, NOR_READY_BUSY_PIN) != NOR_READY_STATE) && (timeout > 0)) 
  {
    timeout--;
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

