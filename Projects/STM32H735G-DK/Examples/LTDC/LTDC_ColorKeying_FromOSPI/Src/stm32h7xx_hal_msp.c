/**
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying_FromOSPI/Src/stm32h7xx_hal_msp.c
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

/** @defgroup LTDC_ColorKeying_FromOSPI
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
  * @brief LTDC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  
  gpio_init_structure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);
  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);
  
  gpio_init_structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);
  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  gpio_init_structure.Pin       = GPIO_PIN_7 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
  
  gpio_init_structure.Pin       = GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);
  
  gpio_init_structure.Alternate = GPIO_AF13_LTDC;
  gpio_init_structure.Pin       = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);
  
  gpio_init_structure.Alternate = GPIO_AF9_LTDC;
  gpio_init_structure.Pin       = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);
  
  /** Toggle Sw reset of LTDC peripheral */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();
  
  LCD_DISP_CTRL_GPIO_CLK_ENABLE();
  LCD_BL_CTRL_GPIO_CLK_ENABLE();
  LCD_DISP_EN_GPIO_CLK_ENABLE();

  /* LCD_DISP_EN GPIO configuration */
  gpio_init_structure.Pin       = LCD_DISP_EN_PIN;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(LCD_DISP_EN_GPIO_PORT, &gpio_init_structure);

  /* LCD_DISP_CTRL GPIO configuration */
  gpio_init_structure.Pin       = LCD_DISP_CTRL_PIN;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(LCD_DISP_CTRL_GPIO_PORT, &gpio_init_structure);

  /* LCD_BL_CTRL GPIO configuration */
  gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;  /* LCD_BL_CTRL pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &gpio_init_structure);
  
  
  /* De-assert display enable LCD_DISP_EN pin */
  HAL_GPIO_WritePin(LCD_DISP_EN_GPIO_PORT, LCD_DISP_EN_PIN, GPIO_PIN_RESET);
  
  /* Assert display enable LCD_DISP_CTRL pin */
  HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT, LCD_DISP_CTRL_PIN, GPIO_PIN_SET);
  
  /* Assert backlight LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);

  /*LCD clock config*/
  
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* RK043FN48H LCD clock configuration */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/83 = 9.63 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 9.63 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;
  PeriphClkInitStruct.PLL3.PLL3N = 160;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 83;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL1VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL1VCIRANGE_0;

  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) ;

  /* Set LTDC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0xF, 0);

  /* Enable LTDC Interrupt */
  HAL_NVIC_EnableIRQ(LTDC_IRQn);
  
  HAL_Delay(30);
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

