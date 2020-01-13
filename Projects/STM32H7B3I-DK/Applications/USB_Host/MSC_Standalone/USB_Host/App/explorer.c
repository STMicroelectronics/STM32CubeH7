/**
******************************************************************************
  * @file    : USB_Host/MSC_Standalone/USB_Host/App/explorer.c
  * @author  : MCD Application Team
  * @brief   : Explore the USB flash disk content
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "explorer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USBH_HandleTypeDef hUsbHostHS;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Displays disk content.
* @param  path: Pointer to root path
* @param  recu_level: Disk content level
* @retval Operation result
*/
FRESULT msc_explore_disk(char *path, uint8_t recu_level)
{
  FRESULT res = FR_OK;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[14];

  res = f_opendir(&dir, path);
  if(res == FR_OK)
  {
    while(USBH_MSC_IsReady(&hUsbHostHS))
    {
      res = f_readdir(&dir, &fno);
      if(res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if(fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;
      strcpy(tmp, fn);

      if(recu_level == 1)
      {
        USBH_UsrLog("   |__");
      }
      else if(recu_level == 2)
      {
        USBH_UsrLog("   |   |__");
      }
      if((fno.fattrib & AM_DIR) == AM_DIR)
      {
        strcat(tmp, "\n");
        USBH_UsrLog((void *)tmp);
        msc_explore_disk(fn, 2);
      }
      else
      {
        strcat(tmp, "\n");
        USBH_UsrLog((void *)tmp);
      }

      if(((fno.fattrib & AM_DIR) == AM_DIR)&&(recu_level == 2))
      {
        msc_explore_disk(fn, 2);
      }
    }
    f_closedir(&dir);
  }
  return res;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
