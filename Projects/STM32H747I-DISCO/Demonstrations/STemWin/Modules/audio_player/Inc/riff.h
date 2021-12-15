/**
  ******************************************************************************
  * @file    audio_player/Inc/riff.h
  * @author  MCD Application Team
  * @brief   This file includes AVI RIFF types definitions
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
#ifndef __RIFF_H_
#define __RIFF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <inttypes.h>

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM    (0x0001)
#endif
#define WAVE_FORMAT_ALAW   (0x0006)
#define WAVE_FORMAT_MULAW  (0x0007)

#define AVIF_HASINDEX      0x10

struct RIFF_avih {
  uint32_t                 us_frame;          /* microsec per frame */
  uint32_t                 bps;               /* byte/s overall */
  uint32_t                 pad_gran;          /* pad_gran (???) */
  uint32_t                 flags;
  uint32_t                 frames;            /* # of frames (all) */
  uint32_t                 init_frames;       /* initial frames (???) */
  uint32_t                 streams;
  uint32_t                 bufsize;           /* suggested buffer size */
  uint32_t                 width;
  uint32_t                 height;
  uint32_t                 scale;
  uint32_t                 rate;
  uint32_t                 start;
  uint32_t                 length;
};

struct RIFF_strh {
  unsigned char            type[4];      /* stream type */
  unsigned char            handler[4];
  uint32_t                 flags;
  uint32_t                 priority;
  uint32_t                 init_frames;       /* initial frames (???) */
  uint32_t                 scale;
  uint32_t                 rate;
  uint32_t                 start;
  uint32_t                 length;
  uint32_t                 bufsize;           /* suggested buffer size */
  uint32_t                 quality;
  uint32_t                 samplesize;
  struct
  {
    uint16_t               left;
    uint16_t               top;
    uint16_t               right;
    uint16_t               bottom;
  }rc_frame;
};

struct RIFF_strf_vids {       /* == BitMapInfoHeader */
  uint32_t                 size;
  uint32_t                 width;
  uint32_t                 height;
  uint16_t                 planes;
  uint16_t                 bit_cnt;
  unsigned char            compression[4];
  uint32_t                 image_size;
  uint32_t                 xpels_meter;
  uint32_t                 ypels_meter;
  uint32_t                 num_colors;        /* used colors */
  uint32_t                 imp_colors;        /* important colors */
  /* may be more for some codecs */
};

struct RIFF_strf_auds {       /* == WaveHeader (?) */
  uint16_t                 format;
  uint16_t                 channels;
  uint32_t                 rate;
  uint32_t                 av_bps;
  uint16_t                 blockalign;
  uint16_t                 size;
};

struct AVI_HDR {
  unsigned char            riff_id[4];
  uint32_t                 riff_size;
  unsigned char            riff_type[4];

  unsigned char            hdrl_list_id[4];
  uint32_t                 hdrl_size;
  unsigned char            hdrl_type[4];

  unsigned char            avih_id[4];
  uint32_t                 avih_size;
  struct RIFF_avih         avih;
};

struct AVIX_HDR {
  unsigned char            riff_id[4];
  uint32_t                 riff_size;
  unsigned char            riff_type[4];

  unsigned char            data_list_id[4];
  uint32_t                 data_size;
  unsigned char            data_type[4];
};

struct AVI_HDR_VIDEO {
  unsigned char            strl_list_id[4];
  uint32_t                 strl_size;
  unsigned char            strl_type[4];

  unsigned char            strh_id[4];
  uint32_t                 strh_size;
  struct RIFF_strh         strh;

  unsigned char            strf_id[4];
  uint32_t                 strf_size;
  struct RIFF_strf_vids    strf;
};

struct AVI_HDR_AUDIO {
  unsigned char            strl_list_id[4];
  uint32_t                 strl_size;
  unsigned char            strl_type[4];

  unsigned char            strh_id[4];
  uint32_t                 strh_size;
  struct RIFF_strh         strh;

  unsigned char            strf_id[4];
  uint32_t                 strf_size;
  struct RIFF_strf_auds    strf;
};

struct AVI_HDR_ODML {
  unsigned char            strl_list_id[4];
  uint32_t                 strl_size;
  unsigned char            strl_type[4];

  unsigned char            strh_id[4];
  uint32_t                 strh_size;
  uint32_t                 total_frames;
};

struct AVI_DATA {
  unsigned char            data_list_id[4];
  uint32_t                 data_size;
  unsigned char            data_type[4];
  /* audio+video data follows */
};

struct CHUNK_HDR {
  unsigned char            id[4];
  uint32_t                 size;
};

typedef struct AVIOLDINDEX {
  unsigned char              id[4];
  uint32_t                   size;
  struct {
    unsigned char            idx_id[4];
    uint32_t                 idx_flags;
    uint32_t                 idx_offset;
    uint32_t                 idx_size;
  } aIndex[];
} _AVIOLDINDEX;

#ifdef __cplusplus
}
#endif

#endif /* __RIFF_H_ */


