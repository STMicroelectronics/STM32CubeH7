/**
  ******************************************************************************
  * @file    BSP/CM7/Src/audio_play.c
  * @author  MCD Application Team
  * @version V0.0.1
  * @date    20-January-2018
  * @brief   This example code shows how to use the audio feature in the
  *          stm32h747i_eval driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */
/* Private define ------------------------------------------------------------*/

/*Since SysTick is set to 1ms (unless to set it quicker) */
/* to run up to 48khz, a buffer around 1000 (or more) is requested*/
/* to run up to 96khz, a buffer around 2000 (or more) is requested*/
#define AUDIO_DEFAULT_VOLUME    70

/* Audio file size and start address are defined here since the audio file is
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_START_OFFSET_ADDRESS    0            /* Offset relative to audio file header size */
#define AUDIO_BUFFER_SIZE            2048

/* Audio file size and start address are defined here since the audio file is
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_SIZE               524288
#define AUDIO_START_OFFSET_ADDRESS    0            /* Offset relative to audio file header size */
#define AUDIO_FILE_ADDRESS            0x08080000   /* Audio file address */
/* Private typedef -----------------------------------------------------------*/
typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_INIT,
  AUDIO_STATE_PLAYING,
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

typedef struct {
  uint8_t buff[AUDIO_BUFFER_SIZE];
  uint32_t fptr;
  BUFFER_StateTypeDef state;
  uint32_t AudioFileSize;
  uint32_t *SrcAddress;
}AUDIO_BufferTypeDef;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ALIGN_32BYTES (static AUDIO_BufferTypeDef  buffer_ctl);
static AUDIO_PLAYBACK_StateTypeDef  audio_state;
static uint32_t JoyState = JOY_NONE;
__IO uint32_t uwVolume = 20;
__IO uint32_t uwPauseEnabledStatus = 0;
uint32_t OutputDevice = 0;
uint32_t bytesread;
uint8_t FreqStr[256] = {0};
uint32_t AudioFreq[8] = {96000, 48000, 44100, 32000, 22050, 16000, 11025, 8000};
TS_ActionTypeDef ts_action = TS_ACT_NONE;
BSP_AUDIO_Init_t AudioPlayInit;

/* Private function prototypes -----------------------------------------------*/
static void Audio_SetHint(uint32_t Index);
static uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData);
static void AudioDeviceUpdate(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
void AudioPlay_demo (void)
{
  uint32_t *AudioFreq_ptr;
  uint32_t y_size;

  BSP_LCD_GetYSize(0, &y_size);

  AudioFreq_ptr = &AudioFreq[1]; /*96K*/
  ButtonState = 0;

  uint8_t FreqStr[256] = {0};
  uint8_t ts_status = BSP_ERROR_NONE;
  Point Points2[] = {{100, 140}, {160, 180}, {100, 220}};

  uwPauseEnabledStatus = 1; /* 0 when audio is running, 1 when Pause is on */
  uwVolume = 40;

  Audio_SetHint(0);
  UTIL_LCD_SetFont(&Font20);

  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy = 5;
  hTS.Width = 800;
  hTS.Height = 480;

  ts_status = BSP_TS_Init(0, &hTS);
  BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);

  if (ts_status != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"ERROR", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)"Touch Screen cannot be initialized", CENTER_MODE);
  }
  AudioPlayInit.Device = AUDIO_OUT_DEVICE_SPK_HP;
  AudioPlayInit.ChannelsNbr = 2;
  AudioPlayInit.SampleRate = AUDIO_FREQUENCY_48K;
  AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit.Volume = uwVolume;

  if(BSP_AUDIO_OUT_Init(0, &AudioPlayInit) == 0)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"  AUDIO CODEC   OK  ", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"  AUDIO CODEC  FAIL ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  /*
  Start playing the file from a circular buffer, once the DMA is enabled, it is
  always in running state. Application has to fill the buffer with the audio data
  using Transfer complete and/or half transfer complete interrupts callbacks
  (BSP_AUDIO_OUT_TransferComplete_CallBack() or BSP_AUDIO_OUT_HalfTransfer_CallBack()...
  */
  AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);

  /* Display the state on the screen */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"       PLAYING...     ", CENTER_MODE);

  sprintf((char*)FreqStr, "       VOL:    %lu     ", uwVolume);
  UTIL_LCD_DisplayStringAt(0,  LINE(9), (uint8_t *)FreqStr, CENTER_MODE);

  sprintf((char*)FreqStr, "      FREQ: %lu     ", *AudioFreq_ptr);
  UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)FreqStr, CENTER_MODE);

  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, y_size - 40, (uint8_t *)"Hear nothing ? Have you copied the audio file with STM-LINK UTILITY ?", CENTER_MODE);

  UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"                        ", CENTER_MODE);
  Touchscreen_DrawBackground_Circles(16);

  UTIL_LCD_SetFont(&Font20);
  /* Audio is playing */
  UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(100, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(140, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);

  /* IMPORTANT:
  AUDIO_Process() is called by the SysTick Handler, as it should be called
  within a periodic process */

  /* Infinite loop */
  while (1)
  {
   /* IMPORTANT: AUDIO_Process() should be called within a periodic process */
    AUDIO_Process();
    AudioDeviceUpdate();
    /* Get the TouchScreen State */
    ts_action = (TS_ActionTypeDef) TouchScreen_GetTouchPosition();

    switch (ts_action)
    {

    case TS_ACT_VOLUME_UP:
      /* Increase volume by 5% */
      if (uwVolume < 95)
        uwVolume += 5;
      else
        uwVolume = 100;
      sprintf((char*)FreqStr, "       VOL:    %lu     ", uwVolume);
      BSP_AUDIO_OUT_SetVolume(0, uwVolume);
      UTIL_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)FreqStr, CENTER_MODE);
      break;
    case TS_ACT_VOLUME_DOWN:
      /* Decrease volume by 5% */
      if (uwVolume > 5)
        uwVolume -= 5;
      else
        uwVolume = 0;
      sprintf((char*)FreqStr, "       VOL:    %lu     ", uwVolume);
      BSP_AUDIO_OUT_SetVolume(0, uwVolume);
      UTIL_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)FreqStr, CENTER_MODE);
      break;
    case TS_ACT_FREQ_DOWN:
      /*Decrease Frequency */
      if (*AudioFreq_ptr != 8000)
      {
        AudioFreq_ptr++;
        sprintf((char*)FreqStr, "      FREQ: %lu     ", *AudioFreq_ptr);
        BSP_AUDIO_OUT_Stop(0);
        BSP_AUDIO_OUT_SetSampleRate(0, *AudioFreq_ptr);
        AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);
        BSP_AUDIO_OUT_SetVolume(0, uwVolume);
      }
      UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)FreqStr, CENTER_MODE);
      break;
    case TS_ACT_FREQ_UP:
      /* Increase Frequency */
      if (*AudioFreq_ptr != 96000)
      {
        AudioFreq_ptr--;
        sprintf((char*)FreqStr, "      FREQ: %lu     ", *AudioFreq_ptr);
        BSP_AUDIO_OUT_Stop(0);
        BSP_AUDIO_OUT_SetSampleRate(0, *AudioFreq_ptr);
        AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);
        BSP_AUDIO_OUT_SetVolume(0, uwVolume);
      }
      sprintf((char*)FreqStr, "      FREQ: %lu     ", *AudioFreq_ptr);
      UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)FreqStr, CENTER_MODE);
      break;
    case TS_ACT_PAUSE:
      /* Set Pause / Resume */
      if (uwPauseEnabledStatus == 1)
      { /* Pause is enabled, call Resume */
        BSP_AUDIO_OUT_Resume(0);
        uwPauseEnabledStatus = 0;
        UTIL_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"       PLAYING...     ", CENTER_MODE);
        UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_FillRect(100, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);
        UTIL_LCD_FillRect(140, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);
      }
      else
      { /* Pause the playback */
        BSP_AUDIO_OUT_Pause(0);
        uwPauseEnabledStatus = 1;
        UTIL_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"       PAUSE  ...     ", CENTER_MODE);
        UTIL_LCD_FillRect(100, 140, 80 , 80, UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);
      }
      break;

    default:
      break;
    }

    if (CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      BSP_JOY_DeInit(JOY1, JOY_ALL);

      return;
    }
  }
}

/**
  * @brief  Display Audio demo hint
  * @param  None
  * @retval None
  */
static void Audio_SetHint(uint32_t Index)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set Audio Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 120, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  if(Index == 0)
  {
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SET MUTE / SET VOLUME / SET SAMPLE RATE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User button for next menu          ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"Use touch screen +/- to change Volume    ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Use touch screen +/- to change Frequency ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 75, (uint8_t *)"Touch upper part of the screen to Pause/Resume    ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 90, (uint8_t *)"Use Joy keys to set output device    ", CENTER_MODE);
  }
  else
  {
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SET CHANNEL NUMBER/ SET BIT PER SAMPLE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User button for next menu          ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"Use Joy keys UP/DOWN to set Channels Number    ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Use Joy keys LEFT/RIGHT to set resolution    ", CENTER_MODE);
  }

  UTIL_LCD_DrawRect(10, 130, x_size - 20, y_size - 140, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 131, x_size - 22, y_size - 142, UTIL_LCD_COLOR_BLUE);
}


/**
  * @brief  Starts Audio streaming.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_Start(uint32_t *psrc_address, uint32_t file_size)
{
  uint32_t bytesread;

  buffer_ctl.state = BUFFER_OFFSET_NONE;
  buffer_ctl.AudioFileSize = file_size;
  buffer_ctl.SrcAddress = psrc_address;

  bytesread = GetData( (void *)psrc_address,
                       0,
                       &buffer_ctl.buff[0],
                       AUDIO_BUFFER_SIZE);
  if(bytesread > 0)
  {

    BSP_AUDIO_OUT_Play(0, (uint8_t *)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
    audio_state = AUDIO_STATE_PLAYING;
    buffer_ctl.fptr = bytesread;
    return AUDIO_ERROR_NONE;
  }
  return AUDIO_ERROR_IO;
}

/**
  * @brief  Manages Audio process.
  * @param  None
  * @retval Audio error
  */
uint8_t AUDIO_Process(void)
{
  uint32_t bytesread;
  AUDIO_ErrorTypeDef error_state = AUDIO_ERROR_NONE;

  switch(audio_state)
  {
  case AUDIO_STATE_PLAYING:

    if(buffer_ctl.fptr >= buffer_ctl.AudioFileSize)
    {
      /* Play audio sample again ... */
      buffer_ctl.fptr = 0;
      error_state = AUDIO_ERROR_EOF;
    }

    /* 1st half buffer played; so fill it and continue playing from bottom*/
    if(buffer_ctl.state == BUFFER_OFFSET_HALF)
    {
      bytesread = GetData((void *)buffer_ctl.SrcAddress,
                          buffer_ctl.fptr,
                          &buffer_ctl.buff[0],
                          AUDIO_BUFFER_SIZE /2);

      if( bytesread >0)
      {
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread;
              /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE/2);
      }
    }

    /* 2nd half buffer played; so fill it and continue playing from top */
    if(buffer_ctl.state == BUFFER_OFFSET_FULL)
    {
      bytesread = GetData((void *)buffer_ctl.SrcAddress,
                          buffer_ctl.fptr,
                          &buffer_ctl.buff[AUDIO_BUFFER_SIZE /2],
                          AUDIO_BUFFER_SIZE /2);
      if( bytesread > 0)
      {
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread;
      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&buffer_ctl.buff[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2);
      }
    }
    break;

  default:
    error_state = AUDIO_ERROR_NOTREADY;
    break;
  }
  return (uint8_t) error_state;
}

/**
  * @brief  Gets Data from storage unit.
  * @param  None
  * @retval None
  */
static uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData)
{
  uint8_t *lptr = pdata;
  uint32_t ReadDataNbr;

  ReadDataNbr = 0;
  while(((offset + ReadDataNbr) < buffer_ctl.AudioFileSize) && (ReadDataNbr < NbrOfData))
  {
    pbuf[ReadDataNbr]= lptr [offset + ReadDataNbr];
    ReadDataNbr++;
  }
  return ReadDataNbr;
}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32769i_discovery_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief  Manages the full Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Interface)
{
  if(audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 2nd part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_FULL;
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Interface)
{
  if(audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 1st part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_HALF;
  }
}

/**
  * @brief  Manages the DMA FIFO error event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Interface)
{
  /* Display message on the LCD screen */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"       DMA  ERROR     ", CENTER_MODE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_USER) != RESET)
  {
    return;
  }

  /* could also generate a system reset to recover from the error */
  /* .... */
}


static void AudioDeviceUpdate(void)
{
  JoyState = BSP_JOY_GetState(JOY1);

  switch(JoyState)
  {
  case JOY_UP:
    BSP_AUDIO_OUT_Stop(0);
    BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_HEADPHONE);
    AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);

    UTIL_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"AUDIO_OUT_DEVICE_HEADPHONE   ", CENTER_MODE);
    break;
  case JOY_DOWN:
    BSP_AUDIO_OUT_Stop(0);
    BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_SPEAKER);
    AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);
    UTIL_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"AUDIO_OUT_DEVICE_SPEAKER   ", CENTER_MODE);
    break;
  case JOY_LEFT:
    BSP_AUDIO_OUT_Stop(0);
    BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_SPK_HP);
    AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);
    UTIL_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"AUDIO_OUT_DEVICE_SPK_HP   ", CENTER_MODE);
    break;
  case JOY_RIGHT:
    BSP_AUDIO_OUT_Stop(0);
    BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_AUTO);
    AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);
    UTIL_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"AUDIO_OUT_DEVICE_AUTO      ", CENTER_MODE);
    break;
  default:
    break;
  }
}

/**
  * @}
  */

/**
  * @}
  */

