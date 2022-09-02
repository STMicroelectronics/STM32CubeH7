/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  MCD Application Team
  * @brief   General low level driver configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                     www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF_H
#define __USBD_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Use the USBD Librairy V2.10.0 */
#define USE_USB_DEVICE_LIB_V2_10_0

/* Activate the IAD option */
#define USBD_COMPOSITE_USE_IAD                             1U

/* Activate the composite builder */
#define USE_USBD_COMPOSITE

/* Activate Printer and CDC classes in composite builder */
#define USBD_CMPSIT_ACTIVATE_CDC                           1U
#define USBD_CMPSIT_ACTIVATE_PRINTER                       1U

/* The definition of endpoint numbers must respect the order of classes instantiation  */
#define PRNT_IN_EP                           0x83U  /* EP3 for PRNT data IN */
#define PRNT_OUT_EP                          0x02U  /* EP2 for dPRNT .ata OUT */

#define CDC_IN_EP                             0x81U  /* EP1 for CDC data IN */
#define CDC_OUT_EP                            0x01U  /* EP1 for CDC data OUT */
#define CDC_CMD_EP                            0x82U  /* EP2 for CDC commands */

/* Common Config */
#define USBD_MAX_NUM_INTERFACES               6
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 0x100
#define USBD_SUPPORT_USER_STRING              1
#define USBD_SELF_POWERED                     1
#define USBD_DEBUG_LEVEL                      0
#define USBD_CONFIG_STR_DESC_IDX              4U
#define USBD_CONFIG_BMATTRIBUTES              0xC0U
#define USBD_CONFIG_MAXPOWER                  0x32U

/* Exported macro ------------------------------------------------------------*/
/* Memory management macros */
#define USBD_malloc               malloc
#define USBD_free                 free
#define USBD_memset               memset
#define USBD_memcpy               memcpy

/* DEBUG macros */
#if (USBD_DEBUG_LEVEL > 0)
#define  USBD_UsrLog(...)   printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_UsrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 1)

#define  USBD_ErrLog(...)   printf("ERROR: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_ErrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 2)
#define  USBD_DbgLog(...)   printf("DEBUG : ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_DbgLog(...)
#endif

/* Exported functions ------------------------------------------------------- */
#ifdef USE_USB_FS
extern PCD_HandleTypeDef hpcd_USB_FS;
#endif
#ifdef USE_USB_HS
extern PCD_HandleTypeDef hpcd_USB_HS;
#endif

#endif /* __USBD_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
