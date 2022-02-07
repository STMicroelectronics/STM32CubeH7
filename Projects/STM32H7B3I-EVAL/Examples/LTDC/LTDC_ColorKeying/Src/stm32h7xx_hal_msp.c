/**
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying/Src/stm32h7xx_hal_msp.c
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

/** @defgroup LTDC_ColorKeying
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void LCD_InitSequence(void);
static void LCD_Reset(void);

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
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;  
  GPIO_InitTypeDef GPIO_Init_Structure;
  LCD_Reset();
  LCD_InitSequence();
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the LTDC Clock */
  __HAL_RCC_LTDC_CLK_ENABLE();
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  
  /*** LTDC Pins configuration ***/
  /* GPIOI configuration */
  GPIO_Init_Structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &GPIO_Init_Structure);
  
  /* GPIOJ configuration */
  GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);
  
  /* GPIOK configuration */
  GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
  
  /* RK070ER9427 typical PCLK is 33.3 MHz so the PLL3R is configured to provide this clock */ 
  /* RK070ER9427 LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 2 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 400 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 400/12 = 33.3Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 33.3 Mhz */    
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 12;    
  PeriphClkInitStruct.PLL3.PLL3N = 200;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 12;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);   
  HAL_GPIO_Init(GPIOK, &GPIO_Init_Structure);
  
  /* Set LTDC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0xF, 0);   
  
  /* Enable LTDC Interrupt */
  HAL_NVIC_EnableIRQ(LTDC_IRQn); 

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
  * @brief  LCD Reset
  *         Hw reset the LCD activating its XRES signal (active low for some time)
  *         and deactivating it later.
  * @retval None
  */
static void LCD_Reset(void)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  LCD_RESET_GPIO_CLK_ENABLE();
  
  /* Configure the GPIO Reset pin */
  gpio_init_structure.Pin   = LCD_RESET_PIN;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = LCD_RESET_PULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_RESET_GPIO_PORT , &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT , LCD_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(20);/* wait 20 ms */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT , LCD_RESET_PIN, GPIO_PIN_SET);/* Deactivate XRES */
  HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */
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

