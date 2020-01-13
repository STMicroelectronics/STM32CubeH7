/**
  ******************************************************************************
  * @file    net_cellular.c
  * @author  MCD Application Team
  * @brief   CELLULAR-specific BSD-like socket wrapper
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

/*#define ENABLE_NET_DBG_INFO */
#include "net_connect.h"
#include "net_internals.h"

#include "cellular_version.h"

#if defined(CELLULAR_INIT_FIRMWARE_VERSION)
#include "cellular_init.h"
#include "cellular_service_os.h"
#elif defined(CELLULAR_VERSION_FIRMWARE_VERSION)
#include "cellular_mngt.h"
#include "cellular_datacache.h"
#include "com_sockets.h"
#endif /* defined(CELLULAR_INIT_FIRMWARE_VERSION) */

#include "dc_common.h"
#include "dc_control.h"


/* Private defines -----------------------------------------------------------*/
#define NET_CELLULAR_MAX_CHANNEL_NBR    4

#define CELLULAR_FREE_SOCKET            0
#define CELLULAR_ALLOCATED_SOCKET       1
#define CELLULAR_BIND_SOCKET            2
#define CELLULAR_SEND_OK                4
#define CELLULAR_RECV_OK                8
#define CELLULAR_CONNECTED_SOCKET      16
#define CELLULAR_CONNECTED_SOCKET_RW   (CELLULAR_CONNECTED_SOCKET | CELLULAR_SEND_OK | CELLULAR_RECV_OK)

#define CELLULAR_SOCK_DEFAULT_TO    10000
#define CELLULAR_ATTACHMENT_TIMEOUT    180000U

#if !defined NET_CELLULAR_THREAD_PRIO
#define NET_CELLULAR_THREAD_PRIO   osPriorityAboveNormal
#endif  /* !defined NET_CELLULAR_THREAD_PRIO */

#if !defined NET_CELLULAR_THREAD_STACK_SIZE
#define NET_CELLULAR_THREAD_STACK_SIZE  DEFAULT_THREAD_STACK_SIZE
#endif  /* !defined NET_CELLULAR_THREAD_PRIO */

typedef enum
{
  CELLULAR_RET_OK                = 0,
  CELLULAR_RET_ERROR             = 1,
  CELLULAR_RET_SIM_NOT_INSERTED  = 2,
} CELLULAR_Ret_t;

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  const char *apn;
  const char *username;
  const char *password;
  bool use_internal_sim;
} cellular_data_t;

typedef struct
{
  uint8_t  status;
} cellular_Channel_t;

/* Private variables ---------------------------------------------------------*/
static osThreadId CellIfThreadId = NULL;
static osMessageQId cellular_queue;
static volatile bool connection_requested;
static volatile bool stop_requested;
static dc_cellular_info_t dc_cellular_info;
static dc_cellular_data_info_t  dc_cellular_data_info;
static dc_sim_info_t dc_sim_info;
static dc_nifman_info_t dc_nifman_info;

static cellular_Channel_t CellularChannel[NET_CELLULAR_MAX_CHANNEL_NBR] = {.0};



/* Private function prototypes -----------------------------------------------*/

/* Interface APIs */
static int32_t net_cellular_if_init(net_if_handle_t *pnetif);
static int32_t net_cellular_if_deinit(net_if_handle_t *pnetif);

static int32_t net_cellular_if_start(net_if_handle_t *pnetif);
static int32_t net_cellular_if_stop(net_if_handle_t *pnetif);

static int32_t net_cellular_if_connect(net_if_handle_t *pnetif);
static int32_t net_cellular_if_disconnect(net_if_handle_t *pnetif);

/* Socket BSD Like APIs */
static int32_t net_cellular_socket(int32_t domain, int32_t type, int32_t protocol);
static int32_t net_cellular_bind(int32_t sock, const sockaddr_t *addr, int32_t addrlen);
static int32_t net_cellular_listen(int32_t sock, int32_t backlog);
static int32_t net_cellular_accept(int32_t sock, sockaddr_t *addr, int32_t *addrlen);
static int32_t net_cellular_connect(int32_t sock, const sockaddr_t *addr, int32_t addrlen);
static int32_t net_cellular_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t net_cellular_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t net_cellular_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *to,
                                   int32_t tolen);
static int32_t net_cellular_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *from,
                                     int32_t *fromlen);
static int32_t net_cellular_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue,
                                       int32_t optlen);
static int32_t net_cellular_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, int32_t *optlen);
static int32_t net_cellular_getsockname(int32_t sock, sockaddr_t *name, int32_t *namelen);
static int32_t net_cellular_getpeername(int32_t sock, sockaddr_t *name, int32_t *namelen);
static int32_t net_cellular_close(int32_t sock, bool isaclone);
static int32_t net_cellular_shutdown(int32_t sock, int32_t mode);

/* Service APIs */
static int32_t net_cellular_if_gethostbyname(net_if_handle_t *pnetif, sockaddr_t *addr, char_t *name);
static int32_t net_cellular_ping(net_if_handle_t *pnetif, sockaddr_t *addr, int32_t count,
 int32_t delay, int32_t response[]);

/* class extension APIs */
static int32_t net_cellular_get_radio_info(net_cellular_radio_results_t *results);


/* Internal usage */
static void cellif_input(void const *argument);
static void cellular_set_config(const char *oper_ap_code,
                                const char *username,
                                const char *password,
                                bool use_internal_sim);
static void cellular_notif_cb(dc_com_event_id_t dc_event_id, const void *private_gui_data);
static uint16_t Generate_RngLocalPort(uint16_t localport);

static net_if_drv_t   *net_cellular_init_class(void)
{
  net_if_drv_t *p;

  p = NET_MALLOC(sizeof(net_if_drv_t));
  if (p)
  {
    p->if_class = NET_INTERFACE_CLASS_CELLULAR;

    /* Interface APIs */
    p->if_init = net_cellular_if_init;
    p->if_deinit = net_cellular_if_deinit;

    p->if_start = net_cellular_if_start;
    p->if_stop = net_cellular_if_stop;

    p->if_connect = net_cellular_if_connect;
    p->if_disconnect = net_cellular_if_disconnect;

    /* Socket BSD Like APIs */
    p->socket        = net_cellular_socket;
    p->bind          = net_cellular_bind;
    p->listen        = net_cellular_listen;
    p->accept        = net_cellular_accept;
    p->connect       = net_cellular_connect;
    p->send          = net_cellular_send;
    p->recv          = net_cellular_recv;
    p->sendto        = net_cellular_sendto;
    p->recvfrom      = net_cellular_recvfrom;
    p->setsockopt    = net_cellular_setsockopt;
    p->getsockopt    = net_cellular_getsockopt;
    p->getsockname   = net_cellular_getsockname;
    p->getpeername   = net_cellular_getpeername;
    p->close         = net_cellular_close;
    p->shutdown      = net_cellular_shutdown;

    /* Service */
    p->gethostbyname = net_cellular_if_gethostbyname;

    p->ping = net_cellular_ping;
  }
  else
  {
    NET_DBG_ERROR("can't allocate memory for cellular_driver class");
    p = NULL;
  }
  return p;
}

/**
  * @brief  Build the Network interface
  *         It includes memory allocation for all internal class and extension
  * @param  pnetif points to a interface handle (in/out)
  * @retval
  *          = @if_init.
  */
int32_t cellular_net_driver(net_if_handle_t *pnetif)
{
    net_if_drv_t *p;

    p = net_cellular_init_class();

    if (NULL == p)
    {
        NET_DBG_ERROR("can't allocate memory for cellular driver class");
        return NET_ERROR_NO_MEMORY;
    }
    else
    {
        /* class extension */
        p->extension.cellular = NET_MALLOC(sizeof(net_if_cellular_class_extension_t));
        if (NULL == p->extension.cellular)
        {
            NET_DBG_ERROR("can't allocate memory for cellular_driver class\n");
            NET_FREE(p);
            p = NULL;
        }
        else
        {
            p->extension.cellular->get_radio_results = net_cellular_get_radio_info;
            pnetif->pdrv = p;
        }
    }

    return net_cellular_if_init(pnetif);
}

/**
  * @brief  Initialize the cellular (modules and memory init)
  * @param  pnetif points to a interface handle (in/out)
  * @retval
  *          = 0  : done.
  */
static int32_t net_cellular_if_init(net_if_handle_t *pnetif)
{
    /* statical init of cellular components */
    connection_requested = false;
    stop_requested = false;
    cellular_init();

    return  NET_OK;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_cellular_if_deinit(net_if_handle_t *pnetif)
{
    NET_DBG_PRINT("\nEntering in net_cellular_if_deinit()");

    NET_FREE(pnetif->pdrv->extension.cellular);
    pnetif->pdrv->extension.cellular = NULL;
    NET_FREE(pnetif->pdrv);
    pnetif->pdrv = NULL;

    return  NET_OK;
}



/**
  * @brief  Start the cellular (modem switch on)
  * @param  pnetif points to a interface handle (in/out)
  * @retval
  *          < 0  : if there is an error.
  *          = 0  : on success.
  */
static int32_t net_cellular_if_start(net_if_handle_t *pnetif)
{
    const net_cellular_credentials_t *credentials =  pnetif->pdrv->extension.cellular->credentials;


    NET_PRINT("\n*** C2C connection ***\n\n");

    /* check APN is entered */
    if ((credentials->apn == 0) || (strlen(credentials->apn) == 0))
    {
        NET_DBG_ERROR("operator credentials (APN at least) must be set");
        return NET_ERROR_PARAMETER;
    }

    osMessageQDef(CellIfqueue, 6, uint32_t);
    cellular_queue = osMessageCreate(osMessageQ(CellIfqueue), NULL);

    /* create the task that handles the cellular */
    osThreadDef(CellIf, cellif_input, NET_CELLULAR_THREAD_PRIO, 0, NET_CELLULAR_THREAD_STACK_SIZE);
    CellIfThreadId = osThreadCreate(osThread(CellIf), pnetif);

    if (CellIfThreadId == NULL)
    {
        Error_Handler();

    }
#if (STACK_ANALYSIS_TRACE == 1)
    else
    {
        stackAnalysis_addStackSizeByHandle(CellIfThreadId, NET_CELLULAR_THREAD_STACK_SIZE);
    }
#endif /* STACK_ANALYSIS_TRACE */
    return NET_OK;
}


static int32_t net_cellular_if_stop(net_if_handle_t *pnetif)
{
    dc_cellular_target_state_t target_state;
    stop_requested = true;

    target_state.rt_state     = DC_SERVICE_ON;
    target_state.target_state = DC_TARGET_STATE_OFF;

    (void)dc_com_write(&dc_com_db,
                       DC_CELLULAR_TARGET_STATE_CMD,
                       (void *)&target_state,
                       sizeof(target_state));
    return  NET_OK;
}


/**
  * @brief  Connect the cellular (@IP allocation)
  * @param  pnetif points to a interface handle (in/out)
  * @retval
  *          < 0  : if there is an error.
  *          = 0  : on success.
  */
static int32_t net_cellular_if_connect(net_if_handle_t *pnetif)
{
  dc_cellular_target_state_t target_state;

  /* 'cst targetstate full' command:  new modem state requested: modem manages full data transfert */
  target_state.rt_state     = DC_SERVICE_ON;
  target_state.target_state = DC_TARGET_STATE_FULL;
  (void)dc_com_write(&dc_com_db, DC_CELLULAR_TARGET_STATE_CMD, (void *)&target_state, sizeof(target_state));
  connection_requested = true;
  return NET_OK;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_cellular_if_disconnect(net_if_handle_t *pnetif)
{
    net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
    return  NET_OK;
}


/**
  * @brief  create a socket.
  *
  * @param  domain is currently only NET_AF_INET (IPv6 not supported)
  *         type can be :
  *           - connected (NET_SOCK_STREAM)
  *           - connectionless (NET_SOCK_DGRAM), not supported in this version
  *         protocol is NET_IPPROTO_TCP
  * @retval ret
  *          <  0  : if there is an error.
  *          >= 0  : socket number.
  */
static int32_t net_cellular_socket(int32_t domain, int32_t type, int32_t protocol)
{
    int32_t ret = NET_OK;

    int32_t net_socket;
    int32_t net_family = COM_AF_INET;
    int32_t net_type = COM_SOCK_DGRAM;
    int32_t net_protocol = COM_IPPROTO_UDP;

    if (domain != NET_AF_INET)
    {
        NET_DBG_ERROR("invalid domain");
        ret = NET_ERROR_UNSUPPORTED;
    }
    else
    {
        /* Only support PROT_IP/TCP/UDP/IPV4 are supported */
        if ((protocol != NET_IPPROTO_TCP) && (protocol != NET_IPPROTO_UDP))
        {
            NET_DBG_ERROR("invalid protocol");
            ret = NET_ERROR_UNSUPPORTED;
        }
        else
        {
            /* currently only SOCK_DGRAM and SOCK_STREAM are supported */
            switch (type)
            {
            case NET_SOCK_STREAM:
                net_family = COM_AF_INET;
                net_type = COM_SOCK_STREAM;
                net_protocol = COM_IPPROTO_TCP;

                ret = NET_OK;

                break;

            case NET_SOCK_DGRAM:
                net_family = COM_AF_INET;
                net_type = COM_SOCK_DGRAM;
                net_protocol = COM_IPPROTO_UDP;

                ret = NET_OK;

                break;
            default:

                ret = NET_ERROR_UNSUPPORTED;
                NET_DBG_ERROR("Undefined protocol");
                break;
            }
        }
    }

    if (ret == NET_OK)
    {
        NET_DBG_INFO("Trying to find a free socket\n");

        /* Find and create a free socket on the network interface */
        net_socket = com_socket(net_family, net_type, net_protocol);

        /* A free socket has been found */
        if (net_socket >= 0)
        {
            NET_DBG_INFO("Found socket : %lu\n", net_socket);

            if (net_socket < NET_CELLULAR_MAX_CHANNEL_NBR)
            {
                CellularChannel[net_socket].status = CELLULAR_ALLOCATED_SOCKET;
                ret = net_socket;
            }
            else
            {
                NET_DBG_ERROR("More socket supported (%lu) than allowed (%d)",
                              net_socket,
                              NET_CELLULAR_MAX_CHANNEL_NBR);
                ret = NET_ERROR_INVALID_SOCKET;
            }
        }
        else
        {
            NET_DBG_ERROR("Could not find a free socket.");
            ret = NET_ERROR_INVALID_SOCKET;
        }
    }

    return ret;
}

static int32_t net_cellular_bind(int32_t sock, const sockaddr_t *addr, int32_t addrlen)
{
    int32_t ret;
    com_sockaddr_in_t saddr;

    saddr.sin_len = sizeof(com_sockaddr_in_t);
    saddr.sin_family = (((sockaddr_in_t *)addr)->sin_family == NET_AF_INET) ? COM_AF_INET : COM_AF_UNSPEC;
    saddr.sin_port = ((sockaddr_in_t *)addr)->sin_port;
    memcpy(&saddr.sin_addr, &((sockaddr_in_t *)addr)->sin_addr, 4);

    ret = com_bind(sock, (com_sockaddr_t *)&saddr, sizeof(com_sockaddr_in_t));

    return (ret == COM_SOCKETS_ERR_OK) ? NET_OK : NET_ERROR_GENERIC;
}

static int32_t net_cellular_listen(int32_t sock, int32_t backlog)
{
    int32_t ret;
    ret = com_listen(sock, backlog);
    return (ret == COM_SOCKETS_ERR_OK) ? NET_OK : NET_ERROR_GENERIC;
}

static int32_t net_cellular_accept(int32_t sock, sockaddr_t *addr, int32_t *addrlen)
{
    int32_t ret;
    com_sockaddr_t        com_addr;
    sockaddr_in_t *saddr = (sockaddr_in_t *) addr;

    ret = com_accept(sock, &com_addr, addrlen);
    if (ret > 0)
    {
        *addrlen = sizeof(sockaddr_in_t);
        memset(saddr, 0, *addrlen);
        saddr->sin_len = *addrlen;
        saddr->sin_family = NET_AF_INET;
        memcpy(&(saddr->sin_addr), &com_addr.sa_data, 4);
        CellularChannel[ret].status = CELLULAR_CONNECTED_SOCKET_RW;
    }
    return (ret > 0) ? ret : NET_ERROR_GENERIC;

}


/**
  * @brief  connect a socket to a remote host on a given port.
  *
  * @param  sock must be created
  *         addr point to an addresses structure from the Socket library
  *         (If this is a TCP socket,
  *         connect() will actually perfom TCP negociation to open a connection)
  *         addrlen has a size compatible with struct sockaddr_in*
  * @retval ret
  *          < 0  : if there is an error.
  *          = 0  : on success.
  */
static int32_t net_cellular_connect(int32_t sock, const sockaddr_t *addr, int32_t addrlen)
{
    int32_t  ret;
    com_sockaddr_in_t address;
    sockaddr_in_t *dest;

    if ((sock < 0) || (sock >= NET_CELLULAR_MAX_CHANNEL_NBR))
    {
        NET_DBG_ERROR("invalid socket");
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        /* addr is an outbound address */
        if (addrlen == sizeof(com_sockaddr_in_t))
        {


            NET_DBG_INFO("Trying to connect socket %ld\n", sock);

            dest = (sockaddr_in_t *) addr;

            memset(&address, 0, sizeof(com_sockaddr_t));

            address.sin_len     = dest->sin_len;
            address.sin_family  = (uint8_t)COM_AF_INET;
            address.sin_port    = NET_HTONS(Generate_RngLocalPort(NET_NTOHS(dest->sin_port)));
            memcpy(&address.sin_addr.s_addr, &dest->sin_addr, sizeof(com_in_addr_t));

            if (com_connect(sock,
                            (com_sockaddr_t *)&address,
                            (int32_t)sizeof(com_sockaddr_in_t)) == COM_SOCKETS_ERR_OK)
            {
                NET_DBG_INFO("connect done on socket %ld\n", sock);
                ret = NET_OK;
                CellularChannel[sock].status = CELLULAR_CONNECTED_SOCKET_RW;
            }
            else
            {
                NET_DBG_ERROR("Socket %ld cannot be connected", sock);
                ret = NET_ERROR_SOCKET_FAILURE;
            }
        }
        else
        {
            NET_DBG_ERROR("Cannot initiate connect due to an invalid address length");
            ret = NET_ERROR_PARAMETER;
        }
    }
    return ret;
}

/**
  * @brief  Generate a Random Local Port between 49152 and 65535
  *
  * @param  Local port will be randomized if it is 0
  *         The IANA range for ephemeral ports is 49152-65535.
  *         This is mandatory for UG96 FW modem version until R02A08.
  *         Beyond this version, it is done in the modem.
  * @retval ret
  *          Random Local Port
  */

#define PORTBASE  49152
static uint16_t Generate_RngLocalPort(uint16_t localport)
{
    uint16_t RngPort;
    static uint16_t rnglocalport = 0;
    uint16_t random_number = 0;

    if (localport != 0)
    {
        RngPort = localport;
    }
    else
    {
        if (rnglocalport == 0)  /* just at first open since board reboot */
        {
            random_number = rand() & 0xffff;
            rnglocalport = ((uint16_t)(random_number & 0xFFFF) >> 2) + PORTBASE;
        }
        else /* from second time function execution, increment by one */
        {
            rnglocalport += 1;
        }

        if (rnglocalport < PORTBASE) /* Wrap-around */
        {
            rnglocalport = PORTBASE;
        }

        RngPort = rnglocalport;
    }
    return RngPort;
}



static int32_t conv(int32_t ret)
{
    if (ret > 0)
    {
        return ret;
    }
    switch (ret)
    {
    case 0:
        /* With DONT WAIT MODE return 0 when no data is available
        ret = NET_ERROR_CLOSE_SOCKET;  */
        ret = NET_TIMEOUT;

        break;

    case COM_SOCKETS_ERR_WOULDBLOCK:
        ret = NET_TIMEOUT;
        break;

    case COM_SOCKETS_ERR_CLOSING:
        ret = NET_ERROR_CLOSE_SOCKET;
        break;

    case COM_SOCKETS_ERR_TIMEOUT:
        ret = NET_TIMEOUT;
        break;
    }
    return ret;
}
/**
  * @brief  write data to the socket from buffer.
  *
  * @param  sock must be connected
  *         buf points to a buffer
  *         len is the amount of data available in the buffer
  *         If flags is set to NET_MSG_DONTWAIT,
  *           then send() will not block waiting for buffer to become free.
  * @retval ret
  *          <  0  : if there is an error.
  *          >= 0  : the amount of data which was sent.
  */
static int32_t net_cellular_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
    int32_t  ret;


    if ((sock < 0) || (sock >= NET_CELLULAR_MAX_CHANNEL_NBR))
    {
        NET_DBG_ERROR("invalid socket");
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        if (!(CellularChannel[sock].status & CELLULAR_SEND_OK))
        {
            NET_DBG_INFO("Socket %ld has shutdown its sending\n", sock);
            ret = NET_ERROR_SOCKET_FAILURE;
        }
        else
        {

            NET_DBG_INFO("Trying to send (%lu) on socket %ld\n", len, sock);

            ret = com_send(sock,
                           buf,
                           len,
                           (flags & NET_MSG_DONTWAIT) ? COM_MSG_DONTWAIT : COM_MSG_WAIT);
            if (ret < 0)
            {
                NET_DBG_ERROR("Send failed : %lu", ret);
            }
            else
            {
                NET_DBG_INFO("Data sent ! (%lu)\n", ret);
            }
            ret  = conv(ret);
        }
    }
    return ret;
}

/**
  * @brief  read data from the socket and write them into buffer.
  *
  * @param  sock must be connected
  *         buf points to a buffer
  *         len is the maximum amount of data allowed to be written in the buffer
  *         (all the data may not fill the entire buffer)
  *         If flags is set to NET_MSG_DONTWAIT,
  *           then recv() will not block if no data is available.
  * @retval ret
  *          < 0   : if there is an error.
  *          = 0   : no more data
  *                   or the remote host closed the connection gracefully (EOF)
  *          > 0   : the amount of data which was received.
  *                  (may be lower than len, data are reported at the earliest)
  */
static int32_t net_cellular_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
    int32_t ret = -1;

    if ((sock < 0) || (sock >= NET_CELLULAR_MAX_CHANNEL_NBR))
    {
        NET_DBG_ERROR("invalid socket");
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        if (!(CellularChannel [sock].status & CELLULAR_RECV_OK))
        {
            NET_DBG_INFO("Socket %ld has shutdown its receipt\n", sock);
            ret = NET_ERROR_SOCKET_FAILURE;
        }
        else
        {
            NET_DBG_INFO("com_recv in progress of %ld bytes\n", len);
            ret = com_recv(sock,
                           buf,
                           len,
                           (flags & NET_MSG_DONTWAIT) ? COM_MSG_DONTWAIT : COM_MSG_WAIT);

            if (ret > 0)
            {
                NET_DBG_INFO("Amount of received data (ret) = %d\n", ret);
                if (ret > len)
                {
                  NET_WARNING("Received more than requested (received %ld - requested %ld)\n", ret, len);
                }
            }
            ret = conv(ret);
        }
    }
    return ret;
}

static int32_t net_cellular_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *to,
                                   int32_t tolen)
{
    int32_t ret;

    com_sockaddr_in_t addr;
    addr.sin_len = sizeof(com_sockaddr_in_t);
    addr.sin_family = (to->sa_family == NET_AF_INET) ? COM_AF_INET : COM_AF_UNSPEC;
    addr.sin_port = ((sockaddr_in_t *)to)->sin_port;
    memcpy(&addr.sin_addr, &((sockaddr_in_t *)to)->sin_addr, 4);

    ret = com_sendto(sock, buf, len, flags, (com_sockaddr_t *) &addr, sizeof(com_sockaddr_in_t));

    return conv(ret);
}

static int32_t net_cellular_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *from,
                                     int32_t *fromlen)
{
    int32_t ret;
    com_sockaddr_t addr;
    int32_t addrlen = sizeof(com_sockaddr_t);

    ret = com_recvfrom(sock, buf, len, flags, &addr, &addrlen);

    if (ret >= 0)
    {
        memset(from, 0, *fromlen);
        ((sockaddr_in_t *)from)->sin_len = sizeof(sockaddr_in_t);
        ((sockaddr_in_t *)from)->sin_family = (addr.sa_family == COM_AF_INET) ? NET_AF_INET : NET_AF_UNSPEC;
        ((sockaddr_in_t *)from)->sin_port = ((com_sockaddr_in_t *)&addr)->sin_port;
        memcpy(&((sockaddr_in_t *)from)->sin_addr, &((com_sockaddr_in_t *)&addr)->sin_addr, 4);
    }

    return conv(ret);
}

/**
  * @brief  set options associated with a socket.
  *
  * @param  sock number
  *         level gives the layer on which the option resides
  *         level is always specified at socket level in this implementation
  *         optname
  *         optvalue points to a valid value when a option name is entered
  *         optlen
  * @retval ret
  *          < 0   : if there is an error.
  *          = 0   : on success
  */
static int32_t net_cellular_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue,
                                       int32_t optlen)
{
    int32_t ret = NET_ERROR_PARAMETER;

    if ((sock < 0) || (sock >= NET_CELLULAR_MAX_CHANNEL_NBR))
    {
        NET_DBG_ERROR("invalid socket");
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        switch (optname)
        {
        case NET_SO_RCVTIMEO:
            if (NULL != optvalue)
            {
                if (0 == com_setsockopt(sock, COM_SOL_SOCKET, COM_SO_RCVTIMEO, (int32_t *)optvalue, sizeof(int)))
                {
                    ret = NET_OK;
                }
            }
            break;

        case NET_SO_SNDTIMEO:
            if (NULL != optvalue)
            {
                if (0 == com_setsockopt(sock, COM_SOL_SOCKET, COM_SO_SNDTIMEO, (int32_t *)optvalue, sizeof(int)))
                {
                    ret = NET_OK;
                }
            }
            break;

        default:
            ret = NET_ERROR_UNSUPPORTED;
            NET_DBG_ERROR("Options %lu not supported in the modem", optname);
            break;
        }
    }

    return ret;
}


static int32_t net_cellular_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, int32_t *optlen)
{
    return NET_ERROR_UNSUPPORTED;
}
static int32_t net_cellular_getsockname(int32_t sock, sockaddr_t *name, int32_t *namelen)
{
    return NET_ERROR_UNSUPPORTED;
}
static int32_t net_cellular_getpeername(int32_t sock, sockaddr_t *name, int32_t *namelen)
{
    return NET_ERROR_UNSUPPORTED;
}

/**
  * @brief  Free a socket, disconnecting it from the remotehost, if necessary.
  *
  * @param  sock number
  *         isaclone, not supported
  * @retval ret
  *          < 0   : if there is an error.
  *          = 0   : on success
  */
static int32_t net_cellular_close(int32_t sock, bool isaclone)
{
    int32_t ret;

    if ((sock < 0) || (sock >= NET_CELLULAR_MAX_CHANNEL_NBR))
    {
        NET_DBG_ERROR("invalid socket");
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        NET_DBG_INFO("Trying to close socket %ld\n", sock);

        if (com_closesocket(sock) == COM_SOCKETS_ERR_OK)
        {
            NET_DBG_INFO("socket %ld is now closed\n", sock);
            ret = NET_OK;
        }
        else
        {
            NET_DBG_ERROR("socket %ld cannot be closed", sock);
            ret = NET_ERROR_SOCKET_FAILURE;
        }
        CellularChannel[sock].status = CELLULAR_FREE_SOCKET;
    }
    return ret;
}

static int32_t net_cellular_shutdown(int32_t sock, int32_t mode)
{
    int32_t ret = NET_OK;

    if ((sock < 0) || (sock >= NET_CELLULAR_MAX_CHANNEL_NBR))
    {
        NET_DBG_ERROR("invalid socket");
        ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
        NET_DBG_INFO("Force local Shutdown on socket %ld\n", sock);

        if (CellularChannel[sock].status &  CELLULAR_CONNECTED_SOCKET)
        {
            /* reading on the socket is no more possible */
            if (mode == NET_SHUTDOWN_R)
            {
                CellularChannel[sock].status &=  ~CELLULAR_RECV_OK;
            }

            /* writing on the socket is no more possible */
            if (mode == NET_SHUTDOWN_W)
            {
                CellularChannel[sock].status &=  ~CELLULAR_SEND_OK;
            }

            /* reading/writing on the socket is no more possible */
            if (mode == NET_SHUTDOWN_RW)
            {
                CellularChannel[sock].status &=  ~(CELLULAR_SEND_OK | CELLULAR_RECV_OK);
            }
        }
    }
    return ret;
}

static int32_t net_cellular_if_gethostbyname(net_if_handle_t *pnetif, sockaddr_t *addr, char_t *name)
{
    int32_t ret = NET_ERROR_DNS_FAILURE;
    com_sockaddr_t remoteaddr;

    if (addr->sa_len < sizeof(sockaddr_in_t))
    {
        return NET_ERROR_PARAMETER;
    }

    remoteaddr.sa_len = sizeof(com_sockaddr_t);

    if (COM_SOCKETS_ERR_OK == com_gethostbyname((const com_char_t *)name, &remoteaddr))
    {
        if (remoteaddr.sa_family == COM_AF_INET)
        {
            com_sockaddr_in_t *sremoteaddr = (com_sockaddr_in_t *) &remoteaddr;
            uint8_t len = addr->sa_len;
            sockaddr_in_t *saddr = (sockaddr_in_t *) addr;

            memset(saddr, 0, len);
            saddr->sin_len = len;
            saddr->sin_family = NET_AF_INET;
            memcpy(&(saddr->sin_addr), &sremoteaddr->sin_addr, 4);
            ret = NET_OK;
        }
    }

    return ret;
}

static int32_t net_cellular_ping(net_if_handle_t *pnetif, sockaddr_t *addr, int32_t count,
                                 int32_t delay,
                                 int32_t response[])
{
    int32_t ret = NET_OK;
    uint8_t     ipaddr[4];

    int32_t  ping_handle;
    int32_t  ping_result;
    uint8_t  ping_counter;

    com_sockaddr_in_t ping_target;
    com_ping_rsp_t    ping_rsp;

    memcpy(ipaddr, &addr->sa_data[2], 4);

    ping_target.sin_family      = (uint8_t)COM_AF_INET;
    ping_target.sin_port        = (uint16_t)0U;
    ping_target.sin_addr.s_addr = NET_ARTON(ipaddr);
    ping_target.sin_len = (uint8_t)sizeof(com_sockaddr_in_t);

    ping_handle = com_ping();

    if (ping_handle >= 0)
    {
        for (ping_counter = 0; ping_counter < count; ping_counter++)
        {
            ping_result = com_ping_process(ping_handle,
                                           (const com_sockaddr_t *)&ping_target,
                                           (int32_t)ping_target.sin_len,
                                           delay, &ping_rsp);

            if ((ping_result == COM_SOCKETS_ERR_OK)
                    && (ping_rsp.status == COM_SOCKETS_ERR_OK))
            {
                NET_DBG_INFO("Ping: %d bytes from %d.%d.%d.%d: time= %dms\n\r",
                             ping_rsp.bytes,
                             ipaddr[0],
                             ipaddr[1],
                             ipaddr[2],
                             ipaddr[3],
                             ping_rsp.time);
                response[ping_counter] = ping_rsp.time;
            }
            else
            {
                if (ping_result == COM_SOCKETS_ERR_TIMEOUT)
                {
                    NET_DBG_INFO("Ping: timeout from %d.%d.%d.%d\n\r",
                                 ipaddr[0],
                                 ipaddr[1],
                                 ipaddr[2],
                                 ipaddr[3]);
                    ret = NET_TIMEOUT;
                }
                else
                {
                    NET_DBG_INFO("Ping: error from %d.%d.%d.%d\n\r",
                                 ipaddr[0],
                                 ipaddr[1],
                                 ipaddr[2],
                                 ipaddr[3]);
                    ret = NET_ERROR_GENERIC;
                }
            }
        }

        if (com_closeping(ping_handle) != COM_SOCKETS_ERR_OK)
        {
            NET_DBG_ERROR("ping cannot be closed");
        }
    }
    else
    {
        /* Ping handle not received */
        NET_DBG_ERROR("Ping: low-level not ready");
        ret = ping_handle;
    }

    return ret;
}


static int32_t net_cellular_get_radio_info(net_cellular_radio_results_t *results)
{
    int32_t ret = NET_ERROR_PARAMETER;

    if (DC_SERVICE_ON == dc_cellular_info.rt_state)
    {
        results->signal_level_db = (int8_t)dc_cellular_info.cs_signal_level_db;
        ret = NET_OK;
    }
    return ret;
}

#define ONESECOND       1000

static void cellif_input(void const  *argument)
{
    net_if_handle_t *pnetif = (net_if_handle_t *)argument;
    osEvent event;
    dc_com_event_id_t dc_event_id;
    bool  registrationMessage = false;
    bool  ModuleMessage = false;
    int32_t   levelMessage = 1;
    int32_t   tickstart;
    int32_t   tickcurrent;
    int32_t   connection_iteration = 0;


    const net_cellular_credentials_t *cellular_data =  pnetif->pdrv->extension.cellular->credentials;


    tickstart = HAL_GetTick();
    cellular_set_config(cellular_data->apn,
                        cellular_data->username,
                        cellular_data->password,
                        cellular_data->use_internal_sim);

    cellular_start();
    net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);

    /* Registration for Cellular Data Cache */
    dc_com_register_gen_event_cb(&dc_com_db, cellular_notif_cb, (void *) NULL);


    memset((void *)&dc_nifman_info,     0, sizeof(dc_nifman_info_t));
    memset((void *)&dc_cellular_info,   0, sizeof(dc_cellular_info_t));
    memset((void *)&dc_sim_info,        0, sizeof(dc_sim_info_t));

    /* Infinite loop */
    for (;;)
    {
        event = osMessageGet(cellular_queue, ONESECOND);

        if (event.status == osEventMessage)
        {
            dc_event_id = (dc_com_event_id_t)event.value.v;
            NET_DBG_INFO(" \n\n****> event %d get from cellular_queue\n\n", dc_event_id);

            if (dc_event_id == DC_COM_CELLULAR_INFO)
            {
                NET_DBG_INFO("\n***** cellular_info available *****\n");

                dc_com_read(&dc_com_db,
                            DC_COM_CELLULAR_INFO,
                            (void *)&dc_cellular_info,
                            sizeof(dc_cellular_info));
                NET_DBG_INFO("- cellular_info cb -\n");

                NET_DBG_INFO("   state              = %d\n", dc_cellular_info.rt_state);

                if ((dc_cellular_info.rt_state == DC_SERVICE_RUN) || (dc_cellular_info.rt_state == DC_SERVICE_ON))
                {
                    NET_DBG_INFO("   modem_state        = %d\n", dc_cellular_info.modem_state);
                    NET_DBG_INFO("   mno_name           = %s\n", dc_cellular_info.mno_name);
                    NET_DBG_INFO("   state              = %d\n", dc_cellular_info.rt_state);
                    NET_DBG_INFO("   modem_state        = %d\n", dc_cellular_info.modem_state);
                    NET_DBG_INFO("   cs_signal_level    = %d\n", (uint8_t)dc_cellular_info.cs_signal_level);
                    NET_DBG_INFO("   cs_signal_level_db = %d\n", (int8_t)dc_cellular_info.cs_signal_level_db);
                    NET_DBG_INFO("   imei               = %s\n", (char *)&dc_cellular_info.imei);
                    NET_DBG_INFO("   manufacturer_name  = %s\n", (char *)&dc_cellular_info.manufacturer_name);
                    NET_DBG_INFO("   model              = %s\n", (char *)&dc_cellular_info.model);
                    NET_DBG_INFO("   revision           = %s\n", (char *)&dc_cellular_info.revision);
                    NET_DBG_INFO("   serial_number      = %s\n", (char *)&dc_cellular_info.serial_number);
                    NET_DBG_INFO("   iccid              = %s\n", (char *)&dc_cellular_info.iccid);
                    NET_DBG_INFO("--------------------\n");

                    if (registrationMessage == false)
                    {
                        if (strlen((char *)&dc_cellular_info.mno_name) > 0)
                        {
                            NET_PRINT("\nC2C module registered\n");
                            tickcurrent = HAL_GetTick() - tickstart;
                            NET_PRINT("Registration done in %ld msseconds\n", tickcurrent);
                            NET_PRINT("Retrieving the cellular operator: %s\n",
                                      (char *)&dc_cellular_info.mno_name);
                            registrationMessage = true;
                        }
                    }

                    if (ModuleMessage == false)
                    {
                        if (strlen((char *)&dc_cellular_info.manufacturer_name) > 0)
                        {
                            NET_PRINT("Module initialized successfully: %s\n",
                                      (char *)&dc_cellular_info.manufacturer_name);
                            NET_PRINT("ProductID: %s\n",
                                      (char *)&dc_cellular_info.model);
                            NET_PRINT("FW version: %s\n",
                                      (char *)&dc_cellular_info.revision);
                            NET_PRINT("SIM Id (IccID): %s\n",
                                      (char *)&dc_cellular_info.iccid);
                            ModuleMessage = true;

                            /* Retrieve the Cellular module information */
                            strncpy(pnetif->DeviceName, (char *)&dc_cellular_info.manufacturer_name,
                                    NET_DEVICE_NAME_LEN);
                            strncpy(pnetif->DeviceID, (char *)&dc_cellular_info.model, NET_DEVICE_ID_LEN);
                            strncpy(pnetif->DeviceVer, (char *)&dc_cellular_info.revision, NET_DEVICE_VER_LEN);

                            /* Initialise the Channels*/
                            for (int32_t i = 0; i < NET_CELLULAR_MAX_CHANNEL_NBR; i++)
                            {
                                CellularChannel[i].status          = CELLULAR_FREE_SOCKET;
                            }
                            net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
                        }
                    }
                }

                /* Check switch off  */
                if ((stop_requested) && (dc_cellular_info.rt_state != DC_SERVICE_ON))
                {
                    NET_DBG_INFO("Modem is powered off\n\n");
                    stop_requested = false;
                    net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
                    osThreadTerminate(CellIfThreadId);
                    CellIfThreadId = NULL;
                }

                if (dc_cellular_info.rt_state == DC_SERVICE_ON)
                {
                    /* check signal level availability or not, displayed only once */
                    if (levelMessage != dc_cellular_info.cs_signal_level_db)
                    {
                        if (dc_cellular_info.cs_signal_level_db != 0)
                        {
                            NET_PRINT("\nSignal Level: %d dBm \n",
                                      (int8_t)dc_cellular_info.cs_signal_level_db);
                        }
                        else
                        {
                            NET_PRINT("Signal not known or not detectable yet (be patient)\n");
                        }
                        levelMessage = dc_cellular_info.cs_signal_level_db;
                    }
                }
            }
            else if (dc_event_id == DC_COM_CELLULAR_DATA_INFO)
            {
                NET_DBG_INFO("\n***** cellular_data_info available *****\n");
                dc_com_read(&dc_com_db,
                            DC_COM_CELLULAR_DATA_INFO,
                            (void *)&dc_cellular_data_info,
                            sizeof(dc_cellular_data_info));
                NET_DBG_INFO("- dc_cellular_data_info cb -\n");
                NET_DBG_INFO("   state              = %d\n", dc_cellular_data_info.rt_state);
                NET_DBG_INFO("----------------------------\n");
            }
            else if (dc_event_id == DC_COM_SIM_INFO)
            {
                /* check SIM availability */
                dc_com_read(&dc_com_db,
                            DC_COM_SIM_INFO,
                            (void *)&dc_sim_info,
                            sizeof(dc_sim_info));
                NET_DBG_INFO("\n***** sim_info available *****\n");
                NET_DBG_INFO("   state              = %d\n", dc_sim_info.rt_state);
                if (dc_sim_info.rt_state == DC_SERVICE_ON)
                {
                    NET_DBG_INFO("- dc_sim_info cb -\n");
                    NET_DBG_INFO("   active_slot        = %d\n", dc_sim_info.active_slot);
                    NET_DBG_INFO("   sim_status         = %d\n", dc_sim_info.sim_status[dc_sim_info.active_slot]);
                    NET_DBG_INFO("------------------\n");
                    if (dc_sim_info.sim_status[dc_sim_info.active_slot] == DC_SIM_NOT_INSERTED)
                    {
                        NET_PRINT("SIM is not inserted\n");
                    }
                }
            }
            else if (dc_event_id == DC_COM_NIFMAN_INFO)
            {
                NET_DBG_INFO("\n***** nifman_info available *****\n");
                dc_com_read(&dc_com_db,
                            DC_COM_NIFMAN_INFO,
                            (void *)&dc_nifman_info,
                            sizeof(dc_nifman_info));

                if (connection_requested && dc_nifman_info.rt_state == DC_SERVICE_ON)
                {
                    NET_DBG_INFO(" +++++++++++++++++++ Cellular is Data READY +++++++++++++++++++++\n\n");
                    NET_DBG_INFO(" +++++++++++++++++++ IP @ allocated +++++++++++++++++++++\n\n");
                    pnetif->ipaddr = dc_nifman_info.ip_addr.addr;
                    connection_requested = false;
                    net_state_manage_event(pnetif, NET_EVENT_IPADDR);
                }
                NET_DBG_INFO("- nifman_info cb -\n");
                NET_DBG_INFO("   state              = %d\n", dc_nifman_info.rt_state);
                NET_DBG_INFO("   network            = %d\n", dc_nifman_info.network);
                NET_DBG_INFO("   IP@                = %lu\n", dc_nifman_info.ip_addr.addr);
                NET_DBG_INFO("------------------\n");
#if (USE_SOCKETS_TYPE == USE_SOCKETS_LWIP)
            }
            else if (dc_event_id == DC_COM_PPP_CLIENT_INFO)
            {
                NET_DBG_INFO("\n***** ppp_client_info available *****\n");
                dc_com_read(&dc_com_db,
                            DC_COM_PPP_CLIENT_INFO,
                            (void *)&dc_ppp_client_info,
                            sizeof(dc_ppp_client_info));
                NET_DBG_INFO("-- ppp_client_info cb --\n");
                NET_DBG_INFO("   state              = %d\n", dc_ppp_client_info.rt_state);
                NET_DBG_INFO("   IP@                = %lu\n", dc_ppp_client_info.ip_addr.addr);
                NET_DBG_INFO("-------------------------\n");
#endif /* USE_SOCKETS_LWIP */
            }
            else
            {
                NET_DBG_INFO("Unexpected Event message : %d \n", dc_event_id);
            }
        }


        if ((registrationMessage == false) || (ModuleMessage == false))
        {
            NET_PRINT_WO_CR(".");
            connection_iteration++;
            if ((connection_iteration % 60) == 0)
            {
                NET_PRINT(" ");
            }
        }
    }
}

/**
  * @brief  Configure the SIM(s)
  *
  * @param  Access Point Name
  *         Username
  *         password
  *         APN, Username and password are all provided by the cellular operator
  *         use_internal_sim,
  *         - true for the external slot with a dynamic APN
  *         - false with the embedded SIM with a static APN
  */
static void cellular_set_config(const char *oper_ap_code,
                                const char *username,
                                const char *password,
                                bool use_internal_sim)
{
    dc_cellular_params_t cellular_params;

    memset((void *)&cellular_params,     0, sizeof(dc_cellular_params_t));
    dc_com_read(&dc_com_db, DC_COM_CELLULAR_PARAM, (void *)&cellular_params, sizeof(cellular_params));



    if (use_internal_sim == true)
    {
        /* Specify which SIM slot to use at cellular boot time */
        cellular_params.sim_slot[0].sim_slot_type = DC_SIM_SLOT_MODEM_EMBEDDED_SIM;
        cellular_params.sim_slot_nb = 1;

        /* Specify operator credencials : Access Point Number, username and password */
        memcpy(cellular_params.sim_slot[0].apn, "EM", (size_t)sizeof("EM"));

        /* Specify the Context ID */
        cellular_params.sim_slot[0].cid          = CS_PDN_USER_CONFIG_1;

        NET_PRINT("Trying to connect with the embedded SIM\n");
    }
    else
    {
        /* Specify which SIM slot to use at cellular boot time */
        cellular_params.sim_slot[0].sim_slot_type = DC_SIM_SLOT_MODEM_SOCKET;
        cellular_params.sim_slot[1].sim_slot_type = DC_SIM_SLOT_MODEM_EMBEDDED_SIM;
        cellular_params.sim_slot_nb = 2;

        /* Specify operator credencials : Access Point Number, username and password */
        memcpy(cellular_params.sim_slot[0].apn, oper_ap_code, strlen(oper_ap_code)+1);
        memcpy(cellular_params.sim_slot[0].username, username, strlen(username)+1);
        memcpy(cellular_params.sim_slot[0].password, password, strlen(password)+1);

        memcpy(cellular_params.sim_slot[1].apn, "EM", (size_t)sizeof("EM"));


        /* Specify the Context ID */
        cellular_params.sim_slot[0].cid          = CS_PDN_USER_CONFIG_1;
        cellular_params.sim_slot[1].cid          = CS_PDN_USER_CONFIG_1;

        NET_PRINT("Trying to connect with the external SIM\n");
    }

    /* Initialize all other fields */
    cellular_params.set_pdn_mode = 1U;
    cellular_params.target_state = DC_TARGET_STATE_SIM_ONLY;
    cellular_params.nfmc_active  = 0U;
    /*cellular_params.attachment_timeout  = CELLULAR_ATTACHMENT_TIMEOUT;*/

    dc_com_write(&dc_com_db, DC_COM_CELLULAR_PARAM, (void *)&cellular_params, sizeof(cellular_params));
}

static void cellular_notif_cb(dc_com_event_id_t dc_event_id, const void *private_gui_data)
{
    if ((dc_event_id == DC_COM_CELLULAR_INFO) ||
#if (USE_SOCKETS_TYPE == USE_SOCKETS_LWIP)
            (dc_event_id == DC_COM_PPP_CLIENT_INFO) ||
#endif /* USE_SOCKETS_LWIP */
            (dc_event_id == DC_COM_CELLULAR_DATA_INFO) ||
            (dc_event_id == DC_COM_NIFMAN_INFO) ||
            (dc_event_id == DC_COM_SIM_INFO))
    {
        NET_DBG_INFO(" \n\n****> event %d put in cellular_queue\n\n", dc_event_id);
        while (osMessagePut(cellular_queue, (uint32_t)dc_event_id, osWaitForever) != osOK);
    }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
