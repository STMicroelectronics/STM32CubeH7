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

