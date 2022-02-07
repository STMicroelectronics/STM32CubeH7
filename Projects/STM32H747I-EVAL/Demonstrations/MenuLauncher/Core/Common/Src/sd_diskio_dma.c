/**
  ******************************************************************************
  * @file    sd_diskio_dma.c
  * @author  MCD Application Team
  * @brief   SD Disk I/O DMA with RTOS driver template. This file needs to be
             copied at user project alongside the respective header file.
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

/* Includes ------------------------------------------------------------------*/
#include "ff_gen_drv.h"
#include "sd_diskio.h"

#include <cmsis_os.h>

#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define QUEUE_SIZE         (uint32_t) 10
#define READ_CPLT_MSG      (uint32_t) 1
#define WRITE_CPLT_MSG     (uint32_t) 2
#define RW_ERROR_MSG       (uint32_t) 3
#define RW_ABORT_MSG       (uint32_t) 4

/*
 * the following Timeout is useful to give the control back to the applications
 * in case of errors in either BSP_SD_ReadCpltCallback() or BSP_SD_WriteCpltCallback()
 * the value by default is as defined in the BSP platform driver otherwise 30 secs
 *
 */

#define SD_TIMEOUT 30 * 1000

/*
 * Depending on the usecase, the SD card initialization could be done at the
 * application level, if it is the case define the flag below to disable
 * the BSP_SD_Init(0) call in the SD_Initialize().
 */

#define DISABLE_SD_INIT


/*
 * when using cacheable memory region, it may be needed to maintain the cache
 * validity. Enable the define below to activate a cache maintenance at each
 * read and write operation.
 * Notice: This is applicable only for cortex M7 based platform.
 */

#if defined(MPU_DISABLE_CACHE)
#define ENABLE_SD_DMA_CACHE_MAINTENANCE  0
#else
#define ENABLE_SD_DMA_CACHE_MAINTENANCE  1
#endif /* !MPU_DISABLE_CACHE */


/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
static osMessageQId SDQueueID;

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="sdio_heap"
#else
__attribute__((section(".sdio_heap"))) __attribute__ ((aligned (32)))
#endif
static uint8_t buffer[BLOCKSIZE];

/* Private function prototypes -----------------------------------------------*/
static DSTATUS SD_CheckStatus(BYTE lun);
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT SD_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read,
#if  _USE_WRITE == 1
  SD_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/
static DSTATUS SD_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;

  if(BSP_SD_GetCardState(0) == SD_TRANSFER_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_initialize(BYTE lun)
{
  Stat = STA_NOINIT;
  /*
   * check that the kernel has been started before continuing
   * as the osMessage API will fail otherwise
   */
  if(osKernelRunning())
  {
#if !defined(DISABLE_SD_INIT)

    if(BSP_SD_Init(0) == BSP_ERROR_NONE)
    {
      Stat = SD_CheckStatus(lun);
    }

#else
    Stat = SD_CheckStatus(lun);
#endif

    /*
     * if the SD is correctly initialized, create the operation queue
     */

    if (Stat != STA_NOINIT)
    {
      osMessageQDef(SD_Queue, QUEUE_SIZE, uint16_t);
      SDQueueID = osMessageCreate (osMessageQ(SD_Queue), NULL);
    }
  }

  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_status(BYTE lun)
{
  return SD_CheckStatus(lun);
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    osEvent event;

#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
    uint32_t alignedAddr;
#endif

    uint32_t timer = osKernelSysTick() + SD_TIMEOUT;

    /* first ensure the SDCard is ready for a new operation */
    while((BSP_SD_GetCardState(0) == SD_TRANSFER_BUSY))
    {
      if(timer < osKernelSysTick())
        return RES_NOTRDY;
    }

    if (!((uint32_t)buff & 0x3))
    {
        /* Fast path: the provided destination buffer is correctly aligned */
        uint8_t ret = BSP_SD_ReadBlocks_DMA(0,(uint32_t*)buff, (uint32_t)(sector), count);

        if (ret == BSP_ERROR_NONE) {
            /* wait for a message from the queue or a timeout */
            event = osMessageGet(SDQueueID, SD_TIMEOUT);
            if (event.status == osEventMessage) {
                if (event.value.v == READ_CPLT_MSG) {
                    res = RES_OK;
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
                    /*
                    * Invalidate the cache before reading into the buffer,  to get actual data
                    */
                    alignedAddr = (uint32_t)buff & ~0x1F;
                    SCB_InvalidateDCache_by_Addr((uint32_t*)alignedAddr, count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));
#endif
                }
            }
        }
    } else {
        /* Slow path: fetch each sector a part and memcpy to destination buffer */
        uint8_t ret;
        int i;

        for (i = 0; i < count; i++) {
            ret = BSP_SD_ReadBlocks_DMA(0,(uint32_t*)buffer, (uint32_t)sector++, 1);
            if (ret == BSP_ERROR_NONE) {
                /* wait for a message from the queue or a timeout */
                event = osMessageGet(SDQueueID, SD_TIMEOUT);

                if (event.status == osEventMessage) {
                    if (event.value.v == READ_CPLT_MSG) {
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
                        /*
                         * invalidate the scratch buffer before the next read to get the actual data instead of the cached one
                         */
                        SCB_InvalidateDCache_by_Addr((uint32_t*)buffer, BLOCKSIZE);
#endif
                        memcpy(buff, buffer, BLOCKSIZE);
                        buff += BLOCKSIZE;
                    }
                }
            } else
                break;
        }

        if ((i == count) && (ret == BSP_ERROR_NONE))
            res = RES_OK;
    }

    return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  osEvent event;
  DRESULT res = RES_ERROR;

  uint32_t timer = osKernelSysTick() + SD_TIMEOUT;

  /* first ensure the SDCard is ready for a new operation */
  while((BSP_SD_GetCardState(0) == SD_TRANSFER_BUSY))
  {
    if(timer < osKernelSysTick())
      return RES_NOTRDY;
  }

#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
  uint32_t alignedAddr;
#endif
  if (!((uint32_t)buff & 0x3))
  {
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
    /*
     * Invalidate the cache before writing into the buffer.
     * This is not needed if the memory region is configured as W/T.
     */
    alignedAddr = (uint32_t)buff & ~0x1F;
    SCB_InvalidateDCache_by_Addr((uint32_t*)alignedAddr, count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));
#endif

    if(BSP_SD_WriteBlocks_DMA(0,(uint32_t*)buff,
                              (uint32_t) (sector),
                              count) == BSP_ERROR_NONE)
    {
      /* Get the message from the queue */
      event = osMessageGet(SDQueueID, SD_TIMEOUT);

      if (event.status == osEventMessage)
      {
        if (event.value.v == WRITE_CPLT_MSG)
        {
          res = RES_OK;
        }
      }
    }
  }
  else
  {
    /* Slow path, fetch each sector a part and memcpy to destination buffer */
    int i;
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
    /*
     * invalidate the scratch buffer before the next write to get the actual data instead of the cached one
    */
    SCB_InvalidateDCache_by_Addr((uint32_t*)buffer, BLOCKSIZE);
#endif

    for (i = 0; i < count; i++) {
      uint8_t ret = BSP_SD_WriteBlocks_DMA(0,(uint32_t*)buffer, (uint32_t)sector++, 1);
      if (ret == BSP_ERROR_NONE) {
        /* wait for a message from the queue or a timeout */
        event = osMessageGet(SDQueueID, SD_TIMEOUT);

        if (event.status == osEventMessage) {
          if (event.value.v == WRITE_CPLT_MSG) {
            res = RES_OK;
            memcpy((void *)buff, (void *)buffer, BLOCKSIZE);
            buff += BLOCKSIZE;
          }
        }
      } else
        break;
    }
  }
  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  BSP_SD_CardInfo CardInfo;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    BSP_SD_GetCardInfo(0,&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    BSP_SD_GetCardInfo(0,&CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    BSP_SD_GetCardInfo(0,&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize / BLOCKSIZE;
	res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */



/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void BSP_SD_WriteCpltCallback(uint32_t Instance)
{
  /*
   * No need to add an "osKernelRunning()" check here, as the SD_initialize()
   * is always called before any SD_Read()/SD_Write() call
   */
   osMessagePut(SDQueueID, WRITE_CPLT_MSG, osWaitForever);
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void BSP_SD_ReadCpltCallback(uint32_t Instance)
{
  /*
   * No need to add an "osKernelRunning()" check here, as the SD_initialize()
   * is always called before any SD_Read()/SD_Write() call
   */
   osMessagePut(SDQueueID, READ_CPLT_MSG, osWaitForever);
}

/**
  * @brief BSP Error callbacks
  * @retval None
  */
void BSP_SD_ErrorCallback(void)
{
  /* Non Blocking Call to BSP_ErrorHandler() */
  BSP_ErrorHandler();

  /*
   * No need to add an "osKernelRunning()" check here, as the SD_initialize()
   * is always called before any SD_Read()/SD_Write() call
   */
   osMessagePut(SDQueueID, RW_ERROR_MSG, osWaitForever);
}

/**
  * @brief BSP Abort callbacks
  * @retval None
  */
void BSP_SD_AbortCallback(uint32_t Instance)
{
  /* Non Blocking Call to BSP_ErrorHandler() */
  BSP_ErrorHandler();

  /*
   * No need to add an "osKernelRunning()" check here, as the SD_initialize()
   * is always called before any SD_Read()/SD_Write() call
   */
   osMessagePut(SDQueueID, RW_ABORT_MSG, osWaitForever);
}

