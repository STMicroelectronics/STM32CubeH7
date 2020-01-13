/**
  ******************************************************************************
  * @file    net_ip_lwip.h
  * @author  MCD Application Team
  * @brief   Header for the network IP functions.
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

#ifndef NET_IP_LWIP_H
#define NET_IP_LWIP_H

#include "net_connect.h"
#include "net_internals.h"
#include "lwipopts.h"


#define NET_IP_IF_TIMEOUT              1000
#define NET_IP_INPUT_QUEUE_TIMEOUT     1000
#define NET_IP_INPUT_QUEUE_SIZE        128
#define NET_IP_THREAD_SIZE             1024
#define NET_IP_HOSTNAME_MAX_LEN        32

#define NET_IP_FLAG_DEFAULT_INTERFACE        (1<<0)
#define NET_IP_FLAG_TCPIP_STARTED_EXTERNALLY (1<<1)

struct net_ip_if_s
{
    osSemaphoreId connection_semaphore_handle;
    int32_t (*output)(void *context, net_buf_t *p);
    void (*status_cb)(struct net_ip_if_s *net_ip_if);
    void *obj;
    int32_t (*input)(void *context, uint8_t *, uint16_t);
    void (*link_status)(void *context, uint8_t status);

} ;




static int32_t net_ip_add_if(net_if_handle_t *pnetif, int32_t (*if_output)(void *context,net_buf_t *p), uint32_t flag);
static int32_t net_ip_remove_if(net_if_handle_t *pnetif);
static int32_t net_ip_connect(net_if_handle_t *pnetif);
static int32_t net_ip_disconnect(net_if_handle_t *pnetif);



#endif /* NET_IP_LWIP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
