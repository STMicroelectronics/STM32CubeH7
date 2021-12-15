/**
  ******************************************************************************
  * @file    greq_glo.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    15-Oct-2017
  * @brief   This file contains GREQ module definitions to export
  *          For more details about this Library, please refer to document
  *          "UM1798 User manual".
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
#ifndef GREQ_GLO_H
#define GREQ_GLO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_fw_glo.h"

/* Exported constants --------------------------------------------------------*/

/*      C O D E   C O N F I G U R A T I O N                                   */
/* Supported number of bands */
#define GREQ_NB_BANDS_5         (5)
#define GREQ_NB_BANDS_8         (8)
#define GREQ_NB_BANDS_10        (10)

/* Supported Equalizer presets */
#define GREQ_NO_PRESET          (0)
#define GREQ_PRESET_POP         (GREQ_NO_PRESET + 1)
#define GREQ_PRESET_JAZZ        (GREQ_PRESET_POP + 1)
#define GREQ_PRESET_ROCK        (GREQ_PRESET_JAZZ + 1)
#define GREQ_PRESET_VOCAL       (GREQ_PRESET_ROCK + 1)
#define GREQ_PRESET_CLASSICAL   (GREQ_PRESET_VOCAL + 1)
#define GREQ_PRESET_HIPHOP      (GREQ_PRESET_CLASSICAL + 1)
#define GREQ_PRESET_END         (GREQ_PRESET_HIPHOP)

/*      E R R O R   V A L U E S                                               */
#define GREQ_ERROR_NONE                            0
#define GREQ_UNSUPPORTED_NUMBER_OF_BYTEPERSAMPLE  -1
#define GREQ_UNSUPPORTED_NUMBER_OF_CHANNELS       -2
#define GREQ_UNSUPPORTED_NUMBER_OF_BANDS          -3
#define GREQ_UNSUPPORTED_GAIN_PRESET              -4
#define GREQ_UNSUPPORTED_INTERLEAVING_MODE        -5
#define GREQ_UNSUPPORTED_FRAME_SIZE               -6
#define GREQ_UNSUPPORTED_GAIN                     -7
#define GREQ_BAD_HW                               -8

/* Exported types ------------------------------------------------------------*/

struct greq_dynamic_param {
    int16_t   enable;                                   // 0/1 to disable or enable effect
    int16_t   user_gain_per_band_dB[GREQ_NB_BANDS_10];  // in dB from -12 to +12 by 1dB step
    int16_t   gain_preset_idx;                          // see equalizer presets defined above
};
typedef struct greq_dynamic_param greq_dynamic_param_t;

struct greq_static_param {
    int16_t   nb_bands;                     // see supported number of bands defined above
};
typedef struct greq_static_param greq_static_param_t;

/* External variables --------------------------------------------------------*/

extern const uint32_t greq_scratch_mem_size;
extern const uint32_t greq_persistent_mem_size;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* greq_reset() : initializes static memory, states machines, ... */
extern int32_t greq_reset(void *persistent_mem_ptr, void *scratch_mem_ptr);
/* greq_setConfig() : use to change dynamically some parameters */
extern int32_t greq_setConfig(greq_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);
/* greq_getConfig() : use to get values of dynamic parameters */
extern int32_t greq_getConfig(greq_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);
/* greq_setParam() : use to set parameters that won't change during processing */
extern int32_t greq_setParam(greq_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);
/* greq_getParam() : use to get values of static parameters */
extern int32_t greq_getParam(greq_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);
/* greq_process() : this is the main processing routine */
extern int32_t greq_process(buffer_t *input_buffer, buffer_t *output_buffer, void *persistent_mem_ptr);

#ifdef __cplusplus
}
#endif

#endif  /* GREQ_GLO_H */

