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
#include "GUI_AVI_2.h"

#if GUI_WINSUPPORT
  #include "WM.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/			
static U8* const AVI_VIDS_FLAG_TBL[2]={(U8 *)"00dc",(U8 *)"01dc"};
static U8* const AVI_AUDS_FLAG_TBL[2]={(U8 *)"00wb",(U8 *)"01wb"};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Search AVI ID
  * @param  None 
  * @retval ID
  */
static U16 _AVI_SearchID(U8* buf,U16 size,U8 *id)
{
  U16 i;
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
  * @param  havi: AVI handle
  * @param  buf:   
  * @retval AVI status
  */
static AVISTATUS _AVI_GetStreamInfo(AVI_CONTEXT* pavi, U8* buf)
{
  if (pavi) 
  {
      pavi->aviInfo.StreamID = MAKEWORD(buf + 2);
      pavi->aviInfo.StreamSize = MAKEDWORD(buf + 4);

      if(pavi->aviInfo.StreamSize%2)
      {
        pavi->aviInfo.StreamSize++;
      }
      
      if((pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG) || (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG))
      {
        return AVI_OK;
      }
  }

  return AVI_STREAM_ERR;
}

/**
  * @brief  Initialize AVI
  * @param  havi: AVI handle
  * @param  buf:
  * @param  size: AVI file size     
  * @retval AVI status
  */
static AVISTATUS _AVI_Init(AVI_CONTEXT* pavi, U8 *buf, U16 size)		 
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

  if (pavi)
  {
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

        /* Save frames rate in pavi->FramesPerSec */
        pavi->FramesPerSec = strhheader->Rate / strhheader->Scale;

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
  }

  return res;
}

/* shared resource across threads */
#if defined ( __ICCARM__ )
#pragma data_alignment=4
#pragma location="sdio_heap"
#elif defined(__CC_ARM)
__attribute__((section(".sdio_heap"), zero_init)) __attribute__ ((aligned (4)))
#elif defined(__GNUC__)
__attribute__((section(".sdio_heap"))) __attribute__ ((aligned (4)))
#endif
static U8 fourcc[8];

#define FOURCC(a, b, c, d) ((d << 24) | (c << 16) | (b << 8) | a)

static uint32_t fourcc_idx1 = FOURCC('i', 'd', 'x', '1');
static uint32_t fourcc_00dc = FOURCC('0' ,'0', 'd', 'c');

/**
  * @brief  Decode AVI frame
  * @param  havi: AVI handle    
  * @retval 
  */
static int _DecodeFrame(AVI_CONTEXT *pavi)
{
    int offset;
    const U8 *pData;
    int ret = 0;

    pavi->CurrentImage++;

    if (pavi->CurrentImage == pavi->aviInfo.TotalFrame)
    {
        if (pavi->DoLoop)
        {
            pavi->CurrentImage = 0;

            ret = pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->VideoBufferSize, 0);
            if (ret != pavi->VideoBufferSize)
                return ret;

            _AVI_Init(pavi, (U8 *)pavi->pVideoBuffer, pavi->VideoBufferSize);

            offset = _AVI_SearchID((U8 *)pavi->pVideoBuffer,pavi->VideoBufferSize,(U8 *)"movi");
            _AVI_GetStreamInfo(pavi, (U8 *)pavi->pVideoBuffer + offset + 4);

            ret = pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize, offset + 12);
            if (ret != pavi->aviInfo.StreamSize)
                return ret;
        }
        else
        {
            return -1;
        }
    }
    else /* Not the end of the file */
    {
        if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
        {
            U8* dest[1] = { fourcc };
            uint32_t fourcc_token;

            pavi->aviInfo.StreamSize = 0;

            /* Encoders (e.g. ffmpeg) might insert an empty 00dc token in the MJPEG/AVI bitstream */
            while (pavi->aviInfo.StreamSize == 0) {
                memset(fourcc, 0, sizeof(fourcc));

                ret = pavi->pfGetData(pavi->pParam, (unsigned char const**)dest, sizeof(fourcc), 0xFFFFFFFF);
                if (ret != sizeof(fourcc))
                    return ret; /* FS error */

                fourcc_token = *((uint32_t*)fourcc);

                if (fourcc_token == fourcc_idx1) {
                    pavi->CurrentImage = pavi->aviInfo.TotalFrame - 1;
                    return -1; /* End of stream since we reached idx1 table */
                }

                if (fourcc_token != fourcc_00dc)
                    return 1;

                _AVI_GetStreamInfo(pavi, (U8*)fourcc);
            }

            if (pavi->aviInfo.StreamSize > pavi->VideoBufferSize)
                return 1;

            ret = pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize, 0xFFFFFFFF);
            if (ret != pavi->aviInfo.StreamSize)
                return ret;
        }

        if (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
        {
            U32 AudioWdPtr = 0;
            /* Skip all audio data */
            do {
                if (pavi->aviInfo.StreamSize >= pavi->AudioBufferSize)
                    return 1;

                if(AudioWdPtr > (pavi->AudioBufferSize - pavi->aviInfo.StreamSize))
                    AudioWdPtr = 0;

                pData = &pavi->pAudioBuffer[AudioWdPtr];

                ret = pavi->pfGetData(pavi->pParam, &pData, pavi->aviInfo.StreamSize + 8, 0xFFFFFFFF);
                if(ret != pavi->aviInfo.StreamSize + 8)
                    return ret;

                AudioWdPtr += pavi->aviInfo.StreamSize;

                _AVI_GetStreamInfo(pavi, (U8*)&pData[pavi->aviInfo.StreamSize]);
            } while (pavi->aviInfo.StreamID == AVI_AUDS_FLAG);
        }
    }

    return 0;
}

extern uint32_t DecodeBufferAddress;

#define DECODE_CONVERT_OUTPUT_BUFF      0x11

#define FRAME_BUFFER_WIDTH              800
#define FRAME_BUFFER_HEIGHT             480
#define RGB565_BYTES_PER_PIXEL          (uint32_t) (2U)
#define RGB_VIDEO_BUFFER_SIZE           ((uint32_t)(FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * RGB565_BYTES_PER_PIXEL)) /* 750 Kb */
#define RGB_VIDEO_BUFFER_NBR            2

void DMA2D_CopyBuffer_YCbCr_To_RGB_2(uint32_t *pSrc, uint8_t *destination, uint16_t xsize, uint16_t ysize, uint16_t pitch);

int GUI_AVI_DecodeFrame(GUI_AVI_HANDLE havi, uint8_t *destination, uint16_t width, uint16_t height, uint16_t pitch)
{
    AVI_CONTEXT *pavi;
    int ret;

    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi);

    /* Fetch AVI data */
    ret = _DecodeFrame(pavi);

    if (!ret) {
        /* Actual H/W decoding to YCbCr buffers */
        if (pavi->pfDraw)
            ret = pavi->pfDraw(pavi->pVideoBuffer, pavi->aviInfo.StreamSize, 0, 0);

        if (!ret) /* HW decode success */
            DMA2D_CopyBuffer_YCbCr_To_RGB_2((uint32_t*)DecodeBufferAddress,
                                            destination, width, height, width);
    }

    GUI_UNLOCK_H(pavi);

    return ret;
}

/**
  * @brief  Creates a movie handle
  * @param  pfGetData: pointer to GetData function
  * @param  pfNotify:  pointer to Notification callback function  
  * @retval AVI status
  */
GUI_AVI_HANDLE GUI_AVI_CreateEx2(GUI_GET_DATA_FUNC * pfGetData, GUI_AVI_FUNC * pfNotify, bool looping, void* userdata)
{
  AVI_CONTEXT  * pavi;
  GUI_AVI_HANDLE havi;

  /* Allocate context for drawing operation */
  GUI_LOCK();
  havi = GUI_ALLOC_AllocZero(sizeof(AVI_CONTEXT));
  if (havi  ==  0) 
  {
    GUI_UNLOCK();
    return 0;
  }
  
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); 
  {
    /*  Save parameters */
    pavi->pfNotify  = pfNotify;
    pavi->pfGetData = pfGetData;
    pavi->pfDraw = (U32(*)(const void * , U32 ,U32, U32))GUI_JPEG_Draw;
    pavi->DoLoop = looping;
    pavi->pUserData = userdata;
  }
  GUI_UNLOCK_H(pavi);
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
int GUI_AVI_Start2(GUI_AVI_HANDLE havi, void * pParam, U32 FileSize, bool spawnOutputThread)
{
  AVI_CONTEXT  * pavi;
  U32 offset;
  int r = 1;
  const U8  *pData;
  
  if (havi) 
  {
    GUI_LOCK();
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
          pavi->pfJpegInit(spawnOutputThread);
        }

        /* Read first stream */
        if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
        {
          if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize, offset + 12)  ==  (pavi->aviInfo.StreamSize))
          {
          }
        }

        if(pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
        {
          U32 AudioWdPtr  = pavi->AudioBufferSize / 2;
          do
          {
            if(AudioWdPtr >= pavi->AudioBufferSize - pavi->aviInfo.StreamSize - 8)
            {
              AudioWdPtr = 0;
            } 
            pData = &pavi->pAudioBuffer[AudioWdPtr];
            if(pavi->pfGetData(pavi->pParam, &pData, pavi->aviInfo.StreamSize + 8, 0xFFFFFFFF)  ==  pavi->aviInfo.StreamSize + 8)
            {
              AudioWdPtr += pavi->aviInfo.StreamSize;
            }
            _AVI_GetStreamInfo(pavi, (U8 *)pData + pavi->aviInfo.StreamSize);
            
          } while (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG);
        }   
      }
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
void GUI_AVI_SetDevFunc2 (GUI_AVI_HANDLE havi, int IdFunc, void *pDriverFunc)
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
          pavi->pfJpegInit = (tHW_JPEG_Init)pDriverFunc;
          break;
        case GUI_AVI_JPEG_DEINIT:
          pavi->pfJpegDeInit = (tHW_JPEG_DeInit)pDriverFunc;
          break;
        case GUI_AVI_JPEG_DRAW:
          pavi->pfJpegDraw = (tHW_JPEG_Draw)pDriverFunc;
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
  * @brief  Selects AVI decoding mode.
  * @param  havi: AVI handle 
  * @param  Mode: decoding mode 
  * @retval AVI status
  */
int GUI_AVI_SelectMode2 (GUI_AVI_HANDLE havi, int Mode) 
{
  AVI_CONTEXT * pavi;
  int r = 1;
  if (havi) 
  {
    GUI_LOCK();
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
  * @brief  Add Video/Audio decoder Buffer.
  * @param  havi: AVI handle 
  * @retval AVI status
  */
void GUI_AVI_SetBuffers2 (GUI_AVI_HANDLE havi, U8 *pVidBuff, U32 VidBuffSize, U8 *pAudBuff, U16 AudBuffSize)
{
  AVI_CONTEXT * pavi;

  if (havi) 
  {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); 
    {
      pavi->pVideoBuffer = pVidBuff;
      pavi->VideoBufferSize = VidBuffSize;      
      pavi->pAudioBuffer = pAudBuff;
      pavi->AudioBufferSize = AudBuffSize;      
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
int GUI_AVI_Delete2(GUI_AVI_HANDLE havi) 
{
  AVI_CONTEXT  * pavi;
  GUI_AVI_FUNC * pfNotify;
  int r;

  r = 1;
  if (havi) 
  {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); 
    {
      pfNotify = pavi->pfNotify;
      if (pavi->hTimer) 
      {
        GUI_TIMER_Delete(pavi->hTimer);
        pavi->hTimer = 0;
      }
      if(pavi->pfJpegDeInit)
      {
        pavi->pfJpegDeInit();
      }
      if (pfNotify) 
      {
        pfNotify(havi, GUI_AVI_NOTIFICATION_DELETE, 0, pavi->pUserData);
      }   
      r = 0;
    } 
    GUI_UNLOCK_H(pavi);
    
    GUI_ALLOC_Free(havi);
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
int GUI_AVI_GetInfo2(GUI_AVI_HANDLE havi, GUI_AVI_INFO * pInfo) 
{  
  AVI_CONTEXT * pavi;
  
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); 
  {
    pInfo->msPerFrame = pavi->aviInfo.SecPerFrame/ 1000;
    pInfo->NumFrames  = pavi->aviInfo.TotalFrame;
    pInfo->xSize      = pavi->aviInfo.Width;
    pInfo->ySize      = pavi->aviInfo.Height;
  }
  GUI_UNLOCK_H(pavi);
  return 0;
}

