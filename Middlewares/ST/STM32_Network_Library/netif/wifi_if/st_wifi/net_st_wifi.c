/**
  ******************************************************************************
  * @file    net_st_wifi.c
  * @author  MCD Application Team
  * @brief   ST Wi-Fi specific BSD-like socket wrapper
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

#include "net_connect.h"
#include "net_internals.h"
#include "net_buffers.h"
#include "net_wifi.h"
#include "wifi.h"

/* Wi-Fi Driver */
extern wifi_driver_t stw_driver;
wifi_driver_t *wifi_driver = &stw_driver;

int32_t st_wifi_driver(net_if_handle_t *pnetif);

/* STM32 Network library Framework */
static int32_t st_wifi_if_init(net_if_handle_t *pnetif);
static int32_t st_wifi_if_deinit(net_if_handle_t *pnetif);
static int32_t st_wifi_if_start(net_if_handle_t *pnetif);
static int32_t st_wifi_if_stop(net_if_handle_t *pnetif);
static int32_t st_wifi_if_connect(net_if_handle_t *pnetif);
static int32_t st_wifi_if_disconnect(net_if_handle_t *pnetif);
static int32_t st_wifi_if_powersave_enable(net_if_handle_t *pnetif);
static int32_t st_wifi_if_powersave_disable(net_if_handle_t *pnetif);

/*static int32_t st_wifi_if_sleep(net_if_handle_t * pnetif); */
/*static int32_t st_wifi_if_wakeup(net_if_handle_t * pnetif); */

/* Class extension */
static int32_t st_wifi_scan(net_wifi_scan_mode_t mode);
static int32_t st_wifi_get_scan_results(net_wifi_scan_results_t *results, uint8_t number);
static int32_t st_wifi_get_system_info(net_wifi_system_info_t info, void *data);
static int32_t st_wifi_set_param(net_wifi_param_t info, void *data);
int st_wifi_event_callback(void *context, wifi_event_t event, void *data);

/* TCP/IP Library */
extern iplib_t iplib_lwip;
static  iplib_t *iplib = &iplib_lwip;

static int32_t st_wifi_transmit(net_buf_t *net_buf);
/*******************************************************************************
    Wi-Fi Driver
  *******************************************************************************/
int32_t st_wifi_driver(net_if_handle_t *pnetif)
{
    return st_wifi_if_init(pnetif);
}

/*******************************************************************************
    STM32 Network library Framework
  *******************************************************************************/

/* Init */
int32_t  st_wifi_if_init(net_if_handle_t *pnetif)
{
    int32_t ret;
    net_if_drv_t *p = NET_MALLOC(sizeof(net_if_drv_t));

    if (p)
    {
        p->if_class = NET_INTERFACE_CLASS_WIFI;
        p->if_init = st_wifi_if_init;
        p->if_deinit = st_wifi_if_deinit;
        p->if_start = st_wifi_if_start;
        p->if_stop = st_wifi_if_stop;
        p->if_connect = st_wifi_if_connect;
        p->if_disconnect = st_wifi_if_disconnect;
        p->if_powersave_enable = st_wifi_if_powersave_enable;
        p->if_powersave_disable = st_wifi_if_powersave_disable;

        p->ping = icmp_ping;

        p->extension.wifi = NET_MALLOC(sizeof(net_if_wifi_class_extension_t));

        if (NULL == p->extension.wifi)
        {
            NET_DBG_ERROR("can't allocate memory for st_wifi_driver class\n");
            NET_FREE(p);
            ret = NET_ERROR_NO_MEMORY;
        }
        else
        {
            pnetif->pdrv = p;
            p->extension.wifi->scan = st_wifi_scan;
            p->extension.wifi->get_scan_results = st_wifi_get_scan_results;
            p->extension.wifi->get_system_info = st_wifi_get_system_info;
            p->extension.wifi->set_param = st_wifi_set_param;
            (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
            ret = NET_OK;
        }
    }
    else
    {
        NET_DBG_ERROR("can't allocate memory for st_wifi_driver class\n");
        ret = NET_ERROR_NO_MEMORY;
    }

    if (wifi_probe(&pnetif->pdrv->context) == NET_OK)
    {

        if (wifi_init(wifi_driver, st_wifi_event_callback, NULL, pnetif) != WIFI_STATUS_OK)
        {
            ret = NET_ERROR_INTERFACE_FAILURE;
        }
    }
    else
    {
        ret = NET_ERROR_DEVICE_ERROR;
    }

    return ret;
}

/* Deinit */
int32_t  st_wifi_if_deinit(net_if_handle_t *pnetif)
{
    int32_t ret = NET_OK;

    if (wifi_deinit(wifi_driver) != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_INTERFACE_FAILURE;
    }
    else
    {
        ret = NET_OK;
    }

    NET_FREE(pnetif->pdrv->extension.wifi);
    pnetif->pdrv->extension.wifi = NULL;
    NET_FREE(pnetif->pdrv);
    pnetif->pdrv = NULL;

    return ret;
}

/* Start */
int32_t st_wifi_if_start(net_if_handle_t *pnetif)
{
    int32_t ret = NET_OK;

    net_wifi_powersave_t wifi_powersave =
    {
        WIFI_POWERSAVE_LIGHT_SLEEP
    };

    if (wifi_start() != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_INTERFACE_FAILURE;
    }
    else
    {
      // STA mode
      if (wifi_obj->mode == WIFI_MODE_STA)
      {
          /* Retrieve Wi-Fi device information */
          char device_name[NET_DEVICE_NAME_LEN];
          wifi_get_system_info(WIFI_SYSINFO_DEVICE_NAME, (void *)device_name);
          strncpy(pnetif->DeviceName, device_name, NET_DEVICE_NAME_LEN);
          wifi_get_system_info(WIFI_SYSINFO_WLAN_MAC, pnetif->macaddr.mac);

          /* Set default values */
          pnetif->pdrv->extension.wifi->powersave = &wifi_powersave;

      }
      // AP mode
      if (wifi_obj->mode == WIFI_MODE_AP)
      {
        const net_wifi_credentials_t* credentials =  pnetif->pdrv->extension.wifi->credentials;
        wifi_privacy_t privacy;

        switch(credentials->security_mode)
        {
          case WIFI_SM_OPEN :
            privacy = WIFI_PRIVACY_NONE;
            break;
          case WIFI_SM_WEP :
            privacy = WIFI_PRIVACY_WEP;
            break;
          case WIFI_SM_WPA_PSK  :
          case WIFI_SM_WPA2_PSK :
          case WIFI_SM_WPA_WPA2_PSK :
            privacy = WIFI_PRIVACY_WPA;
            break;
          default:
            privacy = WIFI_PRIVACY_NONE;
        }
        wifi_ap_start((char *)credentials->ssid, (char *)credentials->psk, privacy, 3);
      }

      /* Add IP interface */
      ret = iplib->add_if(pnetif, st_wifi_transmit, NET_ETHERNET_FLAG_DEFAULT_IF);
      if (ret == NET_OK)
      {
         if (wifi_obj->mode == WIFI_MODE_STA)
         {
            (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
         }
      }
    }
    return ret;
}

/* Stop */
int32_t  st_wifi_if_stop(net_if_handle_t *pnetif)
{
    int32_t ret = NET_OK;


    if (wifi_stop() != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_INTERFACE_FAILURE;
    }
    if (ret == NET_OK)
    {
              ret =     net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
    }
    return ret;
}

/* Connect */
static int32_t st_wifi_if_connect(net_if_handle_t *pnetif)
{
    int32_t ret = NET_OK;

    const net_wifi_credentials_t *credentials =  pnetif->pdrv->extension.wifi->credentials;

    wifi_privacy_t privacy;

    switch (credentials->security_mode)
    {
    case WIFI_SM_OPEN :
        privacy = WIFI_PRIVACY_NONE;
        break;
    case WIFI_SM_WEP :
        privacy = WIFI_PRIVACY_WEP;
        break;
    case WIFI_SM_WPA_PSK  :
    case WIFI_SM_WPA2_PSK :
    case WIFI_SM_WPA_WPA2_PSK :
        privacy = WIFI_PRIVACY_WPA;
        break;
    default:
        privacy = WIFI_PRIVACY_NONE;
    }

    if (wifi_connect((char *)credentials->ssid, (char *)credentials->psk, privacy, 0) != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_AUTH_FAILURE;
    }
    else
    {
        ret = iplib->connect(pnetif);
    }
    return ret;
}

/* Disconnect */
static int32_t st_wifi_if_disconnect(net_if_handle_t *pnetif)
{
    int32_t ret = NET_ERROR_FRAMEWORK;


    if (wifi_disconnect() != WIFI_STATUS_OK)
    {
        ret =  NET_ERROR_FRAMEWORK;
    }
    else
    {
              ret = net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);

        ret = NET_OK;
    }
    return ret;
}

/* Power-save */
static int32_t st_wifi_if_powersave_enable(net_if_handle_t *pnetif)
{
    int32_t ret = NET_OK;
    wifi_power_mode(*(pnetif->pdrv->extension.wifi->powersave));
    return ret;
}

static int32_t st_wifi_if_powersave_disable(net_if_handle_t *pnetif)
{
    int32_t ret = NET_OK;
    wifi_power_mode(WIFI_POWER_MODE_ACTIVE);

    return ret;
}

/*******************************************************************************
    Class extension
  *******************************************************************************/

/* Scan */
static int32_t st_wifi_scan(net_wifi_scan_mode_t mode)
{

    int32_t ret = NET_ERROR_FRAMEWORK;

    wifi_scan_t params;

    switch (mode)
    {
    case NET_WIFI_SCAN_PASSIVE:
        params.mode = WIFI_SCAN_MODE_PASSIVE;
        break;
    case NET_WIFI_SCAN_ACTIVE:
        params.mode = WIFI_SCAN_MODE_ACTIVE;
        break;
    case NET_WIFI_SCAN_AUTO:
        params.mode = WIFI_SCAN_MODE_AUTO;
        break;
    default:
        params.mode = WIFI_SCAN_MODE_AUTO;
    }

    params.ssid.length = 0;
    params.channels = 0;

    if (wifi_scan(params) != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_GENERIC;
    }
    else
    {
        ret = NET_OK;
    }

    return ret;
}

/* Get scan results */
static int32_t st_wifi_get_scan_results(net_wifi_scan_results_t *results, uint8_t number)
{

    int32_t ret = NET_ERROR_FRAMEWORK;

    uint8_t nb;

    wifi_get_system_info(WIFI_SYSINFO_LAST_SCAN_RESULTS_NUMBER, (void *)&nb);
    if (nb > number)
    {
        nb = number;
    }

    wifi_scan_results_t res;
    res.number = nb;
    res.bss = NET_MALLOC(number * sizeof(wifi_scan_bss_t));

    if (wifi_get_scan_results(&res, nb) == WIFI_STATUS_OK)
    {

        results->number = nb;
        for (uint8_t i = 0; i < nb; i = i + 1)
        {
            results->bss[i].ssid.length = res.bss[i].ssid.length;
            memcpy(results->bss[i].ssid.value, res.bss[i].ssid.value, res.bss[i].ssid.length);
            memcpy(results->bss[i].bssid, res.bss[i].bssid, sizeof(wifi_mac_t));
            results->bss[i].channel = res.bss[i].channel;
            memcpy(results->bss[i].country, res.bss[i].country, 3);
            results->bss[i].rssi = res.bss[i].rssi;
        }
        ret = NET_OK;
    }
    else
    {
        ret = NET_ERROR_GENERIC;
    }
    NET_FREE((void *)(res.bss));

    return ret;
}

/* Get system info */
static int32_t st_wifi_get_system_info(net_wifi_system_info_t info, void *data)
{
    int32_t ret = NET_OK;
    wifi_system_info_t wifi_info;

    switch (info)
    {
    case NET_WIFI_SCAN_RESULTS_NUMBER:
        wifi_info = WIFI_SYSINFO_LAST_SCAN_RESULTS_NUMBER;
        break;
    default:
        return NET_ERROR_PARAMETER;
        break;
    }

    if (wifi_get_system_info(wifi_info, data) != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_PARAMETER;
    }
    else
    {
        ret = NET_OK;
    }
    return ret;
}

/*  Set param */
static int32_t st_wifi_set_param(net_wifi_param_t param, void *data)
{
    int32_t ret = NET_OK;
    wifi_params_t wifi_param;
    void *wifi_data;
    switch (param)
    {
    case NET_WIFI_MODE:
      { wifi_param = WIFI_MODE;
        wifi_data = wifi_os_malloc(1);
        switch (*(uint8_t *)data) {
          case NET_WIFI_MODE_STA:
            *(uint8_t *)wifi_data = WIFI_MODE_STA;
            break;
          case NET_WIFI_MODE_AP:
            *(uint8_t *)wifi_data = WIFI_MODE_AP;
            break;
          default:
            return NET_ERROR_PARAMETER;
            break;
        }
        break;
    }
    default:
        return NET_ERROR_PARAMETER;
        break;
    }

    if (wifi_set_param(wifi_param, wifi_data) != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_PARAMETER;
    }
    else
    {
        ret = NET_OK;
    }
    wifi_os_free(wifi_data);
    return ret;
}

/* Event callback */
int st_wifi_event_callback(void *context, wifi_event_t event, void *data)
{

    net_if_handle_t *pnetif = context;

    switch (event)
    {
    case WIFI_EVENT_LINK_DOWN:
      if (wifi_obj->mode == WIFI_MODE_STA)
        pnetif->iplib_notify_link_change(pnetif, 0);
        break;
    case WIFI_EVENT_LINK_UP:
      if (wifi_obj->mode == WIFI_MODE_STA)
        pnetif->iplib_notify_link_change(pnetif, 1);
      if (wifi_obj->mode == WIFI_MODE_AP)
        (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        net_if_notify(pnetif, NET_EVENT_WIFI, WIFI_SCAN_RESULTS_READY, (void *)data);
        break;
    case WIFI_EVENT_POWER_MODE_COMPLETE:
        net_if_notify(pnetif, NET_EVENT, NET_EVENT_POWERSAVE_ENABLED, (void *)data);
        break;
    case WIFI_EVENT_RX_DATA:
    {
        wifi_rx_data_t *eth_frame = (wifi_rx_data_t *)data;
        net_buf_t *buf = NET_BUF_ALLOC(eth_frame->len + sizeof(wifi_eth_t));
        if (buf == NULL)
        {
            wifi_os_delay(1);
        }
        wifi_eth_receive((uint8_t *)(buf->payload), data);
        pnetif->iplib_input_data(pnetif, buf);
        break;
    }
    default:
        break;
    }
    return 0;
}

/*******************************************************************************
    TCP/IP Library
  *******************************************************************************/

/* Transmit */
static int32_t st_wifi_transmit(net_buf_t *net_buf)
{
    int32_t ret = NET_OK;

    if (wifi_eth_transmit((uint8_t *)(net_buf->payload), (uint16_t)(net_buf->len)) != WIFI_STATUS_OK)
    {
        ret = NET_ERROR_DATA;
    }

    return ret;
}
