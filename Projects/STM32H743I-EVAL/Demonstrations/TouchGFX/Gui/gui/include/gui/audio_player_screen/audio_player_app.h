/**
  ******************************************************************************
  * @file    audioplayer_app.h
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    13-November-2015
  * @brief   header of audio player application file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_PLAYER_APP_H
#define __AUDIO_PLAYER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#ifdef STM32H743xx
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval_audio.h"
#endif

#include "ff_gen_drv.h"
//#include "usbh_diskio.h"
#include "sd_diskio_dma.h"

/* Exported constants --------------------------------------------------------*/
#define MUTE_OFF                      0x00
#define MUTE_ON                       0x01
#define DEFAULT_AUDIO_VOLUME          50

#define AUDIO_OUT_BUFFER_SIZE         (8000)
   
#define AUDIOPLAYER_EQU1_BKP      RTC_BKP_DR0
#define AUDIOPLAYER_EQU2_BKP      RTC_BKP_DR1
#define AUDIOPLAYER_EQU3_BKP      RTC_BKP_DR2
#define AUDIOPLAYER_EQU4_BKP      RTC_BKP_DR3
#define AUDIOPLAYER_LOUD_BKP      RTC_BKP_DR4

#if (!defined ( __GNUC__ ))   
   
/* Equalizer Band Configuration */
#define SET_BAND_PRMS(band, _fltType, _centerHz, _widthHz) \
  (band)->fltType = _fltType; \
  (band)->centerHz = _centerHz; \
  (band)->widthHz = _widthHz;
#endif  

typedef enum
{
  AUDIOPLAYER_ERROR_NONE = 0, 
  AUDIOPLAYER_ERROR_IO,
  AUDIOPLAYER_ERROR_HW, 
  AUDIOPLAYER_ERROR_MEM, 
  AUDIOPLAYER_ERROR_FORMAT_NOTSUPPORTED,   
  
}AUDIOPLAYER_ErrorTypdef;

#define AUDIOPLAYER_StateTypdef  OUT_StateTypdef
/* Exported types ------------------------------------------------------------*/
typedef enum
{
    AUDIO_RECORDER_IDLE   =           0,
    AUDIO_RECORDER_SUSPENDED =        1,    
    AUDIO_RECORDER_RECORDING =        2,
    AUDIO_RECORDER_PLAYING   =        3,
    AUDIO_RECORDER_RECORD_PAUSE =     4,
    AUDIO_RECORDER_PLAY_PAUSE   =     5,
    AUDIO_RECORDER_RECORD_EOF =       6,
    AUDIO_RECORDER_PLAY_EOF =         7,
    AUDIO_RECORDER_ERROR        =     0xFF,      
  
}IN_StateTypdef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,  
  REC_BUFFER_OFFSET_HALF,  
  REC_BUFFER_OFFSET_FULL,   
  PLAY_BUFFER_OFFSET_HALF,  
  PLAY_BUFFER_OFFSET_FULL,     
}
BUFFER_StateTypeDef;


typedef enum
{
  AUDIOPLAYER_STOP = 0, 
  AUDIOPLAYER_START,   
  AUDIOPLAYER_PLAY,
  AUDIOPLAYER_PAUSE, 
  AUDIOPLAYER_EOF,   
  AUDIOPLAYER_ERROR,  
  
}OUT_StateTypdef;

typedef enum {
  BUFFER_EMPTY = 0,  
  BUFFER_FULL,     
}WR_BUFFER_StateTypeDef;


typedef struct 
{
  uint8_t         buff[AUDIO_OUT_BUFFER_SIZE];
  uint32_t        volume;
  uint32_t        mute;
  uint32_t        freq;   
  OUT_StateTypdef state;    
}AUDIO_ProcessTypdef ;


typedef struct 
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

}WAV_InfoTypedef ;   

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Play(uint32_t frequency);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Mute(uint8_t state);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_GetFileInfo(uint8_t* file, WAV_InfoTypedef* info);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SelectFile(uint8_t * file);
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_DeInit(void);
uint32_t                 AUDIOPLAYER_GetProgress (void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume);
uint32_t                 AUDIOPLAYER_GetVolume(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetPosition(uint32_t position);

AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyError(void);

#if (!defined ( __GNUC__ ))
void    AUDIOPLAYER_SetEqGain(uint8_t BandNum, int16_t NewGainValue);
void    AUDIOPLAYER_SetLoudnessGain(int16_t NewGainValue);
int16_t AUDIOPLAYER_GetLoudnessGain(void);
int16_t AUDIOPLAYER_GetEqGain(uint8_t BandNum);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__AUDIO_PLAYER_APP_H */

