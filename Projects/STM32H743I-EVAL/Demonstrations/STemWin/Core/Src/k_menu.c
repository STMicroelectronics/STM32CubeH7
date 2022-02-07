/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team
  * @brief   This file provides the first demo menu functions
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
#include "main.h"
#include "k_modules_res.c"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel MENU routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmmenu;

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
#define DEMO_HOME_SIGNATURE 0x5AA55BBB

/* Private variables ---------------------------------------------------------*/
BUTTON_Handle hButCenter;
WM_HWIN       hMotion;
int id;

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontRounded22_Prop1 = {
   0x0020                                   /* first character */
  ,0x007E                                   /* last character */ 
  ,&GUI_FontRounded22_CharInfo[  0]         /* address of first character */
  ,(GUI_CONST_STORAGE GUI_FONT_PROP_EXT *)0 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT GUI_FontRounded22 = {
   GUI_FONTTYPE_PROP_AA4_EXT /* type of font    */
  ,22                        /* height of font  */
  ,22                        /* space of font y */
  ,1                         /* magnification x */
  ,1                         /* magnification y */
  ,{&GUI_FontRounded22_Prop1}
  ,22                        /* Baseline */
  ,11                        /* Height of lowercase characters */
  ,15                        /* Height of capital characters */
};

/* Table of the bitmaps to be used on the Main Menu */
static const BITMAP_ITEM _aBitmapItem[] = {
  {&bmaudio_player_idle_210x210,     &bmaudio_player_clicked_210x210,    "AUDIO PLAYER"       , 0},
  {&bmvideo_player_idle_210x210,     &bmvideo_player_clicked_210x210,    "VIDEO PLAYER"       , 1 },
  {&bmvideo_game_idle_210x210,       &bmvideo_game_clicked_210x210,      "GAME"               , 2 },
  {&bmclock_date_idle_210x210,       &bmclock_date_clicked_210x210,      "CLOCK & WEATHER"    , 3 },
  {&bmgraphic_effects_idle_210x210,  &bmgraphic_effects_clicked_210x210, "GRAPHIC EFFECT"     , 4 },
};

static WM_HTIMER hTimerTime;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define ID_BUTTON_INFO            (GUI_ID_USER + 0x00)
#define ID_BUTTON_MODULE          (GUI_ID_USER + 0x01)
#define ID_BUTTON_BACK            (GUI_ID_USER + 0x02)
#define ID_MSD                    (GUI_ID_USER + 0x03)
#define ID_BUTTON_MODULE0         (GUI_ID_USER + 0x04)
#define ID_BUTTON_MODULE1         (GUI_ID_USER + 0x05)
#define ID_BUTTON_MODULE2         (GUI_ID_USER + 0x06)
#define ID_BUTTON_MODULE3         (GUI_ID_USER + 0x07)

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern void Startup_info(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);


/**
  * @brief  Paints back button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_back(BUTTON_Handle hObj) 
{ 
  if(module_prop[id].in_use == 0)
    GUI_DrawBitmap(&bmmenu, 13, 17);  
}

/**
  * @brief  callback for back button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_back(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_back(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for ok button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_ok(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      /* Don't draw anything here */
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
  if(module_prop[id].in_use == 0)
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
  int       Id, NCode;
  RTC_HandleTypeDef RtcHandle; 
  WM_HWIN hItem;
  
  switch (pMsg->MsgId) 
  {
    case WM_TIMER:
      hItem = WM_GetDialogItem(pMsg->hWin, ID_MSD);
      WM_DeleteWindow(hItem);
      WM_DeleteTimer(hTimerTime);
      break;
    case WM_NOTIFY_PARENT:
      /* Notification code */
      NCode = pMsg->Data.v;
      Id    = WM_GetId(pMsg->hWinSrc);
      switch (NCode) 
      {
        case WM_NOTIFICATION_RELEASED:
          switch (Id)
          {
            case ID_BUTTON_MODULE:
              if((id == 0) || (id == 1))
              {
                if(k_StorageGetStatus(MSD_DISK_UNIT))
                {
                  module_prop[id].in_use = 1;
                  module_prop[id].module->startup(pMsg->hWin, - WM_GetWindowOrgX(pMsg->hWin), 0);
                }
                else
                {
                  hItem = TEXT_CreateEx(400, 450, 180, 20, pMsg->hWin, WM_CF_SHOW, TEXT_CF_LEFT, ID_MSD, "Please insert SDCard");
                  TEXT_SetFont(hItem, GUI_FONT_20_1);
                  TEXT_SetDefaultTextColor(0xFF000000);
                  TEXT_SetTextColor(hItem, GUI_WHITE);
                  hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 2000, 0);
                }
              }
              else
              {
                module_prop[id].in_use = 1;
                WM_ValidateWindow(WM_HBKWIN);
                module_prop[id].module->startup(pMsg->hWin, - WM_GetWindowOrgX(pMsg->hWin), 0);
              }
              break;
            case ID_BUTTON_MODULE0:
              WM_MOTION_SetMovement(hMotion, GUI_COORD_X, 500, 100);
              break;
            case ID_BUTTON_MODULE1:
              WM_MOTION_SetMovement(hMotion, GUI_COORD_X, -500, 100);
              break;
            case ID_BUTTON_MODULE2:
              WM_MOTION_SetMovement(hMotion, GUI_COORD_X, 500, 200);
              break;
            case ID_BUTTON_MODULE3:
              WM_MOTION_SetMovement(hMotion, GUI_COORD_X, -500, 200);
              break;
            case ID_BUTTON_INFO:
              Startup_info(pMsg->hWin, 0, 0);
              break;
            case ID_BUTTON_BACK:
              RtcHandle.Instance = RTC;
              __HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);
              RTC->BKP1R = DEMO_HOME_SIGNATURE;
              /* Enable the write protection for RTC registers */
              __HAL_RTC_WRITEPROTECTION_ENABLE(&RtcHandle);
              NVIC_SystemReset();
              break;
            }
          break;
        case WM_NOTIFICATION_CHILD_DELETED:
          module_prop[id].in_use = 0;
          break;
      }
      break;
    case WM_PAINT:
      if(module_prop[id].in_use == 0)
        GUI_DrawBitmap(&bmbkc_16bit, 0, 0);
      break;
  }
}

/**
  * @brief  Callback routine of the menu
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDraw(WM_MESSAGE * pMsg) 
{
  WM_HWIN     hWin;
  PARA        *pPara;
  int         i, a1000, Swap;
  I32         SinHQ;
  I32         CosHQ;
  I32         a;
  ITEM_INFO   ItemInfo;

  hWin = pMsg->hWin;
  
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      if(module_prop[id].in_use == 0)
      {
        WM_GetUserData(hWin, &pPara, sizeof(pPara));
        /* One time initialization of parameter structure */
        if (pPara->xSizeItem == 0) 
        {
          pPara->xSizeWindow = WM_GetWindowSizeX(hWin);
          pPara->ySizeWindow = WM_GetWindowSizeY(hWin);
          pPara->xSizeItem   = pPara->pBitmapItem[0].pBitmap->XSize;
          pPara->ySizeItem   = pPara->pBitmapItem[0].pBitmap->YSize;
          pPara->rx          = ( pPara->xSizeWindow      - pPara->xSizeItem) /2;
          pPara->ry          = ((pPara->ySizeWindow) - pPara->ySizeItem)/2;
          pPara->mx          = pPara->xSizeWindow / 2;
          pPara->my          = pPara->ry + 90;
        }
        /* Calculate current positions of items */
        a1000 = (pPara->Pos * 3600) / pPara->NumItems;
        for (i = 0; i < pPara->NumItems; i++) 
        {
          a = 90000 + a1000 + (i * 360000) / pPara->NumItems;
          SinHQ = GUI__SinHQ(a);
          CosHQ = GUI__CosHQ(a);
          pPara->pItemInfo[i].Index = i;
          pPara->pItemInfo[i].xPos  = pPara->mx - ((CosHQ * pPara->rx) >> 16);
          pPara->pItemInfo[i].yPos  = pPara->my + ((SinHQ * pPara->ry) >> 16);
        }
        /* Sort items to be able to draw background items first */
        do
        {
          Swap = 0;
          for (i = 0; i < (pPara->NumItems - 1); i++) 
          {
            if (pPara->pItemInfo[i].yPos > pPara->pItemInfo[i + 1].yPos) 
            {
              ItemInfo                = pPara->pItemInfo[i];
              pPara->pItemInfo[i]     = pPara->pItemInfo[i + 1];
              pPara->pItemInfo[i + 1] = ItemInfo;
              Swap = 1;
            }
          }
        } while (Swap);
        
        /* Draw items */
        for (i = 0; i < pPara->NumItems; i++) 
        {
          GUI_DrawBitmap((pPara->pBitmapItem + pPara->pItemInfo[i].Index)->pBitmap, pPara->pItemInfo[i].xPos - pPara->xSizeItem / 2, pPara->pItemInfo[i].yPos - pPara->ySizeItem / 2);
        }
        /* Display the relative text on the final Move */
        if (pPara->FinalMove | (pPara->Pos == 0)) 
        {
          GUI_SetTextMode(GUI_TM_TRANS);
          GUI_SetFont(&GUI_FontRounded22);
          GUI_SetColor(GUI_WHITE);
          GUI_DispStringHCenterAt((pPara->pBitmapItem + pPara->pItemInfo[pPara->NumItems - 1].Index)->pText, pPara->xSizeWindow / 2, pPara->ySizeWindow - 25);
          GUI_DrawBitmap((pPara->pBitmapItem + pPara->pItemInfo[pPara->NumItems - 1].Index)->pBitmapClicked, pPara->pItemInfo[pPara->NumItems - 1].xPos - pPara->xSizeItem / 2, pPara->pItemInfo[pPara->NumItems - 1].yPos - pPara->ySizeItem / 2);
          id = (pPara->pBitmapItem + pPara->pItemInfo[pPara->NumItems - 1].Index)->Id;  
          WM_SendMessageNoPara(WM_HBKWIN, WM_NOTIFY_PARENT);
        }
    break;
    }
  }
}

/**
  * @brief  Callback routine of transparent motion window which 
  *         is responsible of processing the motion messages.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMotion(WM_MESSAGE * pMsg) 
{
  WM_MOTION_INFO * pInfo;
  WM_HWIN          hWin;
  PARA           * pPara;

  hWin = pMsg->hWin;
  
  switch (pMsg->MsgId) 
  {
    case WM_MOTION:
      WM_GetUserData(hWin, &pPara, sizeof(pPara));
      pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
      switch (pInfo->Cmd) 
      {
        case WM_MOTION_INIT:
          pInfo->Flags = WM_CF_MOTION_X | WM_MOTION_MANAGE_BY_WINDOW;
          pInfo->SnapX = 100;
          break;
        case WM_MOTION_MOVE:
          pPara->FinalMove = pInfo->FinalMove;
          pPara->Pos += pInfo->dx;
          if (pPara->Pos > pPara->NumItems * 100) 
          {
            pPara->Pos -= pPara->NumItems * 100;
          }
          if (pPara->Pos < 0) 
          {
            pPara->Pos += pPara->NumItems * 100;
          }
          WM_Invalidate(WM_GetParent(hWin));
          break;
        case WM_MOTION_GETPOS:
          pInfo->xPos = pPara->Pos;
          break;
      }
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
  static ITEM_INFO  aItemInfo[GUI_COUNTOF(_aBitmapItem)];
  static PARA       Para;
  WM_HWIN           hDraw;
 
  PARA            * pPara;
  int               xSizeWindow;
  int               ySizeWindow;
  int               xSize;
  int               ySize; 
  BUTTON_Handle     hBut;
  BUTTON_Handle     hBut_info;
  
  /* Enable the window motion */
  WM_MOTION_Enable(1);
  /* Set the buttons to react only on level changes, only.*/
  BUTTON_SetReactOnLevel();
  
  /* Create the global window */
  WM_SetCallback(WM_HBKWIN, _cbBk);
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  
  /* Initialize parameter structure for items to be shown */
  pPara = &Para;
  pPara->NumItems    = GUI_COUNTOF(_aBitmapItem);
  pPara->pBitmapItem = _aBitmapItem;
  pPara->pItemInfo   = aItemInfo;
  pPara->Pos         = 0;
  /* Create the menu window */
  hDraw              = WM_CreateWindowAsChild(20, 40, xSize - 40, ySize - 80, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbDraw, sizeof(pPara));
  
  /* Create transparent window which receives the motion messages */
  xSizeWindow        = WM_GetWindowSizeX(hDraw);
  ySizeWindow        = WM_GetWindowSizeY(hDraw);
  hMotion            = WM_CreateWindowAsChild(0, ySizeWindow / 2, xSizeWindow, ySizeWindow / 2, hDraw, WM_CF_SHOW | WM_CF_HASTRANS, _cbMotion, sizeof(pPara));
  
  /* Add pointer to parameter structure to windows */
  WM_SetUserData(hDraw,   &pPara, sizeof(pPara));
  WM_SetUserData(hMotion, &pPara, sizeof(pPara));
  
  /* Create different buttons to be used */
  hBut = BUTTON_CreateEx(260, 265, 118, 118, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE);
  WM_SetCallback(hBut, _cbButton_ok);
  
  hBut = BUTTON_CreateEx(115, 214, 118, 118, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE0);
  WM_SetCallback(hBut, _cbButton_ok);

  hBut = BUTTON_CreateEx(486, 214, 118, 118, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE1);
  WM_SetCallback(hBut, _cbButton_ok);
  
  hBut = BUTTON_CreateEx(186, 108, 118, 118, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE2);
  WM_SetCallback(hBut, _cbButton_ok);

  hBut = BUTTON_CreateEx(415, 108, 118, 118, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE3);
  WM_SetCallback(hBut, _cbButton_ok);

  hBut_info = BUTTON_CreateEx(580, 420, 50, 50, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_INFO);
  WM_SetCallback(hBut_info, _cbButton_info);  
    
  hBut = BUTTON_CreateEx(580, 17, 50, 50, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_BACK);  
  WM_SetCallback(hBut, _cbButton_back);
}

