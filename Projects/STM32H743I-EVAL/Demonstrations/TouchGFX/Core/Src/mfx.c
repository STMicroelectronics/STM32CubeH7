/**
  ******************************************************************************
  * @file    mfx.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel menu functions
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
#ifdef STM32H747xx
#include "stm32h7xx_hal.h"
#endif

#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_sd.h"

#include <cmsis_os.h>
#include "mfx.h"
#include "storage_app.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MFX
  * @brief Kernel menu routines
  * @{
  */

/* External variables --------------------------------------------------------*/
#if defined (USE_TS_INT)
extern osSemaphoreId TSSemaphoreID;
#endif /* USE_TS_INT */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define MFX_IRQ_PENDING_GPIO    0x01
#define MFX_IRQ_PENDING_IDD     0x02
#define MFX_IRQ_PENDING_ERROR   0x04
#define MFX_IRQ_PENDING_TS_DET  0x08
#define MFX_IRQ_PENDING_TS_NE   0x10
#define MFX_IRQ_PENDING_TS_TH   0x20
#define MFX_IRQ_PENDING_TS_FULL 0x40
#define MFX_IRQ_PENDING_TS_OVF  0x80

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static osSemaphoreId MfxIntSemaphore;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void MfxThread(void const * argument);

/**
  * @brief  Mfx thread initialization to manage interrupt
  * @param  None
  * @retval None
  */
void MfxInit(void)
{
  /* Create  mfx task */
  osThreadDef(MFX_Thread, MfxThread, osPriorityRealtime, 0, 128);
  osThreadCreate (osThread(MFX_Thread), NULL);
}

/**
  * @brief  interrupt detection
  * @param  None
  * @retval None
  */
void MfxDetectInt(void)
{
  /* Release the semaphore if the thread was created prior to this call */
  if(MfxIntSemaphore)
  {
    osSemaphoreRelease(MfxIntSemaphore);
  }
}


/**
  * @brief  Mfx Thread
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */

__IO uint32_t errorSrc = 0;
__IO uint32_t errorMsg = 0;

static void MfxThread(void const * argument)
{
  uint32_t irqPending;

  /* Create Semaphore lock */
  osSemaphoreDef(MFX_IDD_SEM);

  /* Create the Semaphore used by the two threads */
  MfxIntSemaphore = osSemaphoreCreate(osSemaphore(MFX_IDD_SEM), 1);

  /* Get the semaphore */
  osSemaphoreWait(MfxIntSemaphore, osWaitForever);

  for( ;; )
  {
    osSemaphoreWait(MfxIntSemaphore, osWaitForever);

    /* Disable MFX interrupt to manage current one */
    HAL_NVIC_DisableIRQ(MFX_IRQOUT_EXTI_IRQn);

    irqPending = MFXSTM32L152_GlobalITStatus(&Io_CompObj, MFXSTM32L152_IRQ_GPIO);;

    /* GPIO IT from MFX */
    if(irqPending & MFX_IRQ_PENDING_GPIO)
    {
#if defined (USE_TS_INT)
      uint32_t statusGpio = MFXSTM32L152_IO_ITStatus(&Io_CompObj, LCD_INT_PIN | SD_DETECT_PIN);
#else
      uint32_t statusGpio =MFXSTM32L152_IO_ITStatus(&Io_CompObj, SD_DETECT_PIN);
#endif /* USE_TS_INT */

#if defined (USE_TS_INT)
      if(statusGpio & LCD_INT_PIN)
      {
        osSemaphoreRelease(TSSemaphoreID);
      }
#endif /* USE_TS_INT */

      if((statusGpio & SD_DETECT_PIN) && StorageEvent)
      {
        if(BSP_SD_IsDetected(0) == SD_PRESENT)
        {
          /* Notify App about connection event */
          osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
        }
        else
        {
          /* Notify App about disconnection event */
          osMessagePut ( StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0);
        }
      }

      /* Clear IO Expander IT */
      BSP_IO_ClearIT(0,statusGpio);
    }

    /* Ack all IRQ pending except GPIO if any */
    irqPending &= ~MFX_IRQ_PENDING_GPIO;
    if (irqPending)
    {
      MFXSTM32L152_IO_WritePin(&Io_CompObj,MFXSTM32L152_REG_ADR_IRQ_ACK,irqPending);
    }

    /* Re-enable MFX interrupt */
    HAL_NVIC_EnableIRQ(MFX_IRQOUT_EXTI_IRQn);
  }
}


/**
  * @}
  */

/**
  * @}
  */

