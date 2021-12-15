/**
  ******************************************************************************
  * @file    audio_player/Inc/coders.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for Coders layer
  *          routines
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
#ifndef __CODERS_H_
#define __CODERS_H_

/* Includes ------------------------------------------------------------------*/
#include "songutilities.h"
#include "audio_conf.h"

#if defined(__WAV_DECODER__) || defined(__WAV_ENCODER__)
/* wav processing routines */
#include "wavprocess.h"
#endif /* __WAV_DECODER__ */

#if defined(__MP3_DECODER__) || defined(__MP3_ENCODER__)
/* mp3 processing routines */
#include "mp3process.h"
#endif /* __MP3_DECODER__ */

#if defined(__WMA_DECODER__) || defined(__WMA_ENCODER__)
/* wma processing routines */
#include "wmaprocess.h"
#endif /* __WMA_DECODER__ */

#if defined(__AAC_DECODER__) || defined(__AAC_ENCODER__)
/* aac processing routines */
#include "aacprocess.h"
#endif /* __AAC_DECODER__ */

#if defined(__ADPCM_DECODER__) || defined(__ADPCM_ENCODER__)
/* adpcm processing routines */
#include "adpcmprocess.h"
#endif /* __ADPCM_DECODER__ */

#if defined(__SPEEX_DECODER__) || defined(__SPEEX_ENCODER__)
/* speex processing routines */
#include "speexprocess.h"
#endif /* __SPEEX_DECODER__ */

#if defined(__G711_DECODER__) || defined(__G711_ENCODER__)
/* g711 processing routines */
#include "g711process.h"
#endif /* __G711_DECODER__ */

#if defined(__G726_DECODER__) || defined(__G726_ENCODER__)
/* g726 processing routines */
#include "g726process.h"
#endif /* __G726_DECODER__ */

#if defined(__AVI_DECODER__)
/* avi processing routines */
#include "aviprocess.h"
#endif /* __AVI_DECODER__ */

/** @addtogroup PLAYER_CODERS
* @{
*/

/** @addtogroup Decoders
* @{
*/

/** @defgroup Decoders_Exported_Constants
* @{
*/

/* Note: The packet sizes are expressed in Bytes by default */

/*-- FOR AUDIO RECORDER --*/
#define MAX_IN_PACKET_SZE                              (uint32_t)(576*2*2)  /* 576 stereo samples in 16-bits*/

/*-- FOR AUDIO PLAYER --*/
#define MAX_OUT_DEFAULT_PACKET_SZE                     (uint32_t)(3072*2*2) /* 3072 stereo samples in 16-bits*/
#define MAX_AUDIO_DEFAULT_HEADER_SIZE                  200

/*-- FOR AUDIO RECORDERS and PLAYERS --*/
#if defined(__WAV_DECODER__) || defined(__WAV_ENCODER__)
#ifndef WAV_PACKET_SZE
#define WAV_PACKET_SZE                                WAV_PACKET_SZE_PROC   /* 480 stereo samples in 16-bits*/
#endif

#ifndef WAVIN_PACKET_SZE
#define WAVIN_PACKET_SZE                              WAVIN_PACKET_SZE_PROC /* 480  stereo samples in 16-bit*/
#endif
#endif /* __WAV_DECODER__ */

#if defined(__MP3_DECODER__) || defined(__MP3_ENCODER__)
#ifndef MP3_PACKET_SZE
#define MP3_PACKET_SZE                                (uint32_t)(1152*2*2) /* Decoder: 1152 stereo samples in 16-bits*/
#endif

#ifndef MP3IN_PACKET_SZE
#define MP3IN_PACKET_SZE                              (uint32_t)(576*2*2) /* Encoder: 576 stereo samples in 16-bit*/
#endif
#endif /* __MP3_DECODER__ */

/*-- FOR AUDIO PLAYER --*/
#if defined(__WMA_DECODER__)
#ifndef WMA_PACKET_SZE
#define WMA_PACKET_SZE                                (uint32_t)(3072*2*2) /* Max decoded frame size 2048 samples x 1.5 = 3072 stereo samples in 16-bits*/
#endif
#endif /* __WMA_DECODER__ */

#if defined(__AAC_DECODER__)
#ifndef AAC_PACKET_SZE
#define AAC_PACKET_SZE                                (uint32_t)(1024*2*2) /* 1024 stereo samples in 16-bits */
#endif
#endif /* __AAC_DECODER__ */

/*-- FOR VOCODERS --*/
#if defined(__ADPCM_DECODER__) || defined(__ADPCM_ENCODER__)
#ifndef ADPCM_PACKET_SZE
#define ADPCM_PACKET_SZE                               (uint32_t)(ADPCM_FRAME_SIZE * 2 * 2) /* ADPCM_FRAME_SIZE stereo samples in 16-bits */
#endif
#endif /* __ADPCM_DECODER__ */

#if defined(__SPEEX_DECODER__) || defined(__SPEEX_ENCODER__)
#ifndef SPEEX_PACKET_SZE
#define SPEEX_PACKET_SZE                                (uint32_t)(RAW_FRAME_SIZE * 2 * 2) /* NarrowBand: 160 stereo samples in 16-bits or Wideband: 320 stereo samples in 16-bits */
#endif
#endif /* __SPEEX_DECODER__ */


#if defined(__G711_DECODER__) || defined(__G711_ENCODER__)
#ifndef G711_PACKET_SZE
#define G711_PACKET_SZE                                (uint32_t)(G711_FRAME_SIZE * 2 * 2) /* G711_FRAME_SIZE stereo samples in 16-bits */
#endif
#endif /* __G711_DECODER__ */

#if defined(__G726_DECODER__) || defined(__G726_ENCODER__)
#ifndef G726_PACKET_SZE
#define G726_PACKET_SZE                                (uint32_t)(G726_FRAME_SIZE * 2 * 2) /* G726_FRAME_SIZE stereo samples in 16-bits */
#endif
#endif /* __G726_DECODER__ */

#if defined(__AVI_DECODER__)
#ifndef AVI_PACKET_SZE
#define AVI_PACKET_SZE                                  AVI_PACKET_SZE_PROC   /* 1024 stereo samples in 16-bits */
#endif
#endif /* __AVI_DECODER__ */
/**
* @}
*/

/** @defgroup Decoders_Exported_Types
* @{
*/
/* Decoders structure */
typedef struct {
  uint32_t      (*DecoderInit)            (uint8_t* pHeader,
                                           fnReadCallback_TypeDef* pReadCallback,
                                           fnSetPositionCallback_TypeDef* pSetPosCallback,
                                           fnGetSizeCallback_TypeDef pGetSizeCallback,
                                           void *pUserData);

  uint32_t      (*DecoderDeInit)          (void);
  uint32_t      (*DecoderStopMode)        (void);
  uint32_t      (*Decoder_StreamForward)  (void);
  uint32_t      (*Decoder_StreamBackward) (void);
  uint32_t      (*Decoder_DecodeData)     (__IO int16_t *,uint32_t,void*); /* Data Processing routine to provide output PCM stream */
  uint32_t      (*Decoder_ReadTags)       (TAGS_TypeDef*, FIL *file);
  uint32_t      (*Decoder_GetSamplingRate)(void);

  uint32_t      (*Decoder_GetBitRateKbps)(void);
  uint32_t      (*Decoder_GetNbChannels)(void);
  uint32_t      (*Decoder_GetNbBits)(void);

  uint32_t      (*Decoder_SetProgressTime)(uint32_t NewTime);
  uint32_t      (*Decoder_GetStreamLength)(uint32_t fLength);
  uint32_t      (*Decoder_GetElapsedTime) (uint32_t CurrPos);
  uint32_t      (*Decoder_VBRDetected)    (void);
  uint32_t      PacketSize;
} Decoder_TypeDef;


/* Encoders structure */
typedef struct {
  uint32_t      (*EncoderInit)            (uint32_t Freq, uint8_t* Param);
  uint32_t      (*EncoderDeInit)          (void);
  uint32_t      (*EncoderStopMode)        (void);
  uint32_t      (*Encoder_EncodeData)     (int8_t *pBufIn, int8_t *pBufOut, uint32_t* nEnc, void* pOpt);
  uint32_t      (*Encoder_HeaderUpdate)   (uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct, uint32_t* fptr);
  uint32_t      (*Encoder_SetTags)        (TAGS_TypeDef*, void *file);
  uint32_t      (*Encoder_GetSampleRate)  (void);
  uint32_t      (*Encoder_GetStreamLength)(void);
  uint32_t      (*Encoder_GetElapsedTime) (void);
  uint32_t      PacketSize;
} Encoder_TypeDef;

/**
* @}
*/

/** @defgroup Decoders_Exported_Functions
* @{
*/
int8_t CODERS_SelectDecoder(Decoder_TypeDef* pDecoderStruct, int8_t ch);
int8_t CODERS_SelectEncoder(Encoder_TypeDef* pEncoderStruct, int8_t ch);

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

#endif /*__CODERS_H_*/


