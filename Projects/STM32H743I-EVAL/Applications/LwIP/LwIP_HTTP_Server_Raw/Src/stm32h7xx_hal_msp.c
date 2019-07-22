/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Raw/Src/tm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *         
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup stm32h7xx_HAL_Applications
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
  * @brief  ADC_MspInit
  * @param  None
  * @retval None
  */
//void HAL_ADC_MspInit(ADC_HandleTypeDef *heth)
//{
//   GPIO_InitTypeDef GPIO_InitStructure;
//   
//  /* Enable GPIOF clock */
//   __HAL_RCC_GPIOF_CLK_ENABLE();
//   
//   /* Configure PF10 as analog input */
//  GPIO_InitStructure.Pin = GPIO_PIN_10;
//  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStructure.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
//   
//   /* ADC3 Periph clock enable */
//   __HAL_RCC_ADC3_CLK_ENABLE();
//}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
