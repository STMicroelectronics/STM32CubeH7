/**
  ******************************************************************************
  * @file    audio_player/Src/player.c
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
#include "player.h"

/** @addtogroup AUDIO_PLAYER_CORE
  * @{
  */

/** @defgroup PLAYER_CORE
  * @brief Audio Player Core
  * @{
  */

/* Private define ------------------------------------------------------------*/
/** @defgroup PLAYER_CORE_Private_Constants
  * @{
  */
#define MSG_ERR_MOD_YLINE         16

/*SRC module selected*/
#define SELECT_SRC_NONE     0
#define SELECT_SRC_236      1
#define SELECT_SRC_441      2

/* MemPool size */
#ifdef PLAYER_DISABLE_SRC
/* MemPool size is same as of the Audio Buffer size */
#define   MEMPOOLBUFSZE  MAX_OUT_PACKET_SZE
#else /* !PLAYER_DISABLE_SRC */
#define   MEMPOOLBUFSZE  (3*MAX_OUT_PACKET_SZE)
#endif /* PLAYER_DISABLE_SRC */

/* Player Instance Magic ID */
#define   VALID_PLAYER_HANDLE           0xC0D0AF01
#define   INVALID_PLAYER_HANDLE         0xABADC0DE

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/** @defgroup PLAYER_CORE_Private_Types
  * @{
  */
typedef enum
{
  DECODER_STATE_IDLE = 0,
  DECODER_STATE_STOP,
  DECODER_STATE_RUNNING,
  DECODER_STATE_EOF,
  DECODER_STATE_ERROR
} DECODER_StateTypeDef;

typedef struct
{
  /* Audio player Output Thread data */
  osMessageQId   AudioOutEvent;
  osThreadId     AudioOutThreadId;
} PLAYER_Out_Thread_Data_t;

typedef struct
{
  /* Audio player Decoder Thread data */
  osSemaphoreId  AudioDecSemaphore;
  osThreadId     AudioDecThreadId;
} PLAYER_Dec_Thread_Data_t;

#ifndef PLAYER_DISABLE_SRC
typedef struct
{
  /* SRC: Sample Rate Converter parameters */
  void    *pSrc236PersistentMem;
  void    *pSrc441PersistentMem;
  void    *pSrcScratchMem;

  uint8_t  SrcTypeSelected;  /* 236, 441, None */
  uint8_t  SrcIterations;

  int16_t  AudioBufferSize;  /* SRC input buffer size in bytes*/
  uint8_t  WavToSrcTmpBuffer[SRC236_FRAME_SIZE_MAX*2*2]; /* SRC_FRAME_SIZE (240>147) * 2 (bytes x sample) * 2 (stereo) */
} PLAYER_SRC_Params_t;
#endif /* !PLAYER_DISABLE_SRC */

typedef struct
{
  /* Pre/Post Processing Callbacks */
  fnUserCallback_TypeDef*      pPrePrcessingCallback;
  fnUserCallback_TypeDef*      pPostPrcessingCallback;
  fnEndOfFileCallback_TypeDef* pEndOfFileCallback;
} PLAYER_Callback_t;

typedef struct
{
  /* Input Audio File*/
  FIL       AudioFile;
  char      FilePath[MAX_FILE_NAME_LENGHT];
  char      FileExtension;
  uint8_t   tHeaderTmp[MAX_AUDIO_HEADER_SIZE];

  /* Generic Audio Info */
  AUDIO_FILE_InfoTypeDef AudioFormatData;
} PLAYER_INPUT_FileInfo_t;

typedef struct
{
  /* Input and Output Buffers for Audio Module Interface */
  buffer_t InputBuffer;
  buffer_t OutputBuffer;

  /* To the output codec */
  AUDIO_OUT_BufferTypeDef  BufferCtl;
} PLAYER_BUFFER_Info_t;

typedef struct
{
  /* Decoder Instance */
  Decoder_TypeDef sDecoderStruct;

  /* Current Decoder Processing State */
  __IO DECODER_StateTypeDef CurrentDecoderState;
} PLAYER_DEC_Info_t;

typedef struct
{
  uint32_t                  MagicID;

  PLAYER_Out_Thread_Data_t  OutThreadInfo;
  PLAYER_Dec_Thread_Data_t  DecThreadInfo;

#ifndef PLAYER_DISABLE_SRC
  /* Source Rate Conversion Info */
  PLAYER_SRC_Params_t       SrcInfo;
#endif /* !PLAYER_DISABLE_SRC */

  /* Pre/Post Processing Callbacks */
  PLAYER_Callback_t         ProcessCb;

  /* Current Audio Processing State */
  AUDIO_StateTypeDef        CurrentAudioState;

  /* Input File data */
  PLAYER_INPUT_FileInfo_t   InputFileInfo;

  /* Buffers */
  PLAYER_BUFFER_Info_t      Buffers;

  /* Audio FWK Handle */
  AUDIO_HANDLE_t           *hAudio;

  /* Current Volume Value */
  __IO uint8_t              uwVolumeRef;
  
  /* Decoder Instance */
  PLAYER_DEC_Info_t         Decoder;
} PLAYER_Instance_t;
BSP_AUDIO_Init_t AudioPlayInit;
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup PLAYER_CORE_Private_Variables
  * @{
  */
/* Memory for Audio Buffers */
#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="audio_buffers"
#else
__attribute__((section(".audio_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t AudioBuffers[AUDIO_OUT_BUFFER_SIZE];

/* MemPool Write: In Buffer to read audio file */
#ifndef PLAYER_DISABLE_SRC
#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="audio_buffers"
#else
__attribute__((section(".audio_buffers"))) __attribute__ ((aligned (32)))
#endif
static int8_t Buffer1[MAX_OUT_PACKET_SZE];
#else /* PLAYER_DISABLE_SRC */
#ifdef PLAYER_DISABLE_SRC_ZERO_COPY
/* No extra buffer used for SRC */
static int8_t *Buffer1 = (int8_t *)AudioBuffers;
#else /* !PLAYER_DISABLE_SRC_ZERO_COPY */
#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="audio_buffers"
#else
__attribute__((section(".audio_buffers"))) __attribute__ ((aligned (32)))
#endif
static int8_t Buffer1[MAX_OUT_PACKET_SZE];
#endif /* PLAYER_DISABLE_SRC_ZERO_COPY */
#endif /* !PLAYER_DISABLE_SRC */

#ifdef AUDIO_DEBUG
static __IO uint32_t StartTick = 0;
static __IO uint32_t DurationInTicks = 0;
#endif /* AUDIO_DEBUG */

static __IO uint8_t num_instances = 0;
static PLAYER_Instance_t *PlayerInstances[MAX_PLAYER_INSTANCES_NBR] = {0, 0};

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
static AUDIO_ErrorTypeDef PlayerGetFileInfo(PLAYER_Instance_t *Instance, char * file_name, AUDIO_FILE_InfoTypeDef *info);
/* init functions */
static uint8_t PlayerStreamInit(uint32_t AudioVol, uint32_t AudioFreq);  /* config SAI output Audio stream*/
/* internal DMA IT output Callback processing*/
static AUDIO_ErrorTypeDef AudioOutCallBackProcess(PLAYER_Instance_t *Instance, uint16_t offset); /* Loop2: DMA IT callback processing in output SRC loop*/
static AUDIO_ErrorTypeDef SRC_ProcessBuffer(PLAYER_Instance_t *Instance, uint16_t offset); /* read MemPool and SRC audio processing*/
static AUDIO_ErrorTypeDef DecodeAudioBuffer(PLAYER_Instance_t *Instance, uint16_t nb_buffers); /* read from storage device and decode buffer */
/* reset audio player buffers */
static uint32_t ResetBuffers(PLAYER_Instance_t *Instance);
/* Close audio player handle */
static uint32_t PLAYER_Close(PLAYER_Instance_t *Instance);
/* Set position callback */
static uint32_t SetPosition_Callback(void *pUser, uint32_t Pos);
static uint32_t GetFileSize_Callback(void *pUser);
/* Notify decoder task */
static void NotifyDecoder(PLAYER_Instance_t *Instance);
/* Read Data Callback function */
static uint32_t Dec_ReadDataCallback(void* pCompressedData, uint32_t nDataSizeInChars, void* pUserData);
/* Audio Output Thread */
static void Audio_Out_Thread(void const * argument);
/* Audio Decoder Thread */
static void Decoder_Thread(void const * argument);

/* Private functions ---------------------------------------------------------*/
/** @defgroup PLAYER_CORE_Private_Functions
  * @{
  */
/**
* @brief  Audio Decoder task
* @param  argument: pointer that is passed to the thread function as start argument.
* @retval None
*/
static void Decoder_Thread(void const * argument)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)argument;

  if(NULL == Instance)
  {
    return;
  }

  for( ;; )
  {
    if(osSemaphoreWait(Instance->DecThreadInfo.AudioDecSemaphore, osWaitForever) == osOK)
    {
      /* Loop1 processing*/
      DecodeAudioBuffer(Instance, 0);
    }
  }
}

/**
  * @brief  Audio Output task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Audio_Out_Thread(void const * argument)
{
  osEvent event;

  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)argument;

  if(NULL == Instance)
  {
    return;
  }

  for(;;)
  {
    event = osMessageGet(Instance->OutThreadInfo.AudioOutEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      if(Instance->CurrentAudioState == AUDIO_STATE_PLAY)
      {
        int32_t sizeAvailable = Instance->Decoder.sDecoderStruct.PacketSize;

#ifndef PLAYER_DISABLE_SRC

        /* Check if no more samples to decode from input file */
        if (Instance->Decoder.CurrentDecoderState == DECODER_STATE_EOF)
        {
          /* Check if pool state is not used */
          if (FWK_MEMPOOL_GetState(Instance->hAudio->pMemPool) == FWK_MEMPOOL_STATUS_NOT_USED)
          {
            /* Last samples were outputted */
            Instance->CurrentAudioState = AUDIO_STATE_EOF;
          }
        }

        if(Instance->CurrentAudioState == AUDIO_STATE_PLAY)
        {
          /* Check if pool state is not full */
          if (FWK_MEMPOOL_GetState(Instance->hAudio->pMemPool) != FWK_MEMPOOL_STATUS_FULL)
          {
            /* Check if pool state is not empty */
            if (FWK_MEMPOOL_GetState(Instance->hAudio->pMemPool) == FWK_MEMPOOL_STATUS_EMPTY)
            {
              sizeAvailable = Instance->hAudio->pMemPool->buffSze; /* Empty or EOF case */


            }
            else
            {
              FWK_MEMPOOL_GetAvailableSize(Instance->hAudio->pMemPool, &sizeAvailable); /* Other */
            }
          }
          else
          {
            sizeAvailable = 0; /* Full case */
          }
        }
#endif /* !PLAYER_DISABLE_SRC*/

        if(Instance->CurrentAudioState == AUDIO_STATE_PLAY)
        {
          /* MemPool writing from Buffer1 filled with audio file (Lower priority) */
          /* Callback: Loop while there is enough room in Pool_1 for decoding */
          if(sizeAvailable >= Instance->Decoder.sDecoderStruct.PacketSize)
          {
            /* Notify Decoder task to start decoding new buffers */
            NotifyDecoder(Instance);
          }

          switch(event.value.v)
          {
            case BUFFER_OFFSET_HALF:
              AudioOutCallBackProcess(Instance, 0);
              break;
            case BUFFER_OFFSET_FULL:
              AudioOutCallBackProcess(Instance, Instance->Buffers.BufferCtl.size/2);
              break;
            default:
              break;
          }
        }
      }

      if(Instance->CurrentAudioState == AUDIO_STATE_EOF)
      {
        if(Instance->ProcessCb.pEndOfFileCallback)
        {
          Instance->ProcessCb.pEndOfFileCallback();
        }
      }
    }
  }
}

/**
* @brief  Sets the current position of the audio file pointer.
* @param  pUser: pointer to user data holding the player instance
* @param  Pos: new file position to be set
* @retval set position in the file
*/
static uint32_t SetPosition_Callback(void *pUser, uint32_t Pos)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pUser;

  /* Call the Fat FS seek function */
  if(Pos != Instance->InputFileInfo.AudioFile.fptr)
  {
    Pos = f_lseek(&(Instance->InputFileInfo.AudioFile), Pos);
  }

  return Pos;
}

/**
* @brief  Get the current audio file size.
* @param  pUser: pointer to user data
* @retval Size of the file
*/
static uint32_t GetFileSize_Callback(void *pUser)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pUser;

  return f_size(&(Instance->InputFileInfo.AudioFile));
}

/**
* @brief  Initializes Audio Interface.
* @param  Instance: Audio Player instance
* @retval Audio error
*/
static AUDIO_ErrorTypeDef PLAYER_MEMPOOL_Init(PLAYER_Instance_t *Instance)
{
  /* MemPool: RAM dynamic memory allocation,
  Allocate SRAM memory MemPool structure */
  Instance->hAudio = (AUDIO_HANDLE_t*) PLAYER_MALLOC(sizeof(AUDIO_HANDLE_t));
  if (Instance->hAudio == NULL)
  {
    return AUDIO_ERROR_MEM;
  }

  Instance->hAudio->pMemPool = (FWK_MEMPOOL_t*) PLAYER_MALLOC(sizeof(FWK_MEMPOOL_t)); /* 0x10: 16 */
  if (Instance->hAudio->pMemPool == NULL)
  {
    return AUDIO_ERROR_MEM;
  }

  /* Allocate Mutex for lock access */
  osMutexDef(AudioLockMutex);
  Instance->hAudio->pMemPool->lock = (uint32_t)osMutexCreate(osMutex(AudioLockMutex));

  /* Allocate SRAM memory for MemPool Buffer: at least 3 times Max output audio decoder packet */
  Instance->hAudio->pMemPool->pBuff = (uint8_t*) PLAYER_MALLOC(MEMPOOLBUFSZE);
  if(Instance->hAudio->pMemPool->pBuff == NULL)
  {
    return AUDIO_ERROR_MEM;
  }

  FWK_MEMPOOL_Init(Instance->hAudio->pMemPool, MEMPOOLBUFSZE);

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Initializes Audio Interface.
* @param  Instance: Audio Player instance
* @retval Audio error
*/
static AUDIO_ErrorTypeDef PLAYER_SRC_Init(PLAYER_Instance_t *Instance)
{
#ifndef PLAYER_DISABLE_SRC
  uint32_t src_scratch_mem_size = 0;

  /* Allocate mem for SRC236 and SRC411 */
  Instance->SrcInfo.pSrc236PersistentMem = PLAYER_MALLOC(src236_persistent_mem_size);  /* 0x1EC: 492  */
  Instance->SrcInfo.pSrc441PersistentMem = PLAYER_MALLOC(src441_persistent_mem_size);  /* 0x0E8: 232 */
  if ((Instance->SrcInfo.pSrc236PersistentMem == NULL) || (Instance->SrcInfo.pSrc441PersistentMem == NULL))
  {
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

  Instance->SrcInfo.pSrcScratchMem = PLAYER_MALLOC(src_scratch_mem_size);
  if (Instance->SrcInfo.pSrcScratchMem == NULL)
  {
    return AUDIO_ERROR_MEM;
  }
#endif /* !PLAYER_DISABLE_SRC */
  return AUDIO_ERROR_NONE;
}

/*******************************************************************************
Static Functions
*******************************************************************************/
/**
* @brief  Gets the file info from header.
* @param  file_idx: File index
* @param  info: Pointer to Audio file (MP3, WAV)
* @retval Audio error
*/
static AUDIO_ErrorTypeDef PlayerGetFileInfo(PLAYER_Instance_t *Instance, char * file_name, AUDIO_FILE_InfoTypeDef *info)
{
  uint32_t bytesread;
  uint8_t *header;
  uint8_t error_tmp=0;

  if((NULL == file_name)
  || (strlen(file_name) > MAX_FILE_NAME_LENGHT))
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }

  /*header pointer allocation*/
  header=Instance->InputFileInfo.tHeaderTmp;

  /* Open Audio File */
  if(f_open(&Instance->InputFileInfo.AudioFile, file_name, FA_OPEN_EXISTING | FA_READ) != FR_OK)
  {
    memset((char *)Instance->InputFileInfo.FilePath, 0, sizeof(Instance->InputFileInfo.FilePath));
    return AUDIO_ERROR_IO;
  }

  /* Select Audio Decoder with Abs Layer */
  sprintf((char *)Instance->InputFileInfo.FilePath, file_name);

  /* Get the extension of audio file */
  Instance->InputFileInfo.FileExtension =  Instance->InputFileInfo.FilePath[strlen(Instance->InputFileInfo.FilePath) - 1];

  /* ++ Read Header File information ++ */
  if(f_read(&Instance->InputFileInfo.AudioFile, header, MAX_AUDIO_HEADER_SIZE, (void *)&bytesread) != FR_OK)
  {
    f_close(&Instance->InputFileInfo.AudioFile);
    memset((char *)Instance->InputFileInfo.FilePath, 0, sizeof(Instance->InputFileInfo.FilePath));
    return AUDIO_ERROR_IO;
  }

  /* Inititalize the decoder Abs layer structure instance with matching decoder decoder or Null pointers */
  if(CODERS_SelectDecoder(&(Instance->Decoder.sDecoderStruct), Instance->InputFileInfo.FileExtension) != 0)
  {
    f_close(&Instance->InputFileInfo.AudioFile);
    memset((char *)Instance->InputFileInfo.FilePath, 0, sizeof(Instance->InputFileInfo.FilePath));
    return AUDIO_ERROR_CODEC;
  }

  /* Initialization of the decoder */
  if (Instance->Decoder.sDecoderStruct.DecoderInit != NULL)
  {
    error_tmp=Instance->Decoder.sDecoderStruct.DecoderInit((uint8_t*)header, Dec_ReadDataCallback, SetPosition_Callback, GetFileSize_Callback, (void *)Instance);

    if (error_tmp == 0)
    {
      /* ++ Fill the "info" structure with audio format parameters after decoding++ */

      /* Extract the current sampling rate */
      if (Instance->Decoder.sDecoderStruct.Decoder_GetSamplingRate != NULL)
      {
        info->SampleRate = Instance->Decoder.sDecoderStruct.Decoder_GetSamplingRate();
        /* Check for supported SRC rates */
        switch(info->SampleRate)
        {
        case 8000:
        case 16000:
        case 24000:
        case 32000:
        case 44100:
        case 48000:
        case 96000:
          break;
        default:
          PLAYER_Close(Instance);
          return (AUDIO_ERROR_SRC);
        }
      }
      info->FileSize = f_size(&Instance->InputFileInfo.AudioFile); /*in bytes*/

      /* --MP3 --*/
      if (Instance->InputFileInfo.FileExtension=='3')
      {
        info->NbrChannels = Instance->Decoder.sDecoderStruct.Decoder_GetNbChannels();  /*stereo== 2*/

        info->FileFormat ='3';   /*MP3*/
        info->AudioFormat ='P';  /*PCM*/

        info->BitPerSample =16;
        /*After decoding: ((NumberOfFrames * mp3_Info.nSamplesPerFrame))/ NumberOfBytes;*/
        info->ByteRate = Instance->Decoder.sDecoderStruct.Decoder_GetBitRateKbps()*1024/8;
        info->Duration = Instance->Decoder.sDecoderStruct.Decoder_GetStreamLength(PLAYER_GetFileLength(Instance));
      }
      /*--WAVE--*/
      else if ((Instance->InputFileInfo.FileExtension=='v')||(Instance->InputFileInfo.FileExtension=='V'))
      {
        info->NbrChannels = Instance->Decoder.sDecoderStruct.Decoder_GetNbChannels();

        info->FileFormat = 'V';   /*WAVE*/
#ifdef __WAV_DECODER__
        if (DecWavFmtStruct.FormatTag == 1)
        {
          info->AudioFormat = 'P';
        }
        else
        {
          PLAYER_Close(Instance);
          return AUDIO_ERROR_INVALID_PARAMS;
        }
#else /* !#ifdef __WAV_DECODER__ */
        info->AudioFormat = 'P';
#endif /* #ifdef __WAV_DECODER__ */
        info->BitPerSample = Instance->Decoder.sDecoderStruct.Decoder_GetNbBits();
        info->ByteRate     = (Instance->Decoder.sDecoderStruct.Decoder_GetSamplingRate())*(Instance->Decoder.sDecoderStruct.Decoder_GetNbChannels())*(Instance->Decoder.sDecoderStruct.Decoder_GetNbBits())/8; /*raw PCM data*/
        info->Duration     = Instance->Decoder.sDecoderStruct.Decoder_GetStreamLength(PLAYER_GetFileLength(Instance));
      }
#ifdef __AVI_DECODER__
      /*--AVI--*/
      else if ((Instance->InputFileInfo.FileExtension=='i')||(Instance->InputFileInfo.FileExtension=='I'))
      {
        info->NbrChannels = Instance->Decoder.sDecoderStruct.Decoder_GetNbChannels();

        info->FileFormat = 'I';   /*AVI*/
        info->AudioFormat = 'P';

        info->BitPerSample = Instance->Decoder.sDecoderStruct.Decoder_GetNbBits();
        info->ByteRate     = (Instance->Decoder.sDecoderStruct.Decoder_GetSamplingRate())*(Instance->Decoder.sDecoderStruct.Decoder_GetNbChannels())*(Instance->Decoder.sDecoderStruct.Decoder_GetNbBits())/8; /*raw PCM data*/
        info->Duration     = Instance->Decoder.sDecoderStruct.Decoder_GetStreamLength(PLAYER_GetFileLength(Instance));
      }
#endif /* __AVI_DECODER__ */
      else
      {
        PLAYER_Close(Instance);
        return AUDIO_ERROR_INVALID_PARAMS;
      }
    }
    else
    {
      f_close(&Instance->InputFileInfo.AudioFile);
      memset((char *)Instance->InputFileInfo.FilePath, 0, sizeof(Instance->InputFileInfo.FilePath));
      return AUDIO_ERROR_CODEC;
    }
  }

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Initializes the Audio player.
* @param  AudioFreq: Audio sampling frequency
* @retval None
*/
static uint8_t PlayerStreamInit(uint32_t AudioVol, uint32_t AudioFreq)
{
  uint8_t retval = 0;
  
  
  AudioPlayInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioPlayInit.ChannelsNbr = 2;
  AudioPlayInit.SampleRate = AudioFreq ;
  AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit.Volume = AudioVol;
  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */
  if(BSP_AUDIO_OUT_Init(0, &AudioPlayInit) != 0)
  {
    retval = 1;
  }


  return retval;
}

/**
* @brief  Read from USB a number of samples such that after SRC conversion
*         480 stereo samples are provided to the codec each 10 ms
* @param  bytesread: returns to the caller the bytes read from USB
* @param  offset: first flip flop buffer or second flip flop buffer
* @retval error: error type (default AUDIO_ERROR_NONE)
*/
static AUDIO_ErrorTypeDef SRC_ProcessBuffer(PLAYER_Instance_t *Instance, uint16_t offset)
{
  uint32_t bytesread;

  /* Bypass SRC processing */
#ifdef PLAYER_DISABLE_SRC
#ifdef PLAYER_DISABLE_SRC_ZERO_COPY
  /* Read from memory pool1 to temporary module processing computation buffer */
  if(FWK_MEMPOOL_ReadPtrInc(Instance->hAudio->pMemPool, Instance->Buffers.BufferCtl.size/2) != FWK_MEMPOOL_ERROR_NONE)
  {
    return (AUDIO_ERROR_IO);
  }
#else
  /* Read from memory pool1 to temporary module processing computation buffer */
  if(FWK_MEMPOOL_Read(Instance->hAudio->pMemPool, (uint8_t*)&(Instance->Buffers.BufferCtl.buff[offset]),  Instance->Buffers.BufferCtl.size/2) != FWK_MEMPOOL_ERROR_NONE)
  {
    return (AUDIO_ERROR_IO);
  }
#endif
  bytesread = Instance->Buffers.BufferCtl.size/2; /*force value*/
  Instance->Buffers.BufferCtl.fptr += bytesread;
#else /* !PLAYER_DISABLE_SRC */
  if (Instance->SrcInfo.SrcTypeSelected == SELECT_SRC_NONE)
  {
    /* Read from memory pool1 to temporary module processing computation buffer */
    if(FWK_MEMPOOL_Read(Instance->hAudio->pMemPool, (uint8_t*)&(Instance->Buffers.BufferCtl.buff[offset]),  Instance->Buffers.BufferCtl.size/2) != FWK_MEMPOOL_ERROR_NONE)
    {
      return (AUDIO_ERROR_IO);
    }
    bytesread = Instance->Buffers.BufferCtl.size/2; /*force value*/
    Instance->Buffers.BufferCtl.fptr += bytesread;
  }
  /* Selected SRC processing */
  else
  {
    uint16_t i;
    for (i = 0; i < Instance->SrcInfo.SrcIterations; ++i)
    {
      uint32_t src_error;
      buffer_t *pInputBuffer  = &Instance->Buffers.InputBuffer;
      buffer_t *pOutputBuffer = &Instance->Buffers.OutputBuffer;

      /* Read from memory pool1 to temporary module processing computation buffer*/
      if(FWK_MEMPOOL_Read(Instance->hAudio->pMemPool, (uint8_t*)&Instance->SrcInfo.WavToSrcTmpBuffer, Instance->SrcInfo.AudioBufferSize)== FWK_MEMPOOL_ERROR_NONE)
      {
        bytesread=Instance->SrcInfo.AudioBufferSize;
        Instance->Buffers.BufferCtl.fptr += bytesread;

        Instance->Buffers.OutputBuffer.data_ptr = &(Instance->Buffers.BufferCtl.buff[offset + i*Instance->Buffers.BufferCtl.size /(2*Instance->SrcInfo.SrcIterations)]);

        if (Instance->SrcInfo.SrcTypeSelected == SELECT_SRC_236)/* SrcTypeSelected == 236 */
        {
          src_error = src236_process(pInputBuffer, pOutputBuffer, Instance->SrcInfo.pSrc236PersistentMem);
          if (src_error != SRC236_ERROR_NONE)
          {
            return (AUDIO_ERROR_SRC);
          }
        }
        else /* Instance->SrcInfo.SrcTypeSelected == 441 */
        {
          src_error = src441_process(pInputBuffer, pOutputBuffer, Instance->SrcInfo.pSrc441PersistentMem);
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
    }
  }
#endif /* PLAYER_DISABLE_SRC */

  if(Instance->ProcessCb.pPostPrcessingCallback != NULL)
  {
    /* Set Post Processing Buffer */
    uint8_t * PostProcessingBuffer = (uint8_t *)&(Instance->Buffers.BufferCtl.buff[offset]);
    Instance->ProcessCb.pPostPrcessingCallback(PostProcessingBuffer, Instance->Buffers.BufferCtl.size/2);
  }

  return AUDIO_ERROR_NONE;
}

/**
  * @brief  Reset All buffers.
  * @param  Instance: Audio Player instance
  * @retval 0 if success; -1 otherwise
  */
static uint32_t ResetBuffers(PLAYER_Instance_t *Instance)
{
  uint32_t i =0;

  /*reset MemPool buffer*/
  FWK_MEMPOOL_DeInit(Instance->hAudio->pMemPool);
  FWK_MEMPOOL_Init(Instance->hAudio->pMemPool,MEMPOOLBUFSZE);

  /*reset header buffer*/
  for (i = 0; i < MAX_AUDIO_HEADER_SIZE; i++)
  {
    Instance->InputFileInfo.tHeaderTmp[i] = 0;
  }

  /*reset output double buffer*/
  for (i = 0; i < Instance->Buffers.BufferCtl.size; i++)
  {
    Instance->Buffers.BufferCtl.buff[i] = 0;
  }

  Instance->Decoder.CurrentDecoderState = DECODER_STATE_IDLE;

  return 0;
}

/**
* @brief  Close Audio decoder instance.
* @param  None
* @retval None
* @note: call before Player_Stop then exit.
*/
static uint32_t PLAYER_Close(PLAYER_Instance_t *Instance)
{
  /* Close the decoder instance */
  if (Instance->Decoder.sDecoderStruct.DecoderDeInit != NULL)
  {
    Instance->Decoder.sDecoderStruct.DecoderDeInit();

    /* Empty the decoder structure */
    CODERS_SelectDecoder(&(Instance->Decoder.sDecoderStruct), ' ');
  }

  /* Close the current file */
  if(strlen(Instance->InputFileInfo.FilePath) > 0)
  {
    f_close(&Instance->InputFileInfo.AudioFile);
    memset((char *)Instance->InputFileInfo.FilePath, 0, sizeof(Instance->InputFileInfo.FilePath));
  }

  return AUDIO_ERROR_NONE;
}

/**
* @brief  This function Manages the DMA Transfer interrupts callback processing.
* @param  uint16_t offset
* @retval audio_error
*/
static AUDIO_ErrorTypeDef AudioOutCallBackProcess(PLAYER_Instance_t *Instance, uint16_t offset)
{
  AUDIO_ErrorTypeDef audio_error = AUDIO_ERROR_NONE;

  audio_error = SRC_ProcessBuffer(Instance, offset);
  if (audio_error != AUDIO_ERROR_NONE)
  {
    return audio_error;
  }
  Instance->Buffers.BufferCtl.state = BUFFER_OFFSET_NONE;
  return audio_error;
}

/**
* @brief  SWI EXTI0 line detection callback.
*         Loop1: processing to fill MemPool with Buffer1 after USB key file read
* @retval None
*/
static void NotifyDecoder(PLAYER_Instance_t *Instance)
{
  /* Release the semaphore if the thread was created prior to this call */
  if(Instance->DecThreadInfo.AudioDecSemaphore)
  {
    if(Instance->Decoder.CurrentDecoderState == DECODER_STATE_IDLE)
    {
      /* osStatus xReturn = */ osSemaphoreRelease(Instance->DecThreadInfo.AudioDecSemaphore);
      /* configASSERT( xReturn != osErrorOS ); */
    }
  }
}

/**
  * @}
  */

/** @defgroup PLAYER_CORE_Exported_Functions
  * @{
  */
/**
* @brief  Initializes Audio Interface.
* @param  Volume: initial volume value
* @param  pHandle: Audio Player handle
* @retval Audio error
*/
AUDIO_ErrorTypeDef PLAYER_Init(uint8_t Volume, PLAYER_HANDLE_h *pHandle)
{
  /* Allocate Player Handle */
  PLAYER_Instance_t *Instance = PLAYER_MALLOC(sizeof(PLAYER_Instance_t));  /* 0x0E8: 232 */
  if (NULL == Instance)
  {
    return AUDIO_ERROR_MEM;
  }

  memset(Instance, 0, sizeof(PLAYER_Instance_t));

  /* Set Handle */
  *pHandle = (PLAYER_HANDLE_h)Instance;

  /* Set Value Value to Default setting */
  Instance->uwVolumeRef = AUDIO_PLAYER_DEFAULT_VOLUME;

  if(PLAYER_SRC_Init(Instance) != AUDIO_ERROR_NONE)
  {
    PLAYER_DeInit(*pHandle);
    return (AUDIO_ERROR_SRC);
  }

  if(PLAYER_MEMPOOL_Init(Instance) != AUDIO_ERROR_NONE)
  {
    PLAYER_DeInit(*pHandle);
    return (AUDIO_ERROR_MEM);
  }

  /* Create Audio Post Processing Queue */
  osMessageQDef(AUDIO_Out_Queue, 10, uint16_t);
  Instance->OutThreadInfo.AudioOutEvent = osMessageCreate (osMessageQ(AUDIO_Out_Queue), NULL); 
  if(0 == Instance->OutThreadInfo.AudioOutEvent)
  {
    PLAYER_DeInit(*pHandle);
    return AUDIO_ERROR_MEM;
  }

  /* Create Semaphore lock */
  osSemaphoreDef(AUDIO_READER_SEM);
  /* Create the Semaphore used by the two threads */
  Instance->DecThreadInfo.AudioDecSemaphore = osSemaphoreCreate(osSemaphore(AUDIO_READER_SEM), 1);
  /* Get the semaphore */
  osSemaphoreWait(Instance->DecThreadInfo.AudioDecSemaphore, osWaitForever);

  /* Create Audio Decoder task */
  osThreadDef(AUDIO_Dec_Thread, Decoder_Thread, AUDIO_DEC_THREAD_PRIORITY_L, 0, AUDIO_DEC_THREAD_STACK_SIZE);
  Instance->DecThreadInfo.AudioDecThreadId = osThreadCreate (osThread(AUDIO_Dec_Thread), (void *)Instance);

  /* Create Audio Output Processing task */
  osThreadDef(osAudioOut_Thread, Audio_Out_Thread, AUDIO_OUT_THREAD_PRIORITY, 0, AUDIO_OUT_THREAD_STACK_SIZE);
  Instance->OutThreadInfo.AudioOutThreadId = osThreadCreate (osThread(osAudioOut_Thread), (void *)Instance);

  if((Instance->OutThreadInfo.AudioOutThreadId != 0) && (Instance->DecThreadInfo.AudioDecThreadId != 0))
  {
    Instance->CurrentAudioState = AUDIO_STATE_STOP;
  }
  else
  {
    PLAYER_DeInit(*pHandle);
    return AUDIO_ERROR_MEM;
  }
  AudioPlayInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioPlayInit.ChannelsNbr = 2;
  AudioPlayInit.SampleRate = I2S_AUDIOFREQ_48K ;
  AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit.Volume = Instance->uwVolumeRef;
  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */
  if(BSP_AUDIO_OUT_Init(0,&AudioPlayInit) != AUDIO_ERROR_NONE)
  {
    PLAYER_DeInit(*pHandle);
    return AUDIO_ERROR_MEM;
  }


  Instance->CurrentAudioState = AUDIO_STATE_STOP;
  PlayerInstances[num_instances] = Instance;
  num_instances++;

  return AUDIO_ERROR_NONE;
}

/**
* @brief  De-Initializes Audio Interface.
* @param  pHandle: Audio Player handle
* @retval Audio error
*/
AUDIO_ErrorTypeDef PLAYER_DeInit(PLAYER_HANDLE_h pHandle)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;

  if(NULL != Instance)
  {
    /* Immediately stop HW */
    if(Instance->CurrentAudioState != AUDIO_STATE_INIT)
    {
      BSP_AUDIO_OUT_DeInit(0);
      Instance->CurrentAudioState = AUDIO_STATE_INIT;
    }

    /* Terminate Output thread */
    if(Instance->OutThreadInfo.AudioOutThreadId != 0)
    {
      osThreadTerminate(Instance->OutThreadInfo.AudioOutThreadId);
      Instance->OutThreadInfo.AudioOutThreadId = 0;
    }

    /* Terminate Decoder thread */
    if(Instance->DecThreadInfo.AudioDecThreadId != 0)
    {
      osThreadTerminate(Instance->DecThreadInfo.AudioDecThreadId);
      Instance->DecThreadInfo.AudioDecThreadId = 0;
    }

    if(Instance->DecThreadInfo.AudioDecSemaphore != 0)
    {
      osSemaphoreDelete(Instance->DecThreadInfo.AudioDecSemaphore);
      Instance->DecThreadInfo.AudioDecSemaphore = 0;
    }

    if(Instance->OutThreadInfo.AudioOutEvent != 0)
    {
      vQueueDelete(Instance->OutThreadInfo.AudioOutEvent); 
      Instance->OutThreadInfo.AudioOutEvent = 0;
    }

    PLAYER_Stop(pHandle);

    if(NULL != Instance->hAudio)
    {
      if (Instance->hAudio->pMemPool != NULL)
      {
        FWK_MEMPOOL_DeInit(Instance->hAudio->pMemPool);
        if(Instance->hAudio->pMemPool->pBuff != NULL)
        {
          PLAYER_FREE(Instance->hAudio->pMemPool->pBuff);
          Instance->hAudio->pMemPool->pBuff = NULL;
        }
        if(Instance->hAudio->pMemPool->lock)
        {
          osMutexDelete((void *)Instance->hAudio->pMemPool->lock);
        }
        PLAYER_FREE(Instance->hAudio->pMemPool);
        Instance->hAudio->pMemPool = NULL;
      }
      PLAYER_FREE(Instance->hAudio);
      Instance->hAudio = NULL;
    }

#ifndef PLAYER_DISABLE_SRC
    if (Instance->SrcInfo.pSrcScratchMem != NULL)
    {
      PLAYER_FREE (Instance->SrcInfo.pSrcScratchMem);
      Instance->SrcInfo.pSrcScratchMem = NULL;
    }

    if (Instance->SrcInfo.pSrc441PersistentMem != NULL)
    {
      PLAYER_FREE (Instance->SrcInfo.pSrc441PersistentMem);
      Instance->SrcInfo.pSrc441PersistentMem = NULL;
    }

    if (Instance->SrcInfo.pSrc236PersistentMem != NULL)
    {
      PLAYER_FREE (Instance->SrcInfo.pSrc236PersistentMem);
      Instance->SrcInfo.pSrc236PersistentMem = NULL;
    }
#endif /* !PLAYER_DISABLE_SRC */

    if(pHandle)
    {
      PLAYER_FREE (pHandle);
      pHandle = NULL;
    }

    PlayerInstances[num_instances] = 0;
    num_instances--;

    return AUDIO_ERROR_NONE;
  }
  else
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }
}

/**
* @brief  Get File information.
* @param  pHandle: Audio Player handle
* @param  file_name: File name
* @param  AudioInfoData: pointer to audio data info to fill
* @retval Audio error
*/
AUDIO_ErrorTypeDef PLAYER_GetFileInfo(PLAYER_HANDLE_h pHandle, char *file_name, AUDIO_FILE_InfoTypeDef *AudioInfoData)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if(NULL == Instance)
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }

  /*-- Select decoder type and Get info from header --*/
  if(PlayerGetFileInfo(Instance, file_name, AudioInfoData) != AUDIO_ERROR_NONE)
  {
    return AUDIO_ERROR_CODEC;
  }

  PLAYER_Close(Instance);
  return AUDIO_ERROR_NONE;
}

/**
* @brief  Starts Audio streaming.
* @param  pHandle: Audio Player handle
* @param  file_name: File name
* @retval Audio error
*/
AUDIO_ErrorTypeDef PLAYER_Start(PLAYER_HANDLE_h pHandle, char *file_name)
{
  AUDIO_ErrorTypeDef error = AUDIO_ERROR_NONE;
  AUDIO_ErrorTypeDef error_tmp;

#ifndef PLAYER_DISABLE_SRC
  src236_static_param_t src236_static_param;
  src441_static_param_t src441_static_param;
  uint32_t src_error;
#endif /* !PLAYER_DISABLE_SRC */

  uint32_t input_frame_size = 0;

  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if((NULL == Instance)
  || (Instance->CurrentAudioState != AUDIO_STATE_STOP))
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }

  /* Reinit and force close file*/
  PLAYER_Close(Instance);

  /* Leave critical section before start reading the file */
  /*-- Select decoder type and Get info from header --*/
  error_tmp=PlayerGetFileInfo(Instance, file_name, &Instance->InputFileInfo.AudioFormatData);
  if(error_tmp != AUDIO_ERROR_NONE)
  {
    return error_tmp;
  }

  /* Input stereo audio only */
//  if(Instance->InputFileInfo.AudioFormatData.NbrChannels != 2)
//  {
//    f_close(&Instance->InputFileInfo.AudioFile);
//    return (AUDIO_ERROR_INVALID_PARAMS);
//  }

  /* Final Output double buffer structure */
  Instance->Buffers.BufferCtl.state = BUFFER_OFFSET_NONE;
  /*eqv read file pointer on output */
  Instance->Buffers.BufferCtl.fptr = 0;
  /* Set Audio Buffers address and size */
  Instance->Buffers.BufferCtl.buff = AudioBuffers;
  Instance->Buffers.BufferCtl.size = AUDIO_OUT_BUFFER_SIZE;

  /*reset audio player buffers*/
  ResetBuffers(Instance);

#ifdef PLAYER_DISABLE_SRC
  input_frame_size = Instance->Buffers.BufferCtl.size/2; /* half buff, stereo, byte x sample */
  BSP_AUDIO_OUT_SetFrequency(Instance->InputFileInfo.AudioFormatData.SampleRate);
#else /* PLAYER_DISABLE_SRC == 0 */
  /*Input frame size to read from MemPool before SRC processing to get 480 samples output*/
  switch(Instance->InputFileInfo.AudioFormatData.SampleRate)
  {
  case 8000:
    src236_static_param.src_mode = SRC236_RATIO_6;
    Instance->SrcInfo.SrcTypeSelected = SELECT_SRC_236;
    Instance->SrcInfo.SrcIterations = 1;
    input_frame_size = Instance->Buffers.BufferCtl.size/(8*Instance->SrcInfo.SrcIterations*6);
    break;
  case 16000:
    src236_static_param.src_mode = SRC236_RATIO_3;
    Instance->SrcInfo.SrcTypeSelected = SELECT_SRC_236;
    Instance->SrcInfo.SrcIterations = 1;
    input_frame_size = Instance->Buffers.BufferCtl.size/(8*Instance->SrcInfo.SrcIterations*3);
    break;
  case 24000:
    src236_static_param.src_mode = SRC236_RATIO_2;
    Instance->SrcInfo.SrcTypeSelected = SELECT_SRC_236;
    Instance->SrcInfo.SrcIterations = 1;
    input_frame_size = Instance->Buffers.BufferCtl.size/(8*Instance->SrcInfo.SrcIterations*2);
    break;
  case 32000:
    src236_static_param.src_mode = SRC236_RATIO_3_2;
    Instance->SrcInfo.SrcTypeSelected = SELECT_SRC_236;
    Instance->SrcInfo.SrcIterations = 2;   /* frame size smaller but processing repeated 2 times */
    input_frame_size = Instance->Buffers.BufferCtl.size/(8*Instance->SrcInfo.SrcIterations*3/2);
    break;
  case 44100:
    /* src441_static_param does not have params to be configured */
    Instance->SrcInfo.SrcTypeSelected = SELECT_SRC_441;
    Instance->SrcInfo.SrcIterations = 3;   /* frame size smaller but processing repeated 3 times */
    input_frame_size = (Instance->Buffers.BufferCtl.size/480)*441/(8*Instance->SrcInfo.SrcIterations);
    break;
  case 48000:
    Instance->SrcInfo.SrcTypeSelected = SELECT_SRC_NONE;
    Instance->SrcInfo.SrcIterations = 2;   /* frame size smaller but processing repeated 2 times considering SRC236 input req.*/
    input_frame_size = Instance->Buffers.BufferCtl.size/(8*Instance->SrcInfo.SrcIterations); /* half buff, stereo, byte x sample */
    break;
  case 96000:
    src236_static_param.src_mode = SRC236_RATIO_1_2;
    Instance->SrcInfo.SrcTypeSelected = SELECT_SRC_236;
    Instance->SrcInfo.SrcIterations = 4;  /* frame size smaller but processing repeated 4 times */
    input_frame_size = Instance->Buffers.BufferCtl.size/(8*Instance->SrcInfo.SrcIterations*1/2);
    break;
  default:
    PLAYER_Close(Instance);
    return (AUDIO_ERROR_SRC);
  }

  Instance->SrcInfo.AudioBufferSize = input_frame_size*4; /* stereo & byte x sample */

  if (Instance->SrcInfo.SrcTypeSelected == SELECT_SRC_236)
  {
    /* SRC236 effect reset */
    src_error = src236_reset(Instance->SrcInfo.pSrc236PersistentMem, Instance->SrcInfo.pSrcScratchMem);
    if (src_error != SRC236_ERROR_NONE)
    {
      PLAYER_Close(Instance);
      return (AUDIO_ERROR_SRC);
    }
    /* SRC236 effect static parameters setting */
    src_error = src236_setParam(&src236_static_param, Instance->SrcInfo.pSrc236PersistentMem);
    if (src_error != SRC236_ERROR_NONE)
    {
      PLAYER_Close(Instance);
      return (AUDIO_ERROR_SRC);
    }
  }

  if (Instance->SrcInfo.SrcTypeSelected == SELECT_SRC_441)
  {
    /* SRC236 effect reset */
    src_error = src441_reset(Instance->SrcInfo.pSrc441PersistentMem, Instance->SrcInfo.pSrcScratchMem);
    if (src_error != SRC441_ERROR_NONE)
    {
      PLAYER_Close(Instance);
      return (AUDIO_ERROR_SRC);
    }
    /* SRC236 effect static parameters setting */
    src_error = src441_setParam(&src441_static_param, Instance->SrcInfo.pSrc441PersistentMem);
    if (src_error != SRC441_ERROR_NONE)
    {
      PLAYER_Close(Instance);
      return (AUDIO_ERROR_SRC);
    }
  }
#endif /* !PLAYER_DISABLE_SRC */

  /* Buffers used for audio module interface after MemPool */
#ifdef PLAYER_DISABLE_SRC
  Instance->Buffers.InputBuffer.data_ptr = (uint8_t*)&(Instance->Buffers.BufferCtl.buff[0]);
#else /* PLAYER_DISABLE_SRC == 0 */
  Instance->Buffers.InputBuffer.data_ptr = &Instance->SrcInfo.WavToSrcTmpBuffer;
#endif /* PLAYER_DISABLE_SRC */
  Instance->Buffers.InputBuffer.nb_bytes_per_Sample = Instance->InputFileInfo.AudioFormatData.BitPerSample/8; /* 8 bits in 0ne byte */
  Instance->Buffers.InputBuffer.nb_channels = Instance->InputFileInfo.AudioFormatData.NbrChannels;
  Instance->Buffers.InputBuffer.mode = INTERLEAVED;
  Instance->Buffers.InputBuffer.buffer_size = input_frame_size;                    /* in number of samples */

  Instance->Buffers.OutputBuffer.nb_bytes_per_Sample = Instance->InputFileInfo.AudioFormatData.BitPerSample/8; /* 8 bits in 0ne byte */
  Instance->Buffers.OutputBuffer.nb_channels = Instance->InputFileInfo.AudioFormatData.NbrChannels;
  Instance->Buffers.OutputBuffer.mode = INTERLEAVED;
  Instance->Buffers.OutputBuffer.buffer_size = Instance->Buffers.BufferCtl.size/8; /* half buff of stereo samples in bytes */

  if(Instance->CurrentAudioState != AUDIO_STATE_INIT)
  {
    BSP_AUDIO_OUT_DeInit(0);
  }

#ifdef PLAYER_DISABLE_SRC
  /* Adjust the Audio frequency codec is always 48Khz for this application*/
  if (PlayerStreamInit(Instance->uwVolumeRef, Instance->InputFileInfo.AudioFormatData.SampleRate) != 0)
  {
    PLAYER_Close(Instance);
    return AUDIO_ERROR_CODEC;
  }
  BSP_AUDIO_OUT_SetFrequency(Instance->InputFileInfo.AudioFormatData.SampleRate);
#else /* !PLAYER_DISABLE_SRC == 0 */
  /* Adjust the Audio frequency codec is always 48Khz for this application*/
  if (PlayerStreamInit(Instance->uwVolumeRef, I2S_AUDIOFREQ_48K) != 0)
  {
    PLAYER_Close(Instance);
    return AUDIO_ERROR_CODEC;
  }
#endif /* PLAYER_DISABLE_SRC */

  /* Configure the Decoder task priority */
  if (Instance->InputFileInfo.FileExtension=='3')
  {
    /* Switch MP3 decoder priority to Low */
    osThreadSetPriority(Instance->DecThreadInfo.AudioDecThreadId, AUDIO_DEC_THREAD_PRIORITY_L);
  }
  else
  {
    /* Switch WAV decoder priority to High */
    osThreadSetPriority(Instance->DecThreadInfo.AudioDecThreadId, AUDIO_DEC_THREAD_PRIORITY_H);
  }

  /*** Resume Audio Decoder thread ***/
  if(Instance->DecThreadInfo.AudioDecThreadId != 0)
  {
    osThreadResume(Instance->DecThreadInfo.AudioDecThreadId);
  }

  /*** Get Data from Media Storage ***/

  /*++ Loop1 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  /* -- Write MemPool and Fill Buffer1 from Audio file at start
  (same code as callback processing of EXT0 line SWI) */
  DecodeAudioBuffer(Instance, 0);

  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  /* -- Loop2: Process MemPool and Fill complete double output buffer at first
  time */
  error = SRC_ProcessBuffer(Instance, 0);
  if ( error == AUDIO_ERROR_NONE)
  {
    error = SRC_ProcessBuffer(Instance, Instance->Buffers.BufferCtl.size/2);
  }

  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  /* Loop1 (again if new available space) */
  DecodeAudioBuffer(Instance, 0);
  
  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  /*  Play complete double output buffer at first time */
  if ( error == AUDIO_ERROR_NONE)
  {
    if(Instance->Buffers.BufferCtl.fptr != 0)
    {
#ifdef AUDIO_DEBUG
      StartTick = HAL_GetTick();
#endif /* AUDIO_DEBUG */

      /*** Resume Audio Output thread ***/
      if(Instance->OutThreadInfo.AudioOutThreadId != 0)
      {
        osThreadResume(Instance->OutThreadInfo.AudioOutThreadId);
      }

      /*DMA stream from output double buffer to codec in Circular mode launch*/
      BSP_AUDIO_OUT_Play(0,(uint8_t *)&(Instance->Buffers.BufferCtl.buff[0]), Instance->Buffers.BufferCtl.size);

      /* Set Volume */
      // BSP_AUDIO_OUT_SetVolume(Instance->uwVolumeRef);

      Instance->CurrentAudioState = AUDIO_STATE_PLAY;

      return AUDIO_ERROR_NONE;
    }
  }

  PLAYER_Stop(Instance);

  return error;
}

/**
  * @brief  Decode Audio buffers.
  *         Write MemPool and Fill Buffer1 from Audio file
  * @param  Instance: Audio Player instance
  * @param  nb_buffers : number of buffers to decode; if '0' decode until no free memory
  * @retval Audio error
  */
AUDIO_ErrorTypeDef DecodeAudioBuffer(PLAYER_Instance_t *Instance, uint16_t nb_buffers)
{
  int16_t iteration     = (int16_t)nb_buffers;
  int32_t sizeAvailable = 0;

#ifdef PLAYER_DISABLE_SRC
  /* Decode only one buffer */
  iteration = 1;
#endif /* PLAYER_DISABLE_SRC */

  Instance->Decoder.CurrentDecoderState = DECODER_STATE_RUNNING;

  /* Check the end of file */
  if (Instance->InputFileInfo.AudioFile.fptr >= f_size(&Instance->InputFileInfo.AudioFile))
  {
    Instance->Decoder.CurrentDecoderState = DECODER_STATE_EOF;
    FWK_MEMPOOL_Free(Instance->hAudio->pMemPool);
    return AUDIO_ERROR_NONE;
  }

  /*++ Loop1 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  /* -- Write MemPool and Fill Buffer1 from Audio file at start (same code as callback processing of EXT0 line SWI) */
  FWK_MEMPOOL_GetAvailableSize(Instance->hAudio->pMemPool, &sizeAvailable);
  while (sizeAvailable >= Instance->Decoder.sDecoderStruct.PacketSize)
  {
    uint32_t decsize = 0; /*returned output decoder size in bytes*/
    static uint32_t offset = 0;

    if(Instance->Decoder.CurrentDecoderState == DECODER_STATE_STOP)
    {
      /* Someone asked to immediately stop decoder */
      return AUDIO_ERROR_NONE;
    }

    /* Audio Decoder between Audio File (MST) and Buffer1 then MemPool writing */
    if (Instance->Decoder.sDecoderStruct.Decoder_DecodeData != NULL)
    {
      uint32_t DecSampNbBytes=Instance->InputFileInfo.AudioFormatData.BitPerSample/8;    /* nb bytes per sample - 2 bytes*/
      uint32_t DecNumChannels= Instance->InputFileInfo.AudioFormatData.NbrChannels;      /* stereo channels? */

      decsize = Instance->Decoder.sDecoderStruct.Decoder_DecodeData((__IO int16_t*)(&Buffer1[offset]), (Instance->Decoder.sDecoderStruct.PacketSize/(DecNumChannels*DecSampNbBytes)), NULL);
    }

    if(decsize != 0)
    {
#ifndef PLAYER_DISABLE_SRC_ZERO_COPY
      /*write Buffer1 to MemPool*/
      FWK_MEMPOOL_Write(Instance->hAudio->pMemPool, (uint8_t*)(&Buffer1[offset]), decsize); /* input buffer1 in bytes */
#else /* PLAYER_DISABLE_SRC_ZERO_COPY */
      /* increment MemPool write pointer */
      FWK_MEMPOOL_WritePtrInc(Instance->hAudio->pMemPool, decsize); /* input buffer1 in bytes */
      offset = (offset == 0) ? 1 : 0; /* flip flop */
#endif /* !PLAYER_DISABLE_SRC_ZERO_COPY */
      iteration--;
    }
    else /* End of file detected */
    {
      Instance->Decoder.CurrentDecoderState = DECODER_STATE_EOF;
      FWK_MEMPOOL_Free(Instance->hAudio->pMemPool);
      return AUDIO_ERROR_NONE;
    }

    if(iteration == 0) /* will never happen if nb_buffers == 0 */
    {
      break;
    }

    /*update available size in MemPool*/
    FWK_MEMPOOL_GetAvailableSize(Instance->hAudio->pMemPool, &sizeAvailable);
  }

  Instance->Decoder.CurrentDecoderState = DECODER_STATE_IDLE;
  return AUDIO_ERROR_NONE;
}

/**
  * @brief  Select Audio file.
  * @param  pHandle: Audio Player handle
  * @param  file_name: audio file.
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  PLAYER_SelectFile(PLAYER_HANDLE_h pHandle, char* file_name)
{
  AUDIO_ErrorTypeDef ret = AUDIO_ERROR_IO;

  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if(NULL == Instance)
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }

  /*-- Select decoder type and Get info from header --*/
  ret=PlayerGetFileInfo(Instance, file_name, &Instance->InputFileInfo.AudioFormatData);
  if(ret != AUDIO_ERROR_NONE)
  {
    return AUDIO_ERROR_CODEC;
  }

  PLAYER_Close(Instance);
  return ret;  
}

/**
  * @brief  Stops Audio streaming.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef PLAYER_Stop(PLAYER_HANDLE_h pHandle)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if(NULL == Instance)
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }

  Instance->CurrentAudioState = AUDIO_STATE_STOP;

  /*Stop the output audio stream*/
  BSP_AUDIO_OUT_Stop(0);

  /* Close Player */
  PLAYER_Close(Instance);

  return AUDIO_ERROR_NONE;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  PLAYER_GetVolume(PLAYER_HANDLE_h pHandle)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if(NULL == Instance)
  {
    return 0;
  }

  return Instance->uwVolumeRef;
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AUDIO_ErrorTypeDef  PLAYER_SetVolume(PLAYER_HANDLE_h pHandle, uint32_t volume)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if(NULL == Instance)
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }

  if((volume > 0) && (volume <= 100))
  {
    Instance->uwVolumeRef = volume;
    PLAYER_SetConfig(pHandle, AUDIO_CFG_VOLUME_SET);
    return AUDIO_ERROR_NONE;    
  }
  else
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }
}

/**
* @brief  Detect touch screen state and modify audio state machine accordingly
* @param  None
* @retval None
*/
AUDIO_ErrorTypeDef PLAYER_SetConfig(PLAYER_HANDLE_h pHandle, AUDIO_PLAYBACK_CfgTypeDef AudioConfig)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if(NULL == Instance)
  {
    return AUDIO_ERROR_INVALID_PARAMS;
  }

  switch(AudioConfig)
  {
  case AUDIO_CFG_VOLUME_UP:
    if( Instance->uwVolumeRef <= 90)
    {
      Instance->uwVolumeRef += 10;
    }
    BSP_AUDIO_OUT_SetVolume(0,Instance->uwVolumeRef);
    break;

  case AUDIO_CFG_VOLUME_DOWN:
    if( Instance->uwVolumeRef > 90)
    {
      Instance->uwVolumeRef -= 10;
    }
    BSP_AUDIO_OUT_SetVolume(0,Instance->uwVolumeRef);
    break;

  case AUDIO_CFG_VOLUME_SET:
    BSP_AUDIO_OUT_SetVolume(0,Instance->uwVolumeRef);
    break;

  case AUDIO_CFG_MUTE_ON:
    BSP_AUDIO_OUT_Mute(0);
    break;

  case AUDIO_CFG_MUTE_OFF:
   BSP_AUDIO_OUT_UnMute(0);
    break;

  case AUDIO_CFG_PAUSE:
    if(BSP_AUDIO_OUT_Pause(0) == BSP_ERROR_NONE)
    {
      Instance->CurrentAudioState = AUDIO_STATE_PAUSE;
    }
    break;

  case AUDIO_CFG_RESUME:
    if(BSP_AUDIO_OUT_Resume(0) == BSP_ERROR_NONE)
    {
      Instance->CurrentAudioState = AUDIO_STATE_PLAY;
    }
    break;

  case AUDIO_CFG_NONE:
  default:
    break;
  }

  return AUDIO_ERROR_NONE;
}

/**
* @brief  Manages the DMA Complete Transfer interrupt
*         Calculates the remaining file size and new position of the pointer.
* @param  None
* @retval None
*/
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
#ifdef AUDIO_DEBUG
  /* For Debug purpos */
  uint32_t CurrentTick = 0;

  CurrentTick     = HAL_GetTick();
  DurationInTicks = CurrentTick - StartTick;
  StartTick       = CurrentTick;
#endif /* AUDIO_DEBUG */

  /* Iterate on all created instance ! */
  for(uint8_t instance=0; instance < MAX_PLAYER_INSTANCES_NBR; instance++)
  {
    PLAYER_Instance_t *Instance = PlayerInstances[instance];

    if(NULL != Instance)
    {
      if(Instance->CurrentAudioState == AUDIO_STATE_PLAY)
      {
        Instance->Buffers.BufferCtl.state = BUFFER_OFFSET_FULL;

        /* Trigger the Audio Player Output Processing task */
        osMessagePut ( Instance->OutThreadInfo.AudioOutEvent, BUFFER_OFFSET_FULL, 0);
      }
    }
  }
}

/**
* @brief  Manages the DMA Half Transfer interrupt.
* @param  None
* @retval None
*/
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Iterate on all created instance ! */
  for(uint8_t instance=0; instance < MAX_PLAYER_INSTANCES_NBR; instance++)
  {
    PLAYER_Instance_t *Instance = PlayerInstances[instance];

    if(NULL != Instance)
    {
      if(Instance->CurrentAudioState == AUDIO_STATE_PLAY)
      {
        Instance->Buffers.BufferCtl.state = BUFFER_OFFSET_HALF;

        /* Trigger the Audio Player Output Processing task */
        osMessagePut ( Instance->OutThreadInfo.AudioOutEvent, BUFFER_OFFSET_HALF, 0);
      }
    }
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  /* Iterate on all created instance ! */
  for(uint8_t instance=0; instance < MAX_PLAYER_INSTANCES_NBR; instance++)
  {
    PLAYER_Instance_t *Instance = PlayerInstances[instance];

    if(NULL != Instance)
    {
      if(Instance->CurrentAudioState == AUDIO_STATE_PLAY)
      {
        PLAYER_Stop(Instance);
      }
      Instance->CurrentAudioState = AUDIO_STATE_ERROR;
    }
  }
}

/**
* @brief  Callback function to supply the decoder with input  bitsteram.
* @param  pCompressedData: pointer to the target buffer to be filled.
* @param  nDataSizeInChars: number of data to be read in bytes
* @param  pUserData: optional parameter (not used in this version)
* @retval return the decode frame.
*/
static uint32_t Dec_ReadDataCallback(void *    pCompressedData,        /* [OUT] Bitbuffer */
                                  uint32_t  nDataSizeInChars,       /* sizeof(Bitbuffer) */
                                  void *    pUserData               /* Application-supplied parameter */ )
{
  /* data number read */
  uint32_t NumberOfData = 0;
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pUserData;

  uint32_t error_tmp = 0x00;

  error_tmp = f_read(&Instance->InputFileInfo.AudioFile, pCompressedData, nDataSizeInChars, (UINT*)(&NumberOfData));

  /* Check read issue */
  if (error_tmp != FR_OK)
  {
    Instance->CurrentAudioState = AUDIO_STATE_ERROR;
  }

  return  NumberOfData;
}

/**
* @brief  Sets a new progress time value.
* @param  Pos: pointer position to be set
* @retval None
*/
uint32_t PLAYER_SeekToTime(PLAYER_HANDLE_h pHandle, uint32_t NewTime)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;

  if(Instance)
  {
    if(Instance->Decoder.sDecoderStruct.Decoder_SetProgressTime != NULL)
    {
      uint32_t retval = 0;
      AUDIO_StateTypeDef PreviousAudioState = Instance->CurrentAudioState;

      /* Make sure the output audio stream is stopped */
      Instance->CurrentAudioState = AUDIO_STATE_STOP;
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);

      /* Stop decoder */
      Instance->Decoder.CurrentDecoderState = DECODER_STATE_STOP;
      osDelay(10);

      /* Reset Buffers */
      ResetBuffers(Instance);

      /* Seek to new time */
      retval = Instance->Decoder.sDecoderStruct.Decoder_SetProgressTime(NewTime);

      /* Restart the output audio stream from new time */
      if(!retval)
      {
        Instance->Buffers.BufferCtl.fptr = 0;

        DecodeAudioBuffer(Instance, 0);
        if ( SRC_ProcessBuffer(Instance, 0) == AUDIO_ERROR_NONE)
        {
          if(SRC_ProcessBuffer(Instance, Instance->Buffers.BufferCtl.size/2) == AUDIO_ERROR_NONE)
          {
            DecodeAudioBuffer(Instance, 0);
            if(Instance->Buffers.BufferCtl.fptr > 0)
            {
              BSP_AUDIO_OUT_Play(0,(uint8_t *)&(Instance->Buffers.BufferCtl.buff[0]), Instance->Buffers.BufferCtl.size);
              if(PreviousAudioState == AUDIO_STATE_PAUSE)
              {
                /* Pause Player after seeking */
                if(BSP_AUDIO_OUT_Pause(0) != BSP_ERROR_NONE)
                {
                  return 1;
                }
              }
              Instance->CurrentAudioState = PreviousAudioState;
            }
          }
        }
      }
    }
  }

  return 0;
}


/**
* @brief  get file size
* @param  None
* @retval file size in bytes
*/
uint32_t PLAYER_GetFileLength(PLAYER_HANDLE_h pHandle)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;

  if(Instance)
  {
    /* Call the Fat FS seek function */
    return f_size(&Instance->InputFileInfo.AudioFile);
  }
  else
  {
    return 0;
  }
}


/**
* @brief  Updates the current time information (expressed in seconds).
* @param  Length: pointer to the variable containing the total audio track length
* @param  Total: pointer to the variable containing the elapsed time
* @retval 0 if Passed, !0 else.
*/
uint32_t PLAYER_GetTimeInfo(PLAYER_HANDLE_h pHandle, uint32_t* Length, uint32_t* Elapsed)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;

  if(Instance)
  {
    if(strlen(Instance->InputFileInfo.FilePath) > 0) /* Only if a file was already selected */
    {
      if (NULL != Length)
      {
        if (NULL == Instance->Decoder.sDecoderStruct.Decoder_GetStreamLength)
        {
          /* Playback not running yet */
          if(PlayerGetFileInfo(Instance, Instance->InputFileInfo.FilePath, &Instance->InputFileInfo.AudioFormatData) != AUDIO_ERROR_NONE)
          {
            return AUDIO_ERROR_CODEC;
          }
          *Length = Instance->InputFileInfo.AudioFormatData.Duration;
          PLAYER_Close(Instance);
        }
        else
        {
          *Length = Instance->InputFileInfo.AudioFormatData.Duration;
        }
      }
      if (NULL != Elapsed)
      {
        if(NULL != Instance->Decoder.sDecoderStruct.Decoder_GetElapsedTime)
        {
          *Elapsed = Instance->Decoder.sDecoderStruct.Decoder_GetElapsedTime(Instance->InputFileInfo.AudioFile.fptr);
        }
        else
        {
          *Elapsed = 0; /* Playback not yet started */
        }
      }
      return AUDIO_ERROR_NONE;
    }
  }

  return AUDIO_ERROR_NOT_INITIALIZED;
}

/**
* @brief  Get the Audio Player State
* @param  None
* @retval Current Audio Player State
*/
AUDIO_StateTypeDef PLAYER_GetState(PLAYER_HANDLE_h pHandle)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;
  if(Instance)
  {
    return Instance->CurrentAudioState;
  }
  else
  {
    return AUDIO_STATE_ERROR;
  }
}

/**
* @brief  Register User's Pre/Post Processing Callback
* @param  None
* @retval None
*/
void PLAYER_RegisterCallbacks( PLAYER_HANDLE_h pHandle,
                              fnUserCallback_TypeDef*      pPrePrcessingCallback,
                              fnUserCallback_TypeDef*      pPostPrcessingCallback,
                              fnEndOfFileCallback_TypeDef* pEndOfFileCallback)
{
  PLAYER_Instance_t *Instance = (PLAYER_Instance_t *)pHandle;

  if(Instance)
  {
    Instance->ProcessCb.pPrePrcessingCallback  = pPrePrcessingCallback;
    Instance->ProcessCb.pPostPrcessingCallback = pPostPrcessingCallback;
    Instance->ProcessCb.pEndOfFileCallback     = pEndOfFileCallback;
  }
}


/* Default Memory Pool Lock/Unlock functions */
void FWK_MEMPOOL_Lock(FWK_MEMPOOL_t *pMemPool)
{
  if(pMemPool && pMemPool->lock)
  {
    osMutexWait((void *)pMemPool->lock, osWaitForever);
  }
}

void FWK_MEMPOOL_Unlock(FWK_MEMPOOL_t *pMemPool)
{
  if(pMemPool && pMemPool->lock)
  {
    osMutexRelease((void *)pMemPool->lock);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

