/**
  ******************************************************************************
  * @file    USB_Host/HID_RTOS/CM7/Src/mouse.c
  * @author  MCD Application Team
  * @brief   This file implements Functions for mouse menu
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
extern USBH_HandleTypeDef hUSB_Host;

/* Private define ------------------------------------------------------------ */

/* Left Button Report data */
#define HID_MOUSE_BUTTON1               0x01
/* Right Button Report data */
#define HID_MOUSE_BUTTON2               0x02
/* Middle Button Report data */
#define HID_MOUSE_BUTTON3               0x04

/* Mouse directions */
#define MOUSE_TOP_DIR                   0x80
#define MOUSE_BOTTOM_DIR                0x00
#define MOUSE_LEFT_DIR                  0x80
#define MOUSE_RIGHT_DIR                 0x00

#define MOUSE_WINDOW_X                  140
#define MOUSE_WINDOW_Y                  100
#define MOUSE_WINDOW_X_MAX              181
#define MOUSE_WINDOW_Y_MIN              101
#define MOUSE_WINDOW_HEIGHT             80
#define MOUSE_WINDOW_WIDTH              200

#define HID_MOUSE_BUTTON_HEIGHT         15
#define HID_MOUSE_BUTTON_WIDTH          25
#define HID_MOUSE_BUTTON1_XCHORD        145
#define HID_MOUSE_BUTTON2_XCHORD        225
#define HID_MOUSE_BUTTON3_XCHORD        315
#define HID_MOUSE_BUTTON_YCHORD         185

#define MOUSE_LEFT_MOVE                  1
#define MOUSE_RIGHT_MOVE                 2
#define MOUSE_UP_MOVE                    3
#define MOUSE_DOWN_MOVE                  4

#define HID_MOUSE_HEIGHTLSB              2
#define HID_MOUSE_WIDTHLSB               2
#define HID_MOUSE_RES_X                  4
#define HID_MOUSE_RES_Y                  4

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
static void USR_MOUSE_Init(void);
static void HID_MOUSE_UpdatePosition(int8_t x, int8_t y);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Manages Mouse Menu Process.
  * @param  None
  * @retval None
  */
void HID_MouseMenuProcess(void)
{
  switch (hid_demo.mouse_state)
  {
  case HID_MOUSE_IDLE:
    hid_demo.mouse_state = HID_MOUSE_START;
    UTIL_LCD_ClearStringLine(18);
    HID_SelectItem(DEMO_MOUSE_menu, 0);
    hid_demo.select = 0;
    prev_select = 0;
    break;

  case HID_MOUSE_WAIT:
    if (hid_demo.select != prev_select)
    {
      prev_select = hid_demo.select;
      HID_SelectItem(DEMO_MOUSE_menu, hid_demo.select & 0x7F);

      /* Handle select item */
      if (hid_demo.select & 0x80)
      {
        hid_demo.select &= 0x7F;
        switch (hid_demo.select)
        {
        case 0:
          hid_demo.mouse_state = HID_MOUSE_START;
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

  case HID_MOUSE_START:
    USR_MOUSE_Init();
    hid_demo.mouse_state = HID_MOUSE_WAIT;
    HID_MOUSE_UpdatePosition(0, 0);
    break;

  default:
    break;
  }
}

/**
  * @brief  Init Mouse window.
  * @param  None
  * @retval None
  */
static void USR_MOUSE_Init(void)
{
  UTIL_LCD_TRACE_ClearTextZone();
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
  UTIL_LCD_DisplayStringAtLine(4,
                              (uint8_t *)
                              "USB HID Host Mouse Demo...                     ");
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

  /* Display Mouse Window */
UTIL_LCD_DrawRect(MOUSE_WINDOW_X, MOUSE_WINDOW_Y, MOUSE_WINDOW_WIDTH,
                   MOUSE_WINDOW_HEIGHT,UTIL_LCD_COLOR_WHITE);
  HID_MOUSE_ButtonReleased(0);
  HID_MOUSE_ButtonReleased(1);
  HID_MOUSE_ButtonReleased(2);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);

  HID_MOUSE_UpdatePosition(0, 0);
}

/**
  * @brief  Processes Mouse data.
  * @param  data: Mouse data to be displayed
  * @retval None
  */
void USR_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef * data)
{
  if ((data->x != 0) && (data->y != 0))
  {
    HID_MOUSE_UpdatePosition(data->x, data->y);
  }
}

/**
  * @brief  Handles mouse scroll to update the mouse position on display window.
  * @param  x: USB HID Mouse X co-ordinate
  * @param  y: USB HID Mouse Y co-ordinate
  * @retval None
  */
static void HID_MOUSE_UpdatePosition(int8_t x, int8_t y)
{
  static int16_t x_loc = 0, y_loc = 0;
  static int16_t prev_x = 5, prev_y = 1;

  if ((x != 0) || (y != 0))
  {
    x_loc += x / 2;
    y_loc += y / 10;

    if (y_loc > MOUSE_WINDOW_HEIGHT - 21)
    {
      y_loc = MOUSE_WINDOW_HEIGHT - 21;
    }
    if (x_loc > MOUSE_WINDOW_WIDTH - 15)
    {
      x_loc = MOUSE_WINDOW_WIDTH - 15;
    }

    if (y_loc < 2)
    {
      y_loc = 2;
    }
    if (x_loc < 2)
    {
      x_loc = 2;
    }
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DisplayChar(MOUSE_WINDOW_X + prev_x, MOUSE_WINDOW_Y + prev_y, 'x');

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DisplayChar(MOUSE_WINDOW_X + x_loc, MOUSE_WINDOW_Y + y_loc, 'x');

    prev_x = x_loc;
    prev_y = y_loc;
  }
}

/**
  * @brief  Handles mouse button press.
  * @param  button_idx: Mouse button pressed
  * @retval None
  */
void HID_MOUSE_ButtonPressed(uint8_t button_idx)
{
  /* Set the color for button press status */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);

  /* Change the color of button pressed to indicate button press */
  switch (button_idx)
  {
    /* Left Button Pressed */
  case 0:
    UTIL_LCD_FillRect(HID_MOUSE_BUTTON1_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT,UTIL_LCD_COLOR_BLUE);
    break;

    /* Right Button Pressed */
  case 1:
    UTIL_LCD_FillRect(HID_MOUSE_BUTTON2_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT,UTIL_LCD_COLOR_BLUE);
    break;

    /* Middle button Pressed */
  case 2:
    UTIL_LCD_FillRect(HID_MOUSE_BUTTON3_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT,UTIL_LCD_COLOR_BLUE);
    break;
  }
}

/**
  * @brief  Handles mouse button release.
  * @param  button_idx: Mouse button released
  * @retval None
  */
void HID_MOUSE_ButtonReleased(uint8_t button_idx)
{
  /* Set the color for release status */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);

  /* Change the color of button released to default button color */
  switch (button_idx)
  {
    /* Left Button Released */
  case 0:
    UTIL_LCD_FillRect(HID_MOUSE_BUTTON1_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT,UTIL_LCD_COLOR_WHITE);
    break;

    /* Right Button Released */
  case 1:
    UTIL_LCD_FillRect(HID_MOUSE_BUTTON2_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT,UTIL_LCD_COLOR_WHITE);
    break;

    /* Middle Button Released */
  case 2:
    UTIL_LCD_FillRect(HID_MOUSE_BUTTON3_XCHORD, HID_MOUSE_BUTTON_YCHORD,
                     HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT,UTIL_LCD_COLOR_WHITE);
    break;
  }
}

