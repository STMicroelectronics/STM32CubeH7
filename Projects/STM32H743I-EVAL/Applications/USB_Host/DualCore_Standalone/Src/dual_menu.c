/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/dual_menu.c
  * @author  MCD Application Team
  * @brief   Dual Core Process
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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

/* Private typedef -----------------------------------------------------------*/
uint8_t *DEMO_main_menu[] =
{
  (uint8_t *)"      1 - Mass Storage demo                                                 ",
  (uint8_t *)"      2 - Human Interface demo                                              ",
  (uint8_t *)"      3 - Re-Enumerate MSC                                                  ",
  (uint8_t *)"      4 - Re-Enumerate HID                                                  ",
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t prev_select = 0;
DEMO_StateMachine demo;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Menu Init
  * @param  None
  * @retval None
  */
void DUAL_MenuInit(void)
{
  /* Start HID Interface */
  LCD_UsrTrace("USB Host Full speed initialized.\n");
  /* Start MSC Interface */
  LCD_UsrTrace("USB Host High speed initialized.\n");

  GUI_SetTextColor(GUI_COLOR_GREEN);
  GUI_DisplayStringAtLine(17, (uint8_t *)"Use [Joystick Left/Right] to scroll up/down");
  GUI_DisplayStringAtLine(18, (uint8_t *)"Use [Joystick Up/Down] to scroll DUAL menu");
  Demo_SelectItem(DEMO_main_menu, 0);
}

/**
  * @brief  Manages DUAL Menu Process.
  * @param  None
  * @retval None
  */
void DUAL_MenuProcess(void)
{
  switch(demo.state)
  {
  case DEMO_IDLE:
    Demo_SelectItem(DEMO_main_menu, 0);
    GUI_SetTextColor(GUI_COLOR_GREEN);
    GUI_DisplayStringAtLine(17, (uint8_t *)"Use [Joystick Left/Right] to scroll up/down");
    LCD_UsrTrace("Select an operation to continue.\n");
    demo.state = DEMO_WAIT;
    demo.select = 0;
    break;
  case DEMO_WAIT:

    if(demo.select != prev_select)
    {
      prev_select = demo.select;
      Demo_SelectItem(DEMO_main_menu, demo.select & 0x7F);

      /* Handle select item */
      if(demo.select & 0x80)
      {
        demo.select &= 0x7F;

        switch(demo.select)
        {
        case 0:
          demo.state = DEMO_MSC_START;
          break;

        case 1:
          demo.state = DEMO_HID_START;
          break;

        case 2:
          demo.state = DEMO_MSC_REENUMERATE;
          break;

        case 3:
          demo.state = DEMO_HID_REENUMERATE;
          break;

        default:
          break;
        }
      }
    }
    break;

  case DEMO_MSC_START:
    if(Appli_HS_state == APPLICATION_HS_READY)
    {
      /* If HS device connected*/
      demo.state = DEMO_MSC;
      demo.msc_state = APPLI_MSC_IDLE;
    }
    else
    {
      LCD_ErrTrace("No Mass storage device connected.\n");
      demo.state = DEMO_WAIT;
    }
    break;

  case DEMO_HID_START:
    if(Appli_FS_state == APPLICATION_FS_READY)
    {
      /*if FS device connected*/
      demo.state = DEMO_HID;
      demo.hid_state = APPLI_HID_IDLE;
    }
    else
    {
      LCD_ErrTrace("No HID device connected.\n");
      demo.state = DEMO_WAIT;
    }
    break;

  case DEMO_MSC:
    MSC_MenuProcess();
    break;

  case DEMO_HID:
    HID_MenuProcess();
    break;

  case DEMO_MSC_REENUMERATE: /* Re-Enumeration MSC */
    LCD_DbgTrace("Starting Enumeration MSC....");
    LCD_DbgTrace("\n");
    USBH_ReEnumerate(&hUSBHost_HS);
    demo.state = DEMO_WAIT;
    break;

  case DEMO_HID_REENUMERATE: /* Re-Enumeration HID */
    LCD_DbgTrace("Starting Enumeration HID....");
    LCD_DbgTrace("\n");
    USBH_ReEnumerate(&hUSBHost_FS);
    demo.state = DEMO_WAIT;
    break;

  default:
    break;
  }

  if(Appli_FS_state == APPLICATION_FS_DISCONNECT)
  {
    Appli_FS_state = APPLICATION_IDLE;
    UTIL_LCD_TRACE_ClearTextZone();
    LCD_ErrTrace("device FS disconnected!\n");
    demo.state = DEMO_IDLE;
    demo.select = 0;
  }

  if(Appli_HS_state == APPLICATION_HS_DISCONNECT)
  {
    Appli_HS_state = APPLICATION_IDLE;
    UTIL_LCD_TRACE_ClearTextZone();
    LCD_ErrTrace("device HS disconnected!\n");
    demo.state = DEMO_IDLE;
    demo.select = 0;
  }
}

/**
  * @brief  Probes the joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void Demo_ProbeKey(uint32_t state)
{
  if((state == JOY_UP) && (demo.select > 0))
  {
    demo.select--;
  }
  else if(((demo.state == DEMO_WAIT) && (state == JOY_DOWN) && (demo.select < 3)) ||
          ((demo.state == DEMO_MSC)  && (state == JOY_DOWN) && (demo.select < 2)) ||
          ((demo.state == DEMO_HID)  && (state == JOY_DOWN) && (demo.select < 1)))
  {
    demo.select++;
  }
  else if(state == JOY_SEL)
  {
    demo.select |= 0x80;
  }
}

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
void Demo_SelectItem(uint8_t **menu, uint8_t item)
{
  GUI_ClearStringLine(19);
  GUI_ClearStringLine(20);
  GUI_ClearStringLine(21);
  GUI_SetTextColor(GUI_COLOR_WHITE);

  switch(item)
  {
  case 0:
    GUI_SetBackColor(GUI_COLOR_MAGENTA);
    GUI_DisplayStringAtLine(19, menu[0]);
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(20, menu[1]);
    GUI_DisplayStringAtLine(21, menu[2]);
    break;

  case 1:
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(19, menu[0]);
    GUI_SetBackColor(GUI_COLOR_MAGENTA);
    GUI_DisplayStringAtLine(20, menu[1]);
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(21, menu[2]);
    break;

  case 2:
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(19, menu[0]);
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(20, menu[1]);
    GUI_SetBackColor(GUI_COLOR_MAGENTA);
    GUI_DisplayStringAtLine(21, menu[2]);
    break;

  case 3:
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(19, menu[1]);
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(20, menu[2]);
    GUI_SetBackColor(GUI_COLOR_MAGENTA);
    GUI_DisplayStringAtLine(21, menu[3]);
    break;

  default:
    break;
  }
  GUI_SetBackColor(GUI_COLOR_BLACK);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void BSP_JOY_Callback(JOY_TypeDef JOY, JOYPin_TypeDef JoyPin)
{
  Demo_ProbeKey(JoyPin);
  
  if(demo.state != DEMO_HID)
  {
    switch (JoyPin)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
