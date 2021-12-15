/**
  ******************************************************************************
  * @file    SAI/SAI_AudioPlayback/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h745i_discovery.h"
#include "stm32h745i_discovery_bus.h"
#include "stm32h745i_discovery_audio.h"
#include "audio.h"
#include "../Components/wm8994/wm8994.h"
#include "pdm2pcm_glo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

/* SAI PDM input definitions */
/* SAI input peripheral configuration defines */
#define AUDIO_IN_SAI_PDMx                           SAI4_Block_A
#define AUDIO_IN_SAI_PDMx_CLK_ENABLE()              __HAL_RCC_SAI4_CLK_ENABLE()
#define AUDIO_IN_SAI_PDMx_CLK_DISABLE()             __HAL_RCC_SAI4_CLK_DISABLE()

#define AUDIO_IN_SAI_PDMx_CLK_IN_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_IN_SAI_PDMx_CLK_IN_PIN                GPIO_PIN_5
#define AUDIO_IN_SAI_PDMx_CLK_IN_PORT               GPIOE
#define AUDIO_IN_SAI_PDMx_DATA_IN_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_IN_SAI_PDMx_DATA_IN_PIN               GPIO_PIN_4
#define AUDIO_IN_SAI_PDMx_DATA_IN_PORT              GPIOE
#define AUDIO_IN_SAI_PDMx_DATA_CLK_AF               GPIO_AF10_SAI4
#define AUDIO_IN_SAI_PDMx_IRQHandler                SAI4_IRQHandler
#define AUDIO_IN_SAI_PDMx_IRQ                       SAI4_IRQn

/* SAI PDM DMA Stream definitions */
#define AUDIO_IN_SAI_PDMx_DMAx_CLK_ENABLE()         __HAL_RCC_BDMA_CLK_ENABLE()
#define AUDIO_IN_SAI_PDMx_DMAx_STREAM               BDMA_Channel1
#define AUDIO_IN_SAI_PDMx_DMAx_REQUEST              BDMA_REQUEST_SAI4_A
#define AUDIO_IN_SAI_PDMx_DMAx_IRQ                  BDMA_Channel1_IRQn
#define AUDIO_IN_SAI_PDMx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_IN_SAI_PDMx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD
#define AUDIO_IN_SAI_PDMx_DMAx_IRQHandler           BDMA_Channel1_IRQHandler

/* SAI output definitions */
/* SAI output peripheral configuration defines */
#define AUDIO_OUT_SAIx                           SAI2_Block_A
#define AUDIO_OUT_SAIx_CLK_ENABLE()              __HAL_RCC_SAI2_CLK_ENABLE()
#define AUDIO_OUT_SAIx_CLK_DISABLE()             __HAL_RCC_SAI2_CLK_DISABLE()
#define AUDIO_OUT_SAIx_AF                        GPIO_AF10_SAI2

#define AUDIO_OUT_SAIx_SD_FS_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define AUDIO_OUT_SAIx_SD_FS_SCK_GPIO_PORT       GPIOI
#define AUDIO_OUT_SAIx_FS_PIN                    GPIO_PIN_7
#define AUDIO_OUT_SAIx_SCK_PIN                   GPIO_PIN_5
#define AUDIO_OUT_SAIx_SD_PIN                    GPIO_PIN_6
#define AUDIO_OUT_SAIx_MCLK_ENABLE()             __HAL_RCC_GPIOI_CLK_ENABLE()
#define AUDIO_OUT_SAIx_MCLK_GPIO_PORT            GPIOI
#define AUDIO_OUT_SAIx_MCLK_PIN                  GPIO_PIN_4

/* SAI DMA Stream definitions */
#define AUDIO_OUT_SAIx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_OUT_SAIx_DMAx_STREAM               DMA2_Stream1
#define AUDIO_OUT_SAIx_DMAx_REQUEST              DMA_REQUEST_SAI2_A
#define AUDIO_OUT_SAIx_DMAx_IRQ                  DMA2_Stream1_IRQn
#define AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD
#define AUDIO_OUT_SAIx_DMAx_IRQHandler           DMA2_Stream1_IRQHandler

/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
#define AUDIO_IRQ_PREPRIO                    ((uint32_t)0x0E)

#endif /* __MAIN_H */

