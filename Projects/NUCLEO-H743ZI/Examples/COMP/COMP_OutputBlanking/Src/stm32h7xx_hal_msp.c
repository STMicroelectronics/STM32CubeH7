/**
  ******************************************************************************
  * @file    COMP/COMP_OutputBlanking/Src/stm32h7xx_hal_msp.c
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

/** @defgroup COMP_OutputBlanking
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
  *           - NVIC configuration for COMP interrupt request enable
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
  GPIO_InitTypeDef      GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  COMPx_INPUT_GPIO_CLK_ENABLE();
  COMPx_OUTPUT_GPIO_CLK_ENABLE();
  /* COMP Periph clock enable */
  COMPx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* COMPx input GPIO pin configuration */
  GPIO_InitStruct.Pin = COMPx_INPUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(COMPx_INPUT_GPIO_PORT, &GPIO_InitStruct);

  /* COMPx output GPIO pin configuration */
  GPIO_InitStruct.Pin = COMPx_OUTPUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = COMPx_OUTPUT_AF;
  HAL_GPIO_Init(COMPx_OUTPUT_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes the COMP MSP.
  * @param  hcomp: pointer to a COMP_HandleTypeDef structure that contains
  *         the configuration information for the specified COMP.  
  * @retval None
  */
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
  /*##-1- Disable COMP peripheral clock #####################################*/   
   __HAL_RCC_COMP12_CLK_DISABLE();
  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the COMPx GPIO pin */
  HAL_GPIO_DeInit(COMPx_INPUT_GPIO_PORT,COMPx_INPUT_PIN);
  HAL_GPIO_DeInit(COMPx_OUTPUT_GPIO_PORT,COMPx_OUTPUT_PIN);
}

/**
  * @brief  Initializes the TIM PWM MSP.
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef      GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  TIMx_GPIO_CLK_ENABLE();
  /* TIM1 Periph clock enable */
  TIMx_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* TIMx GPIO pin configuration */
  GPIO_InitStruct.Pin = TIMx_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = TIMx_AF;
  HAL_GPIO_Init(TIMx_GPIO_PORT, &GPIO_InitStruct);
}


/**
  * @brief  DeInitializes TIM PWM MSP.
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  /* Enable TIMx reset state */
  TIMx_FORCE_RESET();
  /* Release TIMx from reset state */
  TIMx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the GPIO pin */
  HAL_GPIO_DeInit(TIMx_GPIO_PORT, TIMx_PIN);
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

