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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA2D_HandleTypeDef hdma2d;
extern TIM_HandleTypeDef        TimHandle;
extern LTDC_HandleTypeDef hltdc;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void LCD_DeInitSequence(void);
static void LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc);
static void DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d);
/**
* @brief  Jump to Sub demo application
* @param  Address  : Address where the sub demonstration code is located
* @retval None
*/
void BSP_SetSubDemoAddress(uint32_t JumpAddress)
{
  /* Store the address of the Sub Demo binary */
  HAL_PWR_EnableBkUpAccess();
  WRITE_REG(BKP_REG_SUBDEMO_ADDRESS, (uint32_t) JumpAddress);
  HAL_PWR_DisableBkUpAccess();

  HAL_Delay(100);

  LCD_DeInitSequence();
  LTDC_MspDeInit(&hltdc);
  DMA2D_MspDeInit(&hdma2d);

  hltdc.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc);
  HAL_DMA2D_DeInit(&hdma2d);
  
  HAL_Delay(200);

  /* Disable the MPU */
  HAL_MPU_Disable();
  
  /* Disable and Invalidate I-Cache */
  SCB_DisableICache();
  SCB_InvalidateICache();
  
  /* Disable, Clean and Invalidate D-Cache */
  SCB_DisableDCache();
  SCB_CleanInvalidateDCache();
  
  /* Disable the TIM6 global Interrupt */
  HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
  HAL_TIM_Base_DeInit(&TimHandle);
  
  /* Do Software Reset */
  HAL_NVIC_SystemReset();
}

/**
  * @brief  DeInitializes LCD GPIO special pins MSP.
  * @param  None
  * @retval None
  */
static void LCD_DeInitSequence(void)
{
  GPIO_InitTypeDef  gpio_init_structure;
  /* LCD_DISP GPIO configuration */

  /* LCD_RESET GPIO deactivation */
  gpio_init_structure.Pin       = GPIO_PIN_7;
  HAL_GPIO_DeInit(GPIOK, gpio_init_structure.Pin);

  /* LCD_BL_CTRL GPIO configuration */
  /* LCD_BL_CTRL GPIO deactivation */
  gpio_init_structure.Pin       = GPIO_PIN_1;
  HAL_GPIO_DeInit(GPIOA, gpio_init_structure.Pin);
}

/**
  * @brief  De-Initializes the BSP LTDC Msp
  * @param  hltdc  LTDC handle
  * @retval None
  */
static void LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)
{
  GPIO_InitTypeDef  gpio_init_structure;

  if(hltdc->Instance == LTDC)
  {
    /* LTDC Pins deactivation */
    /* GPIOI deactivation */
    gpio_init_structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_DeInit(GPIOI, gpio_init_structure.Pin);

    /* GPIOJ deactivation */
    gpio_init_structure.Pin       = GPIO_PIN_All;
    HAL_GPIO_DeInit(GPIOJ, gpio_init_structure.Pin);
    /* GPIOK deactivation */
    gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_DeInit(GPIOK, gpio_init_structure.Pin);

    /** Force and let in reset state LTDC */
    __HAL_RCC_LTDC_FORCE_RESET();

    /** Disable the LTDC */
    __HAL_RCC_LTDC_CLK_DISABLE();
  }
}

/**
  * @brief  De-Initializes the BSP DMA2D Msp
  * @param  hdma2d  DMA2D handle
  * @retval None
  */
static void DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)
{
  if(hdma2d->Instance == DMA2D)
  {
    /** Disable IRQ of DMA2D IP */
    HAL_NVIC_DisableIRQ(DMA2D_IRQn);

    /** Force and let in reset state DMA2D */
    __HAL_RCC_DMA2D_FORCE_RESET();

    /** Disable the DMA2D */
    __HAL_RCC_DMA2D_CLK_DISABLE();
  }
}

