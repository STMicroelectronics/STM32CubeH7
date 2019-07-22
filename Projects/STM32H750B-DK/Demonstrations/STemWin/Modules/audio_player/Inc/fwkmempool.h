/**
******************************************************************************
* @file    audio_player/Inc/fwkmempool.h
* @author  MCD Application Team
* @brief   This file includes MP3 processing layer headers
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
#ifndef __FWK_MEMPOOL_H
#define __FWK_MEMPOOL_H

/** @addtogroup AUDIO_FRAMEWORK
* @{
*/

/** @defgroup FWK_MEMPOOL
* @brief Audio Memory Pool Manager
* @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "player.h"
#include "string.h"

/*--------------------------------------------------*/
/* Some structures definitions: from audiofwk_types.h */

typedef struct
{
  uint32_t            buffSze;
  uint32_t            readPtr;        /*index in buffer: 0...buffSze*/
  uint32_t            writePtr;       /*index in buffer: 0...buffSze*/
  volatile uint32_t   lock;           /*memory pool access locking*/
  volatile uint32_t   used;           /*pool is used by the application*/
  uint8_t            *pBuff;          /*buffer pointer*/
} FWK_MEMPOOL_t;

typedef struct
{
  FWK_MEMPOOL_t* pMemPool; /*used for MemPool Buffer management*/
  void* pData0;
  void* pData1;
} AUDIO_HANDLE_t;

/*--------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Error messages */
#define FWK_MEMPOOL_ERROR_NONE                  (0)
#define FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED    (-1)

/* Error messages */
#define FWK_MEMPOOL_STATUS_EMPTY                (0)
#define FWK_MEMPOOL_STATUS_FULL                 (1)
#define FWK_MEMPOOL_STATUS_USED                 (2)
#define FWK_MEMPOOL_STATUS_NOT_USED             (3)

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t FWK_MEMPOOL_Init(FWK_MEMPOOL_t *pMemPool, uint32_t BufferSze);
int32_t FWK_MEMPOOL_DeInit(FWK_MEMPOOL_t *pMemPool);
int32_t FWK_MEMPOOL_Write(FWK_MEMPOOL_t *pMemPool, uint8_t *pData, uint32_t DataSze);
int32_t FWK_MEMPOOL_Read(FWK_MEMPOOL_t *pMemPool, uint8_t *pData, uint32_t DataSze);
int32_t FWK_MEMPOOL_GetState(FWK_MEMPOOL_t *pMemPool);
int32_t FWK_MEMPOOL_ReadPtrInc(FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes);
int32_t FWK_MEMPOOL_ReadPtrDec(FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes);
int32_t FWK_MEMPOOL_WritePtrInc(FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes);
int32_t FWK_MEMPOOL_WritePtrDec(FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes);
int32_t FWK_MEMPOOL_GetAvailableSize(FWK_MEMPOOL_t *pMemPool, int32_t *pSize);
int32_t FWK_MEMPOOL_Free (FWK_MEMPOOL_t *pMemPool);

/* Function for Memory Pool access lock : to be specified by the user */
void FWK_MEMPOOL_Lock(FWK_MEMPOOL_t *pMemPool);
void FWK_MEMPOOL_Unlock(FWK_MEMPOOL_t *pMemPool);

#ifdef __cplusplus
}
#endif

/**
* @}
*/

/**
* @}
*/

#endif /*__FWK_MEMPOOL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
