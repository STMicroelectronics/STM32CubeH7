/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team
  * @brief   This file provides the first demo menu functions
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "k_modules_res.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel MENU routines
  * @{
  */

/* Module configuration ------------------------------------------------------*/
#define DEMO_CHECK_MEMORY_LEAK

/* External variables --------------------------------------------------------*/
extern void Startup_info(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
extern uint32_t AutoDemoTimeOutMs;

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  const GUI_BITMAP * pBitmap;
  const GUI_BITMAP * pBitmapClicked;
  const char       * pText;
  int              Id;
} BITMAP_ITEM;

typedef struct {
  int xPos;
  int yPos;
  int Index;
} ITEM_INFO;

typedef struct {
  int                 Pos;
  int                 NumItems;
  int                 xSizeItem,   ySizeItem;
  int                 xSizeWindow, ySizeWindow;
  int                 rx,          ry;
  int                 mx,          my;
  int                 FinalMove;
  const BITMAP_ITEM * pBitmapItem;
  ITEM_INFO         * pItemInfo;
} PARA;

/* Private defines -----------------------------------------------------------*/
#define ID_TEXT_STORAGE           (GUI_ID_USER + 0x09)

#define ID_BUTTON_INFO            (GUI_ID_USER + 0x10)
#define ID_BUTTON_MODULE1         (GUI_ID_USER + 0x11)
#define ID_BUTTON_MODULE2         (GUI_ID_USER + 0x12)

#define ID_TIMER_STORAGE          (GUI_ID_USER + 0x20)
#define ID_TIMER_AUTO_DEMO        (GUI_ID_USER + 0x21)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static WM_HWIN        hbuttons = 0;
static WM_HTIMER      hTimerStorage = 0;
static uint8_t        current_module_id = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints module1 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_module1(BUTTON_Handle hObj)
{
  if(module_prop[current_module_id].in_use == 0)
    GUI_DrawBitmap(&bmgraphic_effects_idle, 0, 0);
}

/**
  * @brief  callback for module1 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_module1(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_module1(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints module2 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_module2(BUTTON_Handle hObj)
{
  if(module_prop[current_module_id].in_use == 0)
    GUI_DrawBitmap(&bmvideo_game_idle, 0, 0);
}

/**
  * @brief  callback for module1 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_module2(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_module2(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints info button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_info(BUTTON_Handle hObj)
{
  if(module_prop[current_module_id].in_use == 0)
    GUI_DrawBitmap(&bminfo, 0, 0);
}

/**
  * @brief  callback for info button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_info(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_info(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of the window used
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg)
{
  int Id, NCode;
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
    case WM_TIMER:

      Id = WM_GetTimerId(pMsg->Data.v);

      if (Id == ID_TIMER_STORAGE)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_STORAGE);
        WM_HideWindow(hItem);
        WM_DeleteTimer(hTimerStorage);
        hTimerStorage = 0;
      }
      break;
    case WM_NOTIFY_PARENT:
      /* Notification code */
      NCode = pMsg->Data.v;
      Id    = WM_GetId(pMsg->hWinSrc);
      switch (NCode)
      {
        case WM_NOTIFICATION_CLICKED:

          if(hTimerStorage)
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_STORAGE);
            WM_HideWindow(hItem);
            WM_DeleteTimer(hTimerStorage);
            hTimerStorage = 0;
          }

          switch (Id)
          {
            case ID_BUTTON_MODULE1:
              if(Storage_GetStatus(MSD_DISK_UNIT) )
              {
                module_prop[1].in_use = 1;
                module_prop[1].module->startup(pMsg->hWin, 0, 0);
              }
              else
              {
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_STORAGE);
                WM_ShowWindow(hItem);
                TEXT_SetFont(hItem, GUI_FONT_8_1);
                TEXT_SetTextColor(hItem, GUI_WHITE);
                hTimerStorage = WM_CreateTimer(pMsg->hWin, ID_TIMER_STORAGE, 2000, 0);
              }
              break;
            case ID_BUTTON_MODULE2:
              module_prop[0].in_use = 1;
              module_prop[0].module->startup(pMsg->hWin, 0, 0);
              break;
            case ID_BUTTON_INFO:
              Startup_info(pMsg->hWin, 0, 0);
              break;
            }
          break;
        case WM_NOTIFICATION_CHILD_DELETED:
          module_prop[0].in_use = 0;
          module_prop[1].in_use = 0;
          break;
      }
        break;
    case WM_PAINT:
  if(module_prop[current_module_id].in_use == 0)
    GUI_DrawBitmap(&bmbkc_16bit, 0, 0);
  break;
  }
}

/**
  * @brief  Mainmenu function
  *         is responsible of processing the motion messages.
  * @param  None
  * @retval None
  */
void k_InitMenu(void)
{

  int               xSize;
  int               ySize;
  BUTTON_Handle     hItem;

  current_module_id = 0;

  /* Create the global window */
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();


  hbuttons = WM_CreateWindowAsChild(0, 0, xSize, ySize, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbBk, 0);

  hItem = BUTTON_CreateEx(90, 76, 120, 120, hbuttons , WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE1);
  WM_SetCallback(hItem, _cbButton_module1);

  hItem = BUTTON_CreateEx(270, 76, 120, 120, hbuttons , WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE2);
  WM_SetCallback(hItem, _cbButton_module2);

  hItem = BUTTON_CreateEx(440, 232, 30, 30, hbuttons , WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_INFO);
  WM_SetCallback(hItem, _cbButton_info);

  hItem = TEXT_CreateEx(287, 243, 145, 10, hbuttons , WM_CF_HIDE, TEXT_CF_LEFT, ID_TEXT_STORAGE, "Please insert media storage");
  TEXT_SetFont(hItem, GUI_FONT_8_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);
}

