/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : USB_Host/HID_Standalone/USB_Host/App/usb_host.h
  * @author  : MCD Application Team
  * @brief   : Header for usb_host.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_HOST__H__
#define __USB_HOST__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

/* USER CODE BEGIN INCLUDE */
#include "usbh_hid.h"
#include "usbh_hid_parser.h"
/* USER CODE END INCLUDE */

/** @addtogroup USBH_OTG_DRIVER
  * @{
  */

/** @defgroup USBH_HOST USBH_HOST
  * @brief Host file for Usb otg low level driver.
  * @{
  */

/** @defgroup USBH_HOST_Exported_Variables USBH_HOST_Exported_Variables
  * @brief Public variables.
  * @{
  */

/**
  * @}
  */

/** Status of the application. */
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_START,
  APPLICATION_READY,
  APPLICATION_DISCONNECT
}ApplicationTypeDef;

/** USER CODE BEGIN1 **/
typedef enum {
  HID_APP_IDLE = 0,
  HID_APP_WAIT,
  HID_APP_MOUSE,
  HID_APP_KEYBOARD,
}HID_APP_State;
/** USER CODE END1 **/

/**/
/** @defgroup USBH_HOST_Exported_FunctionsPrototype USBH_HOST_Exported_FunctionsPrototype
  * @brief Declaration of public functions for Usb host.
  * @{
  */

/* Exported functions -------------------------------------------------------*/

/** @brief USB Host initialization function. */
void MX_USB_HOST_Init(void);

void MX_USB_HOST_Process(void);
void USBH_HID_AppProcess(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_HOST__H__ */

