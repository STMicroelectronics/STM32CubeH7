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
#include "GUI_AVI.h"
#include "video_player_app.h"
/* Generated ressourses */
#include "video_player_res.c"

/** @addtogroup VIDEO_PLAYER_MODULE
  * @{
  */

/** @defgroup VIDEO_PLAYER
  * @brief video player routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _StopPlay(FIL * file) ;
static uint8_t _StartPlay(char * filename, FIL * file, uint16_t x0, uint16_t y0);
static void _cbMediaConnection(WM_MESSAGE * pMsg);
static void _AddEntireFolder(char *Foldername);
void _cbNotify(GUI_HMEM hMem, int Notification, U32 CurrentFrame);
/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  video_player_board =
{
  2,
  "Video Player",
  0,
  0,
  Startup,
  NULL,
};

/* Private defines -----------------------------------------------------------*/
#define VIDEO_FRAME_ID                  (GUI_ID_USER + 0x00)
#define PLAY_VIDEO_ID                   (GUI_ID_USER + 0x01)
#define ID_BUTTON_STOP                  (GUI_ID_USER + 0x02)
#define ID_BUTTON_MENU                  (GUI_ID_USER + 0x03)
#define ID_BUTTON_BACK                  (GUI_ID_USER + 0x04)
#define ID_BUTTON_FORE                  (GUI_ID_USER + 0x05)
#define ID_BUTTON_HW_JPEG               (GUI_ID_USER + 0x06)
#define ID_BUTTON_CHROMART              (GUI_ID_USER + 0x07)
#define ID_SLIDER_VOLUME                (GUI_ID_USER + 0x08)
#define ID_TEXT_FILENAME                (GUI_ID_USER + 0x09)
#define ID_TIME                         (GUI_ID_USER + 0x0A)
#define ID_ELAPSED_TIME                 (GUI_ID_USER + 0x0B)
#define ID_VIDEO_NAME                   (GUI_ID_USER + 0x0C)
#define ID_FPS                          (GUI_ID_USER + 0x0D)
#define ID_FEATURES_CPU                 (GUI_ID_USER + 0x0E)
#define ID_SLIDER_DURATION              (GUI_ID_USER + 0x0F)
#define ID_VIDEO_FORMAT                 (GUI_ID_USER + 0x10)
#define ID_TIMER_CONNECTION             (GUI_ID_USER + 0x11)
#define ID_TIMER_PROCESS                (GUI_ID_USER + 0x12)
#define ID_WARNING_ERROR                (GUI_ID_USER + 0x13)
#define ID_WARNING_MSG                  (GUI_ID_USER + 0x14)
#define ID_TIMER_TOUCH                  (GUI_ID_USER + 0x15) 
#define ID_TIMER_CPU                    (GUI_ID_USER + 0x16) 
#define ID_BUTTON_MENU_INFO             (GUI_ID_USER + 0x17)

#define VIDEO_IDLE             0
#define VIDEO_PLAY             1
#define VIDEO_STOP             2
#define VIDEO_PAUSE            3

extern GUI_CONST_STORAGE GUI_BITMAP bminfo;
extern GUI_CONST_STORAGE GUI_BITMAP bmbkc_16bit;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "Window", VIDEO_FRAME_ID, 0, 0, 640, 480, 0, 0x0, 0 },
};

GUI_AVI_HANDLE havi;
uint32_t cpu = 0;
uint8_t toggle_hw = 1;
uint8_t toggle_chromART = 1;
uint8_t VideoPlayer_State = VIDEO_IDLE;
uint8_t SelLayer = 0;
uint8_t VisLayer = 1;
FIL     Video_File;
GUI_AVI_INFO Video_Info;

static uint8_t  toggle = 0;
static WM_HWIN  playbackwin = 0;
static WM_HWIN  TouchWindow = 0;
static WM_HWIN  VideoWin = 0;
static WM_HWIN  hFrame = 0;
static uint32_t  Video_FilePos = 0;
static WM_HTIMER hTimer; 
static WM_HTIMER TouchTimer;
static uint32_t  elapsed_time = 0;

/* Buffers for right and left samples */
static FILELIST_FileTypeDef VideoList;

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="video_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".video_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".video_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t VideoBuffer[AVI_VIDEO_BUF_SIZE];

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="audio_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".audio_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".audio_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t AudioBuffer[AUDIO_BUFFER_SIZE + 8];

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Paints menu button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_menu(BUTTON_Handle hObj) 
{  
  GUI_DrawBitmap(&bmmenu, 13, 17);  
}

/**
  * @brief  Paints play button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_play(BUTTON_Handle hObj) 
{  
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
  if(toggle_hw == 0)
  {
    GUI_DrawBitmap(&bmJPEG_umpressed, 6, 6);
  }
  else
  {
    GUI_DrawBitmap(&bmJPEG_pressed, 6, 6);
  }  
}

/**
  * @brief  Paints chromart button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_chromart(BUTTON_Handle hObj) 
{  
  if(toggle_chromART == 0)
  {
    GUI_DrawBitmap(&bmChromART_umpressed, 6, 6);
  }
  else
  {
    GUI_DrawBitmap(&bmChromART_pressed, 6, 6);
  }  
}

/**
  * @brief  Paints back button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_back(BUTTON_Handle hObj) 
{  
  GUI_DrawBitmap(&bmprevious, 0, 0);
}

/**
  * @brief  Paints forward button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_fore(BUTTON_Handle hObj) 
{  
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
    case  WM_TOUCH:
      pState = (const GUI_PID_STATE *)pMsg->Data.p;
      if (pState) 
      {
        if (pState->Pressed == 1)
        {
          if(!toggle)
          {
            VisLayer = 0;
          }
          else
          {
            hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_BACK);
            WM_ShowWindow(hItem);
            
            hItem = WM_GetDialogItem(playbackwin, PLAY_VIDEO_ID);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_FORE);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_BUTTON_MENU);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
            WM_ShowWindow(hItem);
          
            hItem = WM_GetDialogItem(playbackwin, ID_TIME);
            WM_ShowWindow(hItem);
          
            VisLayer = 1;
            WM_DeleteWindow(TouchWindow);
            TouchWindow = 0;
            WM_RestartTimer(TouchTimer, 5000);
          }
          toggle ^= 1;
          WM_Exec();
        }
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
      prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);    
      hStatusTimer = WM_CreateTimer(pMsg->hWin, ID_TIMER_CONNECTION, 500, 0);      
      break;
    case WM_TIMER:
      if(prev_msd_status != k_StorageGetStatus(MSD_DISK_UNIT))
      {
        prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);
        if(prev_msd_status == 0)
        { 
          if(VideoPlayer_State != VIDEO_IDLE)
          {
            _StopPlay(&Video_File);
          }   
          if(havi != 0)
          {
            GUI_AVI_Delete (havi);
          }
          havi = 0;
          GUI_EndDialog(playbackwin, 0);
          SelLayer = 0;
          GUI_SetLayerVisEx (1, 0);
          VisLayer = 2;
          GUI_SelectLayer(0); 
          WM_DeleteWindow(hFrame);
          WM_HideWindow(VideoWin);        
          WM_Exec();
          WM_DeleteWindow(VideoWin);
          hFrame = 0;    
        }
      }
      if(hStatusTimer != 0)
      {
        WM_RestartTimer(pMsg->Data.v, 500);
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
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Get data.
  * @param  p: Handle to file structure
  * @param  ppData: pointer to data buffer to be read
  * @param  NumBytesReq: number of bytes to be read
  * @param  Off: offset in the file    
  * @retval number of read bytes
  */
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off)
{
  unsigned int NumBytesRead;
  FRESULT res;
  
  if(Off != 0xFFFFFFFF)
  {
    /* Set file pointer to the required position */
    f_lseek((FIL *)p, Off);
  }
  
  /* Read data into buffer */
  res = f_read((FIL *)p, (U8 *)*ppData, NumBytesReq, &NumBytesRead);
  
  if((res != FR_OK) ||( NumBytesRead == 0))
  {
    NumBytesRead = 0;
  }
   
  /* Return number of available bytes */
  return NumBytesRead;  
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
  char tmp[64];
  int r = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:   
      hItem = BUTTON_CreateEx(205, 230, 21, 23, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_BACK);
      WM_SetCallback(hItem, _cbButton_back);
      
      hItem = BUTTON_CreateEx(250, 170, 140, 140, pMsg->hWin, WM_CF_SHOW, 0, PLAY_VIDEO_ID);
      WM_SetCallback(hItem, _cbButton_play);
          
      hItem = BUTTON_CreateEx(415, 230, 21, 23, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_FORE);
      WM_SetCallback(hItem, _cbButton_fore);
      
      hItem = BUTTON_CreateEx(15, 260, 77, 77, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HW_JPEG);
      WM_SetCallback(hItem, _cbButton_hw_jpeg); 
     
      hItem = BUTTON_CreateEx(15, 345, 77, 77, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CHROMART);
      WM_SetCallback(hItem, _cbButton_chromart);    
      
      hItem = BUTTON_CreateEx(580, 17, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MENU);
      WM_SetCallback(hItem, _cbButton_menu);      
      
      /* Title Initialization in play list */   
      hItem = TEXT_CreateEx(25, 445, 40, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_ELAPSED_TIME, "00:00");
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetTextColor(hItem, 0xFF3C3D48);
      
      /* Title Initialization in play list */   
      hItem = TEXT_CreateEx(595, 445, 40, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_TIME, "00:00");
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetTextColor(hItem, 0xFF3C3D48);
      
      hItem = TEXT_CreateEx(37, 26, 80, 15, pMsg->hWin, WM_CF_SHOW, TEXT_CF_LEFT, ID_FPS, "Fps: 20 fps");
      TEXT_SetFont(hItem, GUI_FONT_13B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);

      hItem = TEXT_CreateEx(130, 26, 85, 15, pMsg->hWin, WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_CPU, "CPU load: 0%");
      TEXT_SetFont(hItem, GUI_FONT_13B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE); 
      
      hItem = SLIDER_CreateEx(70, 450, 500, 6, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_DURATION);
      SLIDER_SetBkColor(hItem, GUI_TRANSPARENT);
      SLIDER_SetFocusColor (hItem, 0xFF8080FF);
      SLIDER_SetValue(hItem, 0);
      SLIDER_SetWidth(hItem, 0);
      SLIDER_SetSTSkin(hItem);    
     
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
            TouchWindow = WM_CreateWindowAsChild(100, 70, 440, 300, WM_GetDesktopWindowEx(1), WM_CF_SHOW | WM_CF_HASTRANS, _cbTouch , 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);
            WM_HideWindow(hItem);
            hItem = WM_GetDialogItem(pMsg->hWin, PLAY_VIDEO_ID);
            WM_HideWindow(hItem);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_FORE);
            WM_HideWindow(hItem);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
            WM_HideWindow(hItem);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
            WM_HideWindow(hItem);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME);
            WM_HideWindow(hItem);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
            WM_HideWindow(hItem);
            GUI_Exec1();
            VisLayer = 0;
            toggle ^= 1;
          }
          WM_RestartTimer(pMsg->Data.v, 5000);
        }
      }
      break;    
    case WM_DELETE:
      if(hTimer != 0)
      {
        WM_DeleteTimer(hTimer);
        hTimer = 0;
      }
      playbackwin = 0;
      break;    
    
    case WM_PAINT:
      GUI_SetBkColor(GUI_TRANSPARENT);
      GUI_Clear();
      GUI_SetColor(0x7F2D272C);
      GUI_FillRect(30, 20, 115, 45);
      GUI_FillRect(120, 20, 220, 45);
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
                if(VideoPlayer_State != VIDEO_IDLE)
                {
                  _StopPlay(&Video_File);
                }
                if(havi != 0)
                {
                  GUI_AVI_Delete (havi);
                }
                havi = 0;
                GUI_EndDialog(pMsg->hWin, 0);
                SelLayer = 0;
                GUI_SetLayerVisEx (1, 0);
                VisLayer = 2;
                GUI_SelectLayer(0); 
                WM_DeleteWindow(hFrame);
                WM_HideWindow(VideoWin);
                WM_Exec();
                WM_DeleteWindow(VideoWin);
                hFrame = 0;
                break;
            }
            break;
          case ID_BUTTON_STOP:
            switch(NCode) 
            {
              case WM_NOTIFICATION_RELEASED:
                if((VideoPlayer_State == VIDEO_PLAY) ||(VideoPlayer_State == VIDEO_PAUSE))
                {
                  VideoPlayer_State = VIDEO_STOP;
                  _StopPlay(&Video_File);
                  hItem = WM_GetDialogItem(hFrame, ID_WARNING_MSG);
                  TEXT_SetText(hItem, "Press Play to Start");
                  WM_InvalidateWindow(hItem);
                  WM_Paint(hItem);
                }
                WM_InvalidateWindow(pMsg->hWin);
                break;
            }
            break;
          case ID_BUTTON_FORE:
            switch(NCode)
            {
              case WM_NOTIFICATION_RELEASED:
                if(Video_FilePos < (VideoList.ptr - 1))
                {
                  Video_FilePos++;
                }
                else 
                {        
                  Video_FilePos = 0; 
                }  
                if((VideoPlayer_State == VIDEO_PLAY) ||
                   (VideoPlayer_State == VIDEO_PAUSE))
                {
                  _StopPlay(&Video_File);
                   VideoPlayer_State = VIDEO_PLAY;
                  _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0);
                }
                else
                {   
                  r = _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0); 
                  _StopPlay(&Video_File);  
                  if(r == 0)
                  {
                    hItem = WM_GetDialogItem(hFrame, ID_WARNING_MSG);
                    TEXT_SetText(hItem, "Press Play to Start");
                    WM_InvalidateWindow(hItem);
                    WM_Paint(hItem);        
                  }
                }
                break;
            }
            break;
            /* Notifications sent by 'progress' Slider */
        case ID_SLIDER_DURATION:
          if(NCode == WM_NOTIFICATION_CLICKED)
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
            int32_t newpos;
            if(VideoPlayer_State == VIDEO_PLAY)
            {
              GUI_AVI_Pause(havi);
              hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
              newpos = (SLIDER_GetValue(hItem) * Video_Info.NumFrames)/100;
              GUI_AVI_GotoFrame(havi, newpos);
              GUI_AVI_Play(havi);
              elapsed_time = (Video_Info.msPerFrame * newpos)/ 1000;
              hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
              sprintf((char *)tmp , "%02lu:%02lu", elapsed_time/60, elapsed_time%60 );
              TEXT_SetText(hItem, tmp); 
              WM_InvalidateWindow(hItem);
              WM_Paint(hItem);
            }
            if(VideoPlayer_State == VIDEO_PAUSE)
            {
              hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
              newpos = (SLIDER_GetValue(hItem) * Video_Info.NumFrames)/100;
              GUI_AVI_GotoFrame(havi, newpos);
              elapsed_time = (Video_Info.msPerFrame * newpos)/ 1000;
              hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
              sprintf((char *)tmp , "%02lu:%02lu", elapsed_time/60, elapsed_time%60 );
              TEXT_SetText(hItem, tmp);
              WM_InvalidateWindow(hItem);
              WM_Paint(hItem);
            }
          }
          break;
        case ID_BUTTON_BACK:
          switch(NCode)
          {
            case WM_NOTIFICATION_RELEASED:
              if(Video_FilePos > 0)
              {
                Video_FilePos--;
              }
              else 
              {        
                Video_FilePos = VideoList.ptr - 1; 
              }  
              if((VideoPlayer_State == VIDEO_PLAY) ||
                 (VideoPlayer_State == VIDEO_PAUSE))
              {
                _StopPlay(&Video_File);
                VideoPlayer_State = VIDEO_PLAY;          
                _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0);
              }
              else
              {
                r = _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0); 
                _StopPlay(&Video_File);
                if(r == 0)
                {
                  hItem = WM_GetDialogItem(hFrame, ID_WARNING_MSG);
                  TEXT_SetText(hItem, "Press Play to Start");
                  WM_InvalidateWindow(hItem);
                  WM_Paint(hItem); 
                }
              }
              break;
          }
          break;      
        case PLAY_VIDEO_ID:
          switch(NCode)
          {
            case WM_NOTIFICATION_RELEASED:
              if((VideoPlayer_State == VIDEO_IDLE) || (VideoPlayer_State == VIDEO_PAUSE))
              {
                hTimer = WM_CreateTimer(playbackwin, ID_TIMER_PROCESS, 250, 0);  
                GUI_AVI_Play(havi); 
                VideoPlayer_State = VIDEO_PLAY;
              }
              else if(VideoPlayer_State == VIDEO_STOP)
              {
                VideoPlayer_State = VIDEO_PLAY;          
                _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File, 0, 0);
              }
              else
              {
                GUI_AVI_Pause(havi);
                VideoPlayer_State = VIDEO_PAUSE; 
                if(hTimer != 0)
                {
                  WM_DeleteTimer(hTimer);
                  hTimer = 0;
                }            
              }
              break;
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
  * @brief  Callback function for movie player. Uses multiple buffering if
  *         available to avoid tearing effects.
  * @param  hMem:         pointer to parent handle
  * @param  Notification: notification ID
  * @param  CurrentFrame: current playing frame
  * @retval None
  */
void _cbNotify(GUI_HMEM hMem, int Notification, U32 CurrentFrame) 
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
      
    case GUI_AVI_NOTIFICATION_PREDRAW:
      GUI_SelectLayer(0); 
      GUI_MULTIBUF_Begin();
      FrameInitTick = osKernelSysTick(); 
      break;
      
    case GUI_AVI_NOTIFICATION_POSTDRAW:
      FrameDuration = osKernelSysTick() - FrameInitTick;
      /* We need to limit the FPS to 25fps */
      if(FrameDuration < 40)
      {
        osDelay(40 - FrameDuration);
        FrameDuration = 40;
      }
      if(((osKernelSysTick() - InitTick ) > 250) || (InitTick == 0))
      {
        hItem = WM_GetDialogItem(playbackwin, ID_FPS);
        sprintf(tmp, "Rate : %lu fps", 1000/FrameDuration);
        TEXT_SetText(hItem, tmp);
        WM_InvalidateWindow(hItem);
        WM_Paint(hItem);
        
        InitTick = osKernelSysTick();
        
        if(VisLayer == 1)
        {
          hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
          SLIDER_SetValue(hItem, (GUI_AVI_GetFrameIndex(havi)* 100)/ Video_Info.NumFrames); 
          hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
          elapsed_time = (Video_Info.msPerFrame * GUI_AVI_GetFrameIndex(havi))/ 1000;
          sprintf((char *)tmp , "%02lu:%02lu", elapsed_time/60, elapsed_time%60 );
          TEXT_SetText(hItem, tmp);
          WM_InvalidateWindow(hItem);
          WM_Paint(hItem);
        }
      }
      GUI_MULTIBUF_End();
      if(VisLayer == 1)
      {
        GUI_SelectLayer(0); 
      }
      break;
         
    case GUI_AVI_NOTIFICATION_EOF:
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
  
  if(f_open(file, filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {   
    elapsed_time = 0;
    GUI_AVI_Start   (havi, file, f_size(file));
    GUI_AVI_GetInfo (havi, &Video_Info);
    
    if((Video_Info.xSize != LCD_GetXSize()) || (Video_Info.ySize != LCD_GetYSize()))
    {
      _StopPlay(file);
      VideoPlayer_State = VIDEO_STOP;
      
      hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
      TEXT_SetText(hItem, "00:00");
      WM_InvalidateWindow(hItem);
      WM_Paint(hItem);
      
      hItem = WM_GetDialogItem(playbackwin, ID_TIME);
      TEXT_SetText(hItem, "00:00");
      WM_InvalidateWindow(hItem);
      WM_Paint(hItem);
      
      hItem = WM_GetDialogItem(playbackwin, PLAY_VIDEO_ID);
      WM_InvalidateWindow(hItem);
      WM_Paint(hItem);
      
      hItem = WM_GetDialogItem(hFrame, ID_WARNING_ERROR);
      TEXT_SetText(hItem, "Error : Not Supported Video (Press Next/Previous)");
      WM_InvalidateWindow(hItem);
      WM_Paint(hItem);
      
      hItem = WM_GetDialogItem(hFrame, ID_WARNING_MSG);
      TEXT_SetText(hItem, "Only M-JPEG Format and 640x480 video are supported");
      WM_InvalidateWindow(hItem);
      WM_Paint(hItem);      
        
      return 1;
    }
    
    GUI_AVI_Show(havi, ((LCD_GetXSize() - Video_Info.xSize)/2), ((LCD_GetYSize() - Video_Info.ySize)/2), 0); 
    
    hTimer = WM_CreateTimer(playbackwin, ID_TIMER_PROCESS, 250, 0);  
    
    /* Update video total time */
    hItem = WM_GetDialogItem(playbackwin, ID_TIME);
    ms = Video_Info.msPerFrame;
    frames = Video_Info.NumFrames;
    duration = (frames * ms)/1000;
    sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
    TEXT_SetText(hItem, tmp);          
    WM_InvalidateWindow(playbackwin);  
    
    hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
    TEXT_SetText(hItem, "00:00");
    WM_InvalidateWindow(hItem);
    WM_Paint(hItem);
    
    hItem = WM_GetDialogItem(hFrame, ID_WARNING_MSG);
    TEXT_SetText(hItem, "");
    WM_InvalidateWindow(hItem);
    WM_Paint(hItem);    
    
    hItem = WM_GetDialogItem(hFrame, ID_WARNING_ERROR);
    TEXT_SetText(hItem, "");
    WM_InvalidateWindow(hItem);
    WM_Paint(hItem);    
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
  WM_HWIN  hItem;
  
  elapsed_time = 0;
  GUI_AVI_Stop (havi);
  
  hItem = WM_GetDialogItem(playbackwin, ID_ELAPSED_TIME);
  TEXT_SetText(hItem, "00:00");
  hItem = WM_GetDialogItem(playbackwin, ID_SLIDER_DURATION);
  SLIDER_SetValue(hItem, 0);
  
  if(hTimer != 0)
  {
    WM_DeleteTimer(hTimer);
    hTimer = 0;
  }
  f_close(file); 
  WM_InvalidateWindow(hFrame);  
}

/**
  * @brief  callback for video window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbVideoWindow(WM_MESSAGE * pMsg) 
{
  WM_HWIN  hItem;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    hItem = TEXT_CreateEx(70, 220, 500, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_WARNING_MSG, "");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20B_1);
    TEXT_SetTextColor(hItem, GUI_BLACK);   
    hItem = TEXT_CreateEx(70, 170, 500, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_WARNING_ERROR, "");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20B_1);
    TEXT_SetTextColor(hItem, GUI_BLACK);  
    
    break;
  case WM_PAINT:
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
static void _cbDialog_info(WM_MESSAGE * pMsg) {
  WM_HWIN  hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) 
  {
    case WM_INIT_DIALOG:
      WM_CreateWindowAsChild(639, 479, 1, 1, VideoWin, WM_CF_SHOW, _cbMediaConnection , 0);
      hItem = BUTTON_CreateEx(580, 17, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MENU_INFO);
      WM_SetCallback(hItem, _cbButton_menu); 
      
      hItem = TEXT_CreateEx(180, 195, 400, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_FILENAME, "Please Add Video file in the SDCard");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      break;
    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_16bit, 0, 0);
      GUI_DrawBitmap(&bminfo, 155, 180);
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch(Id) 
      {
        case ID_BUTTON_MENU_INFO:
          switch(NCode) 
          {
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
  * @brief  Callback routine of the video main dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
 
  switch (pMsg->MsgId) 
  {
    case WM_INIT_DIALOG:
      WM_CreateWindowAsChild(639, 479, 1, 1, VideoWin, WM_CF_SHOW, _cbMediaConnection , 0);
      
      /* Playlist not empty, so start play first item */
      if(VideoList.ptr > 0)
      {  
        hFrame = WM_CreateWindowAsChild(0, 0, 640, 480,pMsg->hWin, WM_CF_SHOW, _cbVideoWindow , 0);
        GUI_SelectLayer(1); 
        VisLayer = 1;
        playbackwin = WM_CreateWindowAsChild(0, 0, 640, 480, WM_GetDesktopWindowEx(1), WM_CF_SHOW, _cbplaybackwin , 0);              
        GUI_SelectLayer(0);
        VideoPlayer_State = VIDEO_PLAY;  
        _StartPlay((char *)VideoList.file[0].name, &Video_File, 0, 0);     
        GUI_SelectLayer(1);
      }
      break;

    case WM_PAINT: 
      GUI_SetColor(GUI_WHITE);
      if(VideoList.ptr == 0)
      {
        GUI_SetBkColor(0xFF884130);
        GUI_Clear();
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }    
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
  char *fn;
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 
  
  res = f_opendir(&dir, Foldername);
  
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
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            
            strncpy((char *)VideoList.file[VideoList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            VideoList.ptr++;
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
  VideoPlayer_State = VIDEO_IDLE;  
  Video_FilePos = 0;
    
  havi = GUI_AVI_CreateEx(_GetData, _cbNotify) ;
  
  GUI_AVI_SetDevFunc (havi, GUI_AVI_JPEG_INIT, (void(*)(void))HW_JPEG_Init);
  GUI_AVI_SetDevFunc (havi, GUI_AVI_JPEG_DEINIT, (void(*)(void))HW_JPEG_DeInit);
  GUI_AVI_SetDevFunc (havi, GUI_AVI_JPEG_DRAW, (void(*)(void))HW_JPEG_Draw);

  GUI_AVI_SetBuffers (havi, VideoBuffer, AVI_VIDEO_BUF_SIZE, AudioBuffer, AUDIO_BUFFER_SIZE);
  /* Mode: 1 -> HW, 0 -> SW */
  GUI_AVI_SelectMode(havi, 1); 
  
  VisLayer = 0;
  SelLayer = 0;
  TouchWindow = 0;
    
  toggle_hw = 1;
  VideoWin = 0;
  if(VideoList.ptr == 0)
  {
    _AddEntireFolder("0:");
  }
  _AddEntireFolder("0:/Video");
    
  if(VideoList.ptr == 0)
  {
    SelLayer = 0;
    VideoWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog_info, hWin, xpos, ypos);
  }
  else
  {
    GUI_SetLayerVisEx(1, 1);
    SelLayer = 1;
    VideoWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
    WM_EnableMemdev(VideoWin);
  }
}

/**
  * @}
  */

/**
  * @}
  */
  
