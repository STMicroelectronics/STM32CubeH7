/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
#include "k_module.h"

#ifdef LCD_CLK_TUNING
extern void LCD_CLK_Tuning(void);
#endif /* LCD_CLK_TUNING */

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
extern LTDC_HandleTypeDef hltdc;
extern SAI_HandleTypeDef          haudio_out_sai;
extern SAI_HandleTypeDef          haudio_in_sai;
DFSDM_Filter_HandleTypeDef hAudioInLeftFilter;
DFSDM_Filter_HandleTypeDef hAudioInRightFilter;
extern JPEG_HandleTypeDef         JPEG_Handle;
extern osSemaphoreId              osDma2dSemph;
extern int                        GameIsInuse;
extern int                        OSReady;
DMA2D_HandleTypeDef               DMA2D_Handle;
#ifdef LCD_CLK_TUNING
extern TIM_HandleTypeDef        TimHandleLcd;
#endif /* LCD_CLK_TUNING */

/* Private function prototypes -----------------------------------------------*/

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
/*void SVC_Handler(void)
{
}*/

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
/*void PendSV_Handler(void)
{
}*/

/**
  * @brief This function handles SysTick Handler.
  * @param None
  * @retval None
  */
void SysTick_Handler(void)
{
  osSystickHandler();
}


void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hltdc);
}


/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MFX_IRQOUT_PIN);
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_TAMPER_PIN);
}


/* SAI Audio OUT DMA2_Stream1 */
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  BSP_AUDIO_OUT_IRQHandler(0);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void JPEG_IRQHandler(void)
{
  HAL_JPEG_IRQHandler(&JPEG_Handle);
}

/**
  * @brief  This function handles MDMA interrupt request.
  * @param  None
  * @retval None
  */

void MDMA_IRQHandler(void)
{
  /* Check the interrupt and clear flag */
  HAL_MDMA_IRQHandler(JPEG_Handle.hdmain);
  HAL_MDMA_IRQHandler(JPEG_Handle.hdmaout);  
}

/**
* @brief  This function handles SDMMC interrupt request.
* @param  None
* @retval None
*/
void SDMMC1_IRQHandler(void)
{
BSP_SD_IRQHandler(0);
}


void DMA2D_IRQHandler(void)
{ 
  if(OSReady != 1)
  {
    HAL_DMA2D_IRQHandler(&DMA2D_Handle);
  }
  else
  {
    osSemaphoreRelease(osDma2dSemph);
    DMA2D->CR &= ~(DMA2D_IT_TC);
    DMA2D->IFCR = DMA2D_FLAG_TC;    
  }
}

#ifdef LCD_CLK_TUNING
/**
  * @brief  This function handles TIM7 interrupt request.
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void)
{
  LCD_CLK_Tuning();
  __HAL_TIM_CLEAR_IT(&TimHandleLcd, TIM_IT_UPDATE);
}
#endif /* LCD_CLK_TUNING */

/**
  * @}
  */

/**
  * @}
  */

