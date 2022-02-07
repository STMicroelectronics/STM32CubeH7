/**
  ******************************************************************************
  * @file    USB_Device/HID_LPM_Standalone/Src/stm32h7xx_it.c 
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
uint8_t HID_Buffer[4];
extern PCD_HandleTypeDef hpcd;
extern USBD_HandleTypeDef USBD_Device;
/* Private function prototypes ----------------------------------------------- */
static void GetPointerData(uint8_t * pbuf);
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
  static __IO uint32_t counter = 0;
  HAL_IncTick();

  /* check Joystick state every polling interval (10ms) */
  if (counter++ == USBD_HID_GetPollingInterval(&USBD_Device))
  {
    GetPointerData(HID_Buffer);

    /* send data though IN endpoint */
    if ((HID_Buffer[1] != 0) || (HID_Buffer[2] != 0))
    {
      USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
    }
    counter = 0;
  }
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


/**
  * @brief  This function handles USB OTG FS/HS Wakeup IRQ Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void)
{
  if ((&hpcd)->Init.low_power_enable)
  {
    SystemClockConfig_STOP();

    /* Ungate PHY clock */
    __HAL_PCD_UNGATE_PHYCLOCK((&hpcd));
  }
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
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_WAKEUP_PIN);
}

/**
  * @brief  Gets Pointer Data.
  * @param  pbuf: Pointer to report
  * @retval None
  */
static void GetPointerData(uint8_t * pbuf)
{
  int8_t x = 0, y = 0;

  switch (BSP_JOY_GetState(JOY1))
  {
  case JOY_LEFT:
    x -= CURSOR_STEP;
    break;

  case JOY_RIGHT:
    x += CURSOR_STEP;
    break;

  case JOY_UP:
    y -= CURSOR_STEP;
    break;

  case JOY_DOWN:
    y += CURSOR_STEP;
    break;

  default:
    break;
  }

  pbuf[0] = 0;
  pbuf[1] = x;
  pbuf[2] = y;
  pbuf[3] = 0;
}

/**
  * @}
  */

/**
  * @}
  */

