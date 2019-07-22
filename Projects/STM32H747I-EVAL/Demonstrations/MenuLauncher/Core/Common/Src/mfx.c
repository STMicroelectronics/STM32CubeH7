/**
  ******************************************************************************
  * @file    mfx.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel menu functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

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

    irqPending = MFX_IO_Read(IO_I2C_ADDRESS, MFXSTM32L152_REG_ADR_IRQ_PENDING);

    /* GPIO IT from MFX */
    if(irqPending & MFX_IRQ_PENDING_GPIO)
    {
#if defined (USE_TS_INT)
      uint32_t statusGpio = BSP_IO_ITGetStatus(LCD_INT_PIN | SD_DETECT_PIN);
#else
      uint32_t statusGpio = BSP_IO_ITGetStatus(SD_DETECT_PIN);
#endif /* USE_TS_INT */

#if defined (USE_TS_INT)
      if(statusGpio & LCD_INT_PIN)
      {
        osSemaphoreRelease(TSSemaphoreID);
      }
#endif /* USE_TS_INT */

      if((statusGpio & SD_DETECT_PIN) && StorageEvent)
      {
        if(BSP_SD_IsDetected() == SD_PRESENT)
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
      BSP_IO_ITClearPin(statusGpio);
    }

    /* Ack all IRQ pending except GPIO if any */
    irqPending &= ~MFX_IRQ_PENDING_GPIO;
    if (irqPending)
    {
      MFX_IO_Write(IO_I2C_ADDRESS, MFXSTM32L152_REG_ADR_IRQ_ACK, irqPending);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
