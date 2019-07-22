/**
  ******************************************************************************
  * @file    BSP/Src/stm32h7xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
extern SAI_HandleTypeDef haudio_out_sai;
extern SAI_HandleTypeDef haudio_in_sai;

extern SDRAM_HandleTypeDef sdramHandle; 
extern LTDC_HandleTypeDef  hltdc_discovery;
extern MMC_HandleTypeDef uSdHandle;

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
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(WAKEUP_BUTTON_PIN);
}

/**
  * @brief  Handles LTDC interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
void BSP_LCD_LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(hltdc_discovery));
}

/**
  * @brief  This function handles LTDC Error interrupt Handler.
  * @note : Can be surcharged by application code implementation of the function.
  */
void BSP_LCD_LTDC_ER_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(hltdc_discovery));
}

/**
  * @brief  This function handles DMA2 Stream 1 interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

void AUDIO_IN_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_in_sai.hdmarx);
}

/**
  * @brief  This function handles BDMA Channel 1 for SAI_PDM interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_IN_SAI_PDMx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_in_sai.hdmarx);
}

/**
  * @brief  Handles MDMA transfer interrupt request.
  * @retval None
  */
void MDMA_IRQHandler(void)
{
  HAL_MDMA_IRQHandler(sdramHandle.hmdma);
}

/**
  * @brief  This function handles MMC interrupt request.
  * @param  None
  * @retval None
  */
void SDMMC1_IRQHandler(void)
{
  BSP_MMC_IRQHandler();
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
