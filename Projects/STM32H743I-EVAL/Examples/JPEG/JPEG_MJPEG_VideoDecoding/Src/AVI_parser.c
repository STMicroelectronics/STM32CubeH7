/**
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecoding/Src/AVI_parser.c 
  * @author  MCD Application Team  
  * @brief   This file provides routines for AVI parsing.
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

  /* Includes ------------------------------------------------------------------*/

#include "AVI_parser.h"


/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/      
uint8_t* const AVI_VIDS_FLAG_TBL[2]={(uint8_t*)"00dc",(uint8_t*)"01dc"};
uint8_t* const AVI_AUDS_FLAG_TBL[2]={(uint8_t*)"00wb",(uint8_t*)"01wb"};
/* Private functions ---------------------------------------------------------*/
AVISTATUS __AVI_Init( AVI_CONTEXT * pavi, uint8_t *buf, uint32_t size);
uint32_t  __AVI_SearchID(uint8_t* buf,  uint32_t size, uint8_t *id);
AVISTATUS __AVI_GetStreamInfo( AVI_CONTEXT * pavi, uint8_t* buf);


/**
  * @brief  Search AVI ID
  * @param  buf: 
  * @param  size:
  * @param  id:
  * @retval ID
  */
uint32_t __AVI_SearchID(uint8_t* buf,  uint32_t size, uint8_t *id)
{
 uint16_t i;
  size-=4;
  for(i=0;i<size;i++)
  {
    if(buf[i] == id[0])
      if(buf[i+1] == id[1])
        if(buf[i+2] == id[2])  
          if(buf[i+3] == id[3])
            return i;  
  }
  return 0;    
}

/**
  * @brief  Get AVI file information
  * @param  havi: AVI handle
  * @param  buf:   
  * @retval AVI status
  */
AVISTATUS __AVI_GetStreamInfo( AVI_CONTEXT * pavi, uint8_t* buf)
{
  
  pavi->aviInfo.StreamID = AVI_MAKEWORD (buf+2);    
  pavi->aviInfo.StreamSize = AVI_MAKEDWORD (buf+4);  
  
  if(pavi->aviInfo.StreamSize%2)
    pavi->aviInfo.StreamSize++;
  
  if((pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG) || (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG))
  {
    return AVI_OK;
  }
  
  return AVI_STREAM_ERR;  
}

/**
  * @brief  Initialize AVI
  * @param  havi: AVI context
  * @param  buf:
  * @param  size: AVI file size     
  * @retval AVI status
  */
AVISTATUS __AVI_Init( AVI_CONTEXT * pavi, uint8_t *buf, uint32_t size)     
{
  uint16_t offset;
  uint8_t *tbuf;
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
    
    pavi->aviInfo.VideoFLAG=(uint8_t*)AVI_VIDS_FLAG_TBL[0];      
    pavi->aviInfo.AudioFLAG=(uint8_t*)AVI_AUDS_FLAG_TBL[1];      
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
      
    }else
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
  }else if(strhheader->StreamType == AVI_AUDS_STREAM)         
  { 
    pavi->aviInfo.VideoFLAG=(uint8_t*)AVI_VIDS_FLAG_TBL[1];          
    pavi->aviInfo.AudioFLAG=(uint8_t*)AVI_AUDS_FLAG_TBL[0];          
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
  offset= __AVI_SearchID(tbuf,size,(uint8_t*)"movi");
  if(offset == 0)
  {
    return AVI_MOVI_ERR;
  }
  
  if(pavi->aviInfo.SampleRate)
  {
    tbuf+=offset;
    offset= __AVI_SearchID(tbuf,size,pavi->aviInfo.AudioFLAG);      
    if(offset == 0)
    {
      return AVI_STREAM_ERR;
    }
    
    tbuf+=offset+4;
    pavi->aviInfo.AudioBufSize=*((uint16_t*)tbuf);            
  }
  
  return res;
}


/**
  * @brief  parse AVI file header
  * @param  pavi: AVI context
  * @param  file:   AVI file
  * @param  pVideoBuffer   : video buffer
  * @param  VideoBufferSize: video buffer size
  * @param  pAudioBuffer   : audio buffer
  * @param  AudioBufferSize: audio buffer size
* @retval AVI status (0 : no Error, 1: Error occurred)
  */
uint32_t AVI_ParserInit(AVI_CONTEXT * pavi, FIL *file, uint8_t *pVideoBuffer, uint32_t VideoBufferSize, uint8_t *pAudioBuffer, uint32_t AudioBufferSize)
{
  uint32_t readSize= 0;
  
  pavi->pVideoBuffer = pVideoBuffer;
  pavi->VideoBufferSize = VideoBufferSize;

  pavi->pAudioBuffer = pAudioBuffer;
  pavi->AudioBufferSize = AudioBufferSize;
  
  f_lseek(file, 0 );
  
  f_read(file, pavi->pVideoBuffer, VideoBufferSize,(UINT*) &readSize );
  if(readSize != VideoBufferSize)
  {
    return 1;
  }
  else if(__AVI_Init(pavi, pavi->pVideoBuffer, VideoBufferSize) != AVI_OK)
  {
    return 1;    
  }

  pavi->FileSize= f_size(file);
  pavi->CurrentImage=0;  
  
  return  0;
}

/**
  * @brief  Get AVI frame
  * @param  pavi: AVI context
  * @param  file:   AVI file
  * @retval type of frame  (audio frame or video frame )
  */
uint32_t AVI_GetFrame(AVI_CONTEXT * pavi, FIL *file)  
{
  uint32_t  offset ;
  uint32_t readSize = 0;
  
  if(pavi->CurrentImage== 0 )
  {
    
    f_lseek(file, 0 ); /* Go to the file start */
    /* Read data*/
    f_read(file, pavi->pVideoBuffer, pavi->VideoBufferSize, (UINT*)&readSize );
    
    /* Check for "movi" tag */
    offset = __AVI_SearchID(pavi->pVideoBuffer,pavi->VideoBufferSize,(uint8_t*)"movi");
    /* Read first Frame info*/
    __AVI_GetStreamInfo(pavi, pavi->pVideoBuffer + offset +4);
    /* go to the first frame offset in the avi file*/
    f_lseek(file, offset + 12 );
    
  }

  /* Get the current frame size */
  pavi->FrameSize = pavi->aviInfo.StreamSize; 
  
  if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
  {
    /* the Frame is a Video Frame */
       
    /* Read The current frame + the header of the next frame (8 bytes) */
    f_read(file, pavi->pVideoBuffer, pavi->FrameSize + 8 , (UINT*)&readSize );
    
    /* Get the info of the next frame */
    __AVI_GetStreamInfo(pavi, pavi->pVideoBuffer + pavi->aviInfo.StreamSize );
    /* Return VIDEO frame */
    return AVI_VIDEO_FRAME;
  }
  if (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
  { /* the Frame is an Audio Frame */
    
    /* Read The current frame + the header of the next frame (8 bytes) */
    f_read(file, pavi->pAudioBuffer, pavi->FrameSize + 8, (UINT*)&readSize );

    /* Get the info of the next frame */
    __AVI_GetStreamInfo(pavi, pavi->pAudioBuffer + pavi->aviInfo.StreamSize );
    /* Return VIDEO frame */
    return AVI_AUDIO_FRAME;
    
  }
  
  return 0;
}





