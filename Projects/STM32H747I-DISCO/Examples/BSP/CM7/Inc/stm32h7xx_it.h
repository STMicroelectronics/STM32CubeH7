/**
  ******************************************************************************
  * @file    BSP/CM7/Inc/stm32h7xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers for Cortex-M7.
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
#ifndef __STM32H7xx_IT_H
#define __STM32H7xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void DMA2_Stream6_IRQHandler(void);
void DMA2D_IRQHandler(void);
void BSP_LCD_LTDC_IRQHandler(void);
void BSP_LCD_LTDC_ER_IRQHandler(void);
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void);
void AUDIO_IN_SAI_PDMx_DMAx_IRQHandler(void);
void MDMA_IRQHandler(void);
void SDMMC1_IRQHandler(void);
void DCMI_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void AUDIO_IN_SAIx_DMAx_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#endif /* __STM32H7xx_IT_H */

