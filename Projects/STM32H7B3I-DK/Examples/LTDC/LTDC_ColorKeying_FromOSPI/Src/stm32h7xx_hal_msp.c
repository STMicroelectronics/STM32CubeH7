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
static void LCD_InitSequence(void);
void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi) ;

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
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);

  /* GPIOK configuration */
  GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOK, &GPIO_Init_Structure);
  
  /** Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();
  
  /* RK043FN48H LCD clock configuration */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 4 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/83 = 9.63 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 9.63 Mhz */
  PeriphClkInitStruct.PeriphClockSelection   = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 6;
  PeriphClkInitStruct.PLL3.PLL3N = 200;
  PeriphClkInitStruct.PLL3.PLL3P = 10;
  PeriphClkInitStruct.PLL3.PLL3Q = 10;
  PeriphClkInitStruct.PLL3.PLL3R = 83;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = 0;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
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
  
  /* LCD_DISP_CTRL GPIO configuration */
  LCD_BL_CTRL_GPIO_CLK_ENABLE();
  gpio_init_structure.Pin       = LCD_DISP_CTRL_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(LCD_DISP_CTRL_GPIO_PORT, &gpio_init_structure);
  
  /* LCD_BL_CTRL GPIO configuration */
  LCD_BL_CTRL_GPIO_CLK_ENABLE();
  
  gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;   
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &gpio_init_structure);
  
  /* De-assert display enable LCD_DISP_EN pin */
  HAL_GPIO_WritePin(LCD_DISP_EN_GPIO_PORT, LCD_DISP_EN_PIN, GPIO_PIN_RESET);
  
  /* Assert display enable LCD_DISP_CTRL pin */
  HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT, LCD_DISP_CTRL_PIN, GPIO_PIN_SET);
  
  /* Assert backlight LCD_BL_CTRL pin */
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
  * @brief OSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral
  *           - NVIC configuration for DMA and OSPI interrupts
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  OSPIM_CfgTypeDef OSPIM_Cfg_Struct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the OctoSPI memory interface clocks */
  OSPI_CLK_ENABLE();
  OSPIM_CLK_ENABLE();
  /* Reset the OctoSPI memory interface */
  OSPI_FORCE_RESET();
  OSPI_RELEASE_RESET();
  
  /* Enable GPIO clocks */
  OSPI_CS_GPIO_CLK_ENABLE();
  OSPI_CLK_GPIO_CLK_ENABLE();
  OSPI_D0_GPIO_CLK_ENABLE();
  OSPI_D1_GPIO_CLK_ENABLE();
  OSPI_D2_GPIO_CLK_ENABLE();
  OSPI_D3_GPIO_CLK_ENABLE();
  OSPI_D4_GPIO_CLK_ENABLE();
  OSPI_D5_GPIO_CLK_ENABLE();
  OSPI_D6_GPIO_CLK_ENABLE();
  OSPI_D7_GPIO_CLK_ENABLE();
  OSPI_DQS_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* OSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
  HAL_GPIO_Init(OSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
  /* OSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(OSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D0_PIN;
  HAL_GPIO_Init(OSPI_D0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
  /* OSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D1_PIN;
  HAL_GPIO_Init(OSPI_D1_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D2_PIN;
  HAL_GPIO_Init(OSPI_D2_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D3_PIN;
  HAL_GPIO_Init(OSPI_D3_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D4_PIN;
  HAL_GPIO_Init(OSPI_D4_GPIO_PORT, &GPIO_InitStruct);
  
  /* OSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
  GPIO_InitStruct.Pin       = OSPI_D5_PIN;
  HAL_GPIO_Init(OSPI_D5_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D6_PIN;
  HAL_GPIO_Init(OSPI_D6_GPIO_PORT, &GPIO_InitStruct);

  /* OSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
  GPIO_InitStruct.Pin       = OSPI_D7_PIN;
  HAL_GPIO_Init(OSPI_D7_GPIO_PORT, &GPIO_InitStruct);
  
  /* OSPI DQS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_DQS_PIN;
  HAL_GPIO_Init(OSPI_DQS_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for OSPI #########################################*/
  /* NVIC configuration for OSPI interrupt */
  HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);

  /*##-4- Configure the OctoSPI IO Manager ####################################*/
  OSPIM_Cfg_Struct.ClkPort    = 1;
  OSPIM_Cfg_Struct.DQSPort    = 1;
  OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
  OSPIM_Cfg_Struct.IOLowPort  = HAL_OSPIM_IOPORT_1_LOW;
  OSPIM_Cfg_Struct.NCSPort    = 1;

  if (HAL_OSPIM_Config(hospi, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    BSP_LED_On(LED_RED);
    
    /* User may add here some code to deal with this error */
    while(1)
    {
    }
  }
 
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

