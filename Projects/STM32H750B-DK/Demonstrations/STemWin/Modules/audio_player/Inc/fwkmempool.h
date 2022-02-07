/**
******************************************************************************
* @file    audio_player/Inc/fwkmempool.h
* @author  MCD Application Team
* @brief   This file includes MP3 processing layer headers
******************************************************************************
* @attention
*
* Copyright (c) 2018 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
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

