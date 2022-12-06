/**
  ******************************************************************************
  * @file    player.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio Out (playback) interface API
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
#include "audio_player_app.h"
#include "cmsis_os.h"

/** @addtogroup AUDIO_PLAYER_CORE
* @{
*/

/** @defgroup AUDIO_CORE
* @brief Audio Player Core
* @{
*/
/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Input Audio File*/
FIL AudioFile;
Audio_InfoTypeDef AudioFormatData;
/* MemPool: Audio handler variables */
AUDIO_HANDLE_t AudioHandle; /* instance */
AUDIO_HANDLE_t *hptr;  /* pointer on instance */
uint32_t OutDecPacketSizeBytes = MAX_OUT_PACKET_SZE; /* in bytes */
/* Audio Decoder structure instance when present */
Decoder_TypeDef sDecoderStruct;

/* Input buffer size for MemPool read */
int32_t sizeAvailable;    /* MemPool available size in bytes*/

/* Input Audio File parameters */
static char FileExtension = ' ';

/* Double BUFFER for Output Audio stream */
/* as Buffer location should be aligned to cache line size (32 bytes) */
#if defined ( __CC_ARM )  /* !< ARM Compiler */
/* Buffer location should aligned to cache line size (32 bytes) */
ALIGN_32BYTES (static AUDIO_OUT_BufferTypeDef  BufferCtl) __attribute__((section(".RAM_D3")));
#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
#pragma location=0x38000000
/* Buffer location should aligned to cache line size (32 bytes) */
ALIGN_32BYTES (static AUDIO_OUT_BufferTypeDef  BufferCtl);
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
/* Buffer location should aligned to cache line size (32 bytes) */
ALIGN_32BYTES (static AUDIO_OUT_BufferTypeDef  BufferCtl) __attribute__((section(".RAM_D3")));
#endif

/* SRC: Sample Rate Converter variables */
static uint8_t SrcTypeSelected;     /* 236, 441, None */
static uint8_t SrcIterations_Frame; /* number of SRC iterations per frame */
static uint32_t AudioReadSize;      /* number of bytes to retrieve from µSD*/
/*BUFFERs for Audio Module interface*/
static  buffer_t InputBuffer;
static  buffer_t *pInputBuffer = &InputBuffer;
static  buffer_t OutputBuffer;
static  buffer_t *pOutputBuffer = &OutputBuffer;
/* Buffer used to store the audio file header */
static uint8_t tHeaderTmp[MAX_AUDIO_HEADER_SIZE];
/* MemPool Write: In Buffer to read audio file */
static int8_t Buffer1[MAX_OUT_PACKET_SZE];
/* MemPool Read: Out Buffer for Audio module processing */
static uint8_t WavToSrcTmpBuffer[SRC236_FRAME_SIZE_MAX*2*2]; /* SRC_FRAME_SIZE (240>147) * 2 (bytes x sample) * 2 (stereo) */

/* Private function prototypes -----------------------------------------------*/
static uint8_t PLAYER_StreamInit(uint32_t AudioFreq, uint32_t Device, uint32_t BitsPerSample, uint32_t ChannelsNbr, uint32_t Volume);
static AUDIO_ErrorTypeDef PLAYER_GetFileHeaderInfo(char* file_name, Audio_InfoTypeDef *info);
static AUDIO_ErrorTypeDef PLAYER_TransfertFromMemPool(uint16_t Offset_frames,  uint16_t length_frames);
/* Audio SRC module parameters for memory allocation*/
void *pSrc236PersistentMem;
void *pSrc441PersistentMem;
void *pSrcScratchMem;

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Initializes memory used by audio interface.
* @param  None
* @retval player error
*/
AUDIO_ErrorTypeDef PLAYER_MEMPOOL_Init(void)
{
  uint32_t src_scratch_mem_size;

  /* Allocate mem for SRC236 and SRC411 */
  pSrc236PersistentMem = PLAYER_MALLOC(src236_persistent_mem_size);  /* 0x1EC: 492  */
  pSrc441PersistentMem = PLAYER_MALLOC(src441_persistent_mem_size);  /* 0x0E8: 232 */
  if ((pSrc236PersistentMem == NULL) || (pSrc441PersistentMem == NULL))
  {
    PLAYER_FREE (pSrc236PersistentMem);
    pSrc236PersistentMem = NULL;
    PLAYER_FREE (pSrc441PersistentMem);
    pSrc441PersistentMem = NULL;
    return AUDIO_ERROR_MEM;
  }

  if (src236_scratch_mem_size > src441_scratch_mem_size)
  {
    src_scratch_mem_size = src236_scratch_mem_size;    /* 0x784:1924 */
  }
  else
  {
    src_scratch_mem_size = src441_scratch_mem_size;    /* 0xC9C: 3228*/
  }
  pSrcScratchMem = PLAYER_MALLOC(src_scratch_mem_size);

  if (pSrcScratchMem == NULL)
  {
    PLAYER_FREE (pSrcScratchMem);
    pSrcScratchMem = NULL;
    return AUDIO_ERROR_MEM;
  }

  /* MemPool: RAM dynamic memory allocation,
  Allocate SRAM memory MemPool structure */
  AudioHandle.pMemPool = (FWK_MEMPOOL_t*) PLAYER_MALLOC(sizeof(FWK_MEMPOOL_t)); /* 0x10: 16 */

  if (AudioHandle.pMemPool == NULL)
  {
    PLAYER_FREE(AudioHandle.pMemPool);
    AudioHandle.pMemPool = NULL;
    return AUDIO_ERROR_MEM;
  }
  /* Allocate SRAM memory for MemPool Buffer: at least 3 times Max output audio decoder packet */
  AudioHandle.pMemPool->pBuff = (uint8_t*) PLAYER_MALLOC(MEMPOOLBUFSZE);    /* 0x9000: 36864 */
  if(AudioHandle.pMemPool->pBuff == NULL)
  {
    PLAYER_FREE(AudioHandle.pMemPool->pBuff);
    AudioHandle.pMemPool->pBuff = NULL;
    return AUDIO_ERROR_MEM;
  }
  FWK_MEMPOOL_Init(AudioHandle.pMemPool, MEMPOOLBUFSZE);

  /* Pointer to Audio handler structure*/
  hptr = (AUDIO_HANDLE_t*)&AudioHandle;

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Initializes the Audio player.
* @param  AudioFreq: Audio sampling frequency
* @retval player error
*/
static uint8_t PLAYER_StreamInit(uint32_t AudioFreq, uint32_t Device, uint32_t BitsPerSample, uint32_t ChannelsNbr, uint32_t Volume)
{
  BSP_AUDIO_Init_t AudioInit;

  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */

  AudioInit.Device = Device;
  AudioInit.SampleRate = AudioFreq;
  AudioInit.BitsPerSample = BitsPerSample;
  AudioInit.ChannelsNbr = ChannelsNbr;
  AudioInit.Volume = Volume;
  if(BSP_AUDIO_OUT_Init(AUDIO_OUT_INSTANCE, &AudioInit) != 0)
  {
    return AUDIO_ERROR_IO;
  }
  else
  {
    return AUDIO_ERROR_NONE;
  }

}


/**
* @brief  Starts Audio streaming.
* @param  file_name: audio file name
* @param  volume: Volume level to be set in percentage from 0% to 100% (0 for
*         Mute and 100 for Max volume level).
* @retval player error
*/
AUDIO_ErrorTypeDef PLAYER_Start(char * file_name, uint32_t volume)
{
  AUDIO_ErrorTypeDef error = AUDIO_ERROR_NONE;
  src236_static_param_t src236_static_param;
  src441_static_param_t src441_static_param;
  uint32_t src_input_frame_size;
  uint32_t src_error;
  uint8_t SrcIterations; /* total number of SRC iterations */


  if(file_name != NULL)  {

    /* reset audio player buffers */
    PLAYER_ResetBuffers();

    /* Select decoder type and Get info from header */
    memset(&AudioFormatData,0x00,sizeof(Audio_InfoTypeDef));

    error = PLAYER_GetFileHeaderInfo(file_name, &AudioFormatData);
    if(error != AUDIO_ERROR_NONE)
    {
      return AUDIO_ERROR_CODEC;
    }

    /* Input stereo audio only */
    if(AudioFormatData.NbrChannels != 2)
    {
      f_close(&AudioFile);
      return (AUDIO_ERROR_INVALID_VALUE);
    }

    /* Input frame size to read from MemPool before SRC processing to get 480 samples output */
    switch(AudioFormatData.SampleRate)
    {
    case 8000:
      src236_static_param.src_mode = SRC236_RATIO_6;
      SrcTypeSelected = SELECT_SRC_236;
      SrcIterations_Frame = 1;
      SrcIterations = SrcIterations_Frame*FACTOR_BUFFER_INC;
      src_input_frame_size = AUDIO_OUT_BUFFER_SIZE/(8*SrcIterations*6);
      break;
    case 16000:
      src236_static_param.src_mode = SRC236_RATIO_3;
      SrcTypeSelected = SELECT_SRC_236;
      SrcIterations_Frame = 1;
      SrcIterations = SrcIterations_Frame*FACTOR_BUFFER_INC;
      src_input_frame_size = AUDIO_OUT_BUFFER_SIZE/(8*SrcIterations*3);
      break;
    case 22050:
      src236_static_param.src_mode = SRC236_RATIO_2;
      SrcTypeSelected = SELECT_SRC_236;
      SrcIterations_Frame = 1;
      SrcIterations = SrcIterations_Frame*FACTOR_BUFFER_INC;
      src_input_frame_size = AUDIO_OUT_BUFFER_SIZE/(8*SrcIterations*2);
      break;
    case 32000:
      src236_static_param.src_mode = SRC236_RATIO_3_2;
      SrcTypeSelected = SELECT_SRC_236;
      SrcIterations_Frame = 2;
      SrcIterations = SrcIterations_Frame*FACTOR_BUFFER_INC;   /* frame size smaller but processing repeated 2 times */
      src_input_frame_size = AUDIO_OUT_BUFFER_SIZE/(8*SrcIterations*3/2);
      break;
    case 44100:
      /* src441_static_param does not have params to be configured */
      SrcTypeSelected = SELECT_SRC_441;
      SrcIterations_Frame = 3;
      SrcIterations = SrcIterations_Frame*FACTOR_BUFFER_INC;   /* frame size smaller but processing repeated 3 times */
      src_input_frame_size = (AUDIO_OUT_BUFFER_SIZE/480)*441/(8*SrcIterations);
      break;
    case 48000:
      SrcTypeSelected = SELECT_SRC_NONE;
      SrcIterations_Frame = 2;
      SrcIterations = SrcIterations_Frame*FACTOR_BUFFER_INC;   /* frame size smaller but processing repeated 2 times considering SRC236 input req.*/
      src_input_frame_size = AUDIO_OUT_BUFFER_SIZE/(8*SrcIterations); /* half buff, stereo, byte x sample */
      break;
    case 96000:
      src236_static_param.src_mode = SRC236_RATIO_1_2;
      SrcTypeSelected = SELECT_SRC_236;
      SrcIterations_Frame = 4;
      SrcIterations = SrcIterations_Frame*FACTOR_BUFFER_INC;  /* frame size smaller but processing repeated 4 times */
      src_input_frame_size = AUDIO_OUT_BUFFER_SIZE/(8*SrcIterations*1/2);
      break;
    default:
      f_close(&AudioFile);
      return (AUDIO_ERROR_INVALID_VALUE);
    }

    /* Buffer size in number of bytes used to read MemPool */
    AudioReadSize = src_input_frame_size*4; /* stereo & byte x sample */

    if (SrcTypeSelected == SELECT_SRC_236)
    {
      /* SRC236 effect reset */
      src_error = src236_reset(pSrc236PersistentMem, pSrcScratchMem);
      if (src_error != SRC236_ERROR_NONE)
      {
        return (AUDIO_ERROR_SRC);
      }
      /* SRC236 effect static parameters setting */
      src_error = src236_setParam(&src236_static_param, pSrc236PersistentMem);
      if (src_error != SRC236_ERROR_NONE)
      {
        return (AUDIO_ERROR_SRC);
      }
    }

    if (SrcTypeSelected == SELECT_SRC_441)
    {
      /* SRC236 effect reset */
      src_error = src441_reset(pSrc441PersistentMem, pSrcScratchMem);
      if (src_error != SRC441_ERROR_NONE)
      {
        return (AUDIO_ERROR_SRC);
      }
      /* SRC236 effect static parameters setting */
      src_error = src441_setParam(&src441_static_param, pSrc441PersistentMem);
      if (src_error != SRC441_ERROR_NONE)
      {
        return (AUDIO_ERROR_SRC);
      }
    }

    /* Buffers used for audio module interface after MemPool */
    InputBuffer.data_ptr = &WavToSrcTmpBuffer;
    InputBuffer.nb_bytes_per_Sample = AudioFormatData.BitPerSample/8;  /* 8 bits in 0ne byte */
    InputBuffer.nb_channels = AudioFormatData.NbrChannels;
    InputBuffer.mode = INTERLEAVED;
    InputBuffer.buffer_size = src_input_frame_size;                    /* in number of samples */

    OutputBuffer.nb_bytes_per_Sample = AudioFormatData.BitPerSample/8; /* 8 bits in 0ne byte */
    OutputBuffer.nb_channels = AudioFormatData.NbrChannels;
    OutputBuffer.mode = INTERLEAVED;
    OutputBuffer.buffer_size = AUDIO_OUT_BUFFER_SIZE/8; /* half buff of stereo samples in bytes (ie 1 frame 10m in samples) */

     /* Adjust the Audio frequency codec is always 48Khz for this application*/
    if (PLAYER_StreamInit(AUDIO_FREQUENCY_48K, AUDIO_OUT_DEVICE_HEADPHONE, AudioFormatData.BitPerSample, AudioFormatData.NbrChannels, volume) != 0)
    {
      return AUDIO_ERROR_CODEC;
    }

    /* Final Output double buffer structure */
    BufferCtl.state = PLAY_BUFFER_OFFSET_NONE;
    /*eqv read file pointer on output */
    BufferCtl.fptr = 0;

    /* WAV format */
    if (FileExtension =='V') /*bypass header: only for Audio files WAV format*/
    {
      /* Get Data from audio file and Remove Wave format header */
      f_lseek(&AudioFile, 44);
    }

    /* Loop1 : Write MemPool and Fill Buffer1 from Audio file at start */
    PLAYER_TransfertToMemPool();

    /*Loop2: Process MemPool and Fill complete double output buffer at first time */
    error = PLAYER_TransfertFromMemPool(0,FACTOR_BUFFER_INC);
    if ( error == AUDIO_ERROR_NONE)
    {
      error = PLAYER_TransfertFromMemPool(FACTOR_BUFFER_INC,FACTOR_BUFFER_INC);
    }

    /* Loop1 if new available space  */
    PLAYER_TransfertToMemPool();

    /*  Play complete double output buffer at first time */
    if ( error == AUDIO_ERROR_NONE)
    {
      if(BufferCtl.fptr != 0)
      {
#if CLEAN_DCACHE
        /* Clean Data Cache to update the content of the SRAM */
        SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);
#endif

        /*DMA stream from output double buffer to codec in Circular mode launch*/
        BSP_AUDIO_OUT_Play(AUDIO_OUT_INSTANCE, (uint8_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);
        return AUDIO_ERROR_NONE;
      }
    }
  }
  return AUDIO_ERROR_IO;
}

/**
* @brief  Compute available size in MemPool
* @param  None
* @retval sizeAvailable
*/
int32_t  PLAYER_GetAvailableSizeMemPool(void)
{
  /* Check if pool state is not full */
  if (FWK_MEMPOOL_GetState(hptr->pMemPool) != FWK_MEMPOOL_STATUS_FULL)
  {
    /* Check if pool state is not empty */
    if (FWK_MEMPOOL_GetState(hptr->pMemPool) == FWK_MEMPOOL_STATUS_EMPTY)
    {
      sizeAvailable = hptr->pMemPool->buffSze; /* Empty case*/
    }
    else
    {
      FWK_MEMPOOL_GetAvailableSize(hptr->pMemPool, &sizeAvailable); /* Other case */
    }
  }
  else
  {
    sizeAvailable = 0; /* Full case */
  }

  return sizeAvailable;
}


/**
* @brief  Manages Loop1 Audio process.
*         Write MemPool and Fill Buffer1 from Audio file
* @param  None
* @retval player error
*/
AUDIO_ErrorTypeDef PLAYER_TransfertToMemPool(void)
{
  uint32_t DecSampNbBytes = 0; /* bytes per sample */
  uint32_t DecNumChannels = 0; /* stereo channels*/
  uint32_t decsize = 0;            /* returned output decoder size in bytes*/

  /* Loop1: Write MemPool and Fill Buffer1 from Audio file at start */

  PLAYER_GetAvailableSizeMemPool();

  while (sizeAvailable >= OutDecPacketSizeBytes)
  {
    /* Audio Decoder between Audio File (MST) and Buffer1 then MemPool writing */
    if (sDecoderStruct.Decoder_DecodeData != NULL)
    {
      DecSampNbBytes=AudioFormatData.BitPerSample/8;    /* 2 bytes*/
      DecNumChannels= AudioFormatData.NbrChannels;      /* stereo*/
      /*OutDecPacketSizeBytes: frame packet to decode (in bytes)*/

      decsize = sDecoderStruct.Decoder_DecodeData((__IO int16_t*)(&Buffer1[0]), (OutDecPacketSizeBytes/(DecNumChannels*DecSampNbBytes)), NULL);
    }

    if(decsize != 0)
    {
      /*write Buffer1 to MemPool*/
      FWK_MEMPOOL_Write(hptr->pMemPool, (uint8_t*)Buffer1, decsize); /* input buffer1 in bytes */
    }
    else /* End of file detected*/
    {
     return AUDIO_ERROR_NONE;
    }

    /* Update available size in MemPool */
    PLAYER_GetAvailableSizeMemPool();
  }

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Reset All buffers.
* @param  None
* @retval player
*/
uint32_t PLAYER_ResetBuffers(void)
{
  uint32_t i =0;

  /* Reset MemPool buffer */
  FWK_MEMPOOL_Init(AudioHandle.pMemPool,MEMPOOLBUFSZE);

  /* Reset header buffer*/
  for (i = 0; i < MAX_AUDIO_HEADER_SIZE; i++)
  {
    tHeaderTmp[i] = 0;
  }

  /* Reset output double buffer*/
  for (i = 0; i < AUDIO_OUT_BUFFER_SIZE; i++)
  {
    BufferCtl.buff[i] = 0;
  }

  return 0;
}

/**
* @brief  Close the decoder instance
* @param  None
* @retval player error
*/
AUDIO_ErrorTypeDef PLAYER_Close(void)
{

  if (sDecoderStruct.DecoderDeInit != NULL)
  {
    sDecoderStruct.DecoderDeInit();
  }

  /* Empty the decoder structure */
  CODERS_SelectDecoder(&sDecoderStruct, ' ');

  /* Close audio file */
   f_close(&AudioFile);

  return AUDIO_ERROR_NONE;

}

/**
* @brief  This function Manage data processing (fill output buffer, refill MemPool...)
*         requested at DMA Transfer interrupts.
* @param  uint16_t  audio data offset in samples
* @retval player error
*/
AUDIO_ErrorTypeDef PLAYER_OutputBufferCbkProc(uint16_t offset) /* in samples */
{
  AUDIO_ErrorTypeDef audio_error = AUDIO_ERROR_NONE;

  audio_error = PLAYER_TransfertFromMemPool((((offset/WAV_PACKET_SZE))% NBFrames_OutputBuf), FACTOR_BUFFER_INC); /* in frames */

  return audio_error;
}

/**
* @brief  Gets the file info from header.
* @param  file_name: audio file name
* @param  info: Pointer to Audio file (MP3, WAV)
* @retval player error
*/
static AUDIO_ErrorTypeDef PLAYER_GetFileHeaderInfo(char * file_name, Audio_InfoTypeDef *info)
{
  uint32_t bytesread;
  uint8_t  *header;
  uint8_t  error_dec=0;

  /* Header pointer allocation*/
  header = tHeaderTmp;

  /* Open Audio File */
  if(f_open(&AudioFile, file_name, FA_OPEN_EXISTING | FA_READ) != FR_OK)
  {
    return AUDIO_ERROR_IO;
  }

  /* Get the extension of audio file */
  FileExtension =  file_name[strlen(file_name) - 1];


  /* Read Header File information */
  if(f_read(&AudioFile, header, MAX_AUDIO_HEADER_SIZE, (void *)&bytesread) != FR_OK)
  {
    f_close(&AudioFile);
    return AUDIO_ERROR_IO;
  }

  /* Initialize the decoder Abs layer structure instance with matching decoder decoder or Null pointers */
  if(CODERS_SelectDecoder(&sDecoderStruct, FileExtension) != 0)
  {
    return AUDIO_ERROR_CODEC;
  }

  /* Use the most suitable packet size */
  OutDecPacketSizeBytes = sDecoderStruct.PacketSize;

  /* Initialization of the decoder */
  if (sDecoderStruct.DecoderInit != NULL)
  {
    error_dec=sDecoderStruct.DecoderInit((uint8_t*)header,Dec_ReadDataCallback, PLAYER_SetFilePosition);

    if (error_dec == 0)
    {
      /* ++ Fill the "info" structure with audio format parameters after decoding++ */

      /* Extract the current sampling rate */
      if (sDecoderStruct.Decoder_GetSamplingRate != NULL)
      {
        info->SampleRate = sDecoderStruct.Decoder_GetSamplingRate();
      }
      info->FileSize = PLAYER_GetFileLength();

      /* ++ MP3 ++ */
      if (FileExtension=='3')
      {
        info->NbrChannels = sDecoderStruct.Decoder_GetNbChannels();

        info->FileFormat ='3';   /* MP3 */
        info->AudioFormat ='P';  /* PCM */

        info->BitPerSample = AUDIO_RESOLUTION_16B;

        /*After decoding: ((NumberOfFrames * mp3_Info.nSamplesPerFrame))/ NumberOfBytes;*/
        info->ByteRate =sDecoderStruct.Decoder_GetBitRateKbps()*1024/8;

        info->Duration = sDecoderStruct.Decoder_GetStreamLength(info->FileSize);

      }
      /* ++ WAVE ++ */
      else if ((FileExtension=='v')||(FileExtension=='V'))
      {
        info->NbrChannels = sDecoderStruct.Decoder_GetNbChannels();

        info->FileFormat = 'V';   /* WAVE */
#ifdef __WAV_DECODER__
        if (DecWavFmtStruct.FormatTag == 1)
        {
          info->AudioFormat = 'P';
        }
        else
        {
          return AUDIO_ERROR_INVALID_VALUE;
        }
#else
        info->AudioFormat = 'P';
#endif
        info->BitPerSample =sDecoderStruct.Decoder_GetNbBits();
        info->ByteRate = (sDecoderStruct.Decoder_GetSamplingRate())*(sDecoderStruct.Decoder_GetNbChannels())*(sDecoderStruct.Decoder_GetNbBits())/8; /*raw PCM data*/
        info->Duration = sDecoderStruct.Decoder_GetStreamLength(info->FileSize);
      }

      else
      {
        return AUDIO_ERROR_INVALID_VALUE;
      }
    }
    else
    {
      return AUDIO_ERROR_CODEC;
    }
  }

  return AUDIO_ERROR_NONE;
}
/**
* @brief  Read a number of samples from MemPool Buffer such that after SRC conversion
*         stereo samples are copied into Output buffer and sent to codec through DMA.
*         At same time, it reads data from audio file and refill MemPool buffer.
* @param  offset_frames: returns to the caller the bytes read from audio file
* @param  length_frames: first flip flop buffer or second flip flop buffer
* @retval error: error type (default AUDIO_ERROR_NONE)
*/

static AUDIO_ErrorTypeDef PLAYER_TransfertFromMemPool(uint16_t offset_frames,  uint16_t length_frames) /* at frames number */
{
  uint32_t bytesread;
  uint32_t src_error;
  uint16_t i, j;

  /* frames number */
  uint16_t frames_size1, frames_size2, frames_end;
  uint16_t frames_buffer_size, samples_per_frame, SrcIterations_per_frame;

  /* samples number */
  uint16_t offset, size1, size2;

  /* Compute params in frames number */
  frames_buffer_size = (2*FACTOR_BUFFER_INC);
  samples_per_frame= WAV_PACKET_SZE ;
  SrcIterations_per_frame= SrcIterations_Frame;
  frames_end = (offset_frames+length_frames);

  if (frames_end > frames_buffer_size){
    frames_size2=(frames_end%frames_buffer_size);
    frames_size1=(length_frames-frames_size2);
  }
  else{
    frames_size1=length_frames;
    frames_size2=0;
  }

  /* Compute params in samples number  */
  offset=offset_frames*samples_per_frame; /*in bytes*/
  size1=frames_size1*samples_per_frame;   /*in bytes*/
  size2=frames_size2*samples_per_frame;   /*in bytes*/

  /* Bypass SRC processing */
  if (SrcTypeSelected == SELECT_SRC_NONE)
  {
    for (j = 0; j < length_frames; ++j) /* loop on length_frames */
    {

      if ( j < frames_size1 )
      {
        /* Read from memory pool1 to temporary module processing computation buffer */
        if(FWK_MEMPOOL_Read(hptr->pMemPool, (uint8_t*)&BufferCtl.buff[offset+j*samples_per_frame] , samples_per_frame)!= FWK_MEMPOOL_ERROR_NONE)
        {
          return (AUDIO_ERROR_IO);
        }
      }
      else
      {
        /* Read from memory pool1 to temporary module processing computation buffer */
        if(FWK_MEMPOOL_Read(hptr->pMemPool, (uint8_t*)&BufferCtl.buff[0+(j-frames_size1)*samples_per_frame] , samples_per_frame)!= FWK_MEMPOOL_ERROR_NONE)

        {
          return (AUDIO_ERROR_IO);
        }
      }
      /*-----------------------------------------------------------------------------------------*/
      /* Loop1 processing: Compute sizeAvailable in MemPool and activate SWI for MemPool writing */
      PLAYER_TransfertToMemPool();
      /*-----------------------------------------------------------------------------------------*/
    } /* for length_frames */

    bytesread=(size1+size2); /*force value*/
    BufferCtl.fptr += bytesread;

  }
  /* Selected SRC processing */
  else
  {
    for (j = 0; j < length_frames; ++j) /* for length_frames */
    {
      for (i = 0; i < SrcIterations_per_frame; ++i) /* for 1 frame */
      {

        if ( j < frames_size1 )
        {
          OutputBuffer.data_ptr = &BufferCtl.buff[offset + (i+(j*SrcIterations_per_frame))*(samples_per_frame /SrcIterations_per_frame)];

        }
        else
        {
          OutputBuffer.data_ptr = &BufferCtl.buff[0 + (i+((j-frames_size1)*SrcIterations_per_frame))*(samples_per_frame /SrcIterations_per_frame)];

        }

        /* Read from memory pool1 to temporary module processing computation buffer*/

        if(FWK_MEMPOOL_Read(hptr->pMemPool, (uint8_t*)&WavToSrcTmpBuffer, AudioReadSize)== FWK_MEMPOOL_ERROR_NONE)
        {
          bytesread=AudioReadSize;
          BufferCtl.fptr += bytesread;

          if (SrcTypeSelected == SELECT_SRC_236)/* SrcTypeSelected == 236 */
          {
            src_error = src236_process(pInputBuffer, pOutputBuffer, pSrc236PersistentMem);
            if (src_error != SRC236_ERROR_NONE)
            {
              return (AUDIO_ERROR_SRC);
            }
          }
          else /* SrcTypeSelected == 441 */
          {
            src_error = src441_process(pInputBuffer, pOutputBuffer, pSrc441PersistentMem);
            if (src_error != SRC441_ERROR_NONE)
            {
              return (AUDIO_ERROR_SRC);
            }
          }
        }
        else
        {
          return (AUDIO_ERROR_IO);
        }
      } /* for i on 1 frame */

      /* Loop1 processing: Compute sizeAvailable in MemPool and activate SWI for MemPool writing */
      PLAYER_TransfertToMemPool();

    } /* for j on length_frames */

  } /*if SrcTypeSelected...*/


#if CLEAN_DCACHE
  if (frames_size2 == 0){
    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[offset], size1); /* in samples */
  }
  else{
    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[offset], size1); /* in samples */
    SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[0], size2);      /* in samples */
  }
#endif

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Sets the current position of the audio file pointer.
* @param  Position: audio file position to be set
* @retval player error
*/
uint32_t PLAYER_SetFilePosition(uint32_t Position)
{
  if(Position != AudioFile.fptr)
  {
    f_lseek(&AudioFile, Position);
  }

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Get file size
* @param  None
* @retval file size in bytes
*/
uint32_t PLAYER_GetFileLength(void)
{
  /* Call the Fat FS seek function */
  return f_size(&AudioFile);
}


/**
* @brief  Updates the current time information (expressed in seconds).
* @param  Length: pointer to the variable containing the total audio track length
* @param  Total: pointer to the variable containing the elapsed time
* @retval none
*/
void PLAYER_GetTimeInfo(uint32_t* Length, uint32_t* Elapsed)
{
  if (sDecoderStruct.Decoder_GetStreamLength != NULL && Length != NULL )
  {
    *Length = sDecoderStruct.Decoder_GetStreamLength(PLAYER_GetFileLength());
  }
  if (sDecoderStruct.Decoder_GetElapsedTime != NULL && Elapsed != NULL)
  {
    *Elapsed = sDecoderStruct.Decoder_GetElapsedTime(AudioFile.fptr);
  }
}
/**
* @brief  Get File information.
* @param  file_name: File name
* @param  AudioInfoData: pointer to audio data info to fill
* @retval player error
*/
AUDIO_ErrorTypeDef PLAYER_GetFileInfo(char * file_name, Audio_InfoTypeDef *AudioInfoData)
{
  AUDIO_ErrorTypeDef ErrorCode = AUDIO_ERROR_NONE;

 /* Select decoder type and Get info from header */
  ErrorCode = PLAYER_GetFileHeaderInfo(file_name, AudioInfoData);

  PLAYER_Close();
  return ErrorCode;
}

/**
* @brief  Sets a new progress time value.
* @param  NewTime: pointer position to be set
* @retval player error
*/
uint32_t PLAYER_SeekToTime(uint32_t NewTime)
{
  AUDIO_ErrorTypeDef error = AUDIO_ERROR_NONE;

  if(sDecoderStruct.Decoder_SetProgressTime != NULL)
  {
    uint32_t retval = 0;

    /* Make sure the output audio stream is stopped */
    BSP_AUDIO_OUT_Stop(AUDIO_OUT_INSTANCE);

    /* Stop decoder */
    osDelay(10);

    /* Reset Buffers */
    PLAYER_ResetBuffers();

    /* Seek to new time */
    retval = sDecoderStruct.Decoder_SetProgressTime(NewTime);

    /* Restart the output audio stream from new time */
    if(!retval)
    {
      /* Loop1 : Write MemPool and Fill Buffer1 from Audio file at start */
      PLAYER_TransfertToMemPool();

      /*Loop2: Process MemPool and Fill complete double output buffer at first time */
      error = PLAYER_TransfertFromMemPool(0,FACTOR_BUFFER_INC);
      if ( error == AUDIO_ERROR_NONE)
      {
        error = PLAYER_TransfertFromMemPool(FACTOR_BUFFER_INC,FACTOR_BUFFER_INC);
      }

      /* Loop1 if new available space  */
      PLAYER_TransfertToMemPool();

      /*  Play complete double output buffer at first time */
      if ( error == AUDIO_ERROR_NONE)
      {
        if(BufferCtl.fptr != 0)
        {
#if CLEAN_DCACHE
          /* Clean Data Cache to update the content of the SRAM */
          SCB_CleanDCache_by_Addr((uint32_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);
#endif

          /*DMA stream from output double buffer to codec in Circular mode launch*/
          BSP_AUDIO_OUT_Play(AUDIO_OUT_INSTANCE, (uint8_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE);
          return AUDIO_ERROR_NONE;
        }
      }
    }
  }

  return 0;
}

/**
* @brief  Callback function to supply the decoder with input bit-stream.
* @param  pCompressedData: pointer to the target buffer to be filled.
* @param  nDataSizeInChars: number of data to be read in bytes
* @param  pUserData: optional parameter (not used in this version)
* @retval return the decode frame.
*/
uint32_t Dec_ReadDataCallback(void *    pCompressedData,        /* [OUT] Bitbuffer */
                                  uint32_t  nDataSizeInChars,       /* sizeof(Bitbuffer) */
                                  void *    pUserData               /* Application-supplied parameter */ )
{
  uint32_t error_dec = 0x00;
 /* data number read */
  uint32_t NumberOfData = 0;

  error_dec = f_read(&AudioFile, pCompressedData, nDataSizeInChars, (void*)(&NumberOfData));
  if (error_dec != FR_OK)
  {
    /* no data available */
    return 0;
  }

  return  NumberOfData;
}

/**
* @}
*/

/**
* @}
*/

