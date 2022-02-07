/**
  ******************************************************************************
  * @file    Display/LTDC_PicturesFromSDCard/Src/fatfs_storage.c
  * @author  MCD Application Team
  * @brief   This file includes the Storage (FatFs) driver for the STM32H7B3I-DK
  *          application.
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
#include "fatfs_storage.h"

extern void DMA2D_CopyBufferWithAlpha_filter(const uint16_t xSize, const uint16_t ySize, void * pSrc, void * pDst);

/** @addtogroup STM32H7B3I-DK_FAT_FS
  * @{
  */

/** @defgroup FATFS_STORAGE
  * @brief This file includes the Storage (FatFs) driver for the STM32H7B3I-DK
  *        application.
  * @{
  */

/** @defgroup FATFS_STORAGE_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Variables
  * @{
  */
#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="sdio_heap"
#else
__attribute__((section(".sdio_heap"))) __attribute__ ((aligned (32)))
#endif
uint8_t sector[512];

FILINFO fno;

const uint8_t SlidesCheck[2] =
{
  0x42, 0x4D
};
uint32_t BytesRead = 0;

/**
  * @}
  */


/** @defgroup FATFS_STORAGE_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Functions
  * @{
  */

/**
  * @brief  Open a file and copy its content to a buffer
  * @param  DirName: the Directory name to open
  * @param  FileName: the file name to open
  * @param  BufferAddress: A pointer to a buffer to copy the file to
  * @param  FileLen: the File length
  * @retval err: Error status (0=> success, otherwise, fail)
  */
uint32_t Storage_OpenReadFile(uint8_t storage_id, uint8_t *Address, const char* BmpName)
{
  uint32_t index = 0, size = 0, i1 = 0;
  uint32_t BmpAddress;
  FIL F1;

  if(!Storage_GetStatus(storage_id))
  {
    return 1;
  }

  if (f_open(&F1, (TCHAR const*)BmpName, FA_READ) != FR_OK)
  {
    return 2;
  }
  if (f_read (&F1, sector, 30, (UINT *)&BytesRead) != FR_OK)
  {
    f_close (&F1);
    return 3;
  }
  if(!Storage_GetStatus(storage_id))
  {
    return 1;
  }
  f_close (&F1);
  
  BmpAddress = (uint32_t)sector;

  /* Read bitmap size */
  size = *(uint16_t *) (BmpAddress + 2);
  size |= (*(uint16_t *) (BmpAddress + 4)) << 16;  
 
  /* Get bitmap data address offset */
  index = *(uint16_t *) (BmpAddress + 10);
  index |= (*(uint16_t *) (BmpAddress + 12)) << 16;
  
  if(f_open (&F1, (TCHAR const*)BmpName, FA_READ) != FR_OK)
  {
    return 2;
  }

  do
  {
    if(!Storage_GetStatus(storage_id))
    {
      break;
    }

    if (size < 256*2)
    {
      i1 = size;
    }
    else
    {
      i1 = 256*2;
    }
    size -= i1;

    if(f_read (&F1, sector, i1, (UINT *)&BytesRead) != FR_OK)
    {
      break;
    }

    for (index = 0; index < i1; index++)
    {
      *(__IO uint8_t*) (Address) = *(__IO uint8_t *)BmpAddress;
      
      BmpAddress++;  
      Address++;
    }  
    
    BmpAddress = (uint32_t)sector;
  }
  while (size > 0);

  f_close (&F1);

  return 0;
}

/**
  * @brief  List up to 25 file on the root directory with extension .BMP
  * @param  None
  * @retval The number of the found files
  */
uint32_t Storage_GetDirectoryBitmapFiles (const char* DirName, char* Files[])
{
  FILINFO fno;
  DIR dir;
  uint32_t counter = 0, index = 0;
  FRESULT res;

  /* Open directory */
  res = f_opendir(&dir, (TCHAR const*)DirName);
  
  if (res == FR_OK)
  {
    for (;;)
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
        break;
      if (fno.fname[0] == '.')
        continue;

      if (!(fno.fattrib & AM_DIR))
      {
        do
        {
          counter++;
        }
        while (fno.fname[counter] != 0x2E); /* . */


        if (index < MAX_BMP_FILES)
        {
          if ((fno.altname[counter + 1] == 'B') && (fno.altname[counter + 2] == 'M') && (fno.altname[counter + 3] == 'P'))
          {
            if(sizeof(fno.fname) >= (MAX_BMP_FILE_NAME + 2))
            {
              sprintf (Files[index], "%s", fno.fname);
              index++;
            }
          }
        }
        counter = 0;
      }
    }
  }
  f_mount(NULL, (TCHAR const*)"",0);
  return index;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
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
  * @brief  Draws a bitmap picture loaded in the internal Flash (32 bpp).
  * @param  Xpos: 
  * @param  Ypos: 
  * @param  pbmp: 
  * @retval None
  */
void LCD_DrawBitmap_Effect(const uint8_t storage_id, const uint8_t swap, const uint16_t xSize, const uint16_t ySize, uint8_t *pbmp, void * pDst)
{
  if(Storage_GetStatus(storage_id))
  {
    if(swap == 1)
    {
      uint32_t y = 0;
      /* Set the address */
      uint32_t address = (uint32_t)pDst;

      /* Bypass the bitmap header */
      pbmp += (54 + (xSize * (ySize - 1) * 4));

      /* Convert picture to ARGB8888 pixel format */
      for(y=0; y < ySize; y++)
      {
        /* Pixel format conversion */
        DMA2D_CopyBufferWithAlpha_filter(xSize, 1, (uint8_t *)pbmp, (uint32_t *)address);

        /* Increment the source and destination buffers */
        address += (xSize*4);
        pbmp    -= (xSize*4);
      }
    }
    else
    {
      DMA2D_CopyBufferWithAlpha_filter(xSize, ySize, (uint8_t *)pbmp, (uint32_t *)pDst);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

