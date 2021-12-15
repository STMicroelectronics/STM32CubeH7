/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_Standalone/FatFs/App/app_fatfs.c
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
  APPLICATION_SD_PLUGGED,
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
FIL SDFile;       /* File  object for SD */
char SDPath[4];   /* SD logical drive path */
/* USER CODE BEGIN PV */
FS_FileOperationsTypeDef Appli_state = APPLICATION_IDLE;
uint8_t workBuffer[_MAX_SS];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static int32_t FS_FileOperations(void);
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
  * @brief FatFs application main process
  * @param  None
  * @retval Process result
  */
int32_t MX_FATFS_Process(void)
{
  /* USER CODE BEGIN FATFS_Process */
  int32_t process_res = APP_OK;
  /* Mass Storage Application State Machine */
  switch(Appli_state)
  {
  case APPLICATION_INIT:
    if(BSP_SD_IsDetected(0))
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
    Appli_state = APPLICATION_SD_UNPLUGGED;

    }

    break;
  case APPLICATION_RUNNING:
      process_res = FS_FileOperations();
      Appli_state = APPLICATION_IDLE;
    break;

  case APPLICATION_SD_UNPLUGGED:
    process_res = APP_SD_UNPLUGGED;
    break;
  case APPLICATION_SD_PLUGGED:
    BSP_SD_Init(0);
    BSP_SD_DetectITConfig(0);
    Appli_state = APPLICATION_RUNNING;
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
static int32_t FS_FileOperations(void)
{
  FRESULT res; /* FatFs function common result code */
  uint32_t byteswritten, bytesread; /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs and uSD diskio driver"; /* File write buffer */
  uint8_t rtext[100]; /* File read buffer */

  /* Register the file system object to the FatFs module */
  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
  {
    /* Create and Open a new text file object with write access */
    if(f_open(&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
      /* Write data to the text file */
      res = f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten);

      if((byteswritten > 0) && (res == FR_OK))
      {
        /* Close the open text file */
        f_close(&SDFile);

        /* Open the text file object with read access */
        if(f_open(&SDFile, "STM32.TXT", FA_READ) == FR_OK)
        {
          /* Read data from the text file */
          res = f_read(&SDFile, rtext, sizeof(rtext), (void *)&bytesread);

          if((bytesread > 0) && (res == FR_OK))
          {
            /* Close the open text file */
            f_close(&SDFile);

            /* Compare read data with the expected data */
            if((bytesread == byteswritten))
            {
              /* Success of the demo: no error occurrence */
              return 0;
            }
          }
        }
      }
    }
  }
  /* Error */
  return -1;
}

/**
  * @brief  BSP SD Callback.
  * @param  Instance SD Instance
  * @param  Status   Pin status
  * @retval None.
  */
void BSP_SD_DetectCallback(uint32_t Instance, uint32_t Status)
{
  if (Instance ==0)
  {
    if (Status == SD_PRESENT)
    {
      Appli_state = APPLICATION_RUNNING;
    }
    else
    {
      Appli_state = APPLICATION_SD_UNPLUGGED;
      f_mount(NULL, (TCHAR const*)"", 0);
    }
  }
}
/* USER CODE END Application */

