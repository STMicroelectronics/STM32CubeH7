/**
  ******************************************************************************
  * @file    sd_diskio.c
  * @author  MCD Application Team
  * @brief   SD Disk I/O template driver.This file needs to be renamed and copied
             into the application project alongside the respective header file
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
#include "ff_gen_drv.h"
#include "sd_diskio.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* use the default SD timeout as defined in the platform BSP driver*/
#if defined(SDMMC_DATATIMEOUT)
#define SD_TIMEOUT SDMMC_DATATIMEOUT
#elif defined(SD_DATATIMEOUT)
#define SD_TIMEOUT SD_DATATIMEOUT
#else
#define SD_TIMEOUT 30 * 1000
#endif

#define SD_DEFAULT_BLOCK_SIZE 512

/*
 * Depending on the usecase, the SD card initialization could be done at the
 * application level, if it is the case define the flag below to disable
 * the BSP_SD_Init() call in the SD_Initialize().
 */

#define DISABLE_SD_INIT

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
static __IO UBaseType_t uxSavedInterruptStatus;

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
/* Determine whether we are in thread mode or handler mode. */
static void EnterCriticalSection(void)
{
  uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
}

static void LeaveCriticalSection(void)
{
  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

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
#if !defined(DISABLE_SD_INIT)

  if(BSP_SD_Init() == MSD_OK)
  {
    Stat = SD_CheckStatus(lun);
  }

#else
  Stat = SD_CheckStatus(lun);
#endif
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
  DRESULT res = RES_OK;
  uint8_t sd_error = MSD_OK;
  
  EnterCriticalSection();
  sd_error = BSP_SD_ReadBlocks((uint32_t*)buff,
                               (uint32_t) (sector),
                               count, SD_TIMEOUT);
  if(sd_error == MSD_OK)
  {
    /* wait until the read operation is finished */
    while(BSP_SD_GetCardState()!= MSD_OK)
    {
      __NOP();
    }
    LeaveCriticalSection();
  }
  else
  {
    res = RES_ERROR;
    LeaveCriticalSection();
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
  DRESULT res = RES_OK;
  uint8_t sd_error = MSD_OK;

  EnterCriticalSection();
  sd_error = BSP_SD_WriteBlocks((uint32_t*)buff,
                                (uint32_t)(sector),
                                count, SD_TIMEOUT);
  if(sd_error == MSD_OK)
  {
	/* wait until the Write operation is finished */
    while(BSP_SD_GetCardState() != MSD_OK)
    {
      __NOP();
    }    
    LeaveCriticalSection();
  }
  else
  {
    res = RES_ERROR;
    LeaveCriticalSection();
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
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

