/**
  ******************************************************************************
  * @file    USB_Host/AUDIO_Standalone/Src/menu.c
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
FILELIST_FileTypeDef file_list;
uint8_t prev_select = 0;
AUDIO_DEMO_SelectMode audio_select_mode;
AUDIO_DEMO_StateMachine audio_demo;
AUDIO_PLAYBACK_StateTypeDef audio_state;

uint8_t *AUDIO_main_menu[] = {
  (uint8_t *)
    "      1 - Explore audio file                                         ",
  (uint8_t *)
    "      2 - Start audio Player                                         ",
  (uint8_t *)
    "      3 - Re-Enumerate                                               ",
};

/* Private function prototypes ----------------------------------------------- */
static uint8_t Audio_ShowWavFiles(void);
static void Audio_ChangeSelectMode(AUDIO_DEMO_SelectMode select_mode);
static void LCD_ClearTextZone(void);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void AUDIO_MenuInit(void)
{
  audio_demo.state = AUDIO_DEMO_IDLE;
  audio_select_mode = AUDIO_SELECT_MENU;
  AUDIO_MenuProcess();
}

/**
  * @brief  Manages Audio Menu Process.
  * @param  None
  * @retval None
  */
void AUDIO_MenuProcess(void)
{
  switch (audio_demo.state)
  {
  case AUDIO_DEMO_IDLE:
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DisplayStringAtLine(16,
                                (uint8_t *)
                                "                                                 ");
    UTIL_LCD_DisplayStringAtLine(17,
                                (uint8_t *)
                                "Use [Joystick Left/Right] to scroll up/down       ");
    UTIL_LCD_DisplayStringAtLine(18,
                                (uint8_t *)
                                "Use [Joystick Up/Down] to scroll audio menu      ");
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

    AUDIO_MenuSelectItem(AUDIO_main_menu, 0);
    audio_demo.state = AUDIO_DEMO_WAIT;
    audio_demo.select = 0;
    break;

  case AUDIO_DEMO_WAIT:
    if (audio_demo.select != prev_select)
    {
      prev_select = audio_demo.select;
      AUDIO_MenuSelectItem(AUDIO_main_menu, audio_demo.select & 0x7F);

      /* Handle select item */
      if (audio_demo.select & 0x80)
      {
        audio_demo.select &= 0x7F;

        switch (audio_demo.select)
        {
        case 0:
          audio_demo.state = AUDIO_DEMO_EXPLORE;
          break;

        case 1:
          /* Display HMI messages */
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
          UTIL_LCD_DisplayStringAtLine(14,
                                      (uint8_t *)
                                      "                                             ");
          UTIL_LCD_DisplayStringAtLine(15,
                                      (uint8_t *)
                                      "                                             ");
          UTIL_LCD_DisplayStringAtLine(16,
                                      (uint8_t *)
                                      "Use [User Tamper] To Stop and return from player");
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

          /* Set PLAYBACK state and start playing 1st file */
          audio_state = AUDIO_STATE_IDLE;
          audio_demo.state = AUDIO_DEMO_PLAYBACK;
          Audio_ChangeSelectMode(AUDIO_PLAYBACK_CONTROL);
          break;

        case 2:
          audio_demo.state = AUDIO_REENUMERATE;
          break;

        default:
          break;
        }
      }
    }
    break;

  case AUDIO_DEMO_EXPLORE:
    if (Appli_state == APPLICATION_READY)
    {
      if (Audio_ShowWavFiles() > 0)
      {
        LCD_ErrTrace("There is no WAV file on the microSD.\n");
        Audio_ChangeSelectMode(AUDIO_SELECT_MENU);
      }
      else
      {
        audio_demo.state = AUDIO_DEMO_WAIT;
      }
    }
    break;

  case AUDIO_DEMO_PLAYBACK:
    if (Appli_state == APPLICATION_READY)
    {
      if (audio_state == AUDIO_STATE_IDLE)
      {
        /* Start Playing */
        audio_state = AUDIO_STATE_INIT;
        if (AUDIO_Start(0) == AUDIO_ERROR_IO)
        {
          Audio_ChangeSelectMode(AUDIO_SELECT_MENU);
        }
        else
        {
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
          UTIL_LCD_DisplayStringAtLine(10, (uint8_t *) "[  UP   ] : Volume +");
          UTIL_LCD_DisplayStringAtLine(11, (uint8_t *) "[ DOWN  ] : Volume -");
          UTIL_LCD_DisplayStringAtLine(12, (uint8_t *) "[ LEFT  ] : Previous");
          UTIL_LCD_DisplayStringAtLine(13, (uint8_t *) "[ RIGHT ] : Next");
          UTIL_LCD_DisplayStringAtLine(14,
                                      (uint8_t *) "[  SEL  ] : Pause/Resume");
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        }
      }
      else                      /* Not idle */
      {
        if (AUDIO_Process() == AUDIO_ERROR_IO)
        {
          Audio_ChangeSelectMode(AUDIO_SELECT_MENU);
        }
      }
    }
    break;

  case AUDIO_REENUMERATE:
    /* Force Audio Device to re-enumerate */
    USBH_ReEnumerate(&hUSBHost);
    audio_demo.state = AUDIO_DEMO_WAIT;
    break;

  default:
    break;
  }

  if (Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    Audio_ChangeSelectMode(AUDIO_SELECT_MENU);
  }
}

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
void AUDIO_MenuSelectItem(uint8_t ** menu, uint8_t item)
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
    UTIL_LCD_DisplayStringAtLine(20, menu[1]);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_MAGENTA);
    UTIL_LCD_DisplayStringAtLine(21, menu[2]);
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
  * @brief  Probes the Audio joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void AUDIO_MenuProbeKey(uint32_t state)
{
  /* Handle Joystick inputs */
  if (audio_select_mode == AUDIO_SELECT_MENU)
  {
    /* Handle Menu inputs */
    if ((state == JOY_UP) && (audio_demo.select > 0))
    {
      audio_demo.select--;
    }
    else if ((audio_demo.select < 2) && (state == JOY_DOWN))
    {
      audio_demo.select++;
    }
    else if (state == JOY_SEL)
    {
      audio_demo.select |= 0x80;
    }
  }
  else
  {
    AUDIO_PlaybackProbeKey(state);
  }
}

void BSP_JOY_Callback(JOY_TypeDef JOY, JOYPin_TypeDef JoyPin)
{
  if (audio_select_mode == AUDIO_SELECT_MENU)
  {
    AUDIO_MenuProbeKey(JoyPin);

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
  else if (audio_select_mode == AUDIO_PLAYBACK_CONTROL)
  {
    AUDIO_PlaybackProbeKey(JoyPin);
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
    if (audio_demo.state == AUDIO_DEMO_PLAYBACK)
    {
      /* Prevent debounce effect for user Tamper button */
      if ((HAL_GetTick() - debounce_time) > 50)
      {
        debounce_time = HAL_GetTick();
      }
      else
      {
        return;
      }

      /* Change the selection type */
      if (audio_select_mode == AUDIO_SELECT_MENU)
      {
        Audio_ChangeSelectMode(AUDIO_PLAYBACK_CONTROL);
      }
      else if (audio_select_mode == AUDIO_PLAYBACK_CONTROL)
      {
        Audio_ChangeSelectMode(AUDIO_SELECT_MENU);
      }

    }
  }
}

/**
  * @brief  Changes the selection mode.
  * @param  select_mode: Selection mode
  * @retval None
  */
static void Audio_ChangeSelectMode(AUDIO_DEMO_SelectMode select_mode)
{
  if (select_mode == AUDIO_SELECT_MENU)
  {
    AUDIO_MenuSelectItem(AUDIO_main_menu, 0x00);
    LCD_ClearTextZone();
    UTIL_LCD_TRACE_UpdateDisplay();
    audio_demo.state = AUDIO_DEMO_IDLE;
    AUDIO_Stop();
  }
  else if (select_mode == AUDIO_PLAYBACK_CONTROL)
  {
    LCD_ClearTextZone();
    AUDIO_MenuSelectItem(AUDIO_main_menu, 0xFF);
  }
  audio_select_mode = select_mode;
  audio_demo.select = 0;
}

/**
  * @brief  Shows audio file (*.wav) on the root
  * @param  None
  * @retval None
  */
static uint8_t Audio_ShowWavFiles(void)
{
  uint8_t i = 0;
  uint8_t line_idx = 0;

  if ((FileList.ptr > 0) && (BSP_SD_IsDetected(0)))
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    LCD_UsrTrace("audio file(s) [ROOT]:\n\n");

    for (i = 0; i < FileList.ptr; i++)
    {
      line_idx++;
      if (line_idx > 9)
      {
        line_idx = 0;
        LCD_UsrTrace("> Press [Tamper] To Continue.\n");

        /* Tamper Button in polling */
        while (BSP_PB_GetState(BUTTON_TAMPER) != SET)
        {
          /* Wait for User Input */
        }
      }
      LCD_DbgTrace("   |__");
      LCD_DbgTrace((char *)FileList.file[i].name);
      LCD_DbgTrace("\n");
    }
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    LCD_UsrTrace("\nEnd of files list.\n");
    return 0;
  }
  else
  {
    return 1;
  }
}

/**
  * @brief  Clears the text zone.
  * @param  None
  * @retval None
  */
static void LCD_ClearTextZone(void)
{
  uint8_t i = 0;

  for (i = 0; i < 12; i++)
  {
    UTIL_LCD_ClearStringLine(i + 3);
  }
}

