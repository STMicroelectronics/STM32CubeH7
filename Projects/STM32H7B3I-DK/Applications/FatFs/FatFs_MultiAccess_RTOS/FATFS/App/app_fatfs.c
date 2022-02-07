/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FatFs/FatFs_MultiAccess_RTOS/FatFs/App/app_fatfs.c
  * @author  MCD Application Team
  * @brief   FatFs_uSD_Standalone application file
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
#include "app_fatfs.h"
#include "cmsis_os.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_INIT,
  APPLICATION_RUNNING,
  APPLICATION_SD_UNPLUGGED,
}FS_FileOperationsTypeDef;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FATFS_MKFS_ALLOWED 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;    /* File system object for SD logical drive */
FIL File1, File2 ;       /* File  object for SD */
char SDPath[4];   /* SD logical drive path */
uint32_t disk_op = 0, DiskReadyEvent=0x1, DiskRemoveEvent =0x2;
/* USER CODE BEGIN PV */
FS_FileOperationsTypeDef Appli_state = APPLICATION_IDLE;
uint8_t workBuffer[_MAX_SS];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static int32_t FS_File1Operations(void);
/* USER CODE END PFP */

/**
  * @brief FatFs initialization
  * @param  None
  * @retval Initialization result 
  */
int32_t MX_FATFS_Init(void) 
{
  /*## FatFS: Link the disk I/O driver(s)  ###########################*/  
  if (FATFS_LinkDriver(&SD_Driver, SDPath) != 0)
  /* USER CODE BEGIN FATFS_Init */
  {
    return APP_ERROR;
  }
  else
  {
    Appli_state = APPLICATION_INIT;
    return APP_OK;
  }
  /* USER CODE END FATFS_Init */
}
 
/**
  * @brief FatFs Deinitialization
  * @param  None
  * @retval Deinitialization result 
  */
int32_t MX_FATFS_DeInit(void) 
{
 /*## FatFS: UnLink the disk I/O driver(s)  ###########################*/  
  if (FATFS_UnLinkDriver(SDPath) != 0)
  /* USER CODE BEGIN FATFS_Init */
  {
    return APP_ERROR;
  }
  else
  {
    Appli_state = APPLICATION_INIT;
    return APP_OK;
  }
  /* USER CODE END FATFS_Init */
} 
/**
  * @brief FatFs application main process
  * @param  None
  * @retval Process result 
  */
int32_t MX_FATFS_Process(void)
{
  /* USER CODE BEGIN FATFS_Process */
  int32_t process_res = APP_OK;  
  
  switch(Appli_state)
  {
  case APPLICATION_INIT:
    if(BSP_SD_IsDetected(0) == SD_PRESENT)
    {
#if FATFS_MKFS_ALLOWED
      FRESULT res;

      res = f_mkfs(SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer));

      if (res != FR_OK)
      {
        process_res = APP_ERROR;
      }
      else
      {
        process_res = APP_INIT;
        Appli_state = APPLICATION_RUNNING;
      }
#else
      process_res = APP_INIT;
      Appli_state = APPLICATION_RUNNING;
#endif
    }
    else
    {
    process_res = APP_SD_UNPLUGGED;
    Appli_state = APPLICATION_SD_UNPLUGGED;
      
    }

    break;
  case APPLICATION_RUNNING:
    process_res = FS_File1Operations();
    Appli_state = APPLICATION_IDLE;
    break;

  case APPLICATION_SD_UNPLUGGED:
    process_res = APP_SD_UNPLUGGED;
    break;

  case APPLICATION_IDLE:
  default:
    break;
  }
  return process_res;
  /* USER CODE END FATFS_Process */
}
/**
  * @brief  Gets Time from RTC (generated when FS_NORTC==0; see ff.c)
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
  * @brief File system : file operation
  * @retval File operation result 
  */
static int32_t FS_File1Operations(void)
{
  FRESULT res; /* FatFs function common result code */
  uint32_t byteswritten ; /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs and uSD diskio driver"; /* File write buffer */

  /* Register the file system object to the FatFs module */
  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 1) == FR_OK)
  {
    /* Create and Open a new text file object with write access */
    if(f_open(&File1, "STM32_1.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
      /* Allow Second task to have access to FatFs */
      osMessageQueuePut(DiskEvent, &DiskReadyEvent, 100, osWaitForever);     
      osDelay(100);	  
	  /* Write data to the text file */
      res = f_write(&File1, wtext, sizeof(wtext), (void *)&byteswritten);

      if((byteswritten > 0) && (res == FR_OK))
      {
        /* Close the open text file */
        f_close(&File1);
        disk_op ++;
        /* If last access to Disk, unlink drive */
        while(disk_op < 2);
        osMessageQueuePut(DiskEvent, &DiskRemoveEvent, 100, 0);
        disk_op = 0;
        return 0;
      }           
	} 
 }
  /* Error */
  return -1;
}

/**
  * @brief File system : file operation
  * @retval File operation result 
  */
int32_t FS_File2Operations(void)
{
  FRESULT res; /* FatFs function common result code */
  uint32_t byteswritten ; /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs and uSD diskio driver"; /* File write buffer */
  
  /* Create and Open a new text file object with write access */
  if(f_open(&File2, "STM32_2.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
  {
    /* Write data to the text file */
    res = f_write(&File2, wtext, sizeof(wtext), (void *)&byteswritten);
    
    if((byteswritten > 0) && (res == FR_OK))
    {
      /* Close the open text file */
      f_close(&File2);           
      disk_op ++;
      return 0;
    }
  }
  /* Error */
  return -1;
}
/* USER CODE END Application */

