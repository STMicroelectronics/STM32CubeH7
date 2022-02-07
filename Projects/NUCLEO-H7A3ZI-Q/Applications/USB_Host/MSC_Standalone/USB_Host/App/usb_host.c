/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : USB_Host/MSC_Standalone/USB_Host/App/usb_host.c
  * @author  : MCD Application Team
  * @brief   : This file implements the USB Host
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

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern char USBDISKPath[4];          /* USB Host logical drive path */
extern FATFS USBDISKFatFs;           /* File system object for USB disk logical drive */
extern void Error_Handler(void);
__IO MSC_APP_State msc_app_state;
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void USBH_Clock_Config(void);
extern FRESULT msc_explore_disk(char *path, uint8_t recu_level);
/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostHS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
/**
  * @brief USB Clock Configuration
  * @retval None
  */
void USBH_Clock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /* PLL3 for USB Clock */
  PeriphClkInitStruct.PLL3.PLL3M = 8;
  PeriphClkInitStruct.PLL3.PLL3N = 336;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 7;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/**
* @brief  Manages MSC Application Process.
* @param  None
* @retval None
*/
void USBH_MSC_AppProcess(void)
{
  switch(msc_app_state)
  {
  case MSC_APP_WAIT:
    msc_app_state = MSC_APP_FILE_OPERATIONS;
    break;

  case MSC_APP_FILE_OPERATIONS:
    /* Read and Write File Here */
    if(Appli_state == APPLICATION_READY)
    {
      USBH_UsrLog("\n\n*** Files operations ***\n");
      msc_app_state = MSC_APP_EXPLORER;
      if(msc_file_operations() != 0)
      {
        msc_app_state = MSC_APP_IDLE;
        return;
      }
    }
    break;

  case MSC_APP_EXPLORER:
    /* Display disk content */
    if(Appli_state == APPLICATION_READY)
    {
      USBH_UsrLog("\n\n*** Explore Disk ***\n\n");
      msc_explore_disk("0:/", 1);
      msc_app_state = MSC_APP_IDLE;
    }
    break;

  default:
    break;
  }
  if(Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    USBH_UsrLog("USB device disconnected !!! \n");
    msc_app_state = MSC_APP_WAIT;
  }
}
/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  USBH_Clock_Config();

  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostHS, USBH_UserProcess, HOST_HS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostHS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostHS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  USBH_UsrLog(" **** USB OTG HS in FS MSC Host **** \n");
  USBH_UsrLog("USB Host library started.\n");

  /* Initialize Application and MSC process */

  USBH_UsrLog("Starting MSC Application");
  USBH_UsrLog("Connect your MSC Device\n");
  msc_app_state = MSC_APP_WAIT;

  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostHS);

  /* MSC Application Process */
  USBH_MSC_AppProcess();
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_CONNECTION:
  break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    if (f_mount(NULL, "", 0) != FR_OK)
    {
      USBH_UsrLog("ERROR : Cannot DeInitialize FatFs! \n");
    }
    if (FATFS_UnLinkDriver(USBDISKPath) != 0)
    {
      USBH_UsrLog("ERROR : Cannot UnLink FatFS Driver! \n");
    }
    break;

  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_READY;
    if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath)==0)
    {
      if (f_mount(&USBDISKFatFs, "", 0)!= FR_OK)
      {
        USBH_UsrLog("ERROR : Cannot Initialize FatFs! \n");
      }
    }
    break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

