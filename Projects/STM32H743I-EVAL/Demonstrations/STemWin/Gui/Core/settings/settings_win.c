/**
  ******************************************************************************
  * @file    settings_win.c
  * @author  MCD Application Team
  * @brief   settings functions
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

/* Includes ------------------------------------------------------------------*/
#include "DIALOG.h"
#include "k_module.h"
#include "settings_res.c"
#include "k_modules_res.h"
#include "cpu_utils.h"

/** @addtogroup SETTINGS_MODULE
  * @{
  */

/** @defgroup SETTINGS
  * @brief settings routines
  * @{
  */
  
/* External variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmbkc_16bit;
extern GUI_CONST_STORAGE GUI_BITMAP bmmenu;

/* Private function prototypes -----------------------------------------------*/
void Startup_info(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private defines -----------------------------------------------------------*/
#define ID_WINDOW_0               (GUI_ID_USER + 0x00)
#define ID_IMAGE_0                (GUI_ID_USER + 0x01)
#define ID_TEXT_BOARD             (GUI_ID_USER + 0x02)
#define ID_TEXT_CORE              (GUI_ID_USER + 0x03)
#define ID_TEXT_CPU               (GUI_ID_USER + 0x04)
#define ID_TEXT_VERSION           (GUI_ID_USER + 0x05)
#define ID_TEXT_COPYRIGHT         (GUI_ID_USER + 0x06)
#define ID_TEXT_BOARD_1           (GUI_ID_USER + 0x07)
#define ID_TEXT_BOARD_2           (GUI_ID_USER + 0x08)
#define ID_TEXT_CORE_1            (GUI_ID_USER + 0x09)
#define ID_TEXT_CPU_1             (GUI_ID_USER + 0x0A)
#define ID_TEXT_VERSION_1         (GUI_ID_USER + 0x0B)
#define ID_TEXT_TITLE             (GUI_ID_USER + 0x0C)
#define ID_BUTTON_EXIT            (GUI_ID_USER + 0x0D)
#define ID_IMAGE_BOARD            (GUI_ID_USER + 0x0E)
#define ID_IMAGE_MCU              (GUI_ID_USER + 0x0F)
#define ID_IMAGE_CPU              (GUI_ID_USER + 0x10)
#define ID_IMAGE_FVERSION         (GUI_ID_USER + 0x11)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "", ID_WINDOW_0,                    0,    0,  640, 480, 0, 0x64, 0 }, 
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_0,                 33,  180, 574, 51,  0, 0,    0 },
  { TEXT_CreateIndirect, "System Information", ID_TEXT_TITLE,  200,  40, 300, 40,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "Board", ID_TEXT_BOARD,               65,  150, 100, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "MCU", ID_TEXT_CORE,                 220, 150, 100, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "CPU Speed", ID_TEXT_CPU,             350, 150, 100, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "Firm. Version", ID_TEXT_VERSION,   495, 150, 117, 20,  0, 0x0,  0 },  

  { TEXT_CreateIndirect, " STM32H743I", ID_TEXT_BOARD_1,  30,  295, 115, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "   -EVAL  ",  ID_TEXT_BOARD_2,  37,  315, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " STM32H7", ID_TEXT_CORE_1,     195, 310, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " 400MHz", ID_TEXT_CPU_1,       360, 310, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "V1.8.0", ID_TEXT_VERSION_1,    520, 310, 100, 20, 0, 0x0, 0 },
  
  { TEXT_CreateIndirect, "Copyright (c) STMicroelectronics 2017", ID_TEXT_COPYRIGHT, 400, 450, 240, 20, 0, 0x0, 0 },
};


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_exit(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmmenu, 13, 17);
}

/**
  * @brief  callback for Exit button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_exit(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_exit(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

static void _cbDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int Id, NCode;

  switch (pMsg->MsgId) 
  {
    case WM_INIT_DIALOG:
      hItem = BUTTON_CreateEx(580, 17, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      
      /* Initialization of 'Board : STM32H743I' */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      /* Initialization of 'Core: STM32H7 Series' */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      /* Initialization of 'CPU Speed : 400MHz' */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      /* Initialization of 'Firmware Version */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      /* Initialization of 'Board : STM32H743I' */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD_1);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, 0xFF39A9DC);
      
      /* Initialization of 'Board : STM32H743I' */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD_2);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, 0xFF39A9DC);
      
      /* Initialization of 'Core: STM32H7 Series' */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE_1);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, 0xFF39A9DC);
      
      /* Initialization of 'CPU Speed : 400MHz' */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU_1);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, 0xFF39A9DC);
      
      /* Initialization of 'Firmware Version */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION_1);
      TEXT_SetFont(hItem, &GUI_Font20B_1);
      TEXT_SetTextColor(hItem, 0xFF39A9DC); 
      
      /* ST Copyright */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_COPYRIGHT);
      TEXT_SetFont(hItem, GUI_FONT_16_ASCII);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      /* Screen title */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
      TEXT_SetFont(hItem, GUI_FONT_32B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      IMAGE_CreateEx(37,  190, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_IMAGE_BOARD);
      IMAGE_CreateEx(192, 190, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_IMAGE_MCU);
      IMAGE_CreateEx(347, 190, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_IMAGE_CPU);
      IMAGE_CreateEx(502, 190, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_IMAGE_FVERSION);
      break;
    case WM_PAINT:
      GUI_SetColor(0xFF39A9DC);
      GUI_DrawBitmap(&bmbkc_16bit, 0, 0);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_BOARD);
      IMAGE_SetBitmap(hItem, &bmBoard);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_MCU);
      IMAGE_SetBitmap(hItem, &bmChip);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_CPU);
      IMAGE_SetBitmap(hItem, &bmSpeed);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_FVERSION);
      IMAGE_SetBitmap(hItem, &bmFirmware);
      
      GUI_AA_DrawRoundedRect(20,  140, 155, 340, 30);
      GUI_AA_DrawRoundedRect(175, 140, 310, 340, 30);
      GUI_AA_DrawRoundedRect(330, 140, 465, 340, 30);
      GUI_AA_DrawRoundedRect(485, 140, 620, 340, 30);
      break;
      
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch(Id)
      {
        case ID_BUTTON_EXIT:
          switch(NCode)
          {
            case WM_NOTIFICATION_CLICKED:
              break;
            case WM_NOTIFICATION_RELEASED:
              GUI_EndDialog(pMsg->hWin, 0);
              break;
          }
          break;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Setting startup window
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
void Startup_info(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */
  
