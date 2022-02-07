/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FatFs/FatFs_RAMDisk/FatFs/Target/sram_diskio.c
  * @author  MCD Application Team
  * @brief   SRAM Disk I/O driver
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
/* USER CODE END Header */ 

/* Includes ------------------------------------------------------------------*/
#include "ff_gen_drv.h"
#include "sram_diskio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Block Size in Bytes */
#define BLOCK_SIZE              512
#define SRAM_DISK_BASE_ADDR 	CD_AXISRAM1_BASE          
#define SRAM_DISK_SIZE 		    96U * 1024U            
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
DSTATUS SRAMDISK_initialize (BYTE);
DSTATUS SRAMDISK_status (BYTE);
DRESULT SRAMDISK_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT SRAMDISK_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT SRAMDISK_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

const Diskio_drvTypeDef SRAMDISK_Driver =
{
  SRAMDISK_initialize,
  SRAMDISK_status,
  SRAMDISK_read,
#if  _USE_WRITE == 1
  SRAMDISK_write,
#endif /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  SRAMDISK_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SRAMDISK_initialize(BYTE lun)
{
  /* USER CODE BEGIN SRAMDISK_initialize */
  Stat = STA_NOINIT;

  /* Configure the SRAM device */

   Stat &= ~STA_NOINIT;

  return Stat;
  /* USER CODE END SRAMDISK_initialize */
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SRAMDISK_status(BYTE lun)
{
  return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SRAMDISK_read(BYTE lun, BYTE * buff, DWORD sector, UINT count)
{
  uint32_t *pSrcBuffer = (uint32_t *)buff;
  uint32_t BufferSize = (BLOCK_SIZE * count) ;
  uint32_t *pSramAddress = (uint32_t *) (SRAM_DISK_BASE_ADDR + (sector * BLOCK_SIZE));
  
  memcpy(pSrcBuffer, pSramAddress , BufferSize);

  return RES_OK;
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
DRESULT SRAMDISK_write(BYTE lun, const BYTE * buff, DWORD sector, UINT count)
{
  uint32_t *pSrcBuffer = (uint32_t *)buff;
  uint32_t BufferSize = (BLOCK_SIZE * count) ;
  uint32_t *pSramAddress = (uint32_t *) (SRAM_DISK_BASE_ADDR + (sector * BLOCK_SIZE));

  memcpy(pSramAddress, pSrcBuffer, BufferSize); 

  return RES_OK;
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
DRESULT SRAMDISK_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    *(DWORD*)buff = SRAM_DISK_SIZE / BLOCK_SIZE;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = BLOCK_SIZE;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = 1;
	res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */

