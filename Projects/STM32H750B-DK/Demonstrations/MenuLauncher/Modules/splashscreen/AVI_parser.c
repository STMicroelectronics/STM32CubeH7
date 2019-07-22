/**
  ******************************************************************************
  * @file    JPEG/MJPEG_VideoDecodingFromQSPI/Src/AVI_parser.c 
  * @author  MCD Application Team
  * @version V0.5.0
  * @date    18-February-2017  
  * @brief   This file provides routines for AVI parsing.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

  /* Includes ------------------------------------------------------------------*/

#include "AVI_parser.h"


/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/			
static U8* const AVI_VIDS_FLAG_TBL[2]={(U8 *)"00dc",(U8 *)"01dc"};
static U8* const AVI_AUDS_FLAG_TBL[2]={(U8 *)"00wb",(U8 *)"01wb"};

uint32_t FrameIndex=0;
 
/* Private functions ---------------------------------------------------------*/
AVISTATUS __AVI_Init( AVI_CONTEXT * pavi, uint8_t *buf, uint16_t size);
uint32_t  __AVI_SearchID(uint8_t* buf,  uint16_t size, uint8_t *id);
AVISTATUS __AVI_GetStreamInfo( AVI_CONTEXT * pavi, uint8_t* buf);


/**
  * @brief  Search AVI ID
  * @param  buf: 
  * @param  size:
  * @param  id:
  * @retval ID
  */
uint32_t __AVI_SearchID(uint8_t* buf,  uint16_t size, uint8_t *id)
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
AVISTATUS __AVI_Init( AVI_CONTEXT * pavi, uint8_t *buf, uint16_t size)		 
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
  pavi->FileSize = aviheader->FileSize;
  
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
  offset = __AVI_SearchID(tbuf,size,(uint8_t *)"movi");
  if(offset == 0)
  {
    return AVI_MOVI_ERR;
  }
  
  if(pavi->aviInfo.SampleRate)
  {
    tbuf+=offset;
    offset = __AVI_SearchID(tbuf,size,pavi->aviInfo.AudioFLAG);
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
  * @param  AVI_FileAddress:   AVI file address 
  * @retval AVI status (0 : no Error, 1: Error occured)
  */
uint32_t AVI_ParserInit(AVI_CONTEXT * pavi, uint32_t AVI_FileAddress)
{
  pavi->AVIFileAddress = AVI_FileAddress;

  pavi->pVideoBuffer = (uint8_t *)pavi->AVIFileAddress;
  pavi->pAudioBuffer = (uint8_t *)pavi->AVIFileAddress;

  if(__AVI_Init(pavi, pavi->pVideoBuffer, (uint16_t)MAX_CHUNK_SIZE) != AVI_OK)
  {
    return 1;    
  }

  pavi->CurrentImage=0;
  pavi->Offset = 0;  
  
  return 0;
}

/**
  * @brief  Get AVI frame
  * @param  pavi: AVI context
  * @param  file:   AVI file
  * @retval type of frame  (audio frame or video frame )
  */
uint32_t AVI_GetFrame(AVI_CONTEXT * pavi)  
{
  if(pavi->CurrentImage == 0)
  {
    pavi->pVideoBuffer = (uint8_t *)pavi->AVIFileAddress;
    
    /* Check for "movi" tag */
    pavi->Offset = __AVI_SearchID(pavi->pVideoBuffer,(uint16_t)MAX_CHUNK_SIZE,(uint8_t *)"movi");
    /* Get first Frame info*/
    __AVI_GetStreamInfo(pavi, pavi->pVideoBuffer + pavi->Offset + 4);
    /* go to the first frame offset in the avi file*/
    pavi->Offset += 12;  
  }

  /* Get the current frame size */
  pavi->FrameSize = pavi->aviInfo.StreamSize; 
  
  if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
  {
    /* the Frame is a Video Frame */
    
    /* Go to The current frame */
    pavi->pVideoBuffer = ((uint8_t *)pavi->AVIFileAddress) + pavi->Offset;  
    /* Increment the offset with the current frame size + the header of the next frame (8 bytes)*/  
    pavi->Offset += pavi->FrameSize + 8;
    
    /* Get the info of the next frame */
    __AVI_GetStreamInfo(pavi, pavi->pVideoBuffer + pavi->aviInfo.StreamSize );
    /* Return VIDEO frame */
    return AVI_VIDEO_FRAME;
  }
  if (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
  { /* the Frame is an Audio Frame */
    
    /* Go to The current frame */
    pavi->pAudioBuffer = ((uint8_t *)pavi->AVIFileAddress) + pavi->Offset; 
    /* Increment the offset with the current frame size + the header of the next frame (8 bytes)*/  
    pavi->Offset += pavi->FrameSize + 8;
    
    /* Get the info of the next frame */
    __AVI_GetStreamInfo(pavi, pavi->pAudioBuffer + pavi->aviInfo.StreamSize );
    /* Return VIDEO frame */
    return AVI_AUDIO_FRAME;
    
  }
  
  return AVI_END_FILE;
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
