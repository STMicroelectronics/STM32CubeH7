/**
  ******************************************************************************
  * @file    biq_glo.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    15-Oct-2017
  * @brief   This file contains BiQuad module definitions to export
  *          For more details about this Library, please refer to document
  *          "UM1625 User manual".
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
#ifndef BIQUAD_DF1_CASCADE_GLO_H
#define BIQUAD_DF1_CASCADE_GLO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_fw_glo.h"

/* Exported constants --------------------------------------------------------*/

/*      E R R O R   V A L U E S                                               */
#define BIQ_ERROR_NONE                           0
#define BIQ_UNSUPPORTED_INTERLEAVING_MODE       -1
#define BIQ_UNSUPPORTED_NUMBER_OF_CHANNELS      -2
#define BIQ_UNSUPPORTED_NUMBER_OF_BYTEPERSAMPLE -3
#define BIQ_BAD_HW                              -4

/* Exported types ------------------------------------------------------------*/

struct biquad_df1_cascade_dynamic_param {
    uint8_t enable;
    uint8_t nb_sos;
    int32_t biquad_coeff[60];   /* 6*MAX_NB_SOS */
};
typedef struct biquad_df1_cascade_dynamic_param biquad_df1_cascade_dynamic_param_t;


struct biquad_df1_cascade_static_param {
  uint8_t    nb_sos;
  int32_t    biquad_coeff[60];  /* 6*MAX_NB_SOS */
};
typedef struct biquad_df1_cascade_static_param biquad_df1_cascade_static_param_t;

/* External variables --------------------------------------------------------*/

extern const uint32_t biquad_df1_cascade_scratch_mem_size;
extern const uint32_t biquad_df1_cascade_persistent_mem_size;


/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* biquad_df1_cascade_reset() : initializes static memory, states machines, ... */
extern int32_t biquad_df1_cascade_reset(void *persistent_mem_ptr, void *scratch_mem_ptr);

/* biquad_df1_cascade_setConfig() : use to change dynamically some parameters */
extern int32_t biquad_df1_cascade_setConfig(biquad_df1_cascade_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);

/* biquad_df1_cascade_getConfig() : use to get values of dynamic parameters */
extern int32_t biquad_df1_cascade_getConfig(biquad_df1_cascade_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);

/* biquad_df1_cascade_setParam() : use to set parameters that won't change during processing */
extern int32_t biquad_df1_cascade_setParam(biquad_df1_cascade_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);

/* biquad_df1_cascade_getParam() : use to get values of static parameters */
extern int32_t biquad_df1_cascade_getParam(biquad_df1_cascade_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);

/* biquad_df1_cascade_process() : this is the main processing routine */
extern int32_t biquad_df1_cascade_process(buffer_t *input_buffer, buffer_t *output_buffer, void *persistent_mem_ptr);

#ifdef __cplusplus
}
#endif

#endif // BIQUAD_DF1_CASCADE_GLO_H

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
