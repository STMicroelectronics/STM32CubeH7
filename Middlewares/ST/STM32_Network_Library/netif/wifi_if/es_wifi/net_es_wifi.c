/**
  ******************************************************************************
  * @file    net_es_wifi.c
  * @author  MCD Application Team
  * @brief   ES Wi-Fi specific BSD-like socket wrapper
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
#include "es_wifi.h"

#define ESWIFI_MAX_CHANNEL_NBR           4

#define WIFI_FREE_SOCKET                 0U
#define WIFI_ALLOCATED_SOCKET            1U
#define WIFI_BIND_SOCKET                 2U
#define WIFI_SEND_OK                     4U
#define WIFI_RECV_OK                     8U
#define WIFI_CONNECTED_SOCKET            16U
#define WIFI_STARTED_CLIENT_SOCKET       32U
#define WIFI_STARTED_SERVER_SOCKET       64U
#define WIFI_CONNECTED_SOCKET_RW         (WIFI_CONNECTED_SOCKET | WIFI_SEND_OK | WIFI_RECV_OK)

/* Declaration of generic class functions               */

int32_t es_wifi_driver(net_if_handle_t *pnetif);

static int32_t es_wifi_if_init(net_if_handle_t *pnetif);
static int32_t es_wifi_if_deinit(net_if_handle_t *pnetif);

static int32_t es_wifi_if_start(net_if_handle_t *pnetif);
static int32_t es_wifi_if_stop(net_if_handle_t *pnetif);

static int32_t es_wifi_if_connect(net_if_handle_t *pnetif);
static int32_t es_wifi_if_disconnect(net_if_handle_t *pnetif);

static int32_t es_wifi_socket(int32_t domain, int32_t type, int32_t protocol);
static int32_t es_wifi_bind(int32_t sock, const sockaddr_t *addr, int32_t addrlen);
static int32_t es_wifi_listen(int32_t sock, int32_t backlog);
static int32_t es_wifi_accept(int32_t sock, sockaddr_t *addr, int32_t *addrlen);
static int32_t es_wifi_connect(int32_t sock, const sockaddr_t *addr, int32_t addrlen);
static int32_t es_wifi_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t es_wifi_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t es_wifi_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *to, int32_t tolen);
static int32_t es_wifi_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *from,
                                int32_t *fromlen);
static int32_t es_wifi_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue, int32_t optlen);
static int32_t es_wifi_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, int32_t *optlen);
static int32_t es_wifi_getsockname(int32_t sock, sockaddr_t *name, int32_t *namelen);
static int32_t es_wifi_getpeername(int32_t sock, sockaddr_t *name, int32_t *namelen);
static int32_t es_wifi_close(int32_t sock, bool isaclone);
static int32_t es_wifi_shutdown(int32_t sock, int32_t mode);

static int32_t es_wifi_gethostbyname(net_if_handle_t *pnetif, sockaddr_t *addr, char_t *name);
static int32_t es_wifi_ping(net_if_handle_t *pnetif, sockaddr_t *addr, int32_t count, int32_t delay,
                            int32_t response[]);

/* Declaration and definition of class-specific functions */
static void check_connection_lost(net_if_handle_t *pnetif, int32_t n);
static  int32_t  es_wifi_scan(net_wifi_scan_mode_t mode);

/* internal structure to mabage es_wfi socket */
typedef struct eswifi_tls_data_s
{
    char *tls_ca_certs; /**< Socket option. */
    char *tls_ca_crl;   /**< Socket option. */
    char *tls_dev_cert; /**< Socket option. */
    char *tls_dev_key;  /**< Socket option. */
    uint8_t *tls_dev_pwd;        /**< Socket option. */
    bool tls_srv_verification;   /**< Socket option. */
    char *tls_srv_name;          /**< Socket option. */
} eswifi_tls_data_t;

typedef struct WIFI_Channel_s
{
    uint8_t  Number;
    uint8_t  status;
    uint8_t  protocol;
    uint8_t  type;
    uint16_t localport;
    ipaddr_t localaddress;
    uint32_t maxpconnections;
    int32_t  blocking;
    int32_t  sendtimeout;
    int32_t  recvtimeout;
    net_if_notify_func notify_callback;
    void          *notify_context;
    net_if_handle_t  *pnetif;

#ifdef NET_MBEDTLS_WIFI_MODULE_SUPPORT
    eswifi_tls_data_t        tlsData;
#endif /* NET_MBEDTLS_WIFI_MODULE_SUPPORT */

} WIFI_Channel_t;

static WIFI_Channel_t WifiChannel[ESWIFI_MAX_CHANNEL_NBR] = {.0};

int32_t es_wifi_driver(net_if_handle_t *pnetif)
{
    return es_wifi_if_init(pnetif);
}


/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
int32_t  es_wifi_if_init(net_if_handle_t *pnetif)
{
    int32_t ret;
    net_if_drv_t *p = NET_MALLOC(sizeof(net_if_drv_t));
    if (p != NULL)
    {
        p->if_class = NET_INTERFACE_CLASS_WIFI;

        p->if_init = es_wifi_if_init;
        p->if_deinit = es_wifi_if_deinit;

        p->if_start = es_wifi_if_start;
        p->if_stop = es_wifi_if_stop;

        p->if_connect = es_wifi_if_connect;
        p->if_disconnect = es_wifi_if_disconnect;

        p->socket = es_wifi_socket;
        p->bind = es_wifi_bind;
        p->listen = es_wifi_listen;
        p->accept = es_wifi_accept;
        p->connect = es_wifi_connect;
        p->send = es_wifi_send;
        p->recv = es_wifi_recv;
        p->sendto = es_wifi_sendto;
        p->recvfrom = es_wifi_recvfrom;
        p->setsockopt = es_wifi_setsockopt;
        p->getsockopt = es_wifi_getsockopt;
        p->getsockname = es_wifi_getsockname;
        p->getpeername = es_wifi_getpeername;
        p->close = es_wifi_close;
        p->shutdown = es_wifi_shutdown;

        p->gethostbyname = es_wifi_gethostbyname;
        p->ping = es_wifi_ping;
        p->extension.wifi = NET_MALLOC(sizeof(net_if_wifi_class_extension_t));
        if (NULL == p->extension.wifi)
        {
            NET_DBG_ERROR("can't allocate memory for es_wifi_driver class\n");
            NET_FREE(p);
            ret = NET_ERROR_NO_MEMORY;
        }
        else
        {
            pnetif->pdrv = p;
            p->extension.wifi->scan = es_wifi_scan;
            net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);

            ret = NET_OK;
        }
    }
    else
    {
        NET_DBG_ERROR("can't allocate memory for es_wifi_driver class\n");
        ret = NET_ERROR_NO_MEMORY;
    }
    return ret;
}

static int32_t es_wifi_if_deinit(net_if_handle_t *pnetif)
{
    NET_FREE(pnetif->pdrv->extension.wifi);
    pnetif->pdrv->extension.wifi = NULL;
    NET_FREE(pnetif->pdrv);
    pnetif->pdrv = NULL;
    return NET_OK;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
int32_t es_wifi_if_start(net_if_handle_t *pnetif)
{
    int32_t ret;

    ES_WIFIObject_t  *pEsWifiObj;

    if (wifi_probe(&pnetif->pdrv->context) == NET_OK)
    {
        pEsWifiObj = pnetif->pdrv->context;
        /* Init the WiFi module */
        if (NET_OK != ES_WIFI_Init(pEsWifiObj))
        {
            ret = NET_ERROR_INTERFACE_FAILURE;
        }
        else
        {
            /* Retrieve the WiFi module information */
            if (pEsWifiObj->Product_Name != NULL)
            {
                strncpy(pnetif->DeviceName, (char *)pEsWifiObj->Product_Name, NET_DEVICE_NAME_LEN);
            }
            if (pEsWifiObj->Product_ID != NULL)
            {
                strncpy(pnetif->DeviceID, (char *)pEsWifiObj->Product_ID, NET_DEVICE_ID_LEN);
            }
            if (pEsWifiObj->FW_Rev != NULL)
            {
                strncpy(pnetif->DeviceVer, (char *)pEsWifiObj->FW_Rev, NET_DEVICE_VER_LEN);
            }

            ES_WIFI_GetMACAddress(pEsWifiObj, pnetif->macaddr.mac);


            /* Initialise the Channels*/
            for (int32_t i = 0; i < ESWIFI_MAX_CHANNEL_NBR; i++)
            {
                WifiChannel[i].status          = WIFI_FREE_SOCKET;
                WifiChannel[i].Number          = (uint32_t) i;
                WifiChannel[i].recvtimeout     = MIN(NET_SOCK_DEFAULT_RECEIVE_TO, ES_WIFI_MAX_SO_TIMEOUT);
                WifiChannel[i].sendtimeout     = MIN(NET_SOCK_DEFAULT_SEND_TO, ES_WIFI_MAX_SO_TIMEOUT);
                WifiChannel[i].blocking        = 1; /* default blocking */
                WifiChannel[i].pnetif          = pnetif;
            }
            net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
            ret = NET_OK;
        }
    }
    else
    {
        ret = NET_ERROR_DEVICE_ERROR;
    }
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_if_stop(net_if_handle_t *pnetif)
{
    net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
    return NET_OK;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_if_connect(net_if_handle_t *pnetif)
{
    int32_t ret;
    uint8_t addr[4];
    ES_WIFIObject_t  *pEsWifiObj = pnetif->pdrv->context;

    const net_wifi_credentials_t *credentials =  pnetif->pdrv->extension.wifi->credentials;

    ret = ES_WIFI_Connect(pEsWifiObj, credentials->ssid, credentials->psk,
                          (ES_WIFI_SecurityType_t) credentials->security_mode);
    if (ret == ES_WIFI_STATUS_OK)
    {
        ret = ES_WIFI_GetNetworkSettings(pEsWifiObj);
    }

    if (NET_OK == ret)
    {
        if (pEsWifiObj->NetSettings.IsConnected)
        {
            memcpy(addr, pEsWifiObj->NetSettings.IP_Addr, 4);
            pnetif->ipaddr = NET_ARTON(addr);
            net_state_manage_event(pnetif, NET_EVENT_IPADDR);
            ret = ES_WIFI_STATUS_OK;

        }
        else
        {
            ret = ES_WIFI_STATUS_ERROR;
        }
    }
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_if_disconnect(net_if_handle_t *pnetif)
{
    ES_WIFIObject_t  *pEsWifiObj = pnetif->pdrv->context;
    ES_WIFI_Disconnect(pEsWifiObj);
    net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
    return NET_OK;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_socket(int32_t domain, int32_t type, int32_t protocol)
{
    int32_t ret = NET_OK;

    if (domain != NET_AF_INET)
    {
        ret = NET_ERROR_UNSUPPORTED;
    }
    else
    {
        /* currently only SOCK_DGRAM and SOCK_STREAM are supported */
        if ((type != NET_SOCK_STREAM) && (type != NET_SOCK_DGRAM))
        {
            ret = NET_ERROR_UNSUPPORTED;
        }
        else
        {
            /* Only support PROT_IP/TCP/UDP/IPV4 are supported */
            if ((protocol != NET_IPPROTO_TCP) && (protocol != NET_IPPROTO_UDP))
            {
                ret = NET_ERROR_UNSUPPORTED;
            }
        }
    }

    if (ret == NET_OK)
    {
        ret = NET_ERROR_INVALID_SOCKET;

        for (int32_t i = 0; i < ESWIFI_MAX_CHANNEL_NBR; i++)
        {
            if (WifiChannel[i].status == WIFI_FREE_SOCKET)
            {
                /* Initialise the socket*/
                WifiChannel[i].status = WIFI_ALLOCATED_SOCKET;
                if (type == NET_SOCK_DGRAM)
                {
                    WifiChannel[i].status |= WIFI_SEND_OK | WIFI_RECV_OK;
                }
                WifiChannel[i].protocol        = protocol;
                WifiChannel[i].type            = type;
                ret = i;
                break;
            }
        }
    }

    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_bind(int32_t sock, const sockaddr_t *addr, int32_t addrlen)
{
    int32_t ret;
    sockaddr_in_t *source;

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        /* STREAM sockets cannot be bound after connection. */
        if ((WifiChannel [sock].status == WIFI_ALLOCATED_SOCKET)
                || ((WifiChannel [sock].status & WIFI_ALLOCATED_SOCKET) && (WifiChannel [sock].type == NET_SOCK_DGRAM)))
        {
            if (addrlen == sizeof(sockaddr_in_t))
            {
                source = (sockaddr_in_t *)addr;
                WifiChannel [sock].localport = NET_NTOHS(source->sin_port);
                WifiChannel [sock].localaddress = S_ADDR(source->sin_addr);
                WifiChannel [sock].status |= WIFI_BIND_SOCKET;
                ret = NET_OK;
            }
            else
            {
                ret = NET_ERROR_PARAMETER;
            }
        }
        else
        {
            ret = NET_ERROR_INVALID_SOCKET;
        }
    }
    return ret;
}


/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_listen(int32_t sock, int32_t backlog)
{
    int32_t ret;

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        ES_WIFIObject_t  *pEsWifiObj = WifiChannel[sock].pnetif->pdrv->context;
        ES_WIFI_Conn_t conn;

        conn.Number = sock;
        conn.LocalPort = WifiChannel[sock].localport;
        conn.Type = (WifiChannel[sock].protocol == NET_IPPROTO_TCP) ? ES_WIFI_TCP_CONNECTION : ES_WIFI_UDP_CONNECTION;
        conn.Backlog = backlog;
        ret = ES_WIFI_StartServerSingleConn(pEsWifiObj, &conn);
    }

    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_accept(int32_t sock, sockaddr_t *addr, int32_t *addrlen)
{
    int32_t ret;

    sockaddr_in_t *addrin = (sockaddr_in_t *)addr;

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        ES_WIFI_Conn_t conn;

        conn.Number = sock;

        ret = ES_WIFI_WaitServerConnection(WifiChannel[sock].pnetif->pdrv->context, 20000, &conn);

        if (ES_WIFI_STATUS_OK == ret)
        {
            addrin->sin_port = NET_NTOHS(conn.RemotePort);
            S_ADDR(addrin->sin_addr) = NET_ARTON(conn.RemoteIP);
            WifiChannel [sock].status |= WIFI_CONNECTED_SOCKET_RW | WIFI_STARTED_SERVER_SOCKET ;
            ret = sock;
        }
        else if (ES_WIFI_STATUS_TIMEOUT == ret)
        {
            ret =  NET_ERROR_SOCKET_FAILURE;
        }
    }
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_connect(int32_t sock, const sockaddr_t *addr, int32_t addrlen)
{
    int32_t  ret;
    sockaddr_in_t *dest;

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        ES_WIFI_Conn_t conn;
#ifdef NET_MBEDTLS_WIFI_MODULE_SUPPORT
        eswifi_tls_data_t *tlsdata;
        uint32_t      casize;
        uint32_t      keysize;
        uint32_t      devsize;
#endif /* NET_MBEDTLS_WIFI_MODULE_SUPPORT */

        /*addr is an outbound address */
        if (addrlen == sizeof(sockaddr_in_t))
        {
            dest = (sockaddr_in_t *) addr;
            conn.Number = sock;
            conn.LocalPort = WifiChannel[sock].localport;
            conn.RemotePort = NET_NTOHS(dest->sin_port);
            NET_NTOAR(S_ADDR(dest->sin_addr), conn.RemoteIP);

            switch (WifiChannel[sock].protocol)
            {
            case NET_IPPROTO_TCP :
                conn.Type = ES_WIFI_TCP_CONNECTION;
                ret = NET_OK;
                break;

            case NET_IPPROTO_UDP :
                ret = NET_ERROR_UNSUPPORTED;
                break;

            case NET_IPPROTO_TCP_TLS :
#ifdef NET_MBEDTLS_WIFI_MODULE_SUPPORT
                tlsdata = &WifiChannel[sock].tlsData;
                conn.Type = ES_WIFI_TCP_SSL_CONNECTION;

                casize  = (tlsdata->tls_ca_certs == 0) ? 0 : strlen(tlsdata->tls_ca_certs) + 1;
                keysize = (tlsdata->tls_dev_key == 0) ? 0 : strlen(tlsdata->tls_dev_key) + 1;
                devsize = (tlsdata->tls_dev_cert == 0) ? 0 : strlen(tlsdata->tls_dev_cert) + 1;
                conn.TLScheckMode = ES_WIFI_TLS_CHECK_NOTHING;
                if (casize) conn.TLScheckMode = ES_WIFI_TLS_CHECK_ROOTCA;
                if (keysize) conn.TLScheckMode =ES_WIFI_TLS_CHECK_DEVICE_CERTS;
                ret = ES_WIFI_StoreCreds(WifiChannel[sock].pnetif->pdrv->context, ES_WIFI_FUNCTION_TLS,
                                         ES_WIFI_TLS_MULTIPLE_WRITE_SLOT,
                                         (uint8_t *) tlsdata->tls_ca_certs, casize,
                                         (uint8_t *) tlsdata->tls_dev_cert, devsize,
                                         (uint8_t *) tlsdata->tls_dev_key, keysize);

#else
                ret = NET_ERROR_UNSUPPORTED;
#endif /* NET_MBEDTLS_WIFI_MODULE_SUPPORT */
                break;

            default:
                ret = NET_ERROR_PARAMETER;
            }

            if (ret == NET_OK)
            {
                if (NET_OK == ES_WIFI_StartClientConnection(WifiChannel[sock].pnetif->pdrv->context, &conn))
                {
                    WifiChannel[sock].status |= WIFI_CONNECTED_SOCKET_RW;
                }
                else
                {
                    ret = NET_ERROR_SOCKET_FAILURE;
                }
            }
        }
        else
        {
            ret = NET_ERROR_PARAMETER;
        }
    }
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_shutdown(int32_t sock, int32_t mode)
{

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        return  NET_ERROR_INVALID_SOCKET;
    }

    if (WifiChannel[sock].status &  WIFI_CONNECTED_SOCKET)
    {
        if (mode == NET_SHUTDOWN_R)
        {
            WifiChannel[sock].status &=  ~WIFI_RECV_OK;
        }
        if (mode == NET_SHUTDOWN_W)
        {
            WifiChannel[sock].status &=  ~WIFI_SEND_OK;
        }
        if (mode == NET_SHUTDOWN_RW)
        {
            WifiChannel[sock].status &=  ~(WIFI_SEND_OK | WIFI_RECV_OK);
        }
    }
    return NET_OK;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_close(int32_t sock, bool isaclone)
{

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        return  NET_ERROR_INVALID_SOCKET;
    }

    if (WifiChannel [sock].status & WIFI_STARTED_SERVER_SOCKET)
    {
        if (isaclone)
        {
            if (ES_WIFI_CloseServerConnection(WifiChannel[sock].pnetif->pdrv->context, sock) != NET_OK)
            {
                WifiChannel[sock].status = WIFI_FREE_SOCKET;
                return NET_ERROR_SOCKET_FAILURE;
            }
            WifiChannel[sock].status = WIFI_FREE_SOCKET;
            return NET_OK;
        }
        else
        {
            if (ES_WIFI_StopServerSingleConn(WifiChannel[sock].pnetif->pdrv->context, sock) != NET_OK)
            {
                WifiChannel[sock].status = WIFI_FREE_SOCKET;
                return NET_ERROR_SOCKET_FAILURE;
            }
            WifiChannel[sock].status = WIFI_FREE_SOCKET;
            return NET_OK;
        }
    }
    else
    {
        ES_WIFI_Conn_t conn;
        conn.Number = sock;
        if (ES_WIFI_StopClientConnection(WifiChannel[sock].pnetif->pdrv->context, &conn) != NET_OK)
        {
            WifiChannel[sock].status = WIFI_FREE_SOCKET;
            return NET_ERROR_SOCKET_FAILURE;
        }
        WifiChannel[sock].status = WIFI_FREE_SOCKET;
        return NET_OK;
    }
}


static void check_connection_lost(net_if_handle_t *pnetif, int32_t n)
{
    if (n <= 0)
    {
        if (NET_STATE_CONNECTED == pnetif->state)
        {
            if (ES_WIFI_IsConnected(pnetif->pdrv->context) == 0)
            {
                net_if_notify(pnetif, NET_EVENT_STATE_CHANGE, NET_STATE_CONNECTION_LOST, NULL);
            }
        }
    }
    else if (n > 0)
    {
        if (NET_STATE_CONNECTION_LOST == pnetif->state)
        {
            net_if_notify(pnetif, NET_EVENT_STATE_CHANGE, NET_STATE_CONNECTED, NULL);
        }
    }
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
    int32_t ret = NET_ERROR_DATA;
    uint16_t SentDatalen;

    if (flags != 0)
    {
        return NET_ERROR_UNSUPPORTED;
    }

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        return NET_ERROR_INVALID_SOCKET;
    }

    if (!(WifiChannel [sock].status & WIFI_SEND_OK))
    {
        return NET_ERROR_SOCKET_FAILURE;
    }


    if (len > ES_WIFI_PAYLOAD_SIZE)
    {
        len = ES_WIFI_PAYLOAD_SIZE;
    }
    if (NET_OK == ES_WIFI_SendData(WifiChannel[sock].pnetif->pdrv->context,
                                   sock,
                                   buf,
                                   len,
                                   &SentDatalen,
                                   WifiChannel[sock].sendtimeout))
    {
        ret = SentDatalen;
    }
    else
    {
        ret = NET_ERROR_SOCKET_FAILURE;
    }
    check_connection_lost(WifiChannel[sock].pnetif, ret);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
    int32_t ret = NET_ERROR_DATA;
    uint16_t ReceivedDatalen;
    uint32_t     timeout;


    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        return NET_ERROR_INVALID_SOCKET;
    }

    if (!(WifiChannel [sock].status & WIFI_RECV_OK))
    {
        return NET_ERROR_SOCKET_FAILURE;
    }


    if (flags == NET_MSG_DONTWAIT)
    {
        timeout = 0;
    }
    else
    {
        timeout = WifiChannel[sock].recvtimeout;
    }

    if (len > ES_WIFI_PAYLOAD_SIZE)
    {
        len = ES_WIFI_PAYLOAD_SIZE;
    }

    if (NET_OK == ES_WIFI_ReceiveData(WifiChannel[sock].pnetif->pdrv->context,
                                      sock,
                                      buf,
                                      len,
                                      &ReceivedDatalen,
                                      timeout))
    {
        ret = ReceivedDatalen;
    }
    else
    {
        ret = NET_ERROR_SOCKET_FAILURE;
    }

    check_connection_lost(WifiChannel[sock].pnetif, ret);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *to, int32_t tolen)
{
    int32_t ret = NET_OK;

    if ((flags != 0) || (to->sa_family != NET_AF_INET))
    {
        return NET_ERROR_UNSUPPORTED;
    }

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        return NET_ERROR_INVALID_SOCKET;
    }

    if (WifiChannel[sock].protocol != NET_IPPROTO_UDP)
    {
        return NET_ERROR_SOCKET_FAILURE;
    }

    if (!(WifiChannel [sock].status & WIFI_SEND_OK))
    {
        return NET_ERROR_SOCKET_FAILURE;
    }

    if (!(WifiChannel [sock].status & WIFI_STARTED_CLIENT_SOCKET))
    {
        sockaddr_in_t *dest;
        ES_WIFI_Conn_t conn;

        if (tolen == sizeof(sockaddr_in_t))
        {
            dest = (sockaddr_in_t *) to;
            conn.Number = sock;
            conn.LocalPort = WifiChannel[sock].localport;
            conn.RemotePort = NET_NTOHS(dest->sin_port);
            NET_NTOAR(S_ADDR(dest->sin_addr), conn.RemoteIP);
            conn.Type = ES_WIFI_UDP_CONNECTION;

            if (NET_OK == ES_WIFI_StartClientConnection(WifiChannel[sock].pnetif->pdrv->context, &conn))
            {
                WifiChannel[sock].status |= WIFI_STARTED_CLIENT_SOCKET;
            }
            else
            {
                ret = NET_ERROR_SOCKET_FAILURE;
            }
        }
    }

    if (ret == NET_OK)
    {
        uint16_t SentDatalen;

        if (len > ES_WIFI_PAYLOAD_SIZE)
        {
            len = ES_WIFI_PAYLOAD_SIZE;
        }

        ipaddr_t remoteaddr = 0;
        sockaddr_in_t *saddr = (sockaddr_in_t *) to;
        remoteaddr = NET_NTOHL(saddr->sin_addr);

        uint32_t IPaddr = NET_NTOHL(remoteaddr);
        uint16_t Port = NET_NTOHS(saddr->sin_port);

        if (NET_OK == ES_WIFI_SendDataTo(WifiChannel[sock].pnetif->pdrv->context,
                                         sock,
                                         buf,
                                         len,
                                         &SentDatalen,
                                         WifiChannel[sock].sendtimeout,
                                         (uint8_t *) &IPaddr,
                                         Port))
        {
            ret = SentDatalen;
        }
        else
        {
            ret = NET_ERROR_SOCKET_FAILURE;
        }
        check_connection_lost(WifiChannel[sock].pnetif, ret);
    }

    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t es_wifi_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *from,
                                int32_t *fromlen)
{
    int32_t ret = NET_OK;
    uint32_t timeout = 0;

    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        return NET_ERROR_INVALID_SOCKET;
    }

    if (WifiChannel[sock].protocol != NET_IPPROTO_UDP)
    {
        return NET_ERROR_SOCKET_FAILURE;
    }

    if (!(WifiChannel [sock].status & WIFI_RECV_OK))
    {
        return NET_ERROR_SOCKET_FAILURE;
    }

    if (!(flags & NET_MSG_DONTWAIT))
    {
        timeout = WifiChannel[sock].recvtimeout;
        if (timeout > ES_WIFI_MAX_SO_TIMEOUT)
        {
            return NET_ERROR_UNSUPPORTED;
        }
    }

    if (!(WifiChannel [sock].status & WIFI_STARTED_CLIENT_SOCKET))
    {
        /* TODO: If starting by reading, would mean that es_wifi "server mode" is needed?       */
        /* Workaround sending a fake packet, so that further recvfrom() is possible without error.  */
        /* If no better idea, could send an mdns advertisement.                   */

        flags = 0;
        sockaddr_in_t saddr;
        saddr.sin_len = sizeof(sockaddr_in_t);
        saddr.sin_family = NET_AF_INET;
        saddr.sin_addr = net_aton_r((const char_t *)"224.0.0.251");
        saddr.sin_port = NET_HTONS(5353);

        ret = es_wifi_sendto(sock, NULL, 0, flags, (sockaddr_t *) &saddr, sizeof(sockaddr_in_t));
    }

    if (ret == NET_OK)
    {
        uint16_t ReceivedDatalen;
        uint32_t IPaddr = 0;
        uint16_t port = 0;

        if (len > ES_WIFI_PAYLOAD_SIZE)
        {
            len = ES_WIFI_PAYLOAD_SIZE;
        }

        if (NET_OK == ES_WIFI_ReceiveDataFrom(WifiChannel[sock].pnetif->pdrv->context,
                                              sock,
                                              buf,
                                              len,
                                              &ReceivedDatalen,
                                              timeout,
                                              (uint8_t *) &IPaddr,
                                              &port))
        {
            sockaddr_in_t saddr;
            memset(&saddr, 0, sizeof(sockaddr_in_t));
            saddr.sin_family = NET_AF_INET;
            saddr.sin_addr = IPaddr;
            saddr.sin_port = NET_HTONS(port);
            saddr.sin_len = sizeof(sockaddr_in_t);
            memset(from, 0, *fromlen);
            memcpy(from, &saddr, MIN(*fromlen, saddr.sin_len));
            *fromlen = sizeof(sockaddr_in_t);
            ret = ReceivedDatalen;
        }
        else
        {
            ret = NET_ERROR_SOCKET_FAILURE;
        }
    }
    check_connection_lost(WifiChannel[sock].pnetif, ret);

    return ret;
}

/**
  * @brief  function description
  * @param  Params
  * @retval socket status
  */

#define OPTCHECKTYPE(type,optlen)       if (sizeof(type)!= optlen) return NET_ERROR_PARAMETER
#define OPTCHECKSTRING(opt,optlen)       if (strlen(opt)!= (optlen-1)) return NET_ERROR_PARAMETER

static int32_t es_wifi_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue, int32_t optlen)
{
    int32_t ret = NET_ERROR_PARAMETER;


    if ((sock < 0) || (sock >= ESWIFI_MAX_CHANNEL_NBR))
    {
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        switch (optname)
        {
        case NET_SO_RCVTIMEO:
            /* Receive time out. */
            WifiChannel[sock].recvtimeout = *(const int32_t *) optvalue;
            ret = NET_OK;
            break;

        case NET_SO_SNDTIMEO:
            WifiChannel[sock].sendtimeout = *(int32_t *) optvalue;

            if (WifiChannel[sock].protocol == (uint8_t) NET_IPPROTO_UDP)
            {
                if (WifiChannel[sock].sendtimeout > NET_UDP_MAX_SEND_BLOCK_TO)
                {
                    WifiChannel[sock].sendtimeout = NET_UDP_MAX_SEND_BLOCK_TO;
                }
            }
            ret = NET_OK;
            break;
            /*
                case NET_SO_EVENT_CALLBACK:
                   WifiChannel[sock].notify_callback = ((net_event_handler_t*)optvalue)->callback;
                   WifiChannel[sock].notify_context = ((net_event_handler_t*)optvalue)->context;
                  ret = NET_OK;
                  break;
            */

#ifdef NET_MBEDTLS_WIFI_MODULE_SUPPORT
        case NET_SO_SECURE:

            if (WifiChannel[sock].protocol == (uint8_t) NET_IPPROTO_TCP)
            {
                WifiChannel[sock].protocol = (uint8_t) NET_IPPROTO_TCP_TLS;
                ret =  NET_OK;
            }
            else
            {
                NET_DBG_ERROR("Only TCP socket supports TLS protocol\n");
            }
            break;

        case NET_SO_TLS_DEV_CERT:

            OPTCHECKSTRING(optvalue, optlen);
            if (WifiChannel[sock].protocol != (uint8_t) NET_IPPROTO_TCP_TLS)
            {
                NET_DBG_ERROR("Failed to set tls device certificats, please set socket as secure before\n");
                ret = NET_ERROR_IS_NOT_SECURE;
            }
            WifiChannel[sock].tlsData.tls_dev_cert = (char *) optvalue;
            ret =  NET_OK;
            break;

        case NET_SO_TLS_DEV_KEY:

            OPTCHECKSTRING(optvalue, optlen);
            if (WifiChannel[sock].protocol != (uint8_t) NET_IPPROTO_TCP_TLS)
            {
                NET_DBG_ERROR("Failed to set tls device key, please set socket as secure before\n");
                return NET_ERROR_IS_NOT_SECURE;
            }
            WifiChannel[sock].tlsData.tls_dev_key = (char *) optvalue;
            ret =  NET_OK;
            break;

        case NET_SO_TLS_CA_CERT:

            if (WifiChannel[sock].protocol != (uint8_t) NET_IPPROTO_TCP_TLS)
            {
                NET_DBG_ERROR("Failed to set tls root ca, please set socket as secure before\n");
                return NET_ERROR_IS_NOT_SECURE;
            }
            /* FIXME OPTCHECKSTRING(optvalue,optlen); */
            WifiChannel[sock].tlsData.tls_ca_certs = (char *) optvalue;
            ret =  NET_OK;
            break;

        case NET_SO_TLS_CA_CRL:

            OPTCHECKSTRING(optvalue, optlen);
            if (WifiChannel[sock].protocol != (uint8_t) NET_IPPROTO_TCP_TLS)
            {
                NET_DBG_ERROR("Failed to set tls root ca, please set socket as secure before\n");
                return NET_ERROR_IS_NOT_SECURE;
            }
            WifiChannel[sock].tlsData.tls_ca_crl = (char *) optvalue;
            ret =  NET_OK;
            break;

        case NET_SO_TLS_PASSWORD:

            OPTCHECKSTRING(optvalue, optlen);
            if (WifiChannel[sock].protocol != (uint8_t) NET_IPPROTO_TCP_TLS)
            {
                NET_DBG_ERROR("Failed to set tls password, please set socket as secure before\n");
                return NET_ERROR_IS_NOT_SECURE;
            }
            WifiChannel[sock].tlsData.tls_dev_pwd = (unsigned char *) optvalue;
            ret =  NET_OK;
            break;


        case NET_SO_TLS_SERVER_VERIFICATION:

            OPTCHECKTYPE(bool, optlen);
            if (WifiChannel[sock].protocol != (uint8_t) NET_IPPROTO_TCP_TLS)
            {
                NET_DBG_ERROR("Failed to set tls server verification, please set socket as secure before\n");
                return NET_ERROR_IS_NOT_SECURE;
            }
            WifiChannel[sock].tlsData.tls_srv_verification = (*(bool *)optvalue > 0) ? true : false;
            ret = NET_OK;
            break;

        case NET_SO_TLS_SERVER_NAME:

            OPTCHECKSTRING(optvalue, optlen);
            if (WifiChannel[sock].protocol != (uint8_t) NET_IPPROTO_TCP_TLS)
            {
                NET_DBG_ERROR("Failed to set tls server name, please set socket as secure before\n");
                return NET_ERROR_IS_NOT_SECURE;
            }
            WifiChannel[sock].tlsData.tls_srv_name = (char *)optvalue;
            ret = NET_OK;
            break;
#endif /* NET_MBEDTLS_WIFI_MODULE_SUPPORT */
        case  NET_SO_BLOCKING:
            OPTCHECKTYPE(bool, optlen);
            if (*(bool *)optvalue == false)
            {
                /* Workaroud , non blocking socket are not supported so force timeout to min value */
                WifiChannel[sock].sendtimeout = 1;
                WifiChannel[sock].recvtimeout = 1;
            }
            ret =  NET_OK;
            break;
			
		case	NET_SO_TLS_CERT_PROF:
		    NET_DBG_ERROR("Ignore this option because it embedded inside Inventek Wifi module and can not be changed\n");
		    ret =  NET_OK;
            break;


        default :
            break;
        }
    }
    return ret;
}

static int32_t es_wifi_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, int32_t *optlen)
{
    return NET_ERROR_UNSUPPORTED;
}

static int32_t es_wifi_getsockname(int32_t sock, sockaddr_t *name, int32_t *namelen)
{
    return NET_ERROR_UNSUPPORTED;
}

static int32_t es_wifi_getpeername(int32_t sock, sockaddr_t *name, int32_t *namelen)
{
    return NET_ERROR_UNSUPPORTED;
}

static int32_t es_wifi_gethostbyname(net_if_handle_t *pnetif, sockaddr_t *addr, char_t *name)
{
    int32_t ret = NET_ERROR_DNS_FAILURE;
    uint8_t ipaddr[6];

    if (addr->sa_len < sizeof(sockaddr_in_t))
    {
        return NET_ERROR_PARAMETER;
    }

    if (ES_WIFI_STATUS_OK == ES_WIFI_DNS_LookUp(pnetif->pdrv->context, (char *)name, ipaddr))
    {
        uint8_t len = addr->sa_len;
        sockaddr_in_t *saddr = (sockaddr_in_t *) addr;

        memset(saddr, 0, len);
        saddr->sin_len = len;
        saddr->sin_family = NET_AF_INET;
        memcpy(&(saddr->sin_addr), ipaddr, 4);
        ret = NET_OK;
    }

    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */


int32_t es_wifi_ping(net_if_handle_t *pnetif, sockaddr_t *addr, int32_t count, int32_t delay, int32_t response[])
{
    int32_t ret;
    uint8_t     ipaddr[6];

    memcpy(ipaddr, &addr->sa_data[2], 6);

    ret = ES_WIFI_Ping(pnetif->pdrv->context, ipaddr, count, delay, (int32_t *)response);
    if (ret < 0)
    {
        return ret;
    }
    return NET_OK;
}

static  int32_t es_wifi_scan(net_wifi_scan_mode_t mode)
{
    return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

