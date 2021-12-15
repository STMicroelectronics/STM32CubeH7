/**
  ******************************************************************************
  * @file    sdmmc.c
  * @author  MCD Application Team
  * @brief   This file includes the MMC supported STM32H7B3I-EVAL 
             board.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - ToBeUpdated :  SDMMC  mounted on STM32H7B3I EVAL board.

  @endverbatim
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
#include "stm32h7xx_hal.h"
#include "memory.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
char SDPath[4]; /* SD card logical drive path */
FIL Binary_File;  /* File object */

/* SDCARD transfer state definition */
#if (BINARY_AREA == USE_SDCARD)
uint32_t SDCARD_Startup(void)
{
  /*##-1- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    uint32_t readSize = 0;
    
    /*##-2- Init the SD Card #################################################*/
    if(BSP_SD_Init(0) != BSP_ERROR_NONE)
    {
      return MEMORY_ERROR;
    }
    
    if(BSP_SD_IsDetected(0) == SD_PRESENT)
    {
      /*##-3- Register the file system object to the FatFs module ##############*/
      if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
      {
        /*##-4- Open the Binary file with read access #######################*/
        if(f_open(&Binary_File, "Project.bin", FA_READ) == FR_OK)
        {
          /*##-5- Copy the Binary to the Application address #######################*/
          if(f_read(&Binary_File,(uint32_t *)APPLICATION_ADDRESS, BINARY_SIZE, (UINT*)&readSize))
          {
            return MEMORY_ERROR;
          }
          
          /*##-6- De-Initialize the non-volatile memory  #######################*/
          if(BSP_SD_DeInit(0) != BSP_ERROR_NONE)
          {
            return MEMORY_ERROR;
          }
        }
        else
        {
          return MEMORY_ERROR;
        }
      }
    }
    else
    {
      return MEMORY_ERROR;
    }
  }
  return MEMORY_OK; 
}
#endif /* (BINARY_AREA == SDCARD) */
