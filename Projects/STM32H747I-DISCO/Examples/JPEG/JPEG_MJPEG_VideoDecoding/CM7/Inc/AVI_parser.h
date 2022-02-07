/**
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecoding/CM7/Inc/AVI_parser.h
  * @author  MCD Application Team
  * @brief   Header for AVI_parser.c file
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

#ifndef  __UTIL_LCD_AVI_H
#define  __UTIL_LCD_AVI_H

#include "main.h"
#include <string.h>
    
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define AVI_RIFF_ID      0x46464952  
#define AVI_AVI_ID      0x20495641
#define AVI_LIST_ID      0x5453494C  
#define AVI_HDRL_ID      0x6C726468  
#define AVI_MOVI_ID      0x69766F6D   
#define AVI_STRL_ID      0x6C727473  

#define AVI_AVIH_ID      0x68697661   
#define AVI_STRH_ID      0x68727473  
#define AVI_STRF_ID      0x66727473   
#define AVI_STRD_ID      0x64727473   

#define AVI_VIDS_STREAM            0x73646976    
#define AVI_AUDS_STREAM            0x73647561     
                                        
                                        
#define AVI_VIDS_FLAG            0x6463      
#define AVI_AUDS_FLAG            0x7762 


#define AVI_FORMAT_MJPG            0x47504A4D 


#define  AVI_END_FILE     0
#define  AVI_VIDEO_FRAME  1
#define  AVI_AUDIO_FRAME  2
#define  AVI_ERROR        3
  
/* Exported macro ------------------------------------------------------------*/
#define   AVI_MAKEWORD(ptr)  (uint16_t)(((uint16_t)*((uint8_t*)(ptr))<<8)|(uint16_t)*(uint8_t*)((ptr)+1))
#define  AVI_MAKEDWORD(ptr)  (uint32_t)(((uint16_t)*(uint8_t*)(ptr)|(((uint16_t)*(uint8_t*)(ptr+1))<<8)|\
            (((uint16_t)*(uint8_t*)(ptr+2))<<16)|(((uint16_t)*(uint8_t*)(ptr+3))<<24)))   
/* Exported types ------------------------------------------------------------*/  
typedef enum {
  AVI_OK   =0,      
  AVI_RIFF_ERR,      
  AVI_AVI_ERR,      
  AVI_LIST_ERR,      
  AVI_HDRL_ERR,      
  AVI_AVIH_ERR,      
  AVI_STRL_ERR,      
  AVI_STRH_ERR,      
  AVI_STRF_ERR,      
  AVI_MOVI_ERR,      
  AVI_FORMAT_ERR,      
  AVI_STREAM_ERR,      
}AVISTATUS;


typedef struct  
{  
  uint32_t RiffID;      
  uint32_t FileSize;      
  uint32_t AviID;      
}AVI_HEADER;

typedef struct
{  
  uint32_t FrameID;      
  uint32_t FrameSize;      
}FRAME_HEADER;

typedef struct
{  
  uint32_t ListID;      
  uint32_t BlockSize;      
  uint32_t ListType;      
}LIST_HEADER;

typedef struct
{  
  uint32_t BlockID;    
  uint32_t BlockSize;    
  uint32_t SecPerFrame;  
  uint32_t MaxByteSec;   
  uint32_t PaddingGranularity; 
  uint32_t Flags;    
  uint32_t TotalFrame;    
  uint32_t InitFrames;    
  uint32_t Streams;    
  uint32_t RefBufSize;    
  uint32_t Width;    
  uint32_t Height;    
  uint32_t Reserved[4];  
}AVIH_HEADER;


typedef struct
{  
  uint32_t BlockID;      
  uint32_t BlockSize;      
  uint32_t StreamType;      
  uint32_t Handler;      
  uint32_t Flags;        
  uint16_t Priority;      
  uint16_t Language;      
  uint32_t InitFrames;      
  uint32_t Scale;      
  uint32_t Rate;       
  uint32_t Start;      
  uint32_t Length;      
  uint32_t RefBufSize;      
  uint32_t Quality;      
  uint32_t SampleSize;      
  struct        
  {        
    uint16_t  Left;
    uint16_t  Top;
    uint16_t  Right;
    uint16_t  Bottom;
  }Frame;        
}STRH_HEADER;


typedef struct
{
  uint32_t     BmpSize;    
  uint32_t     Width;      
  uint32_t     Height;      
  uint16_t     Planes;      
  uint16_t     BitCount;      
  uint32_t     Compression;    
  uint32_t     SizeImage;      
  uint32_t     XpixPerMeter;    
  uint32_t     YpixPerMeter;    
  uint32_t     ClrUsed;      
  uint32_t     ClrImportant;    
}BMP_HEADER;


typedef struct 
{
  uint8_t  rgbBlue;      
  uint8_t  rgbGreen;       
  uint8_t  rgbRed;       
  uint8_t  rgbReserved;    
}AVIRGBQUAD;

typedef struct 
{
  uint32_t BlockID;    
  uint32_t BlockSize;    
  BMP_HEADER bmiHeader;    
  AVIRGBQUAD bmColors[1];  
}STRF_BMPHEADER;  


typedef struct 
{
  uint32_t BlockID;      
  uint32_t BlockSize;      
  uint16_t FormatTag;      
  uint16_t Channels;        
  uint32_t SampleRate;     
  uint32_t BaudRate;       
  uint16_t BlockAlign;     
  uint16_t Size;      
}STRF_WAVHEADER;


typedef struct
{    
  uint32_t  SecPerFrame;    
  uint32_t  TotalFrame;
  uint32_t  FrameRate;
  uint32_t  Width;      
  uint32_t  Height;      
  uint32_t  SampleRate;     
  uint16_t  Channels;        
  uint16_t  AudioBufSize;    
  uint16_t  AudioType;        
  uint16_t  StreamID;      
  uint32_t  StreamSize;      
  uint8_t*  VideoFLAG;      
  uint8_t*  AudioFLAG;      
}AVI_INFO;

typedef struct
{
  AVI_INFO  aviInfo;       
  uint32_t  CurrentImage;              
  uint32_t  FrameSize;
  uint8_t   *pVideoBuffer;  
  uint32_t  VideoBufferSize;      
  uint8_t   *pAudioBuffer;    
  uint32_t  AudioBufferSize;    
  uint32_t  FileSize;                 
  
} AVI_CONTEXT;




/* Exported functions ------------------------------------------------------- */  
uint32_t AVI_ParserInit(AVI_CONTEXT * pavi, FIL *file, uint8_t *pVideoBuffer, uint32_t VideoBufferSize, uint8_t *pAudioBuffer, uint32_t AudioBufferSize);
uint32_t AVI_GetFrame(AVI_CONTEXT * pavi, FIL *file);


#if defined(__cplusplus)
}
#endif

#endif   

