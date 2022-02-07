/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESmodes/Src/stm32h7xx_hal_msp.c
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
extern CRYP_HandleTypeDef hcryp;
extern DMA_HandleTypeDef hdmaIn;
extern DMA_HandleTypeDef hdmaOut;   
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief CRYP MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripherals clock enable
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
  /* Enable CRYP clock */
  __HAL_RCC_CRYP_CLK_ENABLE();
  
  /* Force the CRYP Peripheral Clock Reset */  
  __HAL_RCC_CRYP_FORCE_RESET(); 
  
  /* Release the CRYP Peripheral Clock Reset */  
  __HAL_RCC_CRYP_RELEASE_RESET();
 /* Enable and set CRYP Interrupt to the highest priority */
  HAL_NVIC_SetPriority(CRYP_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(CRYP_IRQn);
  
  /* Enable DMA2 clocks */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA2_FORCE_RESET();
  __HAL_RCC_DMA2_RELEASE_RESET();
  
  /***************** Configure common DMA In parameters ***********************/
  hdmaIn.Init.Request             = DMA_REQUEST_CRYP_IN;
  hdmaIn.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdmaIn.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdmaIn.Init.MemInc              = DMA_MINC_ENABLE;
  hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaIn.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdmaIn.Init.Mode                = DMA_NORMAL;
  hdmaIn.Init.Priority            = DMA_PRIORITY_HIGH;
  hdmaIn.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdmaIn.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdmaIn.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdmaIn.Init.PeriphBurst         = DMA_PBURST_SINGLE;
  hdmaIn.Instance = DMA2_Stream6;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hdmain, hdmaIn);
  
  /* De-initialize the Stream for new transfer */
  HAL_DMA_DeInit(hcryp->hdmain);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(hcryp->hdmain);      
  
  /* NVIC configuration for DMA Input data interrupt */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  
  /***************** Configure common DMA Out parameters **********************/
  hdmaOut.Init.Request             = DMA_REQUEST_CRYP_OUT;
  hdmaOut.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdmaOut.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdmaOut.Init.MemInc              = DMA_MINC_ENABLE;
  hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaOut.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdmaOut.Init.Mode                = DMA_NORMAL;
  hdmaOut.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  hdmaOut.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdmaOut.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdmaOut.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdmaOut.Init.PeriphBurst         = DMA_PBURST_SINGLE;
  hdmaOut.Instance = DMA2_Stream5;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hdmaout, hdmaOut);
  
  /* Deinitialize the Stream for new processing */
  HAL_DMA_DeInit(&hdmaOut);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(&hdmaOut);
  
  /* NVIC configuration for DMA output data interrupt */
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);  
}

/**
  * @brief CRYP MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripherals clock
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Disable CRYP clock */
  __HAL_RCC_CRYP_CLK_DISABLE();
  /* Disable DMA2 clocks */
  __HAL_RCC_DMA2_CLK_DISABLE();  
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

