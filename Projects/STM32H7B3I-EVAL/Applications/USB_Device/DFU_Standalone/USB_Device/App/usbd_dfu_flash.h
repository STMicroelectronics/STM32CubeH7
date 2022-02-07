/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           USB_Device/DFU_Standalone/USB_Device/App/usbd_dfu_flash.h
  * @author         MCD Application Team
  * @brief          Header for usbd_dfu_flash.c file.
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
#ifndef __USBD_DFU_FLASH_H__
#define __USBD_DFU_FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_MEDIA USBD_MEDIA
  * @brief Header file for the usbd_dfu_flash.h file.
  * @{
  */

/** @defgroup USBD_MEDIA_Exported_Defines USBD_MEDIA_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_MEDIA_Exported_Types USBD_MEDIA_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_MEDIA_Exported_Macros USBD_MEDIA_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */
#define FLASH_END_ADDR       (uint32_t)(0x081FFFFF)
/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_MEDIA_Exported_Variables USBD_MEDIA_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** MEDIA Interface callback. */
extern USBD_DFU_MediaTypeDef USBD_DFU_Flash_fops;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MEDIA_Exported_FunctionsPrototype USBD_MEDIA_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

/* USER CODE END EXPORTED_FUNCTIONS */

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

#endif /* __USBD_DFU_FLASH_H__ */

