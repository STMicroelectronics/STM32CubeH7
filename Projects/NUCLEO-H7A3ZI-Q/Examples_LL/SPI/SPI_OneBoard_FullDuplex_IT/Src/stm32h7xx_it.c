/**
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_OneBoard_FullDuplex_IT/Src/stm32h7xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_it.h"

/** @addtogroup STM32H7xx_LL_Examples
  * @{
  */

/** @addtogroup SPI_OneBoard_FullDuplex_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
}

/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles external lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void USER_BUTTON_IRQHANDLER(void)
{
  /* Manage Flags */
  if(LL_EXTI_IsActiveFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);

    /* Manage code in main.c */
    UserButton_Callback(); 
  }
}

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
    /* Check RXP flag value in ISR register */
    if(LL_SPI_IsActiveFlag_RXP(SPI1) && LL_SPI_IsEnabledIT_RXP(SPI1))
    {
      /* Call function Reception Callback */
      SPI1_Rx_Callback();
      return;
    }
    /* Check TXP flag value in ISR register */
    else if((LL_SPI_IsActiveFlag_TXP(SPI1) && LL_SPI_IsEnabledIT_TXP(SPI1)))
    {
      /* Call function Reception Callback */
      SPI1_Tx_Callback();
      return;
    }
    /* Check CRCERR/UDR flag value in ISR register */
    else if(LL_SPI_IsActiveFlag_OVR(SPI1) || LL_SPI_IsActiveFlag_UDR(SPI1))
    {
      /* Call Error function */
      SPI1_TransferError_Callback();
    }
    /* Check EOT flag value in ISR register */
    else if(LL_SPI_IsActiveFlag_EOT(SPI1) && LL_SPI_IsEnabledIT_EOT(SPI1))
    {
      /* Call function Reception Callback */
      SPI1_EOT_Callback();
      return;
    }
}

/**
  * @brief  This function handles SPI6 interrupt request.
  * @param  None
  * @retval None
  */
void SPI6_IRQHandler(void)
{
    /* Check RXP flag value in ISR register */
    if(LL_SPI_IsActiveFlag_RXP(SPI6) && LL_SPI_IsEnabledIT_RXP(SPI6))
    {
      /* Call function Reception Callback */
      SPI6_Rx_Callback();
      return;
    }
    /* Check TXP flag value in ISR register */
    else if((LL_SPI_IsActiveFlag_TXP(SPI6) && LL_SPI_IsEnabledIT_TXP(SPI6)))
    {
      /* Call function Reception Callback */
      SPI6_Tx_Callback();
      return;
    }
    /* Check CRCERR/UDR flag value in ISR register */
    else if(LL_SPI_IsActiveFlag_OVR(SPI6) || LL_SPI_IsActiveFlag_UDR(SPI6))
    {
      /* Call Error function */
      SPI6_TransferError_Callback();
    }
    /* Check EOT flag value in ISR register */
    else if(LL_SPI_IsActiveFlag_EOT(SPI6) && LL_SPI_IsEnabledIT_EOT(SPI6))
    {
      /* Call function Reception Callback */
      SPI6_EOT_Callback();
      return;
    }
}

/**
  * @}
  */

/**
  * @}
  */

