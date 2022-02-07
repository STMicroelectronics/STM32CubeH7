/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/menu.c 
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
CDC_DEMO_StateMachine CdcDemo;
CDC_DEMO_SelectMode   CdcSelectMode;
uint8_t               PrevSelect = 0;
uint8_t *CDC_main_menu[] = 
{
  (uint8_t *)"      1 - Send Data                                                         ",
  (uint8_t *)"      2 - Receive Data                                                      ",
  (uint8_t *)"      3 - Configuration                                                     ",
  (uint8_t *)"      4 - Re-Enumerate                                                      ",
};

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
* @brief  Demo state machine.
* @param  None
* @retval None
*/
void Menu_Init(void)
{
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAtLine(14, (uint8_t *)"                                          ");
  UTIL_LCD_DisplayStringAtLine(15, (uint8_t *)"Use [Buttons Left/Right] to scroll up/down");
  UTIL_LCD_DisplayStringAtLine(16, (uint8_t *)"Use [Joystick Up/Down] to scroll CDC menu ");
  
  CdcDemo.state = CDC_DEMO_IDLE;
  CDC_MenuProcess();
}

/**
* @brief  Probes the CDC joystick state.
* @param  state: Joystick state
* @retval None
*/
static void CDC_DEMO_ProbeKey(uint32_t state)
{
  /* Handle Joystick inputs */
  if (CdcSelectMode == CDC_SELECT_MENU)
  {
    if ((state == JOY_UP) && (CdcDemo.select > 0))
    {
      CdcDemo.select--;
    }
    else
      if (((CdcDemo.state == CDC_DEMO_WAIT) && (state == JOY_DOWN) &&
           (CdcDemo.select < 3)) || ((CdcDemo.state == CDC_DEMO_SEND) &&
                                     (state == JOY_DOWN) &&
                                     (CdcDemo.select < 2)) ||
          ((CdcDemo.state == CDC_DEMO_RECEIVE) && (state == JOY_DOWN) &&
           (CdcDemo.select < 1)) || ((CdcDemo.state == CDC_DEMO_CONFIGURATION)
                                     && (state == JOY_DOWN) &&
                                     (CdcDemo.select < 2)))
    {
      CdcDemo.select++;
    }
    else if (state == JOY_SEL)
    {
      CdcDemo.select |= 0x80;
    }
  }
  else if (CdcSelectMode == CDC_SELECT_CONFIG)
  {
    CDC_Settings_ProbeKey(state);
  }
  else if (CdcSelectMode == CDC_SELECT_FILE)
  {
    CDC_SendFile_ProbeKey(state);
  }
}

/**
* @brief  Manages CDC Menu Process.
* @param  None
* @retval None
*/
void CDC_MenuProcess(void)
{
  switch (CdcDemo.state)
  {
  case CDC_DEMO_IDLE:
    CDC_SelectItem(CDC_main_menu, 0);
    CdcDemo.state = CDC_DEMO_WAIT;
    CdcDemo.select = 0;
    break;

  case CDC_DEMO_WAIT:
    if (CdcDemo.select != PrevSelect)
    {
      PrevSelect = CdcDemo.select;
      CDC_SelectItem(CDC_main_menu, CdcDemo.select & 0x7F);

      /* Handle select item */
      if (CdcDemo.select & 0x80)
      {
        switch (CdcDemo.select & 0x7F)
        {
        case 0:
          CdcDemo.state = CDC_DEMO_SEND;
          CdcDemo.Send_state = CDC_SEND_IDLE;
          break;

        case 1:
          CdcDemo.state = CDC_DEMO_RECEIVE;
          CdcDemo.Receive_state = CDC_RECEIVE_IDLE;
          break;

        case 2:
          CdcDemo.state = CDC_DEMO_CONFIGURATION;
          CdcDemo.Configuration_state = CDC_CONFIGURATION_IDLE;
          break;

        case 3:
          CdcDemo.state = CDC_DEMO_REENUMERATE;
          break;

        default:
          break;
        }
      }
    }
    break;

  case CDC_DEMO_SEND:
    if (Appli_state == APPLICATION_READY)
    {
      CDC_Handle_Send_Menu();
    }
    else
    {
      CdcDemo.state = CDC_DEMO_WAIT;
    }
    break;

  case CDC_DEMO_RECEIVE:
    if (Appli_state == APPLICATION_READY)
    {
      CDC_Handle_Receive_Menu();
    }
    else
    {
      CdcDemo.state = CDC_DEMO_WAIT;
    }
    break;

  case CDC_DEMO_CONFIGURATION:
    if (Appli_state == APPLICATION_READY)
    {
      CDC_Handle_Configuration_Menu();
    }
    else
    {
      CdcDemo.state = CDC_DEMO_WAIT;
    }
    break;

  case CDC_DEMO_REENUMERATE:
    /* Force Device to re-enumerate */
    USBH_ReEnumerate(&hUSBHost);
    CdcDemo.state = CDC_DEMO_WAIT;
    break;

  default:
    break;
  }

  CdcDemo.select &= 0x7F;

  if (Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    UTIL_LCD_TRACE_ClearTextZone();
    LCD_ErrTrace("CDC device disconnected!\n");
    CDC_ChangeSelectMode(CDC_SELECT_MENU);
    CdcDemo.state = CDC_DEMO_IDLE;
    CdcDemo.select = 0;
  }
}

/**
* @brief  Manages the menu on the screen.
* @param  menu: Menu table
* @param  item: Selected item to be highlighted
* @retval None
*/
void CDC_SelectItem(uint8_t ** menu, uint8_t item)
{
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

  switch (item)
  {
  case 0:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(19, menu[0]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(20, menu[1]);
    UTIL_LCD_DisplayStringAtLine(21, menu[2]);
    break;

  case 1:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(19, menu[0]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(20, menu[1]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(21, menu[2]);
    break;

  case 2:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(19, menu[0]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(20, menu[1]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(21, menu[2]);
    break;

  case 3:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(19, menu[1]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(20, menu[2]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(21, menu[3]);
    break;

  case 4:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(19, menu[2]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(20, menu[3]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(21, menu[4]);
    break;

  default:
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAtLine(19, menu[0]);
    UTIL_LCD_DisplayStringAtLine(20, menu[1]);
    UTIL_LCD_DisplayStringAtLine(21, menu[2]);
    break;
  }
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
}

/**
* @brief  Changes the selection mode.
* @param  select_mode: Selection mode
* @retval None
*/
void CDC_ChangeSelectMode(CDC_DEMO_SelectMode select_mode)
{
  if (select_mode == CDC_SELECT_CONFIG)
  {
    LCD_ClearTextZone();
    CDC_SelectItem(DEMO_CONFIGURATION_menu, 0xFF);
    CdcSettingsState.select = 0;
    CDC_SelectSettingsItem(0);
  }
  else if (select_mode == CDC_SELECT_FILE)
  {
    CDC_SelectItem(DEMO_SEND_menu, 0xFF);
    CdcSettingsState.select = 0;
  }
  else if (select_mode == CDC_SELECT_MENU)
  {
    if (CdcDemo.state == CDC_DEMO_CONFIGURATION)
    {
      CDC_SelectSettingsItem(0xFF);
      CDC_SelectItem(DEMO_CONFIGURATION_menu, 0);
    }

    if (CdcDemo.state == CDC_DEMO_SEND)
    {
      CDC_SelectItem(DEMO_SEND_menu, 1);
    }
  }
  CdcSelectMode = select_mode;
  CdcDemo.select = 0;
}


void BSP_JOY_Callback(JOY_TypeDef JOY, JOYPin_TypeDef JoyPin)
{
  CDC_DEMO_ProbeKey(JoyPin); 
  if((CdcSelectMode == CDC_SELECT_MENU) && (CdcDemo.state != CDC_DEMO_RECEIVE))
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
  if(Button == BUTTON_TAMPER)
  {
    if(CdcDemo.state == CDC_DEMO_SEND)
    {
      /* Prevent debounce effect for Tamper button */
      if((HAL_GetTick() - debounce_time) > 50)
      {
        debounce_time = HAL_GetTick();
      }
      else
      {
        return;
      }  
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK); 
      /* Change the selection type */
      if(CdcSelectMode == CDC_SELECT_MENU)
      {
        CDC_ChangeSelectMode(CDC_SELECT_CONFIG); 
      }
      else if(CdcSelectMode == CDC_SELECT_CONFIG)
      {
        CDC_ChangeSelectMode(CDC_SELECT_MENU);
      }
      
      else if(CdcSelectMode == CDC_SELECT_FILE)
      {
        CDC_ChangeSelectMode(CDC_SELECT_FILE);
      }
    }
  }
}

/**
* @brief  Clears the text zone.
* @param  None
* @retval None
*/
void LCD_ClearTextZone(void)
{
  uint8_t i = 0;

  for (i = 0; i < 12; i++)
  {
    UTIL_LCD_ClearStringLine(i + 3);
  }
}

