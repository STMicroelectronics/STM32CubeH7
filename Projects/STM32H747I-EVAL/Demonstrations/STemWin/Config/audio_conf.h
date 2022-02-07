/**
  ******************************************************************************
  * @file    audio_conf.h
  * @author  MCD Application Team
  * @brief   Audio processing configuration file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_CONF_H_
#define __AUDIO_CONF_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup AUDIO_PLAYER
* @{
*/

/** @defgroup AUDIO_CONF_Template
* @brief Audio Player Configuration Template file
* @{
*/

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
/** @defgroup AUDIO_CONF_Exported_Constants
* @{
*/
/*-- Audio Decoder Codecs Support --*/
#define __WAV_DECODER__
#define __AVI_DECODER__

/*-- Specify "malloc" function to be used by the Player to allocate memory region --*/
#ifndef PLAYER_MALLOC
#define PLAYER_MALLOC(x)                pvPortMalloc(x)
#endif /* PLAYER_MALLOC */

/*-- Specify "free" function to be used by Player to release, previously, allocated memory region --*/
#ifndef PLAYER_FREE
#define PLAYER_FREE(x)                  vPortFree(x)
#endif /* PLAYER_FREE */

/*-- Audio Pack size : use default value defined in coders.h header file --*/
/* #define MAX_OUT_DEFAULT_PACKET_SZE                     (uint32_t)(1920*2*2) */       /* 1920 stereo samples in 16-bits*/

/*-- Static allocation for MP3 Decoder --*/
#ifdef __MP3_DECODER__
#define AUDIO_CODEC_MP3_STATIC_ALLOC    /* Enable Static memory allocation for MP3 Decoder */
#endif /* __MP3_DECODER__ */

/*-- Static allocation for AVI Decoder --*/
#ifdef __AVI_DECODER__
#define AUDIO_CODEC_AVI_STATIC_ALLOC    /* Enable Static memory allocation for AVI Decoder */ /* This should be enabled until having final fix for memory leak issue */
#endif /* __AVI_DECODER__ */

/*+++++++++++++++++++++ AUDIO PLAYER Constants +++++++++++++++++++++++++++++++*/
/*-- Default Audio Player Volume */
#define AUDIO_PLAYER_DEFAULT_VOLUME   50

/*-- Audio Volume Backup Register --*/
#define AUDIO_PLAYER_BKP_REG          RTC_BKP_DR0
#define AUDIO_PLAYER_EQU1_BKP         RTC_BKP_DR1
#define AUDIO_PLAYER_EQU2_BKP         RTC_BKP_DR2
#define AUDIO_PLAYER_EQU3_BKP         RTC_BKP_DR3
#define AUDIO_PLAYER_EQU4_BKP         RTC_BKP_DR4
#define AUDIO_PLAYER_EQU5_BKP         RTC_BKP_DR5
#ifdef EQ_HIGH_QUALITY
#define AUDIO_PLAYER_EQU6_BKP         RTC_BKP_DR6
#define AUDIO_PLAYER_EQU7_BKP         RTC_BKP_DR7
#define AUDIO_PLAYER_EQU8_BKP         RTC_BKP_DR8
#define AUDIO_PLAYER_EQU9_BKP         RTC_BKP_DR9
#define AUDIO_PLAYER_EQU10_BKP        RTC_BKP_DR10
#endif /* EQ_HIGH_QUALITY */
#define AUDIO_PLAYER_LOUD_BKP         RTC_BKP_DR11

/* Audio Decoder thread stack size and priority */
#define AUDIO_DEC_THREAD_STACK_SIZE   (8 * configMINIMAL_STACK_SIZE)
#define AUDIO_DEC_THREAD_PRIORITY_H   osPriorityAboveNormal
#define AUDIO_DEC_THREAD_PRIORITY_L   osPriorityNormal

/* Audio Output thread stack size and priority */
#define AUDIO_OUT_THREAD_STACK_SIZE   (8 * configMINIMAL_STACK_SIZE)
#define AUDIO_OUT_THREAD_PRIORITY     osPriorityHigh

/* Audio Player Algo configuration */
/* #define PLAYER_DISABLE_SRC */                /* Disable SRC Algo : for experimental usage */
/* #define PLAYER_DISABLE_SRC_ZERO_COPY */      /* Audio memory pool Zero Copy when SRC is disabled : for experimental usage */

/**
* @}
*/

/* Exported types ------------------------------------------------------------*/
/** @defgroup AUDIO_CONF_Exported_Types
* @{
*/
/**
 * @brief  Read Callback functions type
 */
typedef uint32_t (fnReadCallback_TypeDef)(
                                          void *       pCompressedData,             /* [OUT] Pointer to buffer to fill with coded MP3 data */
                                          uint32_t     nDataSizeInBytes,            /* Buffer size in Bytes */
                                          void *       pUser                        /* User pointer: can be used to pass additional parameters */
                                            );

typedef void (fnXFerCpltCallback_TypeDef)( uint8_t Direction,
                                          uint8_t** pbuf,
                                          uint32_t* pSize);

typedef uint32_t (fnSetPositionCallback_TypeDef)( void* pUser,
                                                 uint32_t Pos);

typedef void (fnUserCallback_TypeDef)( uint8_t* pBuff,
                                      uint32_t Size);

typedef void (fnEndOfFileCallback_TypeDef)(void);

typedef uint32_t (fnGetSizeCallback_TypeDef)( void* pUser);

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif  /* __AUDIO_CONF_H_ */

