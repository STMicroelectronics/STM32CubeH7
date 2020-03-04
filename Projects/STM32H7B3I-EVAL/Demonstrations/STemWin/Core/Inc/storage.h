/**
  ******************************************************************************
  * @file    storage.h
  * @author  MCD Application Team
  * @brief   Header for storage.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STORAGE_H
#define __STORAGE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#if defined(USE_SDCARD)
#include "sd_diskio.h"
#endif /* USE_SDCARD */

#if defined(USE_USB_FS) || defined(USE_USB_HS)
#include "usbh_diskio.h"
#endif /* USE_USB_FS | USE_USB_HS */

/* Exported constants --------------------------------------------------------*/
#define FILEMGR_LIST_DEPDTH                    24
#define FILEMGR_FILE_NAME_SIZE                256
#define FILEMGR_MAX_LEVEL                       3
#define FILEMGR_MAX_EXT_SIZE                    3

#define FILETYPE_DIR                            0
#define FILETYPE_FILE                           1

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MSD_DISK_UNIT ,
  USB_DISK_UNIT ,
  NUM_DISK_UNITS
}
STORAGE_IdTypeDef;

typedef enum
{
  STORAGE_NOINIT
, STORAGE_UNMOUNTED /* same as STORAGE_CONNECTED */
, STORAGE_MOUNTED
} STORAGE_Status_t;

typedef enum
{
  USBDISK_DISCONNECTION_EVENT = 1,
  USBDISK_CONNECTION_EVENT,
  USBDISK_ACTIVE_CLASS_EVENT,
  MSDDISK_DISCONNECTION_EVENT,
  MSDDISK_CONNECTION_EVENT,
}
STORAGE_EventTypeDef;

typedef struct _FILELIST_LineTypeDef
{
  uint8_t               type;
  uint8_t               name[FILEMGR_FILE_NAME_SIZE];

}
FILELIST_LineTypeDef;

typedef struct _FILELIST_FileTypeDef
{
  FILELIST_LineTypeDef  file[FILEMGR_LIST_DEPDTH] ;
  uint16_t              ptr;
}
FILELIST_FileTypeDef;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void        Storage_Init(void);
void        Storage_DeInit(void);
uint8_t     Storage_GetStatus (uint8_t unit);
const char *Storage_GetDrive (uint8_t unit);

extern osMessageQId StorageEvent;

#define     k_StorageInit()           Storage_Init()
#define     k_StorageGetStatus(u)     Storage_GetStatus(u)

#ifdef __cplusplus
}
#endif

#endif /*__STORAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
