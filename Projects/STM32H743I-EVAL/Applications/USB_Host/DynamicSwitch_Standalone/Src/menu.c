/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/menu.c
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
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
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  msc_demo.state = MSC_DEMO_IDLE;
  hid_demo.state = HID_DEMO_IDLE;
  audio_demo.state = AUDIO_DEMO_IDLE;
}

/**
  * @brief  Manages DS Menu Process.
  * @param  None
  * @retval None
  */
void DS_MenuProcess(void)
{
  switch (Appli_state)
  {
  case APPLICATION_IDLE:
    break;

  case APPLICATION_MSC:
    MSC_MenuProcess();
    break;

  case APPLICATION_AUDIO:
    AUDIO_MenuProcess();
    break;

  case APPLICATION_HID:
    HID_MenuProcess();
    break;

  default:
    break;
  }
  if (Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    LCD_ErrTrace("USB device disconnected!\n");
    Menu_Init();

    LCD_UsrTrace("Plug your device To Continue...\n");
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void BSP_JOY_Callback(JOY_TypeDef JOY, JOYPin_TypeDef JoyPin)
{
 
    if(Appli_state == APPLICATION_MSC)
    {
      MSC_DEMO_ProbeKey(JoyPin);
    }
    else if(Appli_state == APPLICATION_HID)
    {
      HID_DEMO_ProbeKey(JoyPin);
    }
    else if(Appli_state == APPLICATION_AUDIO)
    {
      if(audio_select_mode == AUDIO_SELECT_MENU)
      {
        AUDIO_MenuProbeKey(JoyPin);
      }
      else if(audio_select_mode == AUDIO_PLAYBACK_CONTROL)
      {
        AUDIO_PlaybackProbeKey(JoyPin);
      }
    }

    if(Appli_state != APPLICATION_HID)
    {
      switch(JoyPin)
      {
      case JOY_LEFT:
        UTIL_LCD_TRACE_ScrollBack();
        break;

      case JOY_RIGHT:
        UTIL_LCD_TRACE_ScrollForward();
        break;

      default:
        break;
      }
    }
   HAL_Delay(400); 
}   
/**
  * @brief  Button Callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
   static uint32_t debounce_time = 0;

  if(audio_demo.state == AUDIO_DEMO_PLAYBACK)
  {
    if(Button == BUTTON_TAMPER)
    {
      /* Prevent debounce effect for user Tamper */
      if((HAL_GetTick() - debounce_time) > 50)
      {
        debounce_time = HAL_GetTick();
      }
      else
      {
        return;
      }

      /* Change the selection type */
      if(audio_select_mode == AUDIO_SELECT_MENU)
      {
        Audio_ChangeSelectMode(AUDIO_PLAYBACK_CONTROL);
      }
      else if(audio_select_mode == AUDIO_PLAYBACK_CONTROL)
      {
        Audio_ChangeSelectMode(AUDIO_SELECT_MENU);
      }
    }
  }
}

