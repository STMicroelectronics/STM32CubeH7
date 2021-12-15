/**
  ******************************************************************************
  * @file    BSP/CM7/Src/audio_record.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the
  *          stm32h747i_discovery driver
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
#define AUDIO_BUFF_SIZE  4096
#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)
#define AUDIO_NB_BLOCKS    ((uint32_t)4)
#define AUDIO_IN_PDM_BUFFER_SIZE  256
uint32_t PDM ;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t  internal_buffer[AUDIO_BLOCK_SIZE];
#if defined ( __ICCARM__ )  /* !< ICCARM Compiler */
#pragma location=0x38000000
uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE];
#pragma location=0x38000200
int32_t  RecordBuffer1[AUDIO_BUFF_SIZE];
#pragma location=0x38004200
int32_t  RecordBuffer2[AUDIO_BUFF_SIZE];
#pragma location=0x38000800
uint16_t  PlayBuffer[AUDIO_BUFF_SIZE];
#elif defined ( __CC_ARM )  /* !< ARM Compiler */
ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
ALIGN_32BYTES (uint16_t RecordBuffer1[AUDIO_BUFF_SIZE]) __attribute__((section(".RAM_D3")));
ALIGN_32BYTES (uint16_t RecordBuffer2[AUDIO_BUFF_SIZE]) __attribute__((section(".RAM_D3")));
ALIGN_32BYTES (uint16_t PlayBuffer[AUDIO_BUFF_SIZE]) __attribute__((section(".RAM_D3")));
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
ALIGN_32BYTES (uint16_t RecordBuffer1[AUDIO_BUFF_SIZE]) __attribute__((section(".RAM_D3")));
ALIGN_32BYTES (uint16_t RecordBuffer2[AUDIO_BUFF_SIZE]) __attribute__((section(".RAM_D3")));
ALIGN_32BYTES (uint16_t PlayBuffer[AUDIO_BUFF_SIZE]) __attribute__((section(".RAM_D3")));
#endif

int32_t *test_buffer[AUDIO_BUFF_SIZE*2] = {0};

ALIGN_32BYTES (uint16_t  RecPlayback[AUDIO_BUFF_SIZE]);
ALIGN_32BYTES (uint16_t  PlaybackBuffer[2*AUDIO_BUFF_SIZE]);

/* Pointer to record_data */
uint32_t playbackPtr;

uint32_t  InState = 0;
uint32_t  OutState = 0;

uint32_t AudioBufferOffset;
uint32_t PlaybackStarted = 0;
BSP_AUDIO_Init_t  AudioInInit;
BSP_AUDIO_Init_t  AudioOutInit;
BSP_AUDIO_Init_t AnalogInInit;

static uint32_t AudioFreq[9] = {8000 ,11025, 16000, 22050, 32000, 44100, 48000, 96000, 192000};
uint32_t *AudioFreq_ptr;
uint32_t audio_Control=0;
uint32_t VolumeLevel = 80, AudioOutState, AudioInState, MuteState;
char text[256];
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

/* Private function prototypes -----------------------------------------------*/
static void AudioLoopback_SetHint(uint8_t Index);
static void AudioRec_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Record and Playback multi-buffer mode test
  *         Record:
  *          - Audio IN instance: 1 (DFSDM)
  *          - Audio IN Device  : digital MIC1 and MIC2
  *          - Audio IN number of channel  : 2
  *         Playback:
  *          - Audio OUT instance: 0 (SAI)
  *          - Audio OUT Device  : HDMI
  * @retval None
  */
void REC_INSTANCE1_HDMI_demo(void)
{
  uint32_t y_size;

  BSP_LCD_GetYSize(0, &y_size);
  AudioLoopback_SetHint(4);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO PLAYBACK USING HDMI", CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 24, (uint8_t *)"Make sure J23 & J24 are in position 2-3 ", CENTER_MODE);

  uint32_t index;

  AudioFreq_ptr = AudioFreq+5; /* AUDIO_FREQUENCY_44K; */

  AudioOutInit.Device = AUDIO_OUT_DEVICE_HDMI;
  AudioOutInit.ChannelsNbr = 2;
  AudioOutInit.SampleRate = *AudioFreq_ptr;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = VolumeLevel;

  AudioInInit.Device = AUDIO_IN_DEVICE_DIGITAL_MIC1 | AUDIO_IN_DEVICE_DIGITAL_MIC2;
  AudioInInit.ChannelsNbr = 2;
  AudioInInit.SampleRate = *AudioFreq_ptr;
  AudioInInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInInit.Volume = VolumeLevel;

  /* Initialize Audio Recorder with 2 channels to be used */
  BSP_AUDIO_IN_Init(1, &AudioInInit);

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  /* Start Recording */
  BSP_AUDIO_IN_RecordChannels(1, (uint8_t**)test_buffer, AUDIO_BUFF_SIZE);

  while (1)
  {
    if(AudioBufferOffset == BUFFER_OFFSET_HALF)
    {
      for(index = 0 ; index < AUDIO_BUFF_SIZE/2; index++)
      {
        PlaybackBuffer[2*index]       = (uint16_t)(RecordBuffer1[index] >> 8);
        PlaybackBuffer[(2*index) + 1] = (uint16_t)(RecordBuffer2[index] >> 8);
      }

      if(PlaybackStarted == 0)
      {
        /* Play the recorded buffer*/
        BSP_AUDIO_OUT_Play(0, (uint8_t*)&PlaybackBuffer[0], 2*2*AUDIO_BUFF_SIZE);
        BSP_AUDIO_OUT_GetState(0, &AudioOutState);
        BSP_AUDIO_IN_GetState(1, &AudioInState);
        sprintf((char*)text, "FREQ: %6lu     %s       VOL:    %3lu", *AudioFreq_ptr, Audio_In_State[AudioInState], VolumeLevel);
        UTIL_LCD_DisplayStringAt(0, y_size - 70, (uint8_t *)text, CENTER_MODE);
        sprintf((char*)text, "               %s                  ", Audio_Out_State[AudioOutState]);
        UTIL_LCD_DisplayStringAt(0, y_size - 54, (uint8_t *)text, CENTER_MODE);

        PlaybackStarted = 1;
      }
      AudioBufferOffset  = BUFFER_OFFSET_NONE;
    }

    if(AudioBufferOffset == BUFFER_OFFSET_FULL)
    {
      for(index = AUDIO_BUFF_SIZE/2 ; index < AUDIO_BUFF_SIZE; index++)
      {
        PlaybackBuffer[2*index]       = (uint16_t)(RecordBuffer1[index] >> 8);
        PlaybackBuffer[(2*index) + 1] = (uint16_t)(RecordBuffer2[index] >> 8);
      }
      AudioBufferOffset  = BUFFER_OFFSET_NONE;
    }

    if (CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      BSP_AUDIO_IN_StopChannels(1, AudioInInit.Device);
      BSP_AUDIO_IN_DeInit(1);

      PlaybackStarted = 0;
      for(index = 0; index < 2* AUDIO_BUFF_SIZE; index++)
      {
        PlaybackBuffer[index] = 0;
      }
      return;
    }
  }
}

/**
  * @brief  Audio Record and Playback using digital mic.
  *         Record:
  *          - Audio IN instance: 2 PDM
  *          - Audio IN Device  : AUDIO_IN_DEVICE_DIGITAL_MIC
  *          - Audio IN number of channel  : 1
  *         Playback:
  *          - Audio OUT instance: 0 (SAI)
  *          - Audio OUT Device  : AUDIO_OUT_DEVICE_HEADPHONE
  * @retval None
  */
void REC_INSTANCE2_PDM_demo(void)
{
  uint32_t channel_nbr = 2;

  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  /* Set Audio Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 90, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD SAI PDM EXAMPLE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 24, (uint8_t *)"Make sure the SW2 is in position PDM ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 40,  (uint8_t *)"Press User button for next menu", CENTER_MODE);
  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 100, x_size - 20, y_size - 110, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 101, x_size - 22, y_size - 112, UTIL_LCD_COLOR_BLUE);

  AudioFreq_ptr = AudioFreq+2; /* AUDIO_FREQUENCY_16K; */

  AudioOutInit.Device = AUDIO_OUT_DEVICE_AUTO;
  AudioOutInit.ChannelsNbr = channel_nbr;
  AudioOutInit.SampleRate = *AudioFreq_ptr;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = VolumeLevel;

  AudioInInit.Device = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInInit.ChannelsNbr = channel_nbr;
  AudioInInit.SampleRate = *AudioFreq_ptr;
  AudioInInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInInit.Volume = VolumeLevel;

  BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);

  /* Initialize Audio Recorder with 2 channels to be used */
  BSP_AUDIO_IN_Init(2, &AudioInInit);
  BSP_AUDIO_IN_GetState(2, &InState);

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_HEADPHONE);

  /* Start Recording */
  UTIL_LCD_DisplayStringAt(0, 190, (uint8_t *)"Start Recording ", CENTER_MODE);
  BSP_AUDIO_IN_RecordPDM(2, (uint8_t*)&recordPDMBuf, 2*AUDIO_IN_PDM_BUFFER_SIZE);

  /* Play the recorded buffer*/
  UTIL_LCD_DisplayStringAt(0, 220, (uint8_t *)"Play the recorded buffer... ", CENTER_MODE);
  BSP_AUDIO_OUT_Play(0, (uint8_t*)&RecPlayback[0], 2*AUDIO_BUFF_SIZE);

  while (1)
  {

    if (CheckForUserInput() > 0)
    {

      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      BSP_AUDIO_IN_Stop(2);
      BSP_AUDIO_IN_DeInit(2);
      PlaybackStarted = 0;
      return;
    }
  }
}

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
void REC_INSTANCE1_SingleBuff_demo(void)
{
  uint32_t channel_nbr = 2;
  uint32_t y_size, x_size;
   TouchScreenDemo=1;
   audio_Control=1;
  ButtonState = 0;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  AudioLoopback_SetHint(0);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy = 0;

  BSP_TS_Init(0, &hTS);
  BSP_TS_EnableIT(0);
  Touchscreen_DrawBackground_Circles(32);

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

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  /* Initialize Audio Recorder with 2 channels to be used */
  BSP_AUDIO_IN_Init(1, &AudioInInit);
  BSP_AUDIO_IN_GetState(1, &InState);

  BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_HEADPHONE);

  /* Start Recording */
  BSP_AUDIO_IN_Record(1, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFF_SIZE);

  while (1)
  {
    if(AudioBufferOffset == BUFFER_OFFSET_FULL)
    {
      if(PlaybackStarted == 0)
      {
        /* Play the recorded buffer*/
        BSP_AUDIO_OUT_Play(0, (uint8_t*)&PlayBuffer[0], 2*AUDIO_BUFF_SIZE);
        BSP_AUDIO_OUT_GetState(0, &AudioOutState);
        BSP_AUDIO_IN_GetState(1, &AudioInState);
        UTIL_LCD_SetFont(&Font16);
        sprintf((char*)text, "FREQ: %6lu   %s    VOL:    %3lu", *AudioFreq_ptr, Audio_In_State[AudioInState], VolumeLevel);
        UTIL_LCD_DisplayStringAt(0, y_size - 250, (uint8_t *)text, CENTER_MODE);
        sprintf((char*)text, "            %s               ", Audio_Out_State[AudioOutState]);
        UTIL_LCD_DisplayStringAt(0, y_size - 234, (uint8_t *)text, CENTER_MODE);
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
      case 1:
        /* Decrease Frequency */
        if (*AudioFreq_ptr != 96000)
        {
          AudioFreq_ptr++;
          BSP_AUDIO_OUT_Stop(0);
          BSP_AUDIO_IN_Stop(1);
          BSP_AUDIO_OUT_SetSampleRate(0, *AudioFreq_ptr);
          BSP_AUDIO_IN_SetSampleRate(1, *AudioFreq_ptr);
          BSP_AUDIO_IN_Record(1, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFF_SIZE);
          HAL_Delay(40);
          PlaybackStarted = 0;
          sprintf((char*)text, "FREQ: %6lu   %s    VOL:    %3lu", *AudioFreq_ptr, Audio_In_State[AudioInState], VolumeLevel);
          UTIL_LCD_DisplayStringAt(0, y_size - 250, (uint8_t *)text, CENTER_MODE);
        }
        break;
      case 2:
        /* Increase Frequency */
        if (*AudioFreq_ptr != 8000)
        {
          AudioFreq_ptr--;
          sprintf((char*)FreqStr, "      FREQ: %lu     ", *AudioFreq_ptr);
          BSP_AUDIO_OUT_Stop(0);
          BSP_AUDIO_IN_Stop(1);
          BSP_AUDIO_OUT_SetSampleRate(0, *AudioFreq_ptr);
          BSP_AUDIO_IN_SetSampleRate(1, *AudioFreq_ptr);
          BSP_AUDIO_IN_Record(1, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFF_SIZE);
          HAL_Delay(40);
          PlaybackStarted = 0;
          sprintf((char*)text, "FREQ: %6lu   %s    VOL:    %3lu", *AudioFreq_ptr, Audio_In_State[AudioInState], VolumeLevel);
          UTIL_LCD_DisplayStringAt(0, y_size - 250, (uint8_t *)text, CENTER_MODE);
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
      BSP_AUDIO_IN_Stop(1);
      BSP_AUDIO_IN_DeInit(1);
      BSP_TS_DisableIT(0);
      BSP_TS_DeInit(0);
      PlaybackStarted = 0;
      TouchScreenDemo=0;
      audio_Control=0;
      return;
    }
  }
}

/**
  * @brief  Audio Record and Playback using digital mic.
  *         Record:
  *          - Audio IN instance: 0 SAI
  *          - Audio IN Device  : AUDIO_IN_DEVICE_DIGITAL_MIC
  *          - Audio IN number of channel  : 2
  *         Playback:
  *          - Audio OUT instance: 0 (SAI)
  *          - Audio OUT Device  : AUDIO_OUT_DEVICE_HEADPHONE
  * @retval None
  */
void AudioRec_demo (void)
{
  uint32_t  block_number;
  uint32_t y_size;

  BSP_LCD_GetYSize(0, &y_size);

  AudioRec_SetHint();

  AudioOutInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioOutInit.ChannelsNbr = 2;
  AudioOutInit.SampleRate = AUDIO_FREQUENCY_16K;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = 100;

  AudioInInit.Device = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInInit.ChannelsNbr = 2;
  AudioInInit.SampleRate = AUDIO_FREQUENCY_16K;
  AudioInInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInInit.Volume = 80;

  /* Initialize Audio Recorder */
  if (BSP_AUDIO_IN_Init(0, &AudioInInit) == BSP_ERROR_NONE)
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"  AUDIO RECORD INIT OK  ", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  AudioBufferOffset = BUFFER_OFFSET_NONE;

  /* Display the state on the screen */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)"       RECORDING...     ", CENTER_MODE);

  /* Start Recording: Size in number of bytes */
  BSP_AUDIO_IN_Record(0, (uint8_t*)&internal_buffer, 2*AUDIO_BLOCK_SIZE);

  for (block_number = 0; block_number < AUDIO_NB_BLOCKS; block_number++)
  {
    /* Wait end of half block recording */
    while(AudioBufferOffset != BUFFER_OFFSET_HALF)
    {
      if (CheckForUserInput() > 0)
      {
        ButtonState = 0;
        /* Stop Recorder before close Test */
        BSP_AUDIO_IN_Stop(0);
        return;
      }
    }
    AudioBufferOffset = BUFFER_OFFSET_NONE;
    /* Copy recorded 1st half block in SDRAM */
    memcpy((uint32_t *)(AUDIO_REC_START_ADDR + (block_number * AUDIO_BLOCK_SIZE * 2)),
           internal_buffer,
           AUDIO_BLOCK_SIZE);

    /* Wait end of one block recording */
    while(AudioBufferOffset != BUFFER_OFFSET_FULL)
    {
      if (CheckForUserInput() > 0)
      {
        ButtonState = 0;
        /* Stop Recorder before close Test */
        BSP_AUDIO_IN_Stop(0);
        return;
      }
    }
    AudioBufferOffset = BUFFER_OFFSET_NONE;
    /* Copy recorded 2nd half block in SDRAM */
    memcpy((uint32_t *)(AUDIO_REC_START_ADDR + (block_number * AUDIO_BLOCK_SIZE * 2) + (AUDIO_BLOCK_SIZE)),
           (uint16_t *)(&internal_buffer[AUDIO_BLOCK_SIZE/2]),
           AUDIO_BLOCK_SIZE);
  }

  /* Stop recorder */
  BSP_AUDIO_IN_Stop(0);
  BSP_AUDIO_IN_DeInit(0);

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, y_size - 65, (uint8_t *)"RECORDING DONE, START PLAYBACK...", CENTER_MODE);

  /* -----------Start Playback -------------- */
  /* Initialize audio IN at REC_FREQ*/
  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  /* Play the recorded buffer*/
  AUDIO_Start((uint32_t *)AUDIO_REC_START_ADDR, AUDIO_BLOCK_SIZE * AUDIO_NB_BLOCKS * 2);
  UTIL_LCD_DisplayStringAt(0, y_size - 40, (uint8_t *)"PLAYBACK DONE", CENTER_MODE);

  while (1)
  {
    AUDIO_Process();

    if (CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      return;
    }
  }
}

/**
  * @brief  Manages the DMA Full Transfer complete interrupt.
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  if(Instance == 2U)
  {
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t *)&recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE/2], AUDIO_IN_PDM_BUFFER_SIZE*2);

    BSP_AUDIO_IN_PDMToPCM(Instance, (uint16_t*)&recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE/2], &RecPlayback[playbackPtr]);

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&RecPlayback[playbackPtr], AUDIO_IN_PDM_BUFFER_SIZE/4);

    playbackPtr += AUDIO_IN_PDM_BUFFER_SIZE/4/2;
    if(playbackPtr >= AUDIO_BUFF_SIZE)
      playbackPtr = 0;
  }
  else
  {
    AudioBufferOffset = BUFFER_OFFSET_FULL;
  }
}


/**
* @brief  Manages the DMA Half Transfer complete interrupt.
* @retval None
*/
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  if(Instance == 2U)
  {
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t *)&recordPDMBuf[0], AUDIO_IN_PDM_BUFFER_SIZE*2);

    BSP_AUDIO_IN_PDMToPCM(Instance, (uint16_t*)&recordPDMBuf[0], &RecPlayback[playbackPtr]);

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&RecPlayback[playbackPtr], AUDIO_IN_PDM_BUFFER_SIZE/4);

    playbackPtr += AUDIO_IN_PDM_BUFFER_SIZE/4/2;
    if(playbackPtr >= AUDIO_BUFF_SIZE)
    {
      playbackPtr = 0;
    }
  }
  else
  {
    AudioBufferOffset = BUFFER_OFFSET_HALF;
  }
}

/**
* @brief  Display Audio Record demo hint
* @param  None
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
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font16);

  if(Index == 0)
  {
    UTIL_LCD_DisplayStringAt(0, 5, (uint8_t *)"DFSDM RECORDING & SAI PLAYING in Single buffer mode ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *) "Use touch screen to Switch frequency    ", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(0, 5, (uint8_t *)"DFSDM RECORDING & SAI PLAYING in Multi buffer mode ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *) "Use touch screen to Pause/Resume MIC1/MIC2    ", CENTER_MODE);
  }
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"       Press TAMPER button for next menu          ", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size - 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size - 102, UTIL_LCD_COLOR_BLUE);
}

/**
* @brief  Display Audio Record demo hint
* @param  None
* @retval None
*/
static void AudioRec_SetHint(void)
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
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD EXAMPLE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 24, (uint8_t *)"Make sure the SW2 is in position CODEC ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 40,  (uint8_t *)"Press User button for next menu", CENTER_MODE);
  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 100, x_size - 20, y_size - 110, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 101, x_size - 22, y_size - 112, UTIL_LCD_COLOR_BLUE);

}

/**
* @}
*/

/**
* @}
*/

