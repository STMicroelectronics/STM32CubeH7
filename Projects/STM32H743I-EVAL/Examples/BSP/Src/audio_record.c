/**
  ******************************************************************************
  * @file    BSP/Src/audio_record.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DFSDM HAL API to realize
  *          audio recording.
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
#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)
#define AUDIO_NB_BLOCKS    ((uint32_t)4)
#define AUDIO_BUFF_SIZE  4096
ALIGN_32BYTES (uint16_t  RecPlayback[AUDIO_BUFF_SIZE]);
static uint16_t  internal_buffer[AUDIO_BLOCK_SIZE];
/* Private define ------------------------------------------------------------*/
/* Audio frequency */
#define AUDIO_FREQUENCY            AUDIO_FREQUENCY_16K
#define AUDIO_IN_PDM_BUFFER_SIZE 256
#define AUDIO_IN_INTERFACE_SAI   0
#define AUDIO_IN_INTERFACE_DFSDM 1
#define AUDIO_IN_INTERFACE_PDM   2
/* Size of the recorder buffer */
#define RECORD_BUFFER_SIZE        4096
int32_t  RecordBuffer1[AUDIO_BUFFER_SIZE/2];
int32_t  RecordBuffer2[AUDIO_BUFFER_SIZE/2];
int32_t *test_buffer[] = {RecordBuffer1, RecordBuffer2};
uint16_t  RecBuffer[AUDIO_BUFFER_SIZE];

uint32_t  InState = 0;
uint32_t  OutState = 0;

uint32_t AudioBufferOffset;
uint32_t PlaybackStarted = 0;
BSP_AUDIO_Init_t  AudioInInit;
BSP_AUDIO_Init_t  AudioOutInit;

static uint32_t AudioFreq[8] = {8000 ,11025, 16000, 22050, 32000, 44100, 48000, 96000};
uint32_t *AudioFreq_ptr;
uint32_t VolumeLevel = 100, AudioOutState, AudioInState;
Point PointsAudio[] = {{600, 340}, {660, 380}, {600, 420}};
char text[256];
extern AUDIO_ErrorTypeDef AUDIO_Start(uint32_t *psrc_address, uint32_t file_size);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Define record Buf at D3SRAM @0x38000000 since the BDMA for SAI4 use only this memory */
#if defined ( __CC_ARM )  /* !< ARM Compiler */
  ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
  ALIGN_32BYTES (uint16_t recordBuf[RECORD_BUFFER_SIZE*2]) __attribute__((section(".RAM_D3")));
  ALIGN_32BYTES (uint16_t PlayBuffer[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
  #pragma location=0x38000000
  uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE];
#pragma location=0x38000200
uint16_t  PlayBuffer[AUDIO_BUFFER_SIZE];
  #pragma location=0x38001200
  uint16_t recordBuf[RECORD_BUFFER_SIZE*2];

#elif defined ( __GNUC__ )  /* !< GNU Compiler */
  ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
  ALIGN_32BYTES (uint16_t recordBuf[RECORD_BUFFER_SIZE*2]) __attribute__((section(".RAM_D3")));
  ALIGN_32BYTES (uint16_t PlayBuffer[AUDIO_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
#endif

uint16_t playbackBuf[RECORD_BUFFER_SIZE*2];

/* Pointer to record_data */
uint32_t playbackPtr;


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
  * @brief  Audio Record and Playback using analog mic.
  *         Record:
  *          - Audio IN instance: 2 PDM
  *          - Audio IN Device  : AUDIO_IN_DEVICE_ANALOG_MIC
  *          - Audio IN number of channel  : 1
  *         Playback:
  *          - Audio OUT instance: 0 (SAI)
  *          - Audio OUT Device  : AUDIO_OUT_DEVICE_HEADPHONE
  * @retval None
  */
void REC_PDM_demo(void)
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
  AudioOutInit.Volume = 80;

  AudioInInit.Device = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInInit.ChannelsNbr = channel_nbr;
  AudioInInit.SampleRate = *AudioFreq_ptr;
  AudioInInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInInit.Volume = 80;

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
void REC_SingleBuff_demo(void)
{
  uint32_t channel_nbr = 2;
  uint32_t y_size, x_size;

  ButtonState = 0;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  AudioLoopback_SetHint(0);

  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;
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

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  /* Initialize Audio Recorder with 2 channels to be used */
  BSP_AUDIO_IN_Init(1, &AudioInInit);
  BSP_AUDIO_IN_GetState(1, &InState);

  BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_HEADPHONE);

  /* Start Recording */
  BSP_AUDIO_IN_Record(1, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFFER_SIZE);

  while (1)
  {
    if(AudioBufferOffset == BUFFER_OFFSET_FULL)
    {
      if(PlaybackStarted == 0)
      {
        /* Play the recorded buffer*/
        BSP_AUDIO_OUT_Play(0, (uint8_t*)&PlayBuffer[0], 2*AUDIO_BUFFER_SIZE);
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
      case 3:
        /* Decrease Frequency */
        if (*AudioFreq_ptr != 96000)
        {
          AudioFreq_ptr++;
          BSP_AUDIO_OUT_Stop(0);
          BSP_AUDIO_IN_Stop(1);
          BSP_AUDIO_OUT_SetSampleRate(0, *AudioFreq_ptr);
          BSP_AUDIO_IN_SetSampleRate(1, *AudioFreq_ptr);
          BSP_AUDIO_IN_Record(1, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFFER_SIZE);
          HAL_Delay(40);
          PlaybackStarted = 0;

          sprintf((char*)text, "FREQ: %6lu     %s       VOL:    %3lu", *AudioFreq_ptr, Audio_In_State[AudioInState], VolumeLevel);
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
          BSP_AUDIO_IN_Record(1, (uint8_t*)&PlayBuffer, 2*AUDIO_BUFFER_SIZE);
          HAL_Delay(40);
          PlaybackStarted = 0;

          sprintf((char*)text, "FREQ: %6lu     %s       VOL:    %3lu", *AudioFreq_ptr, Audio_In_State[AudioInState], VolumeLevel);
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
  AudioUpdate =1;
  
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;
  hTS.Width = x_size;
  hTS.Height = y_size;

  /* Touchscreen initialization */
  BSP_TS_Init(0, &hTS);

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetFont(&Font24);

  UTIL_LCD_FillRect(140, 215, 80, 50, UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_FillRect(230, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_FillRect(260, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(150, 230, (uint8_t *)"MIC1", LEFT_MODE);

  UTIL_LCD_FillRect(320, 215, 80, 50, UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_FillRect(410, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_FillRect(440, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(330, 230, (uint8_t *)"MIC2", LEFT_MODE);

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
  BSP_AUDIO_IN_Init(1, &AudioInInit);

  AudioBufferOffset  = BUFFER_OFFSET_NONE;

  /* Start Recording */
  BSP_AUDIO_IN_RecordChannels(1, (uint8_t**)test_buffer, AUDIO_BUFFER_SIZE/2);
  MicPauseResume.Mic1_Paused = 0;
  MicPauseResume.Mic2_Paused = 0;

  while (1)
  {
    if(AudioBufferOffset == BUFFER_OFFSET_HALF)
    {
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RecordBuffer1[0], 2*AUDIO_BUFFER_SIZE);
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RecordBuffer2[0], 2*AUDIO_BUFFER_SIZE);
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
      SCB_CleanDCache_by_Addr((uint32_t *)PlayBuffer,  AUDIO_BUFFER_SIZE);

      if(PlaybackStarted == 0)
      {
        /* Play the recorded buffer*/
        BSP_AUDIO_OUT_Play(0, (uint8_t*)&PlayBuffer[0],AUDIO_BUFFER_SIZE);
        PlaybackStarted = 1;
      }
      AudioBufferOffset  = BUFFER_OFFSET_NONE;
    }
    TS_AudioUpdate();

    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      BSP_AUDIO_IN_StopChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC);
      BSP_AUDIO_IN_DeInit(1);
      BSP_TS_DisableIT(0);
      BSP_TS_DeInit(0);
      PlaybackStarted = 0;

      return;
    }
  }
}

/**
  * @brief  Audio Record and Playback using analog mic.
  *         Record:
  *          - Audio IN instance: 0 SAI
  *          - Audio IN Device  : AUDIO_IN_DEVICE_ANALOG_MIC
  *          - Audio IN number of channel  : 1
  *         Playback:
  *          - Audio OUT instance: 0 (SAI)
  *          - Audio OUT Device  : AUDIO_OUT_DEVICE_HEADPHONE
  * @retval None
  */
void AudioRec_demo (void)
{

  uint32_t  block_number;
  uint32_t x_size, y_size;
  uint16_t x1, y1;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  Point Points2[] = {{x_size/2+105, y_size/2-40}, {x_size/2+175, y_size/2}, {x_size/2+105, y_size/2+40}};

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 10;

  BSP_TS_Init(0, &hTS);
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  /* Set Audio Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 120, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SAI CODEC AUDIO RECORD", CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Put SW2 switch to the Codec position (left position)", CENTER_MODE);

  UTIL_LCD_DrawRect(11, 131, x_size - 22, y_size - 132, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(10, 132, x_size - 20, y_size - 133, UTIL_LCD_COLOR_BLUE);
   /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillCircle(x_size/2-150, y_size/2, 60, UTIL_LCD_COLOR_CYAN);
  UTIL_LCD_FillCircle(x_size/2-150, y_size/2, 60 - 5, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillCircle(x_size/2-150, y_size/2, 40, UTIL_LCD_COLOR_RED);


  UTIL_LCD_FillCircle(x_size/2-10, y_size/2, 60, UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_FillCircle(x_size/2-10, y_size/2, 60 - 5, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(x_size/2-40, y_size/2-40, 25 , 80, UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_FillRect(x_size/2-5, y_size/2-40, 25 , 80, UTIL_LCD_COLOR_GRAY);


  UTIL_LCD_FillCircle(x_size/2+130, y_size/2, 60, UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_FillCircle(x_size/2+130, y_size/2, 60 - 5, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GRAY);

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

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);
  /* Initialize Audio Recorder */
  if (BSP_AUDIO_IN_Init(0, &AudioInInit) != BSP_ERROR_NONE)

  {
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, y_size - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, y_size - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  AudioBufferOffset = BUFFER_OFFSET_NONE;
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, 350, (uint8_t *)" Recording ... ", CENTER_MODE);


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
  /* -----------Start Playback -------------- */
  /* Initialize audio IN at REC_FREQ*/
  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  UTIL_LCD_FillCircle(x_size/2-150, y_size/2, 60, UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_FillCircle(x_size/2-150, y_size/2, 60 - 5, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillCircle(x_size/2-150, y_size/2, 40, UTIL_LCD_COLOR_GRAY);


  UTIL_LCD_FillCircle(x_size/2-10, y_size/2, 60, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillCircle(x_size/2-10, y_size/2, 60 - 5, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(x_size/2-40, y_size/2-40, 25 , 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(x_size/2-5, y_size/2-40, 25 , 80, UTIL_LCD_COLOR_BLUE);


  UTIL_LCD_FillCircle(x_size/2+130, y_size/2, 60, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillCircle(x_size/2+130, y_size/2, 60 - 5, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_BLUE);

  /* Play the recorded buffer*/
  AUDIO_Start((uint32_t*)AUDIO_REC_START_ADDR, AUDIO_BLOCK_SIZE * AUDIO_NB_BLOCKS * 2);
  UTIL_LCD_DisplayStringAt(0, 350, (uint8_t *)" Playing ... ", CENTER_MODE);

  while (1)
  {
    BSP_TS_GetState(0, &TS_State);
    if(TS_State.TouchDetected)
    {
      x1 = TS_State.TouchX;
      y1 = TS_State.TouchY;
      if((((y_size/2)-40)<y1)&&(y1<((y_size/2)+40)))
      {
        if((((x_size/2)-70)<x1)&&(x1<((x_size/2)+50)))
        {
          BSP_AUDIO_OUT_Pause(0);
          UTIL_LCD_DisplayStringAt(0, 350, (uint8_t *)" PAUSE ... ", CENTER_MODE);
        }
        if((((x_size/2)+70)<x1)&&(x1<((x_size/2)+190)))
        {
          BSP_AUDIO_OUT_Resume(0);
          UTIL_LCD_DisplayStringAt(0, 350, (uint8_t *)" Playing ... ", CENTER_MODE);
        }
      }
    }
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
  UTIL_LCD_FillRect(0, 0, x_size, 120, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font16);

  if(Index == 0)
  {
    UTIL_LCD_DisplayStringAt(0, 5, (uint8_t *)"DFSDM RECORDING & SAI PLAYING in Single buffer mode ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 24, (uint8_t *)"Make sure the SW2 is in position PDM ", CENTER_MODE);
    
  }
  else
  {
    UTIL_LCD_DisplayStringAt(0, 5, (uint8_t *)"DFSDM RECORDING & SAI PLAYING in Multi buffer mode ", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *) "Use touch screen to Pause/Resume MIC1/MIC2    ", CENTER_MODE);
  }
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 40,  (uint8_t *)"Press User button for next menu", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 130, x_size - 20, y_size - 130, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 131, x_size - 22, y_size - 132, UTIL_LCD_COLOR_BLUE);
}


/**
  * @brief  Update audio process following TS positions
  * @retval None
  */
void TS_AudioUpdate(void)
{
  uint32_t i;
  Point Points1[3], Points2[3];
  static uint8_t event =0;
  Points1[0].X = 230;
  Points1[0].Y = 215;
  Points1[1].X = 280;
  Points1[1].Y = 240;
  Points1[2].X = 230;
  Points1[2].Y = 265;

  Points2[0].X = 410;
  Points2[0].Y = 215;
  Points2[1].X = 450;
  Points2[1].Y = 240;
  Points2[2].X = 410;
  Points2[2].Y = 265;

  if(AudioUpdate == 1)
  {
    BSP_TS_GetState(0, &TS_State);


    if(TS_State.TouchDetected)
      {
      if(((TS_State.TouchX > 230) && (TS_State.TouchX < 280)) && ((TS_State.TouchY > 215) && (TS_State.TouchY < 265)))
      {
        
        if(MicPauseResume.Mic1_Paused == 0)
        {
          UTIL_LCD_FillRect(230, 215, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(260, 215, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillPolygon(Points1, 3, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_PauseChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          MicPauseResume.Mic1_Paused = 1;
          for(i = 0; i < AUDIO_BUFFER_SIZE/2; i++)
          {
            PlayBuffer[2*i+1] = 0;
          }
        }
        else
        {
          UTIL_LCD_FillPolygon(Points1, 3, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(230, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
          UTIL_LCD_FillRect(260, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_ResumeChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          MicPauseResume.Mic1_Paused = 0;
        }
      }

      if(((TS_State.TouchX > 410) && (TS_State.TouchX < 460)) && ((TS_State.TouchY > 215) && (TS_State.TouchY < 265)))
      {
        HAL_Delay(200);
        event =1;
        if(MicPauseResume.Mic2_Paused == 0)
        {
          
          UTIL_LCD_FillRect(410, 215, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(440, 215, 20, 50, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_PauseChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC2);
          MicPauseResume.Mic2_Paused = 1;
          for(i = 0; i < AUDIO_BUFFER_SIZE/2; i++)
          {
            PlayBuffer[2*i] = 0;
          }
        }
        else
        {
          UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_FillRect(410, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
          UTIL_LCD_FillRect(440, 215, 20, 50, UTIL_LCD_COLOR_GREEN);
          BSP_AUDIO_IN_ResumeChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC2);
          BSP_AUDIO_IN_PauseChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          BSP_AUDIO_IN_ResumeChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          if(MicPauseResume.Mic1_Paused == 1)
          {
            BSP_AUDIO_IN_PauseChannels(1, AUDIO_IN_DEVICE_DIGITAL_MIC1);
          }
          MicPauseResume.Mic2_Paused = 0;
        }
      }
    }

    if((MicPauseResume.Mic1_Paused == 0) && (MicPauseResume.Mic2_Paused == 0))
    {
      sprintf((char*)text, "MIC1: Active        MIC2: Active");
      UTIL_LCD_DisplayStringAt(20, 380, (uint8_t *)text, CENTER_MODE);
    }
    else if((MicPauseResume.Mic1_Paused == 1) && (MicPauseResume.Mic2_Paused == 0)&& event==1)
    {
      sprintf((char*)text, "MIC1: Paused        MIC2: Active");
      UTIL_LCD_DisplayStringAt(20, 380, (uint8_t *)text, CENTER_MODE);
    }
    else if((MicPauseResume.Mic1_Paused == 0) && (MicPauseResume.Mic2_Paused == 1)&& event==1)
    {
      sprintf((char*)text, "MIC1: Active        MIC2: Paused");
      UTIL_LCD_DisplayStringAt(20, 380, (uint8_t *)text, CENTER_MODE);
    }
    else if(event ==1) /*     if((MicPauseResume.Mic1_Paused == 1) && (MicPauseResume.Mic2_Paused == 1)) */
    {
      sprintf((char*)text, "MIC1: Paused         MIC2: Paused");
      UTIL_LCD_DisplayStringAt(20, 380, (uint8_t *)text, CENTER_MODE);
    }
    event=0;
  }
}

/**
  * @brief  Manages the DMA Full Transfer complete interrupt.
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  if(Instance==2)
  {  /* Invalidate Data Cache to get the updated content of the SRAM*/
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
  if(Instance==2)
  {
    /* Invalidate Data Cache to get the updated content of the SRAM*/
   SCB_InvalidateDCache_by_Addr((uint32_t *)&recordPDMBuf[0], AUDIO_IN_PDM_BUFFER_SIZE*2);

    BSP_AUDIO_IN_PDMToPCM(Instance, (uint16_t*)&recordPDMBuf[0], &RecPlayback[playbackPtr]);

    /* Clean Data Cache to update the content of the SRAM */
   SCB_CleanDCache_by_Addr((uint32_t *)&RecPlayback[playbackPtr], AUDIO_IN_PDM_BUFFER_SIZE*2);


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
  * @}
  */

/**
  * @}
  */

