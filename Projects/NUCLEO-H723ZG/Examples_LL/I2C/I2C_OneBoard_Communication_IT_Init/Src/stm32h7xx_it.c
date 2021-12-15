/** 
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_IT_Init/Src/stm32h7xx_it.c
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

/** @addtogroup I2C_OneBoard_Communication_IT_Init
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

    /* Manage code in main.c.*/
    UserButton_Callback(); 
  }
}

/**
  * Brief   This function handles I2C1 (Slave) event interrupt request.
  * Param   None
  * Retval  None
  */
void I2C1_EV_IRQHandler(void)
{
  /* Check ADDR flag value in ISR register */
  if(LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
    /* Verify the Address Match with the OWN Slave address */
    if(LL_I2C_GetAddressMatchCode(I2C1) == SLAVE_OWN_ADDRESS)
    {
      /* Verify the transfer direction, a read direction, Slave enters transmitter mode */
      if(LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_READ)
      {
        /* Clear ADDR flag value in ISR register */
        LL_I2C_ClearFlag_ADDR(I2C1);

        /* Enable Transmit Interrupt */
        LL_I2C_EnableIT_TX(I2C1);
      }
      else
      {
        /* Clear ADDR flag value in ISR register */
        LL_I2C_ClearFlag_ADDR(I2C1);

        /* Call Error function */
        Error_Callback();
      }
    }
    else
    {
      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C1);
        
      /* Call Error function */
      Error_Callback();
    }
  }
  /* Check NACK flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_NACK(I2C1))
  {
    /* End of Transfer */
    LL_I2C_ClearFlag_NACK(I2C1);
  }
  /* Check TXIS flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_TXIS(I2C1))
  {
    /* Call function Slave Ready to Transmit Callback */
    Slave_Ready_To_Transmit_Callback();
  }
  /* Check STOP flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_STOP(I2C1))
  {
    /* Clear STOP flag value in ISR register */
    LL_I2C_ClearFlag_STOP(I2C1);
    
    /* Check TXE flag value in ISR register */
    if(!LL_I2C_IsActiveFlag_TXE(I2C1))
    {
      /* Flush the TXDR register */
      LL_I2C_ClearFlag_TXE(I2C1);
    }
  }
  /* Check TXE flag value in ISR register */
  else if(!LL_I2C_IsActiveFlag_TXE(I2C1))
  {
    /* Do nothing */
    /* This Flag will be set by hardware when the TXDR register is empty */
    /* If needed, use LL_I2C_ClearFlag_TXE() interface to flush the TXDR register  */
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}

/**
  * Brief   This function handles I2C1 (Slave) error interrupt request.
  * Param   None
  * Retval  None
  */
void I2C1_ER_IRQHandler(void)
{
  /* Call Error function */
  Error_Callback();
}

/**
  * Brief   This function handles I2C4 (Master) interrupt request.
  * Param   None
  * Retval  None
  */
void I2C4_EV_IRQHandler(void)
{
  __IO uint32_t tmp;

  /* Check RXNE flag value in ISR register */
  if(LL_I2C_IsActiveFlag_RXNE(I2C4))
  {
    /* Call function Master Reception Callback */
    Master_Reception_Callback();
  }
  /* Check STOP flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_STOP(I2C4))
  {
    /* End of Transfer */
    LL_I2C_ClearFlag_STOP(I2C4);

    /* dummy read of the STOP flag to ensure previous write operation (clearing STOP flag) completed */
    tmp = LL_I2C_IsActiveFlag_STOP(I2C4);
    ((void)(tmp)); /* tmp is unused */

    /* Call function Master Complete Callback */
    Master_Complete_Callback();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}

/**
  * Brief   This function handles I2C4 (Master) error interrupt request.
  * Param   None
  * Retval  None
  */
void I2C4_ER_IRQHandler(void)
{
  /* Call Error function */
  Error_Callback();
}

/**
  * @}
  */

/**
  * @}
  */

