/**
  ******************************************************************************
  * @file    net_ip_ethernet.h
  * @author  MCD Application Team
  * @brief   Header for the network interface on ethernet
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

#ifndef NET_IP_ETHERNET_H
#define NET_IP_ETHERNET_H
#include "net_connect.h"
#include "net_internals.h"

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* Exported functions ------------------------------------------------------- */
void net_ethernetif_init(void *context, int32_t (*ethernetif_rx_cb)(void *context,  net_buf_t *p),
                         void (*ethernetif_link_status_cb)(void *context, uint8_t status));
void net_ethernetif_deinit(void);
void net_ethernetif_get_mac_addr(uint8_t *mac_addr);
uint8_t net_ethernetif_get_link_status(void);
int32_t net_ethernetif_output(void *context,net_buf_t *net_buf);
#endif /* NET_IP_ETHERNET_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
