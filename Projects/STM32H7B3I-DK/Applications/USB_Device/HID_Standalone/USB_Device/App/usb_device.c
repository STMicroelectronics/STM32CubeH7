/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/USB_Device/App/usb_device.c
  * @author  MCD Application Team
  * @brief   This file implements the USB Device
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

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
__IO uint32_t remotewakeupon = 0;
uint8_t HID_Buffer[4];
extern PCD_HandleTypeDef hpcd_USB_HS;
#define CURSOR_STEP     5
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void GetPointerData(uint8_t *pbuf);
extern void SystemClockConfig_Resume(void);
/* USER CODE END PFP */

extern void Error_Handler(void);
/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceHS;
extern USBD_DescriptorsTypeDef HID_Desc;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/**
  * @brief  Gets Pointer Data.
  * @param  pbuf: Pointer to report
  * @retval None
  */
void GetPointerData(uint8_t * pbuf)
{
  static int8_t cnt = 0;
  int8_t x = 0, y = 0;

  if (cnt++ > 0)
  {
    x = CURSOR_STEP;
  }
  else
  {
    x = -CURSOR_STEP;
  }
  pbuf[0] = 0;
  pbuf[1] = x;
  pbuf[2] = y;
  pbuf[3] = 0;
}

/**
  * @brief  GPIO EXTI Callback function
  *         Handle remote-wakeup through key button
  * @param  GPIO_Pin
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_KEY_PIN)
  {
    if ((((USBD_HandleTypeDef *) hpcd_USB_HS.pData)->dev_remote_wakeup == 1) &&
        (((USBD_HandleTypeDef *) hpcd_USB_HS.pData)->dev_state ==
         USBD_STATE_SUSPENDED))
    {
      if ((&hpcd_USB_HS)->Init.low_power_enable)
      {
        HAL_ResumeTick();
        SystemClockConfig_Resume();
      }
      /* Activate Remote wakeup */
      HAL_PCD_ActivateRemoteWakeup((&hpcd_USB_HS));

      /* Remote wakeup delay */
      HAL_Delay(10);

      /* Disable Remote wakeup */
      HAL_PCD_DeActivateRemoteWakeup((&hpcd_USB_HS));

      /* change state to configured */
      ((USBD_HandleTypeDef *) hpcd_USB_HS.pData)->dev_state = USBD_STATE_CONFIGURED;

      /* Change remote_wakeup feature to 0 */
      ((USBD_HandleTypeDef *) hpcd_USB_HS.pData)->dev_remote_wakeup = 0;
      remotewakeupon = 1;
    }
    else if (((USBD_HandleTypeDef *) hpcd_USB_HS.pData)->dev_state ==
         USBD_STATE_CONFIGURED)
    {
      GetPointerData(HID_Buffer);
      USBD_HID_SendReport(&hUsbDeviceHS, HID_Buffer, 4);
    }
    else
    {
      /* ... */
    }
  }
}

/* USER CODE END 1 */

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(void)
{
  /* USER CODE BEGIN USB_Device_Init_PreTreatment */
  /* USER CODE END USB_Device_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceHS, &HID_Desc, DEVICE_HS) != USBD_OK) {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceHS, &USBD_HID) != USBD_OK) {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceHS) != USBD_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Device_Init_PostTreatment */

  /* USER CODE END USB_Device_Init_PostTreatment */
}

/**
  * @}
  */

/**
  * @}
  */

