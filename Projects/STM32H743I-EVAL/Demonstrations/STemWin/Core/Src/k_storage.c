/**
  ******************************************************************************
  * @file    k_storage.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel storage functions
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
#include "k_storage.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_STORAGE
  * @brief Kernel storage routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
static struct {
  U32 Mask;
  char c;
} _aAttrib[] = {
  { AM_RDO, 'R' },
  { AM_HID, 'H' },
  { AM_SYS, 'S' },
  { AM_DIR, 'D' },
  { AM_ARC, 'A' },
};
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS mSDDISK_FatFs;         /* File system object for USB disk logical drive */
char mSDDISK_Drive[4];       /* MSD Host logical drive number */

osMessageQId StorageEvent;
DIR dir;
static char         acAttrib[10];
static char         acExt[FILEMGR_MAX_EXT_SIZE];

static uint32_t StorageStatus[NUM_DISK_UNITS];
/* Private function prototypes -----------------------------------------------*/
static void StorageThread(void const * argument);
static void GetExt(char * pFile, char * pExt);
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Storage drives initialization
  * @param  None 
  * @retval None
  */
void k_StorageInit(void)
{
  
   /* Link the micro SD disk I/O driver */
   FATFS_LinkDriver(&SD_Driver, mSDDISK_Drive);
  
   /* Create USB background task */
   osThreadDef(STORAGE_Thread, StorageThread, osPriorityLow, 0, 512);
   osThreadCreate (osThread(STORAGE_Thread), NULL);
  
   /* Create Storage Message Queue */
   osMessageQDef(osqueue, 10, uint16_t);
   StorageEvent = osMessageCreate (osMessageQ(osqueue), NULL);
  
   /* Enable SD Interrupt mode */
   BSP_SD_Init(0);
   BSP_SD_DetectITConfig(0);
     
   if(BSP_SD_IsDetected(0))
   {
     osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
   }
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
      case MSDDISK_CONNECTION_EVENT:
        f_mount(&mSDDISK_FatFs, mSDDISK_Drive, 0);
        StorageStatus[MSD_DISK_UNIT] = 1;
        break;
        
      case MSDDISK_DISCONNECTION_EVENT:
        f_mount(0, mSDDISK_Drive, 0);
        StorageStatus[MSD_DISK_UNIT] = 0;        
        break;         
      }
    }
  }
}

/**
  * @brief  Storage get status
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint8_t k_StorageGetStatus (uint8_t unit)
{  
  return StorageStatus[unit];
}

/**
  * @brief  Storage get capacity
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint32_t k_StorageGetCapacity (uint8_t unit)
{  
  uint32_t   tot_sect = 0;
  FATFS *fs;
  
  if(unit == USB_DISK_UNIT)
  {
    fs = &mSDDISK_FatFs;
    tot_sect = (fs->n_fatent - 2) * fs->csize;
  }
  return (tot_sect);
}

/**
  * @brief  Storage get free space
  * @param  unit: logical storage unit index. 
  * @retval int
  */
uint32_t k_StorageGetFree (uint8_t unit)
{ 
  uint32_t   fre_clust = 0;
  FATFS *fs ;
  FRESULT res = FR_INT_ERR;
  
  if(unit == USB_DISK_UNIT)
  {
    fs = &mSDDISK_FatFs;
    res = f_getfree("0:", (DWORD *)&fre_clust, &fs);
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
  * @brief  Return file extension and removed from file name.
  * @param  pFile: pointer to the file name.
  * @param  pExt:  pointer to the file extension
  * @retval None
  */
static void GetExt(char * pFile, char * pExt) 
{
  int Len;
  int i;
  int j;
  
  /* Search beginning of extension */
  Len = strlen(pFile);
  for (i = Len; i > 0; i--) {
    if (*(pFile + i) == '.') {
      *(pFile + i) = '\0';     /* Cut extension from filename */
      break;
    }
  }
  
  /* Copy extension */
  j = 0;
  while (*(pFile + ++i) != '\0') {
    *(pExt + j) = *(pFile + i);
    j++;
  }
  *(pExt + j) = '\0';          /* Set end of string */
}

/**
  * @brief  Return the extension Only
  * @param  pFile: pointer to the file name.
  * @param  pExt:  pointer to the file extension
  * @retval None
  */
void k_GetExtOnly(char * pFile, char * pExt) 
{
  int Len;
  int i;
  int j;
  
  /* Search beginning of extension */
  Len = strlen(pFile);
  for (i = Len; i > 0; i--) {
    if (*(pFile + i) == '.') {
      break;
    }
  }
  
  /* Copy extension */
  j = 0;
  while (*(pFile + ++i) != '\0') {
    *(pExt + j) = *(pFile + i);
    j++;
  }
  *(pExt + j) = '\0';          /* Set end of string */
}
/**
  * @brief  This function is responsible to pass information about the requested file
  * @param  pInfo: Pointer to structure which contains all details of the requested file.
  * @retval None
  */
int k_GetData(CHOOSEFILE_INFO * pInfo)
{
  char                c;
  int                 i;
  char               tmp[CHOOSEFILE_MAXLEN];  
  FRESULT res = FR_INT_ERR;
  char *fn;
  FILINFO fno;
 
  switch (pInfo->Cmd) 
  {
  case CHOOSEFILE_FINDFIRST:
    f_closedir(&dir); 
    
    /* reformat path */
    memset(tmp, 0, CHOOSEFILE_MAXLEN);
    strcpy(tmp, pInfo->pRoot);
    
    for(i= CHOOSEFILE_MAXLEN; i > 0 ; i--)
    {
      if(tmp[i] == '/')
      {
        tmp[i] = 0 ;
        break;
      }
    }
    
    res = f_opendir(&dir, tmp);
    
    if (res == FR_OK)
    {
      
      res = f_readdir(&dir, &fno);
    }
    break;
    
  case CHOOSEFILE_FINDNEXT:
    res = f_readdir(&dir, &fno);
    break;
  }
  
  if (res == FR_OK)
  {
    fn = fno.fname;

    while (((fno.fattrib & AM_DIR) == 0) && (res == FR_OK))
    {
      
      if((strstr(pInfo->pMask, ".img")))
      {
        if((strstr(fn, ".bmp")) || (strstr(fn, ".jpg")) || (strstr(fn, ".BMP")) || (strstr(fn, ".JPG")))
        {
          break;
        }
        else
        {
          res = f_readdir(&dir, &fno);
          
          if (res != FR_OK || fno.fname[0] == 0)
          {
            f_closedir(&dir); 
            return 1;
          }
          else
          {
            fn = fno.fname;
          }
        }
        
      }
      else if((strstr(pInfo->pMask, ".audio")))
      {
        if((strstr(fn, ".wav")) || (strstr(fn, ".WAV")))
        {
          break;
        }
        else
        {
          res = f_readdir(&dir, &fno);
          
          if (res != FR_OK || fno.fname[0] == 0)
          {
            f_closedir(&dir); 
            return 1;
          }
          else
          {
            fn = fno.fname;
          }
        }
        
      }
      
      else if((strstr(pInfo->pMask, ".video")))
      {
        if((strstr(fn, ".avi")) || (strstr(fn, ".AVI")))
        {
          break;
        }
        else
        {
          res = f_readdir(&dir, &fno);
          
          if (res != FR_OK || fno.fname[0] == 0)
          {
            f_closedir(&dir); 
            return 1;
          }
          else
          {
            fn = fno.fname;
          }
        }
        
      }      
      else if(strstr(fn, pInfo->pMask) == NULL)
      {
        
        res = f_readdir(&dir, &fno);
        
        if (res != FR_OK || fno.fname[0] == 0)
        {
          f_closedir(&dir); 
          return 1;
        }
        else
        {
          fn = fno.fname;
        }
      }
      else
      {
        break;
      }
    }   
    
    if(fn[0] == 0)
    {
      f_closedir(&dir); 
      return 1;
    } 
    
    pInfo->Flags = ((fno.fattrib & AM_DIR) == AM_DIR) ? CHOOSEFILE_FLAG_DIRECTORY : 0;
    
    for (i = 0; i < GUI_COUNTOF(_aAttrib); i++)
    {
      if (fno.fattrib & _aAttrib[i].Mask)
      {
        c = _aAttrib[i].c;
      }
      else
      {
        c = '-';
      }
      acAttrib[i] = c;
    }
    if((fno.fattrib & AM_DIR) == AM_DIR)
    {
      acExt[0] = 0;
    }
    else
    {
      GetExt(fn, acExt);
    }
    pInfo->pAttrib = acAttrib;
    pInfo->pName = fn;
    pInfo->pExt = acExt;
    pInfo->SizeL = fno.fsize;
    pInfo->SizeH = 0;
    
  }
  return res;
}

/**
  * @brief  SD detect callback
  * @param  None
  * @retval None
  */ 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if((BSP_SD_IsDetected(0)))
  {  
    /* After sd disconnection, a SD Init is required */
    BSP_SD_Init(0);
    
    osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
  }
  else
  {
    osMessagePut ( StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0);
  }
}

/**
  * @}
  */

/**
  * @}
  */

