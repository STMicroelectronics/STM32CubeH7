/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_RTOS/FatFs/App/app_fatfs.c
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

FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */
char SDPath[4];   /* SD logical drive path */
/* USER CODE BEGIN PV */
FS_FileOperationsTypeDef Appli_state = APPLICATION_IDLE;
uint8_t workBuffer[_MAX_SS];
uint32_t is_initialized = 0;
extern int32_t ProcessStatus;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static int32_t FS_FileOperations(void);
static void SD_Init(void);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* USER CODE END PFP */

/**
  * @brief  FatFs initialization
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
  * @brief  FatFs application main process
  * @param  None
  * @retval Process result
  */
void MX_FATFS_Process(void *argument)
{
  /* USER CODE BEGIN MX_FATFS_Process */
  int32_t process_res = APP_OK;
  /* Mass Storage Application State Machine */
  while(1)
  {
  switch(Appli_state)
  {
  case APPLICATION_INIT:
    if(BSP_SD_IsDetected(0) == SD_PRESENT)
    {
      SD_Init();
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
      SD_Init();
      process_res = FS_FileOperations();
      Appli_state = APPLICATION_IDLE;
    break;

  case APPLICATION_SD_UNPLUGGED:
    process_res = APP_SD_UNPLUGGED;
    break;

  case APPLICATION_IDLE:
  default:
    break;
  }
  ProcessStatus = process_res;
  }
  /* USER CODE END MX_FATFS_Process */
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
  * @brief File system : file operations
  * @retval File operation result
  */
static int32_t FS_FileOperations(void)
{
  FRESULT res; /* FatFs function common result code */
  uint32_t byteswritten, bytesread; /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs and uSD diskio driver"; /* File write buffer */
  uint8_t rtext[100]; /* File read buffer */

  /* Register the file system object to the FatFs module */
  if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) == FR_OK)
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
              if(Buffercmp((uint32_t *)rtext, (uint32_t *)wtext, sizeof(rtext)))
              {  /* Success of the demo: no error occurrence */
              return 0;
              }
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
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{

  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
/**
  * @brief  BSP SD Callback.
  * @param  Instance SD Instance
  * @param  Status   Pin status
  * @retval None.
  */
void BSP_SD_DetectCallback(uint32_t Instance, uint32_t Status)
{
  if (Status == SD_PRESENT)
  {
    Appli_state = APPLICATION_RUNNING;
    is_initialized = 0;
  }
  else
  {
    Appli_state = APPLICATION_SD_UNPLUGGED;
    f_mount(NULL, (TCHAR const*)"", 0);
  }
}
/**
  * @brief  BSP SD Initialization
  * @param  None
  * @retval None.
  */
static void SD_Init()
{
  if (!is_initialized)
  {
     BSP_SD_Init(0);
     BSP_SD_DetectITConfig(0);
     is_initialized = 1;
  }
}
/* USER CODE END Application */

