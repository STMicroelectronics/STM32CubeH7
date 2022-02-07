/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/Src/stm32h7xx_hal_msp.c
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
  * @brief  SAI MSP Init.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  gpio_init_structure;
  static DMA_HandleTypeDef hdma_sai_tx;
  /* Enable SAI clock */
  AUDIO_OUT_SAIx_CLK_ENABLE();

  /* Enable GPIO clock */
  AUDIO_OUT_SAIx_MCLK_ENABLE();
  AUDIO_OUT_SAIx_SCK_ENABLE();
  AUDIO_OUT_SAIx_SD_ENABLE();
  AUDIO_OUT_SAIx_FS_ENABLE();

  /* CODEC_SAI pins configuration: FS, SCK, MCK and SD pins ------------------*/
  gpio_init_structure.Pin = AUDIO_OUT_SAIx_FS_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = AUDIO_OUT_SAIx_FS_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAIx_FS_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = AUDIO_OUT_SAIx_SCK_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_SAIx_SCK_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAIx_SCK_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin =  AUDIO_OUT_SAIx_SD_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_SAIx_SD_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAIx_SD_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = AUDIO_OUT_SAIx_MCLK_PIN;
  gpio_init_structure.Alternate = AUDIO_OUT_SAIx_MCLK_AF;
  HAL_GPIO_Init(AUDIO_OUT_SAIx_MCLK_GPIO_PORT, &gpio_init_structure);

  /* Enable the DMA clock */
  AUDIO_OUT_SAIx_DMAx_CLK_ENABLE();

  /* Configure the hdma_saiTx handle parameters */
  hdma_sai_tx.Init.Request             = AUDIO_OUT_SAIx_DMAx_REQUEST;
  hdma_sai_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_sai_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_sai_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_sai_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_sai_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  hdma_sai_tx.Init.Mode                = DMA_CIRCULAR;
  hdma_sai_tx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_sai_tx.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdma_sai_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_sai_tx.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_sai_tx.Init.PeriphBurst         = DMA_PBURST_SINGLE;

  hdma_sai_tx.Instance = AUDIO_OUT_SAIx_DMAx_STREAM;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsai, hdmatx, hdma_sai_tx);

  /* Deinitialize the Stream for new transfer */
  (void)HAL_DMA_DeInit(&hdma_sai_tx);

  /* Configure the DMA Stream */
  (void)HAL_DMA_Init(&hdma_sai_tx);

  /* SAI DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_OUT_SAIx_DMAx_IRQ, BSP_AUDIO_OUT_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(AUDIO_OUT_SAIx_DMAx_IRQ);
}

/**
  * @brief  Initialize the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdfsdm_channel);

 /* DFSDM pins configuration: DFSDM_CKOUT, DMIC_DATIN pins ------------------*/
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  /* Enable DFSDM clock */
  AUDIO_DFSDM1_CLK_ENABLE();

  /* Enable GPIO clock */
  AUDIO_DFSDM1_CKOUT_GPIO_CLK_ENABLE();
  AUDIO_DFSDMx_DATIN_MIC1_GPIO_CLK_ENABLE();
  AUDIO_DFSDMx_DATIN_MIC2_GPIO_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = AUDIO_DFSDM1_CKOUT_PIN;
  GPIO_InitStruct.Alternate = AUDIO_DFSDM1_CKOUT_AF;
  HAL_GPIO_Init(AUDIO_DFSDM1_CKOUT_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AUDIO_DFSDMx_DATIN_MIC1_PIN;
  GPIO_InitStruct.Alternate = AUDIO_DFSDMx_DATIN_MIC1_AF;
  HAL_GPIO_Init(AUDIO_DFSDMx_DATIN_MIC1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = AUDIO_DFSDMx_DATIN_MIC2_PIN;
  GPIO_InitStruct.Alternate = AUDIO_DFSDMx_DATIN_MIC2_AF;
  HAL_GPIO_Init(AUDIO_DFSDMx_DATIN_MIC2_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitialize the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspDeInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdfsdm_channel);

  /* DFSDM pins configuration: DFSDM_CKOUT, DMIC_DATIN pins ------------------*/
  GPIO_InitStruct.Pin = AUDIO_DFSDM1_CKOUT_PIN;
  HAL_GPIO_DeInit(AUDIO_DFSDM1_CKOUT_GPIO_PORT, GPIO_InitStruct.Pin);

  GPIO_InitStruct.Pin = AUDIO_DFSDMx_DATIN_MIC1_PIN;
  HAL_GPIO_DeInit(AUDIO_DFSDMx_DATIN_MIC1_GPIO_PORT, GPIO_InitStruct.Pin);
  
  GPIO_InitStruct.Pin = AUDIO_DFSDMx_DATIN_MIC2_PIN;
  HAL_GPIO_DeInit(AUDIO_DFSDMx_DATIN_MIC2_GPIO_PORT, GPIO_InitStruct.Pin);
}

/**
  * @brief  Initialize the DFSDM filter MSP.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  uint32_t i;
  static DMA_HandleTypeDef hDmaDfsdm[2];
  IRQn_Type AUDIO_DFSDM_DMAx_MIC_IRQHandler[2] = {AUDIO_DFSDMx_DMAx_MIC1_IRQ, AUDIO_DFSDMx_DMAx_MIC2_IRQ};
  DMA_Stream_TypeDef* AUDIO_DFSDMx_DMAx_MIC_STREAM[2] = {AUDIO_DFSDMx_DMAx_MIC1_STREAM, AUDIO_DFSDMx_DMAx_MIC2_STREAM};
  uint32_t AUDIO_DFSDMx_DMAx_MIC_REQUEST[2] = {AUDIO_DFSDMx_DMAx_MIC1_REQUEST, AUDIO_DFSDMx_DMAx_MIC2_REQUEST};

  AUDIO_DFSDM1_CLK_ENABLE();
  /* Enable the DMA clock */
  AUDIO_DFSDM1_DMAx_CLK_ENABLE();

  if(hdfsdm_filter->Instance == DFSDM1_Filter0)
  {
    i = 0;
  }
  else
  {
    i = 1;
  }

  /* Configure the hDmaDfsdm[i] handle parameters */
  hDmaDfsdm[i].Init.Request             = AUDIO_DFSDMx_DMAx_MIC_REQUEST[i];
  hDmaDfsdm[i].Instance                 = AUDIO_DFSDMx_DMAx_MIC_STREAM[i];
  hDmaDfsdm[i].Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hDmaDfsdm[i].Init.PeriphInc           = DMA_PINC_DISABLE;
  hDmaDfsdm[i].Init.MemInc              = DMA_MINC_ENABLE;
  hDmaDfsdm[i].Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hDmaDfsdm[i].Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hDmaDfsdm[i].Init.Mode                = DMA_CIRCULAR;
  hDmaDfsdm[i].Init.Priority            = DMA_PRIORITY_HIGH;
  hDmaDfsdm[i].Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  hDmaDfsdm[i].Init.MemBurst            = DMA_MBURST_SINGLE;
  hDmaDfsdm[i].Init.PeriphBurst         = DMA_PBURST_SINGLE;
  hDmaDfsdm[i].State                    = HAL_DMA_STATE_RESET;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hdfsdm_filter, hdmaReg, hDmaDfsdm[i]);

  /* Reset DMA handle state */
  __HAL_DMA_RESET_HANDLE_STATE(&hDmaDfsdm[i]);

  /* Configure the DMA Channel */
  (void)HAL_DMA_Init(&hDmaDfsdm[i]);

  /* DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_DFSDM_DMAx_MIC_IRQHandler[i], BSP_AUDIO_IN_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(AUDIO_DFSDM_DMAx_MIC_IRQHandler[i]);
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

