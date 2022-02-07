/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_audio.h"
#include "stm32h743i_eval_bus.h"
#include "audio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* SAI output definitions */
/* SAI output peripheral configuration defines */
/* SAI peripheral configuration defines */
#define AUDIO_SAIx                           SAI1_Block_A
#define AUDIO_SAIx_CLK_ENABLE()              __HAL_RCC_SAI1_CLK_ENABLE()

#define AUDIO_SAIx_FS_GPIO_PORT              GPIOE
#define AUDIO_SAIx_FS_AF                     GPIO_AF6_SAI1
#define AUDIO_SAIx_FS_PIN                    GPIO_PIN_4
#define AUDIO_SAIx_SCK_GPIO_PORT             GPIOE
#define AUDIO_SAIx_SCK_AF                    GPIO_AF6_SAI1
#define AUDIO_SAIx_SCK_PIN                   GPIO_PIN_5
#define AUDIO_SAIx_SD_GPIO_PORT              GPIOE
#define AUDIO_SAIx_SD_AF                     GPIO_AF6_SAI1
#define AUDIO_SAIx_SD_PIN                    GPIO_PIN_6
#define AUDIO_SAIx_MCLK_GPIO_PORT            GPIOE
#define AUDIO_SAIx_MCLK_AF                   GPIO_AF6_SAI1
#define AUDIO_SAIx_MCLK_PIN                  GPIO_PIN_2

#define AUDIO_SAIx_MCLK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_SAIx_SCK_ENABLE()              __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_SAIx_FS_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_SAIx_SD_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()

/* SAI DMA Stream definitions */
#define AUDIO_SAIx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_SAIx_DMAx_STREAM               DMA2_Stream2
#define AUDIO_SAIx_DMAx_REQUEST              DMA_REQUEST_SAI1_A
#define AUDIO_SAIx_DMAx_IRQ                  DMA2_Stream2_IRQn
#define AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_SAIx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD   
#define AUDIO_SAIx_DMAx_IRQHandler           DMA2_Stream2_IRQHandler 

/* DFSDM Configuration defines */
#define AUDIO_DFSDMx_RIGHT_CHANNEL      DFSDM_CHANNEL_0
#define AUDIO_DFSDMx_LEFT_CHANNEL       DFSDM_CHANNEL_1

#define AUDIO_DFSDMx_LEFT_FILTER        DFSDM1_Filter0
#define AUDIO_DFSDMx_RIGHT_FILTER       DFSDM1_Filter1

   
#define AUDIO_DFSDMx_CLK_ENABLE()                       __HAL_RCC_DFSDM1_CLK_ENABLE()
#define AUDIO_DFSDMx_CKOUT_PIN                          GPIO_PIN_2
#define AUDIO_DFSDMx_CKOUT_DMIC_GPIO_PORT               GPIOC    
#define AUDIO_DFSDMx_CKOUT_DMIC_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_DFSDMx_DMIC_DATIN_PIN                     GPIO_PIN_3
#define AUDIO_DFSDMx_DMIC_DATIN_GPIO_PORT               GPIOC
#define AUDIO_DFSDMx_DMIC_DATIN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_DFSDMx_DMIC_DATIN_AF                      GPIO_AF3_DFSDM1
#define AUDIO_DFSDMx_CKOUT_AF                           GPIO_AF6_DFSDM1
    
/* DFSDM DMA Right and Left channels definitions */
#define AUDIO_DFSDMx_DMAx_CLK_ENABLE()          __HAL_RCC_DMA2_CLK_ENABLE()

#define AUDIO_DFSDMx_DMAx_LEFT_REQUEST          DMA_REQUEST_DFSDM1_FLT0
#define AUDIO_DFSDMx_DMAx_RIGHT_REQUEST         DMA_REQUEST_DFSDM1_FLT1

#define AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE      DMA_PDATAALIGN_WORD
#define AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE         DMA_MDATAALIGN_WORD

#define AUDIO_DFSDMx_DMAx_LEFT_STREAM           DMA2_Stream1
#define AUDIO_DFSDMx_DMAx_LEFT_IRQ              DMA2_Stream1_IRQn   
#define AUDIO_DFSDMx_DMAx_LEFT_IRQHandler       DMA2_Stream1_IRQHandler

#define AUDIO_DFSDMx_DMAx_RIGHT_STREAM          DMA2_Stream0
#define AUDIO_DFSDMx_DMAx_RIGHT_IRQ             DMA2_Stream0_IRQn
#define AUDIO_DFSDMx_DMAx_RIGHT_IRQHandler      DMA2_Stream0_IRQHandler

/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
#define AUDIO_IRQ_PREPRIO                    ((uint32_t)0x0E)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

