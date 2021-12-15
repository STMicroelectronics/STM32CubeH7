/**
  ******************************************************************************
  * @file    Core/Common/Src/stm32h7xx_hal_msp.c
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


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_14
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF4_USART1
#define USARTx_RX_PIN                    GPIO_PIN_15
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF4_USART1

#define RTC_CLOCK_SOURCE_LSI

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USART1 clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

}

/**
  * @brief JPEG MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - NVIC configuration for JPEG interrupt request enable
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
  static MDMA_HandleTypeDef   hmdmaIn;
  static MDMA_HandleTypeDef   hmdmaOut;

  /* Enable JPEG clock */
  __HAL_RCC_JPGDECEN_CLK_ENABLE();

  /* Enable MDMA clock */
  __HAL_RCC_MDMA_CLK_ENABLE();

  HAL_NVIC_SetPriority(JPEG_IRQn, 0x7, 0xF);
  HAL_NVIC_EnableIRQ(JPEG_IRQn);

  /* Input MDMA */
  /* Set the parameters to be configured */
  hmdmaIn.Init.Priority           = MDMA_PRIORITY_HIGH;
  hmdmaIn.Init.Endianness         = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdmaIn.Init.SourceInc          = MDMA_SRC_INC_BYTE;
  hmdmaIn.Init.DestinationInc     = MDMA_DEST_INC_DISABLE;
  hmdmaIn.Init.SourceDataSize     = MDMA_SRC_DATASIZE_BYTE;
  hmdmaIn.Init.DestDataSize       = MDMA_DEST_DATASIZE_WORD;
  hmdmaIn.Init.DataAlignment      = MDMA_DATAALIGN_PACKENABLE;
  hmdmaIn.Init.SourceBurst        = MDMA_SOURCE_BURST_32BEATS;
  hmdmaIn.Init.DestBurst          = MDMA_DEST_BURST_16BEATS;
  hmdmaIn.Init.SourceBlockAddressOffset = 0;
  hmdmaIn.Init.DestBlockAddressOffset = 0;

  /*Using JPEG Input FIFO Threshold as a trigger for the MDMA*/
  hmdmaIn.Init.Request = MDMA_REQUEST_JPEG_INFIFO_TH; /* Set the MDMA HW trigger to JPEG Input FIFO Threshold flag*/
  hmdmaIn.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
  hmdmaIn.Init.BufferTransferLength = 32; /*Set the MDMA buffer size to the JPEG FIFO threshold size i.e 32 bytes (8 words)*/

  hmdmaIn.Instance = MDMA_Channel1;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hjpeg, hdmain, hmdmaIn);

  /* DeInitialize the DMA Stream */
  HAL_MDMA_DeInit(&hmdmaIn);
  /* Initialize the DMA stream */
  HAL_MDMA_Init(&hmdmaIn);

  /* Output MDMA */
  /* Set the parameters to be configured */
  hmdmaOut.Init.Priority        = MDMA_PRIORITY_VERY_HIGH;
  hmdmaOut.Init.Endianness      = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdmaOut.Init.SourceInc       = MDMA_SRC_INC_DISABLE;
  hmdmaOut.Init.DestinationInc  = MDMA_DEST_INC_BYTE;
  hmdmaOut.Init.SourceDataSize  = MDMA_SRC_DATASIZE_WORD;
  hmdmaOut.Init.DestDataSize    = MDMA_DEST_DATASIZE_BYTE;
  hmdmaOut.Init.DataAlignment   = MDMA_DATAALIGN_PACKENABLE;
  hmdmaOut.Init.SourceBurst     = MDMA_SOURCE_BURST_32BEATS;
  hmdmaOut.Init.DestBurst       = MDMA_DEST_BURST_32BEATS;
  hmdmaOut.Init.SourceBlockAddressOffset = 0;
  hmdmaOut.Init.DestBlockAddressOffset = 0;

  /*Using JPEG Output FIFO Threshold as a trigger for the MDMA*/
  hmdmaOut.Init.Request              = MDMA_REQUEST_JPEG_OUTFIFO_TH; /* Set the MDMA HW trigger to JPEG Output FIFO Threshold flag*/
  hmdmaOut.Init.TransferTriggerMode  = MDMA_BUFFER_TRANSFER;
  hmdmaOut.Init.BufferTransferLength = 32; /*Set the MDMA buffer size to the JPEG FIFO threshold size i.e 32 bytes (8 words)*/

  hmdmaOut.Instance = MDMA_Channel0;
  /* DeInitialize the DMA Stream */
  HAL_MDMA_DeInit(&hmdmaOut);
  /* Initialize the DMA stream */
  HAL_MDMA_Init(&hmdmaOut);

  /* Associate the DMA handle */
  __HAL_LINKDMA(hjpeg, hdmaout, hmdmaOut);

  HAL_NVIC_SetPriority(MDMA_IRQn, 0x8, 0xF);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);
}

void HAL_JPEG_MspDeInit(JPEG_HandleTypeDef *hjpeg)
{
  HAL_NVIC_DisableIRQ(JPEG_IRQn);
  HAL_NVIC_DisableIRQ(MDMA_IRQn);

  /* DeInitialize the MDMA Stream */
  HAL_MDMA_DeInit(hjpeg->hdmain);

  /* DeInitialize the MDMA Stream */
  HAL_MDMA_DeInit(hjpeg->hdmaout);
}

