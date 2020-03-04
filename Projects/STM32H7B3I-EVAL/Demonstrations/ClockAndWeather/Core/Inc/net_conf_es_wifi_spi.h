/**
  ******************************************************************************
  * @file    net_conf_es_wif_spi.h
  * @author  MCD Application Team
  * @brief   This file provides the configuration for net
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
