/**
  ******************************************************************************
  * @file    mbedTLS/SSL_Client/Inc/ethernetif.h
  * @author  MCD Application Team
  * @brief   Header for ethernetif.c module
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

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"

/* Exported types ------------------------------------------------------------*/
/* Structure that include link thread parameters */
/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);      
void ethernet_link_thread( void const * argument );
#endif
