/**
  ******************************************************************************
  * @file    avi_aprser.h
  * @author  MCD Application Team
  * @brief   header for avi_parser.c file
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
#ifndef __AVI_PARSER_H
#define __AVI_PARSER_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ff.h"
#include "riff.h"
#include "cmsis_os.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  AVI_OK   = 0,
  AVI_RIFF_ERR,
  AVI_AVI_ERR,
  AVI_LIST_ERR,
  AVI_HDRL_ERR,
  AVI_AVIH_ERR,
  AVI_STRL_ERR,
  AVI_STRH_ERR,
  AVI_STRF_ERR,
  AVI_MOVI_ERR,
  AVI_FORMAT_ERR,
  AVI_STREAM_ERR,
}AVI_Status_t;

typedef struct
{
  uint32_t SecPerFrame;
  uint32_t MaxByteSec;
  uint32_t PaddingGranularity;
  uint32_t Flags;
  uint32_t TotalFrame;
  uint32_t InitFrames;
  uint32_t Streams;
  uint32_t RefBufSize;
  uint32_t Width;
  uint32_t Height;
  uint32_t Reserved[4];
} avi_header_t;

typedef struct
{
  uint32_t StreamType;
  uint16_t Handler;
  uint16_t Flags;
  uint32_t Priority;
  uint32_t Language;
  uint32_t InitFrames;
  uint32_t Scale;
  uint32_t Rate;
  uint32_t Start;
  uint32_t Length;
  uint32_t RefBufSize;
  uint32_t Quality;
  uint32_t SampleSize;
  struct
  {
    uint16_t Left;
    uint16_t Top;
    uint16_t Right;
    uint16_t Bottom;
  }Frame;
}stream_header_t;

typedef struct parser_handle_s {
  FIL                audio_file_handle;
  FIL                video_file_handle;
  riff_handle       *audio_riff;
  riff_handle       *video_riff;
  stream_header_t    stream_header;
  avi_header_t       avi_header;

  uint32_t           current_audio_chunk;
  uint32_t           current_video_chunk;
  uint32_t           current_total_video_chunk;
}parser_handle_t;

typedef enum {
  AUDIO_CHUNK = 0,
  VIDEO_CHUNK,
} ChunkType;

typedef struct avi_parser_s avi_parser_t;

struct avi_parser_s {
	 parser_handle_t parser;
	 AVI_Status_t (*init)(avi_parser_t *avi_parser, char *filename);
	 AVI_Status_t (*deinit)(avi_parser_t *avi_parser);
     uint32_t (*get_data)(avi_parser_t *avi_parser, uint8_t *buffer, ChunkType type);
};

/* Exported constants --------------------------------------------------------*/
#define IS_VIDEO_CHUNK( __rh )   ( strcmp(__rh->c_id, "00dc") == 0 )

#define IS_AUDIO_CHUNK( __rh )   ( strcmp(__rh->c_id, "01wb") == 0 )

#define CHUNK_TYPE( __rh, __type ) (( IS_AUDIO_CHUNK( __rh ) && __type == AUDIO_CHUNK ) || ( IS_VIDEO_CHUNK( __rh ) && __type == VIDEO_CHUNK ))

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AVI_Status_t init_avi_parser(avi_parser_t *avi_parser);

#ifdef __cplusplus
}
#endif
#endif /*__AVI_PARSER_H*/
