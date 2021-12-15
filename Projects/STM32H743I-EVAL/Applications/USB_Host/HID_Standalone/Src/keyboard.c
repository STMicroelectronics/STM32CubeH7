/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Src/keyboard.c
  * @author  MCD Application Team
  * @brief   This file implements the HID keyboard functions
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
#define KYBRD_FIRST_COLUMN               (uint16_t)8
#define KYBRD_LAST_COLUMN                (uint16_t)479
#define KYBRD_FIRST_LINE                 (uint8_t) 100
#define SMALL_FONT_COLUMN_WIDTH                    12
#define SMALL_FONT_LINE_WIDTH                      20
#define KYBRD_LAST_LINE                  (uint16_t)200
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
extern HID_DEMO_StateMachine hid_demo;
extern uint8_t *DEMO_KEYBOARD_menu[];
extern uint8_t prev_select;
extern uint32_t hid_demo_ready;
uint8_t KeybrdCharYpos = 0;
uint16_t KeybrdCharXpos = 0;
uint16_t CurrentLastXpos[KYBRD_LAST_LINE] = { 0 };

/* Private function prototypes ----------------------------------------------- */
static void USR_KEYBRD_Init(void);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Manages Keyboard Menu Process.
  * @param  None
  * @retval None
  */
void HID_KeyboardMenuProcess(void)
{
  switch (hid_demo.keyboard_state)
  {
  case HID_KEYBOARD_IDLE:
    hid_demo.keyboard_state = HID_KEYBOARD_START;
    UTIL_LCD_ClearStringLine(18);
    HID_SelectItem(DEMO_KEYBOARD_menu, 0);
    hid_demo.select = 0;
    prev_select = 0;
    break;

  case HID_KEYBOARD_WAIT:
    if (hid_demo.select != prev_select)
    {
      prev_select = hid_demo.select;
      HID_SelectItem(DEMO_KEYBOARD_menu, hid_demo.select & 0x7F);
      /* Handle select item */
      if (hid_demo.select & 0x80)
      {
        hid_demo.select &= 0x7F;
        switch (hid_demo.select)
        {
        case 0:
          hid_demo.keyboard_state = HID_KEYBOARD_START;
          break;

        case 1:                /* Return */
          UTIL_LCD_TRACE_ClearTextZone();
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
          UTIL_LCD_DisplayStringAtLine(18,
                              (uint8_t *)
                              "Use [Joystick Left/Right] to scroll up/down");
          hid_demo.state = HID_DEMO_REENUMERATE;
          hid_demo.select = 0;
          break;

        default:
          break;
        }
      }
    }
    break;

  case HID_KEYBOARD_START:
    USR_KEYBRD_Init();
    hid_demo.keyboard_state = HID_KEYBOARD_WAIT;
    break;

  default:
    break;
  }
}

/**
  * @brief  Init Keyboard window.
  * @param  None
  * @retval None
  */
static void USR_KEYBRD_Init(void)
{
  UTIL_LCD_TRACE_ClearTextZone();
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);

  UTIL_LCD_DisplayStringAtLine(4,
                              (uint8_t *)
                              "Use Keyboard to type characters:                                                            ");
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

  KeybrdCharYpos = KYBRD_FIRST_LINE;
  KeybrdCharXpos = KYBRD_FIRST_COLUMN;
}

/**
  * @brief  Processes Keyboard data.
  * @param  data: Keyboard data to be displayed
  * @retval None
  */
void USR_KEYBRD_ProcessData(uint8_t data)
{
  if (data == '\n')
  {
    KeybrdCharXpos = KYBRD_FIRST_COLUMN;

    /* Increment char Y position */
    KeybrdCharYpos += SMALL_FONT_LINE_WIDTH;

    if (KeybrdCharYpos > KYBRD_LAST_LINE)
    {
      UTIL_LCD_TRACE_ClearTextZone();
      KeybrdCharYpos = KYBRD_FIRST_LINE;
      KeybrdCharXpos = KYBRD_FIRST_COLUMN;
    }
  }
  else if (data == '\r')
  {
    /* Manage deletion of character and update cursor location */
    if (KeybrdCharXpos == KYBRD_FIRST_COLUMN)
    {
      /* First character of first line to be deleted */
      if (KeybrdCharYpos == KYBRD_FIRST_LINE)
      {
        KeybrdCharXpos = KYBRD_FIRST_COLUMN;
      }
      else
      {
        KeybrdCharYpos -= SMALL_FONT_LINE_WIDTH;
        KeybrdCharXpos = (KYBRD_LAST_COLUMN - SMALL_FONT_COLUMN_WIDTH);
      }
    }
    else
    {
      if (CurrentLastXpos[KeybrdCharYpos] > KYBRD_FIRST_COLUMN)
      {
        CurrentLastXpos[KeybrdCharYpos] -= SMALL_FONT_COLUMN_WIDTH;
        KeybrdCharXpos = CurrentLastXpos[KeybrdCharYpos];
      }
      else if (KeybrdCharYpos > KYBRD_FIRST_LINE)
      {
        KeybrdCharYpos -= SMALL_FONT_LINE_WIDTH;
        CurrentLastXpos[KeybrdCharYpos] -= SMALL_FONT_COLUMN_WIDTH;
        KeybrdCharXpos = CurrentLastXpos[KeybrdCharYpos];
      }
      else
      {
      }
    }
    UTIL_LCD_DisplayChar(CurrentLastXpos[KeybrdCharYpos], KeybrdCharYpos , ' ');
  }
  else
  {
    /* Update the cursor position on LCD */
    UTIL_LCD_DisplayChar(KeybrdCharXpos, KeybrdCharYpos, data);

    /* Increment char X position */
    KeybrdCharXpos += SMALL_FONT_COLUMN_WIDTH;

    CurrentLastXpos[KeybrdCharYpos] = KeybrdCharXpos;
    /* Check if the X position has reached the last column */
    if (KeybrdCharXpos == KYBRD_LAST_COLUMN)
    {
      KeybrdCharXpos = KYBRD_FIRST_COLUMN;

      /* Increment char Y position */
      KeybrdCharYpos += SMALL_FONT_LINE_WIDTH;
    }

    if (KeybrdCharYpos > KYBRD_LAST_LINE)
    {
      UTIL_LCD_TRACE_ClearTextZone();
      KeybrdCharYpos = KYBRD_FIRST_LINE;
      /* Start New Display of the cursor position on LCD */
      UTIL_LCD_DisplayChar(KeybrdCharXpos, KeybrdCharYpos, data);
    }
  }
}

