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

#define SD_DEFAULT_BLOCK_SIZE 512

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

#define ENABLE_SD_DMA_CACHE_MAINTENANCE  1

#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
#define CACHE_BYTES             32      /* Cache Line size in bytes */
#endif /* ENABLE_SD_DMA_CACHE_MAINTENANCE */

/* Private macros ------------------------------------------------------------*/
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
/* Address Align to Cache Line boundary address */
#define CACHE_ALIGN_UP_ADDR(_ADDR_)   (((_ADDR_)+(CACHE_BYTES-1)) &~(CACHE_BYTES-1))
#define CACHE_ALIGN_DOWN_ADDR(_ADDR_) ( (_ADDR_) &~(CACHE_BYTES-1))

/* Size Alignment to Cache Line Size */
#define CACHE_ALIGN_SIZE(_SIZE_) ((((_SIZE_) % CACHE_BYTES) == 0)              \
    ? (_SIZE_) : ((_SIZE_) + CACHE_BYTES - ((_SIZE_) % CACHE_BYTES)))
#endif /* ENABLE_SD_DMA_CACHE_MAINTENANCE */

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

#if _FS_REENTRANT
extern SD_HandleTypeDef uSdHandle;
#endif

/* Private functions ---------------------------------------------------------*/
static DSTATUS SD_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;

#if _FS_REENTRANT
  /* See Ticket 52883 - [FatFS] : Inconsistent Disk Status check in Multi-threading context */
  if(HAL_SD_GetError(&uSdHandle) == HAL_SD_ERROR_NONE)
  {
    Stat &= ~STA_NOINIT;
  }
#else /* _FS_REENTRANT == 0 */
  if(BSP_SD_GetCardState() == SD_TRANSFER_OK)
  {
    Stat &= ~STA_NOINIT;
  }
#endif /* _FS_REENTRANT == 1 */

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

    if ((Stat != STA_NOINIT) && (SDQueueID == NULL) )
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
    uint32_t timer;
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
    uint32_t *alignedAddr = 0;
    uint32_t  newSize     = 0;
    uint32_t  alignedSize = 0;

    /*
       The Cache Maintenance through memory address requires a 32-Byte aligned
       address, align the address and the size with respect to the D-Cache Line
       size to invalidate accordingly.
     */
    alignedAddr = (uint32_t *) CACHE_ALIGN_DOWN_ADDR(((uint32_t )buff));
    newSize = (count * BLOCKSIZE + ((uint32_t)buff - (uint32_t)alignedAddr));
    alignedSize = CACHE_ALIGN_SIZE((newSize));
#endif /* ENABLE_SD_DMA_CACHE_MAINTENANCE */

#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
    /*
       Force Cache Data Lines content write back to memory and Invalidate them.
       Someone (CPU) might be still accessing those cache lines so we need to
       flash/clean recent content to memory and purge/incalide cache lines before
       allowing Hardware to access RAM.
     */
    SCB_CleanDCache_by_Addr(alignedAddr, alignedSize);
#endif /* ENABLE_SD_DMA_CACHE_MAINTENANCE */

    if (!((uint32_t)buff & 0x3)) {
        /* Fast path cause destination buffer is correctly aligned */
        uint8_t status;

        timer = osKernelSysTick() + SD_TIMEOUT;

        /* block until SDIO IP is ready or a timeout occur */
        while (timer > osKernelSysTick()) {
            status = BSP_SD_GetCardState();
            if (status == SD_TRANSFER_OK)
                break;
        }

        if (status != SD_TRANSFER_OK)
            return RES_ERROR;

        //osThreadSuspendAll();
        uint8_t ret = BSP_SD_ReadBlocks_DMA((uint32_t*)buff, (uint32_t)(sector), count);
        //osThreadResumeAll();
        configASSERT(ret == BSP_ERROR_NONE);

        if (ret == BSP_ERROR_NONE) {
            /* wait for a message from the queue or a timeout */
            event = osMessageGet(SDQueueID, SD_TIMEOUT);
            if (event.status == osEventMessage) {
                if (event.value.v == READ_CPLT_MSG) {
                    res = RES_OK;
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
                    /*
                       Invalid Cache Data Lines content so next CPU will ftech data from RAM
                       not Cache Lines.
                     */
                    SCB_InvalidateDCache_by_Addr(alignedAddr, alignedSize);
#endif
                }
            }
        }
    } else {
        /* Slow path, fetch each sector apart and memcpy to destination buffer */
        uint8_t status, ret;
        int i;

        for (i = 0; i < count; i++) {
            timer = osKernelSysTick() + SD_TIMEOUT;

            /* block until SDIO IP is ready or a timeout occur */
            while (timer > osKernelSysTick()) {
                status = BSP_SD_GetCardState();
                if (status == SD_TRANSFER_OK)
                    break;
            }

            if (status != SD_TRANSFER_OK)
                return RES_ERROR;

            ret = BSP_SD_ReadBlocks_DMA((uint32_t*)buffer, (uint32_t)sector++, 1);
            configASSERT(ret == BSP_ERROR_NONE);

            if (ret == BSP_ERROR_NONE) {
                /* wait for a message from the queue or a timeout */
                event = osMessageGet(SDQueueID, SD_TIMEOUT);

                if (event.status == osEventMessage) {
                    if (event.value.v == READ_CPLT_MSG) {
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
                        /* the SCB_InvalidateDCache_by_Addr() requires a 32-Byte aligned address,
                           adjust the address and the D-Cache size to invalidate accordingly.
                        */
                        SCB_InvalidateDCache_by_Addr((uint32_t*)buffer, BLOCKSIZE);
#endif
                        memcpy(buff, buffer, BLOCKSIZE);
                        buff += BLOCKSIZE;
                    }
                }
            }
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
  uint32_t timer;
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
  uint32_t *alignedAddr = 0;
  uint32_t  newSize     = 0;
  uint32_t  alignedSize = 0;

  /*
     The Cache Maintenance through memory address requires a 32-Byte aligned
     address, align the address and the size with respect to the D-Cache Line
     size to invalidate accordingly.
   */
  alignedAddr = (uint32_t *) CACHE_ALIGN_DOWN_ADDR(((uint32_t )buff));
  newSize = (count * BLOCKSIZE + ((uint32_t)buff - (uint32_t)alignedAddr));
  alignedSize = CACHE_ALIGN_SIZE((newSize));
#endif /* ENABLE_SD_DMA_CACHE_MAINTENANCE */

#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
    /*
       Force Cache Data Lines content write back to memory and Invalidate them.
       Someone (CPU) might be still accessing those cache lines so we need to
       flash/clean recent content to memory and purge/incalide cache lines before
       allowing Hardware to access RAM.
     */
    SCB_CleanDCache_by_Addr(alignedAddr, alignedSize);
#endif /* ENABLE_SD_DMA_CACHE_MAINTENANCE */

  if(BSP_SD_WriteBlocks_DMA((uint32_t*)buff,
                           (uint32_t) (sector),
                           count) == BSP_ERROR_NONE)
  {
    /* Get the message from the queue */
    event = osMessageGet(SDQueueID, SD_TIMEOUT);

    if (event.status == osEventMessage)
    {
      if (event.value.v == WRITE_CPLT_MSG)
      {
        timer = osKernelSysTick() + SD_TIMEOUT;
        /* block until SDIO IP is ready or a timeout occur */
        while(timer > osKernelSysTick())
        {
          if (BSP_SD_GetCardState() == SD_TRANSFER_OK)
          {
            res = RES_OK;
            break;
          }
        }
      }
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
    BSP_SD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    BSP_SD_GetCardInfo(&CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    BSP_SD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
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
void BSP_SD_WriteCpltCallback(void)
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
void BSP_SD_ReadCpltCallback(void)
{
  /*
   * No need to add an "osKernelRunning()" check here, as the SD_initialize()
   * is always called before any SD_Read()/SD_Write() call
   */
   osMessagePut(SDQueueID, READ_CPLT_MSG, osWaitForever);
}

void BSP_SD_ErrorCallback(void)
{
  BSP_ErrorHandler();

  osMessagePut(SDQueueID, RW_ERROR_MSG, osWaitForever);
}

void BSP_SD_AbortCallback(void)
{
  BSP_ErrorHandler();

  osMessagePut(SDQueueID, RW_ABORT_MSG, osWaitForever);
}
