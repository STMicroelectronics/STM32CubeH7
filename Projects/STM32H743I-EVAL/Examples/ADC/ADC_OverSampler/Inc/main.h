/**
  ******************************************************************************
  * @file    ADC/ADC_OverSampler/Inc/main.h
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
   
/* Definition for ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC12_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_0
#define ADCx_CHANNEL_GPIO_PORT          GPIOA

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_0
#define SAMPLINGTIME                    ADC_SAMPLETIME_8CYCLES_5

/* Definition for ADCx's Oversampling parameters */
#define OVERSAMPLING_RATIO              1023                               /* 1024-oversampling */                                      
#define RIGHTBITSHIFT                   ADC_RIGHTBITSHIFT_6                /* 6-bit right shift of the oversampled summation */          
#define TRIGGEREDMODE                   ADC_TRIGGEREDMODE_SINGLE_TRIGGER   /* A single trigger for all channel oversampled conversions */
#define OVERSAMPLINGSTOPRESET           ADC_REGOVERSAMPLING_CONTINUED_MODE /* Oversampling buffer maintained during injection sequence */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

