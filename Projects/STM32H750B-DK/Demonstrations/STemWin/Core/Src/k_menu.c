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
#define ID_TEXT_STORAGE           (GUI_ID_USER + 0x00)

#define ID_BUTTON_INFO            (GUI_ID_USER + 0x10)
#define ID_BUTTON_MODULE          (GUI_ID_USER + 0x11)
#define ID_BUTTON_BACK            (GUI_ID_USER + 0x12)
#define ID_BUTTON_MODULE0         (GUI_ID_USER + 0x13)
#define ID_BUTTON_MODULE1         (GUI_ID_USER + 0x14)
#define ID_BUTTON_MODULE2         (GUI_ID_USER + 0x15)
#define ID_BUTTON_MODULE3         (GUI_ID_USER + 0x16)
#define ID_BUTTON_MODULE4         (GUI_ID_USER + 0x17)

#define ID_TIMER_STORAGE          (GUI_ID_USER + 0x20)
#define ID_TIMER_AUTO_DEMO        (GUI_ID_USER + 0x21)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static WM_HWIN        hDraw = 0;
static WM_HWIN        hMotion = 0;
static WM_HTIMER      hTimerStorage = 0;
static WM_HTIMER      hTimerAutoDemo = 0;
static uint8_t        current_module_id = 0;
static uint8_t        auto_demo_increment = 0;
static uint8_t        auto_demo_step = 0;
static __IO uint32_t  BckRegValue = 0;
static int            move_end = 0;
static int            button_clicked = 0;

/* Table of the bitmaps to be used on the Main Menu */
static const BITMAP_ITEM _aBitmapItem[] = {
  {&bmaudio_player_idle,     &bmaudio_player_clicked,    "AUDIO PLAYER"       , 0 },
  {&bmvideo_player_idle,     &bmvideo_player_clicked,    "VIDEO PLAYER"       , 1 },
  {&bmvideo_game_idle,       &bmvideo_game_clicked,      "GAME"               , 2 },
  {&bmclock_date_idle,       &bmclock_date_clicked,      "CLOCK & WEATHER"    , 3 },
  {&bmgraphic_effects_idle,  &bmgraphic_effects_clicked, "GRAPHIC EFFECT"     , 4 },
};

#if defined (DEMO_CHECK_MEMORY_LEAK)
static size_t CurrentFreeHeapSize = 0;
static size_t LastFreeHeapSize = 0;
#endif /* DEMO_CHECK_MEMORY_LEAK */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if 0
/**
  * @brief  Paints back button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_back(BUTTON_Handle hObj) 
{ 
  if(module_prop[id].in_use == 0)
    GUI_DrawBitmap(&bmmenu, 10, 10);
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
#endif

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
  RTC_HandleTypeDef RtcHandle;
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
            case ID_BUTTON_MODULE:
#if defined (DEMO_CHECK_MEMORY_LEAK)
              LastFreeHeapSize = xPortGetFreeHeapSize();
#endif /* DEMO_CHECK_MEMORY_LEAK */
              if( (module_prop[current_module_id].module->id == MODULE_AUDIO_PLAYER) \
               || (module_prop[current_module_id].module->id == MODULE_VIDEO_PLAYER) \
               || (module_prop[current_module_id].module->id == MODULE_GRAPHIC_EFFECTS) )
              {
                if( Storage_GetStatus(USB_DISK_UNIT) || Storage_GetStatus(MSD_DISK_UNIT) )
                {
                  module_prop[current_module_id].in_use = 1;
                  module_prop[current_module_id].module->startup(pMsg->hWin, 0, 0);
                }
                else
                {
                  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_STORAGE);
                  WM_ShowWindow(hItem);
                  TEXT_SetFont(hItem, GUI_FONT_8_1);
                  TEXT_SetTextColor(hItem, GUI_WHITE);
                  hTimerStorage = WM_CreateTimer(pMsg->hWin, ID_TIMER_STORAGE, 2000, 0);
                }
              }
              else
              {
                module_prop[current_module_id].in_use = 1;
                module_prop[current_module_id].module->startup(pMsg->hWin, 0, 0);
              }
              break;
            case ID_BUTTON_MODULE0:
              if(move_end == 0)
              {
                WM_MOTION_SetMovement(hMotion, GUI_COORD_X, 500, 100);
                move_end = 1;
                button_clicked = 1;
              }
              break;
            case ID_BUTTON_MODULE1:
              if(move_end == 0)
              {
                WM_MOTION_SetMovement(hMotion, GUI_COORD_X, -500, 100);
                move_end = 1;
                button_clicked = 1;
              }
              break;
            case ID_BUTTON_MODULE2:
              if(move_end == 0)
              {
                WM_MOTION_SetMovement(hMotion, GUI_COORD_X, 500, 100);
                move_end = 1;
                button_clicked = 1;
              }
              break;
            case ID_BUTTON_MODULE3:
              if(move_end == 0)
              {
                WM_MOTION_SetMovement(hMotion, GUI_COORD_X, -500, 100);
                move_end = 1;
                button_clicked = 1;
              }
              break;
            case ID_BUTTON_MODULE4:
              if(move_end == 0)
              {
                WM_MOTION_SetMovement(hMotion, GUI_COORD_X, -500, 100);
                move_end = 1;
                button_clicked = 1;
              }
              break;              
            case ID_BUTTON_INFO:
              Startup_info(pMsg->hWin, 0, 0);
              break;
            case ID_BUTTON_BACK:
              RtcHandle.Instance = RTC;
              __HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);
              RTC->BKP1R = 0x5AA55BBB;
              /* Enable the write protection for RTC registers */
              __HAL_RTC_WRITEPROTECTION_ENABLE(&RtcHandle);
              NVIC_SystemReset();
              break;
            }
          break;
        case WM_NOTIFICATION_CHILD_DELETED:
          module_prop[current_module_id].in_use = 0;
          if(auto_demo_step > 0)
          {
            auto_demo_increment++;
            auto_demo_step = 0;
          }
#if defined (DEMO_CHECK_MEMORY_LEAK)
          CurrentFreeHeapSize = xPortGetFreeHeapSize();
          if(CurrentFreeHeapSize != LastFreeHeapSize)
          {
            printf("%s() : Memory leak detected after running the module %s : %d/%d\n", __func__, k_ModuleGetName(module_prop[current_module_id].module), LastFreeHeapSize, CurrentFreeHeapSize);
          }
#endif /* DEMO_CHECK_MEMORY_LEAK */
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
  * @brief  Callback routine of the menu
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDraw(WM_MESSAGE * pMsg)
{
  WM_HWIN     hItem;
  PARA        *pPara;
  int         Id, i, a1000, Swap;
  I32         SinHQ;
  I32         CosHQ;
  I32         a;
  ITEM_INFO   ItemInfo;
  uint32_t    auto_demo_timeout = (AutoDemoTimeOutMs * 1000);
  
  switch (pMsg->MsgId)
  {
    case WM_CREATE:
      if(hTimerAutoDemo == 0)
      {
        hTimerAutoDemo = WM_CreateTimer(pMsg->hWin, ID_TIMER_AUTO_DEMO, auto_demo_timeout, 0);
      }
    break;
    
    case WM_TIMER:
      Id = WM_GetTimerId(pMsg->Data.v);

      if (Id == ID_TIMER_STORAGE)
      {
        hItem = WM_GetDialogItem(WM_HBKWIN, ID_TEXT_STORAGE);
        WM_HideWindow(hItem);
        WM_DeleteTimer(hTimerStorage);
        hTimerStorage = 0;
      }
      else if (Id == ID_TIMER_AUTO_DEMO)
      {
        if(hTimerStorage)
        {
          hItem = WM_GetDialogItem(WM_HBKWIN, ID_TEXT_STORAGE);
          WM_HideWindow(hItem);
          WM_DeleteTimer(hTimerStorage);
          hTimerStorage = 0;
        }

        auto_demo_timeout = 1000;

        if( (auto_demo_increment < k_ModuleGetNumber())
         && ((module_prop[current_module_id].in_use == 0) || (auto_demo_step > 0)) )
        {
          if(auto_demo_step == 0)
          {
            auto_demo_step++;
            WM_MOTION_SetMovement(hMotion, GUI_COORD_X, -300, 100);
            WM_InvalidateWindow(hMotion);
          }
          else if (auto_demo_step == 1)
          {
            if( (module_prop[current_module_id].module->id == MODULE_AUDIO_PLAYER) \
             || (module_prop[current_module_id].module->id == MODULE_VIDEO_PLAYER) \
             || (module_prop[current_module_id].module->id == MODULE_GRAPHIC_EFFECTS))
            {
              if( (Storage_GetStatus(USB_DISK_UNIT) == 0) && (Storage_GetStatus(MSD_DISK_UNIT) == 0) )
              {
                hItem = WM_GetDialogItem(WM_HBKWIN, ID_TEXT_STORAGE);
                WM_ShowWindow(hItem);
                hTimerStorage = WM_CreateTimer(pMsg->hWin, ID_TIMER_STORAGE, 2000, 0);
                auto_demo_step = 0;
              }
              else
              {
                auto_demo_step++;
              }
            }
            else
            {
              auto_demo_step++;
            }

            if (auto_demo_step == 2)
            {
#if defined (DEMO_CHECK_MEMORY_LEAK)
              LastFreeHeapSize = xPortGetFreeHeapSize();
#endif /* DEMO_CHECK_MEMORY_LEAK */
              module_prop[current_module_id].in_use = 1;
              module_prop[current_module_id].module->startup(WM_HBKWIN, 0, 0);
              auto_demo_timeout = (AUTO_DEMO_STEP_TIMEOUT * 1000);
            }
          }
          else
          {
            if(module_prop[current_module_id].in_use)
            {
              GUI_EndDialog(*module_prop[current_module_id].module->window , 0);
            }
          }
        }
        else
        {
          if(auto_demo_increment > 0)
          {
            auto_demo_increment = 0;
            WM_MOTION_SetMovement(hMotion, GUI_COORD_X, 300, 100);
            WM_InvalidateWindow(hMotion);
          }
          if(AutoDemoTimeOutMs != AUTO_DEMO_TIMEOUT_0)
          {
            /* Set SW Reset to 1 */
            HAL_PWR_EnableBkUpAccess();
            BckRegValue = READ_REG(BKP_REG_SW_CFG);
            WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x1));
            HAL_PWR_DisableBkUpAccess();
            /* Return back to ML */
            NVIC_SystemReset();
          }
          auto_demo_timeout = (AutoDemoTimeOutMs * 1000);
        }

        if(hTimerAutoDemo != 0)
        {
          WM_RestartTimer(hTimerAutoDemo, auto_demo_timeout);
        }
      }
    break;    
    
    case WM_PAINT:
      if(module_prop[current_module_id].in_use == 0)
      {
        WM_GetUserData(pMsg->hWin, &pPara, sizeof(pPara));
        /* One time initialization of parameter structure */
        if (pPara->xSizeItem == 0) 
        {
          pPara->xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
          pPara->ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
          pPara->xSizeItem   = pPara->pBitmapItem[0].pBitmap->XSize;
          pPara->ySizeItem   = pPara->pBitmapItem[0].pBitmap->YSize;
          pPara->rx          = ( pPara->xSizeWindow      - pPara->xSizeItem) /2;
          pPara->ry          = ((pPara->ySizeWindow) - pPara->ySizeItem - 40)/2;
          pPara->mx          = pPara->xSizeWindow / 2;
          pPara->my          = pPara->ry + 60;
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
          GUI_SetFont(&GUI_FontArialRoundedBold18);
          GUI_SetColor(GUI_WHITE);
          GUI_DispStringHCenterAt((pPara->pBitmapItem + pPara->pItemInfo[pPara->NumItems - 1].Index)->pText, pPara->xSizeWindow / 2, pPara->ySizeWindow - 40);
          GUI_DrawBitmap((pPara->pBitmapItem + pPara->pItemInfo[pPara->NumItems - 1].Index)->pBitmapClicked, pPara->pItemInfo[pPara->NumItems - 1].xPos - pPara->xSizeItem / 2, pPara->pItemInfo[pPara->NumItems - 1].yPos - pPara->ySizeItem / 2);
          current_module_id = (pPara->pBitmapItem + pPara->pItemInfo[pPara->NumItems - 1].Index)->Id;
          WM_SendMessageNoPara(WM_HBKWIN, WM_NOTIFY_PARENT);
          if(move_end == 2)
          {
            move_end = 0;
            button_clicked = 0;
          }
          else if (button_clicked == 0)
          {
            move_end = 0;
            button_clicked = 0;
          }
          else
          {
            move_end++;
          }
        }
      }
    break;
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
      WM_RestartTimer(hTimerAutoDemo, (AutoDemoTimeOutMs * 1000));
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

void k_MenuResetAutoDemo(void)
{
  if(hTimerAutoDemo && hDraw)
  {
    if( (auto_demo_step == 2) && (module_prop[current_module_id].in_use == 1) )
    {
      GUI_EndDialog(*module_prop[current_module_id].module->window, 0);
    }

    auto_demo_step = 0;
    auto_demo_increment = 0;

    if(AutoDemoTimeOutMs != AUTO_DEMO_TIMEOUT_0)
    {
      AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
      HAL_PWR_EnableBkUpAccess();
      BckRegValue = (READ_REG(BKP_REG_SW_CFG) & ~0xFF);
      WRITE_REG(BKP_REG_SW_CFG, (BckRegValue | 0x2));
      HAL_PWR_DisableBkUpAccess();
    }

    WM_RestartTimer(hTimerAutoDemo, (AutoDemoTimeOutMs * 1000));
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
 
  PARA            * pPara;
  int               xSizeWindow;
  int               ySizeWindow;
  int               xSize;
  int               ySize; 
  BUTTON_Handle     hItem;

  current_module_id = 0;
  auto_demo_increment = 0;
  auto_demo_step = 0;
  move_end = 0;
  button_clicked = 0;

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
  hDraw              = WM_CreateWindowAsChild(20, 10, xSize - 40, ySize - 10, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbDraw, sizeof(pPara));
  
  /* Create transparent window which receives the motion messages */
  xSizeWindow        = WM_GetWindowSizeX(hDraw);
  ySizeWindow        = WM_GetWindowSizeY(hDraw);
  hMotion            = WM_CreateWindowAsChild(0, ySizeWindow / 2, xSizeWindow, ySizeWindow / 2, hDraw, WM_CF_SHOW | WM_CF_HASTRANS, _cbMotion, sizeof(pPara));
  
  /* Add pointer to parameter structure to windows */
  WM_SetUserData(hDraw,   &pPara, sizeof(pPara));
  WM_SetUserData(hMotion, &pPara, sizeof(pPara));

  hItem = BUTTON_CreateEx(45, 100, 120, 120, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE0);
  WM_SetCallback(hItem, _cbButton_ok);

  hItem = BUTTON_CreateEx(250, 30, 120, 120, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE1);
  WM_SetCallback(hItem, _cbButton_ok);
  
  hItem = BUTTON_CreateEx(100, 30, 120, 120, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE2);
  WM_SetCallback(hItem, _cbButton_ok);

  hItem = BUTTON_CreateEx(280, 100, 120, 120, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE3);
  WM_SetCallback(hItem, _cbButton_ok);
  
  hItem = BUTTON_CreateEx(440, 232, 30, 30, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_INFO);
  WM_SetCallback(hItem, _cbButton_info);

  /* Create different buttons to be used */
  hItem = BUTTON_CreateEx(150, 120, 210, 160, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_MODULE);
  WM_SetCallback(hItem, _cbButton_ok);
#if 0
  hItem = BUTTON_CreateEx(720, 0, 75, 70, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 0 , ID_BUTTON_BACK);
  WM_SetCallback(hItem, _cbButton_back);
#endif

  hItem = TEXT_CreateEx(307, 243, 225, 10, WM_HBKWIN, WM_CF_HIDE, TEXT_CF_LEFT, ID_TEXT_STORAGE, "Please insert media storage");
  TEXT_SetFont(hItem, GUI_FONT_8_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);
}

