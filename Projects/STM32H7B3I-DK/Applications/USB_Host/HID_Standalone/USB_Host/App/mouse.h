/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : USB_Host/HID_Standalone/USB_Host/App/mouse.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOUSE_H
#define __MOUSE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_host.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HID_MOUSE_App(USBH_HandleTypeDef *phost);

#endif /* __MOUSE_H */

