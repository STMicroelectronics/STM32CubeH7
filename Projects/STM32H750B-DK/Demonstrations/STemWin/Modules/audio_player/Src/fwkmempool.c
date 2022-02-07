/**
******************************************************************************
* @file    audio_player/Src/fwkmempool.c 
* @author  MCD Application Team
* @brief   This file includes MP3 processing layer routines
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
/* Includes ------------------------------------------------------------------*/
#include "fwkmempool.h"

/** @addtogroup AUDIO_FRAMEWORK
* @{
*/

/** @defgroup FWK_MEMPOOL
* @brief Audio Memory Pool Manager
* @{
*/

/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static int32_t MemPoolFreeSpc;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/* Default Memory Pool Lock/Unlock functions */
__weak void FWK_MEMPOOL_Lock(FWK_MEMPOOL_t *pMemPool)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED((void)pMemPool);
}

__weak void FWK_MEMPOOL_Unlock(FWK_MEMPOOL_t *pMemPool)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED((void)pMemPool);
}

/**
* @brief  Memory pool initialization
* @param  pMemPool : Memory pool handler
* @param  BufferSze : Memory pool size
* @retval error value
*/
int32_t FWK_MEMPOOL_Init(FWK_MEMPOOL_t *pMemPool, uint32_t BufferSze)
{
  int i;
  uint8_t *pDest;
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  FWK_MEMPOOL_Lock(pMemPool);

  pMemPool->used     = 0;
  pMemPool->readPtr  = 0;
  pMemPool->writePtr = 0;
  pMemPool->buffSze  = BufferSze;
  pDest = pMemPool->pBuff;
  for (i = 0; i < BufferSze; i++)
  {
    *pDest++ = 0;
  }

  /* Memory pool is empty */
  MemPoolFreeSpc = BufferSze;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Memory pool de-initialization
* @param  pMemPool : Memory pool handler
* @retval error value
*/
int32_t FWK_MEMPOOL_DeInit(FWK_MEMPOOL_t *pMemPool)
{
  int i;
  uint8_t *pDest;
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  FWK_MEMPOOL_Lock(pMemPool);

  pMemPool->readPtr  = 0;
  pMemPool->writePtr = 0;
  pMemPool->used     = 0;
  pDest = pMemPool->pBuff;
  for (i = 0; i < pMemPool->buffSze; i++)
  {
    *pDest++ = 0;
  }

  /* Memory pool is no more used */
  pMemPool->buffSze = 0;
  MemPoolFreeSpc    = pMemPool->buffSze;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Routine to write (fill) in the memory pool
* @param  pMemPool : Memory pool handler
* @param  pData : source buffer
* @param  DataSze : number of bytes to copy
* @retval error value
*/
int32_t FWK_MEMPOOL_Write (FWK_MEMPOOL_t *pMemPool, uint8_t *pData, uint32_t DataSze)
{
  uint32_t tmp = 0;
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }

  FWK_MEMPOOL_Lock(pMemPool);

  if(pMemPool->writePtr + DataSze <= pMemPool->buffSze)
  {
    memcpy(pMemPool->pBuff + pMemPool->writePtr, pData, DataSze);
    pMemPool->writePtr += DataSze;
    if(pMemPool->writePtr == pMemPool->buffSze)
    {
      pMemPool->writePtr = 0;
    }
  }
  else
  {
    tmp = pMemPool->buffSze - pMemPool->writePtr;
    memcpy(pMemPool->pBuff + pMemPool->writePtr, pData, tmp);
    pMemPool->writePtr = 0;

    memcpy(pMemPool->pBuff, pData + tmp, DataSze - tmp);
    pMemPool->writePtr = DataSze - tmp;
  }

  pMemPool->used  = 1;
  MemPoolFreeSpc -= DataSze;
  
  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Routine to read (extract) from the memory pool
* @param  pMemPool : Memory pool handler
* @param  pData : destination buffer
* @param  DataSze : number of bytes to copy
* @retval error value
*/
int32_t FWK_MEMPOOL_Read (FWK_MEMPOOL_t *pMemPool, uint8_t *pData, uint32_t DataSze)
{
  uint32_t tmp = 0;
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }

  FWK_MEMPOOL_Lock(pMemPool);

  if(pMemPool->readPtr + DataSze <= pMemPool->buffSze)
  {
    memcpy(pData, pMemPool->pBuff + pMemPool->readPtr, DataSze);
    pMemPool->readPtr += DataSze;
    if(pMemPool->readPtr == pMemPool->buffSze)
    {
      pMemPool->readPtr = 0;
    }
  }
  else
  {
    tmp = pMemPool->buffSze - pMemPool->readPtr;
    memcpy(pData, pMemPool->pBuff + pMemPool->readPtr, tmp);
    pMemPool->readPtr = 0;

    memcpy(pData + tmp, pMemPool->pBuff, DataSze - tmp);
    pMemPool->readPtr = DataSze - tmp;
  }

  MemPoolFreeSpc += DataSze;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Routine to get the memory pool status
* @param  pMemPool : Memory pool handler
* @retval error value
*/
int32_t FWK_MEMPOOL_GetState (FWK_MEMPOOL_t *pMemPool)
{
  int32_t retval = FWK_MEMPOOL_STATUS_EMPTY;

  FWK_MEMPOOL_Lock(pMemPool);

  if(pMemPool->used)
  {
    if(MemPoolFreeSpc == pMemPool->buffSze)
    {
      /* Memory pool is empty */
      retval = FWK_MEMPOOL_STATUS_EMPTY;
    }
    else if(MemPoolFreeSpc == 0)
    {
      /* Memory pool is full */
      retval = FWK_MEMPOOL_STATUS_FULL;
    }
    else
    {
      /* Memory pool is neither full nor empty */
      retval = FWK_MEMPOOL_STATUS_USED;
    }
  }
  else
  {
    /* Check if all data were consumed in the pool */
    if((MemPoolFreeSpc > pMemPool->buffSze)
         && (pMemPool->readPtr >= pMemPool->writePtr))
    {
      /* It might happen that we continue reading while no more data are written!
       * This would be giving a MemPoolFreeSpc value greater than the MemPool
       * Buffer Size. This is why the check is for value equal or greater than
       * the MemPool buffer size here below.
       */
      retval = FWK_MEMPOOL_STATUS_NOT_USED;
    }
  }

  FWK_MEMPOOL_Unlock(pMemPool);

  return retval;
}

/**
* @brief  Routine to set the pool state to not used
* @param  pMemPool : Memory pool handler
* @retval error value
*/
int32_t FWK_MEMPOOL_Free (FWK_MEMPOOL_t *pMemPool)
{
  FWK_MEMPOOL_Lock(pMemPool);

  pMemPool->used = 0;

  FWK_MEMPOOL_Unlock(pMemPool);

  return 0;
}

/**
* @brief  Increments internal read pointer with NbBytes bytes
* @param  pMemPool : Memory pool handler
* @param  NbBytes : number of bytes for read pointer increase
* @retval error value
*/
int32_t FWK_MEMPOOL_ReadPtrInc (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }

  FWK_MEMPOOL_Lock(pMemPool);

  pMemPool->readPtr += NbBytes;
  if (pMemPool->readPtr >= pMemPool->buffSze)
  {
    pMemPool->readPtr -= pMemPool->buffSze;
  }

  MemPoolFreeSpc += NbBytes;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Decrements internal read pointer with NbBytes bytes
* @param  pMemPool : Memory pool handler
* @param  NbBytes : number of bytes for read pointer decrease
* @retval error value
*/
int32_t FWK_MEMPOOL_ReadPtrDec (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }

  FWK_MEMPOOL_Lock(pMemPool);

  if (pMemPool->readPtr >= NbBytes)
  {
    pMemPool->readPtr -= NbBytes;
  }
  else
  {
    pMemPool->readPtr += (pMemPool->buffSze - NbBytes);
  }

  MemPoolFreeSpc -= NbBytes;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Increments internal write pointer with NbBytes bytes
* @param  pMemPool : Memory pool handler
* @param  NbBytes : number of bytes for write pointer increase
* @retval error value
*/
int32_t FWK_MEMPOOL_WritePtrInc (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }

  FWK_MEMPOOL_Lock(pMemPool);

  pMemPool->writePtr += NbBytes;
  if (pMemPool->writePtr >= pMemPool->buffSze)
  {
    pMemPool->writePtr -= pMemPool->buffSze;
  }

  pMemPool->used  = 1;
  MemPoolFreeSpc -= NbBytes;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Decrements internal write pointer with NbBytes bytes
* @param  pMemPool : Memory pool handler
* @param  NbBytes : number of bytes for write pointer decrease
* @retval error value
*/
int32_t FWK_MEMPOOL_WritePtrDec (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }

  FWK_MEMPOOL_Lock(pMemPool);

  if (pMemPool->writePtr >= NbBytes)
  {
    pMemPool->writePtr -= NbBytes;
  }
  else
  {
    pMemPool->writePtr += (pMemPool->buffSze - NbBytes);
  }

  MemPoolFreeSpc += NbBytes;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}


/**
* @brief  Extracts memory pool available size for write
* @param  pMemPool : Memory pool handler
* @param  pSize : pointer on number of bytes available
* @retval error number
*/
int32_t FWK_MEMPOOL_GetAvailableSize (FWK_MEMPOOL_t *pMemPool, int32_t *pSize)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  FWK_MEMPOOL_Lock(pMemPool);

  *pSize = MemPoolFreeSpc;

  FWK_MEMPOOL_Unlock(pMemPool);

  return error;
}

/**
* @}
*/

/**
* @}
*/

