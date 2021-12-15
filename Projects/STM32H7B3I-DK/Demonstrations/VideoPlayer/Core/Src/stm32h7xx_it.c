/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
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
#include "hwjpeg_decode.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern OSPI_HandleTypeDef hospi_nor[];
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;
#if defined(USE_SDCARD)
extern SD_HandleTypeDef   hsd_sdmmc[];
#endif /* USE_SDCARD */
static volatile int overrunCnt;
extern struct HwJPEG_Context_s HwJPEG_Context;

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

#if defined(USE_SDCARD)
/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  BSP_SD_DETECT_IRQHandler(0);
}
#endif /* USE_SDCARD */

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  BSP_PB_IRQHandler(BUTTON_USER);
}

/**
  * @brief  This function handles OctoSPI interrupt request.
  * @param  None
  * @retval None
  */
void OCTOSPI1_IRQHandler(void)
{
  HAL_OSPI_IRQHandler(&hospi_nor[0]);
}

/**
  * @brief  This function handles DMA2D interrupt request.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
    HAL_DMA2D_IRQHandler(&hdma2d);
}

/**
  * @brief  This function handles LTDC interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
    HAL_LTDC_IRQHandler(&hltdc);
}

/**
  * @brief  This function handles LTDC ERROR interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_ER_IRQHandler(void)
{
  if (LTDC->ISR & 2)
  {
      LTDC->ICR = 2;
      overrunCnt++;
  }
}

/**
  * @brief  This function handles JPEG interrupt request.
  * @param  None
  * @retval None
  */
void JPEG_IRQHandler(void)
{
  HAL_JPEG_IRQHandler(&HwJPEG_Context.hjpeg);
}

/**
  * @brief  This function handles MDMA interrupt request.
  * @param  None
  * @retval None
  */
void MDMA_IRQHandler(void)
{
  HAL_MDMA_IRQHandler(HwJPEG_Context.hjpeg.hdmain);
  HAL_MDMA_IRQHandler(HwJPEG_Context.hjpeg.hdmaout);
}

#if defined(USE_SDCARD)
/**
 * @brief  This function handles SDMMC interrupt request.
 * @param  None
 * @retval None
 */
void SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&hsd_sdmmc[0]);
}
#endif /* USE_SDCARD */


/**
  * @brief  This function handles BDMA Channel0 interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_OUT_I2Sx_DMAx_IRQHandler(void)
{
  BSP_AUDIO_OUT_IRQHandler(1);
}

/**
  * @brief  This function handles DMA2 Stream6 interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  BSP_AUDIO_OUT_IRQHandler(0);
}
