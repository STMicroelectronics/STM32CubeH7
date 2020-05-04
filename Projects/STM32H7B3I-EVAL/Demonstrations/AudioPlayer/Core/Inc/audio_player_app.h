/**
  ******************************************************************************
  * @file    audio_player_app.h
  * @author  MCD Application Team
  * @brief   header of audio player application file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_PLAYER_APP_H
#define __AUDIO_PLAYER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "player.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  AUDIOPLAYER_STOP = 0,
  AUDIOPLAYER_START,
  AUDIOPLAYER_PLAY,
  AUDIOPLAYER_PAUSE,
  AUDIOPLAYER_EOF,
  AUDIOPLAYER_ERROR,

}AUDIOPLAYER_StateTypedef;

typedef struct
{
  uint32_t        volume;
  uint32_t        mute;
  uint32_t        freq;
  AUDIOPLAYER_StateTypedef state;
}AUDIO_ProcessTypdef ;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIO_ErrorTypeDef  AUDIOPLAYER_Init(uint8_t Volume);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Play(char *file_name, uint32_t Volume);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Stop(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Pause(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Resume(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Mute(uint32_t Instance);
AUDIO_ErrorTypeDef  AUDIOPLAYER_UnMute(uint32_t Instance);
AUDIOPLAYER_StateTypedef  AUDIOPLAYER_GetState(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_DeInit(void);
uint32_t AUDIOPLAYER_GetProgress (void);
uint32_t AUDIOPLAYER_GetDuration(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_GetFileInfo(char * file_name, Audio_InfoTypeDef* info);
AUDIO_ErrorTypeDef  AUDIOPLAYER_SetVolume(uint32_t Volume);
uint32_t AUDIOPLAYER_GetVolume(void);
uint32_t  AUDIOPLAYER_SeekToTime(uint32_t SeekTime);
AUDIO_ErrorTypeDef  AUDIOPLAYER_NotifyEndOfFile(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_NotifyError(void);

#ifdef __cplusplus
}
#endif

#endif /*__AUDIO_PLAYER_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
