/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/CM7/Src/file_operations.c
  * @author  MCD Application Team
  * @brief   Write/read file on the disk.
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

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
FATFS USBH_fatfs;
FIL MyFile;
FRESULT res;
uint32_t bytesWritten;
uint8_t rtext[200];
uint8_t wtext[] = "USB Host Library : Mass Storage application";

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Files operations: Read/Write and compare
  * @param  None
  * @retval None
  */
void MSC_File_Operations(void)
{
  uint32_t bytesread;

    LCD_UsrTrace("INFO : FatFs Initialized \n");

    if (f_open(&MyFile, "0:USBHost.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      LCD_ErrTrace("Cannot Open 'USBHost.txt' file \n");
    }
    else
    {
      LCD_UsrTrace("INFO : 'USBHost.txt' opened for write  \n");
      res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&bytesWritten);
      f_close(&MyFile);

      if ((bytesWritten == 0) || (res != FR_OK))  /* EOF or Error */
      {
        LCD_ErrTrace("Cannot Write on the  'USBHost.txt' file \n");
      }
      else
      {
        if (f_open(&MyFile, "0:USBHost.txt", FA_READ) != FR_OK)
        {
          LCD_ErrTrace("Cannot Open 'USBHost.txt' file for read.\n");
        }
        else
        {
          LCD_UsrTrace("INFO : Text written on the 'USBHost.txt' file \n");

          res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);

          if ((bytesread == 0) || (res != FR_OK)) /* EOF or Error */
          {
            LCD_ErrTrace("Cannot Read from the  'USBHost.txt' file \n");
          }
          else
          {
            LCD_UsrTrace("Read Text : \n");
            LCD_DbgTrace((char *)rtext);
            LCD_DbgTrace("\n");
          }
          f_close(&MyFile);
        }
        /* Compare read data with the expected data */
        if ((bytesread == bytesWritten))
        {
          LCD_UsrTrace("INFO : FatFs data compare SUCCES");
          LCD_UsrTrace("\n");
        }
        else
        {
          LCD_ErrTrace("FatFs data compare ERROR");
          LCD_ErrTrace("\n");
      }
    }
  }
}

