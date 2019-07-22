/**
  ******************************************************************************
  * @file    audio_player_app.h
  * @author  MCD Application Team
  * @brief   header of audio player application file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
