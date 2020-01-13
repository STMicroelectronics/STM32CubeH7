/**
  ******************************************************************************
  * @file    net_wifi.h
  * @author  MCD Application Team
  * @brief   Header for the network Wi-Fi class.
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
#ifndef NET_WIFI_H
#define NET_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

#define NET_WIFI_MAC_ADDRESS_SIZE  6
#define NET_WIFI_MAX_SSID_SIZE     32


#define WIFI_SM_OPEN         0x00U
#define WIFI_SM_WEP          0x01U
#define WIFI_SM_WPA_PSK      0x02U
#define WIFI_SM_WPA2_PSK     0x03U
#define WIFI_SM_WPA_WPA2_PSK 0x04U

/* Mode */
typedef enum
{
  NET_WIFI_MODE_STA,
  NET_WIFI_MODE_AP
} net_wifi_mode_t;

/* MAC address */
typedef uint8_t net_wifi_mac_t[NET_WIFI_MAC_ADDRESS_SIZE];

/* SSID */
typedef struct
{
    uint8_t length;
    uint8_t value[NET_WIFI_MAX_SSID_SIZE];
} net_wifi_ssid_t;

/* Scan */
typedef enum
{
    NET_WIFI_SCAN_PASSIVE,
    NET_WIFI_SCAN_ACTIVE,
    NET_WIFI_SCAN_AUTO
} net_wifi_scan_mode_t;

typedef struct net_wifi_scan_bss_s
{
    net_wifi_ssid_t  ssid;
    net_wifi_mac_t   bssid;
    uint8_t          channel;
    uint8_t          country[3];
    uint8_t          rssi;
} net_wifi_scan_bss_t;

typedef struct net_wifi_scan_results_s
{
    uint16_t              number;
    net_wifi_scan_bss_t  *bss;
} net_wifi_scan_results_t;

/* Param */
typedef enum
{
  NET_WIFI_MODE,
} net_wifi_param_t;

/* System info */
typedef enum
{
    NET_WIFI_SCAN_RESULTS_NUMBER,
} net_wifi_system_info_t;

/* Credential configuration */
typedef struct net_wifi_credentials_s
{
    const char *ssid;
    const char *psk;
    uint8_t security_mode;
} net_wifi_credentials_t;

/* Powersave */
typedef enum
{
    WIFI_POWERSAVE_ACTIVE,
    WIFI_POWERSAVE_LIGHT_SLEEP,
    WIFI_POWERSAVE_DEEP_SLEEP
}
net_wifi_powersave_t;




int32_t net_wifi_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode);
int32_t net_wifi_get_scan_results(net_if_handle_t *pnetif, net_wifi_scan_results_t *results, uint8_t number);
int32_t net_wifi_set_credentials(net_if_handle_t *pnetif, const net_wifi_credentials_t *credentials);
int32_t net_wifi_set_powersave(net_if_handle_t *pnetif, const net_wifi_powersave_t *powersave);
int32_t net_wifi_set_param(net_if_handle_t *pnetif, const net_wifi_param_t param, void *data);

#ifdef __cplusplus
}
#endif

#endif /* NET_WIFI_H */
