/**
  ******************************************************************************
  * @file    es_wifi_conf.h
  * @author  MCD Application Team
  * @brief   ES_WIFI configuration file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ES_WIFI_CONF_H
#define __ES_WIFI_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

//#define WIFI_USE_CMSIS_OS

#ifdef WIFI_USE_CMSIS_OS
#include "cmsis_os.h"
extern osMutexId es_wifi_mutex;

#define LOCK_SPI()              osMutexWait(spi_mutex, 0)
#define UNLOCK_SPI()            osMutexRelease(spi_mutex)

#define LOCK_WIFI()             osMutexWait(es_wifi_mutex, 0)
#define UNLOCK_WIFI()           osMutexRelease(es_wifi_mutex)

#define SEM_SIGNAL(a)           osSemaphoreRelease(a)
#define SEM_WAIT(a,timeout)     osSemaphoreWait(a,timeout)

#else /* WIFI_USE_CMSIS_OS */

#define LOCK_SPI()
#define LOCK_WIFI()
#define UNLOCK_SPI()
#define UNLOCK_WIFI()
#define SEM_SIGNAL(a)
#endif /* WIFI_USE_CMSIS_OS */
#include "net_conf_es_wifi_spi.h"

#define ES_WIFI_MAX_SSID_NAME_SIZE                  32
#define ES_WIFI_MAX_PSWD_NAME_SIZE                  32
#define ES_WIFI_PRODUCT_ID_SIZE                     32
#define ES_WIFI_PRODUCT_NAME_SIZE                   32
#define ES_WIFI_FW_REV_SIZE                         24
#define ES_WIFI_API_REV_SIZE                        16
#define ES_WIFI_STACK_REV_SIZE                      16
#define ES_WIFI_RTOS_REV_SIZE                       16

#define ES_WIFI_DATA_SIZE                           1400
#define ES_WIFI_MAX_DETECTED_AP                     20

#define ES_WIFI_TIMEOUT                             0xFFFF

#define ES_WIFI_USE_PING                            1
#define ES_WIFI_USE_AWS                             0
#define ES_WIFI_USE_FIRMWAREUPDATE                  0
#define ES_WIFI_USE_WPS                             0

#define ES_WIFI_USE_SPI                             1
#define ES_WIFI_USE_UART                            (!ES_WIFI_USE_SPI)


#ifdef __cplusplus
}
#endif

#endif /* ES_WIFI_CONF_H */

