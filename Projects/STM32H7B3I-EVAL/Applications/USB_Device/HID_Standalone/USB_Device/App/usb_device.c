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
void USBD_Clock_Config(void);
/* USER CODE END PFP */

extern void Error_Handler(void);
/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceHS;
extern USBD_DescriptorsTypeDef HID_Desc;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
/**
  * @brief USB Clock Configuration
  * @retval None
  */
void USBD_Clock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_CRSInitTypeDef RCC_CRSInitStruct= {0};

  /* Enable HSI48 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }
  /*Configure the clock recovery system (CRS)**********************************/

  /*Enable CRS Clock*/
  __HAL_RCC_CRS_CLK_ENABLE();

  /* Default Synchro Signal division factor (not divided) */
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;

  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB1;

  /* HSI48 is synchronized with USB SOF at 1KHz rate */
  RCC_CRSInitStruct.ReloadValue =  RCC_CRS_RELOADVALUE_DEFAULT;
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;
  RCC_CRSInitStruct.Polarity = RCC_CRS_SYNC_POLARITY_RISING;

  /* Set the TRIM[5:0] to the default value */
  RCC_CRSInitStruct.HSI48CalibrationValue = RCC_CRS_HSI48CALIBRATION_DEFAULT;

  /* Start automatic synchronization */
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);
}
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
  /* USB Clock Initialization */
   USBD_Clock_Config();
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

