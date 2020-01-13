/**
  ******************************************************************************
  * @file    audio_equalizer_app.h
  * @author  MCD Application Team
  * @brief   header of Audio Graphical Equalizer application file
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
