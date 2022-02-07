/**
******************************************************************************
* @file    audio_player/Inc/player.h
* @author  MCD Application Team
* @brief   Header for audioplayer.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_PLAYER_H
#define __AUDIO_PLAYER_H

/** @addtogroup AUDIO_PLAYER_CORE
  * @{
  */

/** @addtogroup PLAYER_CORE
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "ff.h" /* File System */

#include "audio_fw_glo.h"
#include "src236_glo.h"
#include "src441_glo.h"

#include "audio_conf.h"

#include "fwkmempool.h"
#include "songutilities.h"
#include "coders.h"

/* Exported constants --------------------------------------------------------*/
/** @defgroup PLAYER_CORE_Exported_Constants
* @{
*/
#define MUTE_OFF                      0x00
#define MUTE_ON                       0x01

/* Max Filename Length */
#ifndef MAX_FILE_NAME_LENGHT
#define MAX_FILE_NAME_LENGHT               256 /* Full path name */
#endif /* !MAX_FILE_NAME_LENGHT */

/*Default Audio Volume value */
#ifndef AUDIO_PLAYER_DEFAULT_VOLUME
#define AUDIO_PLAYER_DEFAULT_VOLUME               70
#endif /* !AUDIO_PLAYER_DEFAULT_VOLUME */

/*Max Audio decoder packet buffer size (MP3,WAV...)*/
#ifndef MAX_OUT_PACKET_SZE
#define MAX_OUT_PACKET_SZE                        MAX_OUT_DEFAULT_PACKET_SZE     /* Maximum usage in bytes vs supported frame format*/
#endif /* MAX_OUT_PACKET_SZE */

/*Max Audio header information buffer size (MP3,WAV...)*/
#ifndef MAX_AUDIO_HEADER_SIZE
#define MAX_AUDIO_HEADER_SIZE                     MAX_AUDIO_DEFAULT_HEADER_SIZE  /* Maximum usage in bytes*/
#endif /* !MAX_AUDIO_HEADER_SIZE */

/*output double buffer stream to headphones*/
#ifndef AUDIO_OUT_BUFFER_SIZE
#define AUDIO_OUT_BUFFER_SIZE                    (uint32_t)(WAV_PACKET_SZE*2)    /* (480*2*2*2): Output double buffer of 960 stereo samples \
                                                                                    in 16-bits representing 10ms frame at 48kHz \
                                                                                    as (480 * 2 (half + half) * 2 (stereo) * 2 (bytes x sample)) in bytes*/
#endif /* !AUDIO_OUT_BUFFER_SIZE */
#if 0
/*Audio buffer size (for MP3,WAV...) */
#ifndef AUDIO_OUT_BUFFER_SIZE
#define AUDIO_OUT_BUFFER_SIZE                     (MAX_OUT_PACKET_SZE * 2)/3
#endif /* !AUDIO_OUT_BUFFER_SIZE */
#endif

/* Total memory amount required for Player */
#define PLAYER_REQUIRED_MEMORY_SIZE     ((SRC236_FRAME_SIZE_MAX*2*2) + AUDIO_OUT_BUFFER_SIZE + MAX_OUT_PACKET_SZE)

/**
* @}
*/

/* Exported types ------------------------------------------------------------*/
/** @defgroup PLAYER_CORE_Exported_Types
* @{
*/
typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOT_INITIALIZED,
  AUDIO_ERROR_HW,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_MEM,
  AUDIO_ERROR_EOF,
  AUDIO_ERROR_INVALID_PARAMS,
  AUDIO_ERROR_CODEC,
  AUDIO_ERROR_SRC,      /*for player only*/
}AUDIO_ErrorTypeDef;


typedef enum {
  AUDIO_STATE_INIT = 0,
  AUDIO_STATE_WAIT,
  AUDIO_STATE_IDLE,
  AUDIO_STATE_START,
  AUDIO_STATE_STOP,
  AUDIO_STATE_PLAY,
  AUDIO_STATE_EOF,            /*for player only*/
  AUDIO_STATE_PRERECORD,      /*for rec only*/
  AUDIO_STATE_RECORD,         /*for rec only*/
  AUDIO_STATE_NEXT,           /*for player only*/
  AUDIO_STATE_PREVIOUS,       /*for player only*/
  /*AUDIO_STATE_FORWARD,
  AUDIO_STATE_BACKWARD,*/
  AUDIO_STATE_PAUSE,
  AUDIO_STATE_RESUME,
  AUDIO_STATE_VOLUME_UP,
  AUDIO_STATE_VOLUME_DOWN,
  AUDIO_STATE_BACKMENU,
  AUDIO_STATE_ERROR,
}AUDIO_StateTypeDef;

typedef enum {
  BUFFER_OFFSET_NONE = 0, /*empty*/
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

/* Audio buffer control struct */
typedef struct {
  uint8_t *buff;
  uint32_t size;
  BUFFER_StateTypeDef state;   /* empty (none), half, full*/
  uint32_t fptr;
}AUDIO_OUT_BufferTypeDef;

/* Generic type containing main Audio format info */
typedef struct {
  uint32_t FileSize;       /*in bytes*/
  uint32_t FileFormat;     /* '3':MP3; 'V':WAV*/
  uint32_t AudioFormat;    /* 'P':PCM'*/
  uint16_t NbrChannels;
  uint32_t SampleRate;
  uint32_t ByteRate;
  uint16_t BitPerSample;
  uint32_t Duration;       /* File duration in seconds */
}AUDIO_FILE_InfoTypeDef;

typedef enum {
  AUDIO_CFG_NONE,
  AUDIO_CFG_VOLUME_UP,
  AUDIO_CFG_VOLUME_DOWN,
  AUDIO_CFG_VOLUME_SET,
  AUDIO_CFG_MUTE_ON,
  AUDIO_CFG_MUTE_OFF,
  AUDIO_CFG_PAUSE,
  AUDIO_CFG_RESUME,
  AUDIO_CFG_ERROR,
} AUDIO_PLAYBACK_CfgTypeDef;

/*!
 * \brief Player handle type
 */
typedef void *PLAYER_HANDLE_h;

/**
  * @}
  */

/*-----------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
/** @defgroup PLAYER_CORE_Exported_Variables
  * @{
  */
#ifdef __WAV_DECODER__
extern WAVE_FormatTypeDef DecWavFmtStruct;
#endif

#ifdef __MP3_DECODER__
extern TSpiritMP3Info  mp3_Info;
#endif

#define MAX_PLAYER_INSTANCES_NBR  2

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @defgroup PLAYER_CORE_Exported_Functions
  * @{
  */
AUDIO_ErrorTypeDef PLAYER_Init(uint8_t Volume, PLAYER_HANDLE_h *pHandle);
AUDIO_ErrorTypeDef PLAYER_DeInit(PLAYER_HANDLE_h pHandle);
AUDIO_ErrorTypeDef PLAYER_SelectFile(PLAYER_HANDLE_h pHandle, char* file_name);
AUDIO_ErrorTypeDef PLAYER_Start(PLAYER_HANDLE_h pHandle, char *file_name);
AUDIO_ErrorTypeDef PLAYER_Stop(PLAYER_HANDLE_h pHandle);

uint32_t           PLAYER_GetTimeInfo(PLAYER_HANDLE_h pHandle, uint32_t* Length, uint32_t* Elapsed);
uint32_t           PLAYER_GetFileLength(PLAYER_HANDLE_h pHandle);
AUDIO_ErrorTypeDef PLAYER_GetFileInfo(PLAYER_HANDLE_h pHandle, char *file_name, AUDIO_FILE_InfoTypeDef *AudioInfoData);

AUDIO_ErrorTypeDef PLAYER_SetConfig(PLAYER_HANDLE_h pHandle, AUDIO_PLAYBACK_CfgTypeDef AudioConfig);

uint32_t           PLAYER_SeekToTime(PLAYER_HANDLE_h pHandle, uint32_t NewTime);
AUDIO_StateTypeDef PLAYER_GetState(PLAYER_HANDLE_h pHandle);

/* Volume */
uint32_t            PLAYER_GetVolume(PLAYER_HANDLE_h pHandle);
AUDIO_ErrorTypeDef  PLAYER_SetVolume(PLAYER_HANDLE_h pHandle, uint32_t volume);

/* Callback functions */
void PLAYER_RegisterCallbacks( PLAYER_HANDLE_h pHandle,
                              fnUserCallback_TypeDef*      pPrePrcessingCallback,
                              fnUserCallback_TypeDef*      pPostPrcessingCallback,
                              fnEndOfFileCallback_TypeDef* pEndOfFileCallback);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __PLAYER_H */

