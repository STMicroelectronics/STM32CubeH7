/**
  ******************************************************************************
  * @file    audio_fw_glo.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    15-Oct-2017
  * @brief   This file contains audio fw definitions common to all audio modules
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
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
#ifndef _AUDIO_FW_GLO_H_
#define _AUDIO_FW_GLO_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
#define NOT_INTERLEAVED                 0
#define INTERLEAVED                     1

/* Exported types ------------------------------------------------------------*/

typedef struct {
    int32_t       nb_channels;            /* Number of channels */
    int32_t       nb_bytes_per_Sample;    /* Nb bytes per sample    */
    void          *data_ptr;              /* Pointer to data buffer */
    int32_t       buffer_size;            /* Number of samples per channel in the data buffer   */
    int32_t       mode;                   /* buffer mode (interleaved, non interleaved) */
} buffer_t;

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 


#ifdef __cplusplus
}
#endif

#endif /*_AUDIO_FW_GLO_H_*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
