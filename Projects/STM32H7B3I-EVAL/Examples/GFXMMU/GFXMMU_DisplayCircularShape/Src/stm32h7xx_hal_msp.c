/**
  ******************************************************************************
  * @file    GFXMMU/GFXMMU_DisplayCircularShape/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup GFXMMU
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
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspDeInit(void)
{
}


/** @brief GFXMMU BSP Init
  * @param hgfxmmu: GFXMMU handle
  * @retval None
  */
void HAL_GFXMMU_MspInit(GFXMMU_HandleTypeDef *hgfxmmu)
{
  /* GFXMMU clock enable */
  __HAL_RCC_GFXMMU_CLK_ENABLE();

  /* Enable GFXMMU interrupt */
  HAL_NVIC_SetPriority(GFXMMU_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(GFXMMU_IRQn);
}

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
  GPIO_InitTypeDef gpio_init_structure;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the LTDC Clock */
  __HAL_RCC_LTDC_CLK_ENABLE();
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  /* Configure the GPIO Reset pin and BL pin */
  gpio_init_structure.Pin   = GPIO_PIN_2 | GPIO_PIN_1;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA , &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(GPIOA , GPIO_PIN_2, GPIO_PIN_RESET);
  HAL_Delay(20);/* wait 20 ms */
  HAL_GPIO_WritePin(GPIOA , GPIO_PIN_2, GPIO_PIN_SET);/* Deactivate XRES */
  HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */
  HAL_GPIO_WritePin(GPIOA , GPIO_PIN_1, GPIO_PIN_SET);

  /* Configure the GPIO Display Enable pin */
  gpio_init_structure.Pin   = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOK , &gpio_init_structure);
  HAL_GPIO_WritePin(GPIOK , GPIO_PIN_7, GPIO_PIN_SET);

  /*** LTDC Pins configuration ***/
  /* GPIOI configuration */
  gpio_init_structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* GPIOJ configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

  /* GPIOK configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;

  /* Configure LTCD clock*/
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
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);

  /* Set LTDC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0xF, 0);

  /* Enable LTDC Interrupt */
  HAL_NVIC_EnableIRQ(LTDC_IRQn);


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

