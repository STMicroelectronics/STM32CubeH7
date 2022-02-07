/**
  ******************************************************************************
  * @file    OCTOSPI/OCTOSPI_HyperRAM_MemoryMapped/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup Templates
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
  *           - NVIC configuration for OSPI interrupts
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;

  /* Enable the OctoSPI memory interface clock */
  OSPI2_CLK_ENABLE();

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI2_FORCE_RESET();
  __HAL_RCC_OSPI2_RELEASE_RESET();

  /* Enable the GPIO clocks */
  OSPI2_CS_GPIO_CLK_ENABLE();
  OSPI2_CLK_P_GPIO_CLK_ENABLE();
  OSPI2_D0_GPIO_CLK_ENABLE();
  OSPI2_D1_GPIO_CLK_ENABLE();
  OSPI2_D2_GPIO_CLK_ENABLE();
  OSPI2_D3_GPIO_CLK_ENABLE();
  OSPI2_D4_GPIO_CLK_ENABLE();
  OSPI2_D5_GPIO_CLK_ENABLE();
  OSPI2_D6_GPIO_CLK_ENABLE();
  OSPI2_D7_GPIO_CLK_ENABLE();
  OSPI2_DQS_GPIO_CLK_ENABLE();

  /* OSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_CS_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_D0_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_D0_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_D1_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_D1_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI2 D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_D2_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_D2_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI2 D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_D3_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_D3_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI2 D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_D4_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_D4_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI2 D5 GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  GPIO_InitStruct.Pin       = OSPI2_D5_PIN;
  HAL_GPIO_Init(OSPI2_D5_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI2 D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI2_D6_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF3_OCTOSPIM_P2;
  HAL_GPIO_Init(OSPI2_D6_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI2 D7 GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  GPIO_InitStruct.Pin       = OSPI2_D7_PIN;
  HAL_GPIO_Init(OSPI2_D7_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI2 DQS GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P2;
  GPIO_InitStruct.Pin       = OSPI2_DQS_PIN;
  HAL_GPIO_Init(OSPI2_DQS_GPIO_PORT, &GPIO_InitStruct);

  /* Enable and set OctoSPI interrupt to the lowest priority */
  HAL_NVIC_SetPriority(OCTOSPI2_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(OCTOSPI2_IRQn);
 }



/**
  * @brief  DeInitializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspDeInit(void)
{
  /* Disable OctoSPI interrupts */
  HAL_NVIC_DisableIRQ(OCTOSPI2_IRQn);

  /* Reset the OctoSPI memory interface */
  __HAL_RCC_OSPI2_FORCE_RESET();
  __HAL_RCC_OSPI2_RELEASE_RESET();

  /* Disable the OctoSPI memory interface clock */
  __HAL_RCC_OSPI2_CLK_DISABLE();

#if (USE_HAL_OSPI_REGISTER_CALLBACKS == 1)
  MspDeInit++;
#endif
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

