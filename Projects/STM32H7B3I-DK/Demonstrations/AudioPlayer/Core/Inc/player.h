/**
******************************************************************************
* @file    player.h
* @author  MCD Application Team
* @brief   Header for player.c module.
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
#ifndef __PLAYER_H
#define __PLAYER_H

/* Includes ------------------------------------------------------------------*/
#include "audio_conf.h"
#include "src236_glo.h"
#include "src441_glo.h"
#include "fwkmempool.h"
#include "songutilities.h"
#include "coders.h"

/* Exported constants ----------------------------------------------------------*/
/* Max Audio decoder packet buffer size (MP3,WAV...) */
#define MAX_OUT_PACKET_SZE     MAX_OUT_DEFAULT_PACKET_SZE 
/* Max Audio header information buffer size (MP3,WAV...) */
#define MAX_AUDIO_HEADER_SIZE  MAX_AUDIO_DEFAULT_HEADER_SIZE

/* output buffer size in frames */
#define FACTOR_BUFFER_INC     8

/* output double buffer stream to headphones */
/* WAV_PACKET_SZE*2*FACTOR_BUFFER_INC :(480*2*2*2): Output double buffer of 960 
stereo samples in 16-bits representing 10ms frame at 48kHz as 
(480 * 2 (half + half) * 2 (stereo) * 2 (bytes x sample)) in bytes */
#define AUDIO_OUT_BUFFER_SIZE  (uint32_t)(WAV_PACKET_SZE*2*FACTOR_BUFFER_INC)
 
#define NBFrames_OutputBuf   (uint32_t)(2 * FACTOR_BUFFER_INC)
#define MEMPOOLBUFSZE        (3 * MAX_OUT_PACKET_SZE)
/* SRC module selected */
#define SELECT_SRC_NONE     0
#define SELECT_SRC_236      1
#define SELECT_SRC_441      2

/* Exported types ------------------------------------------------------------*/
typedef enum {
  PLAY_BUFFER_OFFSET_NONE = 0,
  PLAY_BUFFER_OFFSET_HALF,
  PLAY_BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

/* Audio buffer control structure */
typedef struct {
  uint8_t buff[AUDIO_OUT_BUFFER_SIZE];
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
}Audio_InfoTypeDef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
  AUDIO_ERROR_INVALID_VALUE,
  AUDIO_ERROR_CODEC,
  AUDIO_ERROR_MEM,
  AUDIO_ERROR_SRC,
}AUDIO_ErrorTypeDef;


/* Exported variables --------------------------------------------------------*/
#ifdef __WAV_DECODER__
extern WAVE_FormatTypeDef DecWavFmtStruct;
#endif

#ifdef __MP3_DECODER__
extern TSpiritMP3Info  mp3_Info;
#endif

/* Exported functions ------------------------------------------------------- */
AUDIO_ErrorTypeDef PLAYER_Start(char * file_name, uint32_t Volume);
AUDIO_ErrorTypeDef PLAYER_Stop(void);
AUDIO_ErrorTypeDef PLAYER_Close(void);
void PLAYER_GetTimeInfo(uint32_t* Length, uint32_t* Elapsed);
uint32_t PLAYER_GetFileLength(void);
uint32_t PLAYER_SetFilePosition(uint32_t Position);
uint32_t PLAYER_SeekToTime(uint32_t NewTime);
AUDIO_ErrorTypeDef PLAYER_GetFileInfo(char * file_name, Audio_InfoTypeDef *AudioInfoData);
unsigned int Dec_ReadDataCallback(void* pCompressedData, uint32_t nDataSizeInChars, void* pUserData);
uint8_t PLAYER_StreamInit(uint32_t AudioFreq, uint32_t Device, uint32_t BitsPerSample, uint32_t ChannelsNbr, uint32_t Volume);
AUDIO_ErrorTypeDef PLAYER_OutputBufferCbkProc(uint16_t offset);
AUDIO_ErrorTypeDef PLAYER_GetFileHeaderInfo(char* file_name, Audio_InfoTypeDef *info);
AUDIO_ErrorTypeDef PLAYER_TransfertToMemPool(void);
AUDIO_ErrorTypeDef PLAYER_TransfertFromMemPool(uint16_t Offset_frames,  uint16_t length_frames);
int32_t  PLAYER_GetAvailableSizeMemPool(void);
uint32_t PLAYER_ResetBuffers(void);
AUDIO_ErrorTypeDef PLAYER_MEMPOOL_Init(void);
#endif /* __PLAYER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
