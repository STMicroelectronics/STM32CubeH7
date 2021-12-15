/**
  ******************************************************************************
  * @file    audioplayer_app.c
  * @author  MCD Application Team
  * @brief   Audio player application functions
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
#include "audio_player_app.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t visualization_samples[64];
BSP_AUDIO_Init_t AudioPlayInit;

static FIL wav_file;
static osMessageQId   AudioEvent = 0;
static osThreadId     AudioThreadId = 0;

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);
static void AUDIO_TransferComplete_CallBack(void);
static void AUDIO_HalfTransfer_CallBack(void);
static void AUDIO_Error_CallBack(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume)
{
  portENTER_CRITICAL();
  
  /* Try to Init Audio interface in different config in case of failure */
  AudioPlayInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioPlayInit.ChannelsNbr = 2;
  AudioPlayInit.SampleRate = AUDIO_FREQUENCY_48K;
  AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit.Volume = volume;
  
  BSP_AUDIO_OUT_Init(0, &AudioPlayInit);

  
  /* Initialize internal audio structure */
  haudio.out.state  = AUDIOPLAYER_STOP;
  haudio.out.mute   = MUTE_OFF;
  haudio.out.volume = volume;  

  /* Register audio BSP drivers callbacks */
  AUDIO_IF_RegisterCallbacks(AUDIO_TransferComplete_CallBack, 
                             AUDIO_HalfTransfer_CallBack, 
                             AUDIO_Error_CallBack);
    
    
  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudio_Thread, Audio_Thread, osPriorityRealtime, 0, 4096);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);
  
  portEXIT_CRITICAL();

  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void)
{
  return haudio.out.state;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AUDIOPLAYER_GetVolume(void)
{
  return haudio.out.volume;
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume)
{
  if(BSP_AUDIO_OUT_SetVolume(0,volume) == BSP_ERROR_NONE)
  {
    haudio.out.volume = volume;
    return AUDIOPLAYER_ERROR_NONE;    
  }
  else
  {
    return AUDIOPLAYER_ERROR_HW;
  }
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency used to play the audio stream.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Play(uint32_t frequency)
{
  uint32_t numOfReadBytes;
  haudio.out.state = AUDIOPLAYER_PLAY;
  
    /* Fill whole buffer @ first time */
    if(f_read(&wav_file, 
              &haudio.buff[0], 
              AUDIO_OUT_BUFFER_SIZE, 
              (void *)&numOfReadBytes) == FR_OK)
    { 
      if(numOfReadBytes != 0)
      {
        BSP_AUDIO_OUT_Stop(0);
        BSP_AUDIO_OUT_SetSampleRate(0, frequency);
        memset(haudio.buff, 0, AUDIO_OUT_BUFFER_SIZE);
        BSP_AUDIO_OUT_Play(0,(uint8_t*)&haudio.buff[0], AUDIO_OUT_BUFFER_SIZE);
       
        return AUDIOPLAYER_ERROR_NONE;
      }
    }
  return AUDIOPLAYER_ERROR_IO;

}

/**
  * @brief  Audio player DeInit
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_DeInit(void)
{

  BSP_AUDIO_OUT_Stop(0);
  BSP_AUDIO_OUT_DeInit(0);

  f_close(&wav_file); 
  
  if(AudioEvent != 0)
  {
    vQueueDelete(AudioEvent); 
    AudioEvent = 0;
  }
  if(AudioThreadId != 0)
  {
    osThreadTerminate(AudioThreadId);
    AudioThreadId = 0;
  }
   
  
  return AUDIOPLAYER_ERROR_NONE;  
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void)
{
  haudio.out.state = AUDIOPLAYER_STOP;
  BSP_AUDIO_OUT_Stop(0); 
  f_close(&wav_file);
  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void)
{
  BSP_AUDIO_OUT_Pause(0);
  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void)
{
  BSP_AUDIO_OUT_Resume(0);
  return AUDIOPLAYER_ERROR_NONE;
}
/**
  * @brief  Sets audio stream position
  * @param  position: stream position.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetPosition(uint32_t position)
{
  long file_pos;
  
  file_pos = (f_size(&wav_file)) / 128 / 100; 
  file_pos *= (position * 128); 
  f_lseek(&wav_file, file_pos);
  
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Sets the volume at mute
  * @param  state: could be MUTE_ON to mute sound or MUTE_OFF to unmute 
  *                the codec and restore previous volume level.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Mute(uint8_t state)
{
   BSP_AUDIO_OUT_Mute(0);
   
   return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Get the wav file information.
  * @param  file: wav file.
  * @param  info: pointer to wav file structure
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_GetFileInfo(char* file, WAV_InfoTypedef* info)
{
  uint32_t numOfReadBytes;
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;
  FIL fsfile;
  
  if( f_open(&fsfile, file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    /* Fill the buffer to Send */
    if(f_read(&fsfile, info, sizeof(WAV_InfoTypedef), (void *)&numOfReadBytes) == FR_OK)
    {
      if((info->ChunkID == 0x46464952) && (info->AudioFormat == 1))
      {
        ret = AUDIOPLAYER_ERROR_NONE;
      }
    }
    f_close(&fsfile);      
  }
  return ret;
}

/**
  * @brief  Select wav file.
  * @param  file: wav file.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SelectFile(char* file)
{
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;
  if( f_open(&wav_file, file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    f_lseek(&wav_file, sizeof(WAV_InfoTypedef));
    ret = AUDIOPLAYER_ERROR_NONE;
  }
  return ret;  
}

/**
  * @brief  Get wav file progress.
  * @param  None
  * @retval file progress.
  */
uint32_t AUDIOPLAYER_GetProgress(void)
{
  return (wav_file.fptr);
}

/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_TransferComplete_CallBack(void)
{
  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_FULL, 0);    
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_HalfTransfer_CallBack(void)
{ 
  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_HALF, 0);    
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_Error_CallBack(void)
{
  haudio.out.state = AUDIOPLAYER_ERROR;
}

/**
  * @brief  Audio task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Audio_Thread(void const * argument)
{
  uint32_t numOfReadBytes;    
  osEvent event;  
  int16_t *samples;
  int i, sample;

  for(;;)
  {
    event = osMessageGet(AudioEvent, 100 );
    
    if( event.status == osEventMessage )
    {
      if(haudio.out.state == AUDIOPLAYER_PLAY)
      {
        switch(event.value.v)
        {
          case PLAY_BUFFER_OFFSET_HALF:
            if(f_read(&wav_file, 
                    &haudio.buff[0], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
            {
              if(numOfReadBytes == 0)
              {
                AUDIOPLAYER_NotifyEndOfFile();
              }
              else
              {
                samples = (int16_t *)&haudio.buff[AUDIO_OUT_BUFFER_SIZE / 2];
                for (int i = 0; i < 64; i++) 
                {
                    sample = ((*samples + 32768) >> 9); /* 0, 128 */ 
                    visualization_samples[i] = (uint8_t)sample;
                    samples += 2;
                }
              }
            }
            else
            {
              AUDIOPLAYER_Stop();
            }
            break;
          case PLAY_BUFFER_OFFSET_FULL:
            if(f_read(&wav_file, 
                    &haudio.buff[AUDIO_OUT_BUFFER_SIZE/2], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
            {
              if(numOfReadBytes == 0)
              { 
                AUDIOPLAYER_NotifyEndOfFile();
              }
              else
              {
                samples = (int16_t *)&haudio.buff[0];
                for (i = 0; i < 64; i++) 
                {
                  sample = ((*samples + 32768) >> 9); /* 0, 128 */
                  visualization_samples[i] = (uint8_t)sample;
                  samples += 2;
                }
              }
            }
            else
            {
              AUDIOPLAYER_Stop();
            }
            break;
          default:
            break;
        }
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

