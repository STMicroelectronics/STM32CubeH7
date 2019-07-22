
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
#ifndef __SPIRIT_LOUDNESS_CONTROL_H__
#define __SPIRIT_LOUDNESS_CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef void TSpiritLdCtrl;

typedef struct {
    int gainQ8;
} TSpiritLdCtrl_Prms;

/* Exported constants --------------------------------------------------------*/
/* SUCCESS codes */
#define SPIRIT_LDCTRL_S_OK                    0

/* ERROR codes */
#define SPIRIT_LDCTRL_E_INVALIDARG            -1
#define SPIRIT_LDCTRL_E_SAMPLERATE            -2
#define SPIRIT_LDCTRL_E_CH                    -3
#define SPIRIT_LDCTRL_E_BADALIGN_PERSIST      -4
#define SPIRIT_LDCTRL_E_BADALIGN_SCRATCH      -5

#define SPIRIT_LDCTRL_PERSIST_SIZE_IN_BYTES   120
#define SPIRIT_LDCTRL_SCRATCH_SIZE_IN_BYTES   512
#define SPIRIT_LDCTRL_MAX_CH                  2
#define SPIRIT_LDCTRL_GAIN_Q_BITS             12
#define SPIRIT_LDCTRL_GAIN_MAX                (1<<(SPIRIT_LDCTRL_GAIN_Q_BITS + 6))
#define SPIRIT_LDCTRL_GAIN_MIN                (1<<(SPIRIT_LDCTRL_GAIN_Q_BITS - 6))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int SpiritLdCtrl_Reset  (      TSpiritLdCtrl *_);
int SpiritLdCtrl_Init   (      TSpiritLdCtrl *_, unsigned long sampleRateHz);
int SpiritLdCtrl_SetPrms(      TSpiritLdCtrl *_, const TSpiritLdCtrl_Prms *prms);
int SpiritLdCtrl_GetPrms(const TSpiritLdCtrl *_,       TSpiritLdCtrl_Prms *prms);
int SpiritLdCtrl_Apply  (      TSpiritLdCtrl *_, int nChannels, short *pcm, short nSamplesPerCh, void *scratch);

#ifdef __cplusplus
}
#endif


#endif /* __SPIRIT_LOUDNESS_CONTROL_H__ */
