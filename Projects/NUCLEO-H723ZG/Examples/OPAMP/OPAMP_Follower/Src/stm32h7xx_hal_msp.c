/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_Follower/Src/stm32h7xx_hal_msp.c
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

/** @defgroup OPAMP_Follower
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
  * @brief COMP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hcomp  COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef *hcomp)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and  Clocks ####################################*/
  /* Enable COMP Periph clock  */
  __HAL_RCC_COMP12_CLK_ENABLE();
  
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure the GPIO used as COMP1 non inverting input  */
  
  /* COMP GPIO pin input configuration */
  GPIO_InitStruct.Pin	= GPIO_PIN_2;
  GPIO_InitStruct.Mode	= GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  /* COMP GPIO pin output configuration */
  GPIO_InitStruct.Pin	    = GPIO_PIN_12;
  GPIO_InitStruct.Mode	    = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull	    = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = GPIO_AF13_COMP1; 
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;  
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes the COMP MSP.
  * @param  hcomp  pointer to a COMP_HandleTypeDef structure that contains
  *         the configuration information for the specified COMP.
  * @retval None
  */
void HAL_COMP_MspDeInit(COMP_HandleTypeDef *hcomp)
{
  /*##-1- Disable COMP Periph clock ##########################################*/
  __HAL_RCC_COMP12_CLK_DISABLE();
	
  /*##-2- Disable  GPIO Clocks ################################*/
	/* De-initialize the COMP GPIO pin */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_12);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);
  /* Disable GPIO clock **************************************************/
  /* Note: In a real application, by disabling GPIO port, be cautious to not  */
  /* interfere with potential other peripherals using other GPIOs on the same */
  /* port.                                                                    */
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
}
/**
  * @brief DAC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  /* DAC Periph clock enable */
  __HAL_RCC_DAC12_CLK_ENABLE();
}

/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac  pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_DAC12_FORCE_RESET();
  __HAL_RCC_DAC12_RELEASE_RESET();
}


/**
  * @brief OPAMP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hopamp: OPAMP handle pointer
  * @retval None
  */
void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef *hopamp)
{
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_ENABLE();  
}

/**
  * @brief  DeInitializes the OPAMP MSP.
  * @param  hopamp  pointer to a OPAMP_HandleTypeDef structure that contains
  *         the configuration information for the specified OPAMP.  
  * @retval None
  */
void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef *hopamp)
{
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_DISABLE();  
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

