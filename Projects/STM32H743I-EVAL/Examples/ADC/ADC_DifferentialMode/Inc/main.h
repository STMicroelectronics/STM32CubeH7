/**
  ******************************************************************************
  * @file    ADC/ADC_DifferentialMode/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated
   resources */

/**
 * @brief Definition for Potentiometer, connected to ADC1
 */ 
#define ADCx_DIFF                            ADC2
#define ADCx_CLK_ENABLE()                    __HAL_RCC_ADC12_CLK_ENABLE()
#define ADCx_DIFF_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

#define ADCx_FORCE_RESET()                   __HAL_RCC_ADC12_FORCE_RESET()
#define ADCx_RELEASE_RESET()                 __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_DIFF_CHANNEL_PIN1               GPIO_PIN_13
#define ADCx_DIFF_CHANNEL_PIN2               GPIO_PIN_14
#define ADCx_DIFF_CHANNEL_GPIO_PORT          GPIOF

/* Definition for ADCx's Differential Channel */
#define ADCx_DIFF_CHANNEL                    ADC_CHANNEL_2

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
