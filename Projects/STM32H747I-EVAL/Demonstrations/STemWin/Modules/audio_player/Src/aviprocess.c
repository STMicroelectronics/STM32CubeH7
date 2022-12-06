/**
  ******************************************************************************
  * @file    audio_player/Src/aviprocess.c
  * @author  MCD Application Team
  * @brief   This file includes AVI audio processing layer routines
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

/* Includes ------------------------------------------------------------------*/
#include "aviprocess.h"
#include "player.h"

/** @addtogroup AUDIO_PROCESS
* @{
*/

/** @defgroup AVI_PROCESS
* @brief AVI Audio Processing routines
* @{
*/

#if defined(__AVI_DECODER__) /* Don't build if not configured for use in audio_app_conf.h */
/* .AVI file format : ----------------------------------------------------------

RIFF                RIFF HEADER
|-AVI               AVI CHUNK
  |-hdrl            MAIN AVI HEADER
  | |-avih          AVI HEADER
  | |-strl          STREAM LIST[One per stream]
  | | |-strh            STREAM HEADER[Required after above]
  | | |-strf            STREAM FORMAT
  | | |-strd            OPTIONAL -- STREAM DATA
  | | |-strn            OPTIONAL -- STREAM NAME
  |-movi            MOVIE DATA
  | |-rec           RECORD DATA[SEE BELOW]
  |   |-[data subchunks]    RAW DATA[SEE BELOW]
  |-idx1            AVI INDEX
    |-[index data]      DATA
--------------------------------------------------------------------------------
*/

/** @defgroup AVI_PROCESS_Private_Defines
* @{
*/
/**
* \def AV_AUDIO_XXX
* Audio Frames formats.
*/
#define AV_AUDIO_NONE           0
#define AV_AUDIO_U8_MONO        1
#define AV_AUDIO_U8_STEREO      2
#define AV_AUDIO_S16_LE_MONO    3
#define AV_AUDIO_S16_LE_STEREO  4
#define AV_AUDIO_S16_BE_MONO    5
#define AV_AUDIO_S16_BE_STEREO  6
#define AV_AUDIO_MP3            7
#define AV_AUDIO_FMT_COUNT      8
#if BYTE_ORDER == BIG_ENDIAN
# define AV_AUDIO_S16_NATIVE_MONO   AV_AUDIO_S16_BE_MONO
# define AV_AUDIO_S16_NATIVE_STEREO AV_AUDIO_S16_BE_STEREO
#else
# define AV_AUDIO_S16_NATIVE_MONO   AV_AUDIO_S16_LE_MONO
# define AV_AUDIO_S16_NATIVE_STEREO AV_AUDIO_S16_LE_STEREO
#endif

/**
* @}
*/

/** @defgroup AVI_PROCESS_Private_Macros
* @{
*/
#define FCC(a,b,c,d) (((uint32_t)a << 24) |\
              ((uint32_t)b << 16) |\
              ((uint32_t)c << 8)  |\
              (uint32_t)d)
#define FCCS(str) FCC(str[0],str[1],str[2],str[3])

/**
* @}
*/

/** @defgroup AVI_PROCESS_Private_Variables
* @{
*/
static AVI_FormatTypeDef DecAviFmtStruct = { 0 };
static fnReadCallback_TypeDef* pAVIReadCallback = NULL;
static fnSetPositionCallback_TypeDef* pAVISetPosCallback = NULL;
static fnGetSizeCallback_TypeDef* pAVIGetSizeCallback = NULL;
static void *pAVIUserData = NULL;
#ifdef AUDIO_DEBUG
static uint32_t MaxDecodeDurationInTicks = 0;
#endif /* AUDIO_DEBUG */
/**
* @}
*/

/** @defgroup AVI_PROCESS_Private_Functions
* @{
*/

/* AVI Decoder Functions ******************************************************/
/* Prototypes */
static uint32_t AviProcess_ParseHeader(uint8_t* pHeader, AVI_FormatTypeDef* pAudioFormatDataStruct);
static uint32_t avi_find_chunk(AVI_FormatTypeDef *h, uint32_t id, long int *pos);
static long int avi_parse_header(AVI_FormatTypeDef *h, long int offset, int level);
static uint32_t avi_read_audio_data(AVI_FormatTypeDef *h, unsigned char *data, uint32_t r_size);
static void     avi_add_movi(AVI_FormatTypeDef *h, int level, long int start, long int size);

#ifdef AUDIO_CODEC_AVI_STATIC_ALLOC
#define MAX_MOVI        16
static struct movi_range  _movi[MAX_MOVI*16];
#endif /* AUDIO_CODEC_AVI_STATIC_ALLOC */

/**
* @brief  Parse the AVI file header and checks if it is valid
* @param  pHeader: pointer to the AVI file header table.
* @param  h: Pointer to the AVI structure to be filled.
* @retval 0 if AVI file is valid, !0 else.
*/
static uint32_t AviProcess_ParseHeader(uint8_t* pHeader, AVI_FormatTypeDef* h)
{
  long int pos, size;
  size = pAVIGetSizeCallback(pAVIUserData);

  for (pos = 0; pos < size;)
  {
    pos += avi_parse_header(h,pos,0);
  }

  /* audio stream ?? */
  if (h->movi)
  {
    h->a_pos = h->movi[0].start;
  }
  else if (h->wave.start)
  {
    h->a_pos = h->wave.start;
  }

  if (FCCS(h->a_strh.type) == FCC('a','u','d','s') ||
      FCCS(h->riff_type) == FCC('W','A','V','E'))
  {
    switch (h->auds.format)
    {
      case WAVE_FORMAT_PCM:
      {
        if (h->auds.size == 8)
        {
          h->afmt.fmtid = AV_AUDIO_U8_MONO;
        }
        if (h->auds.size == 16)
        {
          h->afmt.fmtid = AV_AUDIO_S16_LE_MONO;
        }
        if (h->afmt.fmtid)
        {
          if (h->auds.channels > 1)
          {
            h->afmt.fmtid++; /* mono => stereo */
          }
          h->afmt.rate = h->auds.rate;
        }
      }
      break;
    }
  }

  return (h->afmt.fmtid != AV_AUDIO_NONE) ? 0 : 1;
}

/**
* @brief  Parse the AVI file header and checks if it is valid
* @param  h: Pointer to the AVI structure to be filled.
* @param  offset: offset from where to start parsing the header data.
* @param  level: print indentation level (for debug purpose).
* @retval size of the valid parsed chunk, !0 else.
*/
static long int avi_parse_header(AVI_FormatTypeDef *h, long int offset, int level)
{
  struct CHUNK_HDR chunk;
  struct RIFF_strh strh;
  unsigned char id[4];
  long int pos = offset;

  pAVISetPosCallback(pAVIUserData,pos);
  pos += pAVIReadCallback(&chunk, sizeof(chunk), pAVIUserData);

  switch (FCCS(chunk.id))
  {
    case FCC('R','I','F','F'):
    case FCC('L','I','S','T'):
    {
       pos += pAVIReadCallback(&id, sizeof(id), pAVIUserData);
      if (FCCS(chunk.id) == FCC('R','I','F','F'))
          memcpy(h->riff_type,id,4);
      if (FCCS(id) == FCC('m','o','v','i')) {
          avi_add_movi(h,level,pos,chunk.size-4);
      } else {
          while (pos < offset + chunk.size)
          pos += avi_parse_header(h,pos,level+3);
      }
    }
    break;
    case FCC('a','v','i','h'):
    {
      pos += pAVIReadCallback(&h->avih, sizeof(h->avih), pAVIUserData);
    }
    break;
    case FCC('s','t','r','h'):
    {
      pAVIReadCallback(&strh, sizeof(strh), pAVIUserData);
      memcpy(h->fcc_type,strh.type,sizeof(h->fcc_type));
      if (FCCS(h->fcc_type) == FCC('a','u','d','s'))
      {
        h->a_strh = strh;
      }
      if (FCCS(h->fcc_type) == FCC('v','i','d','s'))
      {
        h->v_strh = strh;
      }
    }
    break;
    case FCC('s','t','r','f'):
    {
      if (FCCS(h->fcc_type) == FCC('a','u','d','s'))
      {
        pAVIReadCallback(&h->auds, sizeof(h->auds), pAVIUserData);
      }
      if (FCCS(h->fcc_type) == FCC('v','i','d','s'))
      {
        pAVIReadCallback(&h->vids, sizeof(h->vids), pAVIUserData);
      }
    }
    break;
    case FCC('d','m','l','h'):
    {
      pAVIReadCallback(&h->dml_frames, sizeof(h->dml_frames), pAVIUserData);
    }
    break;
    case FCC('f','m','t',' '):
    {
      if (FCCS(h->riff_type) == FCC('W','A','V','E'))
      {
        pAVIReadCallback(&h->auds, sizeof(h->auds), pAVIUserData);
      }
    }
    break;
    case FCC('d','a','t','a'):
    {
      if (FCCS(h->riff_type) == FCC('W','A','V','E'))
      {
        h->wave.start = pos;
        h->wave.size  = chunk.size-4;
      }
    }
    break;
  }
  return chunk.size+8;
}

/**
* @brief  Find valid Audio chunks
* @param  h: Pointer to the AVI structure to be filled.
* @param  id: offset from where to start parsing the header data.
* @param  pos: pointer to current parsing position.
* @retval size of the chunk's data, !0 else.
*/
static uint32_t avi_find_chunk(AVI_FormatTypeDef *h, uint32_t id, long int *pos)
{
  struct CHUNK_HDR chunk;
  int n = 0, bytes;

  if (NULL == h->movi)
  {
    /* WAVE */
    if (*pos >= h->wave.start + h->wave.size)
    {
      return 0;
    }
    pAVISetPosCallback(pAVIUserData,*pos);
    bytes = h->wave.start + h->wave.size - *pos;
    if (bytes > 64*1024)
    {
      bytes = 64*1024;
    }
    *pos += bytes;
    return bytes;
  }

  /* AVI + AVIX */
  while (*pos >= h->movi[n].start + h->movi[n].size)
  {
    n++;
    if (n >= h->movi_cnt)
    {
      return 0;
    }
  }

  for (;;)
  {
    if (*pos >= h->movi[n].start + h->movi[n].size)
    {
      n++;
      if (n >= h->movi_cnt)
      {
        return 0;
      }
      *pos = h->movi[n].start;
    }

    pAVISetPosCallback(pAVIUserData,*pos);
    bytes = pAVIReadCallback(&chunk, sizeof(chunk), pAVIUserData);
    if(bytes != sizeof(chunk))
    {
      return 0;
    }

    *pos += bytes;
    if (FCCS(chunk.id) == id)
    {
      return chunk.size;
    }

    /* Skip Video Frames */
    //if (FCCS(chunk.id) == FCC('0','0','d','c'))
    {
      *pos += chunk.size;
    }
    *pos = (*pos + 1) & ~0x01; /* 8-bit align */
  }
}

/**
* @brief  Read Audio data and fill destination buffer
* @param  h: Pointer to the AVI structure to be filled.
* @param  data: pointer to the audio buffer to fill.
* @param  r_size: size of data to read from file.
* @retval size of the chunk's data read from file, !0 else.
*/
static uint32_t avi_read_audio_data(AVI_FormatTypeDef *h, unsigned char *data, uint32_t r_size)
{
  static uint8_t *buf = NULL;
  static uint8_t *alloc_buf = NULL;
  static uint32_t size  = 0;
  static uint32_t bytes = 0;
  uint32_t _r_size = r_size;

  h->tmp_buf  = data;
  if(!buf)
  {
    size = avi_find_chunk(h,FCC('0','1','w','b'),&h->a_pos);
    if (0 == size)
    {
      return 0;
    }

    if(r_size < size)
    {
      /* allocate memory for frame read */
      buf = MEM_MALLOC(size);
      if(NULL == buf)
      {
        return 0;
      }
      h->tmp_buf = buf;
      alloc_buf = buf;
    }

    /* Read Audio Frame */
    h->a_pos += pAVIReadCallback(h->tmp_buf, size, pAVIUserData);
    h->a_bytes += size;
  }

  if(r_size < size)
  {
    if(bytes + r_size <= size)
    {
      memcpy(data, buf, r_size);
      bytes += r_size;
      buf += bytes;
    }
    else
    {
      /* copy part of data then read new frame */
      _r_size -= (size - bytes);
      memcpy(data, buf, (size - bytes));
      MEM_FREE((void *)alloc_buf);
      buf = 0;
      bytes = 0;

      /* allocate memory for frame read */
      buf = MEM_MALLOC(size);
      if(NULL == buf)
      {
        return 0;
      }
      h->tmp_buf = buf;
      alloc_buf = buf;

      size = avi_find_chunk(h,FCC('0','1','w','b'),&h->a_pos);
      if (0 == size)
      {
        return 0;
      }

      /* Read Audio Frame */
      h->a_pos += pAVIReadCallback(h->tmp_buf, size, pAVIUserData);
      h->a_bytes += size;

      memcpy(data, buf, _r_size);
      bytes += _r_size;
    }

    if(bytes >= size)
    {
      MEM_FREE((void *)alloc_buf);
      buf = 0;
    }
  }

  return r_size;
}

/**
* @brief  Add movi data
* @param  h: Pointer to the AVI structure to be filled.
* @param  level: print indentation level (for debug purpose).
* @param  start: start address of movi data to add.
* @param  size: size of movi data to add.
* @retval size of the chunk's data read from file, !0 else.
*/
static void avi_add_movi(AVI_FormatTypeDef *h,  int level,
            long int start, long int size)
{
#ifdef AUDIO_CODEC_AVI_STATIC_ALLOC
  if (h->movi_cnt > MAX_MOVI)
  {
#endif /* AUDIO_CODEC_AVI_STATIC_ALLOC */
    if (0 == h->movi_cnt % 16)
    {
      /* realloc implementation */
      void *p = NULL;
      p = MEM_MALLOC(sizeof(struct movi_range)*(h->movi_cnt+16));
      if (p)
      {
        /* zero the memory */
        if (h->movi != NULL)
        {
          memcpy(p, h->movi, sizeof(struct movi_range)*(h->movi_cnt+16));
          MEM_FREE((void *)h->movi);
        }
      }
      h->movi = (struct movi_range *)p;
    }
#ifdef AUDIO_CODEC_AVI_STATIC_ALLOC
  }
  else
  {
    h->movi = (struct movi_range *)_movi;
  }
#endif /* AUDIO_CODEC_AVI_STATIC_ALLOC */

  h->movi[h->movi_cnt].start = start;
  h->movi[h->movi_cnt].size  = size;

  h->movi_cnt++;
}

/**
* @}
*/

/** @defgroup AVI_PROCESS_Exported_Functions
* @{
*/
/**
* @brief  Decoder structure initialize.
* @param  pHeader: pointer to the audio file header tab.
* @param  pReadCallback: Read callback function.
* @param  pSetPosCallback: Set position callback function
* @retval None.
*/
uint32_t AviProcess_DecInit(uint8_t* pHeader,
                            fnReadCallback_TypeDef* pReadCallback,
                            fnSetPositionCallback_TypeDef* pSetPosCallback,
                            fnGetSizeCallback_TypeDef pGetSizeCallback,
                            void *pUserData)
{
  /* Parse the wav file header and extract required information */
  memset(&DecAviFmtStruct, 0, sizeof(AVI_FormatTypeDef));

  /* Get the upper layer callbacks for reading data and setting file position offset */
  pAVIReadCallback    = pReadCallback;
  pAVISetPosCallback  = pSetPosCallback;
  pAVIUserData        = pUserData;
  pAVIGetSizeCallback = pGetSizeCallback;

  pAVISetPosCallback(pAVIUserData, 0);
  if (AviProcess_ParseHeader(pHeader, &DecAviFmtStruct))
  {
    return 1;
  }

#ifdef AUDIO_DEBUG
  MaxDecodeDurationInTicks = 0;
#endif /* AUDIO_DEBUG */
  return 0;
}

/**
* @brief  Decoder structure de-initialize.
* @param  None.
* @retval 0 if nor error, 1 otherwise.
*/
uint32_t AviProcess_DecDeInit(void)
{
#ifndef AUDIO_CODEC_AVI_STATIC_ALLOC
  AVI_FormatTypeDef *h = &DecAviFmtStruct;
  if(h->movi)
  {
    MEM_FREE((void *)h->movi);
  }

  if(h->tmp_buf)
  {
    MEM_FREE((void *)h->tmp_buf);
  }
#endif /* !AUDIO_CODEC_AVI_STATIC_ALLOC */

  return 0;
}

/**
* @brief  Provide output pcm stream.
* @param  OutPutBuffer: output PCM.
* @param  NbSamples: number of pcm to be decoded.
* @param  none: NULL this parameter is kept in order to be on line with
*         mp3 decoder structures.
* @retval Number of decoded data (in bytes).
*/
uint32_t AviProcess_DecodeData(__IO int16_t* OutPutBuffer, uint32_t NbSamples, void * none)
{
  uint32_t size = 0x00;

  if (pAVIReadCallback != NULL)
  {
#ifdef AUDIO_DEBUG
    /* For Debug purpos */
    static uint32_t DecodeDurationInTicks = 0;
    uint32_t StartTick = HAL_GetTick();
#endif /* AUDIO_DEBUG */
    uint32_t read_size = 0;

    read_size = NbSamples * DecAviFmtStruct.a_strh.samplesize;

    do
    {
      int bytes = avi_read_audio_data(&DecAviFmtStruct, (unsigned char *)OutPutBuffer, read_size);
      if(bytes == 0)
      {
        break;
      }
      size += bytes;
    } while((size > 0) && (size < read_size));

    /* Transforms a 16-bit mono buffer to a 16-bit stereo buffer by duplicating all values */
    if (DecAviFmtStruct.auds.channels == 1)
    {
      size = size * 2;
      if (AviProcess_MonoToStereo((int16_t *)OutPutBuffer, (int16_t *)OutPutBuffer, read_size) != 0)
      {
        size = size/2;
      }
    }
#ifdef AUDIO_DEBUG
    DecodeDurationInTicks = HAL_GetTick() - StartTick;
    if(MaxDecodeDurationInTicks < DecodeDurationInTicks)
    {
      MaxDecodeDurationInTicks = DecodeDurationInTicks;
    }
#endif /* AUDIO_DEBUG */
  }

  return size;
}

/**
* @brief  Get the sampling rate of the current wav stream.
* @param  None.
* @retval Return the sampling rate.
*/
uint32_t AviProcess_DecGetSamplingRate(void)
{
  return DecAviFmtStruct.auds.rate;
}

/**
* @brief  Get the number of channels of the current stream.
* @param  None.
* @retval Return the number of channels.
*/
uint32_t AviProcess_GetNbChannels(void)
{
  return DecAviFmtStruct.auds.channels;
}

/**
* @brief  Get the number of bits of the current stream.
* @param  None.
* @retval Return the number of bits.
*/
uint32_t AviProcess_GetNbBits(void)
{
  return DecAviFmtStruct.auds.size;
}

/**
* @brief  Get the number of samples per frame of the current stream.
* @param  None.
* @retval Return the number of samples per frame.
*/
uint32_t AviProcess_GetNbSamplesFrame(void)
{
  return (DecAviFmtStruct.a_strh.length / DecAviFmtStruct.a_strh.samplesize);
}

/**
* @brief  Get the length of the current pcm stream in seconds.
* @param  fLength: File length.
* @retval return the stream length.
*/
uint32_t AviProcess_DecGetStreamLenght(uint32_t fLength)
{
  return (DecAviFmtStruct.a_strh.length/(DecAviFmtStruct.afmt.rate / DecAviFmtStruct.a_strh.scale));
}

/**
* @brief  Set a new progress time in seconds.
* @param  fLength: New progress time.
* @retval return 0.
*/
uint32_t AviProcess_DecSetProgressTime(uint32_t NewTime)
{
  uint32_t req_size = (NewTime * DecAviFmtStruct.a_strh.rate * DecAviFmtStruct.a_strh.samplesize) / DecAviFmtStruct.a_strh.scale;

  if(req_size < DecAviFmtStruct.a_bytes)
  {
    /* time in the past ; go back the initial position and drop frames */
    if (DecAviFmtStruct.movi)
    {
      DecAviFmtStruct.a_pos = DecAviFmtStruct.movi[0].start;
    }
    else if (DecAviFmtStruct.wave.start)
    {
      DecAviFmtStruct.a_pos = DecAviFmtStruct.wave.start;
    }

    /* Go to the start of audio payload */
    DecAviFmtStruct.a_bytes = 0;
    pAVISetPosCallback(pAVIUserData, DecAviFmtStruct.a_pos);
  }

  while (DecAviFmtStruct.a_bytes < req_size)
  {
    uint32_t size = avi_find_chunk(&DecAviFmtStruct, FCC('0','1','w','b'), &(DecAviFmtStruct.a_pos));
    if (0 == size)
    {
      break;
    }

    /* Drop Audio Frame */
    DecAviFmtStruct.a_pos   += size;
    DecAviFmtStruct.a_bytes += size;
  }

  /* Go to the new audio payload */
  pAVISetPosCallback(pAVIUserData, DecAviFmtStruct.a_pos);

  return 0;
}

/**
* @brief  Get the elapsed time of the current pcm stream.
* @param  CurPos: current position of the audio file pointer.
* @retval Return the elapsed stream time.
*/
uint32_t AviProcess_DecGetElapsedTime(uint32_t CurPos)
{
  uint32_t progress   = 0;
  uint32_t samples     = 0;

  samples = DecAviFmtStruct.a_bytes / DecAviFmtStruct.a_strh.samplesize;
  progress = samples / (DecAviFmtStruct.afmt.rate / DecAviFmtStruct.a_strh.scale);

  return progress;
}

/**
* @brief  Transforms a 16-bit mono buffer to a 16-bit stereo buffer by duplicating all values
* @param  BIn : pointer to the input frame
* @param  BOut : pointer to the output encoded frame (can be the same input buffer)
* @param  Size : Input buffer frame size
* @retval 0 if OK, 1 else.
*/
uint32_t AviProcess_MonoToStereo(int16_t* BIn, int16_t* BOut, uint32_t Size)
{
  int32_t i;

  if ((Size*2) <= AVI_PACKET_SZE_PROC)
  {
    for (i = (Size*2)-1; i >= 0; i -= 2)
    {
      BOut[i] = BIn[i/2];
      BOut[i - 1] = BIn[i/2];
    }
    return 0;
  }
  else
  {
    return 1;
  }
}
#endif /*  __AVI_DECODER__ */

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

