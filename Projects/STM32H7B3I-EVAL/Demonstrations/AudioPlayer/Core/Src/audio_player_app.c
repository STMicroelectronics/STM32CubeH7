/**
  ******************************************************************************
  * @file    audio_player_app.c
  * @author  MCD Application Team
  * @brief   Audio player application functions
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
#include "audio_player_app.h"
#include "audio_lowpower.h"
#include "cmsis_os.h"

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

AUDIO_ProcessTypdef haudio;
static osMessageQId   AudioEvent = 0;
static osThreadId     AudioThreadId = 0;
osMessageQId   LPModeEvent = 0;
extern FIL AudioFile;
extern uint32_t OutDecPacketSizeBytes;

/* Private function prototypes -----------------------------------------------*/
static void AUDIOPLAYER_Thread(void const * argument);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Initializes audio
* @param  Volume:Volume level to be set in percentage from 0% to 100% (0 for
*         Mute and 100 for Max volume level).
* @retval Audio error.
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_Init(uint8_t Volume)
{
  BSP_AUDIO_Init_t AudioPlayInit;

  /* Allocates memory for audio data*/
  if( PLAYER_MEMPOOL_Init() != AUDIO_ERROR_NONE)
  {
    return AUDIO_ERROR_MEM;
  }

  AudioPlayInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioPlayInit.ChannelsNbr = 2;
  AudioPlayInit.SampleRate = AUDIO_FREQUENCY_48K;
  AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit.Volume = Volume;


  if( BSP_AUDIO_OUT_Init(AUDIO_OUT_INSTANCE, &AudioPlayInit) != AUDIO_ERROR_NONE)
  {
    return AUDIO_ERROR_MEM;
  }

  /* Initialize internal audio structure */
  haudio.state  = AUDIOPLAYER_STOP;
  haudio.mute   = 0x0;
  haudio.volume = Volume;
  haudio.freq   = AUDIO_FREQUENCY_48K;

  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 4, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL);
  vQueueAddToRegistry(AudioEvent, "AUDIO_Queue");

  /* Create Audio task */
  osThreadDef(osAudio_Thread, AUDIOPLAYER_Thread, osPriorityAboveNormal, 0, 2*AUDIO_THREAD_STACK_SIZE);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);
  
  /* Create LP mode Queue */
  osMessageQDef(LPMode_Queue, 4, uint16_t);
  LPModeEvent = osMessageCreate (osMessageQ(LPMode_Queue), NULL);
  vQueueAddToRegistry(LPModeEvent, "LPMode_Queue");

#ifdef AUDIO_LOWPOWER_SUPPORT
  osThreadDef(Audio_LPMode, AUDIOLPModeThread, osPriorityBelowNormal, 0, 2*AUDIO_THREAD_STACK_SIZE);
  osThreadCreate (osThread(Audio_LPMode), NULL);
#endif /* AUDIO_LOWPOWER_SUPPORT */
  
  /* Enables and resets CRC-32 from STM32 HW */
  __HAL_RCC_CRC_CLK_ENABLE();
  CRC->CR = CRC_CR_RESET;

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Get audio state
* @param  None.
* @retval Audio state.
*/
AUDIOPLAYER_StateTypedef AUDIOPLAYER_GetState(void)
{
  return haudio.state;
}

/**
* @brief  Get audio volume
* @param  None.
* @retval Audio volume.
*/
uint32_t AUDIOPLAYER_GetVolume(void)
{
  return haudio.volume;
}

/**
* @brief  Set audio volume
* @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for
*         Mute and 100 for Max volume level).
* @retval Audio error.
*/
AUDIO_ErrorTypeDef AUDIOPLAYER_SetVolume(uint32_t Volume)
{
  if(BSP_AUDIO_OUT_SetVolume(AUDIO_OUT_INSTANCE,Volume) == BSP_ERROR_NONE)
  {
    haudio.volume = Volume;
    return AUDIO_ERROR_NONE;
  }
  else
  {
    return AUDIO_ERROR_SRC;
  }
}

/**
* @brief  Play audio stream
* @param  file_name: audio file name
*         Volume: Volume level to be set in percentage from 0% to 100% (0 for
*         Mute and 100 for Max volume level).
* @retval Audio error.
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_Play(char *file_name, uint32_t Volume)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;
  
  haudio.volume = Volume;
  if(file_name != NULL)
  {
    ErrorCode = PLAYER_Start(file_name, Volume);
    if(ErrorCode == AUDIO_ERROR_NONE)
    {
      haudio.state  = AUDIOPLAYER_PLAY;
    }
  }
  return ErrorCode;
}

/**
* @brief  Audio player DeInit
* @param  None.
* @retval Audio error.
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_DeInit(void)
{
  haudio.state = AUDIOPLAYER_STOP;

  BSP_AUDIO_OUT_Stop(AUDIO_OUT_INSTANCE);
  BSP_AUDIO_OUT_DeInit(AUDIO_OUT_INSTANCE);

  PLAYER_Close();

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
  return AUDIO_ERROR_NONE;
}

/**
* @brief  Stop audio player.
* @param  None.
* @retval Audio error.
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_Stop(void)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;

  if(BSP_AUDIO_OUT_Stop(AUDIO_OUT_INSTANCE) == BSP_ERROR_NONE)
  {
    haudio.state = AUDIOPLAYER_STOP;
  }
  else
  {
    ErrorCode = AUDIO_ERROR_CODEC;
  }
  PLAYER_Close();

  return ErrorCode;
}


/**
* @brief  Pause Audio player
* @param  None.
* @retval Audio error.
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_Pause(void)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;

  if(BSP_AUDIO_OUT_Pause(AUDIO_OUT_INSTANCE) == BSP_ERROR_NONE)
  {
    haudio.state = AUDIOPLAYER_PAUSE;
  }
  else
  {
    ErrorCode = AUDIO_ERROR_CODEC;
  }

  return ErrorCode;
}

/**
* @brief  Resume Audio player
* @param  None.
* @retval Audio error.
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_Resume(void)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;

  if(BSP_AUDIO_OUT_Resume(AUDIO_OUT_INSTANCE)== BSP_ERROR_NONE)
  {
    haudio.state = AUDIOPLAYER_PLAY;
  }
  else
  {
    ErrorCode = AUDIO_ERROR_CODEC;
  }
  return ErrorCode;
}

/**
* @brief  Mute audio player
* @param  Instance AUDIO OUT Instance. It can only be 0 (SAI) or 1 (I2S)
* @retval Audio error.
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_Mute(uint32_t Instance)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;

  if(BSP_AUDIO_OUT_Mute(Instance)== BSP_ERROR_NONE)
  {
    haudio.mute = 0x1;
  }
  else
  {
    ErrorCode = AUDIO_ERROR_CODEC;
  }

  return ErrorCode;
}

/**
* @brief  UnMute audio player
* @param  Instance AUDIO OUT Instance. It can only be 0 (SAI) or 1 (I2S)
* @retval Audio error.
*/
AUDIO_ErrorTypeDef AUDIOPLAYER_UnMute(uint32_t Instance)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;

  if(BSP_AUDIO_OUT_UnMute(Instance) == BSP_ERROR_NONE)
  {
    haudio.mute = 0x0;
  }
  else
  {
    ErrorCode = AUDIO_ERROR_CODEC;
  }

  return ErrorCode;
}


/**
* @brief  Get audio file progress.
* @param  None
* @retval file progress value.
*/
uint32_t AUDIOPLAYER_GetProgress(void)
{
  uint32_t progress = 0;

  /* Get the elapsed duration */
  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
  {
    PLAYER_GetTimeInfo(NULL, &progress);
  }

  return progress;
}

/**
  * @brief  Get audio file duration.
  * @param  None
  * @retval file duration value.
  */
uint32_t AUDIOPLAYER_GetDuration(void)
{
  uint32_t duration = 0;

  /* Get the total stream duration */
  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
  {
    PLAYER_GetTimeInfo(&duration, NULL);
  }

  return duration;
}

/**
  * @brief  Get the wav file information.
  * @param  file_name: audio file name
  * @param  info: pointer to audio file structure
  * @retval Audio error.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_GetFileInfo(char* file_name, Audio_InfoTypeDef* info)
{
  return PLAYER_GetFileInfo(file_name, info);
}

/**
  * @brief  Sets audio stream position
  * @param  SeekTime: stream position.
  * @retval Audio error.
  */
uint32_t  AUDIOPLAYER_SeekToTime(uint32_t SeekTime)
{
  uint32_t ret = 0;
  
  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
  {
    ret = PLAYER_SeekToTime(SeekTime);
  }
  
  return ret;
}

/**
 * @brief  Manages the DMA full Transfer complete event
 * @param  Instance:Instance AUDIO OUT Instance. It can only be 0 (SAI) or 1 (I2S)
 * @retval None
 */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  if(haudio.state == AUDIOPLAYER_PLAY)
  {
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_FULL, 0);
  }
}


/**
 * @brief  Manages the DMA Half Transfer complete event
 * @param  Instance:Instance AUDIO OUT Instance. It can only be 0 (SAI) or 1 (I2S)
 * @retval None
 */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  if(haudio.state == AUDIOPLAYER_PLAY)
  {
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_HALF, 0);
  }

}

/**
  * @brief  Manages the DMA FIFO error event
  * @param  Instance AUDIO OUT Instance. It can only be 0 (SAI) or 1 (I2S)
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  haudio.state = AUDIOPLAYER_ERROR;
}

/**
 * @brief  Audio player task
 * @param  argument: pointer that is passed to the thread function as start argument.
 * @retval None
 */
static void AUDIOPLAYER_Thread(void const * argument)
{
  osEvent event;
  int32_t MemPoolsizeAvailable;

  for(;;)
  {
    event = osMessageGet(AudioEvent, osWaitForever );

    if( event.status == osEventMessage )
    {
      if(haudio.state == AUDIOPLAYER_PLAY)
      {
        if(f_eof(&AudioFile))
        {
          AUDIOPLAYER_NotifyEndOfFile();
        }
        else
        {
          MemPoolsizeAvailable = PLAYER_GetAvailableSizeMemPool();

          if(MemPoolsizeAvailable >= OutDecPacketSizeBytes)
          {
              /* Loop1 processing*/
              PLAYER_TransfertToMemPool();
          }

          switch(event.value.v)
          {
          case PLAY_BUFFER_OFFSET_HALF:
              PLAYER_OutputBufferCbkProc(0); /* in samples */
             break;

          case PLAY_BUFFER_OFFSET_FULL:
              PLAYER_OutputBufferCbkProc(AUDIO_OUT_BUFFER_SIZE/2); /* in samples */
            break;

          default:
            break;
          }
        }
      }

    }
  }
}

/**
 * @brief  Notify end of audio file.
 * @param  None
 * @retval Audio error
*/
__weak AUDIO_ErrorTypeDef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  AUDIO_ErrorTypeDef ret = AUDIO_ERROR_NONE;   
  
  return ret;
}

/**
* @brief  Notify audio player error.
* @param  None
* @retval Audio error
*/
__weak AUDIO_ErrorTypeDef  AUDIOPLAYER_NotifyError(void)
{
  return AUDIOPLAYER_Stop();
}

/**
* @}
*/

/**
* @}
*/

