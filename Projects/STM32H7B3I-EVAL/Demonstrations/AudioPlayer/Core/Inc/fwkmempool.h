/**
******************************************************************************
* @file    audio_player/Inc/fwkmempool.h
* @author  MCD Application Team
* @brief   This file includes MP3 processing layer headers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* Protect Critical Section from IT */
#define BACKUP_PRIMASK()    uint32_t primask_bit= __get_PRIMASK()
#define DISABLE_IRQ()       __disable_irq()
#define RESTORE_PRIMASK()   __set_PRIMASK(primask_bit)

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
