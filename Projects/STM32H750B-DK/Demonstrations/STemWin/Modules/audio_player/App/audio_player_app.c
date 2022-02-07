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

/** @addtogroup AUDIO_PLAYER_APP
  * @{
  */

/** @defgroup PLAYER_APPLICATION
  * @brief audio application routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup PLAYER_APPLICATION_Private_Variables
* @{
*/
static PLAYER_HANDLE_h hPlayerHandle = 0;
static char FileName[MAX_FILE_NAME_LENGHT];

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PLAYER_APPLICATION_Exported_Functions
* @{
*/
/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_Init(uint8_t volume)
{
  memset(FileName, 0, sizeof(FileName));
  return PLAYER_Init(volume, &hPlayerHandle);
}

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
void AUDIOPLAYER_RegisterCallbacks(fnUserCallback_TypeDef*      pPrePrcessingCallback,
                                   fnUserCallback_TypeDef*      pPostPrcessingCallback,
                                   fnEndOfFileCallback_TypeDef* pEndOfFileCallback)
{
  PLAYER_RegisterCallbacks(hPlayerHandle, pPrePrcessingCallback , pPostPrcessingCallback, pEndOfFileCallback);
}

/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIO_StateTypeDef  AUDIOPLAYER_GetState(void)
{
  return PLAYER_GetState(hPlayerHandle);
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AUDIOPLAYER_GetVolume(void)
{
  return PLAYER_GetVolume(hPlayerHandle);
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_SetVolume(uint32_t volume)
{
  return PLAYER_SetVolume(hPlayerHandle, volume);
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency used to play the audio stream.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_Play(uint32_t frequency)
{
  AUDIO_ErrorTypeDef Error = AUDIO_ERROR_INVALID_PARAMS;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(frequency);

  if((strlen(FileName) > 0) && (strcmp(FileName, "") != 0))
  {
    Error = PLAYER_Start(hPlayerHandle, FileName);
  }

  return Error;
}

/**
  * @brief  Audio player DeInit
  * @param  None.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_DeInit(void)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_INVALID_PARAMS;

  ErrorCode = PLAYER_DeInit(hPlayerHandle);
  if(ErrorCode == AUDIO_ERROR_NONE)
  {
    memset(FileName, 0, sizeof(FileName));
    hPlayerHandle = 0;
  }

  return ErrorCode;
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_Stop(void)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;

  if((strlen(FileName) > 0) && (strcmp(FileName, "") != 0))
  {
    memset(FileName, 0, sizeof(FileName));
    ErrorCode = PLAYER_Stop(hPlayerHandle);
  }

  return ErrorCode;
}


/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_Pause(void)
{
  return PLAYER_SetConfig(hPlayerHandle, AUDIO_CFG_PAUSE);
}


/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_Resume(void)
{
  return PLAYER_SetConfig(hPlayerHandle, AUDIO_CFG_RESUME);
}
/**
  * @brief  Sets audio stream position
  * @param  position: stream position.
  * @retval Audio state.
  */
uint32_t  AUDIOPLAYER_SeekToTime(uint32_t time)
{
  return PLAYER_SeekToTime(hPlayerHandle, time);
}

/**
  * @brief  Sets the volume at mute
  * @param  state: could be MUTE_ON to mute sound or MUTE_OFF to unmute 
  *                the codec and restore previous volume level.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_Mute(uint8_t state)
{
  AUDIO_ErrorTypeDef Error = AUDIO_ERROR_INVALID_PARAMS;

  if(state == MUTE_ON)
  {
    Error = PLAYER_SetConfig(hPlayerHandle, AUDIO_CFG_MUTE_ON);
  }
  else
  {
    Error = PLAYER_SetConfig(hPlayerHandle, AUDIO_CFG_MUTE_OFF);
  }
   
   return Error;
}

/**
  * @brief  Get the wav file information.
  * @param  file: wav file.
  * @param  info: pointer to wav file structure
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_GetFileInfo(char* file, AUDIO_FILE_InfoTypeDef* info)
{
  return PLAYER_GetFileInfo(hPlayerHandle, file, info);
}

/**
  * @brief  Select wav file.
  * @param  file: wav file.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  AUDIOPLAYER_SelectFile(char* file)
{
  AUDIO_ErrorTypeDef Error = AUDIO_ERROR_INVALID_PARAMS;

  Error = PLAYER_SelectFile(hPlayerHandle, file);
  if(Error  == AUDIO_ERROR_NONE)
  {
    sprintf(FileName, file);    
  }
    
  return Error;
}

/**
  * @brief  Get audio file progress.
  * @param  None
  * @retval file progress.
  */
uint32_t AUDIOPLAYER_GetProgress(void)
{
  uint32_t progress = 0;

  /* Get the elapsed duration */
  PLAYER_GetTimeInfo(hPlayerHandle, NULL, &progress);

  return progress;
}

/**
  * @brief  Get audio file duration.
  * @param  None
  * @retval file progress.
  */
uint32_t AUDIOPLAYER_GetDuration(void)
{
  uint32_t duration = 0;

  /* Get the total stream duration */
  PLAYER_GetTimeInfo(hPlayerHandle, &duration, NULL);

  return duration;
}

/**
  * @brief  Notify the end of audio file.
  * @param  None.
  * @retval Audio state.
  */
__weak void  AUDIOPLAYER_NotifyEndOfFile(void)
{
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

