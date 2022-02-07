/**
  ******************************************************************************
  * @file    net_conf_es_wif_spi.h
  * @author  MCD Application Team
  * @brief   This file provides the configuration for net
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
#ifndef __NET_CONF_ES_WIFI_SPI_H
#define __NET_CONF_ES_WIFI_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "net_conf.h"
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifdef GENERATOR_WAKAAMACLIENT_CLOUD
#undef NET_MBEDTLS_HOST_SUPPORT
#endif

/* to use Inventek Wifi native TLS */
#if 0
#undef NET_MBEDTLS_HOST_SUPPORT
#define NET_MBEDTLS_WIFI_MODULE_SUPPORT
#endif
/* Private functions ---------------------------------------------------------*/
int32_t wifi_probe(void **ll_drv_obj);
void    SPI_WIFI_ISR(void);
void IO_Callback(void);





#ifdef __cplusplus
}
#endif

#endif /* __NET_CONF_ES_WIFI_SPI_H */

