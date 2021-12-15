/**
  ******************************************************************************
  * @file    ADC/ADC_DifferentialMode/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

