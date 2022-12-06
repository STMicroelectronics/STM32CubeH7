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
#include "stm32h735g_discovery.h"
#include "stm32h735g_discovery_bus.h"
#include "audio.h"
#include "../Components/wm8994/wm8994.h"

/* Exported types ------------------------------------------------------------*/

#define DFSDM_MIC_NUMBER                 2
#define AUDIO_I2C_ADDRESS                0x34U
#define Audio_Volume                     70

/* SAI peripheral configuration defines */
#define AUDIO_OUT_SAIx                           SAI1_Block_B
#define AUDIO_OUT_SAIx_CLK_ENABLE()              __HAL_RCC_SAI1_CLK_ENABLE()
#define AUDIO_OUT_SAIx_CLK_DISABLE()             __HAL_RCC_SAI1_CLK_DISABLE()

#define AUDIO_OUT_SAIx_MCLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_OUT_SAIx_MCLK_GPIO_PORT            GPIOF
#define AUDIO_OUT_SAIx_MCLK_PIN                  GPIO_PIN_7
#define AUDIO_OUT_SAIx_MCLK_AF                   GPIO_AF6_SAI1

#define AUDIO_OUT_SAIx_SCK_ENABLE()              __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_OUT_SAIx_SCK_GPIO_PORT             GPIOF
#define AUDIO_OUT_SAIx_SCK_PIN                   GPIO_PIN_8
#define AUDIO_OUT_SAIx_SCK_AF                    GPIO_AF6_SAI1

#define AUDIO_OUT_SAIx_SD_ENABLE()               __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_OUT_SAIx_SD_GPIO_PORT              GPIOF
#define AUDIO_OUT_SAIx_SD_PIN                    GPIO_PIN_6
#define AUDIO_OUT_SAIx_SD_AF                     GPIO_AF6_SAI1

#define AUDIO_OUT_SAIx_FS_ENABLE()               __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_OUT_SAIx_FS_GPIO_PORT              GPIOF
#define AUDIO_OUT_SAIx_FS_PIN                    GPIO_PIN_9
#define AUDIO_OUT_SAIx_FS_AF                     GPIO_AF6_SAI1

/* SAI DMA Stream definitions */
#define AUDIO_OUT_SAIx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_OUT_SAIx_DMAx_STREAM               DMA2_Stream6
#define AUDIO_OUT_SAIx_DMAx_REQUEST              DMA_REQUEST_SAI1_B
#define AUDIO_OUT_SAIx_DMAx_IRQ                  DMA2_Stream6_IRQn
#define AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD
#define AUDIO_OUT_SAIx_DMAx_IRQHandler           DMA2_Stream6_IRQHandler



/* DFSDM Configuration defines */
#define AUDIO_DFSDMx_MIC1_CHANNEL                    DFSDM1_Channel2
#define AUDIO_DFSDMx_MIC2_CHANNEL                    DFSDM1_Channel1
#define AUDIO_DFSDMx_MIC1_CHANNEL_FOR_FILTER         DFSDM_CHANNEL_2
#define AUDIO_DFSDMx_MIC2_CHANNEL_FOR_FILTER         DFSDM_CHANNEL_1
#define AUDIO_DFSDMx_MIC1_FILTER                     DFSDM1_Filter0
#define AUDIO_DFSDMx_MIC2_FILTER                     DFSDM1_Filter1
#define AUDIO_DFSDM1_CLK_ENABLE()                    __HAL_RCC_DFSDM1_CLK_ENABLE()

/* DATIN for MIC1 (GPIOC_PIN_7) */
#define AUDIO_DFSDMx_DATIN_MIC1_PIN                  GPIO_PIN_7
#define AUDIO_DFSDMx_DATIN_MIC1_AF                   GPIO_AF3_DFSDM1
#define AUDIO_DFSDMx_DATIN_MIC1_GPIO_PORT            GPIOE
#define AUDIO_DFSDMx_DATIN_MIC1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()

#define AUDIO_DFSDMx_DMAx_MIC1_STREAM                DMA2_Stream7
#define AUDIO_DFSDMx_DMAx_MIC1_REQUEST               (uint32_t)DMA_REQUEST_DFSDM1_FLT0
#define AUDIO_DFSDMx_DMAx_MIC1_IRQ                   DMA2_Stream7_IRQn
#define AUDIO_DFSDM_DMAx_MIC1_IRQHandler             DMA2_Stream7_IRQHandler


/* DATIN for MIC2 (GPIOE_PIN_7)*/
#define AUDIO_DFSDMx_DATIN_MIC2_PIN                   GPIO_PIN_7
#define AUDIO_DFSDMx_DATIN_MIC2_AF                    GPIO_AF3_DFSDM1
#define AUDIO_DFSDMx_DATIN_MIC2_GPIO_PORT             GPIOE
#define AUDIO_DFSDMx_DATIN_MIC2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()

#define AUDIO_DFSDMx_DMAx_MIC2_STREAM                DMA2_Stream0
#define AUDIO_DFSDMx_DMAx_MIC2_REQUEST               (uint32_t)DMA_REQUEST_DFSDM1_FLT1
#define AUDIO_DFSDMx_DMAx_MIC2_IRQ                   DMA2_Stream0_IRQn
#define AUDIO_DFSDM_DMAx_MIC2_IRQHandler             DMA2_Stream0_IRQHandler

/* CKOUT for all mics connected to DFSDM1 (GPIOB_PIN_0)*/
#define AUDIO_DFSDM1_CKOUT_PIN                       GPIO_PIN_9
#define AUDIO_DFSDM1_CKOUT_AF                        GPIO_AF3_DFSDM1
#define AUDIO_DFSDM1_CKOUT_GPIO_PORT                 GPIOE
#define AUDIO_DFSDM1_CKOUT_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()



#define AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE           DMA_PDATAALIGN_WORD
#define AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE              DMA_MDATAALIGN_WORD
#define AUDIO_DFSDM1_DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()

/* Audio In states */
#define AUDIO_IN_STATE_RESET               0U
#define AUDIO_IN_STATE_RECORDING           1U
#define AUDIO_IN_STATE_STOP                2U
#define AUDIO_IN_STATE_PAUSE               3U

/* Audio bits per sample */
#define AUDIO_RESOLUTION_8B                 8U
#define AUDIO_RESOLUTION_16B                16U
#define AUDIO_RESOLUTION_24B                24U
#define AUDIO_RESOLUTION_32B                32U

/* Audio Out devices */
#define AUDIO_OUT_DEVICE_NONE               0U
#define AUDIO_OUT_DEVICE_SPEAKER            1U
#define AUDIO_OUT_DEVICE_HEADPHONE          2U
#define AUDIO_OUT_DEVICE_SPK_HP             3U
#define AUDIO_OUT_DEVICE_AUTO               4U
#define AUDIO_OUT_DEVICE_HDMI               5U

/** @defgroup STM32H735G_DISCO_AUDIO_Exported_Macros AUDIO Exported Macros
  * @{
  */
#define POS_VAL(VAL)                  (POSITION_VAL(VAL) - 4U)
#define VOLUME_OUT_CONVERT(Volume)    (((Volume) > 100)? 63:((uint8_t)(((Volume) * 63) / 100)))


typedef struct
{
  uint32_t                    Instance;            /* Audio IN instance              */
  uint32_t                    Device;              /* Audio IN device to be used     */
  uint32_t                    SampleRate;          /* Audio IN Sample rate           */
  uint32_t                    BitsPerSample;       /* Audio IN Sample resolution     */
  uint32_t                    ChannelsNbr;         /* Audio IN number of channel     */
  uint16_t                    *pBuff;              /* Audio IN record buffer         */
  uint8_t                     **pMultiBuff;        /* Audio IN multi-buffer          */
  uint32_t                    Size;                /* Audio IN record buffer size    */
  uint32_t                    Volume;              /* Audio IN volume                */
  uint32_t                    State;               /* Audio IN State                 */
  uint32_t                    IsMultiBuff;         /* Audio IN multi-buffer usage    */
  uint32_t                    IsMspCallbacksValid; /* Is Msp Callbacks registered     */
}AUDIO_IN_Ctx_t;

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


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

