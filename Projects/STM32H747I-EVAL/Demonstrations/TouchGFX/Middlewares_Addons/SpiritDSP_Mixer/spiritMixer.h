
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
#ifndef __SPIRITMMIXER_H__
#define __SPIRITMMIXER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct {
    short nChannels;    /* Number of channels in input signal */
    short gainQ15;      /* Gain of channel #1 */
    long  transLen;     /* Transition length in samples */
} TSpiritMixer_Prms;

typedef void TSpiritMixer;

/* Exported constants --------------------------------------------------------*/
/* SUCCESS codes */
#define SPIRIT_MIXER_S_OK                     0

/* ERROR codes */
#define SPIRIT_MIXER_E_INVALIDARG             -1
#define SPIRIT_MIXER_E_CH                     -2
#define SPIRIT_MIXER_E_GAIN                   -3
#define SPIRIT_MIXER_E_BADALIGN_PERSIST       -4

/* Predefined constants */
#define SPIRIT_MIXER_PERSIST_SIZE_IN_BYTES    16
#define SPIRIT_MIXER_MAX_CH                   2
#define SPIRIT_MIXER_MAX_GAIN_Q15             ((1<<15) - 1)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int SpiritMixer_Init   (      TSpiritMixer *_);
int SpiritMixer_SetPrms(      TSpiritMixer *_, const TSpiritMixer_Prms *prms);
int SpiritMixer_GetPrms(const TSpiritMixer *_,       TSpiritMixer_Prms *prms);
int SpiritMixer_Apply  (      TSpiritMixer *_, 
                        const short *in0, const short *in1, int nSamplesPerCh, short *out);

#ifdef __cplusplus
}
#endif

#endif /* __SPIRITMMIXER_H__ */
