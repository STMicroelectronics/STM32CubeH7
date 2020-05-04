
/*
  ******************************************************************************
  * Portions COPYRIGHT 2012 STMicroelectronics
  * Portions SPIRIT Audio Engine Copyright (c) 1995-2009, SPIRIT
  *
  * Licensed under MCD-ST Image SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_image_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef __SPIRITEQ_H__
#define __SPIRITEQ_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct {
    int   fltType;
    int   centerHz;     /* band center frequency in range [0 - sf/2] */
    int   widthHz;      /* band width */
    short gainDb;       /* band gain */
} TSpiritEQ_Band;

typedef void TSpiritEq;

/* Exported constants --------------------------------------------------------*/
/* SUCCESS codes */
#define SPIRIT_EQ_S_OK                        0

/* ERROR codes */
#define SPIRIT_EQ_E_INVALIDARG                -1
#define SPIRIT_EQ_E_SAMPLERATE                -2
#define SPIRIT_EQ_E_MAX_BANDS                 -3
#define SPIRIT_EQ_E_MAX_CH                    -4
#define SPIRIT_EQ_E_FLT_TYPE                  -5
#define SPIRIT_EQ_E_BADALIGN_PERSIST          -6

/* Predefined constants */
#define SPIRIT_EQ_PERSIST_SIZE_IN_BYTES       300
#define SPIRIT_EQ_MAX_BANDS                   4
#define SPIRIT_EQ_MAX_CH                      2
#define SPIRIT_EQ_MAX_GAIN_DB                 12
#define SPIRIT_EQ_MIN_GAIN_DB                 -12

/* Band filter types */
#define SPIRIT_EQ_FLT_TYPE_NONE               0
#define SPIRIT_EQ_FLT_TYPE_SHELVING_LOWPASS   1
#define SPIRIT_EQ_FLT_TYPE_SHELVING_HIPASS    2
#define SPIRIT_EQ_FLT_TYPE_PEAKING            3

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int SpiritEQ_Init     (      TSpiritEq *_, unsigned long sampleRateHz);
int SpiritEQ_FltSet   (      TSpiritEq *_, const TSpiritEQ_Band *prms, int idx);
int SpiritEQ_FltGet   (const TSpiritEq *_,       TSpiritEQ_Band *prms, int idx);
int SpiritEQ_FltReset (      TSpiritEq *_, int idx);
int SpiritEQ_Apply    (      TSpiritEq *_, int nChannels, short *pcm, int nSamplesPerCh);

#ifdef __cplusplus
}
#endif

#endif /* __SPIRITEQ_H__ */
