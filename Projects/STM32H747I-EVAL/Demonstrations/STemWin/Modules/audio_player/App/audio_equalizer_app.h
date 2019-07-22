/**
  ******************************************************************************
  * @file    audio_equalizer_app.h
  * @author  MCD Application Team
  * @brief   header of Audio Graphical Equalizer application file
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
#ifndef __AUDIO_EQUALIZER_APP_H
#define __AUDIO_EQUALIZER_APP_H

/** @addtogroup AUDIO_PLAYER_APP
  * @{
  */

/** @addtogroup EQUALIZER_APPLICATION
  * @{
  */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_conf.h"
#include "audio_fw_glo.h"
#include "greq_glo.h"
#include "biq_glo.h"

/* Exported constants --------------------------------------------------------*/
/** @defgroup EQUALIZER_APPLICATION_Exported_Constants
  * @{
  */
/* Equalizer Memory allocation based on OS functions */
#define AUDIO_EQ_USE_RTOS

/* Equalizer Band Configuration */
#define NB_BIQUAD_SEC_1         1
#define NB_BIQUAD_SEC_2         2
#define NB_BIQUAD_SEC_3         3
#define NB_BIQUAD_SEC_5         5
#define NB_BIQUAD_SEC_9         9
 
#define BIQ_FILT_LOUDNESS       2
#define EQ_MAX_GAIN_DB          20   

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup EQUALIZER_APPLICATION_Exported_Types
  * @{
  */
typedef enum
{
  AUDIO_EQUALIZER_ERROR_NONE = 0,
  AUDIO_EQUALIZER_ERROR_MEM,
  AUDIO_EQUALIZER_ERROR_ALGO,
  AUDIO_EQUALIZER_ERROR_INVALID_VALUE
} AUDIO_EQUALIZER_ErrorTypDef;

/* Callback functions type*/
typedef uint32_t (fnBkupRestoreCallback_TypeDef)( uint32_t Reg );
typedef void     (fnBkupSaveCallback_TypeDef)    ( uint32_t Reg, uint32_t Value);

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup EQUALIZER_APPLICATION_Exported_Functions
  * @{
  */
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_Init(uint8_t use_graphic_eq, uint8_t use_loudness,
                                        fnBkupRestoreCallback_TypeDef*  pBkupRestoreCallback,
                                        fnBkupSaveCallback_TypeDef*     pBkupSaveCallback);
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_DeInit(void);
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_SetupBuffer(buffer_t* pBufferSetup);

AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_SetEqGain(uint8_t BandNum, int16_t NewGainValue);
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_SetLoudness(int16_t NewLoudnessValue);
int16_t                      AUDIOEQ_GetEqGain(uint8_t BandNum);
int16_t                      AUDIOEQ_GetLoudness(void);

/* Post Processing function */
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_Apply(uint8_t* pBuff, uint32_t Size);

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

#endif /*__AUDIO_EQUALIZER_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
