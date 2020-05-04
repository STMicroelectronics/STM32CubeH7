/**
  ******************************************************************************
  * @file    storage_app.h
  * @author  MCD Application Team
  * @brief   Header for storage_app.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
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

#include <cmsis_os.h>

#include <string.h>

#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

/* Exported constants --------------------------------------------------------*/
#define FILEMGR_LIST_DEPDTH                      24
#define FILEMGR_FILE_NAME_SIZE                  256
#define FILEMGR_MAX_LEVEL                        3  
#define FILEMGR_MAX_EXT_SIZE                     3
   
#define FILETYPE_DIR                             0
#define FILETYPE_FILE                            1

#define NUM_DISK_UNITS                           2
#define MSD_DISK_UNIT                            0
#define USB_DISK_UNIT                            1     

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
TCHAR   *STORAGE_GetDrive (uint8_t unit);
uint32_t STORAGE_GetCapacity (uint8_t unit);
uint32_t STORAGE_GetFree (uint8_t unit);
void     STORAGE_NotifyConnectionChange(uint8_t unit, uint8_t state);


#ifdef __cplusplus
}
#endif

#endif /*__STORAGE_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
