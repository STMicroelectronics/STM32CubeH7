/**
  ******************************************************************************
  * @file    OSPI/OSPI_NOR_MemoryMapped_DTR/Src/stm32h7xx_hal_msp.c
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

/** @defgroup HAL_MSP
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
  OSPIM_CfgTypeDef OSPIM_Cfg_Struct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the OctoSPI memory interface clocks */
  OSPI_CLK_ENABLE();
  OSPIM_CLK_ENABLE();
  /* Reset the OctoSPI memory interface */
  OSPI_FORCE_RESET();
  OSPI_RELEASE_RESET();
  
  /* Enable GPIO clocks */
  OSPI_CS_GPIO_CLK_ENABLE();
  OSPI_CLK_GPIO_CLK_ENABLE();
  OSPI_D0_GPIO_CLK_ENABLE();
  OSPI_D1_GPIO_CLK_ENABLE();
  OSPI_D2_GPIO_CLK_ENABLE();
  OSPI_D3_GPIO_CLK_ENABLE();
  OSPI_D4_GPIO_CLK_ENABLE();
  OSPI_D5_GPIO_CLK_ENABLE();
  OSPI_D6_GPIO_CLK_ENABLE();
  OSPI_D7_GPIO_CLK_ENABLE();
  OSPI_DQS_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* OSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
  HAL_GPIO_Init(OSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
  /* OSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(OSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
  /* OSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D0_PIN;
  HAL_GPIO_Init(OSPI_D0_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D1_PIN;
  HAL_GPIO_Init(OSPI_D1_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D2_PIN;
  HAL_GPIO_Init(OSPI_D2_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D3_PIN;
  HAL_GPIO_Init(OSPI_D3_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D4_PIN;
  HAL_GPIO_Init(OSPI_D4_GPIO_PORT, &GPIO_InitStruct);
  
  /* OSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
  GPIO_InitStruct.Pin       = OSPI_D5_PIN;
  HAL_GPIO_Init(OSPI_D5_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
  GPIO_InitStruct.Pin       = OSPI_D6_PIN;
  HAL_GPIO_Init(OSPI_D6_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
  GPIO_InitStruct.Pin       = OSPI_D7_PIN;
  HAL_GPIO_Init(OSPI_D7_GPIO_PORT, &GPIO_InitStruct);
  
  /* OSPI DQS GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
  GPIO_InitStruct.Pin       = OSPI_DQS_PIN;
  HAL_GPIO_Init(OSPI_DQS_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for OSPI #########################################*/
  /* NVIC configuration for OSPI interrupt */
  HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);

  /*##-4- Configure the OctoSPI IO Manager ####################################*/
  OSPIM_Cfg_Struct.ClkPort    = 1;
  OSPIM_Cfg_Struct.DQSPort    = 1;
  OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
  OSPIM_Cfg_Struct.IOLowPort  = HAL_OSPIM_IOPORT_1_LOW;
  OSPIM_Cfg_Struct.NCSPort    = 1;

  if (HAL_OSPIM_Config(hospi, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    BSP_LED_On(LED_RED);
    
    /* User may add here some code to deal with this error */
    while(1)
    {
    }
  }
 
}

/**
  * @brief OSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef *hospi)
{
  /*##-1- Disable the NVIC for OSPI and DMA ##################################*/
  HAL_NVIC_DisableIRQ(OCTOSPI2_IRQn);

  /*##-2- Disable peripherals ################################################*/
  /* De-Configure OSPI pins */
  HAL_GPIO_DeInit(OSPI_CS_GPIO_PORT, OSPI_CS_PIN);
  HAL_GPIO_DeInit(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN);
  HAL_GPIO_DeInit(OSPI_D0_GPIO_PORT, OSPI_D0_PIN);
  HAL_GPIO_DeInit(OSPI_D1_GPIO_PORT, OSPI_D1_PIN);
  HAL_GPIO_DeInit(OSPI_D2_GPIO_PORT, OSPI_D2_PIN);
  HAL_GPIO_DeInit(OSPI_D3_GPIO_PORT, OSPI_D3_PIN);
  HAL_GPIO_DeInit(OSPI_D4_GPIO_PORT, OSPI_D4_PIN);
  HAL_GPIO_DeInit(OSPI_D5_GPIO_PORT, OSPI_D5_PIN);
  HAL_GPIO_DeInit(OSPI_D6_GPIO_PORT, OSPI_D6_PIN);
  HAL_GPIO_DeInit(OSPI_D7_GPIO_PORT, OSPI_D7_PIN);
  HAL_GPIO_DeInit(OSPI_DQS_GPIO_PORT, OSPI_DQS_PIN);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the OctoSPI memory interface */
  OSPI_FORCE_RESET();
  OSPI_RELEASE_RESET();

  /* Disable the OctoSPI memory interface clocks */
  OSPI_CLK_DISABLE();
  OSPIM_CLK_DISABLE();
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

