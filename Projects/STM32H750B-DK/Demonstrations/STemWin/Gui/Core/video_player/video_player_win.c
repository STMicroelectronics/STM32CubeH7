/**
  ******************************************************************************
  * @file    videoplayer_win.c
  * @author  MCD Application Team
  * @brief   Video player functions
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
#include "GUI_AVI.h"
#include "video_player_app.h"
#include "audio_player_app.h"

/** @addtogroup VIDEO_PLAYER_MODULE
  * @{
  */

/** @defgroup VIDEO_PLAYER
  * @brief video player routines
  * @{
  */


// #define DISABLE_AUDIO_PLAYBACK

/* External variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmChromART_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmChromART_umpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmJPEG_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmJPEG_umpressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmprevious;
extern GUI_CONST_STORAGE GUI_BITMAP bmnext;
extern GUI_CONST_STORAGE GUI_BITMAP bmplay;
extern GUI_CONST_STORAGE GUI_BITMAP bmpause;

/* Private function prototypes -----------------------------------------------*/
static WM_HWIN hWinVideoPlayer = 0;
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _StopPlay(FIL * file) ;
static uint8_t _StartPlay(char * filename, FIL * file, uint16_t x0, uint16_t y0);
static void _cbMediaConnection(WM_MESSAGE * pMsg);
static void _AddEntireFolder(char *Foldername);
static int _ShowMessageBox(WM_HWIN hWin, const char* pText);
void _cbNotify(GUI_HMEM hMem, int Notification, U32 SkipNFrames);
/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  video_player_board =
{
  MODULE_VIDEO_PLAYER,
  "Video Player",
  &hWinVideoPlayer,
  0,
  0,
  Startup,
  NULL,
};

/* Private defines -----------------------------------------------------------*/
#define VIDEO_FRAME_ID                  (GUI_ID_USER + 0x00)
#define ID_BUTTON_PLAY                  (GUI_ID_USER + 0x01)
#define ID_BUTTON_STOP                  (GUI_ID_USER + 0x02)
#define ID_BUTTON_MENU                  (GUI_ID_USER + 0x03)
#define ID_BUTTON_PREV                  (GUI_ID_USER + 0x04)
#define ID_BUTTON_NEXT                  (GUI_ID_USER + 0x05)
#define ID_BUTTON_HW_JPEG               (GUI_ID_USER + 0x06)
#define ID_BUTTON_CHROMART              (GUI_ID_USER + 0x07)
#define ID_SLIDER_VOLUME                (GUI_ID_USER + 0x08)
#define ID_TEXT_FILENAME                (GUI_ID_USER + 0x09)
#define ID_TOTAL_TIME                   (GUI_ID_USER + 0x0A)
#define ID_ELAPSED_TIME                 (GUI_ID_USER + 0x0B)
#define ID_VIDEO_NAME                   (GUI_ID_USER + 0x0C)
#define ID_FPS                          (GUI_ID_USER + 0x0D)
#define ID_FEATURES_CPU                 (GUI_ID_USER + 0x0E)
#define ID_SLIDER_DURATION              (GUI_ID_USER + 0x0F)
#define ID_VIDEO_FORMAT                 (GUI_ID_USER + 0x10)
#define ID_TIMER_CONNECTION             (GUI_ID_USER + 0x11)
#define ID_TIMER_TOUCH                  (GUI_ID_USER + 0x12)
#define ID_TIMER_CPU                    (GUI_ID_USER + 0x13)

#define ID_WINDOW_0                     (GUI_ID_USER + 0x20)
#define ID_TEXT_0                       (GUI_ID_USER + 0x21)

#define ID_SLIDER_VIDEO_VOLUME          (GUI_ID_USER + 0x30)

typedef enum video_player_stat_e
{
  VIDEO_FAILED = 0,
  VIDEO_IDLE,
  VIDEO_STOP,
  VIDEO_PAUSE,
  VIDEO_PLAY,
} video_player_stat_t;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "Window", VIDEO_FRAME_ID, 0, 0, 480, 272, (WM_CF_SHOW | WM_CF_HASTRANS), 0x0, 0 },
};

/*********************************************************************
*
*       _aMessageCreate
*/
static const GUI_WIDGET_CREATE_INFO _aMessageCreate[] = {
  { WINDOW_CreateIndirect   , "MessageBox"    , ID_WINDOW_0 ,   0 ,   0 , 200 ,  30 , (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_FGND), 0x0 , 0 },
  { TEXT_CreateIndirect     , "Message"       , ID_TEXT_0   ,   5 ,   5 , 190 ,  20 , (GUI_TA_HCENTER | GUI_TA_VCENTER)  , 0x0 , 0 },
};

static GUI_AVI_HANDLE havi;
uint32_t cpu = 0;
uint8_t toggle_hw = 1;
uint8_t toggle_chromART = 1;
static video_player_stat_t VideoPlayer_State = VIDEO_IDLE;
uint8_t SelLayer = 0;
uint8_t VisLayer = 1;

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="sdio_heap"
#else
__attribute__((section(".sdio_heap"))) __attribute__ ((aligned (32)))
#endif
FIL     Video_File;
GUI_AVI_INFO Video_Info;

static WM_HWIN  MediaConnectionWin = 0;
static WM_HWIN  playbackwin = 0;
static WM_HWIN  TouchWindow = 0;
static WM_HWIN  MsgWin = 0;
static uint32_t  Video_FilePos = 0;
static WM_HTIMER TouchTimer;
static uint32_t  elapsed_time = 0;

static FILELIST_FileTypeDef VideoList;

/* Buffers for right and left samples */
#if defined ( __GNUC__ )
__attribute__((section(".avi_video_buffers"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="avi_video_buffers"
#elif defined( __CC_ARM )
__attribute__((section(".avi_video_buffers"), zero_init)) __attribute__ ((aligned (32)))
#endif
uint8_t AVI_VideoBuffer[AVI_VIDEO_BUF_SIZE];

#if defined ( __GNUC__ )
__attribute__((section(".audio_buffers"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="audio_buffers"
#elif defined( __CC_ARM )
__attribute__((section(".audio_buffers"), zero_init)) __attribute__ ((aligned (32)))
#endif
uint8_t AVI_AudioBuffer[AUDIO_BUFFER_SIZE + 8];

static uint8_t PlayerSeekInProgress  = 0;

/* Default storage ID is USB */
static STORAGE_IdTypeDef storage_id = USB_DISK_UNIT;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Paints menu button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_menu(BUTTON_Handle hObj) 
{
  GUI_DrawBitmap(&bmmenu, 10, 10);
}

/**
  * @brief  Paints play button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_play(BUTTON_Handle hObj) 
{  
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  if(VideoPlayer_State != VIDEO_PLAY)
  {
    GUI_DrawBitmap(&bmplay, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmpause, 0, 0);
  }
}

/**
  * @brief  Paints HW jpeg button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_hw_jpeg(BUTTON_Handle hObj) 
{  
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  if(toggle_hw == 0)
  {
    GUI_DrawBitmap(&bmJPEG_umpressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmJPEG_pressed, 0, 0);
  }  
}

/**
  * @brief  Paints chromart button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_chromart(BUTTON_Handle hObj) 
{  
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  if(toggle_chromART == 0)
  {
    GUI_DrawBitmap(&bmChromART_umpressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmChromART_pressed, 0, 0);
  }  
}

/**
  * @brief  Paints back button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_back(BUTTON_Handle hObj) 
{  
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  GUI_DrawBitmap(&bmprevious, 0, 0);
}

/**
  * @brief  Paints forward button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_fore(BUTTON_Handle hObj) 
{  
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  GUI_DrawBitmap(&bmnext, 0, 0);
}

/**
  * @brief  callback for play button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_play(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_play(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
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
  * @brief  callback for hw_jpeg button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_hw_jpeg(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_hw_jpeg(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for chromart button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_chromart(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_chromart(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for forward button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_fore(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_fore(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for menu button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_menu(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_menu(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for touch
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbTouch(WM_MESSAGE * pMsg) 
{
  const GUI_PID_STATE * pState;
  WM_HWIN hItem = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
    {
      hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_PREV);
      WM_HideWindow(hItem);

      hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_PLAY);
      WM_HideWindow(hItem);

      hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_NEXT);
      WM_HideWindow(hItem);

      hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_MENU);
      WM_HideWindow(hItem);

      hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
      WM_HideWindow(hItem);

      hItem = WM_GetDialogItem(playbackwin, ID_TOTAL_TIME);
      WM_HideWindow(hItem);

      hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
      WM_HideWindow(hItem);

      hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_VIDEO_VOLUME);
      WM_HideWindow(hItem);

      if( (WM_IsWindow(MsgWin)) && (VideoPlayer_State > VIDEO_IDLE) )
      {
        WM_HideWindow(MsgWin);
      }
      VisLayer = 0;
    }
    break;
    case  WM_TOUCH:
    {
      pState = (const GUI_PID_STATE *)pMsg->Data.p;
      if (pState) 
      {
        if (pState->Pressed == 1)
        {
          hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_MENU);
          if(!WM_IsVisible(hItem))
          {
            hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_MENU);
            WM_ShowWindow(hItem);
            
            hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_PREV);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_NEXT);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_PLAY);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_TOTAL_TIME);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
            WM_ShowWindow(hItem);

            if(Video_Info.AudioType)
            {
              hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_VIDEO_VOLUME);
              WM_ShowWindow(hItem);
            }

            if( (WM_IsWindow(MsgWin)) && (VideoPlayer_State > VIDEO_IDLE) )
            {
              WM_ShowWindow(MsgWin);
            }
          }
          WM_DeleteWindow(pMsg->hWin);
        }
        }
      }
      break;     
    case WM_DELETE:
    {
      TouchWindow = 0;
      VisLayer = 1;
      if (TouchTimer)
        WM_RestartTimer(TouchTimer, 5000);
    }
    break;
    default:
      WM_DefaultProc(pMsg);
      break;    
  }
}

/**
  * @brief  Callback function of the media connection status
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMediaConnection(WM_MESSAGE * pMsg) 
{
  static WM_HTIMER hStatusTimer;  
  static uint8_t   prev_msd_status = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      prev_msd_status = k_StorageGetStatus(storage_id);
      hStatusTimer = WM_CreateTimer(pMsg->hWin, ID_TIMER_CONNECTION, 100, 0);
      break;
    case WM_TIMER:
      if(prev_msd_status != k_StorageGetStatus(storage_id))
      {
        prev_msd_status = k_StorageGetStatus(storage_id);
        if(prev_msd_status == 0)
        {
          GUI_EndDialog(hWinVideoPlayer, 0);
        }
      }
      if(hStatusTimer != 0)
      {
        WM_RestartTimer(pMsg->Data.v, 100);
      }
      else
      {
        WM_DeleteTimer(hStatusTimer);
        hStatusTimer = 0;
      }
      break;
    case WM_DELETE:
      if(hStatusTimer != 0)
      {
        WM_DeleteTimer(hStatusTimer);
        hStatusTimer = 0;
      }
      MediaConnectionWin = 0;
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  callback for video window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbplaybackwin(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int Id, NCode;
  const GUI_PID_STATE * pState;
  char tmp[64];
  int r = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:   

      hItem = BUTTON_CreateEx(138, 125, 21, 23, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PREV);
      WM_SetCallback(hItem, _cbButton_back);
      
      hItem = BUTTON_CreateEx(199, 95, 82, 82, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PLAY);
      WM_SetCallback(hItem, _cbButton_play);
          
      hItem = BUTTON_CreateEx(321, 125, 21, 23, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_NEXT);
      WM_SetCallback(hItem, _cbButton_fore);
      
      hItem = BUTTON_CreateEx(5, 150, 45, 45, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HW_JPEG);
      WM_SetCallback(hItem, _cbButton_hw_jpeg); 
     
      hItem = BUTTON_CreateEx(5, 200, 45, 45, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CHROMART);
      WM_SetCallback(hItem, _cbButton_chromart);    
      
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MENU);
      WM_SetCallback(hItem, _cbButton_menu);      
      
      /* Elapsed Time Initialization */
      hItem = TEXT_CreateEx(38, 248, 30, 12, pMsg->hWin, WM_CF_SHOW, (TEXT_CF_LEFT | TEXT_CF_VCENTER), ID_ELAPSED_TIME, "00:00");
      TEXT_SetFont(hItem, GUI_FONT_10_1);
      TEXT_SetTextColor(hItem, 0xFF3C3D48);
      
      /* Total Time Initialization */
      hItem = TEXT_CreateEx(412, 248, 30, 12, pMsg->hWin, WM_CF_SHOW, (TEXT_CF_RIGHT | TEXT_CF_VCENTER), ID_TOTAL_TIME, "00:00");
      TEXT_SetFont(hItem, GUI_FONT_10_1);
      TEXT_SetTextColor(hItem, 0xFF3C3D48);
      
      hItem = TEXT_CreateEx(10, 5, 65, 10, pMsg->hWin, WM_CF_SHOW, (TEXT_CF_LEFT | TEXT_CF_VCENTER), ID_FPS, "Rate: 25 fps");
      TEXT_SetFont(hItem, GUI_FONT_10_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);

      hItem = TEXT_CreateEx(82, 5, 75, 10, pMsg->hWin, WM_CF_SHOW, (TEXT_CF_LEFT | TEXT_CF_VCENTER), ID_FEATURES_CPU, "CPU load: 0%");
      TEXT_SetFont(hItem, GUI_FONT_10_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      hItem = SLIDER_CreateEx(70, 250, 340, 8, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_DURATION);
      SLIDER_SetBkColor(hItem, GUI_TRANSPARENT);
      SLIDER_SetFocusColor (hItem, 0xFF8080FF);
      SLIDER_SetValue(hItem, 0);
      SLIDER_SetWidth(hItem, 0);
      SLIDER_SetSTSkin(hItem);    

      /* Volume slider */
      hItem = SLIDER_CreateEx(455, 60, 8, 160, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_SLIDER_VIDEO_VOLUME);
      SLIDER_SetBkColor(hItem, GUI_TRANSPARENT);
      SLIDER_SetFocusColor(hItem, 0xFF8080FF);
      SLIDER_SetValue(hItem, (100 - AUDIOPLAYER_GetVolume()));
      SLIDER_SetWidth(hItem, 0);
      SLIDER_SetSTSkin(hItem);
      SLIDER_SetRange(hItem, 0, 100);
      WM_HideWindow(hItem);

      TouchTimer = WM_CreateTimer(pMsg->hWin, ID_TIMER_TOUCH, 2000, 0);   
      WM_CreateTimer(pMsg->hWin, ID_TIMER_CPU, 5000, 0);   

      break;
      
    case WM_TIMER:
      {
        Id = WM_GetTimerId(pMsg->Data.v);
        if (Id == ID_TIMER_CPU)
        {
          WM_RestartTimer(pMsg->Data.v, 1000);
          /* Show CPU usage */
          cpu = osGetCPUUsage();
          sprintf((char *)tmp , "CPU load : %d%%", osGetCPUUsage());
          hItem = WM_GetDialogItem(pMsg->hWin, ID_FEATURES_CPU);
          TEXT_SetText(hItem, tmp);
          WM_InvalidateWindow(hItem);
          WM_Paint(hItem);
        }
        if (Id == ID_TIMER_TOUCH)
        {
          if(TouchWindow == 0)
          {
            TouchWindow = WM_CreateWindowAsChild(60, 0, 360, 232, WM_GetDesktopWindowEx(1), WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_STAYONTOP, _cbTouch , 0);
          }
          WM_RestartTimer(pMsg->Data.v, 5000);
        }
      }
      break;    
    case WM_DELETE:
      if(TouchTimer != 0)
      {
        WM_DeleteTimer(TouchTimer);
        TouchTimer = 0;
      }
      if(TouchWindow)
      {
        WM_DeleteWindow(TouchWindow);
        TouchWindow = 0;
      }
      if(VideoPlayer_State > VIDEO_IDLE)
      {
        _StopPlay(&Video_File);
      }
      if(havi != 0)
      {
        GUI_AVI_Delete (havi);
      }
      SelLayer = 0;
      GUI_SetLayerVisEx (1, 0);
      VisLayer = 2;
      GUI_SelectLayer(0);
      playbackwin = 0;
      AUDIOPLAYER_DeInit();
      break;    
    
    case WM_PAINT:
      GUI_SetBkColor(GUI_TRANSPARENT);
      GUI_Clear();
      GUI_SetColor(0x7F2D272C);
      GUI_FillRect( 5, 2,  78, 18);
      GUI_FillRect(80, 2, 165, 18);
      break;    
    
    case WM_TOUCH:
      {
        pState = (const GUI_PID_STATE *)pMsg->Data.p;
        if (pState)
        {
          if (pState->Pressed == 1)
          {
            if(TouchTimer != 0)
              WM_RestartTimer(TouchTimer, 5000);
          }
        }
      }
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch(Id)
      {
        /*if(VisLayer == 1)
        {*/   
          case ID_BUTTON_MENU:
            switch(NCode)
            {
              case WM_NOTIFICATION_RELEASED:
                WM_DeleteWindow(hWinVideoPlayer);
                break;
            }
            break;
#if 0
          case ID_BUTTON_STOP:
            switch(NCode) 
            {
              case WM_NOTIFICATION_RELEASED:
                if(VideoPlayer_State > VIDEO_STOP)
                {
                  VideoPlayer_State = VIDEO_STOP;
                  _StopPlay(&Video_File);
                  _ShowMessageBox(WM_GetParent(pMsg->hWin), "Press Play to Start");
                }
                WM_InvalidateWindow(pMsg->hWin);
                break;
            }
            break;
#endif
            /* Notifications sent by 'progress' Slider */
        case ID_SLIDER_DURATION:
          {
            if(VideoPlayer_State > VIDEO_STOP)
            {
              static uint32_t NewTime = 0;

              if(NCode == WM_NOTIFICATION_CLICKED)
              {
                PlayerSeekInProgress = 1;
              }
              else if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
              {
                /* Get Slider Position - gives position in ms */
                hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
                NewTime = SLIDER_GetValue(hItem);

                sprintf((char *)tmp , "%02lu:%02lu", NewTime/60, NewTime%60 );
                hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
                TEXT_SetText(hItem, tmp);
              }
              else if(NCode == WM_NOTIFICATION_RELEASED)
              {
                uint8_t Playing = (VideoPlayer_State == VIDEO_PLAY) ? 1 : 0;
                if(Playing)
                {
                  if(Video_Info.AudioType)
                  {
                    AUDIOPLAYER_Pause();
                  }
                  GUI_AVI_Pause(havi);
                }

                GUI_AVI_GotoFrame(havi, ((NewTime * 1000)/Video_Info.msPerFrame));
                if(Video_Info.AudioType)
                {
                  AUDIOPLAYER_SeekToTime(NewTime);
                }

                if(Playing)
                {
                  GUI_AVI_Play(havi);
                  if(Video_Info.AudioType)
                  {
                    AUDIOPLAYER_Resume();
                  }
                }
                PlayerSeekInProgress = 0;
              }
            }
          }
          break;
        case ID_BUTTON_PREV:
          /* No break */
        case ID_BUTTON_NEXT:
          switch(NCode)
          {
            case WM_NOTIFICATION_RELEASED:
              if(Id == ID_BUTTON_PREV)
              {
                if(Video_FilePos > 0)
                Video_FilePos--;
                else
                  Video_FilePos = VideoList.ptr - 1;
              }
              else 
              {        
                if(Video_FilePos < (VideoList.ptr - 1))
                  Video_FilePos++;
                else
                  Video_FilePos = 0;
              }  
              if(VideoPlayer_State > VIDEO_STOP)
              {
                _StopPlay(&Video_File);
                if (VideoPlayer_State == VIDEO_PAUSE)
                {
                  VideoPlayer_State = VIDEO_STOP;
                  _ShowMessageBox(WM_GetParent(pMsg->hWin), "Press Play to Start");
                }
                else
                {
                  VideoPlayer_State = VIDEO_PLAY;
                  r = _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0);
                }
              }
              else
              {
                r = _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0); 
                if(r == 0)
                {
                  if(Video_Info.AudioType)
                  {
                    AUDIOPLAYER_Pause();
                  }
                  GUI_AVI_Pause(havi);
                  _ShowMessageBox(WM_GetParent(pMsg->hWin), "Press Play to Start");
                  VideoPlayer_State = VIDEO_PAUSE;
                }
              }
              WM_InvalidateWindow(pMsg->hWin);
              break;
          }
          break;      
        case ID_BUTTON_PLAY:
          switch(NCode)
          {
            case WM_NOTIFICATION_RELEASED:
              if((VideoPlayer_State == VIDEO_IDLE) || (VideoPlayer_State == VIDEO_PAUSE))
              {
                if(WM_IsWindow(MsgWin))
                {
                  GUI_EndDialog(MsgWin, 0);
                }
                GUI_AVI_Play(havi);
                if(Video_Info.AudioType)
                {
                  AUDIOPLAYER_Resume();
                }
                VideoPlayer_State = VIDEO_PLAY;
              }
              else if(VideoPlayer_State == VIDEO_STOP)
              {
                VideoPlayer_State = VIDEO_PLAY;          
                _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0);
              }
              else if(VideoPlayer_State == VIDEO_PLAY)
              {
                if(Video_Info.AudioType)
                {
                  AUDIOPLAYER_Pause();
                }
                GUI_AVI_Pause(havi);
                VideoPlayer_State = VIDEO_PAUSE; 
              }
              WM_InvalidateWindow(pMsg->hWin);
              break;
          }
          break;

      /* Notifications sent by 'Volume' Slider */
      case ID_SLIDER_VIDEO_VOLUME:
        if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_VIDEO_VOLUME);
          AUDIOPLAYER_SetVolume(100 - SLIDER_GetValue(hItem));
        }
        break;
        /*}*/
        /*if(VisLayer != 2)
        {*/
          case ID_BUTTON_HW_JPEG:
            switch(NCode) 
            {
              case WM_NOTIFICATION_RELEASED:
                toggle_hw = 1- toggle_hw;
                GUI_AVI_SelectMode (havi, toggle_hw);
                if(toggle_hw == 0)
                {
                  toggle_chromART = 0;
                }
                hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CHROMART);
                WM_InvalidateWindow(hItem);
                WM_Paint(hItem);
                WM_InvalidateWindow(pMsg->hWin);
                break;
            }
            break;
          case ID_BUTTON_CHROMART:
            switch(NCode) 
            {
              case WM_NOTIFICATION_RELEASED:
                /* Code to be added */
                toggle_chromART = 1- toggle_chromART;
                if(toggle_chromART == 1)
                {
                  toggle_hw = 1;
                  GUI_AVI_SelectMode (havi, toggle_hw);
                }
                hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HW_JPEG);
                WM_InvalidateWindow(hItem);
                WM_Paint(hItem);
                WM_InvalidateWindow(pMsg->hWin);
                break;
            }
            break;
        /*}*/
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Notify the end of audio file.
  * @param  None.
  * @retval Audio state.
  */
static void _NotifyEndOfFile(void)
{
  _StopPlay(&Video_File);
  if(Video_FilePos < (VideoList.ptr - 1))
  {
    Video_FilePos++;
  }
  else
  {
    Video_FilePos = 0;
  }
  _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0);
}

/**
  * @brief  Callback function for movie player. Uses multiple buffering if
  *         available to avoid tearing effects.
  * @param  hMem:         pointer to parent handle
  * @param  Notification: notification ID
  * @param  SkipNFrames: number of last skipped frames
  * @retval None
  */
void _cbNotify(GUI_HMEM hMem, int Notification, U32 SkipNFrames)
{ 
  char tmp[20]; 
  WM_HWIN  hItem;
  static uint32_t FrameInitTick = 0;
  static uint32_t FrameDuration = 0;
  static uint32_t InitTick = 0;  
 
  switch (Notification) 
  {
    case GUI_AVI_NOTIFICATION_START:
      FrameInitTick = 0;
      FrameDuration = 0;
      break;
      
    case GUI_AVI_NOTIFICATION_SKIP:
      GUI_MULTIBUF_End();
      if(VisLayer == 1)
      {
        GUI_SelectLayer(0);
      }
      break;

    case GUI_AVI_NOTIFICATION_PREDRAW:
      if(GUI_AVI_GetFrameIndex(havi) == 1)
      {
        if(Video_Info.AudioType)
        {
          /* Stop already playing audio */
          if(AUDIOPLAYER_GetState() > AUDIO_STATE_STOP)
          {
            AUDIOPLAYER_Stop();
          }

          /* Start audio playback */
          if(AUDIOPLAYER_Play(Video_Info.SampleRate) != AUDIO_ERROR_NONE)
          {
            /* Should never happen, but disable audio in such case */
            Video_Info.AudioType = 0;
          }
          else
          {
            hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
            if(WM_IsVisible(hItem))
            {
              hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_VIDEO_VOLUME);
              WM_ShowWindow(hItem);
            }
          }
        }
      }
      GUI_SelectLayer(0);
      GUI_MULTIBUF_Begin();
      FrameInitTick = osKernelSysTick();
      break;

    case GUI_AVI_NOTIFICATION_POSTDRAW:
      FrameDuration = osKernelSysTick() - FrameInitTick;

      /* We need to limit the display FPS to the Video FPS */
      if(FrameDuration < Video_Info.msPerFrame)
      {
        osDelay((Video_Info.msPerFrame - FrameDuration));
        FrameDuration = Video_Info.msPerFrame;
      }

      /* Progress Bar update each one second */
      if(((osKernelSysTick() - InitTick ) > 1000) || (InitTick == 0))
      {
        hItem = WM_GetDialogItem(playbackwin, ID_FPS);
        sprintf(tmp, "Rate : %lu fps", 1000/FrameDuration);
        TEXT_SetText(hItem, tmp);
        WM_InvalidateWindow(hItem);
        WM_Paint(hItem);
        
        InitTick = osKernelSysTick();

        if(!PlayerSeekInProgress)
        {
          elapsed_time = (Video_Info.msPerFrame * GUI_AVI_GetFrameIndex(havi))/ 1000;

          hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
          if(WM_IsVisible(hItem))
          {
            SLIDER_SetValue(hItem, elapsed_time);
          }

          hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
          if(WM_IsVisible(hItem))
          {
            sprintf((char *)tmp , "%02lu:%02lu", elapsed_time/60, elapsed_time%60 );
            TEXT_SetText(hItem, tmp);
            WM_InvalidateWindow(hItem);
            WM_Paint(hItem);
          }
        }
      }
      GUI_MULTIBUF_End();
      if(VisLayer == 1)
      {
        GUI_SelectLayer(0); 
      }
      break;

    case GUI_AVI_NOTIFICATION_STOP:
      {
        /* Perform a clean stop of both video and audio players */
      }
      break;

    case GUI_AVI_NOTIFICATION_EOF:
      {
        _NotifyEndOfFile();
      } 
      break;
   }
}

/**
  * @brief  Start play
  * @param  filename: pointer to the video file name
  * @retval None
  */
static uint8_t _StartPlay(char * filename, FIL * file, uint16_t x0, uint16_t y0) 
{
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 
  uint32_t ms, frames, duration;  
  WM_HWIN  hItem;
  
  if(WM_IsWindow(MsgWin))
  {
    GUI_EndDialog(MsgWin, 0);
    GUI_Delay(1);
  }

  if(f_open(file, filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    elapsed_time = 0;
    GUI_AVI_Start   (havi, file, f_size(file));
    GUI_AVI_GetInfo (havi, &Video_Info);

#if defined(DISABLE_AUDIO_PLAYBACK)
    /* Disable Audio Playback */
    Video_Info.AudioType = 0;
#endif /* DISABLE_AUDIO_PLAYBACK */

    if((Video_Info.xSize != LCD_GetXSize()) || (Video_Info.ySize != LCD_GetYSize()))
    {
      /* Should never happen */
      Error_Handler();
    }

    GUI_AVI_Show(havi, ((LCD_GetXSize() - Video_Info.xSize)/2), ((LCD_GetYSize() - Video_Info.ySize)/2), 0); 

    if(Video_Info.AudioType)
    {
      if(AUDIOPLAYER_SelectFile(filename) != AUDIO_ERROR_NONE)
      {
        /* Should never happen, but disable audio in such case */
        Video_Info.AudioType = 0;
      }
    }

    /* Update video total time */
    hItem = WM_GetDialogItem(playbackwin, ID_TOTAL_TIME);
    ms = Video_Info.msPerFrame;
    frames = Video_Info.NumFrames;
    duration = (frames * ms)/1000;
    sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
    TEXT_SetText(hItem, tmp);          
    
    hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
    TEXT_SetText(hItem, "00:00");
    
    hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
    SLIDER_SetRange(hItem, 0, duration);
    SLIDER_SetValue(hItem, 0);

    WM_Update(playbackwin);
    if(TouchTimer != 0)
      WM_RestartTimer(TouchTimer, 5000);
  }
  
  return 0;
}

/**
  * @brief  Stop play 
  * @param  None
  * @retval None
  */
static void _StopPlay(FIL * file) 
{
  AVI_CONTEXT * pavi;

  if(Video_Info.AudioType)
  {
    if(AUDIOPLAYER_GetState() > AUDIO_STATE_STOP)
    {
      AUDIOPLAYER_Stop();
    }
  }

  GUI_AVI_Stop (havi);

  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
  {
    pavi->CurrentImage  = 0;
    pavi->TimeNextFrame = 0;
    f_close(file);
  }
  GUI_UNLOCK_H(pavi);
}

/**
  * @brief  Callback routine of the video main dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog_info(WM_MESSAGE * pMsg) {
  WM_HWIN  hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) 
  {
    case WM_INIT_DIALOG:
      MediaConnectionWin = WM_CreateWindowAsChild(479, 271, 1, 1, pMsg->hWin, WM_CF_SHOW, _cbMediaConnection , 0);
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MENU);
      WM_SetCallback(hItem, _cbButton_menu);
      
      hItem = TEXT_CreateEx(70, 120, 520, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_FILENAME, "Please add video files in the selected media storage");
      TEXT_SetTextAlign(hItem, TEXT_CF_LEFT);
      TEXT_SetFont(hItem, GUI_FONT_13_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      break;
    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_16bit, 0, 0);
      GUI_DrawBitmap(&bminfo, 15, 110);
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch(Id) 
      {
        case ID_BUTTON_MENU:
          if(NCode == WM_NOTIFICATION_RELEASED)
          {
            GUI_EndDialog(pMsg->hWin, 0);
          }
        break;
      }
      break;

    case WM_DELETE:
      hWinVideoPlayer = 0;
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }    
}

/**
  * @brief  Callback routine of the video main dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
 
  switch (pMsg->MsgId) 
  {
    case WM_INIT_DIALOG:
      MediaConnectionWin = WM_CreateWindowAsChild(479, 271, 1, 1, pMsg->hWin, WM_CF_SHOW, _cbMediaConnection , 0);
      
      /* Playlist not empty, so start play first item */
      if(VideoList.ptr > 0)
      {  
        GUI_SelectLayer(1); 
        VisLayer = 1;
        playbackwin = WM_CreateWindowAsChild(0, 0, 480, 272, WM_GetDesktopWindowEx(1), (WM_CF_SHOW | WM_CF_HASTRANS), _cbplaybackwin , 0);
        GUI_SelectLayer(0);
        VideoPlayer_State = VIDEO_PLAY;  

        _StartPlay((char *)VideoList.file[0].name, &Video_File, 0, 0);     
        GUI_SelectLayer(1);
      }
      break;

    case WM_PAINT: 
      if(VideoList.ptr == 0)
      {
        GUI_SetBkColor(0xFF884130);
      }
      else
      {
        GUI_SetBkColor(GUI_BLACK);
      }
      GUI_Clear();
      break;

    case WM_DELETE :
      if(playbackwin)
        WM_DeleteWindow(playbackwin);

      if(MsgWin)
        WM_DeleteWindow(MsgWin);

      if(MediaConnectionWin)
        WM_DeleteWindow(MediaConnectionWin);

      hWinVideoPlayer = 0;
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
* @brief callback Message Box
* @param  pMsg : pointer to data structure
* @retval None
*/
static void _cbMessageBox(WM_MESSAGE* pMsg)
{
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
      WM_SetHasTrans(WM_GetClientWindow(pMsg->hWin));
      WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, GUI_FONT_13B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);
      break;

    case WM_DELETE :
      MsgWin = 0;
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }    
}

/**
* @brief  Show Message Box
* @param  pTitle: pointer to the title
* @param  pText: pointer to the text
* @retval int: YesNo player response
*/
static int _ShowMessageBox(WM_HWIN hWin, const char* pText)
{
  WM_HWIN hItem;

  if(!WM_IsWindow(MsgWin))
  {
    /* Create and display the Dialog Box */
    MsgWin = GUI_CreateDialogBox(_aMessageCreate,
                                GUI_COUNTOF(_aMessageCreate),
                                _cbMessageBox,
                                hWin,
                                140, 121);
  }

  hItem = WM_GetDialogItem(MsgWin, ID_TEXT_0);
  TEXT_SetText(hItem, pText);
  WM_InvalidateWindow(hItem);
  WM_Paint(hItem);
  
  return 1;
}

/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  FIL  file;
  char *fn;
  char tmp[FILEMGR_FILE_NAME_SIZE];
  GUI_AVI_INFO info;
  char folder[256] = { 0 };

  if(Storage_GetStatus(storage_id) == 0)
  {
    return;
  }
  snprintf(folder, sizeof(folder), "%s%s", Storage_GetDrive(storage_id), Foldername);

  res = f_opendir(&dir, folder);
  
  if (res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }
      fn = fno.fname;
     
      if (VideoList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if((strstr(fn, ".avi")) || (strstr(fn, ".AVI")))
          {
            strcpy(tmp, folder);
            strcat(tmp, "/");
            strcat(tmp, fn);
            
            /* Open file, get its AVI info and check for resolution */
            if(f_open(&file, tmp, FA_OPEN_EXISTING | FA_READ) == FR_OK)
            {
              GUI_AVI_Start   (havi, &file, f_size((FIL *)&file));
              GUI_AVI_GetInfo (havi, &info);
              if((info.xSize == LCD_GetXSize()) && (info.ySize == LCD_GetYSize()))
              {
                strncpy((char *)VideoList.file[VideoList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
                VideoList.ptr++;
              }
              GUI_AVI_Stop (havi);
              f_close(&file);
            }
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}

/**
  * @brief  Video window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  toggle_hw = 1;
  toggle_chromART = 1;
  VideoPlayer_State = VIDEO_IDLE;
  Video_FilePos = 0;
  VisLayer = 0;
  SelLayer = 0;
  TouchWindow = 0;
  hWinVideoPlayer = 0;

  memset(&VideoList, 0, sizeof(FILELIST_FileTypeDef));

  havi = GUI_AVI_Create(_cbNotify);

  GUI_AVI_SetBuffers (havi, AVI_VideoBuffer, AVI_VIDEO_BUF_SIZE, AVI_AudioBuffer, AUDIO_BUFFER_SIZE);
  /* Mode: 1 -> HW, 0 -> SW */
  GUI_AVI_SelectMode(havi, 1); 

  _AddEntireFolder("Video");

  if(VideoList.ptr == 0)
  {
    SelLayer = 0;
    hWinVideoPlayer = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog_info, hWin, xpos, ypos);
  }
  else
  {
    GUI_AVI_SetDevFunc (havi, GUI_AVI_JPEG_INIT, (void(*)(void))HW_JPEG_Init);
    GUI_AVI_SetDevFunc (havi, GUI_AVI_JPEG_DEINIT, (void(*)(void))HW_JPEG_DeInit);
    GUI_AVI_SetDevFunc (havi, GUI_AVI_JPEG_DRAW, (void(*)(void))HW_JPEG_Draw);

    /* Initialize Audio Playback */
    if(AUDIOPLAYER_Init(70) == AUDIO_ERROR_NONE)
    {
      AUDIOPLAYER_RegisterCallbacks(NULL, NULL , _NotifyEndOfFile);
    }
    GUI_SetLayerVisEx(1, 1);
    SelLayer = 1;
    hWinVideoPlayer = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
    WM_EnableMemdev(hWinVideoPlayer);
  }
}

/**
  * @}
  */

/**
  * @}
  */
  
