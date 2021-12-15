/**
  ******************************************************************************
  * @file    Audio/Audio_playback_and_record/Src/explorer.c
  * @author  MCD Application Team
  * @brief   This file provides USB Key drive configuration
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
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
FATFS USBH_FatFs;
char USBKey_Path[4] = "0:/";
FILELIST_FileTypeDef FileList;
uint16_t NumObs = 0;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the USB KEY Storage.
  * @param  None
  * @retval Status
  */
uint8_t AUDIO_StorageInit(void)
{
  /* Link the USB Mass Storage disk I/O driver */
  if(FATFS_LinkDriver(&USBH_Driver, USBKey_Path) != 0)
  {
    /* FatFs Initialization Error */
    return 1;
  }

  /* Link the USB Key disk I/O driver */
  if((f_mount(&USBH_FatFs, (TCHAR const*)USBKey_Path, 0) != FR_OK))
  {
    /* FatFs Initialization Error */
    LCD_ErrTrace("Cannot Initialize FatFs! \n");
    return 1;
  }
  else
  {
    LCD_DbgTrace ("INFO : FatFs Initialized! \n");
    return 0;
  }
}

/**
  * @brief  Copies disk content in the explorer list.
  * @param  None
  * @retval Operation result
  */
FRESULT AUDIO_StorageParse (void)
{
  FRESULT res = FR_OK;  /* Return value */
  DIR dir;              /* Directory search object */
  FILINFO fno;          /* File information */
  FileList.ptr = 0;

  /* Start to search for wave files */
  res = f_findfirst(&dir, &fno, USBKey_Path, "*.wav");

  /* Repeat while an item is found */
  while (fno.fname[0])
  {
    if(res == FR_OK)
    {
      if(FileList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if((fno.fattrib & AM_DIR) == 0)
        {
          if((strstr(fno.fname, "wav")) || (strstr(fno.fname, "WAV")))
          {
            strncpy((char *)FileList.file[FileList.ptr].name, (char *)fno.fname, FILEMGR_FILE_NAME_SIZE);
            FileList.file[FileList.ptr].type = FILETYPE_FILE;
            FileList.ptr++;
          }
        }
      }
      /* Search for next item */
      res = f_findnext(&dir, &fno);
    }
    else
    {
      break;
    }
  }

  NumObs = FileList.ptr;
  f_closedir(&dir);

  return res;
}

/**
  * @brief  Shows audio file (*.wav) on the root
  * @param  None
  * @retval None
  */
uint8_t AUDIO_ShowWavFiles(void)
{
  uint8_t i = 0;
  uint8_t line_idx = 0;
  uint8_t ret = FR_OK;

  ret = AUDIO_StorageParse();
  if(ret ==  FR_OK)
  {
    if(FileList.ptr > 0)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      LCD_UsrTrace("audio file(s) [ROOT]:\n\n");

      for(i = 0; i < FileList.ptr; i++)
      {
        line_idx++;
        LCD_DbgTrace("   |__");
        LCD_DbgTrace((char *)FileList.file[i].name);
        LCD_DbgTrace("\n");
      }
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      LCD_UsrTrace("\nEnd of files list.\n");
    }
  }

  return ret;
}

/**
  * @brief  Gets Wav Object Number.
  * @param  None
  * @retval None
  */
uint16_t AUDIO_GetWavObjectNumber(void)
{
  return NumObs;
}

