/**
******************************************************************************
* @file    USB_Host/AUDIO_Standalone/Src/sd_diskio_dma.c
* @author  MCD Application Team
* @brief   SD Disk I/O driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"


/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private variables --------------------------------------------------------- */
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
__IO uint32_t writestatus, readstatus = 0, BlockSize=1;
/* Private function prototypes ----------------------------------------------- */
static DSTATUS SD_CheckStatus(BYTE lun);
DSTATUS SD_initialize(BYTE);
DSTATUS SD_status(BYTE);
DRESULT SD_read(BYTE, BYTE *, DWORD, UINT);
#if _USE_WRITE == 1
DRESULT SD_write(BYTE, const BYTE *, DWORD, UINT);
#endif                          /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE, BYTE, void *);
#endif                          /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef SD_Driver = {
  SD_initialize,
  SD_status,
  SD_read,
#if  _USE_WRITE == 1
  SD_write,
#endif                          /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  SD_ioctl,
#endif                          /* _USE_IOCTL == 1 */
};

/* Private functions --------------------------------------------------------- */
static DSTATUS SD_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;
  if (BSP_SD_GetCardState(0) == SD_TRANSFER_OK)
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
  return SD_CheckStatus(lun);
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
/**/

/**
* @brief  Reads Sector(s)
* @param  lun : not used
* @param  *buff: Data buffer to store read data
* @param  sector: Sector address (LBA)
* @param  count: Number of sectors to read (1..128)
* @retval DRESULT: Operation result
*/
DRESULT SD_read(BYTE lun, BYTE * buff, DWORD sector, UINT count)
{
  DRESULT res = RES_OK;
  uint32_t timeout = 100000;
  BSP_SD_CardInfo CardInfo;
  static int flag=0;

  /*Get BlockSize  just oneTime*/
  if ( flag==0)
  {
    /*GetSD Info*/
    BSP_SD_GetCardInfo(0,&CardInfo);

    /*Get Block Size*/
    BlockSize=CardInfo.LogBlockSize;
    flag =1;
  }

  /* Clean and Invalidate data cache */
  SCB_CleanInvalidateDCache_by_Addr((uint32_t *)(buff), (count * BlockSize) * 8);
  BSP_SD_ReadBlocks_DMA(0,(uint32_t*)buff,(uint32_t) (sector), count);

  /* Wait for Rx Transfer completion */
  while (readstatus == 0)
  {
  }

  readstatus = 0;
  while(BSP_SD_GetCardState(0)!= SD_TRANSFER_OK)
  {
    if (timeout-- == 0)
    {
      return RES_ERROR;
    }
  }
  res = RES_OK;

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
DRESULT SD_write(BYTE lun, const BYTE * buff, DWORD sector, UINT count)
{
  BSP_SD_CardInfo CardInfo;
  DRESULT res = RES_ERROR;
  static int flag=0;

  /*Get BlockSize  just oneTime*/
  if ( flag==0)
  {
    /*GetSD Info*/
    BSP_SD_GetCardInfo(0,&CardInfo);
    /*Get Block Size*/
    BlockSize=CardInfo.LogBlockSize;
    flag =1;
  }

  /*Clean DataCache */
  SCB_CleanDCache_by_Addr((uint32_t*)buff,(count * BlockSize) * 8);
  BSP_SD_WriteBlocks_DMA(0,(uint32_t *) buff, (uint32_t) (sector), count) ;

  /* Wait for Tx Transfer completion */
  while (writestatus == 0);
  writestatus = 0;

  /* wait until the Write operation is finished */
  while (BSP_SD_GetCardState(0) != SD_TRANSFER_OK)
  {
  }
  res = RES_OK;
  return res;
}
#endif                          /* _USE_WRITE == 1 */

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

  if (Stat & STA_NOINIT)
    return RES_NOTRDY;

  switch (cmd)
  {
    /* Make sure that no pending write process */
  case CTRL_SYNC:
    res = RES_OK;
    break;

    /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT:
    BSP_SD_GetCardInfo(0,&CardInfo);
    *(DWORD *) buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

    /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE:
    BSP_SD_GetCardInfo(0,&CardInfo);
    *(WORD *) buff = CardInfo.LogBlockSize;
    BlockSize=CardInfo.LogBlockSize;
    res = RES_OK;
    break;

    /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE:
    BSP_SD_GetCardInfo(0,&CardInfo);
    *(DWORD *) buff = CardInfo.LogBlockSize;
    BlockSize=CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif                          /* _USE_IOCTL == 1 */



/**
* @brief BSP Tx Transfer completed callbacks
* @param None
* @retval None
*/
void BSP_SD_WriteCpltCallback(uint32_t Instance)
{
  writestatus = 1;
}

/**
* @brief BSP Rx Transfer completed callbacks
* @param None
* @retval None
*/
void BSP_SD_ReadCpltCallback(uint32_t Instance)
{
  readstatus = 1;
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
