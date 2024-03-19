/**
  ******************************************************************************
  * @file    PSSI_Transmit_Receive_DMA/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup Templates
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

/**
  * @brief Initialize the PSSI MSP.
  * @param  hpssi Pointer to a PSSI_HandleTypeDef structure that contains
  *                the configuration information for the specified PSSI.
  * @retval None
  */
void HAL_PSSI_MspInit(PSSI_HandleTypeDef *hpssi)
{
  static DMA_HandleTypeDef hdma_handler_tx, hdma_handler_rx;
  GPIO_InitTypeDef gpio_init_structure;

  /*** Enable peripherals and GPIO clocks ***/
  /* Enable PSSI clock */
  __HAL_RCC_DCMI_PSSI_CLK_ENABLE();

  __HAL_RCC_DCMI_PSSI_FORCE_RESET();
  __HAL_RCC_DCMI_PSSI_RELEASE_RESET();
  
  /* Enable DMA clock */
  PSSI_DMA_TX_CLK_ENABLE();
  PSSI_DMA_RX_CLK_ENABLE();
  
  /* Enable GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*** Configure the GPIO ***/
  /* Configure PSSI GPIO as alternate function */
 /* Clock*/
  gpio_init_structure.Pin       = GPIO_PIN_6;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_PSSI;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  /*D0-->D4*/
   gpio_init_structure.Pin      = GPIO_PIN_6 | GPIO_PIN_7 |GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_PSSI;  
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);
  
  /*D5*/
  gpio_init_structure.Pin       = GPIO_PIN_3;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_PSSI;  
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
  
  /*D6-D7*/
  gpio_init_structure.Pin       = GPIO_PIN_8 | GPIO_PIN_9 ;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_PSSI;  
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);
  
  /*DE*/
  gpio_init_structure.Pin       = GPIO_PIN_4;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_PSSI;  
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  /*** Configure the DMA TX***/
  /* Set the parameters to be configured */
  hdma_handler_tx.Init.Request             = DMA_REQUEST_DCMI_PSSI;
  hdma_handler_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_handler_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_handler_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_handler_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_handler_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_handler_tx.Init.Mode                = DMA_NORMAL;
  hdma_handler_tx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_handler_tx.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdma_handler_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_handler_tx.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_handler_tx.Init.PeriphBurst         = DMA_PBURST_SINGLE; 

  hdma_handler_tx.Instance = PSSI_DMA_TX_STREAM;

  /* Associate the initialized DMA Tx handle to the PSSI handle */
  __HAL_LINKDMA(hpssi, hdmatx, hdma_handler_tx);

    /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(PSSI_DMA_TX_IRQ, 0x0F, 0);
  HAL_NVIC_EnableIRQ(PSSI_DMA_TX_IRQ);

  /* Configure the DMA stream */
  HAL_DMA_Init(hpssi->hdmatx);
  
  /*** Configure the DMA RX ***/
  /* Set the parameters to be configured */
  hdma_handler_rx.Init.Request             = DMA_REQUEST_DCMI_PSSI;
  hdma_handler_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_handler_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_handler_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_handler_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_handler_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_handler_rx.Init.Mode                = DMA_NORMAL;
  hdma_handler_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_handler_rx.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdma_handler_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_handler_rx.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_handler_rx.Init.PeriphBurst         = DMA_PBURST_SINGLE; 

  hdma_handler_rx.Instance = PSSI_DMA_RX_STREAM;

  /* Associate the initialized DMA Rx handle to the PSSI handle */
  __HAL_LINKDMA(hpssi, hdmarx, hdma_handler_rx);
  
  /*** Configure the NVIC for PSSI and DMA ***/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(PSSI_DMA_RX_IRQ, 0x0F, 0);
  HAL_NVIC_EnableIRQ(PSSI_DMA_RX_IRQ);
  /* Configure the DMA stream */
  HAL_DMA_Init(hpssi->hdmarx);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /* Enable GPIO Channels Clock */
  TIMx_CHANNEL_GPIO_PORT();

  /* Configure PE.09 (TIM1_Channel1) in output, push-pull, alternate function mode */
  /* Common configuration for the channel */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL1;
  
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL1, &GPIO_InitStruct); 
}

/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspInit(void)
{*/
/*}*/

/**
  * @brief  DeInitializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspDeInit(void)
{*/
/*}*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

