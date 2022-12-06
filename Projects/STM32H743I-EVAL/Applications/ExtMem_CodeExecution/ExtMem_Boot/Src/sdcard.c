/**
  ******************************************************************************
  * @file    sdcard.c
  * @author  MCD Application Team
  * @brief   This file includes the sdcard supported STM32H743I-EVAL evaluation
             board.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - ToBeUpdated :  SDCard  mounted on STM32H743I Evaluation board.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "memory.h"
#include "memory_msp.h"
#include "ff_gen_drv.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_sd.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* use the default SD timout as defined in the platform BSP driver*/
#if defined(SDMMC_DATATIMEOUT)
#define SD_TIMEOUT SDMMC_DATATIMEOUT
#elif defined(SD_DATATIMEOUT)
#define SD_TIMEOUT SD_DATATIMEOUT
#else
#define SD_TIMEOUT 30 * 1000
#endif


/* Private variables ---------------------------------------------------------*/
#if (BINARY_AREA == USE_SDCARD)

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
SD_HandleTypeDef uSdHandle;

/* Private function prototypes -----------------------------------------------*/
static DSTATUS SD_CheckStatus(BYTE lun);
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read,
#if  _USE_WRITE == 1
  NULL,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  NULL,
#endif /* _USE_IOCTL == 1 */
};


/**
  * @brief  Initialize the SDCARD through the FATFS, copy binary to its execution
  *         area and finally free no more needed resources.
  * @param  None
  * @retval None
  */
uint32_t SDCARD_Startup(void)
{   
  uint32_t bytesread;   /* File read counts */
  FATFS SDFatFs;        /* File system object for SD card logical drive */
  FIL BinaryFile;       /* File object */
  char SDPath[4];       /* SD card logical drive path */
  
  if(FATFS_LinkDriver(&SD_Driver, SDPath) != 0)
  {
    return MEMORY_ERROR;
  }

  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
  {
    return MEMORY_ERROR;
  }

  if(f_open(&BinaryFile, BINARY_FILENAME, FA_OPEN_EXISTING | FA_READ) != FR_OK)
  {
    return MEMORY_ERROR;
  }

  if(f_read(&BinaryFile, (void*)APPLICATION_ADDRESS, f_size(&BinaryFile), (UINT*)&bytesread) != FR_OK)
  {
    return MEMORY_ERROR;
  }
  
  if(f_size(&BinaryFile) != bytesread)
  {
    return MEMORY_ERROR;
  }
              
  f_close(&BinaryFile);
                
  FATFS_UnLinkDriver(SDPath);
  
  HAL_SD_DeInit(&uSdHandle);
  SDCARD_MspDeInit();

  return MEMORY_OK;
}

/* Private functions ---------------------------------------------------------*/
static DSTATUS SD_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;

  if(HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
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
  BSP_IO_Init_t            IO_Init;
  
  /* uSD device interface configuration */
  uSdHandle.Instance = SDMMC1;

  uSdHandle.Init.ClockDiv            = 2;
  uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
  
  /* Initialize all required GPIO PINs */
  SDCARD_MspInit();
  
  /* Initialize the BSP IO driver and configure the SD Detect and SD Transceiver pins */
  if (BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE) != BSP_ERROR_NONE)
  {
    return RES_ERROR;
  }
  else
  {
    IO_Init.Pin  = SD_DETECT_PIN;
    IO_Init.Mode = IO_MODE_INPUT;
    IO_Init.Pull = IO_PULLUP;
    if (BSP_IO_Init(0, &IO_Init) != BSP_ERROR_NONE)
    {
      return RES_ERROR;
    }

    IO_Init.Pin  = SD_LDO_SEL_PIN;
    IO_Init.Mode = IO_MODE_OUTPUT_PP;
    IO_Init.Pull = IO_PULLUP;
    if (BSP_IO_Init(0, &IO_Init) != BSP_ERROR_NONE)
    {
      return RES_ERROR;
    }
  }

  /* Read the status joystick pins   */
  if(((uint32_t)BSP_IO_ReadPin(0, SD_DETECT_PIN)&SD_DETECT_PIN) == SD_DETECT_PIN)
  {
    return RES_ERROR;
  }

  /* HAL SD initialization */
  if(HAL_SD_Init(&uSdHandle) != HAL_OK)
  {
    return RES_ERROR;
  }

  return RES_OK;
  
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

  if(HAL_SD_ReadBlocks(&uSdHandle,
                       (uint8_t *)buff,
                       sector,
                       count,
                       SD_TIMEOUT) == HAL_OK)
  {
    /* wait until the read operation is finished */
    while(HAL_SD_GetCardState(&uSdHandle) != HAL_SD_CARD_TRANSFER)
    {
    }
    res = RES_OK;
  }

  return res;
}

#endif /* (BINARY_AREA == USE_SDCARD) */
