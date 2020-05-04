/**
  ******************************************************************************
  * @file    src441glo.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    15-Oct-2017
  * @brief   This file contains SRC441 module definitions to export
  *          For more details about this Library, please refer to document
  *          "UM1640 User manual".
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
#ifndef SRC441_GLO_H
#define SRC441_GLO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_fw_glo.h"

/* Exported constants --------------------------------------------------------*/

/* input frame MAX size in samples 147 (3.333ms @44.1kHz) -> to save memory   */
/* if buffer of 441 samples is needed (10ms @44.1kHz)the scr is called 3 times*/
/* The value cannot be modified (unless providing a new library accordingly)  */
#define SRC411_FRAME_SIZE_MAX                       147

/*      E R R O R   V A L U E S                                               */
#define SRC441_ERROR_NONE                           0
#define SRC441_UNSUPPORTED_MODE                     -1
#define SRC441_BAD_FRAME_SIZE                       -2
#define SRC441_WRONG_NBBYTES_PER_SAMPLES            -3
#define SRC441_UNSUPPORTED_NB_CHANNELS              -4
#define SRC441_UNSUPPORTED_INPLACE_PROCESSING       -5
#define SRC441_BAD_HW                               -6

/* Exported types ------------------------------------------------------------*/

struct src441_dynamic_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   empty;
};
typedef struct src441_dynamic_param src441_dynamic_param_t;


struct src441_static_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   empty;
};
typedef struct src441_static_param src441_static_param_t;

/* External variables --------------------------------------------------------*/

extern const uint32_t src441_scratch_mem_size;
extern const uint32_t src441_persistent_mem_size;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* src441_reset() : initializes static memory, states machines, ... */
extern int32_t src441_reset(void *persistent_mem_ptr, void *scratch_mem_ptr);
/* src441_setConfig() : use to change dynamically some parameters */
extern int32_t src441_setConfig(src441_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);
/* src441_getConfig() : use to get values of dynamic parameters */
extern int32_t src441_getConfig(src441_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);
/* src441_setParam() : use to set parameters that won't change during processing */
extern int32_t src441_setParam(src441_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);
/* src441_getParam() : use to get values of static parameters */
extern int32_t src441_getParam(src441_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);
/* src441_process() : this is the main processing routine */
extern int32_t src441_process(buffer_t *input_buffer, buffer_t *output_buffer, void *persistent_mem_ptr);

#ifdef __cplusplus
}
#endif

#endif  /* SRC441_GLO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
