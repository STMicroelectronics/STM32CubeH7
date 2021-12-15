/**
******************************************************************************
* @file    mp3process.c
* @author  MCD Application Team
* @brief   This file includes MP3 processing layer routines
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
#include "mp3process.h"

/** @addtogroup AUDIO_PROCESS
* @{
*/

/** @defgroup MP3_PROCESS
* @brief Audio MP3 Processing routines
* @{
*/

#if defined( __MP3_ENCODER__) || defined(__MP3_DECODER__)  /* Don't build if not configured for use in audio_conf.h */

/** @defgroup MP3_PROCESS_Private_Constants
* @{
*/
#define MP3_HEADER_SIZE_POSITION        0x06

#define MP3_VBR_FLAGS_FRAMES_PRESENT    0x00000001  /* Frames Flag    : set if value for number of frames in file is stored */
#define MP3_VBR_FLAGS_SIZE_PRESENT      0x00000002  /* Bytes Flag     : set if value for filesize in Bytes is stored        */
#define MP3_VBR_FLAGS_TOC_PRESENT       0x00000004  /* TOC Flag       : set if values for TOC (see below) are stored        */
#define MP3_VBR_FLAGS_SCALE_PRESENT     0x00000008  /* VBR Scale Flag : set if values for VBR scale are stored              */
/**
* @}
*/

/** @defgroup MP3_PROCESS_Private_Variables
* @{
*/
/* Decoder object pointer */
TSpiritMP3Decoder  *MP3Decoder_Instance;

/* Current Stream Info */
TSpiritMP3Info     mp3_Info;

/* Data offset */
uint32_t DataStartOffset = 0x00;
/* BitRateSave*/
__IO uint16_t LastnBitrateKbps = 0x00, VBR_Detect = 0x00, VBR_Flags = 0x00;
uint32_t NumberOfFrames = 0, NumberOfBytes = 0;

uint8_t TableOfContents[100]; /* For correct file seeking support when VBI is detected */
uint32_t VBR_Scale = 0;

int32_t *pcm_buffer1;
uint8_t pbufferID[256];

/**
* @}
*/

/** @defgroup MP3_PROCESS_Private_FunctionPrototypes
* @{
*/
#ifdef __MP3_DECODER__
fnSetPositionCallback_TypeDef* pMP3SetPosCallback;
static uint32_t MP3Process_CheckForVBR(uint8_t *pHeader, uint32_t NbData);
#endif /* __MP3_DECODER__ */

/**
* @}
*/

/** @defgroup MP3_PROCESS_Private_Functions
* @{
*/
/****** MP3 DECODER ***********************************************************/
#ifdef __MP3_DECODER__

/**
* @brief  Decoder structure initialize.
* @param  pHeader: pointer to the audio file header tab.
* @param  pReadCallback: Read callback function.
* @param  pSetPosCallback: Set position callback function
* @retval None.
*/
uint32_t Mp3Process_DecoderInit(uint8_t* pHeader,
                                fnReadCallback_TypeDef* pReadCallback,
                                fnSetPositionCallback_TypeDef* pSetPosCallback)
{
  __IO uint16_t time_out = 1153;
  uint8_t tmpBuffer[10];
  uint8_t nb_frame = 0;

  uint32_t i;

  /* Enables and resets CRC-32 from STM32 HW */
  __HAL_RCC_CRC_CLK_ENABLE();
  CRC->CR = CRC_CR_RESET;

  /* Allocate space for the MP3 decoder instance */
  MP3Decoder_Instance = (TSpiritMP3Decoder*)MP3_DEC_MALLOC(sizeof(TSpiritMP3Decoder)); /* 0x3038: 12344 */

  if (MP3Decoder_Instance == NULL)
  {
    return 0xFF;
  }

  /* Temporary buffer */
  pcm_buffer1 = MP3_DEC_MALLOC((1152<<1) * sizeof(int16_t));  /* 0x1200: 4600 */
  if (pcm_buffer1 == NULL)
  {
    MP3_DEC_FREE(MP3Decoder_Instance);
    MP3Decoder_Instance = NULL;
    return 0xFE;
  }
  /* Initialize the MP3 info structure */
  for (i=0; i < sizeof(TSpiritMP3Info); i++)
  {
    *(uint8_t*)(((uint32_t)&mp3_Info) + i) = 0;
  }

  /* check for headers */
  for (i=0; i < 10; i++)
  {
    tmpBuffer[i] = *(uint8_t*)(((uint32_t)(&pHeader[0])) + i);
  }

  if (tmpBuffer[0] == 'I' && tmpBuffer[1] == 'D' && tmpBuffer[2] == '3')
  {
    /* check for data start offset. */
    DataStartOffset =  ((tmpBuffer[MP3_HEADER_SIZE_POSITION] & 0x7f) << 21)
      | ((tmpBuffer[MP3_HEADER_SIZE_POSITION + 1] & 0x7f) << 14)
        | ((tmpBuffer[MP3_HEADER_SIZE_POSITION + 2] & 0x7f) << 7)
          | (tmpBuffer[MP3_HEADER_SIZE_POSITION + 3] & 0x7f);
  }
  else
  {
    DataStartOffset = 0;
  }

  if (&mp3_Info != NULL)
  {
    /* Initialize the decoder */
    SpiritMP3DecoderInit(MP3Decoder_Instance,
                         pReadCallback ,
                         NULL,
                         NULL);

    pMP3SetPosCallback = pSetPosCallback;

    /* Set cursor to the actual data position */
    pMP3SetPosCallback(DataStartOffset);

    /* MP3 decoder needs to process more than one frame to be sure that headers are correctly decoded */
    do
    {
      SpiritMP3Decode(
                      (TSpiritMP3Decoder*)(MP3Decoder_Instance), /* Decoder structure */
                      (int16_t*)pcm_buffer1,                      /* [OUT] Output PCM buffer */
                      1152,                                       /* Output PCM buffer size in samples */
                      (TSpiritMP3Info*)(&mp3_Info)                /* [OUT] Audio Information structure */
                        );

      /* check for two good frames */
      if (!(!((&mp3_Info)->IsGoodStream) || ((&mp3_Info)->nBitrateKbps == 0) || ((&mp3_Info)->nSampleRateHz == 0)))
      {
        nb_frame++;
      }
    }
    while ( (nb_frame < 1) && (time_out-- != 0));

    /* Set cursor to the actual data position */
    pMP3SetPosCallback(DataStartOffset);

    pReadCallback(pcm_buffer1, ((1152<<1) * sizeof(int16_t)), 0);

    if(MP3Process_CheckForVBR((uint8_t *)pcm_buffer1, ((1152<<1) * sizeof(int16_t))) != 0)
    {
      VBR_Detect = LastnBitrateKbps = 0x00;
    }
  }
  else
  {
    MP3_DEC_FREE(MP3Decoder_Instance);
    MP3Decoder_Instance = NULL;
  }

  /* Free the pcm_buffer1 before exit */
  if (pcm_buffer1 != NULL)
  {
    MP3_DEC_FREE(pcm_buffer1);
    pcm_buffer1 = NULL;
  }

  if (MP3Decoder_Instance != NULL)
  {
    memset(MP3Decoder_Instance, 0, sizeof(TSpiritMP3Decoder));

    /* Re-initialize the decoder */
    SpiritMP3DecoderInit(MP3Decoder_Instance,
                         pReadCallback ,
                         NULL,
                         NULL);

    /* Set cursor to the actual data position */
    pMP3SetPosCallback(DataStartOffset);
  }
  /* Check if the current stream is correct */
  return (mp3_Info.IsGoodStream == 0)? 1:0;
}

/**
* @brief  Deinitializes the MP3 Decoder structure.
* @param  None.
* @retval None.
*/
uint32_t Mp3Process_DecoderDeInit(void)
{
  /* Free all used resources */
  if (MP3Decoder_Instance != NULL)
  {
    MP3_DEC_FREE(MP3Decoder_Instance);
    MP3Decoder_Instance = NULL;
  }

  if (pcm_buffer1 != NULL)
  {
    MP3_DEC_FREE(pcm_buffer1);
    pcm_buffer1 = NULL;
  }

  return 0;
}

/**
* @brief  Provide output PCM stream.
* @param  OutPutBuffer: Output PCM.
* @param  NbSamples: number of pcm to be decoded.
* @param  Usr: Not used.
* @retval Number of decoded data in bytes.
*/
uint32_t Mp3Process_DecodeData(__IO int16_t* OutPutBuffer, uint32_t NbSamples, void * Usr)
{
  uint32_t tmp = 0x00;

  if (MP3Decoder_Instance == NULL)
  {
    return 0;
  }
  /* Start MP3 decoding */
  tmp = SpiritMP3Decode(
                        (TSpiritMP3Decoder*)MP3Decoder_Instance, /* Decoder structure */
                        (int16_t*)OutPutBuffer,                  /*[OUT] Output PCM buffer */
                        NbSamples,                               /* Output PCM buffer size in samples */
                        &mp3_Info                                /* [OUT] Audio Information structure */
                          );

  /* Check the Current bitrate */
  if ((mp3_Info.IsGoodStream == 0x01) && (VBR_Detect == 0x00))
  {
    if (LastnBitrateKbps == 0x00)
    {
      LastnBitrateKbps = mp3_Info.nBitrateKbps;
    }
    else
    {
      if ((LastnBitrateKbps != mp3_Info.nBitrateKbps))
      {
        VBR_Detect = 0x01;
      }
    }
  }
  return tmp*4;
}

/**
* @brief  retrieve mp3 file parameters.
* @param  None.
* @retval pointer to 'TSpiritMP3Info' struct.
*/
TSpiritMP3Info * MP3Process_GetFileInfoStruct(void)
{
  return ( TSpiritMP3Info *)&mp3_Info;
}

/**
* @brief  Provide the tag of the current stream.
* @param  pIDTAG: Pointer to the Tag structure.
* @param  pFile: Pointer to data file.
* @retval None.
*/
uint32_t Mp3Process_RetrieveIDTAGS(TAGS_TypeDef * pIDTAG, FIL *pFile)
{
  uint32_t i;

  /* pbuffer initialization */
  for (i= 0; i< 256; i++)
  {
    pbufferID[i] = 0;
  }

  if (pFile != NULL)
  {
    f_lseek(pFile, 0);
    SongUtilities_MP3TagParser(pbufferID, pIDTAG, pFile);
    f_lseek(pFile, 0);
  }

  return 0;
}

/**
* @brief  Parse the MP3 file header and checks if VBR is valid
* @param  pHeader: pointer to the wave file header table.
* @param  NbData: number of data to be parsed.
* @retval 0 if VBR is valid, !0 else.
*/
static uint32_t MP3Process_CheckForVBR(uint8_t *pHeader, uint32_t NbData)
{
  uint32_t herderindex = 0, VBRheader = 0;

  NumberOfFrames = 0;
  NumberOfBytes  = 0;
  VBR_Detect     = 0;

  do
  {
    if(pHeader[herderindex] == 'X')
    {
      if((pHeader[herderindex] == 'X') && (pHeader[herderindex + 1] == 'i') &&
         (pHeader[herderindex + 2] == 'n') && (pHeader[herderindex + 3] == 'g'))
      {
        VBRheader = VBR_Xing;
        break;
      }
    }
    else if(pHeader[herderindex] == 'I')
    {
      if((pHeader[herderindex] == 'I') && (pHeader[herderindex + 1] == 'n') &&
         (pHeader[herderindex + 2] == 'f') && (pHeader[herderindex + 3] == 'o'))
      {
        VBRheader = VBR_Info;
        break;
      }
    }
    else if(pHeader[herderindex] == 'V')
    {
      if((pHeader[herderindex] == 'V') && (pHeader[herderindex + 1] == 'B') &&
         (pHeader[herderindex + 2] == 'R') && (pHeader[herderindex + 3] == 'I'))
      {
        VBRheader = VBR_VBRI;
        break;
      }
    }
    herderindex++;
  }while(herderindex < NbData);

  /* Check for VBR header ID in 4 ASCII chars, either 'Xing' or 'Info' */
  if((VBRheader == VBR_Xing) || (VBRheader == VBR_Info))
  {
    /* Extract Flags */
    VBR_Flags = ((pHeader[herderindex + 4] << 24) | (pHeader[herderindex + 5] << 16) |
                      (pHeader[herderindex + 6] << 8) | pHeader[herderindex + 7]);

    if(VBR_Flags)
    {
      if(VBR_Flags & MP3_VBR_FLAGS_FRAMES_PRESENT)
      {
        NumberOfFrames = ((pHeader[herderindex + 8] << 24) | (pHeader[herderindex + 9] << 16) |
                          (pHeader[herderindex + 10] << 8) | pHeader[herderindex + 11]);
      }

      if(VBR_Flags & MP3_VBR_FLAGS_SIZE_PRESENT)
      {
        NumberOfBytes = ((pHeader[herderindex + 12] << 24) | (pHeader[herderindex + 13] << 16) |
                         (pHeader[herderindex + 14] << 8) | pHeader[herderindex + 15]);
      }

      if(VBR_Flags & MP3_VBR_FLAGS_TOC_PRESENT)
      {
        memcpy(TableOfContents, &pHeader[herderindex + 16], sizeof(TableOfContents));
      }

      if(VBR_Flags & MP3_VBR_FLAGS_SCALE_PRESENT)
      {
        VBR_Scale = ((pHeader[herderindex + 16] << 24) | (pHeader[herderindex + 17] << 16) |
                         (pHeader[herderindex + 18] << 8) | pHeader[herderindex + 20]);
      }

      VBR_Detect = 1;
    }
  }

  /* Check for VBRI header ID in 4 ASCII chars, always 'VBRI' */
  if (VBRheader == VBR_VBRI)
  {
    NumberOfBytes = ((pHeader[herderindex + 10] << 24) | (pHeader[herderindex + 11] << 16) |
                     (pHeader[herderindex + 12] << 8) | pHeader[herderindex + 13]);

    NumberOfFrames = ((pHeader[herderindex + 14] << 24) | (pHeader[herderindex + 15] << 16) |
                      (pHeader[herderindex + 16] << 8) | pHeader[herderindex + 17]);
    VBR_Detect = 1;
  }

  return (VBR_Detect == 1) ? 0 : 1;
}

/**
* @brief  Set a new progress time in seconds.
* @param  fLength: New progress time.
* @retval return 0.
*/
uint32_t Mp3Process_DecSetProgressTime(uint32_t NewTime)
{
  if ((&mp3_Info)->nBitrateKbps != 0)
  {
    uint32_t NewPos = 0;

    if ((VBR_Detect == 1) && (NumberOfBytes != 0))
    {
      if(VBR_Flags & MP3_VBR_FLAGS_TOC_PRESENT)
      {
        /* Use TOC to get correct position  : seek point in bytes = (toc[idx]/256.0) * total_bitstream_bytes */
        uint32_t idx = (100 * NewTime * mp3_Info.nSampleRateHz) / (NumberOfFrames * mp3_Info.nSamplesPerFrame);

        NewPos = (TableOfContents[idx] * NumberOfBytes)/256;
      }
      else
      {
        NewPos = (DataStartOffset + (NumberOfBytes - ((NewTime * mp3_Info.nSampleRateHz) / (NumberOfFrames * mp3_Info.nSamplesPerFrame))));
      }
    }
    else
    {
      NewPos = (DataStartOffset + (((NewTime * mp3_Info.nBitrateKbps) * 1024) / 8));
    }

    /* Go the new file position */
    pMP3SetPosCallback(NewPos);
    
    return 0;
  }

  return 1;
}
                         
/**
* @brief  Get the length of the current stream.
* @param  fLength: File length.
* @retval return the stream length.
*/
uint32_t Mp3Process_GetStreamLenght(uint32_t fLength)
{
  if (mp3_Info.nBitrateKbps != 0)
  {
    if (VBR_Detect == 1)
    {
      /* Update start offset */
      DataStartOffset = fLength - NumberOfBytes;

      /* Duration = Number of Frames * Samples Per Frame / Sampling Rate */
      return ((NumberOfFrames * mp3_Info.nSamplesPerFrame) / mp3_Info.nSampleRateHz);
    }
    else
    {
      return ((((fLength - DataStartOffset) * 8 ) / 1024) / (mp3_Info.nBitrateKbps ));
    }
  }
  else
  {
    return 0;
  }
}

/**
* @brief  Get the elapsed time of the current stream.
* @param  CurPos: Current position of pointer in the audio stream.
* @retval return the elapsed stream time in seconds.
*/
uint32_t Mp3Process_GetElapsedTime(uint32_t CurPos)
{
  if ((&mp3_Info)->nBitrateKbps != 0)
  {
    if (CurPos >= DataStartOffset)
    {
      if ((VBR_Detect == 1) && (NumberOfBytes != 0))
      {
        return (((CurPos - DataStartOffset) * ((NumberOfFrames * mp3_Info.nSamplesPerFrame) / mp3_Info.nSampleRateHz))/ NumberOfBytes);
      }
      else
      {
        return ((((CurPos - DataStartOffset) * 8  ) / 1024) / ((&mp3_Info)->nBitrateKbps ));
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

/**
* @brief  Variable bit rate detect.
* @param  None.
* @retval return 0 if no VBR is detected else !0.
*/
uint32_t Mp3Process_VBRDetected(void)
{
  return VBR_Detect;
}

/**
* @brief  Get the sampling rate of the current stream.
* @param  None.
* @retval Return the sampling rate.
*/
uint32_t Mp3Process_GetSamplingRate(void)
{

  return (&mp3_Info)->nSampleRateHz;
}

/**
* @brief  Get the Current bit rate (kbps) of the current stream.
*         kilobit per seconds (0 for free-format stream)
* @param  None.
* @retval Return the bit rate.
*/
uint32_t Mp3Process_GetBitRateKbps(void)
{
  return (&mp3_Info)->nBitrateKbps;
}

/**
* @brief  Get the number of channels of the current stream.
* @param  None.
* @retval Return the number of channels.
*/
uint32_t Mp3Process_GetNbChannels(void)
{

  return (&mp3_Info)->nChannels;
}

/**
* @brief  Get the number of bits read after last frame Header of the current stream.
* @param  None.
* @retval Return the number of bits read after frame header.
*/
uint32_t Mp3Process_GetNbBitsReadAfterFrameHeader(void)
{

  return (&mp3_Info)->nBitsReadAfterFrameHeader;
}
/**
* @brief  Get the number of samples per frame of the current stream.
* @param  None.
* @retval Return the number of samples per frame.
*/
uint32_t Mp3Process_GetNbSamplesFrame(void)
{

  return (&mp3_Info)->nSamplesPerFrame;
}

/**
* @brief  Forward the current stream
* @param  None.
* @retval None.
*/
uint32_t Mp3Process_StreamForward(void)
{
  /* N/A */

  return 0;
}

/**
* @brief  Backward the current stream
* @param  None.
* @retval None.
*/
uint32_t Mp3Process_StreamBackward(void)
{
  /* NA */
  return 0;
}

/**
* @brief  Stop file decoding.
* @param  None.
* @retval None.
*/
uint32_t Mp3Process_StopMode(void)
{
  /* NA */
  return 0;
}
#endif /* __MP3_DECODER__ */

/****** MP3 ENCODER ***********************************************************************/
#ifdef __MP3_ENCODER__

#include "spiritMP3Enc.h"

TMP3E_persist* pMP3EncPersistent = NULL;
TMP3E_scratch* pMP3EncScratch = NULL;

uint32_t MP3EncAudioLength = 0;
uint32_t MP3EncSampleRate  = 8000;

uint32_t MP3EncNumChannels = 2;
uint32_t MP3EncBitRate     = 48000;
/**
* @brief  MP3 Encoder initialization.
* @param  Freq: Sampling frequency.
* @param  Param: Not used.
* @retval 0 if success, !0 else.
*/
uint32_t Mp3Process_EncoderInit (uint32_t Freq, uint8_t* Param)
{
  /* Enable the CRC */
#ifdef STM32F10X_CL
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_CRC, ENABLE);
#else
  RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
#endif /* STM32F10X_CL */

  /* Update sample freq */
  MP3EncSampleRate = Freq;

  pMP3EncPersistent = MP3_ENC_MALLOC(MP3E__SizeofPersistent()); /* 8300 */
  if(pMP3EncPersistent == NULL)
  {
    return 0xFF;
  }

  pMP3EncScratch = MP3_ENC_MALLOC(MP3E__SizeofScratch()); /* 7400 */
  if(pMP3EncScratch == NULL)
  {
    MP3_ENC_FREE(pMP3EncPersistent);
    pMP3EncPersistent = NULL;
    return 0xFF;
  }

  /*
  uiSampleRateHz:
  48000, 44100, 32000 (MPEG-1)
  24000, 22050, 16000 (MPEG-2)
  12000, 11025,  8000 (MPEG-2.5)
  iBitrateKbps:
  32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 for MPEG-1 sampling rates
  8,  16, 24, 32, 40, 48, 56, 64,  80,  96,  112, 128, 144, 160 for MPEG-2 sampling rates
  8,  16, 24, 32, 40, 48, 56, 64 for MPEG-2.5 sampling rates
  */

  if (MP3E__Init(pMP3EncPersistent,
                 MP3EncSampleRate,       /* Sample rate */
                 MP3EncBitRate/1000,     /* Bitrate, Kbps */
                 MP3EncNumChannels,      /* Number of input channels */
                 0))                     /* Cut-off freq Hz */

  {
    return 0xFE;
  }


  MP3EncAudioLength = 0;

  return 0;
}

/**
* @brief  Deinitializes the MP3 Encoder.
* @param  None.
* @retval None.
*/
uint32_t Mp3Process_EncoderDeInit (void)
{
  /* Free allocated resources */
  if (pMP3EncPersistent != NULL)
  {
    MP3_ENC_FREE(pMP3EncPersistent);
    pMP3EncPersistent = NULL;
  }

  if (pMP3EncScratch != NULL)
  {
    MP3_ENC_FREE(pMP3EncScratch);
    pMP3EncScratch = NULL;
  }

  MP3EncAudioLength = 0;

  return 0;
}

/**
* @brief  Encoder process.
* @param  pBufIn: Pointer to input buffer.
* @param  pBufOut: Pointer to output buffer.
* @param  nEnc: Number of bytes in the output buffer.
* @param  pOpt: Not used.
* @retval None.
*/
uint32_t Mp3Process_EncodeData (int8_t *pBufIn, int8_t *pBufOut, uint32_t* nEnc, void* pOpt)
{

  /* Encode PCM data and get the MP3 frame */
  *(uint32_t*)pBufOut = (uint32_t) MP3E__Encode576( pMP3EncPersistent,
                                                   pMP3EncScratch,
                                                   (const short   *) pBufIn,
                                                   (const short   *) ((uint32_t)pBufIn + 2),
                                                   MP3EncNumChannels,
                                                   (unsigned int  *)nEnc);

  /* Update the total audio data size */
  /*MP3EncAudioLength += *nEnc;*/
  MP3EncAudioLength += MP3IN_PACKET_SZE;

  return 0;
}

/**
* @brief  Stop Encoder process.
* @param  None.
* @retval None.
*/
uint32_t Mp3Process_EncoderStopMode (void)
{
  return 0;
}

/**
* @brief  Set MP3 tags.
* @param  IdTags: pointer to the Tag structure.
* @param  file: pointer to the data file.
* @retval None.
*/
uint32_t Mp3Process_SetTags (TAGS_TypeDef* IdTags, void *file)
{
  return 0;
}

/**
* @brief  Get the sampling rate of the current MP3 stream.
* @param  None.
* @retval Return the sampling rate.
*/
uint32_t Mp3Process_EncGetSampleRate  (void)
{
  return MP3EncSampleRate;
}

/**
* @brief  Get the length of the current stream in seconds.
* @param  None.
* @retval return the stream length.
*/
uint32_t Mp3Process_EncGetStreamLength(void)
{
  return MP3EncAudioLength;
}

/**
* @brief  Get the elapsed time of the current MP3 stream.
* @param  None.
* @retval Return the elapsed stream time.
*/
uint32_t Mp3Process_EncGetElapsedTime (void)
{
  return (MP3EncAudioLength /
          (MP3EncSampleRate *
           MP3EncNumChannels *
             (16/8)));
}

#endif /* __MP3_ENCODER__ */

#endif /* __MP3_ENCODER__ | __MP3_DECODER__ */
/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

