/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/Inc/main.h
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
#include "stm32h7b3i_eval.h"
#include "stm32h7b3i_eval_io.h"
#include "stm32h7b3i_eval_bus.h"
#include "cs42l51.h"
#include "audio.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  /* Filter parameters */
  DFSDM_Filter_TypeDef   *FilterInstance;
  uint32_t               RegularTrigger;
  uint32_t               SincOrder;
  uint32_t               Oversampling;
  /* Channel parameters */
  DFSDM_Channel_TypeDef *ChannelInstance;
  uint32_t              DigitalMicPins;
  uint32_t              DigitalMicType;
  uint32_t              Channel4Filter;
  uint32_t              ClockDivider;
  uint32_t              RightBitShift;
}MX_DFSDM_Config;

/* Exported constants --------------------------------------------------------*/
#define AUDIO_I2C_ADDRESS                            0x94U
/* SAI peripheral configuration defines */
#define AUDIO_OUT_SAIx                               SAI1_Block_A
#define AUDIO_OUT_SAIx_CLK_ENABLE()                  __HAL_RCC_SAI1_CLK_ENABLE()
#define AUDIO_OUT_SAIx_CLK_DISABLE()                 __HAL_RCC_SAI1_CLK_DISABLE()

#define AUDIO_OUT_SAIx_MCLK_ENABLE()                 __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_OUT_SAIx_MCLK_GPIO_PORT                GPIOG
#define AUDIO_OUT_SAIx_MCLK_PIN                      GPIO_PIN_7
#define AUDIO_OUT_SAIx_MCLK_AF                       GPIO_AF6_SAI1

#define AUDIO_OUT_SAIx_SCK_ENABLE()                  __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_OUT_SAIx_SCK_GPIO_PORT                 GPIOE
#define AUDIO_OUT_SAIx_SCK_PIN                       GPIO_PIN_5
#define AUDIO_OUT_SAIx_SCK_AF                        GPIO_AF6_SAI1

#define AUDIO_OUT_SAIx_SD_ENABLE()                   __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_OUT_SAIx_SD_GPIO_PORT                  GPIOE
#define AUDIO_OUT_SAIx_SD_PIN                        GPIO_PIN_6
#define AUDIO_OUT_SAIx_SD_AF                         GPIO_AF6_SAI1

#define AUDIO_OUT_SAIx_FS_ENABLE()                   __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_OUT_SAIx_FS_GPIO_PORT                  GPIOE
#define AUDIO_OUT_SAIx_FS_PIN                        GPIO_PIN_4
#define AUDIO_OUT_SAIx_FS_AF                         GPIO_AF6_SAI1

/* SAI DMA Stream definitions */
#define AUDIO_OUT_SAIx_DMAx_CLK_ENABLE()             __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_OUT_SAIx_DMAx_STREAM                   DMA2_Stream6
#define AUDIO_OUT_SAIx_DMAx_REQUEST                  DMA_REQUEST_SAI1_A
#define AUDIO_OUT_SAIx_DMAx_IRQ                      DMA2_Stream6_IRQn
#define AUDIO_OUT_SAIx_DMAx_IRQHandler               DMA2_Stream6_IRQHandler

/* DFSDM Configuration defines */
#define AUDIO_DFSDMx_MIC1_CHANNEL                    DFSDM1_Channel3
#define AUDIO_DFSDMx_MIC2_CHANNEL                    DFSDM1_Channel2
#define AUDIO_DFSDMx_MIC1_CHANNEL_FOR_FILTER         DFSDM_CHANNEL_3
#define AUDIO_DFSDMx_MIC2_CHANNEL_FOR_FILTER         DFSDM_CHANNEL_2
#define AUDIO_DFSDMx_MIC1_FILTER                     DFSDM1_Filter0
#define AUDIO_DFSDMx_MIC2_FILTER                     DFSDM1_Filter1
#define AUDIO_DFSDM1_CLK_ENABLE()                    __HAL_RCC_DFSDM1_CLK_ENABLE()

/* DATIN for MIC1 (GPIOC_PIN_7) */
#define AUDIO_DFSDMx_DATIN_MIC1_PIN                  GPIO_PIN_7
#define AUDIO_DFSDMx_DATIN_MIC1_AF                   GPIO_AF4_DFSDM1
#define AUDIO_DFSDMx_DATIN_MIC1_GPIO_PORT            GPIOC
#define AUDIO_DFSDMx_DATIN_MIC1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

/* CKOUT for all mics connected to DFSDM1 (GPIOB_PIN_0)*/
#define AUDIO_DFSDM1_CKOUT_PIN                       GPIO_PIN_0
#define AUDIO_DFSDM1_CKOUT_AF                        GPIO_AF6_DFSDM1
#define AUDIO_DFSDM1_CKOUT_GPIO_PORT                 GPIOB
#define AUDIO_DFSDM1_CKOUT_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

#define AUDIO_DFSDMx_DMAx_MIC1_STREAM                DMA2_Stream7
#define AUDIO_DFSDMx_DMAx_MIC1_REQUEST               (uint32_t)DMA_REQUEST_DFSDM1_FLT0
#define AUDIO_DFSDMx_DMAx_MIC1_IRQ                   DMA2_Stream7_IRQn
#define AUDIO_DFSDM_DMAx_MIC1_IRQHandler             DMA2_Stream7_IRQHandler

#define AUDIO_DFSDMx_DMAx_MIC2_STREAM                DMA2_Stream0
#define AUDIO_DFSDMx_DMAx_MIC2_REQUEST               (uint32_t)DMA_REQUEST_DFSDM1_FLT1
#define AUDIO_DFSDMx_DMAx_MIC2_IRQ                   DMA2_Stream0_IRQn
#define AUDIO_DFSDM_DMAx_MIC2_IRQHandler             DMA2_Stream0_IRQHandler

#define AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE           DMA_PDATAALIGN_WORD
#define AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE              DMA_MDATAALIGN_WORD
#define AUDIO_DFSDM1_DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

