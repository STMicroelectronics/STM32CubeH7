/**
  ******************************************************************************
  * @file    usbh_diskio.c
  * @author  MCD Application Team
  * @brief   USB Host Disk I/O driver (without internal DMA)
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
#include "usbh_diskio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USB_DEFAULT_BLOCK_SIZE 512U

#define ENABLE_USB_DMA_CACHE_MAINTENANCE  0

#if (ENABLE_USB_DMA_CACHE_MAINTENANCE == 1)
#define CACHE_BYTES             32      /* Cache Line size in bytes */
#endif /* ENABLE_USB_DMA_CACHE_MAINTENANCE */

/* Private macros ------------------------------------------------------------*/
#if (ENABLE_USB_DMA_CACHE_MAINTENANCE == 1)
/* Address Align to Cache Line boundary address */
#define CACHE_ALIGN_UP_ADDR(_ADDR_)   (((_ADDR_)+(CACHE_BYTES-1)) &~(CACHE_BYTES-1))
#define CACHE_ALIGN_DOWN_ADDR(_ADDR_) ( (_ADDR_) &~(CACHE_BYTES-1))

/* Size Alignment to Cache Line Size */
#define CACHE_ALIGN_SIZE(_SIZE_) ((((_SIZE_) % CACHE_BYTES) == 0)              \
    ? (_SIZE_) : ((_SIZE_) + CACHE_BYTES - ((_SIZE_) % CACHE_BYTES)))
#endif /* ENABLE_USB_DMA_CACHE_MAINTENANCE */

/* Private variables ---------------------------------------------------------*/
extern USBH_HandleTypeDef  hUSBHost;

/* Private function prototypes -----------------------------------------------*/
DSTATUS USBH_initialize (BYTE);
DSTATUS USBH_status (BYTE);
DRESULT USBH_read (BYTE, BYTE*, DWORD, UINT);

#if _USE_WRITE == 1
  DRESULT USBH_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
  DRESULT USBH_ioctl (BYTE, BYTE, void*);
#endif /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  USBH_Driver =
{
  USBH_initialize,
  USBH_status,
  USBH_read,
#if  _USE_WRITE == 1
  USBH_write,
#endif /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USBH_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : lun id
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_initialize(BYTE lun)
{
  /* CAUTION : USB Host library has to be initialized in the application */

  return RES_OK;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : lun id
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_status(BYTE lun)
{
  DRESULT res = RES_ERROR;

  if(USBH_MSC_UnitIsReady(&hUSBHost, lun))
  {
    res = RES_OK;
  }
  else
  {
    res = RES_ERROR;
  }

  return res;
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
#if (ENABLE_USB_DMA_CACHE_MAINTENANCE == 1)
  static uint32_t *alignedAddr = 0;
  static uint32_t  newSize     = 0;
  static uint32_t  alignedSize = 0;

  /*
     The Cache Maintenance through memory address requires a 32-Byte aligned
     address, align the address and the size with respect to the D-Cache Line
     size to invalidate accordingly.
   */
  alignedAddr = (uint32_t *) CACHE_ALIGN_DOWN_ADDR(((uint32_t )buff));
  newSize = (count * USB_DEFAULT_BLOCK_SIZE + ((uint32_t)buff - (uint32_t)alignedAddr));
  alignedSize = CACHE_ALIGN_SIZE((newSize));
#endif /* ENABLE_USB_DMA_CACHE_MAINTENANCE */

#if (ENABLE_USB_DMA_CACHE_MAINTENANCE == 1)
  /*
     Force Cache Data Lines content write back to memory and Invalidate them.
     Someone (CPU) might be still accessing those cache lines so we need to
     flash/clean recent content to memory and purge/incalide cache lines before
     allowing Hardware to access RAM.
   */
  SCB_CleanInvalidateDCache_by_Addr(alignedAddr, alignedSize);
#endif /* ENABLE_USB_DMA_CACHE_MAINTENANCE */

  if(USBH_MSC_Read(&hUSBHost, lun, sector, buff, count) == USBH_OK)
  {
#if (ENABLE_USB_DMA_CACHE_MAINTENANCE == 1)
    /*
       Invalid Cache Data Lines content so next CPU will ftech data from RAM
       not Cache Lines.
     */
    SCB_InvalidateDCache_by_Addr(alignedAddr, alignedSize);
#endif /* ENABLE_USB_DMA_CACHE_MAINTENANCE */
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSBHost, lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog ("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USBH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;

  if(USBH_MSC_Write(&hUSBHost, lun, sector, (BYTE *)buff, count) == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSBHost, lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = RES_WRPRT;
      break;

    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : lun id
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USBH_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC:
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    if(USBH_MSC_GetLUNInfo(&hUSBHost, lun, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_nbr;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    if(USBH_MSC_GetLUNInfo(&hUSBHost, lun, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_size;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

    /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :

    if(USBH_MSC_GetLUNInfo(&hUSBHost, lun, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_size / USB_DEFAULT_BLOCK_SIZE;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */


