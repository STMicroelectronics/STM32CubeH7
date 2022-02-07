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
#include "visualization.h"
#include "k_modules_res.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_PLAYER
  * @brief audio player routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmplay_clicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_stop;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton_pause;
extern GUI_CONST_STORAGE GUI_BITMAP bmBkc_list;
extern GUI_CONST_STORAGE GUI_BITMAP bmCover_Bkc;
extern GUI_CONST_STORAGE GUI_BITMAP bmBarHaut;
extern GUI_CONST_STORAGE GUI_BITMAP bmNext;
extern GUI_CONST_STORAGE GUI_BITMAP bmPlay;
extern GUI_CONST_STORAGE GUI_BITMAP bmPlaylist;
extern GUI_CONST_STORAGE GUI_BITMAP bmPlaylistButton;
extern GUI_CONST_STORAGE GUI_BITMAP bmPlaylistMenu;
extern GUI_CONST_STORAGE GUI_BITMAP bmRepeatClicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmRepeatUnclicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmShuffleClicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmShuffleUnclicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmVisualSelected;
extern GUI_CONST_STORAGE GUI_BITMAP bmVisualUnselected;
extern GUI_CONST_STORAGE GUI_BITMAP bmPrevious;

/* Private function prototypes -----------------------------------------------*/
static WM_HWIN hWinAudioPlayer;
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void FILEMGR_GetFileOnly (char *file, char *path);
static void _PlayFile(char *filename);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_player_board =
{
  MODULE_AUDIO_PLAYER,
  "Audio Player",
  &hWinAudioPlayer,
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

#if defined ( __GNUC__ )
__attribute__((section(".viz_bitmap_buffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="viz_bitmap_buffer"
#elif defined( __CC_ARM )
__attribute__((section(".viz_bitmap_buffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static uint8_t viz_bitmap[2 * VIZ_WIDTH * VIZ_HEIGHT];

/* Buffer that will contain the visualisation effects */
static const GUI_BITMAP pBMVisual = {
VIZ_WIDTH,                      /* xSize                        */
VIZ_HEIGHT,                     /* ySize                        */
(VIZ_WIDTH * 2),                /* BytesPerLine                 */
(8 * 2),                        /* BitsPerPixel                 */
(unsigned char *)viz_bitmap,    /* Pointer to picture data      */
NULL,                           /* Pointer to palette           */
GUI_DRAW_BMPM565
};

uint8_t visualization_samples[64];

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
#define ID_IMAGE_VIZ              (GUI_ID_USER + 0x13)

/* Polygon to fill on the main window */
const GUI_POINT aPoints[] = {
{ 0, 192},
{ 479, 192},
{ 479, 271},
{ 0, 271}
};

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect,    "Audio Player",   ID_FRAMEWIN_INFO,         0,   0, 480, 272, WM_CF_HASTRANS, 0x0, 0 },
  { TEXT_CreateIndirect,      "",               ID_TITLE,                 0, 155, 480,  40, (TEXT_CF_HCENTER | TEXT_CF_VCENTER), 0x0, 0 },
  { TEXT_CreateIndirect,      "",               ID_ELAPSED_TIME,         95, 252,  30,  22, (TEXT_CF_HCENTER | TEXT_CF_VCENTER), 0x0, 0 },
  { TEXT_CreateIndirect,      "",               ID_TOTAL_TIME,          355, 252,  30,  22, (TEXT_CF_HCENTER | TEXT_CF_VCENTER), 0x0, 0 },
};

static WM_HWIN hMainList; 
static WM_HWIN hMainVisEq; 
static WM_HTIMER hProcessTimer;
static WM_HTIMER hStatusTimer;
static WM_HTIMER hVisEqTimer;
static FILELIST_FileTypeDef  WavList;
static uint16_t              Audio_FilePos = 0; 
static AudioSettingsTypeDef  PlayerSettings;

static uint8_t AudioPlayerInitFailed = 0;
static uint8_t PlayerSeekInProgress  = 0;

/* Default storage ID is USB */
static STORAGE_IdTypeDef storage_id = USB_DISK_UNIT;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Set Playback Info Visibility
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @param  Visible: requested visibility value (0: hidden | 1: visible)
  * @retval None
  */
static void _SetPlaybackInfoVisibility(WM_HWIN hParentWin, uint8_t Visible)
{
  WM_HWIN hItem;

  /* Slider */
  hItem = WM_GetDialogItem(hParentWin, ID_SLIDER_DURATION);

  if(WM_IsVisible(hItem) != Visible) /* Update Visibility */
  {
    if(Visible)
    {
      /* Slider */
      WM_ShowWindow(hItem);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);

      /* Duration Text */
      hItem = WM_GetDialogItem(hParentWin, ID_TOTAL_TIME);
      WM_ShowWindow(hItem);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);

      /* Elapsed Time Text */
      hItem = WM_GetDialogItem(hParentWin, ID_ELAPSED_TIME);
      WM_ShowWindow(hItem);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);
    }
    else
    {
      /* Slider */
      WM_HideWindow(hItem);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);

      /* Duration Text */
      hItem = WM_GetDialogItem(hParentWin, ID_TOTAL_TIME);
      WM_HideWindow(hItem);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);

      /* Elapsed Time Text */
      hItem = WM_GetDialogItem(hParentWin, ID_ELAPSED_TIME);
      WM_HideWindow(hItem);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);
    }
  }
}

/**
  * @brief  callback for audio post processing
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void AudioPlayer_PostProcess(uint8_t* pBuff, uint32_t Size)
{
  if(VisEqClicked == 0)
  {
    int16_t *samples;
    int i, sample;

    samples = (int16_t *)pBuff;
    for (i = 0; i < 64; i++) 
    {
      sample = ((*samples + 32768) >> 9); /* 0, 128 */
      visualization_samples[i] = (uint8_t)sample;
      samples += 2;
    }
  }
}

/**
  * @brief  callback for audio process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioProcess(WM_MESSAGE * pMsg) 
{
  uint32_t Id;  
  WM_HWIN hItem;
  char tmp[16];
  uint32_t progress;
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
        if(AUDIOPLAYER_GetState() == AUDIO_STATE_PLAY)
        {
          if(!PlayerSeekInProgress)
          {
            /* Get the total stream duration and elapsed duration */
            progress = AUDIOPLAYER_GetProgress();

            sprintf((char *)tmp , "%02lu:%02lu", progress/60, progress%60 );
            hItem = WM_GetDialogItem(hWinAudioPlayer, ID_ELAPSED_TIME);
            TEXT_SetText(hItem, tmp);

            /* Set progress slider position */
            hItem = WM_GetDialogItem(hWinAudioPlayer, ID_SLIDER_DURATION);
            SLIDER_SetValue(hItem, progress);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);
          }
        }
        else if(AUDIOPLAYER_GetState() == AUDIO_STATE_EOF)
        {
          /* Get the total stream duration and elapsed duration */
          duration = AUDIOPLAYER_GetDuration();

          sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
          hItem = WM_GetDialogItem(hWinAudioPlayer, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, tmp);

          /* Set progress slider position */
          hItem = WM_GetDialogItem(hWinAudioPlayer, ID_SLIDER_DURATION);
          SLIDER_SetValue(hItem, duration);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
        else if(AUDIOPLAYER_GetState() == AUDIO_STATE_ERROR) /* Something went wrong? */
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
              
            AUDIOPLAYER_Stop();
            _PlayFile((char *)WavList.file[Audio_FilePos].name); 
          }
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
      
      if (WavList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(((strstr(fn, ".wav")) || (strstr(fn, ".WAV")))
#ifdef __MP3_DECODER__
          || ((strstr(fn, ".mp3")) || (strstr(fn, ".MP3")))
#endif /* __MP3_DECODER__ */
#ifdef __AVI_DECODER__
          || ((strstr(fn, ".avi")) || (strstr(fn, ".AVI")))
#endif /* __AVI_DECODER__ */
            )
          {
            AUDIO_FILE_InfoTypeDef AudioInfo = { 0 }; /* Make sure that file format is supported! */            
            strcpy(tmp, folder);
            strcat(tmp, "/");
            strcat(tmp, fn);

            if(AUDIOPLAYER_GetFileInfo(tmp, &AudioInfo) == AUDIO_ERROR_NONE)
            {
              strncpy((char *)WavList.file[WavList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
              WavList.ptr++;

              if((update == 1) && (hWinAudioPlayer != 0))
              {
                if(AUDIOPLAYER_SelectFile(tmp) == AUDIO_ERROR_NONE)
                {
                  FILEMGR_GetFileOnly (tmp, fn);
                }
              }
            }
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
  
  if(AUDIOPLAYER_GetState() == AUDIO_STATE_STOP)
  {
    AUDIO_FILE_InfoTypeDef AudioInfo = { 0 };
    if(AUDIOPLAYER_GetFileInfo(filename, &AudioInfo) == AUDIO_ERROR_NONE)
    {
      if(AUDIOPLAYER_SelectFile(filename) == AUDIO_ERROR_NONE)
      {
        /* Start audio playback */
        if(AUDIOPLAYER_Play(AudioInfo.SampleRate) == AUDIO_ERROR_NONE)
        {
          /* Title */
          FILEMGR_GetFileOnly (tmp, filename);
          TEXT_SetTextColor(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), GUI_WHITE);
          TEXT_SetFont(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), &GUI_FontArial18);
          TEXT_SetText(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), tmp);

          /* Total Time */
          duration = AUDIOPLAYER_GetDuration();
          sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
          hItem = WM_GetDialogItem(hWinAudioPlayer, ID_TOTAL_TIME);
          TEXT_SetText(hItem, tmp);

          hItem = WM_GetDialogItem(hWinAudioPlayer, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, "00:00");

          /* Set duration slider range */
          hItem = WM_GetDialogItem(hWinAudioPlayer, ID_SLIDER_DURATION);
          SLIDER_SetRange(hItem, 0, duration);

          _SetPlaybackInfoVisibility(hWinAudioPlayer, 1);

          AUDIOPLAYER_Mute(PlayerSettings.b.mute);
        }
      }
    }
    else
    {
      TEXT_SetText(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), "Unknown");
      hItem = WM_GetDialogItem(hWinAudioPlayer, ID_TOTAL_TIME);
      TEXT_SetText(hItem, "--:--");

      _SetPlaybackInfoVisibility(hWinAudioPlayer, 0);
    }  
  }
}

/**
  * @brief  Notify the end of audio file.
  * @param  None.
  * @retval Audio state.
  */
void  AUDIOPLAYER_NotifyEndOfFile(void)
{
  uint32_t duration;
  static char duration_string[FILEMGR_FILE_NAME_SIZE];
  WM_HWIN hItem;

  AUDIOPLAYER_Stop();

  /* Restart the Audio Process Update Timer */
  if(hProcessTimer != 0)
  {
    WM_RestartTimer(hProcessTimer, 1000);
  }

  /* Get the total stream duration and elapsed duration */
  duration = AUDIOPLAYER_GetDuration();

  sprintf((char *)duration_string , "%02lu:%02lu", duration/60, duration%60 );
  hItem = WM_GetDialogItem(hWinAudioPlayer, ID_ELAPSED_TIME);
  TEXT_SetText(hItem, duration_string);

  /* Set progress slider position */
  hItem = WM_GetDialogItem(hWinAudioPlayer, ID_SLIDER_DURATION);
  SLIDER_SetValue(hItem, duration);
  WM_InvalidateWindow(hItem);
  WM_Update(hItem);

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
    hItem = WM_GetDialogItem(hWinAudioPlayer, ID_PLAY_BUTTON);
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);
  }
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
      prev_msd_status = k_StorageGetStatus(storage_id);
      hStatusTimer = WM_CreateTimer(pMsg->hWin, 0, 100, 0);
      break;
    
  case WM_TIMER:
    if(prev_msd_status != k_StorageGetStatus(storage_id))
    {
      prev_msd_status = k_StorageGetStatus(storage_id);
      if(prev_msd_status == 0)
      {
        PlayerSettings.b.play = PLAY_INACTIVE;
        PlayerSettings.b.pause = PAUSE_INACTIVE;
        
        WavList.ptr = 0;
        Audio_FilePos = 0;
        
        AUDIOPLAYER_Stop();
        GUI_EndDialog(hWinAudioPlayer, 0);
      }
    }
    if(hStatusTimer != 0)
    {
      WM_RestartTimer(hStatusTimer, 100);
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
  GUI_DrawBitmap(&bmPlaylistMenu, 0, 0);
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
  * @brief  callback for wheel owner draw
  * @param  pDrawItemInfo: pointer to the draw infos needed
  * @retval None
  */
static int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) 
{
  WM_HWIN   hWin;
  int       xSizeWin;
  int       ySizeWin;

  hWin = pDrawItemInfo->hWin;
  
  /* Process messages */
  switch (pDrawItemInfo->Cmd) 
  {
    case WIDGET_ITEM_GET_XSIZE:
      /* Return x-size of item */
      return LISTVIEW_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_GET_YSIZE:
      /* Return y-size of item */
      return LISTVIEW_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW:
      /* Draw item */
      return LISTVIEW_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW_BACKGROUND:
      xSizeWin = WM_GetWindowSizeX(hWin);
      ySizeWin = WM_GetWindowSizeY(hWin);
      
      /* Draw background */
      GUI_SetColor(GUI_TRANSPARENT);
      break;
    case WIDGET_ITEM_DRAW_OVERLAY:
      xSizeWin = WM_GetWindowSizeX(hWin);
      ySizeWin = WM_GetWindowSizeY(hWin);
      
      GUI_EnableAlpha(1);
      
      GUI_SetColor(GUI_TRANSPARENT);
      GUI_FillRect(0, 0, xSizeWin - 1, ySizeWin - 1);
     
      break;
  }
  return 0;
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
  uint32_t index;
  static char tmp[40];
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
   
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:  
      hItem = BUTTON_CreateEx(445, 15, 55, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_LIST_BACK_BUTTON);
      WM_SetCallback(hItem, _cbButton_play_listback_button); 
    
      /* Initialization of 'Listview' */
      hItem = LISTVIEW_CreateEx(187, 56, 465, 300, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_WAVFILE_LIST);
      LISTVIEW_SetFont(hItem, &GUI_FontArial18);
      LISTVIEW_AddColumn(hItem, 360, "Track", GUI_TA_VCENTER | GUI_TA_LEFT);
      LISTVIEW_AddColumn(hItem, 100, "Duration   ", GUI_TA_LEFT);
      LISTVIEW_SetGridVis(hItem, 0);
      LISTVIEW_SetAutoScrollV(hItem, 1);
      LISTVIEW_SetHeaderHeight(hItem, 0);    
      LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, 0xFF000000);
      LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL, 0x0039A9DC); /* GUI_STCOLOR_LIGHTBLUE */
      LISTVIEW_SetFont(hItem, &GUI_FontArial18);
      LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL, 0x0039A9DC);  /* GUI_STCOLOR_LIGHTBLUE */
      LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SEL, GUI_WHITE);
      LISTVIEW_SetRowHeight(hItem, 30);
      LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, 0x0039A9DC);  /* GUI_STCOLOR_LIGHTBLUE */

      SCROLLBAR_SetDefaultWidth(20);
        
      LISTVIEW_SetOwnerDraw(hItem, _OwnerDraw);
      WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
      
      ItemNbr = 0;

      if(WavList.ptr > 0)
      {
        while(ItemNbr < WavList.ptr)
        {
          AUDIO_FILE_InfoTypeDef AudioInfo = { 0 };
          if(AUDIOPLAYER_GetFileInfo((char *)WavList.file[ItemNbr].name, &AudioInfo) == AUDIO_ERROR_NONE)
          {
            LISTVIEW_AddRow(hItem, NULL);
            FILEMGR_GetFileOnly (tmp, (char *)WavList.file[ItemNbr].name);
            LISTVIEW_SetItemText(hItem, 0,ItemNbr, tmp);

            sprintf((char *)tmp , "%02lu:%02lu", AudioInfo.Duration/60, AudioInfo.Duration%60 );
            LISTVIEW_SetItemText(hItem, 1, ItemNbr, tmp);
            LISTVIEW_SetItemTextColor(hItem, 1, ItemNbr, LISTVIEW_CI_UNSEL, 0x0039A9DC); /* GUI_STCOLOR_DARKBLUE */
            LISTVIEW_SetItemTextColor(hItem, 0, ItemNbr, LISTVIEW_CI_UNSEL, 0x0039A9DC); /* GUI_STCOLOR_DARKBLUE */
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
        if(AudioPlayerInitFailed)
        {
          TEXT_SetText(hItem, "Audio Player Initialization Failed!");
        }
        else
        {
          TEXT_SetText(hItem, "No Audio file on the selected media storage");
        }
        WM_InvalidateWindow(pMsg->hWin);
        WM_PaintWindowAndDescs(pMsg->hWin);
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
                    
                    if(AUDIOPLAYER_GetState() == AUDIO_STATE_PLAY)
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
                    LISTVIEW_SetItemTextColor(hItem ,0, Audio_FilePos, LISTVIEW_CI_UNSEL, 0x00002052);    /* GUI_STCOLOR_DARKBLUE */        
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
  WM_HWIN hItem;
    
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      memset((void*)pBMVisual.pData, 0x00, VIZ_WIDTH * VIZ_HEIGHT * 2);

      IMAGE_CreateEx(0,  0, VIZ_WIDTH, VIZ_HEIGHT, pMsg->hWin, WM_CF_SHOW, 0, ID_IMAGE_VIZ);
      break;

    case WM_PAINT:
      if(VisEqClicked == 0)
      {
        visualization_copy_framebuffer((void*)pBMVisual.pData, (VIZ_WIDTH), 0, 0, VIZ_WIDTH, VIZ_HEIGHT);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIZ);
        IMAGE_SetBitmap(hItem, &pBMVisual);
      }
      break;

    case WM_TIMER:
      NCode = pMsg->Data.v;
      Id = WM_GetTimerId(pMsg->Data.v);
      if (Id == ID_WiIN_BKC_PROCESS_TIMER)
      {
        visualization_render(visualization_samples, 64);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_VIZ);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
      }
      if(NCode != 0)
      {
        WM_RestartTimer(pMsg->Data.v, 83);
      }
      break;
      
    case WM_DELETE:
      if(hVisEqTimer != 0)
      {
        WM_DeleteTimer(hVisEqTimer); 
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
  GUI_SetFont(GUI_FONT_13B_1);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_DispStringAt("PLAYLIST", 10, 5);
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
    GUI_DrawBitmap(&bmVisualUnselected, 9, 9);
  }
  else
  {
    GUI_DrawBitmap(&bmVisualSelected, 9, 9);
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
  GUI_DrawBitmap(&bmPlaylist, 10, 10);
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
  GUI_PID_STATE * pState;
  static char tmp[40]; 

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:    
    /* Then add all the needed buttons */
    /* Playlist */
    hItem = BUTTON_CreateEx(203, 10, 73, 26, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_LIST_BUTTON);
    WM_SetCallback(hItem, _cbButton_play_list_button);  
    /* Shuffle */
    hItem = BUTTON_CreateEx(165, 14, 22, 23, pMsg->hWin, WM_CF_SHOW, 0, ID_SHUFFLE_BUTTON);
    WM_SetCallback(hItem, _cbButton_shuffle_button);    
    /* Repeat */
    hItem = BUTTON_CreateEx(300, 14, 24, 22, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat_button);
    /* Play */
    hItem = BUTTON_CreateEx(210, 200, 63, 63, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_BUTTON);
    WM_SetCallback(hItem, _cbButton_play_button);
    /* Previous */
    hItem = BUTTON_CreateEx(115, 215, 35, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_PREVIOUS_BUTTON);
    WM_SetCallback(hItem, _cbButton_previous_button);
    /* Next */
    hItem = BUTTON_CreateEx(334, 215, 35, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_NEXT_BUTTON);
    WM_SetCallback(hItem, _cbButton_next_button);
    /* Stop Button */
    hItem = BUTTON_CreateEx(279, 215, 34, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_STOP_BUTTON);
    WM_SetCallback(hItem, _cbButton_stop_button);
    /* Pause Button */
    hItem = BUTTON_CreateEx(170, 215, 34, 34, pMsg->hWin, WM_CF_SHOW, 0, ID_PAUSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_pause_button);
    /* Visual equaliser */
    hItem = BUTTON_CreateEx(440, 217, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_VISUALEQ_BUTTON);
    WM_SetCallback(hItem, _cbButton_visualeq_button);
    /* Back Button */
    hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BACK_BUTTON);
    WM_SetCallback(hItem, _cbButton_back_button);
    /* Slider Duration */
    hItem = SLIDER_CreateEx(130, 260, 220, 8, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_DURATION);
    SLIDER_SetValue(hItem, 0);
    SLIDER_SetWidth(hItem, 0);
    SLIDER_SetSTSkin(hItem);

    /* Volume slider */
    hItem = SLIDER_CreateEx(0, 191, 480, 8, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_VOLUME);
    SLIDER_SetValue(hItem, PlayerSettings.b.volume);
    SLIDER_SetWidth(hItem, 0);
    SLIDER_SetSTSkin(hItem);
    
    WM_CreateWindowAsChild(479,
                           271,
                           1,
                           1,
                           pMsg->hWin, 
                           WM_CF_SHOW,
                           _cbAudioProcess, 
                           0);
    
    /* Media connection */
    WM_CreateWindowAsChild(479, 271, 1, 1, pMsg->hWin, WM_CF_SHOW, _cbMediaConnection , 0);
    
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
    GUI_DrawBitmap(&bmBarHaut, 0, 157);
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
    k_BkupSaveParameter(AUDIO_PLAYER_BKP_REG, PlayerSettings.d32);    

    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;

    switch(Id) 
    {
      case ID_VISUALEQ_BUTTON:
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if (WM_IsVisible(hMainVisEq))
          {
            WM_HideWindow(hMainVisEq);
            VisEqClicked = 1;

            if(hVisEqTimer != 0)
            {
              WM_DeleteTimer(hVisEqTimer);
              hVisEqTimer = 0;
            }
          }
          else
          {
            WM_ShowWindow(hMainVisEq);
            VisEqClicked = 0;

            if(hVisEqTimer != 0)
            {
              WM_DeleteTimer(hVisEqTimer);
              hVisEqTimer = 0;
            }
            hVisEqTimer = WM_CreateTimer(hMainVisEq, ID_WiIN_BKC_PROCESS_TIMER, 83, 0);
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
            AUDIOPLAYER_SeekToTime(NewTime);

            PlayerSeekInProgress = 0;
          }
        }
        break;      
      /* Notifications sent by 'Play' Button */
      case ID_PLAY_BUTTON: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {        
          if(AUDIOPLAYER_GetState() == AUDIO_STATE_STOP)
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
              TEXT_SetTextColor(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), GUI_WHITE);
              TEXT_SetFont(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), &GUI_FontArial18);
              TEXT_SetText(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), "No Audio file on the selected media storage");
              WM_InvalidateWindow(hWinAudioPlayer);
              WM_PaintWindowAndDescs(hWinAudioPlayer);
            }
            _SetPlaybackInfoVisibility(pMsg->hWin, 1);
          }
          else if(AUDIOPLAYER_GetState() == AUDIO_STATE_PAUSE)
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
          else if(AUDIOPLAYER_GetState() == AUDIO_STATE_ERROR)
          {
            if(AudioPlayerInitFailed == 1)
            {
              TEXT_SetTextColor(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), GUI_WHITE);
              TEXT_SetFont(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), &GUI_FontArial18);
              TEXT_SetText(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), "Audio Player Initialization Failed");
              WM_InvalidateWindow(hWinAudioPlayer);
              WM_PaintWindowAndDescs(hWinAudioPlayer);
            }
          }
        }
        break;
      
      /* Notifications sent by 'pause' Button */
      case ID_PAUSE_BUTTON: 
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if(AUDIOPLAYER_GetState() == AUDIO_STATE_PLAY)
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
          if(AUDIOPLAYER_GetState() == AUDIO_STATE_PLAY)
          {
            PlayerSettings.b.pause = PAUSE_INACTIVE;
            PlayerSettings.b.play = PLAY_INACTIVE;
            
            AUDIOPLAYER_Stop();

            _SetPlaybackInfoVisibility(pMsg->hWin, 0);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);  
          }
        }
        break;

      /* Notifications sent by 'Next' or 'Previous' Buttons */
      case ID_NEXT_BUTTON:
      case ID_PREVIOUS_BUTTON:
        if(NCode == WM_NOTIFICATION_CLICKED)
        {
          if( WavList.ptr > 0)
          {
            if(ShuffleClicked == 0)
            {
              /* Shuffle part */
              Audio_FilePos = (rand() % (WavList.ptr - 1));
            }
            else
            {
              if(Id == ID_NEXT_BUTTON)
              {
                if(Audio_FilePos < (WavList.ptr - 1))
                {
                  Audio_FilePos++;
                }
                else if(PlayerSettings.b.repeat == REPEAT_ALL)
                {
                  Audio_FilePos = 0;
                }
              }
              else /* ID_PREVIOUS_BUTTON */
              {
                if(Audio_FilePos > 0)
                {
                  Audio_FilePos--;
                }
                else if(PlayerSettings.b.repeat == REPEAT_ALL)
                {
                  Audio_FilePos = (WavList.ptr - 1);
                }
              }
            }

            AUDIO_StateTypeDef CurrentAudioState = AUDIOPLAYER_GetState();

            if( (CurrentAudioState == AUDIO_STATE_PLAY) || (CurrentAudioState == AUDIO_STATE_PAUSE) )
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
              
              TEXT_SetTextColor(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), GUI_WHITE);
              TEXT_SetFont(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), &GUI_FontArial18);
              TEXT_SetText(WM_GetDialogItem(hWinAudioPlayer, ID_TITLE), tmp);
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
          hVisEqTimer = 0;
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

    case WM_TOUCH_CHILD:
      Id = WM_GetId(pMsg->hWinSrc);
      switch (Id) {
        case ID_IMAGE_VIZ:
          pState = (GUI_PID_STATE *)((WM_MESSAGE *)pMsg->Data.p)->Data.p;
          if (pState) {
            if (pState->Pressed) {
              visualization_next();
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
    PlayerSettings.d32 = k_BkupRestoreParameter(AUDIO_PLAYER_BKP_REG);
    /* Prevent Volume 0 */
    if(PlayerSettings.b.volume == 0)
    {
      PlayerSettings.b.volume = AUDIO_PLAYER_DEFAULT_VOLUME;
    }

    AudioPlayerInitFailed = 1;
    if(AUDIOPLAYER_Init(PlayerSettings.b.volume) == AUDIO_ERROR_NONE)
    {
      AUDIOPLAYER_RegisterCallbacks(NULL, AudioPlayer_PostProcess , AUDIOPLAYER_NotifyEndOfFile);
      if(WavList.ptr == 0)
      {
        _AddEntireFolder(".", 0);
        _AddEntireFolder("Audio", 0);
#ifdef __MP3_DECODER__
        _AddEntireFolder("Mp3", 0);
#endif /* __MP3_DECODER__ */
#ifdef __AVI_DECODER__
        _AddEntireFolder("Video", 0);
#endif /* __AVI_DECODER__ */
      }
      Audio_FilePos = 0;

      for (int i = 0; i < 64; i++)
      {
        visualization_samples[i] = 64;
      }

    visualization_initialize();
    }
    else
    {
      AudioPlayerInitFailed = 1;
    }

    hWinAudioPlayer = 0;
    VisEqClicked = 1;
    ShuffleClicked = 1;
    PlayerSettings.b.repeat = REPEAT_NONE;
    
    hWinAudioPlayer = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbMainDialog, hWin, xpos, ypos);

    hMainList = WM_CreateWindowAsChild(0, 0, 480, 272, hWinAudioPlayer, WM_CF_HIDE, _cbMainListDialog, 0);

    hMainVisEq = WM_CreateWindowAsChild(140, 45, VIZ_WIDTH, VIZ_HEIGHT, hWinAudioPlayer, (WM_CF_HIDE|WM_CF_FGND|WM_CF_STAYONTOP), _cbMainVisEqDialog, 0);
  }
}

/**
  * @}
  */

/**
  * @}
  */

