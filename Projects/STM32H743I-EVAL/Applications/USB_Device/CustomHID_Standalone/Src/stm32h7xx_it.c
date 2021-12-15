/**
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/Src/stm32h7xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------ */
#include "stm32h7xx_it.h"
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Applications
  * @{
  */

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define CURSOR_STEP     5
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
extern PCD_HandleTypeDef hpcd;
extern USBD_HandleTypeDef USBD_Device;
extern ADC_HandleTypeDef AdcHandle;
extern uint32_t ADCConvertedValue[8];
extern uint32_t ADC_Prev_ConvertedValue;
extern uint8_t SendBuffer[2];

/* Private function prototypes ----------------------------------------------- */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void);
#else
void OTG_HS_IRQHandler(void);
#endif
void EXTI15_10_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
/* Private functions --------------------------------------------------------- */

/******************************************************************************/
/* Cortex-M7 Processor Exceptions Handlers */
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/**
  * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
  HAL_PCD_IRQHandler(&hpcd);
}

/******************************************************************************/
/* STM32H7xx Peripherals Interrupt Handlers */
/* Add here the Interrupt Handler for the used peripheral(s) (PPP), for the */
/* available peripheral interrupt handler's name please refer to the startup */
/* file (startup_stm32h7xx.s).  */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/* void PPP_IRQHandler(void) { } */

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_TAMPER_PIN);
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
  SendBuffer[0] = ADC_REPORT_ID;

  SCB_InvalidateDCache_by_Addr(ADCConvertedValue, 4);

  if (abs((ADCConvertedValue[0] >> 4) - (ADC_Prev_ConvertedValue >> 4)) > 4)
  {
    SendBuffer[1] = (uint8_t) (ADCConvertedValue[0] >> 4);

    USBD_CUSTOM_HID_SendReport(&USBD_Device, SendBuffer, 2);

    ADC_Prev_ConvertedValue = ADCConvertedValue[0];
  }
}

/**
  * @}
  */

/**
  * @}
  */

