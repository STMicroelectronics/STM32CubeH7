/**
  ******************************************************************************
  * @file    BSP/Src/audio_play.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the
  *          stm32h743i_eval_audio driver
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
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
AUDIO_PLAYBACK_StateTypeDef  audio_state;
BSP_AUDIO_Init_t* AudioPlayInit;

__IO uint32_t uwVolume = 20;
uint32_t PauseEnabledStatus = 0;
uint32_t AudioInstance = 0;
uint32_t OutputDevice = 0;
uint32_t bytesread;

uint32_t AudioFreq[8] = {96000, 48000, 44100, 32000, 22050, 16000, 11025, 8000};
uint8_t FreqStr[256] = {0};
static Point Points2[] = {{120, 140}, {160, 180}, {120, 220}};

TS_ActionTypeDef ts_action = TS_ACT_NONE;

/* Private function prototypes -----------------------------------------------*/
static void Audio_SetHint(uint32_t Index);
static uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData);
AUDIO_ErrorTypeDef AUDIO_Start(uint32_t *psrc_address, uint32_t file_size);
AUDIO_ErrorTypeDef AUDIO_Stop(void);

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

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  AudioFreq_ptr = &AudioFreq[1]; /* 48K*/
  ButtonState = 0;
  uwVolume = 70;

  Audio_SetHint(0);
  UTIL_LCD_SetFont(&Font20);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;

  ts_status = BSP_TS_Init(0, &hTS);


  if (ts_status != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"ERROR", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)"Touch Screen cannot be initialized", CENTER_MODE);
  }

  AudioPlayInit->Device = AUDIO_OUT_DEVICE_AUTO;
  AudioPlayInit->ChannelsNbr = 2;
  AudioPlayInit->SampleRate = *AudioFreq_ptr;
  AudioPlayInit->BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit->Volume = uwVolume;

  if(BSP_AUDIO_OUT_Init(AudioInstance, AudioPlayInit) == 0)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DisplayStringAt(0, y_size - 200, (uint8_t *)"  AUDIO CODEC   OK  ", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 215, (uint8_t *)"  AUDIO CODEC  FAIL ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 200, (uint8_t *)" Try to reset board ", CENTER_MODE);
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

  if(ts_status == BSP_ERROR_NONE)
  {
    /* Set touchscreen in Interrupt mode and program EXTI accordingly on falling edge of TS_INT pin */
    ts_status = BSP_TS_EnableIT(0);
    Touchscreen_DrawBackground_Circles(16);
  }

  UTIL_LCD_SetFont(&Font20);

  /* Audio is playing */
  UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(80, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(120, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);

  /* Infinite loop */
  while (1)
  {
      /* Get the TouchScreen State */
      ts_action = (TS_ActionTypeDef) TouchScreen_GetTouchPosition();

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
        sprintf((char*)FreqStr, "       VOL:    %lu     ", uwVolume);
        BSP_AUDIO_OUT_SetVolume(AudioInstance, uwVolume);
        UTIL_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)FreqStr, CENTER_MODE);
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
        sprintf((char*)FreqStr, "       VOL:    %lu     ", uwVolume);
        BSP_AUDIO_OUT_SetVolume(AudioInstance, uwVolume);
        UTIL_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)FreqStr, CENTER_MODE);
        break;
      case TS_ACT_FREQ_DOWN:
        /*Decrease Frequency */
        if (*AudioFreq_ptr != 8000)
        {
          AudioFreq_ptr++;
          BSP_AUDIO_OUT_Stop(AudioInstance);
          BSP_AUDIO_OUT_SetSampleRate(AudioInstance, *AudioFreq_ptr);
          AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);
        }
          sprintf((char*)FreqStr, "      FREQ: %lu     ", *AudioFreq_ptr);
        UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)FreqStr, CENTER_MODE);
        break;
      case TS_ACT_FREQ_UP:
        /* Increase Frequency */
        if (*AudioFreq_ptr != 96000)
        {
          AudioFreq_ptr--;
          BSP_AUDIO_OUT_Stop(AudioInstance);
          BSP_AUDIO_OUT_SetSampleRate(AudioInstance, *AudioFreq_ptr);
          AUDIO_Start((uint32_t *)AUDIO_SRC_FILE_ADDRESS, (uint32_t)AUDIO_FILE_SIZE);
        }
        sprintf((char*)FreqStr, "      FREQ: %lu     ", *AudioFreq_ptr);
        UTIL_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)FreqStr, CENTER_MODE);
        break;
      case TS_ACT_PAUSE:
        /* Set Pause / Resume */
        if (PauseEnabledStatus == 1)
        { /* Pause is enabled, call Resume */
          BSP_AUDIO_OUT_Resume(AudioInstance);
          PauseEnabledStatus = 0;
          UTIL_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"       PLAYING...     ", CENTER_MODE);
          UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(80, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_FillRect(120, 140, 25 , 80, UTIL_LCD_COLOR_BLACK);
        }
        else
        { /* Pause the playback */
          BSP_AUDIO_OUT_Pause(AudioInstance);
          PauseEnabledStatus = 1;
          UTIL_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"       PAUSE  ...     ", CENTER_MODE);
          UTIL_LCD_FillRect(120, 140, 25 , 80, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);
        }
        HAL_Delay(200);
        break;

      default:
        break;
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
    UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press TAMPER button for next menu          ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"Use touch screen +/- to change Volume/Frequency    ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Touch upper part of the screen to Pause/Resume    ", CENTER_MODE);
  }

  UTIL_LCD_DrawRect(10, 130, x_size - 20, y_size - 130, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 131, x_size - 22, y_size - 132, UTIL_LCD_COLOR_BLUE);
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
    BSP_AUDIO_OUT_Play(AudioInstance, &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
    audio_state = AUDIO_STATE_PLAYING;
    buffer_ctl.fptr = bytesread;
    return AUDIO_ERROR_NONE;
  }
  return AUDIO_ERROR_IO;
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
           the callbacks API are defined __weak in the stm32h743i_eval_audio.c file
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
    bytesread = GetData((void *)buffer_ctl.SrcAddress, buffer_ctl.fptr, &buffer_ctl.buff[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2);
    if( bytesread > 0)
    {
      buffer_ctl.state = BUFFER_OFFSET_NONE;
      buffer_ctl.fptr += bytesread;

      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&buffer_ctl.buff[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2);
    }
  }
  /* End of file */
  if(buffer_ctl.fptr >= buffer_ctl.AudioFileSize)
  {
    /* Play audio sample again ... */
    buffer_ctl.fptr = 0;
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
    bytesread = GetData((void *)buffer_ctl.SrcAddress, buffer_ctl.fptr, &buffer_ctl.buff[0], AUDIO_BUFFER_SIZE/2);
    if( bytesread > 0)
    {
      buffer_ctl.state = BUFFER_OFFSET_NONE;
      buffer_ctl.fptr += bytesread;

      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE/2);
    }
  }
  /* End of file */
  if(buffer_ctl.fptr >= buffer_ctl.AudioFileSize)
  {
    /* Play audio sample again ... */
    buffer_ctl.fptr = 0;
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

