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
  *                             www.st.com/SLA0044
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

/* Activate CustomHID and CDC classes in composite builder */
#define USBD_CMPSIT_ACTIVATE_CUSTOMHID                     1U
#define USBD_CMPSIT_ACTIVATE_CDC                           1U

/* The definition of endpoint numbers must respect the order of classes instantiation  */
#define CUSTOM_HID_EPIN_ADDR                  0x81U
#define CUSTOM_HID_EPOUT_ADDR                 0x01U  /* EP1 for data OUT */

#define CDC_IN_EP                             0x82U  /* EP1 for data IN */
#define CDC_OUT_EP                            0x02U  /* EP1 for data OUT */
#define CDC_CMD_EP                            0x83U  /* EP2 for CDC commands */

#define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE     0x02
#define USBD_CUSTOM_HID_REPORT_DESC_SIZE      163

/* Common Config */
#define USBD_MAX_NUM_INTERFACES               6
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 0x100U
#define USBD_SUPPORT_USER_STRING              1U
#define USBD_SUPPORT_USER_STRING_DESC         1U
#define USBD_SELF_POWERED                     1U
#define USBD_DEBUG_LEVEL                      0U
#define USBD_CONFIG_STR_DESC_IDX              4U
#define USBD_CONFIG_BMATTRIBUTES              0xC0U
#define USBD_CONFIG_MAXPOWER                  0x32U

/* Exported macro ------------------------------------------------------------*/
/* Memory management macros */
/** Alias for memory allocation. */
#define USBD_malloc         malloc

/** Alias for memory release. */
#define USBD_free           free

/** Alias for memory set. */
#define USBD_memset         memset

/** Alias for memory copy. */
#define USBD_memcpy         memcpy
/** Alias for delay. */
#define USBD_Delay          HAL_Delay

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
void *USBD_static_malloc(uint32_t size);
void USBD_static_free(void *p);

#endif /* __USBD_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
