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

/** @addtogroup AUDIO_PLAYER_APP
* @{
*/

/** @addtogroup PLAYER_APPLICATION
* @{
*/

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_conf.h"
#include "player.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup PLAYER_APPLICATION_Exported_Functions
* @{
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_Init(uint8_t volume);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Play(uint32_t frequency);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Stop(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Pause(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Resume(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_Mute(uint8_t state);
AUDIO_ErrorTypeDef  AUDIOPLAYER_GetFileInfo(char* file, AUDIO_FILE_InfoTypeDef* info);
AUDIO_ErrorTypeDef  AUDIOPLAYER_SelectFile(char* file);
AUDIO_StateTypeDef  AUDIOPLAYER_GetState(void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_DeInit(void);
uint32_t            AUDIOPLAYER_GetProgress (void);
uint32_t            AUDIOPLAYER_GetDuration (void);
AUDIO_ErrorTypeDef  AUDIOPLAYER_SetVolume(uint32_t volume);
uint32_t            AUDIOPLAYER_GetVolume(void);
void                AUDIOPLAYER_NotifyEndOfFile(void);
uint32_t            AUDIOPLAYER_SeekToTime(uint32_t time);

/* Callback functions */
void                AUDIOPLAYER_RegisterCallbacks(fnUserCallback_TypeDef*      pPrePrcessingCallback,
                                                  fnUserCallback_TypeDef*      pPostPrcessingCallback,
                                                  fnEndOfFileCallback_TypeDef* pEndOfFileCallback);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif /*__AUDIO_PLAYER_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
