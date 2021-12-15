/**
  ******************************************************************************
  * @file    storage_app.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel storage functions
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
#include "storage_app.h"

#define DECODE_MEDIA_DISCONNECTED_MESSAGE 0xFF07

#ifdef __GNUC__
#define PRAGMA_SDIO_HEAP_LOCATION
#define ATTRIBUTE_SDIO_HEAP_LOCATION __attribute__ ((section ("sdio_heap"))) __attribute__ ((aligned(4)))
#elif defined __ICCARM__
#define PRAGMA_SDIO_HEAP_LOCATION _Pragma("location=\"sdio_heap\"")
#define ATTRIBUTE_SDIO_HEAP_LOCATION
#elif defined(__ARMCC_VERSION)
#define PRAGMA_SDIO_HEAP_LOCATION
#define ATTRIBUTE_SDIO_HEAP_LOCATION __attribute__ ((section ("sdio_heap"))) __attribute__ ((aligned(4)))
#endif

ALIGN_32BYTES(PRAGMA_SDIO_HEAP_LOCATION FATFS USBDISK_FatFs ATTRIBUTE_SDIO_HEAP_LOCATION);         /* File system object for USB disk logical drive */
ALIGN_32BYTES(PRAGMA_SDIO_HEAP_LOCATION FATFS MSDDISK_FatFs ATTRIBUTE_SDIO_HEAP_LOCATION);         /* File system object for SDMMC disk logical drive */

char USBDISK_Drive[4] = "0:/";       /* USB Host logical drive number */
char MSDDISK_Drive[4] = "1:/";       /* SDMMC logical drive number */


const Diskio_drvTypeDef * USBDISK_Driver = &USBH_Driver;
const Diskio_drvTypeDef * MSDDISK_Driver = &SD_Driver;

osMessageQId StorageEvent;

extern xQueueHandle GUITaskQueue;
extern osMessageQId FrameEvent;

ALIGN_32BYTES(PRAGMA_SDIO_HEAP_LOCATION USBH_HandleTypeDef hUSBHost ATTRIBUTE_SDIO_HEAP_LOCATION);

static uint32_t StorageStatus[NUM_DISK_UNITS];

static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id);
static void StorageThread(void const * argument);

/**
  * @brief  Storage drives initialization
  * @param  None 
  * @retval None
  */
void STORAGE_Init(void)
{
  /* Create Storage Message Queue */
  osMessageQDef(osqueue, 30, uint16_t);
  StorageEvent = osMessageCreate (osMessageQ(osqueue), NULL);

  /**** USB HOST Initialization ****/
  /* Create USB background task */
  osThreadDef(STORAGE_Thread, StorageThread, osPriorityNormal, 0, 1024);
  osThreadCreate (osThread(STORAGE_Thread), NULL);

  /* Enable the USB voltage level detector */
  HAL_PWREx_EnableUSBVoltageDetector();
  
  /* Init Host Library */
  USBH_Init(&hUSBHost, USBH_UserProcess, 0);

  /* Add Supported Class */
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);

  /* Start Host Process */
  USBH_Start(&hUSBHost);

  /**** SDMMC Initialization ****/
  /* Init the SD Card */
  BSP_SD_Init(0);

  /* NVIC configuration for SDMMC1 interrupts */
  HAL_NVIC_SetPriority(SDMMC1_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(SDMMC1_IRQn);

  /* Enable SD Interrupt mode */
  BSP_SD_DetectITConfig(0);

  if(BSP_SD_IsDetected(0))
  {
    osMessagePut(StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
  }
}

/**
  * @brief  Storage get status
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint8_t STORAGE_GetStatus (uint8_t unit)
{  
  return StorageStatus[unit];
}

/**
  * @brief  Get Drive Logical storage driver name
  * @param  unit: logical storage unit index.
  * @retval TCHAR *
  */
TCHAR *Storage_GetDrive (uint8_t unit)
{
  if(unit == USB_DISK_UNIT)
  {
    return (TCHAR*)USBDISK_Drive;
  }
  else
  {
    return (TCHAR*)MSDDISK_Drive;
  }
}

/**
  * @brief  Storage get capacity
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint32_t STORAGE_GetCapacity (uint8_t unit)
{  
  uint32_t   tot_sect = 0;
  FATFS *fs;
  
  if(unit == USB_DISK_UNIT)
  {
    fs = &USBDISK_FatFs;
  }
  else
  {
    fs = &MSDDISK_FatFs;
  }
  tot_sect = (fs->n_fatent - 2) * fs->csize;
  return (tot_sect);
}

/**
  * @brief  Storage get free space
  * @param  unit: logical storage unit index. 
  * @retval int
  */
uint32_t STORAGE_GetFree (uint8_t unit)
{ 
  uint32_t   fre_clust = 0;
  FATFS *fs ;
  FRESULT res = FR_INT_ERR;
  
  if(unit == USB_DISK_UNIT)
  {
    fs = &USBDISK_FatFs;
    res = f_getfree("0:", (DWORD *)&fre_clust, &fs);
  }
  else
  {
    fs = &MSDDISK_FatFs;
    res = f_getfree("1:", (DWORD *)&fre_clust, &fs);
  }
  
  if(res == FR_OK)
  {
    return (fre_clust * fs->csize);
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id:    Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    osMessagePut(StorageEvent, USBDISK_DISCONNECTION_EVENT, 0);
    break;

  case HOST_USER_CLASS_ACTIVE:
    osMessagePut(StorageEvent, USBDISK_CONNECTION_EVENT, 0);
    break;
  }
}


/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
void STORAGE_AddEntireFolder(char *Foldername, FILELIST_FileTypeDef *list, char *ext)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 

  fno.fsize = _MAX_LFN;

  res = f_opendir(&dir, Foldername);
  
  if (res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;

      if (list->ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(strstr(fn, ext))
          {
            
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            strncpy((char *)list->file[list->ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            list->ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}

/**
  * @brief  Storage Thread
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void StorageThread(void const * argument)
{
  osEvent event;
  
  for( ;; )
  {
    event = osMessageGet( StorageEvent, osWaitForever );

    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
      case USBDISK_CONNECTION_EVENT:
        /* Link the USB Host disk I/O driver */
        if(FATFS_LinkDriver((Diskio_drvTypeDef*)USBDISK_Driver, (TCHAR*)USBDISK_Drive) == 0)
        {
            if (f_mount(&USBDISK_FatFs,USBDISK_Drive,  0) == FR_OK)
            {
                StorageStatus[USB_DISK_UNIT] = 1;
                STORAGE_NotifyConnectionChange (USB_DISK_UNIT, 1);
            }
        }
        break;
        
      case USBDISK_DISCONNECTION_EVENT:
        if(f_mount(NULL, "", 0) == FR_OK)
        {
            if(FATFS_UnLinkDriver(USBDISK_Drive) == 0)
            {
                StorageStatus[USB_DISK_UNIT] = 0;
                STORAGE_NotifyConnectionChange (USB_DISK_UNIT, 0);
            }
        }
        break;  
      
      case MSDDISK_CONNECTION_EVENT:
        /* Enable SD Interrupt mode */
        if(BSP_SD_Init(0) == BSP_ERROR_NONE)
        {
            if(BSP_SD_DetectITConfig(0) == BSP_ERROR_NONE)
            {
                if(FATFS_LinkDriver((Diskio_drvTypeDef*)MSDDISK_Driver, (TCHAR*)MSDDISK_Drive) == 0)
                {
                    if (!f_mount(&MSDDISK_FatFs,MSDDISK_Drive,  0))
                    {
                        StorageStatus[MSD_DISK_UNIT] = 1;
                        STORAGE_NotifyConnectionChange (MSD_DISK_UNIT, 1);        
                    }
                }
            }
        }
        break;
        
      case MSDDISK_DISCONNECTION_EVENT:
        f_mount(NULL, "", 0);
        FATFS_UnLinkDriver(MSDDISK_Drive);
        StorageStatus[MSD_DISK_UNIT] = 0;
        BSP_SD_DeInit(0);
        STORAGE_NotifyConnectionChange (MSD_DISK_UNIT, 0);          
        break;  
      }      
    }
  }
}

/**
  * @brief  Notify Storage Unit connection state.
  * @retval None
  */
__weak void  STORAGE_NotifyConnectionChange(uint8_t unit, uint8_t state)
{
  
}

/**
  * @}
  */

/**
  * @}
  */

