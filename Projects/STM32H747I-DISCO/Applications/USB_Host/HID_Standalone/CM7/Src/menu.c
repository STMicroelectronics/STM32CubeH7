/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/CM7/Src/menu.c
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
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

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
HID_DEMO_StateMachine hid_demo;
uint8_t prev_select = 0;

uint8_t *DEMO_KEYBOARD_menu[] = {
  (uint8_t *)
    "      1 - Start Keyboard / Clear                                            ",
  (uint8_t *)
    "      2 - Return                                                            ",
};

uint8_t *DEMO_MOUSE_menu[] = {
  (uint8_t *)
    "      1 - Start Mouse / Re-Initialize                                       ",
  (uint8_t *)
    "      2 - Return                                                            ",
};

uint8_t *DEMO_HID_menu[] = {
  (uint8_t *)
    "      1 - Start HID                                                         ",
  (uint8_t *)
    "      2 - Re-Enumerate                                                      ",
};

/* Private function prototypes ----------------------------------------------- */
static void USBH_MouseDemo(USBH_HandleTypeDef * phost);
static void USBH_KeybdDemo(USBH_HandleTypeDef * phost);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void HID_MenuInit(void)
{
  /* Start HID Interface */
  USBH_UsrLog("Starting HID Demo");
  USBH_UsrLog("Connect your HID Device");
  hid_demo.state = HID_DEMO_IDLE;
  HID_MenuProcess();
}

/**
  * @brief  Manages HID Menu Process.
  * @param  None
  * @retval None
  */
void HID_MenuProcess(void)
{
    switch(hid_demo.state)
  {
  case HID_DEMO_IDLE:
    hid_demo.state = HID_DEMO_START;
    break;

  case HID_DEMO_START:
    if(Appli_state == APPLICATION_READY)
    {
      if(USBH_HID_GetDeviceType(&hUSBHost) == HID_KEYBOARD)
      {
        hid_demo.state = HID_DEMO_KEYBOARD;
        USR_KEYBRD_Init();
      }
      else if(USBH_HID_GetDeviceType(&hUSBHost) == HID_MOUSE)
      {
        hid_demo.state = HID_DEMO_MOUSE;
        USR_MOUSE_Init();
      }
    }
    break;

  case HID_DEMO_MOUSE:
    if(Appli_state == APPLICATION_READY)
    {
      USBH_MouseDemo(&hUSBHost);
    }
    break;

  case HID_DEMO_KEYBOARD:
    if(Appli_state == APPLICATION_READY)
    {
      USBH_KeybdDemo(&hUSBHost);
    }
    break;

  default:
    break;
  }

  if(Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    UTIL_LCD_TRACE_ClearTextZone();
    LCD_ErrTrace("USB device disconnected!\n");
    hid_demo.state = HID_DEMO_IDLE;
  }
}

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
void HID_SelectItem(uint8_t ** menu, uint8_t item)
{
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

  switch (item)
  {
  case 0:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(20, menu[0]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(21, menu[1]);
    break;

  case 1:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(20, menu[0]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(21, menu[1]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    break;
  }
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);

}

/**
  * @brief  Main routine for Mouse application
  * @param  phost: Host handle
  * @retval None
  */
static void USBH_MouseDemo(USBH_HandleTypeDef * phost)
{
  HID_MOUSE_Info_TypeDef *m_pinfo;

  m_pinfo = USBH_HID_GetMouseInfo(phost);
  if (m_pinfo != NULL)
  {
    /* Handle Mouse data position */
    USR_MOUSE_ProcessData(&mouse_info);

    if (m_pinfo->buttons[0])
    {
      HID_MOUSE_ButtonPressed(0);
    }
    else
    {
      HID_MOUSE_ButtonReleased(0);
    }

    if (m_pinfo->buttons[1])
    {
      HID_MOUSE_ButtonPressed(2);
    }
    else
    {
      HID_MOUSE_ButtonReleased(2);
    }

    if (m_pinfo->buttons[2])
    {
      HID_MOUSE_ButtonPressed(1);
    }
    else
    {
      HID_MOUSE_ButtonReleased(1);
    }
  }
}

/**
  * @brief  Main routine for Keyboard application
  * @param  phost: Host handle
  * @retval None
  */
static void USBH_KeybdDemo(USBH_HandleTypeDef * phost)
{
  HID_KEYBD_Info_TypeDef *k_pinfo;
  char c;
  k_pinfo = USBH_HID_GetKeybdInfo(phost);

  if (k_pinfo != NULL)
  {
    c = USBH_HID_GetASCIICode(k_pinfo);
    if (c != 0)
    {
      USR_KEYBRD_ProcessData(c);
    }
  }
}

