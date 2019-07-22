/**
  ******************************************************************************
  * @file    audio_player/Inc/aviprocess.h
  * @author  MCD Application Team
  * @brief   This file includes AVI Processing layer headers
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
#ifndef __AVIPROCESS_H_
#define __AVIPROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <cmsis_os.h>

#include "ff.h"
#include "audio_conf.h"
#include "coders.h"

/** @addtogroup AUDIO_PROCESS
* @{
*/

/** @defgroup AVI_PROCESS
* @brief AVI Audio Processing routines
* @{
*/

#if defined(__AVI_DECODER__)
#include "riff.h"

/** @defgroup AVI_PROCESS_Exported_Constants
* @{
*/
#ifndef AVI_PACKET_SZE_PROC
#define AVI_PACKET_SZE_PROC                                (uint32_t)(1024 * 2 * 2) /* 1024 stereo samples in 16-bits */
#endif

/**
* @}
*/

/** @defgroup AVI_PROCESS_Exported_Types
* @{
*/

struct movi_range {
  long int start;
  long int size;
};

/* brief  Audio Format structures definition */
typedef struct av_audio_fmt
{
  unsigned int   fmtid;         /* AUDIO_* */
  unsigned int   rate;
} av_audio_fmt_t;

/* AVI file information structure */
typedef struct { 
  /* avi header */
  unsigned char         riff_type[4];
  unsigned char         fcc_type[4];
  struct RIFF_avih      avih;
  struct RIFF_strh      v_strh;
  struct RIFF_strh      a_strh;
  struct RIFF_strf_vids vids;
  struct RIFF_strf_auds auds;
  int32_t               dml_frames;
  struct movi_range     *movi;
  int                   movi_cnt;
  struct movi_range     wave;

  /* libav stuff */
  struct av_audio_fmt   afmt;

  /* status data */
  long int              a_pos;
  long int              v_pos;
  int                   frames;
  long int              a_bytes;

  /* Temp buffer used for multi-frames reading */
  uint8_t              *tmp_buf;
} AVI_FormatTypeDef;

/**
* @}
*/

/** @defgroup AVI_PROCESS_Exported_Functions
* @{
*/
uint32_t AviProcess_DecInit(uint8_t* pHeader,
                            fnReadCallback_TypeDef* pReadCallback,
                            fnSetPositionCallback_TypeDef* pSetPosCallback,
                            fnGetSizeCallback_TypeDef pGetSizeCallback,
                            void *pUserData);
uint32_t AviProcess_DecDeInit(void);
uint32_t AviProcess_DecodeData(__IO int16_t* OutPutBuffer,uint32_t NbSamples,void * none);

uint32_t AviProcess_DecSetProgressTime(uint32_t NewTime);
uint32_t AviProcess_DecGetStreamLenght(uint32_t fLength);
uint32_t AviProcess_DecGetElapsedTime(uint32_t CurPos);
uint32_t AviProcess_DecGetSamplingRate(void);

uint32_t AviProcess_GetNbChannels(void);
uint32_t AviProcess_GetNbBits(void);
uint32_t AviProcess_GetNbSamplesFrame(void);

uint32_t AviProcess_MonoToStereo(int16_t* BIn, int16_t* BOut, uint32_t Size);

/**
* @}
*/

#endif /* __AVI_DECODER__ */

/**
* @}
*/

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __AVIPROCESS_H_ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
