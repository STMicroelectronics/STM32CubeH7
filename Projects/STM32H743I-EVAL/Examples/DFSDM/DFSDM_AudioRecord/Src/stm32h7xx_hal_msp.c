/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA_HandleTypeDef        SaiInputDmaHandle;
DMA_HandleTypeDef        SaiOutputDmaHandle;
DMA_HandleTypeDef        DfsdmLeftDmaHandle;
DMA_HandleTypeDef        DfsdmRightDmaHandle;

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
  GPIO_InitTypeDef  GPIO_Init;  
 
  /* Enable SAI clock */
  AUDIO_OUT_SAIx_CLK_ENABLE();

  /* Configure GPIOs used for SAI1 */
  AUDIO_SAIx_MCLK_ENABLE();
  AUDIO_SAIx_SCK_ENABLE();
  AUDIO_SAIx_FS_ENABLE();
  AUDIO_SAIx_SD_ENABLE();

  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_Init.Alternate = AUDIO_SAIx_FS_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_FS_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_FS_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SCK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SCK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SCK_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SD_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SD_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SD_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_MCLK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_MCLK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_MCLK_GPIO_PORT, &GPIO_Init);
  
  /* Enable the DMA clock */
  AUDIO_SAIx_DMAx_CLK_ENABLE();

  /* Configure the hdma_sai_tx handle parameters */   
  SaiOutputDmaHandle.Init.Request             = AUDIO_SAIx_DMAx_REQUEST;
  SaiOutputDmaHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  SaiOutputDmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  SaiOutputDmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  SaiOutputDmaHandle.Init.PeriphDataAlignment = AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE;
  SaiOutputDmaHandle.Init.MemDataAlignment    = AUDIO_SAIx_DMAx_MEM_DATA_SIZE;
  SaiOutputDmaHandle.Init.Mode                = DMA_CIRCULAR;
  SaiOutputDmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
  SaiOutputDmaHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;         
  SaiOutputDmaHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  SaiOutputDmaHandle.Init.MemBurst            = DMA_MBURST_SINGLE;
  SaiOutputDmaHandle.Init.PeriphBurst         = DMA_PBURST_SINGLE;   

  SaiOutputDmaHandle.Instance = AUDIO_SAIx_DMAx_STREAM;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsai, hdmatx, SaiOutputDmaHandle);

  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&SaiOutputDmaHandle);

  /* Configure the DMA Stream */
  HAL_DMA_Init(&SaiOutputDmaHandle);

  /* SAI DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_SAIx_DMAx_IRQ, AUDIO_IRQ_PREPRIO, 0);
  HAL_NVIC_EnableIRQ(AUDIO_SAIx_DMAx_IRQ);
}


/**
  * @brief  Initialize the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  GPIO_InitTypeDef  GPIO_InitStruct;  
  
  /* Enable DFSDM clock */
  AUDIO_DFSDMx_CLK_ENABLE();
  
  /* Enable GPIO clock */
  AUDIO_DFSDMx_DMIC_DATIN_GPIO_CLK_ENABLE();
  AUDIO_DFSDMx_CKOUT_DMIC_GPIO_CLK_ENABLE();
  
  /* DFSDM pins configuration: DFSDM_CKOUT, DMIC_DATIN1 pins ------------------*/
  GPIO_InitStruct.Pin = AUDIO_DFSDMx_CKOUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = AUDIO_DFSDMx_CKOUT_AF;
  HAL_GPIO_Init(AUDIO_DFSDMx_CKOUT_DMIC_GPIO_PORT, &GPIO_InitStruct);
  
  /* DFSDM pin configuration: DMIC_DATIN1 pin --------------------------------*/
  GPIO_InitStruct.Pin = AUDIO_DFSDMx_DMIC_DATIN_PIN;
  GPIO_InitStruct.Alternate = AUDIO_DFSDMx_DMIC_DATIN_AF;
  HAL_GPIO_Init(AUDIO_DFSDMx_DMIC_DATIN_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitialize the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspDeInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  GPIO_InitTypeDef  GPIO_InitStruct;  
  
  /* DFSDM pin configuration: DMIC_DATIN1 pin --------------------------------*/
  GPIO_InitStruct.Pin = AUDIO_DFSDMx_CKOUT_PIN;
  HAL_GPIO_DeInit(AUDIO_DFSDMx_CKOUT_DMIC_GPIO_PORT, GPIO_InitStruct.Pin);
  GPIO_InitStruct.Pin = AUDIO_DFSDMx_DMIC_DATIN_PIN;
  HAL_GPIO_DeInit(AUDIO_DFSDMx_DMIC_DATIN_GPIO_PORT, GPIO_InitStruct.Pin);
}

/**
  * @brief  Initialize the DFSDM filter MSP.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{  
  /* Enable DFSDM clock */
  AUDIO_DFSDMx_CLK_ENABLE();
  
  /* Enable the DMA clock */
  AUDIO_DFSDMx_DMAx_CLK_ENABLE();
  
  if(hdfsdm_filter->Instance == AUDIO_DFSDMx_LEFT_FILTER)
  {
    /*********** Configure DMA stream for LEFT microphone *******************/
    DfsdmLeftDmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    DfsdmLeftDmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
    DfsdmLeftDmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
    DfsdmLeftDmaHandle.Init.PeriphDataAlignment = AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE;
    DfsdmLeftDmaHandle.Init.MemDataAlignment    = AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE;
    DfsdmLeftDmaHandle.Init.Mode                = DMA_CIRCULAR;
    DfsdmLeftDmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
    DfsdmLeftDmaHandle.Instance                 = AUDIO_DFSDMx_DMAx_LEFT_STREAM;
    DfsdmLeftDmaHandle.Init.Request             = AUDIO_DFSDMx_DMAx_LEFT_REQUEST; 
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hdfsdm_filter, hdmaReg, DfsdmLeftDmaHandle);
    
    /* Reset DMA handle state */
    __HAL_DMA_RESET_HANDLE_STATE(&DfsdmLeftDmaHandle);
    
    /* Configure the DMA Channel */
    HAL_DMA_Init(&DfsdmLeftDmaHandle);      
    
    /* DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_DFSDMx_DMAx_LEFT_IRQ, AUDIO_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(AUDIO_DFSDMx_DMAx_LEFT_IRQ);
  }
  
  if(hdfsdm_filter->Instance == AUDIO_DFSDMx_RIGHT_FILTER)
  {
    /*********** Configure DMA stream for RIGHT microphone ******************/
    DfsdmRightDmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    DfsdmRightDmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
    DfsdmRightDmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
    DfsdmRightDmaHandle.Init.PeriphDataAlignment = AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE;
    DfsdmRightDmaHandle.Init.MemDataAlignment    = AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE;
    DfsdmRightDmaHandle.Init.Mode                = DMA_CIRCULAR;
    DfsdmRightDmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;  
    DfsdmRightDmaHandle.Instance                 = AUDIO_DFSDMx_DMAx_RIGHT_STREAM;
    DfsdmRightDmaHandle.Init.Request             = AUDIO_DFSDMx_DMAx_RIGHT_REQUEST;
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hdfsdm_filter, hdmaReg, DfsdmRightDmaHandle);
    
    /* Reset DMA handle state */
    __HAL_DMA_RESET_HANDLE_STATE(&DfsdmRightDmaHandle);
    
    /* Configure the DMA Channel */
    HAL_DMA_Init(&DfsdmRightDmaHandle);      
    
    /* DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_DFSDMx_DMAx_RIGHT_IRQ, AUDIO_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(AUDIO_DFSDMx_DMAx_RIGHT_IRQ);
  }
}

/**
  * @brief  DeInitialize the DFSDM filter MSP.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterMspDeInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter->Instance == AUDIO_DFSDMx_LEFT_FILTER)
  {
    HAL_DMA_DeInit(&DfsdmLeftDmaHandle);
  }
  if(hdfsdm_filter->Instance == AUDIO_DFSDMx_RIGHT_FILTER)
  {
    HAL_DMA_DeInit(&DfsdmRightDmaHandle);
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

