/**
******************************************************************************
  * @file    : USB_HOST/MSC_Standalone/USB_Host/App/file_operations.c
  * @author  : MCD Application Team
  * @brief   : Write/read file on the disk.
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
#include "file_operations.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
FIL MyFile;
FRESULT res;
uint32_t bytesWritten;
uint8_t rtext[200];
uint8_t wtext[] = "USB Host Library : Mass Storage Example";

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Files operations: Read/Write and compare
* @param  None
* @retval Operation result
*/
uint8_t msc_file_operations(void)
{
  uint32_t bytesread;
  uint8_t retvalue = 0;

  USBH_UsrLog("INFO : FatFs Initialized\n");

  if(f_open(&MyFile, "0:USBHost.txt",FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    USBH_ErrLog("Cannot Open 'USBHost.txt' file \n");
    retvalue = 1;
  }
  else
  {
    USBH_UsrLog("Write Text:");
    USBH_UsrLog("INFO : 'USBHost.txt' opened for write");
    res= f_write (&MyFile, wtext, sizeof(wtext), (void *)&bytesWritten);
    f_close(&MyFile);

    if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/
    {
      USBH_ErrLog("Cannot Write on the  'USBHost.txt' file \n");
      retvalue = 1;
    }
    else
    {
      if(f_open(&MyFile, "0:USBHost.txt", FA_READ) != FR_OK)
      {
        USBH_ErrLog("Cannot Open 'USBHost.txt' file for read.\n");
        retvalue = 1;
      }
      else
      {
        USBH_UsrLog("INFO : Text written on the 'USBHost.txt' file\n");

        res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);

        if((bytesread == 0) || (res != FR_OK)) /*EOF or Error*/
        {
          USBH_ErrLog("Cannot Read from the  'USBHost.txt' file \n");
          retvalue = 1;
        }
        else
        {
          USBH_UsrLog("Read Text:");
          USBH_UsrLog((char *)rtext);
          USBH_UsrLog("\n");
        }
        f_close(&MyFile);
      }
      /* Compare read data with the expected data */
      if((bytesread == bytesWritten))
      {
        USBH_UsrLog("INFO : FatFs data compare SUCCES");
      }
      else
      {
        USBH_ErrLog("FatFs data compare ERROR");
        USBH_ErrLog("\n");
        retvalue = 1;
      }
    }
  }
  return (retvalue);
}

