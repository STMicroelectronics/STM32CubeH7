/**
  ******************************************************************************
  * @file    audioplayer_win.c
  * @author  MCD Application Team
  * @brief   Audio player functions
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
#include "audio_player_app.h"
#include "audio_player_res.c"
#include "visualization.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_PLAYER
  * @brief audio player routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void FILEMGR_GetFileOnly (char *file, char *path);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_player_board =
{
  1,
  "Audio Player",
  0,
  0,
  Startup,
  NULL,
};

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t repeat         : 2;
    uint32_t pause          : 1;
    uint32_t play           : 1;
    uint32_t stop           : 1;    
    uint32_t mute           : 1;
    uint32_t volume         : 8;   
    uint32_t reserved       : 18;
  }b;
}
AudioSettingsTypeDef;

static int VisEqClicked = 1;
static int ShuffleClicked = 1;

/* Buffer that will contain the visualisation effects */
static const GUI_BITMAP pBMVisual = {
320,                            /* xSize*/
190,                            /* ySize */
640,                            /* BytesPerLine */
16,                             /* BitsPerPixel */
(unsigned char *)0xD0400000,    /* Pointer to picture data */
NULL,                           /* Pointer to palette */
GUI_DRAW_BMPM565
};

extern uint8_t visualization_samples[64];

/* Private defines -----------------------------------------------------------*/
#define ID_WiIN_BKC_PROCESS_TIMER 0x12

#define REPEAT_NONE           0x00
#define REPEAT_ALL            0x01

#define PLAY_INACTIVE         0x00
#define PLAY_ACTIVE           0x01

#define PAUSE_INACTIVE        0x00
#define PAUSE_ACTIVE          0x01

#define STOP_INACTIVE         0x00  
#define STOP_ACTIVE           0x01

#define ID_PROCESS_TIMER      0x01
#define ID_PLAYLIST_TIMER     0x02

#define ID_FRAMEWIN_INFO          (GUI_ID_USER + 0x01)
#define ID_WAVFILE_LIST           (GUI_ID_USER + 0x02)
#define ID_PREVIOUS_BUTTON        (GUI_ID_USER + 0x03)
#define ID_PLAY_BUTTON            (GUI_ID_USER + 0x04)
#define ID_NEXT_BUTTON            (GUI_ID_USER + 0x05)
#define ID_STOP_BUTTON            (GUI_ID_USER + 0x06)
#define ID_PAUSE_BUTTON           (GUI_ID_USER + 0x07)
#define ID_PLAY_LIST_BUTTON       (GUI_ID_USER + 0x08)
#define ID_SHUFFLE_BUTTON         (GUI_ID_USER + 0x09)
#define ID_REPEAT_BUTTON          (GUI_ID_USER + 0x0A)
#define ID_VISUALEQ_BUTTON        (GUI_ID_USER + 0x0B)
#define ID_BACK_BUTTON            (GUI_ID_USER + 0x0C)
#define ID_PLAY_LIST_BACK_BUTTON  (GUI_ID_USER + 0x0D)
#define ID_TITLE                  (GUI_ID_USER + 0x0E)
#define ID_ELAPSED_TIME           (GUI_ID_USER + 0x0F)
#define ID_TOTAL_TIME             (GUI_ID_USER + 0x10)
#define ID_SLIDER_VOLUME          (GUI_ID_USER + 0x11)
#define ID_SLIDER_DURATION        (GUI_ID_USER + 0x12)

/* Polygon to fill on the main window */
const GUI_POINT aPoints[] = {
{ 0, 338},
{ 639, 338},
{ 639, 479},
{ 0, 479}
};

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect,    "Audio Player",   ID_FRAMEWIN_INFO,       0, 0, 640, 480, 0, WM_CF_HASTRANS, 0 },
  { TEXT_CreateIndirect,      "",               ID_TITLE,               0,  288,  640,  48,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "00:00",          ID_ELAPSED_TIME,        180, 457,  30,  22,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "00:00",          ID_TOTAL_TIME,          444, 457,  30,  22,  0, 0x0,  0 },
};

static WM_HWIN hMainWin; 
static WM_HWIN hMainList; 
static WM_HWIN hMainVisEq; 
static WM_HTIMER hProcessTimer;
static WM_HTIMER hStatusTimer;
static WM_HTIMER hVisEqTimer;
static WAV_InfoTypedef       WavInfo;
static FILELIST_FileTypeDef  WavList;
static uint16_t              Audio_FilePos = 0; 
static AudioSettingsTypeDef  PlayerSettings;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  callback for audio process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioProcess(WM_MESSAGE * pMsg) 
{
  uint32_t Id;  
  WM_HWIN hItem;
  char tmp[12] = "00:00";
  long progress;
  uint32_t duration;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hProcessTimer = WM_CreateTimer(pMsg->hWin, ID_PROCESS_TIMER, 1000, 0);
      break;
      
    case WM_TIMER:
      Id = WM_GetTimerId(pMsg->Data.v);
      if(Id == ID_PROCESS_TIMER)
      {
        if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
        {
          progress = AUDIOPLAYER_GetProgress();
          /*Set elapsed time */
          duration = progress / WavInfo.ByteRate;
          sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
          hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, tmp);
          /* Set progress slider position */
          hItem = WM_GetDialogItem(hMainWin, ID_SLIDER_DURATION);
          progress = (long)(progress/(WavInfo.FileSize/100));
          SLIDER_SetValue(hItem, progress);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
        else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_EOF)
        {
          /* Set progress slider position */
          hItem = WM_GetDialogItem(hMainWin, ID_SLIDER_DURATION);
          SLIDER_SetValue(hItem, 100);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
          duration = WavInfo.FileSize / WavInfo.ByteRate;
          sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
          hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, tmp);
        }
        if(pMsg->Data.v != 0)
        {
          WM_RestartTimer(pMsg->Data.v, 1000);
        }
      }
      break; 
    case WM_DELETE:
      if(hProcessTimer != 0)
      {
        WM_DeleteTimer(hProcessTimer);
        hProcessTimer = 0;
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
static void _AddEntireFolder(char *Foldername, int update)
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
      
      if (WavList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(((strstr(fn, ".wav")) || (strstr(fn, ".WAV"))))
          {
            
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            strncpy((char *)WavList.file[WavList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            
            if((update == 1) && (hMainWin != 0))
            {
              if(AUDIOPLAYER_GetFileInfo(tmp, &WavInfo) == 0)
              {
                FILEMGR_GetFileOnly (tmp, fn);                
              }
            }
            WavList.ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}


/**
  * @brief  Play wav file.
  * @param  filename: pointer to file name.
  * @retval None
  */
static void _PlayFile(char *filename)
{
  uint32_t duration;
  static char tmp[FILEMGR_FILE_NAME_SIZE];  
  WM_HWIN hItem;
  
  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
  {
    if(AUDIOPLAYER_GetFileInfo(filename, &WavInfo) == 0)
    {
      /* Title */
      FILEMGR_GetFileOnly (tmp, filename);
      TEXT_SetTextColor(WM_GetDialogItem(hMainWin, ID_TITLE), GUI_WHITE);
      TEXT_SetFont(WM_GetDialogItem(hMainWin, ID_TITLE), &GUI_FontLubalGraph24B);
      TEXT_SetTextAlign(WM_GetDialogItem(hMainWin, ID_TITLE), TEXT_CF_HCENTER | TEXT_CF_VCENTER);
      TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), tmp);  
      
      /* Total Time */
      duration = WavInfo.FileSize / WavInfo.ByteRate;     
      sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
      hItem = WM_GetDialogItem(hMainWin, ID_TOTAL_TIME);
      TEXT_SetText(hItem, tmp); 
      
      /* Open audio file */
      if(AUDIOPLAYER_SelectFile(filename) == 0)
      {
        /* start playing */
        AUDIOPLAYER_Play(WavInfo.SampleRate);
        if(PlayerSettings.b.mute == MUTE_ON)
        {
          AUDIOPLAYER_Mute(MUTE_ON);
        }
      }
    }
    else
    {
      TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), "Unknown");  
      hItem = WM_GetDialogItem(hMainWin, ID_TOTAL_TIME);
      TEXT_SetText(hItem, "--:--");      
    }  
  }
}

/**
  * @brief  Notify the end of wav file.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  WM_HWIN hItem;
  
  AUDIOPLAYER_Stop();
  if(PlayerSettings.b.repeat != REPEAT_ALL)
  {
    if(Audio_FilePos < (WavList.ptr - 1))
    {
      if(ShuffleClicked == 0)
      {
        /* Add the shuffle part */
        Audio_FilePos = (rand()) % (WavList.ptr - 1);
      }
      else
      {
        Audio_FilePos++; 
      }
    }
    else 
    {        
      Audio_FilePos = 0;      
    }
    _PlayFile((char *)WavList.file[Audio_FilePos].name);
  }
  else
  {
    PlayerSettings.b.play = PLAY_INACTIVE; 
    hItem = WM_GetDialogItem(hMainWin, ID_PLAY_BUTTON);
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);
  }

  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Callback function of the media connection status
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMediaConnection(WM_MESSAGE * pMsg) 
{
  static uint8_t  prev_msd_status = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);
      hStatusTimer = WM_CreateTimer(pMsg->hWin, 0, 500, 0);
      break;
    
  case WM_TIMER:
    if(prev_msd_status != k_StorageGetStatus(MSD_DISK_UNIT))
    {
      prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);
      if(prev_msd_status == 0)
      {        
        PlayerSettings.b.play = PLAY_INACTIVE;
        PlayerSettings.b.pause = PAUSE_INACTIVE;
        
        WavList.ptr = 0;
        Audio_FilePos = 0;
        
        AUDIOPLAYER_Stop();
        GUI_EndDialog(hMainWin, 0);
      }
    }
    if(hStatusTimer != 0)
    {
      WM_RestartTimer(hStatusTimer, 500);
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
  * @brief  Function to draw list back button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_play_listback_button(BUTTON_Handle hObj) 
{
  GUI_DrawBitmap(&bmPlaylistMenu, 9, 7);
}

/**
  * @brief  Callback of the list back button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_play_listback_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_play_listback_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}


/**
  * @brief  Callback routine of the list dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbMainListDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;  
  int     NCode, Id; 
  int     ItemNbr;
  uint32_t duration, index;
  static char tmp[40];
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
   
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hItem = BUTTON_CreateEx(588, 36, 40, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_LIST_BACK_BUTTON);
      WM_SetCallback(hItem, _cbButton_play_listback_button); 
    
      /* Initialization of 'Listview' */
      hItem = LISTVIEW_CreateEx(107, 56, 425, 300, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_WAVFILE_LIST);
      LISTVIEW_SetFont(hItem, &GUI_FontLubalGraph24B);
      LISTVIEW_AddColumn(hItem, 360, "Track", GUI_TA_VCENTER | GUI_TA_LEFT);
      LISTVIEW_AddColumn(hItem, 60, "Duration   ", GUI_TA_VCENTER);
      LISTVIEW_SetGridVis(hItem, 0);
      LISTVIEW_SetAutoScrollV(hItem, 0);
      LISTVIEW_SetHeaderHeight(hItem, 0);    
      LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, GUI_TRANSPARENT);
      LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL, GUI_STCOLOR_LIGHTBLUE);
      LISTVIEW_SetFont(hItem, &GUI_FontLubalGraph24B);
      LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL, GUI_STCOLOR_LIGHTBLUE);
      LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SEL, GUI_WHITE);
      LISTVIEW_SetRowHeight(hItem, 30);
      LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, GUI_STCOLOR_LIGHTBLUE);
      LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
      WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
      
      ItemNbr = 0;
      if(WavList.ptr > 0)
      {
        while(ItemNbr < WavList.ptr)
        {
          if(AUDIOPLAYER_GetFileInfo((char *)WavList.file[ItemNbr].name, &WavInfo) == 0)
          {
            LISTVIEW_AddRow(hItem, NULL);
            FILEMGR_GetFileOnly (tmp, (char *)WavList.file[ItemNbr].name);
            LISTVIEW_SetItemText(hItem, 0,ItemNbr, tmp);
            duration = WavInfo.FileSize / WavInfo.ByteRate; 
            sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
            LISTVIEW_SetItemText(hItem, 1, ItemNbr, tmp);
            LISTVIEW_SetItemTextColor(hItem, 1, ItemNbr, LISTVIEW_CI_UNSEL, GUI_STCOLOR_DARKBLUE);
            ItemNbr++;
          }
          else
          {
            WavList.ptr = ItemNbr;
            break;
          }
        }
        LISTVIEW_SetSel(hItem, 0);     
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
        FILEMGR_GetFileOnly (tmp, (char *)WavList.file[0].name);      
        TEXT_SetText(hItem, tmp);
      }
      else
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
        TEXT_SetText(hItem, "No available wav files");
      }
      break;
    case WM_PAINT:
      GUI_DrawBitmap(&bmBkc_list, 0, 0); 
      break;
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) {   
        case ID_PLAY_LIST_BACK_BUTTON:
          if(NCode == WM_NOTIFICATION_CLICKED)
          {        
            WM_HideWindow(hMainList);        
            if(VisEqClicked == 0)
            {
              WM_ShowWindow(hMainVisEq);
            }
          }
          break;
        /* Notifications sent by 'ListView' */
        case ID_WAVFILE_LIST:
          if(NCode == WM_NOTIFICATION_CLICKED)
          {        
            GUI_PID_GetCurrentState(&TS_State);
            
            if((TS_State.x < 532 ) & (TS_State.y > 56) & (TS_State.y < (56 + 30*(WavList.ptr))))
            {
              hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
              index = LISTVIEW_GetSel(hItem);
              
              if(index < WavList.ptr)
              {              
                  if(index < WavList.ptr)
                  {
                    
                    if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
                    {    
                        AUDIOPLAYER_Stop();
                    }
                    
                    PlayerSettings.b.pause = PLAY_ACTIVE;
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
                    WM_InvalidateWindow(hItem);
                    WM_Update(hItem);
                    
                    Audio_FilePos = index;
                    _PlayFile((char *)WavList.file[index].name); 
                    
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
                    LISTVIEW_SetItemTextColor(hItem ,0, Audio_FilePos, LISTVIEW_CI_UNSEL, GUI_STCOLOR_DARKBLUE);           
                    WM_InvalidateWindow(hItem);
                    WM_Update(hItem); 
                      
                    PlayerSettings.b.play = PLAY_ACTIVE;
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
                    WM_InvalidateWindow(hItem);
                    WM_Update(hItem); 
                      
                      
                    PlayerSettings.b.pause = PAUSE_INACTIVE;
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
                    WM_InvalidateWindow(hItem);
                    WM_Update(hItem);
                    /* Hide the list, no more needed */
                    WM_HideWindow(hMainList);                   
                    
                    if(VisEqClicked == 0)
                    {
                      WM_ShowWindow(hMainVisEq);
                    }                
                  }
                }
              }
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
  * @brief  Callback routine of the visual equaliser dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbMainVisEqDialog(WM_MESSAGE * pMsg) {
  int Id, NCode;
    
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      memset((void*)0xD0400000, 0x00, 320 * 190 * 2);
      hVisEqTimer = WM_CreateTimer(pMsg->hWin, ID_WiIN_BKC_PROCESS_TIMER, 30, 0);
      break;
    case WM_PAINT:
      visualization_copy_framebuffer((void*)0xD0400000, (320), 0, 0, 320, 190);
      visualization_render(visualization_samples, 64);
      GUI_DrawBitmap(&pBMVisual, 0, 0);
      break;
    case WM_TIMER:
      NCode = pMsg->Data.v;
      Id = WM_GetTimerId(pMsg->Data.v);
      if (Id == ID_WiIN_BKC_PROCESS_TIMER)
      {
        WM_InvalidateWindow(hMainVisEq);
        WM_Update(hMainVisEq);
      }
      if(NCode != 0)
      {
        WM_RestartTimer(pMsg->Data.v, 30);
      }
      break;
    case WM_TOUCH:
      visualization_next();
      break;
      
    case WM_DELETE:
      if(hVisEqTimer != 0)
      {
        WM_DeleteTimer(pMsg->Data.v); 
        hVisEqTimer = 0;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw list back button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_play_list_button(BUTTON_Handle hObj) 
{
  GUI_DrawBitmap(&bmPlaylistButton, 0, 0);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontLubalGraph24B);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_DispStringAt("PLAYLIST", 16, 10);
}

/**
  * @brief  Callback of the playlist back button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_play_list_button(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_play_list_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw shuffle button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_shuffle_button(BUTTON_Handle hObj)
{  
  if(ShuffleClicked == 0)
  {
    GUI_DrawBitmap(&bmShuffleClicked, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmShuffleUnclicked, 0, 0);    
  }
}

/**
  * @brief  Callback of the shuffle button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_shuffle_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_shuffle_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw repeat button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_repeat_button(BUTTON_Handle hObj) 
{  
  if(PlayerSettings.b.repeat == REPEAT_NONE)
  {
    GUI_DrawBitmap(&bmRepeatUnclicked, 0, 0);
  }
  else   
  {
    GUI_DrawBitmap(&bmRepeatClicked, 0, 0);
  }
}

/**
  * @brief  Callback of the repeat button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_repeat_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_repeat_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw play button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_play_button(BUTTON_Handle hObj) 
{
  if(PlayerSettings.b.play == PLAY_INACTIVE)
  {
    GUI_DrawBitmap(&bmPlay, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmplay_clicked, 0, 0);    
  }
}

/**
  * @brief  Callback of the play button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_play_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_play_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw next button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_next_button(BUTTON_Handle hObj) 
{  
  GUI_DrawBitmap(&bmNext, 0, 0);  
}

/**
  * @brief  Callback of the next button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_next_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_next_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw previous button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_previous_button(BUTTON_Handle hObj) 
{  
  GUI_DrawBitmap(&bmPrevious, 0, 0);  
}

/**
  * @brief  Callback of the previous button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_previous_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_previous_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw visual equaliser button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_visualeq_button(BUTTON_Handle hObj) 
{
  if(VisEqClicked == 0)
  {
    GUI_DrawBitmap(&bmVisualUnselected, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmVisualSelected, 0, 0);    
  }
}

/**
  * @brief  Callback of the visual equaliser button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_visualeq_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_visualeq_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw back button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_back_button(BUTTON_Handle hObj)
{  
  GUI_DrawBitmap(&bmPlaylist, 0, 0);  
}

/**
  * @brief  Callback of the back button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_back_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_back_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw pause button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_pause_button(BUTTON_Handle hObj) 
{ 
  GUI_DrawBitmap(&bmbutton_pause, 0, 0);  
}

/**
  * @brief  Callback of the pause button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_pause_button(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_pause_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Function to draw stop button
  * @param  hObj: handle of the button
  * @retval None
  */
static void _OnPaint_stop_button(BUTTON_Handle hObj) 
{ 
  GUI_DrawBitmap(&bmbutton_stop, 0, 0);  
}

/**
  * @brief  Callback of the stop button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_stop_button(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      _OnPaint_stop_button(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of the audio main dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMainDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  static char tmp[40]; 

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:    
    /* Then add all the needed buttons */
    /* Playlist */
    hItem = BUTTON_CreateEx(266, 10, 128, 46, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_LIST_BUTTON);
    WM_SetCallback(hItem, _cbButton_play_list_button);  
    /* Shuffle */
    hItem = BUTTON_CreateEx(220, 14, 38, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_SHUFFLE_BUTTON);
    WM_SetCallback(hItem, _cbButton_shuffle_button);    
    /* Repeat */
    hItem = BUTTON_CreateEx(398, 14, 42, 39, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat_button);
    /* Play */
    hItem = BUTTON_CreateEx(265, 348, 110, 110, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_BUTTON);
    WM_SetCallback(hItem, _cbButton_play_button);
    /* Previous */
    hItem = BUTTON_CreateEx(140, 370, 61, 61, pMsg->hWin, WM_CF_SHOW, 0, ID_PREVIOUS_BUTTON);
    WM_SetCallback(hItem, _cbButton_previous_button);
    /* Next */
    hItem = BUTTON_CreateEx(445, 370, 61, 61, pMsg->hWin, WM_CF_SHOW, 0, ID_NEXT_BUTTON);
    WM_SetCallback(hItem, _cbButton_next_button);
    /* Stop Button */
    hItem = BUTTON_CreateEx(380, 370, 60, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_STOP_BUTTON);
    WM_SetCallback(hItem, _cbButton_stop_button);
    /* Pause Button */
    hItem = BUTTON_CreateEx(205, 370, 60, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_PAUSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_pause_button);
    /* Visual equaliser */
    hItem = BUTTON_CreateEx(600, 385, 32, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_VISUALEQ_BUTTON);
    WM_SetCallback(hItem, _cbButton_visualeq_button);
    /* Back Button */
    hItem = BUTTON_CreateEx(592, 30, 40, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BACK_BUTTON);
    WM_SetCallback(hItem, _cbButton_back_button);
    /* Slider Duration */
    hItem = SLIDER_CreateEx(214, 460, 222, 8, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_DURATION);
    SLIDER_SetValue(hItem, 0);
    SLIDER_SetWidth(hItem, 0);
    SLIDER_SetSTSkin(hItem); 

    /* Volume slider */
    hItem = SLIDER_CreateEx(0, 338, 640, 10, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_VOLUME);
    SLIDER_SetValue(hItem, PlayerSettings.b.volume);
    SLIDER_SetWidth(hItem, 0);
    SLIDER_SetSTSkin(hItem);
    
    WM_CreateWindowAsChild(0,
                           100,
                           10,
                           10,
                           pMsg->hWin, 
                           WM_CF_SHOW,
                           _cbAudioProcess, 
                           0);
    
    /* Media connection */
    WM_CreateWindowAsChild(478, 252, 1, 1, pMsg->hWin, WM_CF_SHOW, _cbMediaConnection , 0);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetFocusColor(hItem, 0xFF821CDF);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_VOLUME);
    SLIDER_SetFocusColor(hItem, 0xFF39A9DC);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    break;
    
  case WM_PAINT:
    /* Add the Cover picture if it exists */   
    GUI_DrawBitmap(&bmCover_Bkc, 0, 0);
    GUI_DrawBitmap(&bmBarHaut, 0, 0);
    GUI_DrawBitmap(&bmBarHaut, 0, 278);   
    GUI_SetColor(GUI_WHITE);
    GUI_FillPolygon(aPoints, GUI_COUNTOF(aPoints), 0, 0);
    break;
    
  case WM_DELETE:
    /* Delete and reset all concerned values */
    AUDIOPLAYER_DeInit();   
    PlayerSettings.b.mute = MUTE_OFF;  
    PlayerSettings.b.pause = PAUSE_INACTIVE;
    PlayerSettings.b.play = PLAY_INACTIVE;
    PlayerSettings.b.stop = STOP_INACTIVE;       
    k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_SETTING_BKP, PlayerSettings.d32);    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;

    switch(Id) 
    {
      case ID_VISUALEQ_BUTTON:
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if (WM_IsCompletelyVisible(hMainVisEq))
          {
            WM_HideWindow(hMainVisEq);
            VisEqClicked = 1;
          }
          else
          {
            WM_ShowWindow(hMainVisEq);
            VisEqClicked = 0;
          }
        }
        break;      
      /* Notifications sent by 'Repeat' Button */
      case ID_REPEAT_BUTTON:
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if(PlayerSettings.b.repeat == REPEAT_ALL)
          {
            PlayerSettings.b.repeat = REPEAT_NONE;
            hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);
          }
          else
          {
            PlayerSettings.b.repeat = REPEAT_ALL;
            hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);          
          }
        }
        break;    
      /* Notifications sent by 'Volume' Slider */
      case ID_SLIDER_VOLUME: 
        if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_VOLUME);
          AUDIOPLAYER_SetVolume(SLIDER_GetValue(hItem));
          PlayerSettings.b.volume = AUDIOPLAYER_GetVolume();
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
        break;      
        
      /* Notifications sent by 'progress' Slider */
      case ID_SLIDER_DURATION: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
          AUDIOPLAYER_SetPosition(SLIDER_GetValue(hItem));
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
        break;      
      /* Notifications sent by 'Play' Button */
      case ID_PLAY_BUTTON: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {        
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
          {
            if(WavList.ptr > 0)
            {
              _PlayFile((char *)WavList.file[Audio_FilePos].name);
              PlayerSettings.b.play = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);                        
            }
            else
            {
              TEXT_SetTextColor(WM_GetDialogItem(hMainWin, ID_TITLE), GUI_WHITE);
              TEXT_SetFont(WM_GetDialogItem(hMainWin, ID_TITLE), &GUI_FontLubalGraph24B);
              TEXT_SetTextAlign(WM_GetDialogItem(hMainWin, ID_TITLE), TEXT_CF_HCENTER | TEXT_CF_VCENTER);  
              TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), "No Audio file on the SD Card");  
            }
          }
          else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {
            if(PlayerSettings.b.pause == PAUSE_ACTIVE)
            {   
              PlayerSettings.b.pause = PAUSE_INACTIVE;
              PlayerSettings.b.play = PLAY_ACTIVE;
              AUDIOPLAYER_Resume();            
              
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem); 
              
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);   
            }
          }  
        }
        break;
      
      /* Notifications sent by 'pause' Button */
      case ID_PAUSE_BUTTON: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {
            if(PlayerSettings.b.pause == PAUSE_INACTIVE)
            {   
              PlayerSettings.b.pause = PAUSE_ACTIVE;
              PlayerSettings.b.play = PAUSE_INACTIVE;
              AUDIOPLAYER_Pause();
              
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem); 
              
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem); 
            }
          }
        }
        break;   
      
      /* Notifications sent by 'STOP' Button */
      case ID_STOP_BUTTON: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {
            PlayerSettings.b.pause = PAUSE_INACTIVE;
            PlayerSettings.b.play = PLAY_INACTIVE;
            
            AUDIOPLAYER_Stop();
            hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
            TEXT_SetText(hItem, "00:00");            
            
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
            SLIDER_SetValue(hItem, 0);  
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);  
          }
        }
        break;

      /* Notifications sent by 'Next' Button */
      case ID_NEXT_BUTTON: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if( WavList.ptr > 0)
          {        
            if(Audio_FilePos < (WavList.ptr - 1))
            {
              if(ShuffleClicked == 0)
              {
                /* Shuffle part */
                Audio_FilePos = (rand() % (WavList.ptr - 1));
              }
              else
              {
                Audio_FilePos++;
              }
            }
            else if(PlayerSettings.b.repeat == REPEAT_ALL)
            {
              Audio_FilePos = 0; 
            }
            
            if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
            {    
              if(PlayerSettings.b.pause == PAUSE_ACTIVE)
              {  
                PlayerSettings.b.pause = PAUSE_INACTIVE;
                hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
                WM_InvalidateWindow(hItem);
                WM_Update(hItem);
                
                PlayerSettings.b.play = PLAY_ACTIVE;
                hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
                WM_InvalidateWindow(hItem);
                WM_Update(hItem);              
              }
              
              AUDIOPLAYER_Stop();
              _PlayFile((char *)WavList.file[Audio_FilePos].name); 
            }
            else
            {
              FILEMGR_GetFileOnly (tmp, (char *)WavList.file[Audio_FilePos].name);
              
              TEXT_SetTextColor(WM_GetDialogItem(hMainWin, ID_TITLE), GUI_WHITE);
              TEXT_SetFont(WM_GetDialogItem(hMainWin, ID_TITLE), &GUI_FontLubalGraph24B);
              TEXT_SetTextAlign(WM_GetDialogItem(hMainWin, ID_TITLE), TEXT_CF_HCENTER | TEXT_CF_VCENTER);
              TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), tmp);  
            }
          }
        }
        break;
      
      /* Notifications sent by 'Previous' Button */
      case ID_PREVIOUS_BUTTON: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if( WavList.ptr > 0)
          {
            if(Audio_FilePos > 0)
            {
              if(ShuffleClicked == 0)
              {
                /* Shuffle part */
                Audio_FilePos = (rand() % (WavList.ptr - 1));
              }
              else
              {
                Audio_FilePos--;
              }            
            }
            else if(PlayerSettings.b.repeat == REPEAT_ALL)
            {
              Audio_FilePos = (WavList.ptr - 1); 
            }          
            
            if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
            {  
              if(PlayerSettings.b.pause == PAUSE_ACTIVE)
              {  
                PlayerSettings.b.pause = PAUSE_INACTIVE;
                hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
                WM_InvalidateWindow(hItem);
                WM_Update(hItem);
                
                PlayerSettings.b.play = PLAY_ACTIVE;
                hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
                WM_InvalidateWindow(hItem);
                WM_Update(hItem);    
              }
              
              AUDIOPLAYER_Stop();
              _PlayFile((char *)WavList.file[Audio_FilePos].name);              
            }
            else
            {
              FILEMGR_GetFileOnly (tmp, (char *)WavList.file[Audio_FilePos].name);
              
              TEXT_SetTextColor(WM_GetDialogItem(hMainWin, ID_TITLE), GUI_WHITE);
              TEXT_SetFont(WM_GetDialogItem(hMainWin, ID_TITLE), &GUI_FontLubalGraph24B);
              TEXT_SetTextAlign(WM_GetDialogItem(hMainWin, ID_TITLE), TEXT_CF_HCENTER | TEXT_CF_VCENTER);
              TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), tmp);  
            }
          }
        }
        break;
      /* Notification sent by "Button_Close" */  
      case ID_BACK_BUTTON:
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if(hProcessTimer != 0)
            WM_DeleteTimer(hProcessTimer);
          if(hStatusTimer != 0)
            WM_DeleteTimer(hStatusTimer);
          if(hVisEqTimer != 0)
            WM_DeleteTimer(hVisEqTimer);
          hProcessTimer = 0;
          hStatusTimer = 0;
          GUI_EndDialog(pMsg->hWin, 0);

        }
        break;
     case ID_PLAY_LIST_BUTTON:
       if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if(VisEqClicked == 0)
          {
            WM_HideWindow(hMainVisEq);
          }
          WM_ShowWindow(hMainList);
        }
        break;
     case ID_SHUFFLE_BUTTON:
       if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if(ShuffleClicked == 0)
            ShuffleClicked = 1;
          else
            ShuffleClicked = 0;
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
  * @brief  Retrieve the file name from a full file path
  * @param  file: pointer to base path
  * @param  path: pointer to full path
  * @retval None
*/
static void FILEMGR_GetFileOnly (char *file, char *path)
{
  char *baseName1, *baseName2;
  baseName1 = strrchr(path,'/');
  baseName2 = strrchr(path,':');
  
  if(baseName1++) 
  { 
    strcpy(file, baseName1);
  }
  else 
  {
    if (baseName2++) 
    {
      strcpy(file, baseName2);
    }
    else
    {
      strcpy(file, path);
    }
  }
}

/**
  * @brief  Audio player window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{  
  if(module_prop[k_ModuleGetIndex(&audio_player_board)].win_state == 0)
  {    
    /*Initialize audio Interface */
    PlayerSettings.d32 = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_SETTING_BKP);
    /* Prevent Volume 0 */
    if(PlayerSettings.b.volume == 0)
    {
      PlayerSettings.b.volume = DEFAULT_AUDIO_VOLUME;
    } 
    if(WavList.ptr == 0)
    {
      _AddEntireFolder("0:", 0);
      _AddEntireFolder("0:/Audio", 0);
    }
    Audio_FilePos = 0; 
    
    AUDIOPLAYER_Init(PlayerSettings.b.volume);  
    
    visualization_initialize();
    
    hMainWin = 0;
    VisEqClicked = 1;
    ShuffleClicked = 1;
    PlayerSettings.b.repeat = REPEAT_NONE;
    
    hMainWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbMainDialog, hWin, xpos, ypos);    
    
    hMainVisEq = WM_CreateWindowAsChild(160, 76, 320, 190, hMainWin, WM_CF_HIDE, _cbMainVisEqDialog, 0);
    
    hMainList = WM_CreateWindowAsChild(0, 0, 640, 480, hMainWin, WM_CF_HIDE, _cbMainListDialog, 0);
  }
}

/**
  * @}
  */

/**
  * @}
  */

