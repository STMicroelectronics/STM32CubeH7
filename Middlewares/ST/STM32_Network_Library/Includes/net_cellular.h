/**
  ******************************************************************************
  * @file    net_cellular.h
  * @author  MCD Application Team
  * @brief   Header for the network Cellular class.
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
#ifndef NET_CELLULAR_H
#define NET_CELLULAR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Network radio results */
typedef struct net_cellular_radio_results_s
{
    int8_t  signal_level_db;
} net_cellular_radio_results_t;

/* Credential configuration */
typedef struct net_cellular_credentials_s
{
    const char *apn;
    const char *username;
    const char *password;
    bool use_internal_sim;
} net_cellular_credentials_t;

/* network extension for Cellular class interface */
int32_t net_cellular_set_credentials(net_if_handle_t *pnetif, const net_cellular_credentials_t *credentials);
int32_t net_cellular_get_radio_results(net_if_handle_t *pnetif, net_cellular_radio_results_t *results);
/* Declaration of cellular network interface constructor */
int32_t cellular_net_driver(net_if_handle_t *pnetif);

#ifdef __cplusplus
}
#endif

#endif /* NET_CELLULAR_H */
