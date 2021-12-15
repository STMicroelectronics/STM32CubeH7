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
#include "k_module.h"
 /** @addtogroup CORE
  * @{
  */

/** @defgroup 
  * @brief  
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern LTDC_HandleTypeDef              hltdc;  
extern DSI_HandleTypeDef               hdsi;  
extern JPEG_HandleTypeDef             *JPEG_Handle;
extern K_ModuleItem_Typedef            audio_recorder_board;
extern SAI_HandleTypeDef               haudio_out_sai;
extern SAI_HandleTypeDef               haudio_in_sai;
DFSDM_Filter_HandleTypeDef             hAudioInLeftFilter;
DFSDM_Filter_HandleTypeDef             hAudioInRightFilter;
extern RTC_HandleTypeDef               RtcHandle;
#if defined(USE_USB_FS) || defined(USE_USB_HS)
extern HCD_HandleTypeDef               hhcd;
#endif /* USE_USB_FS | USE_USB_HS */

/* Private function prototypes -----------------------------------------------*/
extern void xPortSysTickHandler(void);
extern void LTDC_ISR_Handler(void);

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
  Error_Handler();
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  Error_Handler();
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  Error_Handler();
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  Error_Handler();
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
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  osSystickHandler();
}

/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles RTC Alarm interrupt request.
  * @param  None
  * @retval None
  */
void RTC_Alarm_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&RtcHandle);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void HSEM1_IRQHandler(void)
{
  HAL_HSEM_IRQHandler();	  
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_WAKEUP_PIN);
}

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

/**
* @brief  This function handles DSI Handler.
* @param  None
* @retval None
*/
void DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&hdsi);
}

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hltdc);
}

/**
  * @brief  This function handles JPEG interrupt request.
  * @param  None
  * @retval None
  */
void JPEG_IRQHandler(void)
{
  HAL_JPEG_IRQHandler(JPEG_Handle);
}

/* DFSDM_LEFT ***** DMA2 Stream0 */
void AUDIO_DFSDMx_DMAx_LEFT_IRQHandler(void) 
{
  HAL_DMA_IRQHandler(hAudioInLeftFilter.hdmaReg); 
}

/* DFSDM RIGHT **** DMA2_Stream5 */
void AUDIO_DFSDMx_DMAx_RIGHT_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hAudioInRightFilter.hdmaReg);
}


/* SAI Audio OUT DMA2_Stream1 */
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  BSP_AUDIO_OUT_IRQHandler(0);
}

/* SAI Audio IN DMA2_Stream4 */
void AUDIO_IN_SAIx_DMAx_IRQHandler(void)
{
  BSP_AUDIO_IN_IRQHandler(0,AUDIO_IN_DEVICE_DIGITAL_MIC);
}

/**
  * @brief  This function handles MDMA interrupt request.
  * @param  None
  * @retval None
  */

void MDMA_IRQHandler(void)
{
  /* Check the interrupt and clear flag */
  HAL_MDMA_IRQHandler(JPEG_Handle->hdmain);
  HAL_MDMA_IRQHandler(JPEG_Handle->hdmaout);  
}

#if defined(USE_SDCARD)
/**
  * @brief  This function handles SDMMC1 interrupt request.
  * @retval None
  */
void SDMMC1_IRQHandler(void)
{
  BSP_SD_IRQHandler(0);
}
#endif /* USE_SDCARD */

#if defined(USE_USB_FS) || defined(USE_USB_HS)
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
  HAL_HCD_IRQHandler(&hhcd);
}
#endif /* USE_USB_FS | USE_USB_HS */

/**
  * @}
  */

/**
  * @}
  */
    
