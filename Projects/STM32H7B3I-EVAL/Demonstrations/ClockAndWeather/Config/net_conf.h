/**
  ******************************************************************************
  * @file    net_conf.h
  * @author  MCD Application Team
  * @brief   Configures the network socket APIs.
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
#ifndef __NET_CONF_H
#define __NET_CONF_H


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Comment if MBEDTLS on host side is not used */
/* #define NET_MBEDTLS_HOST_SUPPORT */

/* Uncomment if device supports Secure TCP connection */
/* #define NET_MBEDTLS_DEVICE_SUPPORT */

#ifdef NET_USE_RTOS
#include "cmsis_os.h"
#define NET_CALLOC net_calloc
#define NET_MALLOC pvPortMalloc
#define NET_FREE   vPortFree
#else
#define NET_MALLOC malloc
#define NET_CALLOC calloc
#define NET_FREE  free
#endif /* NET_USE_RTOS */


/* MbedTLS configuration */
#ifdef NET_MBEDTLS_HOST_SUPPORT
#define NET_MBEDTLS_DEBUG_LEVEL 1

#if !defined(MBEDTLS_CONFIG_FILE)
#define MBEDTLS_CONFIG_FILE "mbedtls/config.h"
#endif /* MBEDTLS_CONFIG_FILE */
#endif /* NET_MBEDTLS_HOST_SUPPORT */


#define NET_MAX_SOCKETS_NBR            5

#define NET_IF_NAME_LEN                128
#define NET_DEVICE_NAME_LEN            64
#define NET_DEVICE_ID_LEN              64
#define NET_DEVICE_VER_LEN             64


#define NET_SOCK_DEFAULT_RECEIVE_TO    60000
#define NET_SOCK_DEFAULT_SEND_TO       60000
#define NET_UDP_MAX_SEND_BLOCK_TO      1024
#define NET_USE_DEFAULT_INTERFACE      1

#ifdef  ENABLE_NET_DBG
#define NET_DBG_INFO(...)  
#define NET_DBG_ERROR(...)  
#define NET_DBG_PRINT(...)  
#define NET_ASSERT(test,s)  
#define NET_PRINT(...)  
#define NET_PRINT_WO_CR(...)   
#define NET_WARNING(...)  
#else
#define NET_DBG_INFO(...)                                
#define NET_DBG_ERROR(...)  
#define NET_DBG_PRINT(...)  
#define NET_ASSERT(test,s)  
#define NET_PRINT(...)  

#define NET_PRINT_WO_CR(...)   

#define NET_WARNING(...)                               
#endif /* ENABLE_NET_DBG_INFO */

#ifdef __cplusplus
}
#endif

#endif /* __NET_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
