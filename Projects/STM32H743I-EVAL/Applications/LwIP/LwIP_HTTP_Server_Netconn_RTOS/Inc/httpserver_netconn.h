/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/httpserver-netconn.h 
  * @author  MCD Application Team
  * @brief   Header for httpserver-netconn.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#include "lwip/api.h"

void http_server_netconn_init(void);
void DynWebPage(struct netconn *conn);

#endif /* __HTTPSERVER_NETCONN_H__ */

