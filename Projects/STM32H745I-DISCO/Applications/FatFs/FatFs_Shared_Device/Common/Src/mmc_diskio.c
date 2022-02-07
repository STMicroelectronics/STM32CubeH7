/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/Common/Src/mmc_diskio.c
  * @author  MCD Application Team
  * @brief   MMC Disk I/O FatFs driver
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
#include "mmc_diskio.h"
#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*
 * the following Timeout is useful to give the control back to the applications
 * in case of errors in either BSP_MMC_ReadCpltCallback() or BSP_MMC_WriteCpltCallback()
 * the value by default is as defined in the BSP platform driver otherwise 30 secs
 */

#define MMC_TIMEOUT MMC_DATATIMEOUT

#define MMC_DEFAULT_BLOCK_SIZE 512

/*
 * Depending on the usecase, the SD card initialization could be done at the
 * application level, if it is the case define the flag below to disable
 * the BSP_MMC_Init() call in the MMC_Initialize().
 */

//#define DISABLE_MMC_INIT

/*
 * when using cacheable memory region, it may be needed to maintain the cache
 * validity. Enable the define below to activate a cache maintenance at each
 * read and write operation.
 * Notice: This is applicable only for cortex M7 based platform.
 */

#define ENABLE_DMA_CACHE_MAINTENANCE  0

#define EMMC_HSEM_ID (1U)
#define LOCK_HSEM(__sem__)    do                                                \
                              {                                                 \
                                 while(HAL_HSEM_FastTake(__sem__) != HAL_OK) {} \
                              } while(0)

#define UNLOCK_HSEM(__sem__)  do                               \
                              {                                \
                                 HAL_HSEM_Release(__sem__, 0); \
                              } while(0)

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
static volatile  UINT  WriteStatus = 0, ReadStatus = 0;
/* Private function prototypes -----------------------------------------------*/
static DSTATUS MMC_CheckStatus(BYTE lun);
DSTATUS MMC_initialize (BYTE);
DSTATUS MMC_status (BYTE);
DRESULT MMC_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT MMC_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT MMC_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  MMC_Driver =
{
  MMC_initialize,
  MMC_status,
  MMC_read,
#if  _USE_WRITE == 1
  MMC_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  MMC_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/
static DSTATUS MMC_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;
  while (HAL_HSEM_FastTake(EMMC_HSEM_ID) != HAL_OK)
  {
  }

  if((BSP_MMC_GetCardState(0) == BSP_ERROR_NONE))
  {
    Stat &= ~STA_NOINIT;
  }

  HAL_HSEM_Release(EMMC_HSEM_ID, 0);

  return Stat;
}

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS MMC_initialize(BYTE lun)
{
#if !defined(DISABLE_MMC_INIT)

  while (HAL_HSEM_FastTake(EMMC_HSEM_ID) != HAL_OK)
  {
  }

  if(BSP_MMC_Init(0) == BSP_ERROR_NONE)
  {
    HAL_HSEM_Release(EMMC_HSEM_ID, 0);
    Stat = MMC_CheckStatus(lun);
  }
#else
  Stat = MMC_CheckStatus(lun);

#endif
  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS MMC_status(BYTE lun)
{
  return MMC_CheckStatus(lun);
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT MMC_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  ReadStatus = 0;
#if (ENABLE_MMC_DMA_CACHE_MAINTENANCE == 1)
  uint32_t alignedAddr;
#endif


  while (HAL_HSEM_FastTake(EMMC_HSEM_ID) != HAL_OK)
  {
  }


  if(BSP_MMC_ReadBlocks(0, (uint32_t*)buff,
                        (uint32_t) (sector),
                        count) == BSP_ERROR_NONE)
  {
    while(BSP_MMC_GetCardState(0) != BSP_ERROR_NONE)
    {
    }
      res = RES_OK;
#if (ENABLE_MMC_DMA_CACHE_MAINTENANCE == 1)
      /*
      the SCB_InvalidateDCache_by_Addr() requires a 32-Byte aligned address,
      adjust the address and the D-Cache size to invalidate accordingly.
      */
      alignedAddr = (uint32_t)buff & ~0x1F;
      SCB_InvalidateDCache_by_Addr((uint32_t*)alignedAddr, count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));
#endif
  }
  else
  {
    res = RES_NOTRDY;
  }

  HAL_HSEM_Release(EMMC_HSEM_ID, 0);
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
DRESULT MMC_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  WriteStatus = 0;
 /*
  * since the MPU is configured as write-through, see main.c file, there isn't any need
  * to maintain the cache as its content is always coherent with the memory.
  * If needed, check the file "Middlewares/Third_Party/FatFs/src/drivers/sd_diskio_dma_template.c"
  * to see how the cache is maintained during the write operations.
  */

  while (HAL_HSEM_FastTake(EMMC_HSEM_ID) != HAL_OK)
  {
  }
  if(BSP_MMC_WriteBlocks(0, (uint32_t*)buff,
                            (uint32_t)(sector),
                            count) == BSP_ERROR_NONE)
  {
    while(BSP_MMC_GetCardState(0) != BSP_ERROR_NONE)
    {
    }
    res = RES_OK;
  }
  HAL_HSEM_Release(EMMC_HSEM_ID, 0);
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
DRESULT MMC_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  BSP_MMC_CardInfo CardInfo;

  if (Stat & STA_NOINIT) return RES_NOTRDY;


  while ( HAL_HSEM_FastTake(EMMC_HSEM_ID) != HAL_OK)
  {
  }

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    BSP_MMC_GetCardInfo(0, &CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    BSP_MMC_GetCardInfo(0, &CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    BSP_MMC_GetCardInfo(0, &CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize / MMC_DEFAULT_BLOCK_SIZE;
	res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  while(BSP_MMC_GetCardState(0) != BSP_ERROR_NONE)
  {
  }

  HAL_HSEM_Release(EMMC_HSEM_ID, 0);
  return res;
}
#endif /* _USE_IOCTL == 1 */


