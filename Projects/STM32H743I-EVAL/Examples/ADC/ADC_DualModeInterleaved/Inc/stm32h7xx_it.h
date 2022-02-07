/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/Inc/stm32h7xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
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
void EXTI15_10_IRQHandler(void);
void ADCx_IRQHandler(void);
/* Note: On STM32H7xx, ADC2 IRQ handler is the same as ADC1. */
void ADCx_DMA_IRQHandler(void);
void ADCy_DMA_IRQHandler(void);

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
void TIM6_DAC_IRQHandler(void);
void DACx_CHANNELa_DMA_IRQHandler(void);
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


#ifdef __cplusplus
}
#endif

#endif /* __STM32H7xx_IT_H */

