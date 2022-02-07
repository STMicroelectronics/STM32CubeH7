/**
  ******************************************************************************
  * @file    Demonstration/Oscilloscope_SignalsGenerator/Src/stm32h7xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sig_gen.h"
#include "oscillo.h"
#include "stm32h7xx_it.h"

/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DAC_HandleTypeDef    DacHandle;
extern LTDC_HandleTypeDef   hlcd_ltdc;
extern DMA2D_HandleTypeDef  DMA2D_Handle;
extern ADC_HandleTypeDef                AdcHandle_Master;
extern ADC_HandleTypeDef                AdcHandle_Slave;
extern TIM_HandleTypeDef                TimHandle;
extern UART_HandleTypeDef               UartHandle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}



/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DACx_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(DacHandle.DMA_Handle2);
}

/**
  * @brief  This function handles DMA2D interrupt request.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&DMA2D_Handle);
}

/**
  * @brief  This function handles LTDC interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hlcd_ltdc);
}


void ADCx_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(AdcHandle_Master.DMA_Handle);
}
void ADCx_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&AdcHandle_Master);
  HAL_ADC_IRQHandler(&AdcHandle_Slave);
}
void USARTx_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
}
/**
  * @}
  */

/**
  * @}
  */

