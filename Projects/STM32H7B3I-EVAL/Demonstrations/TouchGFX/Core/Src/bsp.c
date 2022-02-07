/**
******************************************************************************
* @file    bsp.c
* @author  MCD Application Team
* @brief   This file provides the kernel bsp functions
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
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_TIM_PERIOD_VALUE              ((uint32_t)200) /* Period Value    */
#define LCD_TIM_PRESCALER_VALUE           ((uint32_t)27)  /* Prescaler Value */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef HLDC_TimHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Set the brightness value
  * @param  Brightness [00: Min (black), 100 Max]
* @retval None
*/
void BSP_SetBrightness(uint32_t Brightness)
{
  __HAL_TIM_SET_COMPARE(&HLDC_TimHandle, TIM_CHANNEL_2, 2U*Brightness);
}

/**
  * @brief  Initializes TIM in PWM mode
  * @param  htim TIM handle
  * @retval None
  */
void BSP_PWM_Init(TIM_HandleTypeDef *htim)
{
  TIM_OC_InitTypeDef LCD_TIM_Config;
  GPIO_InitTypeDef gpio_init_structure;

  /* Timer_Clock = 2 x  APB1_clock = 280 MHz */
  /* PWM_freq = Timer_Clock /(Period x (Prescaler + 1))*/
  /* PWM_freq = 280 MHz /(200 x (27 + 1)) = 50000 Hz*/
  htim->Instance = TIM5;
  (void)HAL_TIM_PWM_DeInit(htim);

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM5_CLK_ENABLE();

  /* Timer channel configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  gpio_init_structure.Alternate = GPIO_AF2_TIM5;
  gpio_init_structure.Pin       = GPIO_PIN_1; /* BL_CTRL */
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  htim->Init.Prescaler         = LCD_TIM_PRESCALER_VALUE;
  htim->Init.Period            = LCD_TIM_PERIOD_VALUE - 1U;
  htim->Init.ClockDivision     = 0;
  htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim->Init.RepetitionCounter = 0;
  htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  (void)HAL_TIM_PWM_Init(htim);

  /* Common configuration for all channels */
  LCD_TIM_Config.OCMode       = TIM_OCMODE_PWM1;
  LCD_TIM_Config.OCPolarity   = TIM_OCPOLARITY_HIGH;
  LCD_TIM_Config.OCFastMode   = TIM_OCFAST_DISABLE;
  LCD_TIM_Config.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  LCD_TIM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  LCD_TIM_Config.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the default pulse value for channel: 50% duty cycle */
  LCD_TIM_Config.Pulse = 100;

  (void)HAL_TIM_PWM_ConfigChannel(&HLDC_TimHandle, &LCD_TIM_Config, TIM_CHANNEL_2);

  /* Start PWM Timer channel */
  (void)HAL_TIM_PWM_Start(&HLDC_TimHandle, TIM_CHANNEL_2);
}


