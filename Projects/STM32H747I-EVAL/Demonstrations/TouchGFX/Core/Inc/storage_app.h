/**
  ******************************************************************************
  * @file    storage_app.h
  * @author  MCD Application Team
  * @brief   Header for storage_app.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STORAGE_APP_H
#define __STORAGE_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef STM32H747xx
#include "stm32h7xx_hal.h"
#endif

#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "sd_diskio_dma.h"

/* Exported constants --------------------------------------------------------*/
#define FILEMGR_LIST_DEPDTH                      24
#define FILEMGR_FILE_NAME_SIZE                  256
#define FILEMGR_MAX_LEVEL                        3  
#define FILEMGR_MAX_EXT_SIZE                     3
   
#define FILETYPE_DIR                             0
#define FILETYPE_FILE                            1

#define NUM_DISK_UNITS                           2
#define USB_DISK_UNIT                            0   
#define MSD_DISK_UNIT                            1  

/* Exported types ------------------------------------------------------------*/
typedef struct _FILELIST_LineTypeDef
{
  uint8_t               type; /* 0, file/ 1: folder */
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

typedef enum
{
  USBDISK_DISCONNECTION_EVENT = 1,  
  USBDISK_CONNECTION_EVENT = 2,  
  MSDDISK_DISCONNECTION_EVENT = 3,  
  MSDDISK_CONNECTION_EVENT = 4,    
}   
STORAGE_EventTypeDef;

/* Exported variables ------------------------------------------------------- */
extern osMessageQId StorageEvent;

/* Exported functions ------------------------------------------------------- */
void     STORAGE_Init(void);
void     STORAGE_AddEntireFolder(char *Foldername, FILELIST_FileTypeDef *list, char *ext);
uint8_t  STORAGE_GetStatus (uint8_t unit);
TCHAR   *Storage_GetDrive (uint8_t unit);
uint32_t STORAGE_GetCapacity (uint8_t unit);
uint32_t STORAGE_GetFree (uint8_t unit);
void     STORAGE_NotifyConnectionChange(uint8_t unit, uint8_t state);


#ifdef __cplusplus
}
#endif

#endif /*__STORAGE_APP_H */

