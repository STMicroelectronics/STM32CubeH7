/**
  ******************************************************************************
  * @file    BSP/Src/audio_play.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the
  *          stm32h7b3i_eval_audio driver
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
#include "audio_16khz_wav.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define USE_SAI_INSTANCE

/* Audio file size */
#define AUDIO_FILE_SIZE               BYTES_IN_AUDIO_WAV

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

#if defined ( __ICCARM__ )
#pragma location=0x38000000
uint16_t  PlayBuffer[AUDIO_BUFFER_SIZE];
#elif defined ( __CC_ARM )
__attribute__((at(0x38000000))) uint16_t  PlayBuffer[AUDIO_BUFFER_SIZE];
#elif defined ( __GNUC__ )
uint16_t __attribute__((section (".RAM_D3")))  PlayBuffer[AUDIO_BUFFER_SIZE];
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AUDIO_BufferTypeDef  buffer_ctl;
static AUDIO_PLAYBACK_StateTypeDef  audio_state;
__IO uint32_t uwVolume = 20;

uint32_t PauseEnabledStatus = 0;
uint32_t updown = 1;

uint32_t AudioFreq[8] = {96000, 48000, 44100, 32000, 22050, 16000, 11025, 8000};

TS_ActionTypeDef ts_action = TS_ACT_NONE;
BSP_AUDIO_Init_t AudioPlayInit;
uint32_t AudioInstance = 0;
uint32_t OutputDevice = 0;
uint8_t FreqStr[256] = {0};

/* Private function prototypes -----------------------------------------------*/
static void Audio_SetHint(uint32_t Index);
static uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData);
AUDIO_ErrorTypeDef AUDIO_Start(uint32_t *psrc_address, uint32_t file_size);
AUDIO_ErrorTypeDef AUDIO_Stop(void);
static uint8_t AudioPlay_GetTouchPosition(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
void AudioPlay_demo (void)
{
  uint32_t *AudioFreq_ptr;
  uint32_t x_size, y_size;
  uint8_t ts_status = BSP_ERROR_NONE;
  static Point PointsVolUp[]   = {{380, 150}, {430, 150}, {405, 100}};
  static Point PointsVolDown[] = {{380, 190}, {430, 190}, {405, 240}};
  static Point PointsFreqUp[]   = {{70, 150}, {120, 150}, {95, 100}};
  static Point PointsFreqDown[] = {{70, 190}, {120, 190}, {95, 240}};

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  ButtonState = 0;
  uwVolume = 70;

#ifdef USE_SAI_INSTANCE
  AudioInstance = 0;
  AudioFreq_ptr = &AudioFreq[5]; /* 16K*/
#else
  AudioInstance = 1;
  AudioFreq_ptr = &AudioFreq[0]; /* 96K*/
#endif

  Audio_SetHint(0);
  UTIL_LCD_SetFont(&Font20);

  hTS.Orientation = TS_SWAP_XY;
  hTS.Accuracy = 0;
  hTS.Width = x_size;
  hTS.Height = y_size;

  ts_status = BSP_TS_Init(0, &hTS);

  if (ts_status != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"ERROR", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)"Touch Screen cannot be initialized", CENTER_MODE);
  }

  AudioPlayInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioPlayInit.ChannelsNbr = 2;
  AudioPlayInit.SampleRate = AUDIO_FREQUENCY_16K;
  AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit.Volume = uwVolume;

  if(BSP_AUDIO_OUT_Init(AudioInstance, &AudioPlayInit) != 0)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"  AUDIO CODEC  FAIL ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 60, (uint8_t *)" Press user button to exit ", CENTER_MODE);
    while (CheckForUserInput() == 0)
    {
    }
    ButtonState = 0;
    BSP_TS_DeInit(0);
  }
  else
  {
    /*
    Start playing the file from a circular buffer, once the DMA is enabled, it is
    always in running state. Application has to fill the buffer with the audio data
    using Transfer complete and/or half transfer complete interrupts callbacks
    (BSP_AUDIO_OUT_TransferComplete_CallBack() or BSP_AUDIO_OUT_HalfTransfer_CallBack()...
    */
    AUDIO_Start((uint32_t *)audio_wav + 11, (uint32_t)AUDIO_FILE_SIZE);

    /* Display the state on the screen */
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"    PLAYING...  ", CENTER_MODE);

    sprintf((char*)FreqStr, "FREQ:%6lu             VOL:%3lu",*AudioFreq_ptr,uwVolume);
    UTIL_LCD_DisplayStringAt(0,  LINE(8), (uint8_t *)FreqStr, CENTER_MODE);

    /* Set touchscreen in Interrupt mode and program EXTI accordingly on falling edge of TS_INT pin */
    ts_status = BSP_TS_EnableIT(0);


    UTIL_LCD_FillPolygon(PointsVolUp, 3, UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_FillPolygon(PointsVolDown, 3, UTIL_LCD_COLOR_RED);
    UTIL_LCD_FillPolygon(PointsFreqUp, 3, UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_FillPolygon(PointsFreqDown, 3, UTIL_LCD_COLOR_RED);



    /* Audio is playing */
    static Point PointsPauseResume[] = {{225, 100}, {275, 125}, {225, 150}};
    UTIL_LCD_FillPolygon(PointsPauseResume, 3, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(225, 100, 25 , 50, UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_FillRect(255, 100, 25 , 50, UTIL_LCD_COLOR_BLACK);


    /* Infinite loop */
    while (1)
    {
      /* IMPORTANT: AUDIO_Process() should be called within a periodic process */
      AUDIO_Process();

      if(AudioUpdate == 1)
      {
        /* Get the TouchScreen State */
        ts_action = (TS_ActionTypeDef) AudioPlay_GetTouchPosition();

        switch (ts_action)
        {
        case TS_ACT_VOLUME_UP:
          /* Increase volume by 5% */
          if (uwVolume < 95)
          {
            uwVolume += 5;
          }
          else
          {
            uwVolume = 100;
          }
          sprintf((char*)FreqStr, "FREQ:%6lu             VOL:%3lu",*AudioFreq_ptr,uwVolume);
          BSP_AUDIO_OUT_SetVolume(AudioInstance, uwVolume);
          UTIL_LCD_DisplayStringAt(0,  LINE(8), (uint8_t *)FreqStr, CENTER_MODE);

          break;
        case TS_ACT_VOLUME_DOWN:
          /* Decrease volume by 5% */
          if (uwVolume > 5)
          {
            uwVolume -= 5;
          }
          else
          {
            uwVolume = 0;
          }
          sprintf((char*)FreqStr, "FREQ:%6lu             VOL:%3lu",*AudioFreq_ptr,uwVolume);
          BSP_AUDIO_OUT_SetVolume(AudioInstance, uwVolume);
          UTIL_LCD_DisplayStringAt(0,  LINE(8), (uint8_t *)FreqStr, CENTER_MODE);
          break;
        case TS_ACT_FREQ_DOWN:
          /*Decrease Frequency */
          if (*AudioFreq_ptr != 8000)
          {
            AudioFreq_ptr++;
            sprintf((char*)FreqStr, "FREQ:%6lu             VOL:%3lu",*AudioFreq_ptr,uwVolume);
            BSP_AUDIO_OUT_Stop(AudioInstance);
            BSP_AUDIO_OUT_SetSampleRate(AudioInstance, *AudioFreq_ptr);
            UTIL_LCD_DisplayStringAt(0,  LINE(8), (uint8_t *)FreqStr, CENTER_MODE);
            AUDIO_Start((uint32_t *)audio_wav + 11, (uint32_t)AUDIO_FILE_SIZE);
          }
          UTIL_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)FreqStr, CENTER_MODE);
          break;
        case TS_ACT_FREQ_UP:
          /* Increase Frequency */
          if (*AudioFreq_ptr != 96000)
          {
            AudioFreq_ptr--;
            sprintf((char*)FreqStr, "FREQ:%6lu             VOL:%3lu",*AudioFreq_ptr,uwVolume);
            BSP_AUDIO_OUT_Stop(AudioInstance);
            BSP_AUDIO_OUT_SetSampleRate(AudioInstance, *AudioFreq_ptr);
            UTIL_LCD_DisplayStringAt(0,  LINE(8), (uint8_t *)FreqStr, CENTER_MODE);
            AUDIO_Start((uint32_t *)audio_wav + 11, (uint32_t)AUDIO_FILE_SIZE);
          }

          UTIL_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)FreqStr, CENTER_MODE);
          break;
        case TS_ACT_PAUSE:
          /* Set Pause / Resume */
          if (PauseEnabledStatus == 1)
          { /* Pause is enabled, call Resume */
            BSP_AUDIO_OUT_Resume(AudioInstance);
            PauseEnabledStatus = 0;
            UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"    PLAYING...  ", CENTER_MODE);
            UTIL_LCD_FillPolygon(PointsPauseResume, 3, UTIL_LCD_COLOR_WHITE);
            UTIL_LCD_FillRect(225, 100, 25 , 50, UTIL_LCD_COLOR_BLACK);
            UTIL_LCD_FillRect(255, 100, 25 , 50, UTIL_LCD_COLOR_BLACK);
          }
          else
          { /* Pause the playback */
            BSP_AUDIO_OUT_Pause(AudioInstance);
            PauseEnabledStatus = 1;
            UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"    PAUSE  ...  ", CENTER_MODE);
            UTIL_LCD_FillRect(225, 100, 60 , 50, UTIL_LCD_COLOR_WHITE);
            UTIL_LCD_FillPolygon(PointsPauseResume, 3, UTIL_LCD_COLOR_GREEN);
          }
          HAL_Delay(200);
          break;

        default:
          break;
        }
        while(TS_State.TouchDetected == 1)
        {
          BSP_TS_GetState(0, &TS_State);
        }
        AudioUpdate = 0;
      }

      if (CheckForUserInput() > 0)
      {
        ButtonState = 0;
        BSP_AUDIO_OUT_Stop(AudioInstance);
        BSP_AUDIO_OUT_DeInit(AudioInstance);
        BSP_TS_DisableIT(0);
        BSP_TS_DeInit(0);
        return;
      }
    }
  }
}

/**
  * @brief  TouchScreen get touch position
  * @retval None
  */
static uint8_t AudioPlay_GetTouchPosition(void)
{
  uint16_t x, y;
  uint8_t touch = 0;

  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  BSP_TS_GetState(0, &TS_State);
  if(TS_State.TouchDetected)
  {
    /* Get X and Y position of the first */
    x = TS_State.TouchX;
    y = TS_State.TouchY;

    if((x > 380) && (x < 430)) /* Volume */
    {
      if((y > 100) && (y < 150)) /* Volume Up*/
      {
        touch = TS_ACT_VOLUME_UP;
      }
      if((y > 190) && (y < 240)) /* Volume Down*/
      {
        touch = TS_ACT_VOLUME_DOWN;
      }
    }

    if((x > 70) && (x < 120)) /* Frequency */
    {
      if((y > 100) && (y < 150)) /* Frequency Up*/
      {
        touch = TS_ACT_FREQ_UP;
      }
      if((y > 190) && (y < 240)) /* Frequency Down*/
      {
        touch = TS_ACT_FREQ_DOWN;
      }
    }

    if((x > 225) && (x < 280)) /* Pause/resume */
    {
      if((y > 100) && (y < 150))
      {
        touch = TS_ACT_PAUSE;
      }
    }
  }

  return touch;
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
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  if(Index == 0)
  {
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SET MUTE / SET VOLUME / SET SAMPLE RATE", CENTER_MODE);
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press TAMPER button for next menu          ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"Use touch screen +/- to change Volume/Frequency    ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Touch upper part of the screen to Pause/Resume    ", CENTER_MODE);
  }

  UTIL_LCD_DrawRect(10, 81, x_size - 20, y_size - 81, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 82, x_size - 22, y_size - 82, UTIL_LCD_COLOR_BLUE);
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
                       (uint8_t*)&PlayBuffer[0],
                       2*AUDIO_BUFFER_SIZE);
  if(bytesread > 0)
  {
    BSP_AUDIO_OUT_Play(AudioInstance, (uint8_t *)PlayBuffer, 2*AUDIO_BUFFER_SIZE);
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
      SCB_InvalidateDCache_by_Addr((uint32_t *)&PlayBuffer[0], AUDIO_BUFFER_SIZE);
      bytesread = GetData((void *)buffer_ctl.SrcAddress,
                          buffer_ctl.fptr,
                          (uint8_t*)&PlayBuffer[0],
                          AUDIO_BUFFER_SIZE );

      if( bytesread >0)
      {
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread;
        /* Clean Data Cache to update the content of the SRAM */
        SCB_CleanDCache_by_Addr((uint32_t*)&PlayBuffer[0], AUDIO_BUFFER_SIZE);
      }
    }

    /* 2nd half buffer played; so fill it and continue playing from top */
    if(buffer_ctl.state == BUFFER_OFFSET_FULL)
    {
      SCB_InvalidateDCache_by_Addr((uint32_t *)&PlayBuffer[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE);
      bytesread = GetData((void *)buffer_ctl.SrcAddress,
                          buffer_ctl.fptr,
                          (uint8_t*)&PlayBuffer[AUDIO_BUFFER_SIZE /2],
                          AUDIO_BUFFER_SIZE );
      if( bytesread > 0)
      {
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread;

        /* Clean Data Cache to update the content of the SRAM */
        SCB_CleanDCache_by_Addr((uint32_t*)&PlayBuffer[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE);
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
           the callbacks API are defined __weak in the stm32h7b3i_discovery_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief  Manages the full Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
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
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
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
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
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

/**
  * @}
  */

/**
  * @}
  */

