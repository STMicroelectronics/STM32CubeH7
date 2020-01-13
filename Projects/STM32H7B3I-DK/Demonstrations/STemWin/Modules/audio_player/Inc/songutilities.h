/**
  ******************************************************************************
  * @file    audio_player/Inc/songutilities.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for memory management
  *          layer routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics International N.V.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SONG_UTILITIES_H
#define __SONG_UTILITIES_H

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <cmsis_os.h>

#include "ff.h"
#include "audio_conf.h" 

/** @addtogroup PLAYER_CODERS
  * @{
  */

/** @addtogroup SongUtilities
  * @{
  */

/** @defgroup SongUtilities_Exported_Types
  * @{
  */
typedef struct {
  char * Title;
  char * Artist;
  char * Album;
  char * Year;
  char * Genre;
  uint32_t SamplingRate;
}TAGS_TypeDef;

/**
  * @}
  */

/** @defgroup SongUtilities_Exported_Constants
  * @{
  */
#ifndef MAX_TAG_STRING_LENGTH
 #define MAX_TAG_STRING_LENGTH          40
#endif /* MAX_TAG_STRING_LENGTH */

/* #define USE_DYNAMIC_MEMORY */ /* uncomment this define to use dynamic memory allocation
                                 in SongUtilities_parseExtendedContentDescriptors(); this
                                 function extracts WMA song description from extended content descriptor object */

/**
  * @}
  */

/** @defgroup SongUtilities_Exported_Macros
  * @{
  */

/* Specify "mem_cpy" function to be used by mp3/wma tag parser */
#ifndef MEM_CPY
 #define MEM_CPY        mem_cpy
#endif /* MEM_CPY */

/* Specify "mem_set" function to be used by mp3/wma tag parser */
#ifndef MEM_SET
 #define MEM_SET          mem_set
#endif /* MEM_SET */

/* Specify "Seek File" function to be used by mp3/wma tag parser */
#ifndef FILE_SEEK
 #define FILE_SEEK        f_lseek
#endif /* FILE_SEEK */

/* Specify "Read File" function to be used by mp3/wma tag parser */
#ifndef FILE_READ
 #define FILE_READ         f_read
#endif /* FILE_READ */

/* Specify "malloc" function to be used by the Player to allocate memory region */
#ifndef MEM_MALLOC
#define MEM_MALLOC(x)       pvPortMalloc(x)
#endif /* MEM_MALLOC */

/* Specify "free" function to be used by Player to release, previously, allocated memory region */
#ifndef MEM_FREE
#define MEM_FREE(x)         vPortFree(x)
#endif /* MEM_FREE */

/**
  * @}
  */

/** @defgroup SongUtilities_Exported_Functions
  * @{
  */
#ifdef __MP3_DECODER__
 int8_t SongUtilities_MP3TagParser(uint8_t * buffer,  TAGS_TypeDef * pIDTAGS ,FIL * file);
#endif /* __MP3_DECODER__ */

#ifdef __WMA_DECODER__
 void SongUtilities_WMATagParser( TAGS_TypeDef *pIDTAG,FIL * file, uint8_t * pBuf );
#endif /* __WMA_DECODER__ */

void StrCtrlLength(void* Dst, void* Src, uint32_t MaxLen);

#endif /* __SONG_UTILITIES_H */

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
