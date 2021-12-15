/**
  ******************************************************************************
  * @file    BSP/CM7/Src/stm32h7xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines for Cortex-M7.
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
#include "main.h"
#include "stm32h7xx_it.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMAMUX_SYNC
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
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

/**
* @brief  This function handles External line 2 interrupt request.
* @param  None
* @retval None
*/
void EXTI2_IRQHandler(void)
{
   BSP_JOY_IRQHandler(JOY1, JOY_SEL);
}

/**
* @brief  This function handles External line 3 interrupt request.
* @param  None
* @retval None
*/
void EXTI3_IRQHandler(void)
{
   BSP_JOY_IRQHandler(JOY1, JOY_DOWN);
}

/**
* @brief  This function handles External line 4 interrupt request.
* @param  None
* @retval None
*/
void EXTI4_IRQHandler(void)
{
   BSP_JOY_IRQHandler(JOY1, JOY_LEFT);
}

/**
* @brief  This function handles External lines 9 to 5 interrupt request.
* @param  None
* @retval None
*/
void EXTI9_5_IRQHandler(void)
{
  BSP_JOY_IRQHandler(JOY1, JOY_RIGHT);
  BSP_JOY_IRQHandler(JOY1, JOY_UP);
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  BSP_PB_IRQHandler(BUTTON_WAKEUP);
}

/**
  * @brief  This function handles SAI DMA interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_IN_SAIx_DMAx_IRQHandler()
{
  BSP_AUDIO_IN_IRQHandler(0,AUDIO_IN_DEVICE_DIGITAL_MIC);
}
/**
  * @brief  This function handles DMA2 Stream 1 interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  BSP_AUDIO_OUT_IRQHandler(0);
}

/**
  * @brief  This function handles BDMA Channel 1 for SAI_PDM interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_IN_SAI_PDMx_DMAx_IRQHandler(void)
{
  BSP_AUDIO_IN_IRQHandler(1, AUDIO_IN_DEVICE_DIGITAL_MIC);
}

/**
  * @brief  Handles MDMA transfer interrupt request.
  * @retval None
  */
void MDMA_IRQHandler(void)
{
    BSP_SDRAM_IRQHandler(0);
}

/**
  * @brief  This function handles SD interrupt request.
  * @param  None
  * @retval None
  */
void SDMMC1_IRQHandler(void)
{
  BSP_SD_IRQHandler(0);
}

/**
  * @brief  DCMI interrupt handler.
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void)
{
   BSP_CAMERA_IRQHandler(0);
}

/**
  * @brief  DMA interrupt handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
  BSP_CAMERA_DMA_IRQHandler(0);
}

/**
  * @}
  */

/**
  * @}
  */

