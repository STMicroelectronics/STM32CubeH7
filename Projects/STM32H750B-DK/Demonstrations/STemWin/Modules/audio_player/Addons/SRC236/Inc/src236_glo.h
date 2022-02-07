/**
  ******************************************************************************
  * @file    src236glo.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    15-Oct-2017
  * @brief   This file provides SRC236 routines.
  *          For more details about this Library, please refer to document
  *          "UM1641 User manual".
******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SRC236_GLO_H
#define SRC236_GLO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_fw_glo.h"

/* Exported constants --------------------------------------------------------*/

/* Input frame MAX size in samples 240 (5ms @ 48kHz) -> to save memory        */
/* if buffer of 480 samples is needed (10ms @ 48kHz) the scr is several times */
/* e.g from 24khz->48khz 1 time is sufficient to generate 480 output samples  */
/* but from 96khz->48khz the SRC is called 4 times to generate 480 samples    */
/* The value cannot be modified by users (the library is built accordingly)   */
#define SRC236_FRAME_SIZE_MAX  240

/*      C O D E   C O N F I G U R A T I O N                                   */
#define SRC236_RATIO_2      0  // Means FS_Out = FS_In * 2
#define SRC236_RATIO_3      1  // Means FS_Out = FS_In * 3
#define SRC236_RATIO_6      2  // Means FS_Out = FS_In * 6
#define SRC236_RATIO_1_2    3  // Means FS_Out = FS_In / 2
#define SRC236_RATIO_1_3    4  // Means FS_Out = FS_In / 3
#define SRC236_RATIO_1_6    5  // Means FS_Out = FS_In / 6
#define SRC236_RATIO_3_2    6  // Means FS_Out = FS_In * 3/2
#define SRC236_RATIO_2_3    7  // Means FS_Out = FS_In * 2/3

/*      E R R O R   V A L U E S                                               */
#define SRC236_ERROR_NONE                              0
#define SRC236_UNSUPPORTED_MODE                        -1
#define SRC236_WRONG_NBBYTES_PER_SAMPLES               -2
#define SRC236_UNSUPPORTED_NB_CHANNELS                 -3
#define SRC236_UNSUPPORTED_INPLACE_PROCESSING          -4
#define SRC236_INPUT_BUFFERS_TOO_BIG                   -5
#define SRC236_BAD_HW                                  -6

/* Exported types ------------------------------------------------------------*/

struct src236_dynamic_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   empty;
};
typedef struct src236_dynamic_param src236_dynamic_param_t;


struct src236_static_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   src_mode;
};
typedef struct src236_static_param src236_static_param_t;

/* External variables --------------------------------------------------------*/

extern const uint32_t src236_scratch_mem_size;
extern const uint32_t src236_persistent_mem_size;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* src236_reset() : initializes static memory, states machines, ... */
extern int32_t src236_reset(void *persistent_mem_ptr, void *scratch_mem_ptr);
/* src236_setConfig() : use to change dynamically some parameters */
extern int32_t src236_setConfig(src236_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);
/* src236_getConfig() : use to get values of dynamic parameters */
extern int32_t src236_getConfig(src236_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);
/* src236_setParam() : use to set parameters that won't change during processing */
extern int32_t src236_setParam(src236_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);
/* src236_getParam() : use to get values of static parameters */
extern int32_t src236_getParam(src236_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);
/* src236_process() : this is the main processing routine */
extern int32_t src236_process(buffer_t *input_buffer, buffer_t *output_buffer, void *persistent_mem_ptr);

#ifdef __cplusplus
}
#endif

#endif  /* SRC236_GLO_H */

