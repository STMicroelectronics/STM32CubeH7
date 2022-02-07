/**
  ******************************************************************************
  * @file    Audio/Audio_playback_and_record/Src/waveplayer.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio Out (playback) interface API
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
#include "waveplayer.h"

/* Private define ------------------------------------------------------------*/
#define TOUCH_NEXT_XMIN         325
#define TOUCH_NEXT_XMAX         365
#define TOUCH_NEXT_YMIN         212
#define TOUCH_NEXT_YMAX         252

#define TOUCH_PREVIOUS_XMIN     250
#define TOUCH_PREVIOUS_XMAX     290
#define TOUCH_PREVIOUS_YMIN     212
#define TOUCH_PREVIOUS_YMAX     252

#define TOUCH_STOP_XMIN         170
#define TOUCH_STOP_XMAX         210
#define TOUCH_STOP_YMIN         212
#define TOUCH_STOP_YMAX         252

#define TOUCH_PAUSE_XMIN        100
#define TOUCH_PAUSE_XMAX        124
#define TOUCH_PAUSE_YMIN        212
#define TOUCH_PAUSE_YMAX        252

#define TOUCH_VOL_MINUS_XMIN    20
#define TOUCH_VOL_MINUS_XMAX    70
#define TOUCH_VOL_MINUS_YMIN    212
#define TOUCH_VOL_MINUS_YMAX    252

#define TOUCH_VOL_PLUS_XMIN     402
#define TOUCH_VOL_PLUS_XMAX     452
#define TOUCH_VOL_PLUS_YMIN     212
#define TOUCH_VOL_PLUS_YMAX     252

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer location should aligned to cache line size (32 bytes) */
ALIGN_32BYTES (static AUDIO_OUT_BufferTypeDef  BufferCtl);
static int16_t FilePos = 0;
static __IO uint32_t uwVolume = 70;

BSP_AUDIO_Init_t  AudioOutInit;
BSP_AUDIO_Init_t  AudioInInit;

static Point NextPoints[] = {{TOUCH_NEXT_XMIN, TOUCH_NEXT_YMIN},
                             {TOUCH_NEXT_XMAX, (TOUCH_NEXT_YMIN+TOUCH_NEXT_YMAX)/2},
                             {TOUCH_NEXT_XMIN, TOUCH_NEXT_YMAX}};
static Point PreviousPoints[] = {{TOUCH_PREVIOUS_XMIN, (TOUCH_PREVIOUS_YMIN+TOUCH_PREVIOUS_YMAX)/2},
                                 {TOUCH_PREVIOUS_XMAX, TOUCH_PREVIOUS_YMIN},
                                 {TOUCH_PREVIOUS_XMAX, TOUCH_PREVIOUS_YMAX}};

WAVE_FormatTypeDef WaveFormat;
FIL WavFile;
extern FILELIST_FileTypeDef FileList;

/* Private function prototypes -----------------------------------------------*/
static AUDIO_ErrorTypeDef GetFileInfo(uint16_t file_idx, WAVE_FormatTypeDef *info);
static uint8_t PlayerInit(uint32_t AudioFreq);
static void AUDIO_PlaybackDisplayButtons(void);
static void AUDIO_AcquireTouchButtons(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes Audio Interface.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Init(void)
{
//  (AUDIO_OUT_DEVICE_AUTO, uwVolume, AUDIO_FREQUENCY_44K
   AudioOutInit.Device = AUDIO_OUT_DEVICE_AUTO;
  AudioOutInit.ChannelsNbr = 2;
  AudioOutInit.SampleRate = AUDIO_FREQUENCY_44K;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = uwVolume;
  if(BSP_AUDIO_OUT_Init(0, &AudioOutInit) == 0)
  {
    return AUDIO_ERROR_NONE;
  }
  else
  {
    return AUDIO_ERROR_IO;
  }
}

/**
  * @brief  Starts Audio streaming.
  * @param  idx: File index
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Start(uint8_t idx)
{
  uint32_t bytesread;

  f_close(&WavFile);
  if(AUDIO_GetWavObjectNumber() > idx)
  {
    GetFileInfo(idx, &WaveFormat);

    /*Adjust the Audio frequency */
    PlayerInit(WaveFormat.SampleRate);

    BufferCtl.state = BUFFER_OFFSET_NONE;

    /* Get Data from USB Flash Disk */
    f_lseek(&WavFile, 0);

    /* Fill whole buffer at first time */
    if(f_read(&WavFile,
              &BufferCtl.buff[0],
              AUDIO_OUT_BUFFER_SIZE,
              (void *)&bytesread) == FR_OK)
    {
      AudioState = AUDIO_STATE_PLAY;
      AUDIO_PlaybackDisplayButtons();
      UTIL_LCD_DisplayStringAt(250, LINE(9), (uint8_t *)"  [PLAY ]", LEFT_MODE);
      {
        if(bytesread != 0)
        {
          /* Clean Data Cache to update the content of the SRAM */
          SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);

          BSP_AUDIO_OUT_Play(0,(uint8_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);
          BufferCtl.fptr = bytesread;
          return AUDIO_ERROR_NONE;
        }
      }
    }
  }
  return AUDIO_ERROR_IO;
}

/**
  * @brief  Manages Audio process.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Process(void)
{
  uint32_t bytesread, elapsed_time;
  AUDIO_ErrorTypeDef audio_error = AUDIO_ERROR_NONE;
  static uint32_t prev_elapsed_time = 0xFFFFFFFF;
  uint8_t str[32];

  switch(AudioState)
  {
  case AUDIO_STATE_PLAY:
    if(BufferCtl.fptr >= WaveFormat.FileSize)
    {
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
      AudioState = AUDIO_STATE_NEXT;
    }

    if(BufferCtl.state == BUFFER_OFFSET_HALF)
    {
      if(f_read(&WavFile,
                &BufferCtl.buff[0],
                AUDIO_OUT_BUFFER_SIZE/2,
                (void *)&bytesread) != FR_OK)
      {
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        return AUDIO_ERROR_IO;
      }

      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE/2);

      BufferCtl.state = BUFFER_OFFSET_NONE;
      BufferCtl.fptr += bytesread;
    }

    if(BufferCtl.state == BUFFER_OFFSET_FULL)
    {
      if(f_read(&WavFile,
                &BufferCtl.buff[AUDIO_OUT_BUFFER_SIZE /2],
                AUDIO_OUT_BUFFER_SIZE/2,
                (void *)&bytesread) != FR_OK)
      {
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        return AUDIO_ERROR_IO;
      }

      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[AUDIO_OUT_BUFFER_SIZE/2], AUDIO_OUT_BUFFER_SIZE/2);

      BufferCtl.state = BUFFER_OFFSET_NONE;
      BufferCtl.fptr += bytesread;
    }

    /* Display elapsed time */
    elapsed_time = BufferCtl.fptr / WaveFormat.ByteRate;
    if(prev_elapsed_time != elapsed_time)
    {
      prev_elapsed_time = elapsed_time;
      sprintf((char *)str, "[%02d:%02d]", (int)(elapsed_time /60), (int)(elapsed_time%60));
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_CYAN);
      UTIL_LCD_DisplayStringAt(263, LINE(8), str, LEFT_MODE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    }

    /* Update audio state machine according to touch acquisition */
    AUDIO_AcquireTouchButtons();
    break;

  case AUDIO_STATE_STOP:
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_FillRect(TOUCH_STOP_XMIN, TOUCH_STOP_YMIN , /* Stop rectangle */
                     TOUCH_STOP_XMAX - TOUCH_STOP_XMIN,
                     TOUCH_STOP_YMAX - TOUCH_STOP_YMIN,UTIL_LCD_COLOR_RED);
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    BSP_AUDIO_OUT_DeInit(0);
    AudioState = AUDIO_STATE_IDLE;
    audio_error = AUDIO_ERROR_IO;
    break;

  case AUDIO_STATE_NEXT:
    if(++FilePos >= AUDIO_GetWavObjectNumber())
    {
      FilePos = 0;
    }
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    AUDIO_PLAYER_Start(FilePos);
    if(uwVolume == 0)
    {
      BSP_AUDIO_OUT_SetVolume(0,uwVolume);
    }
    break;

  case AUDIO_STATE_PREVIOUS:
    if(--FilePos < 0)
    {
      FilePos = AUDIO_GetWavObjectNumber() - 1;
    }
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    AUDIO_PLAYER_Start(FilePos);
    if(uwVolume == 0)
    {
      BSP_AUDIO_OUT_SetVolume(0,uwVolume);
    }
    break;

  case AUDIO_STATE_PAUSE:
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_CYAN);
    UTIL_LCD_DisplayStringAt(250, LINE(9), (uint8_t *)"  [PAUSE]", LEFT_MODE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);    /* Display red pause rectangles */
    UTIL_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN,UTIL_LCD_COLOR_RED);
    UTIL_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN,UTIL_LCD_COLOR_RED);
    BSP_AUDIO_OUT_Pause(0);
    AudioState = AUDIO_STATE_WAIT;
    break;

  case AUDIO_STATE_RESUME:
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_CYAN);
    UTIL_LCD_DisplayStringAt(250, LINE(9), (uint8_t *)"  [PLAY ]", LEFT_MODE);
                                            /* Display blue cyan pause rectangles */
    UTIL_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN,UTIL_LCD_COLOR_CYAN);
    UTIL_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN,UTIL_LCD_COLOR_CYAN);
    BSP_AUDIO_OUT_Resume(0);
    if(uwVolume == 0)
    {
      BSP_AUDIO_OUT_SetVolume(0,uwVolume);
    }
    AudioState = AUDIO_STATE_PLAY;
    break;

  case AUDIO_STATE_VOLUME_UP:
    if( uwVolume <= 90)
    {
      uwVolume += 10;
    }
    BSP_AUDIO_OUT_SetVolume(0,uwVolume);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    sprintf((char *)str,  "Volume : %lu ", uwVolume);
    UTIL_LCD_DisplayStringAtLine(9, str);
    AudioState = AUDIO_STATE_PLAY;
    break;

  case AUDIO_STATE_VOLUME_DOWN:
    if( uwVolume >= 10)
    {
      uwVolume -= 10;
    }
    BSP_AUDIO_OUT_SetVolume(0,uwVolume);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    sprintf((char *)str,  "Volume : %lu ", uwVolume);
    UTIL_LCD_DisplayStringAtLine(9, str);
    AudioState = AUDIO_STATE_PLAY;
    break;

  case AUDIO_STATE_WAIT:
  case AUDIO_STATE_IDLE:
  case AUDIO_STATE_INIT:
  default:
    /* Update audio state machine according to touch acquisition */
    AUDIO_AcquireTouchButtons();
    break;
  }
  return audio_error;
}

/**
  * @brief  Stops Audio streaming.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Stop(void)
{
  AudioState = AUDIO_STATE_STOP;
  FilePos = 0;

  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
  f_close(&WavFile);
  return AUDIO_ERROR_NONE;
}

/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  if(AudioState == AUDIO_STATE_PLAY)
  {
    BufferCtl.state = BUFFER_OFFSET_FULL;
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  if(AudioState == AUDIO_STATE_PLAY)
  {
    BufferCtl.state = BUFFER_OFFSET_HALF;
  }
}
/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Gets the file info.
  * @param  file_idx: File index
  * @param  info: Pointer to WAV file info
  * @retval Audio error
  */
static AUDIO_ErrorTypeDef GetFileInfo(uint16_t file_idx, WAVE_FormatTypeDef *info)
{
  uint32_t bytesread;
  uint32_t duration;
  uint8_t str[FILEMGR_FILE_NAME_SIZE + 20];

  if(f_open(&WavFile, (char *)FileList.file[file_idx].name, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    /* Fill the buffer to Send */
    if(f_read(&WavFile, info, sizeof(WaveFormat), (void *)&bytesread) == FR_OK)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      sprintf((char *)str, "Playing file (%d/%d): %s",
              file_idx + 1, FileList.ptr,
              (char *)FileList.file[file_idx].name);
      UTIL_LCD_ClearStringLine(4);
      UTIL_LCD_DisplayStringAtLine(4, str);

      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_CYAN);
      sprintf((char *)str,  "Sample rate : %d Hz", (int)(info->SampleRate));
      UTIL_LCD_ClearStringLine(6);
      UTIL_LCD_DisplayStringAtLine(6, str);

      sprintf((char *)str,  "Channels number : %d", info->NbrChannels);
      UTIL_LCD_ClearStringLine(7);
      UTIL_LCD_DisplayStringAtLine(7, str);

      duration = info->FileSize / info->ByteRate;
      sprintf((char *)str, "File Size : %d KB [%02d:%02d]", (int)(info->FileSize/1024), (int)(duration/60), (int)(duration%60));
      UTIL_LCD_ClearStringLine(8);
      UTIL_LCD_DisplayStringAtLine(8, str);
      UTIL_LCD_DisplayStringAt(263, LINE(8), (uint8_t *)"[00:00]", LEFT_MODE);

      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      sprintf((char *)str,  "Volume : %lu", uwVolume);
      UTIL_LCD_ClearStringLine(9);
      UTIL_LCD_DisplayStringAtLine(9, str);
      return AUDIO_ERROR_NONE;
    }
    f_close(&WavFile);
  }
  return AUDIO_ERROR_IO;
}

/**
  * @brief  Initializes the Wave player.
  * @param  AudioFreq: Audio sampling frequency
  * @retval None
  */
static uint8_t PlayerInit(uint32_t AudioFreq)
{
  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */
     AudioOutInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioOutInit.ChannelsNbr = 2;
  AudioOutInit.SampleRate = AudioFreq;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = uwVolume;
  if(BSP_AUDIO_OUT_Init(0, &AudioOutInit)!= 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Display interface touch screen buttons
  * @param  None
  * @retval None
  */
static void AUDIO_PlaybackDisplayButtons(void)
{
  UTIL_LCD_SetFont(&LCD_TRACE_HEADER_FONT);
  UTIL_LCD_ClearStringLine(13);            /* Clear dedicated zone */
  UTIL_LCD_ClearStringLine(14);
  UTIL_LCD_ClearStringLine(15);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_FillPolygon(PreviousPoints, 3,UTIL_LCD_COLOR_CYAN);   /* Previous track icon */
  UTIL_LCD_FillRect(TOUCH_PREVIOUS_XMIN, TOUCH_PREVIOUS_YMIN , 10, TOUCH_PREVIOUS_YMAX - TOUCH_PREVIOUS_YMIN,UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_FillPolygon(NextPoints, 3,UTIL_LCD_COLOR_CYAN);       /* Next track icon */
  UTIL_LCD_FillRect(TOUCH_NEXT_XMAX-9, TOUCH_NEXT_YMIN , 10, TOUCH_NEXT_YMAX - TOUCH_NEXT_YMIN,UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN,UTIL_LCD_COLOR_CYAN);    /* Pause rectangles */
  UTIL_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN,UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_FillRect(TOUCH_STOP_XMIN, TOUCH_STOP_YMIN , /* Stop rectangle */
                   TOUCH_STOP_XMAX - TOUCH_STOP_XMIN,
                   TOUCH_STOP_YMAX - TOUCH_STOP_YMIN,UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_DrawRect(TOUCH_VOL_MINUS_XMIN, TOUCH_VOL_MINUS_YMIN , /* VOl- rectangle */
                   TOUCH_VOL_MINUS_XMAX - TOUCH_VOL_MINUS_XMIN,
                   TOUCH_VOL_MINUS_YMAX - TOUCH_VOL_MINUS_YMIN,UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_DisplayStringAt(24, LINE(14), (uint8_t *)"VOl-", LEFT_MODE);
  UTIL_LCD_DrawRect(TOUCH_VOL_PLUS_XMIN, TOUCH_VOL_PLUS_YMIN , /* VOl+ rectangle */
                   TOUCH_VOL_PLUS_XMAX - TOUCH_VOL_PLUS_XMIN,
                   TOUCH_VOL_PLUS_YMAX - TOUCH_VOL_PLUS_YMIN,UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_DisplayStringAt(404, LINE(14), (uint8_t *)"VOl+", LEFT_MODE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_SetFont(&LCD_TRACE_TEXT_FONT);
  UTIL_LCD_ClearStringLine(15);
  UTIL_LCD_DisplayStringAtLine(15, (uint8_t *)"Use stop button to exit");
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_CYAN);
}

/**
  * @brief  Test touch screen state and modify audio state machine according to that
  * @param  None
  * @retval None
  */
static void AUDIO_AcquireTouchButtons(void)
{
  static TS_State_t  TS_State = {0};
  static uint32_t TouchdOn = 0;

  BSP_TS_GetState(0,&TS_State);
  if(TS_State.TouchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
  {
    TouchdOn++;
    TS_State.TouchDetected = 0;
  }
  if(TouchdOn > 2)
  {
    TouchdOn = 0;

    if ((TS_State.TouchX > TOUCH_PAUSE_XMIN) && (TS_State.TouchX < TOUCH_PAUSE_XMAX) &&
        (TS_State.TouchY > TOUCH_PAUSE_YMIN) && (TS_State.TouchY < TOUCH_PAUSE_YMAX))
    {
      HAL_Delay(50);
      TS_State.TouchDetected = 0;
      if (AudioState == AUDIO_STATE_PLAY)
      {
        AudioState = AUDIO_STATE_PAUSE;
      }
      else
      {
        AudioState = AUDIO_STATE_PLAY;
      }
    }
    else if ((TS_State.TouchX > TOUCH_NEXT_XMIN) && (TS_State.TouchX < TOUCH_NEXT_XMAX) &&
             (TS_State.TouchY > TOUCH_NEXT_YMIN) && (TS_State.TouchY < TOUCH_NEXT_YMAX))
    {
      AudioState = AUDIO_STATE_NEXT;
    }
    else if ((TS_State.TouchX > TOUCH_PREVIOUS_XMIN) && (TS_State.TouchX < TOUCH_PREVIOUS_XMAX) &&
             (TS_State.TouchY > TOUCH_PREVIOUS_YMIN) && (TS_State.TouchY < TOUCH_PREVIOUS_YMAX))
    {
      AudioState = AUDIO_STATE_PREVIOUS;
    }
    else if ((TS_State.TouchX > TOUCH_STOP_XMIN) && (TS_State.TouchX < TOUCH_STOP_XMAX) &&
             (TS_State.TouchY > TOUCH_STOP_YMIN) && (TS_State.TouchY < TOUCH_STOP_YMAX))
    {
      AudioState = AUDIO_STATE_STOP;
    }
    else if((TS_State.TouchX > TOUCH_VOL_MINUS_XMIN) && (TS_State.TouchX < TOUCH_VOL_MINUS_XMAX) &&
            (TS_State.TouchY > TOUCH_VOL_MINUS_YMIN) && (TS_State.TouchY < TOUCH_VOL_MINUS_YMAX))
    {
      AudioState = AUDIO_STATE_VOLUME_DOWN;
    }
    else if((TS_State.TouchX > TOUCH_VOL_PLUS_XMIN) && (TS_State.TouchX < TOUCH_VOL_PLUS_XMAX) &&
            (TS_State.TouchY > TOUCH_VOL_PLUS_YMIN) && (TS_State.TouchY < TOUCH_VOL_PLUS_YMAX))
    {
      AudioState = AUDIO_STATE_VOLUME_UP;
    }
  }
}


