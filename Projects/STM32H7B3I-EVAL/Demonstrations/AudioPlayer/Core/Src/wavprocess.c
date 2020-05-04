/**
******************************************************************************
* @file    wavprocess.c
* @author  MCD Application Team
* @brief   This file includes wav processing layer routines
******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "wavprocess.h"

#if defined( __WAV_ENCODER__) || defined(__WAV_DECODER__)  /* Don't build if not configured for use in audio_app_conf.h */

/** @addtogroup STM32_audio_codecs
* @{
*/

/** @defgroup Wavprocess
* @brief WavProcess driver modules
* @{
*/

/* .WAV file format : ----------------------------------------------------------

Endian      Offset      Length      Contents
big         0           4 bytes     'RIFF'             -- 0x52494646
little      4           4 bytes     <file length - 8>
big         8           4 bytes     'WAVE'             -- 0x57415645

Next, the fmt chunk describes the sample format:

big         12          4 bytes     'fmt '             -- 0x666D7420
little      16          4 bytes     0x00000010         -- Length of the fmt data (16 bytes)
little      20          2 bytes     0x0001             -- Format tag: 1 = PCM
little      22          2 bytes     <channels>         -- Channels: 1 = mono, 2 = stereo
little      24          4 bytes     <sample rate>      -- Samples per second: e.g., 22050
little      28          4 bytes     <bytes/second>     -- sample rate * block align
little      32          2 bytes     <block align>      -- channels * bits/sample / 8
little      34          2 bytes     <bits/sample>      -- 8 or 16

Finally, the data chunk contains the sample data:

big         36          4 bytes     'data'             -- 0x64617461
little      40          4 bytes     <length of the data block>
little      44          *           <sample data>
--------------------------------------------------------------------------------
*/

/** @defgroup WavProcess_Private_TypesDefinitions
* @{
*/

/**
* @}
*/

/** @defgroup WavProcess_Private_Defines
* @{
*/

/**
* @}
*/

/** @defgroup WavProcess_Private_Macros
* @{
*/

/**
* @}
*/

/** @defgroup WavProcess_Private_Variables
* @{
*/
#ifdef __WAV_DECODER__
WAVE_FormatTypeDef DecWavFmtStruct;
fnReadCallback_TypeDef* pWavReadCallback;
fnSetPositionCallback_TypeDef* pWAVSetPosCallback;
#endif /* __WAV_DECODER__ */

#ifdef __WAV_ENCODER__
WAVE_FormatTypeDef EncWavFmtStruct;
uint32_t WavEncInited = 0;
#endif /* __WAV_ENCODER__ */

/**
* @}
*/

/** @defgroup WavProcess_Private_FunctionPrototypes
* @{
*/
#ifdef __WAV_DECODER__
static uint32_t ReadUnit(uint8_t NbrOfBytes, uint32_t BytesFormat, uint8_t* pBuff, uint32_t BuffIdx);
#endif /* __WAV_DECODER__ */

#ifdef __WAV_ENCODER__
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pAudioFormatDataStruct);
#endif /* __WAV_ENCODER__ */

/**
* @}
*/

/** @defgroup WavProcess_Private_Functions
* @{
*/

/* WAV Decoder Functions ******************************************************/
#ifdef __WAV_DECODER__

/**
* @brief  Decoder structure initialize.
* @param  pHeader: pointer to the audio file header tab.
* @param  pReadCallback: Read callback function.
* @param  pSetPosCallback: Set position callback function
* @retval None.
*/
uint32_t WavProcess_DecInit(uint8_t* pHeader,
                            fnReadCallback_TypeDef* pReadCallback,
                            fnSetPositionCallback_TypeDef* pSetPosCallback)
{
  /* Parse the wav file header and extract required information */
  if (WavProcess_ParseHeader(pHeader, &DecWavFmtStruct))
  {
    return 1;
  }

  /* Get the upper layer callbacks for reading data and setting file position offset */
  pWavReadCallback = pReadCallback;
  pWAVSetPosCallback = pSetPosCallback;

  /* Go the start of audio payload */
  pWAVSetPosCallback(DecWavFmtStruct.AudioStartAddr);

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
uint32_t WavProcess_DecodeData(__IO int16_t* OutPutBuffer, uint32_t NbSamples, void * none)
{
  uint32_t size = 0x00;

  if (pWavReadCallback != NULL)
  {
    size = pWavReadCallback((int8_t*)OutPutBuffer, (NbSamples * \
      (DecWavFmtStruct.BitsPerSample)/8 * \
        DecWavFmtStruct.NumChannels), \
          NULL);

    /* Transforms a 16-bit mono buffer to a 16-bit stereo buffer by duplicating all values */
    if (DecWavFmtStruct.NumChannels == 1)
    {
      size = size * 2;
      if (WavProcess_MonoToStereo((int16_t *)OutPutBuffer, (int16_t *)OutPutBuffer, \
        (NbSamples * (DecWavFmtStruct.BitsPerSample)/8 * \
          DecWavFmtStruct.NumChannels)) != 0)
      {
        size = 0;
      }
    }
  }

  return size;
}

/**
* @brief  Get the sampling rate of the current wav stream.
* @param  None.
* @retval Return the sampling rate.
*/
uint32_t WavProcess_DecGetSamplingRate(void)
{
  return (DecWavFmtStruct.SampleRate);
}




/**
* @brief  Get the number of channels of the current stream.
* @param  None.
* @retval Return the number of channels.
*/
uint32_t WavProcess_GetNbChannels(void)
{

  return (DecWavFmtStruct.NumChannels);
}

/**
* @brief  Get the number of bits of the current stream.
* @param  None.
* @retval Return the number of bits.
*/
uint32_t WavProcess_GetNbBits(void)
{

  return (DecWavFmtStruct.BitsPerSample);
}
/**
* @brief  Get the number of samples per frame of the current stream.
* @param  None.
* @retval Return the number of samples per frame.
*/
uint32_t WavProcess_GetNbSamplesFrame(void)
{

  return (DecWavFmtStruct.AudioLength);
}


/**
* @brief  Set a new progress time in seconds.
* @param  fLength: New progress time.
* @retval return 0.
*/
uint32_t WavProcess_DecSetProgressTime(uint32_t NewTime)
{
  uint32_t NewPos = 0;

  NewPos = DecWavFmtStruct.AudioStartAddr + (NewTime * (DecWavFmtStruct.SampleRate *
                       DecWavFmtStruct.NumChannels *
                      (DecWavFmtStruct.BitsPerSample/8)));

  /* Go the new file position */
  pWAVSetPosCallback(NewPos);

  return 0;
}

/**
* @brief  Get the length of the current pcm stream in seconds.
* @param  fLength: File length.
* @retval return the stream length.
*/
uint32_t WavProcess_DecGetStreamLenght(uint32_t fLength)
{
  return (uint32_t)(((fLength - DecWavFmtStruct.AudioStartAddr) /
                     ((DecWavFmtStruct.BitsPerSample/8) * DecWavFmtStruct.NumChannels)) /
                    DecWavFmtStruct.SampleRate);
}


/**
* @brief  Get the elapsed time of the current pcm stream.
* @param  CurPos: current position of the audio file pointer.
* @retval Return the elapsed stream time.
*/
uint32_t WavProcess_DecGetElapsedTime(uint32_t CurPos)
{
  return (CurPos / (DecWavFmtStruct.SampleRate *
                    DecWavFmtStruct.NumChannels *
                      (DecWavFmtStruct.BitsPerSample/8)));
}

/**
* @brief  Parse the wave file header and checks if it is valid
* @param  pHeader: pointer to the wave file header table.
* @param  pAudioFormatDataStruct: Pointer to the wave structure to be filled.
* @retval 0 if wave file is valid, !0 else.
*/
uint32_t WavProcess_ParseHeader(uint8_t* pHeader, WAVE_FormatTypeDef* pAudioFormatDataStruct)
{
  uint32_t Temp = 0x00;
  uint32_t ExtraFormatBytes = 0;
  uint32_t HeadTabIdx = 0;

  /* Read chunkID, must be 'RIFF' --------------------------------------------*/   // uint32_t ChunkID;       /* 0 */
  Temp = ReadUnit(4, BIG_ENDIAN, pHeader, HeadTabIdx);
  if (Temp != CHUNK_ID)
  {
    return(INVALID_RIFF_ID);
  }
  HeadTabIdx += 4;

  /* Read the file length ----------------------------------------------------*/    //uint32_t FileSize;      /* 4 */  ?????
  pAudioFormatDataStruct->RIFFchunksize = ReadUnit(4, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  HeadTabIdx += 4;

  /* Read the file format, must be 'WAVE' ------------------------------------*/  //uint32_t FileFormat;    /* 8 */
  Temp = ReadUnit(4, BIG_ENDIAN, pHeader, HeadTabIdx);
  if (Temp != FILE_FORMAT)
  {
    return(INVALID_WAVE_FORMAT);
  }
  HeadTabIdx += 4;

  /* Read the format chunk, must be 'fmt ' -----------------------------------*/  // uint32_t SubChunk1ID;   /* 12 */
  Temp = ReadUnit(4, BIG_ENDIAN, pHeader, HeadTabIdx);
  if (Temp != FORMAT_ID)
  {
    return(INVALID_FORMAT_CHUNK_ID);
  }
  HeadTabIdx += 4;

  /* Read the length of the 'fmt' data, must be 0x10 -------------------------*/  //uint32_t SubChunk1Size; /* 16*/   ???
  Temp = ReadUnit(4, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  if (Temp != 0x10)
  {
    ExtraFormatBytes = 1;
  }
  HeadTabIdx += 4;

  /* Read the audio format, must be 0x01 (PCM) -------------------------------*/  //uint16_t AudioFormat;   /* 20 */
  pAudioFormatDataStruct->FormatTag = ReadUnit(2, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  if (pAudioFormatDataStruct->FormatTag != WAVE_FORMAT_PCM)
  {
    return(UNSUPPORTED_FORMAT_TAG);
  }
  HeadTabIdx += 2;

  /* Read the number of channels: 0x02->Stereo 0x01->Mono --------------------*/  //uint16_t NbrChannels;   /* 22 */
  pAudioFormatDataStruct->NumChannels = ReadUnit(2, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  HeadTabIdx += 2;

  /* Read the Sample Rate ----------------------------------------------------*/  //uint32_t SampleRate;    /* 24 */
  pAudioFormatDataStruct->SampleRate = ReadUnit(4, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  HeadTabIdx += 4;

  /* Read the Byte Rate ------------------------------------------------------*/  // uint32_t ByteRate;      /* 28 */
  pAudioFormatDataStruct->ByteRate = ReadUnit(4, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  HeadTabIdx += 4;

  /* Read the block alignment ------------------------------------------------*/  //uint16_t BlockAlign;    /* 32 */
  pAudioFormatDataStruct->BlockAlign = ReadUnit(2, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  HeadTabIdx += 2;

  /* Read the number of bits per sample --------------------------------------*/  // uint16_t BitPerSample;  /* 34 */
  pAudioFormatDataStruct->BitsPerSample = ReadUnit(2, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  HeadTabIdx += 2;

  if (pAudioFormatDataStruct->BitsPerSample != BITS_PER_SAMPLE_16)
  {
#ifndef SUPORT_8BIT_SAMPLES
    return(UNSUPPORTED_BITS_PER_SAMPLE);
#endif /* SUPORT_8BIT_SAMPLES */
  }

  /* If there are Extra format bytes, these bytes will be defined in "Fact Chunk" */
  if (ExtraFormatBytes == 1)
  {
    /* Read the Extra format bytes here */
  }

  /* Read the Data chunk, must be 'data' -------------------------------------*/  //uint32_t SubChunk2ID;   /* 36 */
  while (ReadUnit(4, BIG_ENDIAN, pHeader, HeadTabIdx) != DATA_ID)
  {
    if (HeadTabIdx >= MAX_AUDIO_WAV_HEADER_SIZE)
    {
     return (INVALID_DATA_CHUNK_ID);
    }
    else
    {
      HeadTabIdx++;
    }
  }

  HeadTabIdx += 4;

  /* Read the number of sample data ------------------------------------------*/  //uint32_t SubChunk2Size; /* 40 */
  pAudioFormatDataStruct->AudioLength = ReadUnit(4, LITTLE_ENDIAN, pHeader, HeadTabIdx);
  HeadTabIdx += 4;

  /* Set the data pointer at the beginning of the effective audio data */
  pAudioFormatDataStruct->AudioStartAddr = HeadTabIdx;

  return(VALID_WAVE_FILE);
}

/**
* @brief  Reads a number of bytes from a buffer and return them
* @param  NbrOfBytes : number of bytes to read. This parameter must be a
*         number between 1 and 4.
* @param  BytesFormat : specifies the bytes endianness.
*         This parameter can be one of the following values: LITTLE_ENDIAN or
*         BIG_ENDIAN
* @param  pBuff : memory address to read from.
* @param  BuffIdx : Buffer index.
* @retval Bytes read from the buffer..
*/
static uint32_t ReadUnit(uint8_t NbrOfBytes, uint32_t BytesFormat, uint8_t* pBuff, uint32_t BuffIdx)
{
  uint32_t index = 0;
  uint32_t tmp = 0;
  uint32_t Idx = BuffIdx;

  if (BytesFormat == LITTLE_ENDIAN)
  {
    for (index = 0; index < NbrOfBytes; index++)
    {
      tmp |= pBuff[Idx++] << (index * 8);
    }
  }
  else
  {
    for (index = NbrOfBytes; index != 0; index--)
    {
      tmp |= pBuff[Idx++] << ((index - 1) * 8);
    }
  }

  return tmp;
}

/**
* @brief  Transforms a 16-bit mono buffer to a 16-bit stereo buffer by duplicating all values
* @param  BIn : pointer to the input frame
* @param  BOut : pointer to the output encoded frame (can be the same input buffer)
* @param  Size : Input buffer frame size
* @retval 0 if OK, 1 else.
*/
uint32_t WavProcess_MonoToStereo(int16_t* BIn, int16_t* BOut, uint32_t Size)
{
  int32_t i;

  if ((Size*2) <= WAV_PACKET_SZE_PROC)
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

#endif /* __WAV_DECODER__ */

/* WAV Encoder Functions ******************************************************/

#ifdef __WAV_ENCODER__

/**
* @brief  Encoder initialization.
* @param  Freq: Sampling frequency.
* @param  pHeader: Pointer to the WAV file header to be written.
* @retval 0 if success, !0 else.
*/
uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t* pHeader)
{
  if (WavEncInited)
  {
    return 1;
  }

  /* Initialize the encoder structure */
  EncWavFmtStruct.SampleRate = Freq;        /* Audio sampling frequency */
  EncWavFmtStruct.NumChannels = 2;          /* Number of channels: 1:Mono or 2:Stereo */
  EncWavFmtStruct.BitsPerSample = 16;       /* Number of bits per sample (16, 24 or 32) */
  EncWavFmtStruct.AudioLength = 0;          /* Total length of useful audio data (payload) */
  EncWavFmtStruct.AudioStartAddr = 44;      /* Relative start address of audio payload */
  EncWavFmtStruct.RIFFchunksize = 44;       /* The file header chunk size */
  EncWavFmtStruct.FormatTag = 1;            /* Audio file format: PCM = 1 */
  EncWavFmtStruct.ByteRate = (EncWavFmtStruct.SampleRate * \
    (EncWavFmtStruct.BitsPerSample/8) * \
      EncWavFmtStruct.NumChannels);            /* Number of bytes per second  (sample rate * block align)  */
  EncWavFmtStruct.BlockAlign = EncWavFmtStruct.NumChannels * \
    (EncWavFmtStruct.BitsPerSample/8);      /* channels * bits/sample / 8 */
  EncWavFmtStruct.DataSize = 0;             /* Total length of useful audio data (payload) */

  /* Parse the wav file header and extract required information */
  if (WavProcess_HeaderInit(pHeader, &EncWavFmtStruct))
  {
    return 1;
  }

  WavEncInited = 1;

  return 0;
}

/**
* @brief  Deinitializes the Wav Encoder.
* @param  None.
* @retval None.
*/
uint32_t WavProcess_EncDeInit(void)
{
  if (WavEncInited)
  {
    WavEncInited = 0;

    return EncWavFmtStruct.AudioLength;
  }
  else
  {
    return 1;
  }
}

/**
* @brief  Encoder process.
* @param  pBufIn: Pointer to input buffer.
* @param  pBufOut: Pointer to output buffer.
* @param  nEnc: Not used.
* @param  pOpt: Not used.
* @retval None.
*/
uint32_t WavProcess_EncodeData(int8_t *pBufIn, int8_t *pBufOut, uint32_t* nEnc, void* pOpt)
{
  if (WavEncInited)
  {
    /* Retrun the same buffer without processing */
    *(uint32_t*)pBufOut = (uint32_t)pBufIn;

    /* Update the total audio data size */
    EncWavFmtStruct.AudioLength += *nEnc;
  }

  return 0;
}

/**
* @brief  Get the sample rate.
* @param  None.
* @retval Current Sample Rate.
*/
uint32_t WavProcess_EncGetSampleRate (void)
{
  if (WavEncInited)
  {
    return EncWavFmtStruct.SampleRate;
  }
  else
  {
    return 0;
  }
}

/**
* @brief  Get the sample rate.
* @param  None.
* @retval Current stream length.
*/
uint32_t WavProcess_EncGetStreamLength (void)
{
  if (WavEncInited)
  {
    return EncWavFmtStruct.AudioLength;
  }
  else
  {
    return 0;
  }
}

/**
* @brief  Get the sample rate.
* @param  None.
* @retval Elapsed time in seconds.
*/
uint32_t WavProcess_EncGetElapsedTime (void)
{
  if (WavEncInited)
  {
    return ((EncWavFmtStruct.AudioLength) /
            (EncWavFmtStruct.SampleRate *
             EncWavFmtStruct.NumChannels *
               (EncWavFmtStruct.BitsPerSample/8)));
  }
  else
  {
    return 0;
  }
}

/**
* @brief  Initialize the wave header file
* @param  pHeader: Header Buffer to be filled
* @param  pAudioFormatDataStruct: Pointer to the wave structure to be filled.
* @retval 0 if passed, !0 if failed.
*/
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pAudioFormatDataStruct)
{
  /* write chunkID, must be 'RIFF'  ------------------------------------------*/
  pHeader[0] = 'R';
  pHeader[1] = 'I';
  pHeader[2] = 'F';
  pHeader[3] = 'F';

  /* Write the file length ----------------------------------------------------*/
  /* The sampling time: this value will be be written back at the end of the
  recording opearation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = 0xFC;
  pHeader[5] = 0x17;
  pHeader[6] = 0x0A;
  pHeader[7] = 0x00;

  /* Write the file format, must be 'WAVE' -----------------------------------*/
  pHeader[8]  = 'W';
  pHeader[9]  = 'A';
  pHeader[10] = 'V';
  pHeader[11] = 'E';

  /* Write the format chunk, must be'fmt ' -----------------------------------*/
  pHeader[12]  = 'f';
  pHeader[13]  = 'm';
  pHeader[14]  = 't';
  pHeader[15]  = ' ';

  /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
  pHeader[16]  = 0x10;
  pHeader[17]  = 0x00;
  pHeader[18]  = 0x00;
  pHeader[19]  = 0x00;

  /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
  pHeader[20]  = 0x01;
  pHeader[21]  = 0x00;

  /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
  pHeader[22]  = pAudioFormatDataStruct->NumChannels;
  pHeader[23]  = 0x00;

  /* Write the Sample Rate in Hz ---------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  pHeader[24]  = (uint8_t)((pAudioFormatDataStruct->SampleRate & 0xFF));
  pHeader[25]  = (uint8_t)((pAudioFormatDataStruct->SampleRate >> 8) & 0xFF);
  pHeader[26]  = (uint8_t)((pAudioFormatDataStruct->SampleRate >> 16) & 0xFF);
  pHeader[27]  = (uint8_t)((pAudioFormatDataStruct->SampleRate >> 24) & 0xFF);

  /* Write the Byte Rate -----------------------------------------------------*/
  pHeader[28]  = (uint8_t)((pAudioFormatDataStruct->ByteRate & 0xFF));
  pHeader[29]  = (uint8_t)((pAudioFormatDataStruct->ByteRate >> 8) & 0xFF);
  pHeader[30]  = (uint8_t)((pAudioFormatDataStruct->ByteRate >> 16) & 0xFF);
  pHeader[31]  = (uint8_t)((pAudioFormatDataStruct->ByteRate >> 24) & 0xFF);

  /* Write the block alignment -----------------------------------------------*/
  pHeader[32]  = pAudioFormatDataStruct->BlockAlign;
  pHeader[33]  = 0x00;

  /* Write the number of bits per sample -------------------------------------*/
  pHeader[34]  = pAudioFormatDataStruct->BitsPerSample;
  pHeader[35]  = 0x00;

  /* Write the Data chunk, must be 'data' ------------------------------------*/
  pHeader[36]  = 'd';
  pHeader[37]  = 'a';
  pHeader[38]  = 't';
  pHeader[39]  = 'a';

  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  pHeader[40]  = 0xFC;
  pHeader[41]  = 0x17;
  pHeader[42]  = 0x0A;
  pHeader[43]  = 0x00;

  /* Return 0 if all operations are OK */
  return 0;
}

/**
* @brief  Initialize the wave header file
* @param  pHeader: Header Buffer to be filled
* @param  pWaveFormatStruct: Pointer to the wave structure to be filled.
* @retval 0 if passed, !0 if failed.
*/
uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct, uint32_t* fptr)
{
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the
  recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = (uint8_t)(*fptr);
  pHeader[5] = (uint8_t)(*fptr >> 8);
  pHeader[6] = (uint8_t)(*fptr >> 16);
  pHeader[7] = (uint8_t)(*fptr >> 24);

  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  *fptr -=44;
  pHeader[40] = (uint8_t)(*fptr);
  pHeader[41] = (uint8_t)(*fptr >> 8);
  pHeader[42] = (uint8_t)(*fptr >> 16);
  pHeader[43] = (uint8_t)(*fptr >> 24);

  /* Return 0 if all operations are OK */
  return 0;
}

#endif /* __WAV_ENCODER__ */

#endif /*  __WAV_ENCODER__ | __WAV_DECODER__ */

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
