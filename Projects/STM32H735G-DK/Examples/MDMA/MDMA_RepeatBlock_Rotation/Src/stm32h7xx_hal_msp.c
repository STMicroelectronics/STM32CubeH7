/**
  ******************************************************************************
  * @file    MDMA/MDMA_RepeatBlock_Rotation/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup stm32h7xx_HAL_Examples
  * @{
  */

/** @defgroup MDMA_RepeatBlock_Rotation
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void LCD_InitSequence(void);
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */


/**
  * @brief LTDC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{  
  GPIO_InitTypeDef GPIO_Init_Structure;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/  
  /* Enable the LTDC Clock */
  __HAL_RCC_LTDC_CLK_ENABLE();
  
  LCD_InitSequence();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;

    GPIO_Init_Structure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &GPIO_Init_Structure);

    GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

    GPIO_Init_Structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

    GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

    GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

    GPIO_Init_Structure.Pin       = GPIO_PIN_7 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

    GPIO_Init_Structure.Pin       = GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

    GPIO_Init_Structure.Alternate = GPIO_AF13_LTDC;
    GPIO_Init_Structure.Pin       = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOA, &GPIO_Init_Structure);

    GPIO_Init_Structure.Alternate = GPIO_AF9_LTDC;
    GPIO_Init_Structure.Pin       = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

    /** Toggle Sw reset of LTDC peripheral */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();
}


/**
  * @brief  LCD Init Sequence
  *         Configure the display pinout.
  * @retval None
  */
static void LCD_InitSequence(void)
{
  GPIO_InitTypeDef  gpio_init_structure;
  /* LCD_DISP GPIO configuration */
  LCD_DISP_EN_GPIO_CLK_ENABLE();
  
  gpio_init_structure.Pin       = LCD_DISP_EN_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_DISP_EN_GPIO_PORT, &gpio_init_structure);
  /* Assert LCD_DISP  pin */
  HAL_GPIO_WritePin(LCD_DISP_EN_GPIO_PORT, LCD_DISP_EN_PIN, GPIO_PIN_SET);
 
  /* LCD_BL_CTRL GPIO configuration */
  LCD_BL_CTRL_GPIO_CLK_ENABLE();
  
  gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &gpio_init_structure);
  /* Assert back-light LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);
}

/**
  * @brief LTDC MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)
{
  
  /*##-1- Reset peripherals ##################################################*/
  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();
  
  /* Release LTDC from reset state */ 
  __HAL_RCC_LTDC_RELEASE_RESET();
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

