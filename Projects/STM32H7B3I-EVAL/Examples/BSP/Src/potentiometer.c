/**
  ******************************************************************************
  * @file    BSP/Src/potentiometer.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SRAM Driver
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

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
BSP_POT_Cb_t Callback;
/* Private function prototypes -----------------------------------------------*/
static void POTENTIOMETER_SetHint(void);
static void POT_MspInit(ADC_HandleTypeDef *hadc);
static void POT_MspDeInit(ADC_HandleTypeDef *hadc);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SRAM Demo
  * @param  None
  * @retval None
  */
void POTENTIOMETER_demo (void)
{
  uint32_t new_level = 0, old_level = 0;
  char text[50];

  ButtonState = 0;
  POTENTIOMETER_SetHint();

  /* POTENTIOMETER initialization */
  Callback.pMspDeInitCb = POT_MspDeInit;
  Callback.pMspInitCb   = POT_MspInit;
  BSP_POT_RegisterMspCallbacks (POT1, &Callback);
  BSP_POT_Init(POT1);

  UTIL_LCD_SetFont(&Font24);
  /* Set the LCD Back Color and Text Color*/
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);

  while(1)
  {
    /* Get potentiometer voltage level */
    new_level = BSP_POT_GetLevel(POT1);
    if((new_level > old_level + 2) || (new_level < old_level - 2))
    {
      old_level = new_level;
      /* Display voltage level */
      sprintf((char*)text," Voltage Level  = %lu ",new_level);
      UTIL_LCD_DisplayStringAt(0, 480/2 + 45, (uint8_t *)text, CENTER_MODE);
    }

    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      return;
    }
  }
}
/**
  * @brief  Initialize POT1 Msp part
  * @param  hadc ADC handle
  * @retval None
  */
static void POT_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  POT1_CLK_ENABLE();
  POT1_CHANNEL_GPIO_CLK_ENABLE();

  /* ADC Channel GPIO pin configuration */
  gpio_init_structure.Pin  = POT1_CHANNEL_GPIO_PIN;
  gpio_init_structure.Mode = GPIO_MODE_ANALOG;
  gpio_init_structure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(POT1_CHANNEL_GPIO_PORT, &gpio_init_structure);
}

/**
  * @brief  De-Initialize POT1 Msp part
  * @param  hadc ADC handle
  * @retval None
  */
static void POT_MspDeInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* POT GPIO pin configuration */
  gpio_init_structure.Pin  = POT1_CHANNEL_GPIO_PIN;
  HAL_GPIO_DeInit(POT1_CHANNEL_GPIO_PORT, gpio_init_structure.Pin);

  /* Disable ADC clock */
  POT1_CLK_DISABLE();
}

/**
  * @brief  Display POTENTIOMETER Demo Hint
  * @param  None
  * @retval None
  */
static void POTENTIOMETER_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"POTENTIOMETER", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to get", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"the potentiometer's voltage level ", CENTER_MODE);

   /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
}

/**
  * @}
  */

/**
  * @}
  */
