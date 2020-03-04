/**
******************************************************************************
* @file    fwkmempool.c
* @author  MCD Application Team
* @brief   This file includes MP3 processing layer routines
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

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
* @brief  Memory pool initialization
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  uint32_t BufferSze : Memory pool size
* @retval error value
*/
int32_t FWK_MEMPOOL_Init(FWK_MEMPOOL_t *pMemPool, uint32_t BufferSze)
{
  int i;
  uint8_t *pDest;
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  pMemPool->readPtr = 0;
  pMemPool->writePtr = 0;
  pMemPool->buffSze = BufferSze;
  pDest = pMemPool->pBuff;
  for (i = 0; i < BufferSze; i++)
  {
    *pDest++ = 0;
  }

  /* Memory pool is empty */
  MemPoolFreeSpc = BufferSze;

  return error;
}


/**
* @brief  Memory pool de-initialization
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @retval error value
*/
int32_t FWK_MEMPOOL_DeInit(FWK_MEMPOOL_t *pMemPool)
{
  int i;
  uint8_t *pDest;
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  pMemPool->readPtr = 0;
  pMemPool->writePtr = 0;
  pDest = pMemPool->pBuff;
  for (i = 0; i < pMemPool->buffSze; i++)
  {
    *pDest++ = 0;
  }

  /* Memory pool is empty */
  MemPoolFreeSpc = pMemPool->buffSze;

  return error;
}


/**
* @brief  Routine to write (fill) in the memory pool
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  uint8_t *pData : source buffer
* @param  uint32_t DataSze : number of bytes to copy
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
  /* */
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

  BACKUP_PRIMASK();
  DISABLE_IRQ();
  MemPoolFreeSpc -= DataSze;
  RESTORE_PRIMASK();

  return error;
}


/**
* @brief  Routine to read (extract) from the memory pool
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  uint8_t *pData : destination buffer
* @param  uint32_t DataSze : number of bytes to copy
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

  BACKUP_PRIMASK();
  DISABLE_IRQ();
  MemPoolFreeSpc += DataSze;
  RESTORE_PRIMASK();

  return error;
}


/**
* @brief  Routine to get the memory pool status
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @retval error value
*/
int32_t FWK_MEMPOOL_GetState (FWK_MEMPOOL_t *pMemPool)
{
  int32_t retval = FWK_MEMPOOL_STATUS_EMPTY;

  BACKUP_PRIMASK();
  DISABLE_IRQ();
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
  RESTORE_PRIMASK();

  return retval;
}


/**
* @brief  Increments internal read pointer with NbBytes bytes
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  uint32_t NbBytes : number of bytes for read pointer increase
* @retval error value
*/
int32_t FWK_MEMPOOL_ReadPtrInc (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }
  pMemPool->readPtr += NbBytes;
  if (pMemPool->readPtr >= pMemPool->buffSze)
  {
    pMemPool->readPtr -= pMemPool->buffSze;
  }

  BACKUP_PRIMASK();
  DISABLE_IRQ();
  MemPoolFreeSpc += NbBytes;
  RESTORE_PRIMASK();

  return error;
}


/**
* @brief  Decrements internal read pointer with NbBytes bytes
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  uint32_t NbBytes : number of bytes for read pointer decrease
* @retval error value
*/
int32_t FWK_MEMPOOL_ReadPtrDec (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }

  if (pMemPool->readPtr >= NbBytes)
  {
    pMemPool->readPtr -= NbBytes;
  }
  else
  {
    pMemPool->readPtr += (pMemPool->buffSze - NbBytes);
  }

  BACKUP_PRIMASK();
  DISABLE_IRQ();
  MemPoolFreeSpc -= NbBytes;
  RESTORE_PRIMASK();

  return error;
}


/**
* @brief  Increments internal write pointer with NbBytes bytes
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  uint32_t NbBytes : number of bytes for write pointer increase
* @retval error value
*/
int32_t FWK_MEMPOOL_WritePtrInc (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }
  pMemPool->writePtr += NbBytes;
  if (pMemPool->writePtr >= pMemPool->buffSze)
  {
    pMemPool->writePtr -= pMemPool->buffSze;
  }

  BACKUP_PRIMASK();
  DISABLE_IRQ();
  MemPoolFreeSpc -= NbBytes;
  RESTORE_PRIMASK();

  return error;
}


/**
* @brief  Decrements internal write pointer with NbBytes bytes
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  uint32_t NbBytes : number of bytes for write pointer decrease
* @retval error value
*/
int32_t FWK_MEMPOOL_WritePtrDec (FWK_MEMPOOL_t  *pMemPool, uint32_t NbBytes)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  if(pMemPool == NULL)
  {
    error = FWK_MEMPOOL_ERROR_POOL_NOT_ALLOCATED;
  }
  if (pMemPool->writePtr >= NbBytes)
  {
    pMemPool->writePtr -= NbBytes;
  }
  else
  {
    pMemPool->writePtr += (pMemPool->buffSze - NbBytes);
  }

  BACKUP_PRIMASK();
  DISABLE_IRQ();
  MemPoolFreeSpc += NbBytes;
  RESTORE_PRIMASK();

  return error;
}


/**
* @brief  Extracts memory pool available size for write
* @param  FWK_MEMPOOL_t *pMemPool : Memory pool handler
* @param  int32_t *pSize : pointer on number of bytes available
* @retval error number
*/
int32_t FWK_MEMPOOL_GetAvailableSize (FWK_MEMPOOL_t *pMemPool, int32_t *pSize)
{
  int32_t error = FWK_MEMPOOL_ERROR_NONE;

  BACKUP_PRIMASK();
  DISABLE_IRQ();
  *pSize = MemPoolFreeSpc;
  RESTORE_PRIMASK();

  return error;
}

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
