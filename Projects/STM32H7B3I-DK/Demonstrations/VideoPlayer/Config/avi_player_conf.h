/**
  ******************************************************************************
  * @file    avi_aprser.h
  * @author  MCD Application Team
  * @brief   avi player config file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AVI_PLAYER_CONF_H
#define __AVI_PLAYER_CONF_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_discovery.h"
#include "stm32h7b3i_discovery_audio.h"
#include "cmsis_os.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* AUDIO config options */
#define ENABLE_AUDIO
#define AUDIO_BUFFER_LOCATION "audio_section"
#define AUDIO_INSTANCE        0  /*0 SAI, I2S:1 */
#define AUDIO_CHUNK_SIZE      4096
#define AUDIO_NBR_CHUNKS      2

#define AUDIO_VOLUME_LEVEL    60
#define AUDIO_NBR_CHANNELS    2
#define AUDIO_OUT_DEVICE      AUDIO_OUT_DEVICE_HEADPHONE
#define AUDIO_FREQUENCY       AUDIO_FREQUENCY_44K
#define AUDIO_SAMPLE_RATE     AUDIO_RESOLUTION_16B

/* Player Config */
#define MAX_FILE_NBR  32
#define MAX_NAME_SIZE 64
#define MSG_QUEUE_SIZE  16
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif /*__AVI_PLAYER_CONF_H*/
