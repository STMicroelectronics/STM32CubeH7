/**
  ******************************************************************************
  * @file    GUI_AVI_2.h
  * @author  MCD Application Team
  * @brief   Header for GUI_AVI_2.c file
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

#ifndef  __GUI_AVI_2_H
#define  __GUI_AVI_2_H

#include "GUI.h"

#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/* Exported types ------------------------------------------------------------*/  
typedef enum 
{
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



#define AVI_RIFF_ID			0x46464952  
#define AVI_AVI_ID			0x20495641
#define AVI_LIST_ID			0x5453494C  
#define AVI_HDRL_ID			0x6C726468	
#define AVI_MOVI_ID			0x69766F6D 	
#define AVI_STRL_ID			0x6C727473	

#define AVI_AVIH_ID			0x68697661 	
#define AVI_STRH_ID			0x68727473	
#define AVI_STRF_ID			0x66727473 	
#define AVI_STRD_ID			0x64727473 	

#define AVI_VIDS_STREAM		        0x73646976		
#define AVI_AUDS_STREAM		        0x73647561 		
                                        
                                        
#define AVI_VIDS_FLAG		        0x6463			
#define AVI_AUDS_FLAG		        0x7762 


#define AVI_FORMAT_MJPG		        0x47504A4D 

typedef GUI_HMEM GUI_AVI_HANDLE;
typedef void GUI_AVI_FUNC(GUI_AVI_HANDLE havi, int Notification, U32 CurrentFrame, void* pUserData);

typedef void (*tHW_JPEG_Init)(bool spawnOutputThread);
typedef void (*tHW_JPEG_DeInit)(void);
typedef U32 (*tHW_JPEG_Draw)(const void * pFileData, U32 DataSize, U32 x0, U32 y0);

typedef struct 
{
  const    U8 * pFileData;
  I32      FileSize;
} GUI_AVI_PARAM;

#if defined (__GNUC__)
typedef struct  __attribute__((packed))
#else
typedef __packed struct
#endif
{	  
	U32 SecPerFrame;		
	U32 TotalFrame;			
	U32 Width;			
	U32 Height;			
	U32 SampleRate; 		
	U16 Channels;	  		
	U16 AudioBufSize;		
	U16 AudioType;	  		
	U16 StreamID;			
	U32 StreamSize;			
	U8* VideoFLAG;			
	U8* AudioFLAG;			
}AVI_INFO;

typedef struct  
{	
	U32 RiffID;			
	U32 FileSize;			
	U32 AviID;			
}AVI_HEADER;

typedef struct
{	
	U32 FrameID;			
	U32 FrameSize;			
}FRAME_HEADER;

typedef struct
{	
	U32 ListID;			
	U32 BlockSize;			
	U32 ListType;			
}LIST_HEADER;

typedef struct
{	
	U32 BlockID;		
	U32 BlockSize;		
	U32 SecPerFrame;	
	U32 MaxByteSec; 	
	U32 PaddingGranularity; 
	U32 Flags;		
	U32 TotalFrame;		
	U32 InitFrames;  	
	U32 Streams;		
	U32 RefBufSize;		
	U32 Width;		
	U32 Height;		
	U32 Reserved[4];	
}AVIH_HEADER;


typedef struct
{	
	U32 BlockID;			
	U32 BlockSize;			
	U32 StreamType;			
	U32 Handler;			
	U32 Flags;  			
	U16 Priority;			
	U16 Language;			
	U32 InitFrames;  		
	U32 Scale;			
	U32 Rate; 			
	U32 Start;			
	U32 Length;			
 	U32 RefBufSize;  		
        U32 Quality;			
	U32 SampleSize;			
	struct				
	{				
	   	U16 Left;
		U16 Top;
		U16 Right;
		U16 Bottom;
	}Frame;				
}STRH_HEADER;


typedef struct
{
	U32	 BmpSize;		
 	U32      Width;			
	U32      Height;			
	U16      Planes;			
	U16      BitCount;			
	U32      Compression;		
	U32      SizeImage;			
	U32      XpixPerMeter;		
	U32      YpixPerMeter;		
	U32      ClrUsed;			
	U32      ClrImportant;		
}BMP_HEADER;


typedef struct 
{
	U8  rgbBlue;			
	U8  rgbGreen; 			
	U8  rgbRed; 			
	U8  rgbReserved;		
}AVIRGBQUAD;

typedef struct 
{
	U32 BlockID;		
	U32 BlockSize;		
	BMP_HEADER bmiHeader;  	
	AVIRGBQUAD bmColors[1];	
}STRF_BMPHEADER;  


typedef struct 
{
	U32 BlockID;			
	U32 BlockSize;			
   	U16 FormatTag;			
	U16 Channels;	  		
	U32 SampleRate; 		
	U32 BaudRate;   		
	U16 BlockAlign; 		
	U16 Size;			
}STRF_WAVHEADER;


typedef struct {
  AVI_INFO            aviInfo;       
  U32                 CurrentImage;  
  U32                 TimeNextFrame; 
  int                 xPos, yPos;    
  int                 DoLoop;        
  GUI_GET_DATA_FUNC   * pfGetData;
  void                * pParam;
  const U8            * pVideoBuffer;
  U32                 VideoBufferSize;
  const U8            * pAudioBuffer;
  U32                 AudioBufferSize;    
  U32                 FileSize;        
  GUI_AVI_FUNC        * pfNotify;        
  GUI_TIMER_HANDLE    hTimer;        
  U8                  SkipMode;
  U8                  AudioSync;
  U32                 FramesPerSec;
  tHW_JPEG_Draw       pfDraw;
  
  tHW_JPEG_Init       pfJpegInit;
  tHW_JPEG_Draw       pfJpegDraw;
  tHW_JPEG_DeInit     pfJpegDeInit;

  void                * pUserData;

} AVI_CONTEXT;

#define	 MAKEWORD(ptr)	(U16)(((U16)*((U8*)(ptr))<<8)|(U16)*(U8*)((ptr)+1))
#define  MAKEDWORD(ptr)	(U32)(((U16)*(U8*)(ptr)|(((U16)*(U8*)(ptr+1))<<8)|\
						(((U16)*(U8*)(ptr+2))<<16)|(((U16)*(U8*)(ptr+3))<<24))) 





/*********************************************************************
*
*       AVI callbacks ID
*/
#define GUI_AVI_JPEG_INIT             0 
#define GUI_AVI_JPEG_DRAW             1 
#define GUI_AVI_JPEG_DEINIT           2 
/*********************************************************************
*
*       AVI file support
*/
#define GUI_AVI_NOTIFICATION_PREDRAW  0 /* Immediately before frame is drawn  */
#define GUI_AVI_NOTIFICATION_POSTDRAW 1 /* Immediately after a frame is drawn */
#define GUI_AVI_NOTIFICATION_START    2 /* Send when start playing a movie    */
#define GUI_AVI_NOTIFICATION_STOP     3 /* avi has stopped                    */
#define GUI_AVI_NOTIFICATION_DELETE   4 /* avi has been deleted               */
#define GUI_AVI_NOTIFICATION_EOF      5 /* end of avi file                    */



typedef struct {
  int xSize;         /* X-size of images                                      */
  int ySize;         /* Y-size of images                                      */
  int msPerFrame;    /* Default duration of 1 frame                           */
  U32 NumFrames;     /* Number of frames                                      */
} GUI_AVI_INFO;

GUI_AVI_HANDLE   GUI_AVI_CreateEx2    (GUI_GET_DATA_FUNC * pfGetData, GUI_AVI_FUNC * pfNotify, bool looping, void* userdata);
int              GUI_AVI_Start2       (GUI_AVI_HANDLE havi, void * pParam, U32 FileSize, bool spawnOutputThread);
void             GUI_AVI_SetDevFunc2  (GUI_AVI_HANDLE havi, int IdFunc, void *pDriverFunc);
int              GUI_AVI_DecodeFrame  (GUI_AVI_HANDLE havi, uint8_t *destination, uint16_t xsize, uint16_t ysize, uint16_t pitch);
int              GUI_AVI_Delete2      (GUI_AVI_HANDLE havi);
int              GUI_AVI_GetInfo2     (GUI_AVI_HANDLE havi, GUI_AVI_INFO * pInfo);
int              GUI_AVI_SelectMode2  (GUI_AVI_HANDLE havi, int Mode);
void             GUI_AVI_SetBuffers2  (GUI_AVI_HANDLE havi, U8 *pVidBuff, U32 VidBuffSize, U8 *pAudBuff, U16 AudBuffSize);

#if defined(__cplusplus)
}
#endif

#endif   /* ifdef __GUI_AVI_2_H */

