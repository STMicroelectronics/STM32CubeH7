/**
  ******************************************************************************
  * @file    GUI_AVI.c
  * @author  MCD Application Team
  * @brief   This file provides routines for AVI decoding.
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
#include <string.h>

#include "GUI_Private.h"
#include "GUI_AVI.h"

#if GUI_WINSUPPORT
  #include "WM.h"
#endif
#include "video_player_app.h"
#include "audio_player_app.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
static U8* const AVI_VIDS_FLAG_TBL[2]={(U8 *)"00dc",(U8 *)"01dc"};
static U8* const AVI_AUDS_FLAG_TBL[2]={(U8 *)"00wb",(U8 *)"01wb"};

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="audio_buffers"
#else
__attribute__((section(".audio_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t AVI_AudioChunk[8]; /* Used to read audio chunks and then seek audio frames */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Search AVI ID
  * @param  None
  * @retval ID
  */
static U32 _AVI_SearchID(U8* buf,U32 size,U8 *id)
{
  U32 i;
  size-=4;
  for(i=0;i<size;i++)
  {
    if(buf[i] == id[0])
    {
      if(buf[i+1] == id[1])
      {
        if(buf[i+2] == id[2])
        {
          if(buf[i+3] == id[3])
          {
            return i;
          }
        }
      }
    }
  }
  return 0;
}

/**
  * @brief  Get AVI file information
  * @param  pavi: AVI handle
  * @param  buf:
  * @retval AVI status
  */
static AVISTATUS _AVI_GetStreamInfo(AVI_CONTEXT * pavi, U8* buf)
    {
      pavi->aviInfo.StreamID = MAKEWORD (buf+2);
      pavi->aviInfo.StreamSize = MAKEDWORD (buf+4);

      if(pavi->aviInfo.StreamSize%2)
      {
        pavi->aviInfo.StreamSize++;
      }

      if((pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG) || (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG))
      {
        return AVI_OK;
      }

  return AVI_STREAM_ERR;
}

/**
  * @brief  Initialize AVI
  * @param  pavi: AVI handle
  * @param  buf:
  * @param  size: AVI file size
  * @retval AVI status
  */
static AVISTATUS _AVI_Init(AVI_CONTEXT * pavi, U8 *buf, U32 size)
{
  U16 offset;
  U8 *tbuf;
  AVISTATUS   res = AVI_OK;
  AVI_HEADER *aviheader;
  LIST_HEADER *listheader;
  AVIH_HEADER *avihheader;
  STRH_HEADER *strhheader;

  STRF_BMPHEADER *bmpheader;
  STRF_WAVHEADER *wavheader;

  tbuf=buf;
  aviheader=(AVI_HEADER*)buf;
  if(aviheader->RiffID != AVI_RIFF_ID)
  {
    return AVI_RIFF_ERR;
  }

  if(aviheader->AviID != AVI_AVI_ID)
  {
    return AVI_AVI_ERR;
  }

  buf+=sizeof(AVI_HEADER);
  listheader=(LIST_HEADER*)(buf);

  if(listheader->ListID != AVI_LIST_ID)
  {
    return AVI_LIST_ERR;
  }

  if(listheader->ListType != AVI_HDRL_ID)
  {
    return AVI_HDRL_ERR;
  }

  buf+=sizeof(LIST_HEADER);
  avihheader=(AVIH_HEADER*)(buf);
  if(avihheader->BlockID != AVI_AVIH_ID)
  {
    return AVI_AVIH_ERR;
  }

  pavi->aviInfo.SecPerFrame=avihheader->SecPerFrame;
  pavi->aviInfo.TotalFrame=avihheader->TotalFrame;
  buf+=avihheader->BlockSize+8;
  listheader=(LIST_HEADER*)(buf);

  if(listheader->ListID != AVI_LIST_ID)
  {
    return AVI_LIST_ERR;
  }

  if(listheader->ListType != AVI_STRL_ID)
  {
    return AVI_STRL_ERR;
  }

  strhheader=(STRH_HEADER*)(buf+12);
  if(strhheader->BlockID != AVI_STRH_ID)
  {
    return AVI_STRH_ERR;
  }

  if(strhheader->StreamType == AVI_VIDS_STREAM)
  {
    if(strhheader->Handler != AVI_FORMAT_MJPG)
    {
      return AVI_FORMAT_ERR;
    }

    pavi->aviInfo.VideoFLAG=(U8*)AVI_VIDS_FLAG_TBL[0];
    pavi->aviInfo.AudioFLAG=(U8*)AVI_AUDS_FLAG_TBL[1];
    bmpheader=(STRF_BMPHEADER*)(buf+12+strhheader->BlockSize+8);
    if(bmpheader->BlockID != AVI_STRF_ID)
    {
      return AVI_STRF_ERR;
    }

    pavi->aviInfo.Width=bmpheader->bmiHeader.Width;
    pavi->aviInfo.Height=bmpheader->bmiHeader.Height;
    buf+=listheader->BlockSize+8;
    listheader=(LIST_HEADER*)(buf);
    if(listheader->ListID != AVI_LIST_ID)
    {
      pavi->aviInfo.SampleRate=0;
      pavi->aviInfo.Channels=0;
      pavi->aviInfo.AudioType=0;
    }
    else
    {
      if(listheader->ListType != AVI_STRL_ID)
      {
        return AVI_STRL_ERR;
      }

      strhheader=(STRH_HEADER*)(buf+12);
      if(strhheader->BlockID != AVI_STRH_ID)
      {
        return AVI_STRH_ERR;
      }

      if(strhheader->StreamType != AVI_AUDS_STREAM)
      {
        return AVI_FORMAT_ERR;
      }

      wavheader=(STRF_WAVHEADER*)(buf+12+strhheader->BlockSize+8);
      if(wavheader->BlockID != AVI_STRF_ID)
      {
        return AVI_STRF_ERR;
      }

      pavi->aviInfo.SampleRate=wavheader->SampleRate;
      pavi->aviInfo.Channels=wavheader->Channels;
      pavi->aviInfo.AudioType=wavheader->FormatTag;
    }
  }
  else if(strhheader->StreamType == AVI_AUDS_STREAM)
  {
    pavi->aviInfo.VideoFLAG=(U8*)AVI_VIDS_FLAG_TBL[1];
    pavi->aviInfo.AudioFLAG=(U8*)AVI_AUDS_FLAG_TBL[0];
    wavheader=(STRF_WAVHEADER*)(buf+12+strhheader->BlockSize+8);
    if(wavheader->BlockID != AVI_STRF_ID)
    {
      return AVI_STRF_ERR;
    }

    pavi->aviInfo.SampleRate=wavheader->SampleRate;
    pavi->aviInfo.Channels=wavheader->Channels;
    pavi->aviInfo.AudioType=wavheader->FormatTag;
    buf+=listheader->BlockSize+8;
    listheader=(LIST_HEADER*)(buf);
    if(listheader->ListID != AVI_LIST_ID)
    {
      return AVI_LIST_ERR;
    }

    if(listheader->ListType != AVI_STRL_ID)
    {
      return AVI_STRL_ERR;
    }

    strhheader=(STRH_HEADER*)(buf+12);
    if(strhheader->BlockID != AVI_STRH_ID)
    {
      return AVI_STRH_ERR;
    }

    if(strhheader->StreamType != AVI_VIDS_STREAM)
    {
      return AVI_FORMAT_ERR;
    }

    bmpheader=(STRF_BMPHEADER*)(buf+12+strhheader->BlockSize+8);
    if(bmpheader->BlockID != AVI_STRF_ID)
    {
      return AVI_STRF_ERR;
    }

    if(bmpheader->bmiHeader.Compression != AVI_FORMAT_MJPG)
    {
      return AVI_FORMAT_ERR;
    }

    pavi->aviInfo.Width=bmpheader->bmiHeader.Width;
    pavi->aviInfo.Height=bmpheader->bmiHeader.Height;
  }
  offset=_AVI_SearchID(tbuf,size,(U8 *)"movi");
  if(offset == 0)
  {
    return AVI_MOVI_ERR;
  }

  if(pavi->aviInfo.SampleRate)
  {
    tbuf+=offset;
    offset=_AVI_SearchID(tbuf,size,pavi->aviInfo.AudioFLAG);
    if(offset == 0)
    {
      return AVI_STREAM_ERR;
    }

    tbuf+=offset+4;
    pavi->aviInfo.AudioBufSize=*((U16*)tbuf);
  }

  return res;
}

/**
  * @brief  Get data.
  * @param  p: Handle to file structure
  * @param  ppData: pointer to data buffer to be read
  * @param  NumBytesReq: number of bytes to be read
  * @param  Off: offset in the file    
  * @retval number of read bytes
  */
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off)
{
  unsigned int NumBytesRead = 0;
  FRESULT res;

  if(Off != 0xFFFFFFFF)
  {
    /* Set file pointer to the required position */
    f_lseek((FIL *)p, Off);
  }

  /* Read data into buffer */
  res = f_read((FIL *)p, (U8 *)*ppData, NumBytesReq, &NumBytesRead);
  
  if(res != FR_OK)
  {
    NumBytesRead = 0;
  }

  /* Return number of available bytes */
  return NumBytesRead;  
}

/**
  * @brief  Seek to offset in file.
  * @param  p: Handle to file structure
  * @param  NumBytesToSeek: number of bytes to be seek after
  * @param  Off: new offset in the file
  * @retval 0 if success
  */
static U32 _SeekFile(void * p, unsigned NumBytesToSeek, U32 Off)
{
  U32 Res = 0;
  unsigned long Pos = (unsigned long)Off;

  if(Off == 0xFFFFFFFF)
  {
    /* Set file pointer to the required position */
    Pos = ((FIL *)p)->fptr;
  }

  Pos += NumBytesToSeek;

  Res = (U32)f_lseek((FIL *)p, Pos);
  return Res;
}

/**
  * @brief  Stop AVI
  * @param  pavi: AVI handle
  * @retval
  */
static int _Stopavi(AVI_CONTEXT *pavi)
{
  int r;

  r = 1;
  if (pavi->hTimer)
  {
    GUI_TIMER_Delete(pavi->hTimer);
    pavi->hTimer = 0;
    r = 0;
  }

  return r;
}

static int _CheckEndOfFile(AVI_CONTEXT * pavi)
{
  if (pavi->CurrentImage  ==  pavi->aviInfo.TotalFrame)
  {
    if (pavi->DoLoop)
    {
      int offset;
      pavi->CurrentImage = 0;

      if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->VideoBufferSize, 0)  !=  pavi->VideoBufferSize)
      {
        return -1;
      }
      _AVI_Init(pavi, (U8 *)pavi->pVideoBuffer, pavi->VideoBufferSize);
      offset = _AVI_SearchID((U8 *)pavi->pVideoBuffer,pavi->VideoBufferSize,(U8 *)"movi");
      _AVI_GetStreamInfo(pavi, (U8 *)pavi->pVideoBuffer + offset + 4);

      if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8, offset + 12)  !=  (pavi->aviInfo.StreamSize + 8))
      {
        return -1;
      }
    }
    else
    {
      return -1; /* EOF */
    }
  }

  return 0;
}
/**
  * @brief  Decode AVI frame
  * @param  pavi: AVI handle
  * @retval
  */
static int _DecodeFrame(AVI_CONTEXT * pavi, int drop)
{
  const U8  *pData;

    do
    {
      pavi->CurrentImage++;
      pavi->TimeNextFrame += pavi->aviInfo.SecPerFrame/ 1000;

      if(!_CheckEndOfFile(pavi)) /* Not the end of the file */
      {
        if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
        {
          if(pavi->aviInfo.StreamSize > pavi->VideoBufferSize)
          {
            return -1;
          }

          if(drop == 0) /* Decode this frame */
          {
            if(pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8, 0xFFFFFFFF)  !=  pavi->aviInfo.StreamSize + 8)
            {
              return -1;
            }
          }
          else /* drop it */
          {
            if(pavi->pfSeek(pavi->pParam, pavi->aviInfo.StreamSize, 0xFFFFFFFF)  !=  0)
            {
              return -1;
            }
            /* Read next Chunk data */
            pData = &pavi->pVideoBuffer[pavi->aviInfo.StreamSize];
            if(pavi->pfGetData(pavi->pParam, &pData, 8, 0xFFFFFFFF)  != 8)
            {
              return -1;
            }
          }

          /* Get Next buffer info */
          _AVI_GetStreamInfo(pavi, (U8 *)pavi->pVideoBuffer + pavi->aviInfo.StreamSize);
        }

        if(pavi->aviInfo.StreamID == AVI_AUDS_FLAG)
        {
          do
          {
            if(pavi->aviInfo.StreamSize > pavi->AudioBufferSize)
            {
              return -2;
            }

            /* Skip Current Audio chunk data */
            if(pavi->pfSeek(pavi->pParam, pavi->aviInfo.StreamSize, 0xFFFFFFFF)  !=  0)
            {
              return -2;
            }

            /* Read next Chunk data */
            pData = &pavi->pAudioBuffer[0];
            if(pavi->pfGetData(pavi->pParam, &pData, 8, 0xFFFFFFFF)  != 8)
            {
              return -2;
            }

            /* Get Next buffer info */
            if (_AVI_GetStreamInfo(pavi, (U8 *)pData) != AVI_OK)
            {
              return 2; /* EOF */
            }

          } while (pavi->aviInfo.StreamID !=  AVI_VIDS_FLAG); /* Decode until next video frame */
        }
      }
      else
      {
        /* EOF */
        return 1;
      }
      drop--;
    } while(drop >= 0);

  return 0;
}

/**
  * @brief  timer callback
  * @param  pTM: pointer to the timer
  * @retval
  */
static void _OnTimer(GUI_TIMER_MESSAGE * pTM)
{
  AVI_CONTEXT * pavi;
  GUI_AVI_FUNC * pfNotify = NULL;
  GUI_AVI_HANDLE havi;
  int Period = 1;
  int SkipNFrames = 0;
  int DecodeError = 0;
  int retval  = 0;

  havi = pTM->Context;
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
  {
    if(pavi->CurrentImage == 0)
    {
      pavi->TimeNextFrame = GUI_GetTime();
    }

    /* Skip images if require more time */
    if ((U32)GUI_GetTime() > pavi->TimeNextFrame)
    {
      SkipNFrames = ((U32)GUI_GetTime() - pavi->TimeNextFrame) / (pavi->aviInfo.SecPerFrame / 1000);
    }

    DecodeError = _DecodeFrame(pavi, SkipNFrames);

    if (DecodeError)
    {
      /* EOF/Skip Errors to be notified once the last frame is displayed */
      if (pavi->pfNotify)
      {
        U32 TotalFrame = pavi->aviInfo.TotalFrame;
        pfNotify = pavi->pfNotify;
        pavi = GUI_UNLOCK_H(pavi);
        if (DecodeError == -1) /* Skip */
        {
          pfNotify(havi, GUI_AVI_NOTIFICATION_SKIP, TotalFrame);
        }
        else if (DecodeError == 1) /* EOF */
        {
          pfNotify(havi, GUI_AVI_NOTIFICATION_EOF, TotalFrame);
        }
        pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
      }
    }
    else
    {
      /* Present last frame */
      if(pavi->CurrentImage > 0) /* Have already decoded at least one frame */
      {
        if (pavi->pfNotify)
        {
          U32 CurrentImage = pavi->CurrentImage;
          pfNotify = pavi->pfNotify;
          pavi = GUI_UNLOCK_H(pavi);
          pfNotify(havi, GUI_AVI_NOTIFICATION_PREDRAW, CurrentImage);
          pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
        }

        /* Handle Video frame */
        WM_Deactivate();
        retval = pavi->pfDraw(pavi->pVideoBuffer, pavi->VideoBufferSize, pavi->xPos, pavi->yPos);
        WM_Activate();

        if ((retval == 0) && (pavi->pfNotify))
        {
          pfNotify = pavi->pfNotify;
          pavi = GUI_UNLOCK_H(pavi);
          pfNotify(havi, GUI_AVI_NOTIFICATION_POSTDRAW, SkipNFrames);
          pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
        }
      }
      Period = pavi->TimeNextFrame - (U32)GUI_GetTime();
    }

    if (Period <= 0)
    {
      Period = 1;
    }
  }
  pavi = GUI_UNLOCK_H(pavi);

  GUI_TIMER_SetPeriod(pTM->hTimer, Period);
  GUI_TIMER_Restart(pTM->hTimer);
}


/**
  * @brief  Creates a movie handle
  * @param  pfGetData: pointer to GetData function
  * @param  pfNotify:  pointer to Notification callback function
  * @retval AVI status
  */
GUI_AVI_HANDLE GUI_AVI_CreateEx(GUI_GET_DATA_FUNC * pfGetData, GUI_AVI_FUNC * pfNotify, GUI_SEEK_FUNC * pfSeek)
{
  AVI_CONTEXT  * pavi;
  GUI_AVI_HANDLE havi = 0;

  /* Allocate context for drawing operation */
  GUI_LOCK();
  havi = GUI_ALLOC_AllocZero(sizeof(AVI_CONTEXT));
  if (havi)
  {
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      /*  Save parameters */
      pavi->pfNotify  = pfNotify;
      pavi->pfGetData = pfGetData;
      pavi->pfSeek = pfSeek;
      pavi->pfDraw = (U32(*)(const void * , U32 ,U32, U32))GUI_JPEG_Draw;
    }
    GUI_UNLOCK_H(pavi);
  }
  GUI_UNLOCK();
  return havi;
}

/**
  * @brief  Starts movies showing
  * @param  havi: AVI handle
  * @param  pParam:  AVI file params
  * @param  FileSize:  AVI file size
  * @retval AVI status
  */
int GUI_AVI_Start(GUI_AVI_HANDLE havi, void * pParam, U32 FileSize)
{
  AVI_CONTEXT  * pavi;
  U32 offset;
  int r = 1;
  const U8  *pData;

  if (havi)
  {
    GUI_LOCK();
    r = 1;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      /*  Save parameters */
      pavi->pParam    = pParam;
      pavi->FileSize  = FileSize;
      pavi->CurrentImage = 0;

      if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->VideoBufferSize, 0)  ==  pavi->VideoBufferSize)
      {
        /* Parse AVI container */
        _AVI_Init(pavi, (U8 *)pavi->pVideoBuffer, pavi->VideoBufferSize);
        offset = _AVI_SearchID((U8 *)pavi->pVideoBuffer,pavi->VideoBufferSize,(U8 *)"movi");
        _AVI_GetStreamInfo(pavi, (U8 *)pavi->pVideoBuffer + offset + 4);

        /* Init Hardware */
        if(pavi->pfJpegInit)
        {
          pavi->pfJpegInit();
        }

        /* Read first stream */
        if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
        {
          if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8, offset + 12)  ==  (pavi->aviInfo.StreamSize + 8))
          {
            _AVI_GetStreamInfo(pavi, (U8 *)pavi->pVideoBuffer + pavi->aviInfo.StreamSize);
          }
        }

        if(pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
        {
          do
          {
            /* Skip Current Audio chunk data */
            if(pavi->pfSeek(pavi->pParam, pavi->aviInfo.StreamSize, 0xFFFFFFFF)  !=  0)
            {
              r = -1;
              break;
            }

            /* Read next Chunk data */
            pData = &pavi->pAudioBuffer[0];
            pavi->pfGetData(pavi->pParam, &pData, 8, 0xFFFFFFFF);

            /* Get Next buffer info */
            _AVI_GetStreamInfo(pavi, (U8 *)pData);

          } while (pavi->aviInfo.StreamID !=  AVI_VIDS_FLAG); /* Decode until next video frame */
        }
      }
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Stop movies showing
  * @param  havi: AVI handle
  * @retval AVI status
  */
int GUI_AVI_Stop(GUI_AVI_HANDLE havi)
{
  AVI_CONTEXT  * pavi;
  GUI_AVI_FUNC * pfNotify = NULL;
  int r = 1;
  U32 CurrentImage;

  if (havi)
  {
    GUI_LOCK();
    r = 1;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      pfNotify = pavi->pfNotify;
      CurrentImage = pavi->CurrentImage;
      if(pavi->pfJpegDeInit)
      {
        pavi->pfJpegDeInit();
      }
      _Stopavi(pavi);
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  
  if ( (r == 0) && pfNotify )
  {
    pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_STOP, CurrentImage);
  }

  return r;
}

/**
  * @brief  Creates a movie handle
  * @param  pfNotify: pointer to notification callback function
  * @retval AVI status
  */
GUI_AVI_HANDLE GUI_AVI_Create(GUI_AVI_FUNC * pfNotify)
{
  return GUI_AVI_CreateEx(_GetData, pfNotify, _SeekFile);
}

/**
  * @brief  Starts playing a movie from the current frame
  * @param  havi: AVI handle
  * @retval AVI status
  */
int GUI_AVI_Play(GUI_AVI_HANDLE havi)
{
  AVI_CONTEXT * pavi;
  GUI_AVI_FUNC * pfNotify = NULL;
  int r = 1;
  U32 CurrentImage;

  if (havi)
  {
    GUI_LOCK();
    r = 1;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      pfNotify = pavi->pfNotify;
      CurrentImage = pavi->CurrentImage;
      /* Initialize timer */
      if (pavi->hTimer  ==  0)
      {
        pavi->hTimer = GUI_TIMER_Create(_OnTimer, 0, havi, 0);
        if (pavi->hTimer)
        {
          r = 0;
        }
      }
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }

  if( (r == 0) && pfNotify )
  {
    pfNotify(havi, GUI_AVI_NOTIFICATION_START, CurrentImage);
  }

  return r;
}

/**
  * @brief  Starts playing a movie starting at the first frame.
  * @param  havi: AVI handle
  * @param  xPos: X-position in screen coordinates to be used
  * @param  yPos: Y-position in screen coordinates to be used
  * @param  DoLoop: - 1 for endless loop, 0 for single loop
  * @retval AVI status
  */
int GUI_AVI_Show(GUI_AVI_HANDLE havi, int xPos, int yPos, int DoLoop)
{
  AVI_CONTEXT * pavi;
  int r = 1;

  if (havi)
  {
    GUI_LOCK();
    r = 1;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      /* Save parameters */
      pavi->xPos   = xPos;
      pavi->yPos   = yPos;
      pavi->DoLoop = DoLoop ? 1 : 0;
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }

  /* Start playing */
  r = GUI_AVI_Play(havi);

  return r;
}

/**
  * @brief  Selects AVI decoding mode.
  * @param  havi: AVI handle
  * @param  Mode: decoding mode
  * @retval AVI status
  */
int GUI_AVI_SelectMode (GUI_AVI_HANDLE havi, int Mode)
{
  AVI_CONTEXT * pavi;
  int r = 1;
  if (havi)
  {
    GUI_LOCK();
    r = 1;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      if(Mode == 0)
      {
        pavi->pfDraw = (U32(*)(const void * , U32 ,U32, U32))GUI_JPEG_Draw;
      }
      else
      {
        pavi->pfDraw = pavi->pfJpegDraw;
      }
      r = 0;
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  register HW JPEG/Audio decoder functions.
  * @param  havi: AVI handle
  * @retval AVI status
  */
void GUI_AVI_SetDevFunc (GUI_AVI_HANDLE havi, int IdFunc, void (* pDriverFunc)(void))
{
  AVI_CONTEXT * pavi;

  if (havi)
  {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      switch (IdFunc)
      {
        case GUI_AVI_JPEG_INIT:
          pavi->pfJpegInit = pDriverFunc;
          break;
        case GUI_AVI_JPEG_DEINIT:
          pavi->pfJpegDeInit = pDriverFunc;
          break;
        case GUI_AVI_JPEG_DRAW:
          pavi->pfJpegDraw = (U32(*)(const void * , U32 ,U32, U32))pDriverFunc;
          if(pavi->pfDraw == NULL)
            pavi->pfDraw = pavi->pfJpegDraw;
          break;
        default:
          break;
      }
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
}

/**
  * @brief  Add Video/Audio decoder Buffer.
  * @param  havi: AVI handle
  * @retval AVI status
  */
void GUI_AVI_SetBuffers (GUI_AVI_HANDLE havi, U8 *pVidBuff, U32 VidBuffSize, U8 *pAudBuff, U16 AudBuffSize)
{
  AVI_CONTEXT * pavi;

  if (havi)
  {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      pavi->pVideoBuffer = pVidBuff;
      pavi->VideoBufferSize = VidBuffSize;
      if((pAudBuff != 0) && (AudBuffSize > 0))
      {
        pavi->pAudioBuffer = pAudBuff;
        pavi->AudioBufferSize = AudBuffSize;
      }
      else
      {
        pavi->pAudioBuffer = AVI_AudioChunk;
        pavi->AudioBufferSize = sizeof(AVI_AudioChunk);
      }
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
}

/**
  * @brief  Removes a movie data structure and associated elements like timers
  *         from memory.
  * @param  havi: AVI handle
  * @retval AVI status
  */
int GUI_AVI_Delete(GUI_AVI_HANDLE havi)
{
  AVI_CONTEXT  * pavi;
  GUI_AVI_FUNC * pfNotify = NULL;
  int r = 1;

  if (havi)
  {
    GUI_LOCK();
    r = 1;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      if(pavi->pfNotify)
      {
        pfNotify = pavi->pfNotify;
        pavi = GUI_UNLOCK_H(pavi);
        pfNotify(havi, GUI_AVI_NOTIFICATION_DELETE, 0);
        pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
      }
      if (pavi->hTimer)
      {
        GUI_TIMER_Delete(pavi->hTimer);
        pavi->hTimer = 0;
      }
      if(pavi->pfJpegDeInit)
      {
        pavi->pfJpegDeInit();
      }
      r = 0;
    }
    GUI_UNLOCK_H(pavi);
    GUI_ALLOC_Free(havi);
    havi = 0;
    GUI_UNLOCK();
  }

  return r;
}

/**
  * @brief  Stops playing. Can be restarted with GUI_AVI_Play()
  * @param  havi: AVI handle
  * @retval AVI status
  */
int GUI_AVI_Pause(GUI_AVI_HANDLE havi)
{
  int r = 1;
  AVI_CONTEXT * pavi;

  if (havi)
  {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    r = _Stopavi(pavi);
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }

  return r;
}

/**
  * @brief  Jumps to the given frame index
  * @param  havi: AVI handle
  * @param  Frame: frame index
  * @retval AVI status
  */
int GUI_AVI_GotoFrame(GUI_AVI_HANDLE havi, U32 Frame)
{
  AVI_CONTEXT * pavi;
  GUI_AVI_FUNC * pfNotify = NULL;
  int r = 1, pos;
  U16 offset = 0;
  const U8  *pData;

  if (havi)
  {
    GUI_LOCK();
    r = 1;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      pos = Frame * (pavi->FileSize/ pavi->aviInfo.TotalFrame);

      pavi->CurrentImage = Frame;

      if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->VideoBufferSize,  pos) ==  pavi->VideoBufferSize)
      {
        if(pos  ==  0)
        {
          offset = _AVI_SearchID((U8 *)pavi->pVideoBuffer, pavi->VideoBufferSize, (U8 *)"movi");
        }

        offset += _AVI_SearchID((U8 *)pavi->pVideoBuffer + offset,pavi->VideoBufferSize, pavi->aviInfo.VideoFLAG);

        _AVI_GetStreamInfo(pavi,(U8 *)pavi->pVideoBuffer + offset);

        if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
        {
          if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8,  pos +offset + 8)  ==  (pavi->aviInfo.StreamSize + 8))
          {
            _AVI_GetStreamInfo(pavi, (U8 *)pavi->pVideoBuffer + pavi->aviInfo.StreamSize);
          }
        }

        if(pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
        {
          do
          {
            /* Skip Current Audio chunk data */
            if(pavi->pfSeek(pavi->pParam, pavi->aviInfo.StreamSize, 0xFFFFFFFF)  ==  0)
            {
              /* Read next Chunk data */
              pData = &pavi->pAudioBuffer[0];
              pavi->pfGetData(pavi->pParam, &pData, 8, 0xFFFFFFFF);

              /* Get Next buffer info */
              _AVI_GetStreamInfo(pavi, (U8 *)pData);
            }
          } while (pavi->aviInfo.StreamID !=  AVI_VIDS_FLAG); /* Decode until next video frame */
        }

        if (pavi->pfNotify)
        {
          pfNotify = pavi->pfNotify;
          pavi = GUI_UNLOCK_H(pavi);
          pfNotify(havi, GUI_AVI_NOTIFICATION_PREDRAW, Frame);
          pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
        }

        if(pavi->pfDraw)
        {
          /* Handle Video frame */
          WM_Deactivate();
          r = pavi->pfDraw(pavi->pVideoBuffer, pavi->VideoBufferSize, pavi->xPos, pavi->yPos);
          WM_Activate();
        }

        if (pavi->pfNotify)
        {
          pfNotify = pavi->pfNotify;
          pavi = GUI_UNLOCK_H(pavi);
          pfNotify(havi, GUI_AVI_NOTIFICATION_POSTDRAW, Frame);
          pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
        }
      }
    }
    pavi = GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Get frame index
  * @param  havi: AVI handle
  * @retval Returns the current frame index.
  */
U32 GUI_AVI_GetFrameIndex(GUI_AVI_HANDLE havi)
{
  AVI_CONTEXT * pavi;
  U32 r = 0xFFFFFFFF;

  if (havi)
  {
    GUI_LOCK();
    r = 0xFFFFFFFF;
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
    {
      r = pavi->CurrentImage;
    }
    GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Fills a GUI_AVI_INFO structure of a movie file not located in
  *         addressable memory area.
  * @param  havi: AVI handle
  * @param  pInfo: pointer to the file information
  * @retval Returns the current frame index.
  */
int GUI_AVI_GetInfo(GUI_AVI_HANDLE havi, GUI_AVI_INFO * pInfo)
{
  AVI_CONTEXT * pavi;
  U32 r = 0xFFFFFFFF;

  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);
  {
    pInfo->msPerFrame = pavi->aviInfo.SecPerFrame/ 1000;
    pInfo->NumFrames  = pavi->aviInfo.TotalFrame;
    pInfo->xSize      = pavi->aviInfo.Width;
    pInfo->ySize      = pavi->aviInfo.Height;
    pInfo->AudioType  = pavi->aviInfo.AudioType;
    pInfo->SampleRate = pavi->aviInfo.SampleRate;

    r = 0;
  }
  GUI_UNLOCK_H(pavi);
  return r;
}

