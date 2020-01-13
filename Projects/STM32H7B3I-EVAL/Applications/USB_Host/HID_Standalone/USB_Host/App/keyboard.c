/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : USB_Host/HID_Standalone/USB_Host/App/keyboard.c
  * @author  : MCD Application Team
  * @brief   : This file implements the USB Host
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "keyboard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main routine for Keyboard application
  * @param  phost: Host handle
  * @retval None
  */
void HID_KEYBRD_App(USBH_HandleTypeDef *phost)
{
  HID_KEYBD_Info_TypeDef *k_pinfo;
  char c;
  k_pinfo = USBH_HID_GetKeybdInfo(phost);

  if(k_pinfo != NULL)
  {
    c = USBH_HID_GetASCIICode(k_pinfo);
    if(c != 0)
    {
      USBH_UsrLog("%c", c);
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
