/**
  ******************************************************************************
  * @file    BSP/Src/audio_loopback.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the
  *          bsp audio driver
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
#include <stdio.h>
#include "string.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int32_t  RecordBuffer1[AUDIO_BUFFER_SIZE/2];
int32_t  RecordBuffer2[AUDIO_BUFFER_SIZE/2];
int32_t *test_buffer[] = {RecordBuffer1, RecordBuffer2};

uint32_t  InState = 0;
uint32_t  OutState = 0;

uint32_t AudioBufferOffset;
uint32_t PlaybackStarted = 0;
BSP_AUDIO_Init_t  AudioInInit;
BSP_AUDIO_Init_t  AudioOutInit;

static uint32_t AudioFreq[8] = {8000 ,11025, 16000, 22050, 32000, 44100, 48000, 96000};
uint32_t *AudioFreq_ptr;
uint32_t VolumeLevel = 100, AudioOutState, AudioInState;
Point PointsAudio[] = {{600, 340}, {660, 220}, {600, 420}};
char text[256];

typedef struct
{
  uint32_t Mic1_Paused;
  uint32_t Mic2_Paused;
}MicPause_t;

MicPause_t MicPauseResume = {0, 0};

char* Audio_Out_State[4] = {
  " AUDIO_OUT_STATE_RESET  ",
  " AUDIO_OUT_STATE_PLAYING",
  " AUDIO_OUT_STATE_STOP   ",
  " AUDIO_OUT_STATE_PAUSE  "
};

char* Audio_In_State[4] = {
  "AUDIO_IN_STATE_RESET    ",
  "AUDIO_IN_STATE_RECORDING",
  "AUDIO_IN_STATE_STOP     ",
  "AUDIO_IN_STATE_PAUSE    "
};

char* Mute_State[2] = {
  " BSP_AUDIO_MUTE_DISABLED",
  " BSP_AUDIO_MUTE_ENABLED ",
};
/* Saturate the record PCM sample */
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/* Private function prototypes -----------------------------------------------*/
static void AudioLoopback_SetHint(uint8_t Index);
static void TS_AudioUpdate(void);
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Audio Record and Playback single-buffer mode test. Change record and
  *         play frequencies
  *         Record:
  *          - Audio IN instance: 2 (DFSDM)
  *          - Audio IN Device  : digital MIC1 and MIC2
  *          - Audio IN number of channel  : 2
  *         Playback:
  *          - Audio OUT instance: 0 (SAI)
  *          - Audio OUT Device  : headphone
  * @note  In this mode, data are processed internally in BSP audio drivers and
  *        copied to user buffer.
  * @retval None
  */
void REC_SingleBuff_demo(void)
{
  uint32_t channel_nbr = 2;
  uint32_t y_size, x_size;

  ButtonState = 0;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  AudioLoopback_SetHint(0);

  hTS.Orientation = TS_SWAP_XY;
  hTS.Accuracy = 0;
  hTS.Width = x_size;
  hTS.Height = y_size;

  BSP_TS_Init(0, &hTS);
  BSP_TS_EnableIT(0);
  Touchscreen_DrawBackground_Circles(64);

  AudioFreq_ptr = AudioFreq+5;

  AudioOutInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioOutInit.ChannelsNbr = channel_nbr;
  AudioOutInit.SampleRate = *AudioFreq_ptr;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = VolumeLevel;

  AudioInInit.Device = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInInit.ChannelsNbr = channel_nbr;
  AudioInInit.SampleRate = *AudioFreq_ptr;
  AudioInInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInInit.Volume = VolumeLevel;

  /* Initialize Audio Recorder with 2 channels to be used */
  BSP_AUDIO_IN_Init(2, &AudioInInit);
  BSP_AUDIO_IN_GetState(2, &InState);

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);
  /* Start Recording */
  BSP_AUDIO_IN_Record(2, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFFER_SIZE);

  while (1)
  {
    if(AudioBufferOffset == BUFFER_OFFSET_HALF)
    {
      if(PlaybackStarted == 0)
      {
        /* Play the recorded buffer*/
        BSP_AUDIO_OUT_Play(0, (uint8_t*)&PlayBuffer[0], 2*AUDIO_BUFFER_SIZE);
        BSP_AUDIO_OUT_GetState(0, &AudioOutState);
        BSP_AUDIO_IN_GetState(2, &AudioInState);
        UTIL_LCD_SetFont(&Font12);
        sprintf((char*)text, "   FREQ: %6lu         ", *AudioFreq_ptr);
        UTIL_LCD_DisplayStringAt(20, y_size - 50, (uint8_t *)text, CENTER_MODE);
        sprintf((char*)text, "%s   ", Audio_Out_State[AudioOutState]);
        UTIL_LCD_DisplayStringAt(20, y_size - 34, (uint8_t *)text, CENTER_MODE);
        sprintf((char*)text, "%s   ", Audio_In_State[AudioInState]);
        UTIL_LCD_DisplayStringAt(20, y_size - 18, (uint8_t *)text, CENTER_MODE);
        PlaybackStarted = 1;
      }
      AudioBufferOffset  = BUFFER_OFFSET_NONE;
    }

    if(AudioUpdate == 1)
    {
      /* Get the TouchScreen State */
      ts_action = (TS_ActionTypeDef) TouchScreen_GetTouchPosition();

      switch (ts_action)
      {
      case 3:
        /* Decrease Frequency */
        if (*AudioFreq_ptr != 96000)
        {
          AudioFreq_ptr++;
          BSP_AUDIO_OUT_Stop(0);
          BSP_AUDIO_IN_Stop(2);
          BSP_AUDIO_OUT_SetSampleRate(0, *AudioFreq_ptr);
          BSP_AUDIO_IN_SetSampleRate(2, *AudioFreq_ptr);
          BSP_AUDIO_IN_Record(2, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFFER_SIZE);
          HAL_Delay(40);
          PlaybackStarted = 0;

          sprintf((char*)text, "        FREQ: %6lu         ", *AudioFreq_ptr);
          UTIL_LCD_DisplayStringAt(0, y_size - 50, (uint8_t *)text, CENTER_MODE);
        }
        break;
      case 2:
        /* Increase Frequency */
        if (*AudioFreq_ptr != 8000)
        {
          AudioFreq_ptr--;
          BSP_AUDIO_OUT_Stop(0);
          BSP_AUDIO_IN_Stop(2);
          BSP_AUDIO_OUT_SetSampleRate(0, *AudioFreq_ptr);
          BSP_AUDIO_IN_SetSampleRate(2, *AudioFreq_ptr);
          BSP_AUDIO_IN_Record(2, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFFER_SIZE);
          HAL_Delay(40);
          PlaybackStarted = 0;

          sprintf((char*)text, "        FREQ: %6lu         ", *AudioFreq_ptr);
          UTIL_LCD_DisplayStringAt(0, y_size - 50, (uint8_t *)text, CENTER_MODE);
        }
        break;

      default:
        break;
      }

      AudioUpdate = 0;
    }

    if (CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      BSP_AUDIO_IN_Stop(2);
      BSP_AUDIO_IN_DeInit(2);
      BSP_TS_DisableIT(0);
      BSP_TS_DeInit(0);
      PlaybackStarted = 0;
      return;
    }
  }
}

/**
  * @brief  Audio Record and Playback multi-buffer mode test
  *         Record:
  *          - Audio IN instance: 2 (DFSDM)
  *          - Audio IN Device  : digital MIC1 and MIC2
  *          - Audio IN number of channel  : 2
  *         Playback:
  *          - Audio OUT instance: 0 (SAI)
  *          - Audio OUT Device  : headphone
  * @note  In this mode, data are processed at application level. The BSP drivers
  *        return only the data acquisition status (completed or half completed)
  * @retval None
  */
void REC_MultiBuff_demo(void)
{
  uint32_t index;
  uint32_t y_size, x_size;

  ButtonState = 0;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  AudioLoopback_SetHint(1);

  hTS.Orientation = TS_SWAP_XY;
  hTS.Accuracy = 0;
  hTS.Width = x_size;
  hTS.Height = y_size;

  /* Touchscreen initialization */
  BSP_TS_Init(0, &hTS);
  BSP_TS_EnableIT(0);

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetFont(&Font24);

  UTIL_LCD_FillRect(80, 200, 80, 50, UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_FillRect(170, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_FillRect(200, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(90, 215, (uint8_t *)"MIC1", LEFT_MODE);

  UTIL_LCD_FillRect(280, 200, 80, 50, UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_FillRect(370, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_FillRect(400, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(290, 215, (uint8_t *)"MIC2", LEFT_MODE);

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);


  AudioFreq_ptr = AudioFreq+2; /* AUDIO_FREQUENCY_16K; */

  AudioOutInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioOutInit.ChannelsNbr = 2;
  AudioOutInit.SampleRate = *AudioFreq_ptr;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = VolumeLevel;

  AudioInInit.Device = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInInit.ChannelsNbr = 2;
  AudioInInit.SampleRate = *AudioFreq_ptr;
  AudioInInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInInit.Volume = VolumeLevel;

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  /* Initialize Audio Recorder with 2 channels to be used */
  BSP_AUDIO_IN_Init(2, &AudioInInit);

  AudioBufferOffset  = BUFFER_OFFSET_NONE;

  /* Start Recording */
  BSP_AUDIO_IN_RecordChannels(2, (uint8_t**)test_buffer, AUDIO_BUFFER_SIZE/2);
  MicPauseResume.Mic1_Paused = 0;
  MicPauseResume.Mic2_Paused = 0;

  while (1)
  {
    if(AudioBufferOffset == BUFFER_OFFSET_HALF)
    {
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RecordBuffer1[0], AUDIO_BUFFER_SIZE);
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RecordBuffer2[0], AUDIO_BUFFER_SIZE);
      for(index = 0 ; index < AUDIO_BUFFER_SIZE/4; index++)
      {
        if(MicPauseResume.Mic1_Paused == 0)
        {
          PlayBuffer[2*index+1]       = (uint16_t)(SaturaLH((RecordBuffer1[index] >> 8), -32760, 32760));
        }
        if(MicPauseResume.Mic2_Paused == 0)
        {
          PlayBuffer[2*index] = (uint16_t)(SaturaLH((RecordBuffer2[index] >> 8), -32760, 32760));
        }
      }
      SCB_CleanDCache_by_Addr((uint32_t *)PlayBuffer, 2*AUDIO_BUFFER_SIZE);

      if(PlaybackStarted == 0)
      {
        /* Play the recorded buffer*/
        BSP_AUDIO_OUT_Play(0, (uint8_t*)&PlayBuffer[0], 2*AUDIO_BUFFER_SIZE);
        PlaybackStarted = 1;
      }
      AudioBufferOffset  = BUFFER_OFFSET_NONE;
    }

    if(AudioBufferOffset == BUFFER_OFFSET_FULL)
    {
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RecordBuffer1[AUDIO_BUFFER_SIZE/4], AUDIO_BUFFER_SIZE);
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RecordBuffer2[AUDIO_BUFFER_SIZE/4], AUDIO_BUFFER_SIZE);
      for(index = AUDIO_BUFFER_SIZE/4 ; index < AUDIO_BUFFER_SIZE/2; index++)
      {
        if(MicPauseResume.Mic1_Paused == 0)
        {
          PlayBuffer[2*index+1]       = (uint16_t)(SaturaLH((RecordBuffer1[index] >> 8), -32760, 32760));
        }
        if(MicPauseResume.Mic2_Paused == 0)
        {
          PlayBuffer[2*index] = (uint16_t)(SaturaLH((RecordBuffer2[index] >> 8), -32760, 32760));
        }
      }

      SCB_CleanDCache_by_Addr((uint32_t *)PlayBuffer, 2*AUDIO_BUFFER_SIZE);
      AudioBufferOffset  = BUFFER_OFFSET_NONE;
    }

    TS_AudioUpdate();

    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      BSP_AUDIO_IN_StopChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC);
      BSP_AUDIO_IN_DeInit(2);
      BSP_TS_DisableIT(0);
      BSP_TS_DeInit(0);
      PlaybackStarted = 0;
      return;
    }
  }
}

/**
  * @brief  Update audio process following TS positions
  * @retval None
  */
void TS_AudioUpdate(void)
{
  uint32_t i;
  Point Points1[3], Points2[3];

  Points1[0].X = 170;
  Points1[0].Y = 200;
  Points1[1].X = 220;
  Points1[1].Y = 225;
  Points1[2].X = 170;
  Points1[2].Y = 250;

  Points2[0].X = 370;
  Points2[0].Y = 200;
  Points2[1].X = 420;
  Points2[1].Y = 225;
  Points2[2].X = 370;
  Points2[2].Y = 250;

  if(AudioUpdate == 1)
  {
    BSP_TS_GetState(0, &TS_State);

    if(TS_State.TouchDetected)
    {
      if(((TS_State.TouchX > 170) && (TS_State.TouchX < 220)) && ((TS_State.TouchY > 200) && (TS_State.TouchY < 250)))
      {
        HAL_Delay(100);
        if(MicPauseResume.Mic1_Paused == 0)
        {
          UTIL_LCD_FillRect(170, 200, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(200, 200, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillPolygon(Points1, 3, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_PauseChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          MicPauseResume.Mic1_Paused = 1;
          for(i = 0; i < AUDIO_BUFFER_SIZE/2; i++)
          {
            PlayBuffer[2*i+1] = 0;
          }
        }
        else
        {
          UTIL_LCD_FillPolygon(Points1, 3, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(170, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
          UTIL_LCD_FillRect(200, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_ResumeChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          MicPauseResume.Mic1_Paused = 0;
        }
      }

      if(((TS_State.TouchX > 370) && (TS_State.TouchX < 420)) && ((TS_State.TouchY > 200) && (TS_State.TouchY < 250)))
      {
        HAL_Delay(100);
        if(MicPauseResume.Mic2_Paused == 0)
        {
          UTIL_LCD_FillRect(370, 200, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(400, 200, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_PauseChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC2);
          MicPauseResume.Mic2_Paused = 1;
          for(i = 0; i < AUDIO_BUFFER_SIZE/2; i++)
          {
            PlayBuffer[2*i] = 0;
          }
        }
        else
        {
          UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(370, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
          UTIL_LCD_FillRect(400, 200, 20, 50, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_ResumeChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC2);
          BSP_AUDIO_IN_PauseChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          BSP_AUDIO_IN_ResumeChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          if(MicPauseResume.Mic1_Paused == 1)
          {
            BSP_AUDIO_IN_PauseChannels(2, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          }
          MicPauseResume.Mic2_Paused = 0;
        }
      }
    }
    AudioUpdate = 0;

    if((MicPauseResume.Mic1_Paused == 0) && (MicPauseResume.Mic2_Paused == 0))
    {
      sprintf((char*)text, "MIC1: Active  MIC2: Active");
      UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)text, CENTER_MODE);
    }
    else if((MicPauseResume.Mic1_Paused == 1) && (MicPauseResume.Mic2_Paused == 0))
    {
      sprintf((char*)text, "MIC1: Paused  MIC2: Active");
      UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)text, CENTER_MODE);
    }
    else if((MicPauseResume.Mic1_Paused == 0) && (MicPauseResume.Mic2_Paused == 1))
    {
      sprintf((char*)text, "MIC1: Active  MIC2: Paused");
      UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)text, CENTER_MODE);
    }
    else /*     if((MicPauseResume.Mic1_Paused == 1) && (MicPauseResume.Mic2_Paused == 1)) */
    {
      sprintf((char*)text, "MIC1: Paused  MIC2: Paused");
      UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)text, CENTER_MODE);
    }
  }
}

/**
  * @brief  Manages the DMA Full Transfer complete interrupt.
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  AudioBufferOffset = BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  AudioBufferOffset = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Display Audio Record demo hint
  * @param  Index Test index
  * @retval None
  */
static void AudioLoopback_SetHint(uint8_t Index)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set Audio Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 90, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font12);

  if(Index == 0)
  {
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"DFSDM RECORDING & SAI PLAYING in Single buffer mode ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Use touch screen +/- to change Frequency    ", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"DFSDM RECORDING & SAI PLAYING in Multi buffer mode ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Use touch screen to Pause/Resume MIC1/MIC2    ", CENTER_MODE);
  }
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press TAMPER button for next menu          ", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 100, x_size - 20, y_size - 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 101, x_size - 22, y_size - 102, UTIL_LCD_COLOR_BLUE);
}

/**
  * @}
  */

/**
  * @}
  */

