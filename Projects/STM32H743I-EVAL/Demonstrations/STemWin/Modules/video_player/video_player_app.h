/**
  ******************************************************************************
  * @file    video_player_app.h
  * @author  MCD Application Team
  * @brief   Header for decode_dma.c module
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
#ifndef __VIDEO_PLAYER_APP_H
#define __VIDEO_PLAYER_APP_H

/* Includes ------------------------------------------------------------------*/
#include "GUI.h"
#include "audio_if.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define CHUNK_SIZE_OUT ((uint32_t)(768 * 10))


#define AVI_VIDEO_BUF_SIZE    (80 * 1024)
#define AUDIO_BUFFER_SIZE      (8 * 1024) 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HW_JPEG_Init(void);
U32  HW_JPEG_Draw (const void * pFileData, U32 DataSize, U32 x0, U32 y0);
void HW_JPEG_DeInit(void);

U32  AUDIO_Init(U8 volume, U32 frequency);
U32  AUDIO_Play(U8 *pData, U16 Size);
U32  AUDIO_Stop   (void);
U32  AUDIO_DeInit (void);
U32  AUDIO_Notify(uint8_t msg);

#endif /* __VIDEO_PLAYER_APP_H */

