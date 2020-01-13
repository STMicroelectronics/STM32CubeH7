/**
  ******************************************************************************
  * @file    net_ip_lwip.c
  * @author  MCD Application Team
  * @brief   lwIP network interface functions
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
#include "net_ip_lwip.h"
#include "net_buffers.h"

#include "lwip/netdb.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"

#define MAX_MTU 1500

#if (osCMSIS >= 0x20000U)
#define osSemaphoreWait         osSemaphoreAcquire
#endif /* osCMSIS */

typedef struct
{
    struct netif *netif;
    net_buf_t      *p;
    uint32_t       tin;
} net_ip_input_queue_item_t;

extern struct netif *netif_list;

static osMessageQId net_ip_input_queue_handle = NULL;
static osSemaphoreId net_ip_input_semaphore_handle = NULL;
static osSemaphoreId net_ip_if_semaphore_handle = NULL;
static osThreadId net_ip_input_thread_handle = NULL;
static uint8_t net_ip_if_number = 0, tcp_ip_started = 0;

static struct pbuf *net_buf2pbuf(net_buf_t **p);
static net_buf_t *pbuf2net_buf(struct pbuf **p);

static  err_t lwip_output_to_driver(struct netif *netif,  struct pbuf *p);
static void lwip_status_cb(struct netif *netif);
static void net_ip_link_status(void *context, uint8_t status);

static  err_t lwip_default_init_if(struct netif *netif);
static int32_t net_ip_queue_input(void *context, net_buf_t *p);
static  void net_ip_input_task(void const *param);



static void net_ip_init(uint32_t        flag);
static void net_ip_deinit(void);
static int32_t net_ip_start(void);
static void net_ip_stop(void);



 void link_socket_to_lwip(net_if_drv_t *drv);
static int32_t net_lwip_socket(int32_t domain, int32_t type, int32_t protocol);
static int32_t net_lwip_bind(int32_t sock, const sockaddr_t *addr, int32_t addrlen);
static int32_t net_lwip_listen(int32_t sock, int32_t backlog);
static int32_t net_lwip_accept(int32_t sock, sockaddr_t *addr, int32_t *addrlen);
static int32_t net_lwip_connect(int32_t sock, const sockaddr_t *addr, int32_t addrlen);
static int32_t net_lwip_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t net_lwip_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t net_lwip_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *to, int32_t tolen);
static int32_t net_lwip_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *from,
                                 int32_t *fromlen);
static int32_t net_lwip_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue, int32_t optlen);
static int32_t net_lwip_getsockopt(int32_t sock, int32_t level, int32_t optname,  void *optvalue, int32_t *optlen);
static int32_t net_lwip_getsockname(int32_t sock, sockaddr_t *name, int32_t *namelen);
static int32_t net_lwip_getpeername(int32_t sock, sockaddr_t *name, int32_t *namelen);
static int32_t net_lwip_close(int32_t sock, bool clone);
static int32_t net_lwip_shutdown(int32_t sock, int32_t mode);
static int32_t net_lwip_gethostbyname(net_if_handle_t *pnetif, sockaddr_t *addr, char_t *name);

/*************************************************************************************************************/
/*                                      interface definition for this lib                                     */
/*              this the only symbol  known outside of this file                                             */

iplib_t iplib_lwip =
{
    net_ip_add_if,
    net_ip_remove_if,
    net_ip_connect,
    net_ip_disconnect
};


static struct pbuf *net_buf2pbuf(net_buf_t **p)
{
    return (struct pbuf *) *p;
}

static net_buf_t *pbuf2net_buf(struct pbuf **p)
{
    return (net_buf_t *) *p;
}



/*    Library initialization , should happen once       */
static  void net_ip_init(uint32_t flag)
{
  if(!tcp_ip_started)
  {
    if ((flag & NET_IP_FLAG_TCPIP_STARTED_EXTERNALLY)==0)
    {
      tcpip_init(NULL, NULL);
      netif_set_default(NULL);
    }
    tcp_ip_started=1;
  }
  
#if (osCMSIS >= 0x20000U)
    net_ip_if_semaphore_handle = (osSemaphoreId) osSemaphoreNew(1,1,NULL);
#else
      osSemaphoreDef(net_ip_if_semaphore);
    net_ip_if_semaphore_handle = (osSemaphoreId) osSemaphoreCreate(osSemaphore(net_ip_if_semaphore), 1);
#endif /* osCMSIS */

}

/*    Library deinitialization , should happen once       */
static void net_ip_deinit(void)
{
    if (net_ip_if_semaphore_handle != NULL)
    {
        (void) osSemaphoreDelete(net_ip_if_semaphore_handle);
        net_ip_if_semaphore_handle = NULL;
    }
    /*tcpip_deinit(NULL, NULL);  TODO */
}

#if (osCMSIS >= 0x20000U )

static const   osThreadAttr_t attr = { 
      .name= "NET_IP_INPUT", 
      .priority =osPriorityAboveNormal,
    . stack_size = NET_IP_THREAD_SIZE };
#endif

/*    Library start , should happen once       */
static int32_t net_ip_start(void)
{
    int32_t  ret;
#if (osCMSIS >= 0x20000U)
    net_ip_input_semaphore_handle = (osSemaphoreId) osSemaphoreNew(1,1,NULL);
#else
    osSemaphoreDef(net_ip_input_semaphore);
    net_ip_input_semaphore_handle = (osSemaphoreId) osSemaphoreCreate(osSemaphore(net_ip_input_semaphore), 1);
#endif /* osCMSIS */
    
#if (osCMSIS >= 0x20000U)
  net_ip_input_queue_handle = osMessageQueueNew(NET_IP_INPUT_QUEUE_SIZE,sizeof(net_ip_input_queue_item_t *),NULL);
#else
    osMessageQDef(net_ip_input_queue, NET_IP_INPUT_QUEUE_SIZE, net_ip_input_queue_item_t *);
    net_ip_input_queue_handle = osMessageCreate(osMessageQ(net_ip_input_queue), NULL);
#endif /* osCMSIS */

#if (osCMSIS < 0x20000U )
   
    osThreadDef(NET_IP_INPUT, net_ip_input_task, osPriorityAboveNormal, 0, NET_IP_THREAD_SIZE);
    net_ip_input_thread_handle = osThreadCreate(osThread(NET_IP_INPUT), NULL);
#else
    net_ip_input_thread_handle = osThreadNew((osThreadFunc_t)net_ip_input_task,NULL,&attr);
#endif
    if ((net_ip_input_semaphore_handle == NULL) || (net_ip_input_queue_handle == NULL)
            || (net_ip_input_thread_handle == NULL))
    {
        net_ip_stop();
        ret = NET_ERROR_NO_MEMORY;
    }
    else
    {
        netif_list = NULL;
        ret = NET_OK;
    }

    return ret;
}

/*    Library stop , should happen once       */
static void net_ip_stop(void)
{
    if (net_ip_input_thread_handle != NULL)
    {
        (void) osThreadTerminate(net_ip_input_thread_handle);
        net_ip_input_thread_handle = NULL;
    }

    if (net_ip_input_queue_handle != NULL)
    {
#if (osCMSIS < 0x20000U )
        (void) osMessageDelete(net_ip_input_queue_handle);
#else
        (void) osMessageQueueDelete(net_ip_input_queue_handle);
#endif
        
        net_ip_input_queue_handle = NULL;
    }

    if (net_ip_input_semaphore_handle != NULL)
    {
        (void) osSemaphoreDelete(net_ip_input_semaphore_handle);
        net_ip_input_semaphore_handle = NULL;
    }
}

/*    Add of a new network interface       */
static int32_t net_ip_add_if(net_if_handle_t *pnetif, int32_t (*if_output)(void *context,net_buf_t *p), uint32_t flag)
{
    struct netif *netif;
#if (osCMSIS < 0x20000U)
    osSemaphoreDef(connection_semaphore);
#endif
    net_ip_if_t *net_ip_if = NULL;

    if (if_output == NULL)
    {
        return NET_ERROR_PARAMETER;
    }

    if (NULL == net_ip_if_semaphore_handle)
    {
        net_ip_init(flag);
    }

    if (osSemaphoreWait(net_ip_if_semaphore_handle, NET_IP_IF_TIMEOUT) != 0)
    {
        return NET_TIMEOUT;
    }

    if (0U == net_ip_if_number)
    {
        if (net_ip_start() != NET_OK)
        {
            goto error;
        }
    }

    net_ip_if = (net_ip_if_t *) NET_MALLOC(sizeof(net_ip_if_t));
    if (NULL == net_ip_if)
    {
        goto error;
    }
    (void) memset(net_ip_if, 0x00, sizeof(net_ip_if_t));


    pnetif->net_ip_if = (net_ip_if_t *) net_ip_if;
    net_ip_if->output = if_output;
#if (osCMSIS >= 0x20000U)
    net_ip_if->connection_semaphore_handle = (osSemaphoreId) osSemaphoreNew(1,1,NULL);
#else
    net_ip_if->connection_semaphore_handle = (osSemaphoreId) osSemaphoreCreate(osSemaphore(connection_semaphore), 1);
#endif /* osCMSIS */

    if (net_ip_if->connection_semaphore_handle == NULL)
    {
        goto error;
    }


    pnetif->iplib_input_data = net_ip_queue_input;
    pnetif->iplib_notify_link_change = net_ip_link_status;


    netif = NET_MALLOC(sizeof(struct netif));
    if (NULL == netif)
    {
        goto error;
    }
    (void) memset(netif, 0x00, sizeof(struct netif));

    net_ip_if->obj = netif_add(netif, NULL, NULL, NULL, NULL, lwip_default_init_if, &tcpip_input);


    netif_set_down(netif);
    netif_set_link_down(netif);
    if (flag & NET_IP_FLAG_DEFAULT_INTERFACE)
    {
        netif_set_default(netif);
    }

    netif_set_status_callback(netif, lwip_status_cb);
    netif_set_link_callback(netif, lwip_status_cb);
    /* output to the device */
    netif->linkoutput = lwip_output_to_driver;
    netif->state = pnetif;

    net_ip_if_number++;

    /* link current network interface to LWIP library */
    link_socket_to_lwip(pnetif->pdrv);


    (void) osSemaphoreRelease(net_ip_if_semaphore_handle);

    /* set netif maximum transfer unit */
    netif->mtu = MAX_MTU;

    netif_set_up(netif);

    return NET_OK;

error:
    pnetif->net_ip_if = NULL;
    if (net_ip_if_number == 0U)
    {
        net_ip_stop();
    }
    if (net_ip_if != NULL)
    {
        if (net_ip_if->connection_semaphore_handle != NULL)
        {
            (void) osSemaphoreDelete(net_ip_if->connection_semaphore_handle);
        }
        NET_FREE(net_ip_if);
    }


    (void) osSemaphoreRelease(net_ip_if_semaphore_handle);
    return NET_ERROR_GENERIC;
}

static int32_t net_ip_remove_if(net_if_handle_t *pnetif)
{

    if (osSemaphoreWait(net_ip_if_semaphore_handle, NET_IP_IF_TIMEOUT) != 0)
    {
        return NET_TIMEOUT;
    }

    if(pnetif->net_ip_if)
    {
        struct netif     *netif = pnetif->net_ip_if->obj;
        if(netif)
        {
            netif_set_down(netif);
            netif_set_link_down(netif);
            netif_remove(netif);
            //NET_FREE(netif ->state);
            NET_FREE(netif);
            net_ip_if_number--;
        }
        pnetif->net_ip_if = NULL;
    }
    if (net_ip_if_number == 0U)
    {
        net_ip_deinit();
    }
    else
      osSemaphoreRelease(net_ip_if_semaphore_handle);

    return NET_OK;
}

static  err_t lwip_default_init_if(struct netif *netif)
{
    char *hostname =  NET_MALLOC(sizeof(char) * ((uint16_t) NET_IP_HOSTNAME_MAX_LEN + 1U));
    if (hostname == NULL)
    {
        return (err_t) ERR_MEM;
    }

    (void) snprintf(hostname, NET_IP_HOSTNAME_MAX_LEN + 1, "generic eth if #%d", netif->num);

    netif->hostname = hostname;
    netif->name[0] = 's';
    netif->name[1] = 't';

    netif->hwaddr_len = 6;

    netif->mtu = MAX_MTU;

    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
    netif->output = etharp_output;


#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

    return (err_t) ERR_OK;
}

/*          CONNECTION      */
static int32_t net_ip_connect(net_if_handle_t *pnetif)
{
    net_ip_if_t     *net_ip_if = pnetif->net_ip_if;
    struct netif     *netif = pnetif->net_ip_if->obj;

    if (NULL == net_ip_if)
    {
        return NET_ERROR_PARAMETER;
    }

    if (!pnetif->dhcp_mode && (pnetif->static_ipaddr == 0U))
    {
        return NET_ERROR_PARAMETER;
    }


    /* inform netif about MAC hardware address */
    (void) memcpy(netif->hwaddr, pnetif->macaddr.mac, sizeof(netif->hwaddr));


    if (pnetif->dhcp_mode)
    {
        (void) dhcp_start(netif);
    }
    else
    {

        netif_set_addr(net_ip_if->obj, (ip4_addr_t const *) &pnetif->static_ipaddr, (ip4_addr_t const *) &pnetif->static_netmask,
                       (ip4_addr_t const *) &pnetif->static_gateway); 



        dhcp_inform(netif);
    }

    return NET_OK;
}

static int32_t net_ip_disconnect(net_if_handle_t *pnetif)
{
    net_ip_if_t     *net_ip_if = pnetif->net_ip_if;


    if (netif_dhcp_data((struct netif *)net_ip_if->obj) != 0)
    {
        (void) dhcp_release(net_ip_if->obj);
        dhcp_stop(net_ip_if->obj);
        dhcp_cleanup(net_ip_if->obj);
    }
    else
    {
        netif_set_addr(net_ip_if->obj, NULL, NULL, NULL);
        pnetif->dhcp_inform_flag = 0;
        dhcp_inform(net_ip_if->obj);
    }

    return (int32_t) ERR_OK;
}
#if 0
static void net_ip_clear_connect(net_ip_if_t *net_ip_if)
{
    if (netif_dhcp_data((struct netif *)net_ip_if->obj))
    {
        dhcp_release(net_ip_if->obj);
        dhcp_stop(net_ip_if->obj);
        dhcp_cleanup(net_ip_if->obj);
    }
    netif_set_addr(net_ip_if->obj, NULL, NULL, NULL);
    netif_set_link_callback(net_ip_if->obj, NULL);
    netif_set_down(net_ip_if->obj);
}
#endif /* 0 */

static void net_ip_input_task(void const *param)
{
    struct netif *netif;
    net_ip_input_queue_item_t *queue_item;

    while (true)
    {
#if (osCMSIS < 0x20000U)
       osEvent event;
       event = osMessageGet(net_ip_input_queue_handle, osWaitForever);
       if (event.status == osEventMessage)
       {
         queue_item = (net_ip_input_queue_item_t *) event.value.p;
#else
       osStatus status;
       status = osMessageQueueGet(net_ip_input_queue_handle, &queue_item, NULL, osWaitForever);
       if (status == osOK)
       {
#endif
            (void) osSemaphoreWait(net_ip_input_semaphore_handle, osWaitForever);
            for (netif = netif_list; netif != NULL; netif = netif->next)
            {
                if (netif == queue_item->netif)
                {
                    break;
                }
            }

            if ((netif == NULL) || (netif->input(net_buf2pbuf(&queue_item->p), netif) != ERR_OK))
            {
                (void) pbuf_free((struct pbuf *) queue_item->p);
            }

            (void) osSemaphoreRelease(net_ip_input_semaphore_handle);
            NET_FREE(queue_item);
        }
    }
}

static int32_t net_ip_queue_input(void *context, net_buf_t *p)
{
    net_ip_if_t *net_ip_if = ((net_if_handle_t *) context)->net_ip_if;
    struct netif *netif;

    /* check netif validity */
    for (netif = netif_list; netif != NULL; netif = netif->next)
    {
        if (netif == net_ip_if->obj)
        {
            break;
        }
    }
    if (netif == NULL)
    {
        return (int32_t) ERR_IF;
    }

    netif->input((struct pbuf *) p, netif);
    return (int32_t) ERR_OK;
}

static  err_t lwip_output_to_driver(struct netif *netif,  struct pbuf *p)
{
    err_t err_val;

    net_if_handle_t       *pnetif = (net_if_handle_t *) netif->state;
    net_ip_if_t *net_ip_if = pnetif->net_ip_if;

    if (net_ip_if->output == NULL)
    {
        return (err_t) ERR_IF;
    }


    err_val = net_ip_if->output(netif,pbuf2net_buf(&p));

    return err_val;
}


static void net_ip_link_status(void *context, uint8_t status)
{
    net_if_handle_t *pnetif = context;
    if (status != 0U)
    {
        netif_set_link_up(pnetif->net_ip_if->obj);
    }
    else
    {
        netif_set_link_down(pnetif->net_ip_if->obj);
    }
}

static void lwip_status_cb(struct netif *netif)
{
    net_if_handle_t *pnetif = netif->state;

    if(pnetif->dhcp_enabled)
    {
        /* lost connection */
        if(netif->ip_addr.addr && !netif_is_link_up(netif) && pnetif->dhcp_release_on_link_lost)
        {
            dhcp_release(netif);
            dhcp_start(netif);
        }

        /* up connection , so need to inform other at first time */
        if(pnetif->dhcp_inform_flag && netif_is_link_up(netif))
        {
            dhcp_inform(netif);
            pnetif->dhcp_inform_flag = 0;
        }
    }




    if ( pnetif->ipaddr != netif->ip_addr.addr)
    {
        pnetif->ipaddr = netif->ip_addr.addr;
     
        pnetif->netmask = netif->netmask.addr;  
        pnetif->gateway = netif->gw.addr;

        if (pnetif->ipaddr) net_state_manage_event(pnetif, NET_EVENT_IPADDR);
    }

    if ( netif_is_link_up(netif))
    {
        net_state_manage_event(pnetif, NET_EVENT_LINK_UP);
    }

    if ( !netif_is_link_up(netif))
    {
        net_state_manage_event(pnetif, NET_EVENT_LINK_DOWN);
    }

}




/*       UTILITIES          */

void link_socket_to_lwip(net_if_drv_t *drv)
{

    drv->socket = net_lwip_socket;
    drv->bind = net_lwip_bind;
    drv->listen = net_lwip_listen;
    drv->accept = net_lwip_accept;
    drv->connect = net_lwip_connect;
    drv->send = net_lwip_send;
    drv->recv = net_lwip_recv;
    drv->sendto = net_lwip_sendto;
    drv->recvfrom = net_lwip_recvfrom;
    drv->setsockopt = net_lwip_setsockopt;
    drv->getsockopt = net_lwip_getsockopt;
    drv->getsockname = net_lwip_getsockname;
    drv->getpeername = net_lwip_getpeername;
    drv->close = net_lwip_close;
    drv->shutdown = net_lwip_shutdown;

    /* Service */
    drv->gethostbyname = net_lwip_gethostbyname;
}


#define LWIP_ERROR_TO_NETERROR(a)       a

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_socket(int32_t domain, int32_t type, int32_t protocol)
{
    int32_t socket;
    socket = (int) lwip_socket(domain, type, protocol);
    return LWIP_ERROR_TO_NETERROR(socket);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */

static int32_t net_lwip_bind(int32_t socket, const sockaddr_t *addr, int32_t addrlen)
{
    int32_t   ret = lwip_bind(socket, (struct sockaddr const *)addr, (uint32_t) addrlen);
    return LWIP_ERROR_TO_NETERROR(ret);
}


/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_listen(int32_t sock, int32_t backlog)
{
    int32_t ret;
    ret = lwip_listen(sock, backlog);
    return LWIP_ERROR_TO_NETERROR(ret);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_accept(int32_t sock, sockaddr_t *addr, int32_t *addrlen)
{
    int32_t ret;
    ret = lwip_accept(sock, (struct sockaddr *)addr, (u32_t *)addrlen);
    return LWIP_ERROR_TO_NETERROR(ret);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_connect(int32_t sock, const sockaddr_t *addr, int32_t addrlen)
{
    int32_t ret;
    ret = lwip_connect(sock, (const struct sockaddr *)addr, (u32_t)addrlen);
    return LWIP_ERROR_TO_NETERROR(ret);
}


/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
    int32_t ret;

    ret = lwip_send(sock, buf, (uint32_t) len, flags);
    if (ret == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            ret = 0;
        }
    }
    else if (ret == 0)
    {
        /* connection close */
        ret = NET_ERROR_DISCONNECTED;
    }
    else
    {
        /* do not catch ret valie otherwise */
    }
    return LWIP_ERROR_TO_NETERROR(ret);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
    int32_t ret;

    ret = lwip_recv(sock, buf, (uint32_t) len, flags);
    if (ret == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            ret = 0;
        }
    }
    else if (ret == 0)
    {
        /* connection close */
        ret = NET_ERROR_DISCONNECTED;
    }
    else
    {
        /* do not catch the ret value otherwise */
    }
    return LWIP_ERROR_TO_NETERROR(ret);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *to, int32_t tolen)
{
    int32_t ret = lwip_sendto(sock, buf, (uint32_t) len, flags, (struct sockaddr *)to, (u32_t) tolen);
    if (ret == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            ret = 0;
        }
    }
    else if (ret == 0)
    {
        /* connection close */
        ret = NET_ERROR_DISCONNECTED;
    }
    else
    {
        /* ret is postif */
    }
    return LWIP_ERROR_TO_NETERROR(ret);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, sockaddr_t *from,
                                 int32_t *fromlen)
{
    int32_t ret = lwip_recvfrom(sock, buf, (uint32_t) len, flags, (struct sockaddr *)from, (u32_t *)&fromlen);
    if (ret == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            ret = 0;
        }
    }
    else if (ret == 0)
    {
        /* connection close */
        ret = NET_ERROR_DISCONNECTED;
    }
    else
    {
        /* ret is greater than zero */
    }
    return ret;
}

/**
  * @brief  function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue, int32_t optlen)
{
    int32_t   ret = lwip_setsockopt(sock, level, optname, optvalue, (uint32_t) optlen);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */

static int32_t net_lwip_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, int32_t *optlen)
{
    int32_t ret = lwip_getsockopt(sock, level, optname, optvalue, (u32_t *) optlen);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_getsockname(int32_t sock, sockaddr_t *name, int32_t *namelen)
{
    int32_t ret = lwip_getsockname(sock, (struct sockaddr *) name, (u32_t *) namelen);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_getpeername(int32_t sock, sockaddr_t *name, int32_t *namelen)
{
    int32_t ret = lwip_getpeername(sock, (struct sockaddr *) name, (u32_t *) namelen);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_close(int32_t sock, bool clone)
{
    int32_t   ret = lwip_close(sock);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_shutdown(int32_t sock, int32_t mode)
{
    int32_t   ret = lwip_shutdown(sock, mode);
    return ret;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
static int32_t net_lwip_gethostbyname(net_if_handle_t *pnetif, sockaddr_t *addr, char_t *name)
{
    int32_t ret = NET_ERROR_DNS_FAILURE;
    struct addrinfo *hostinfo;
    struct addrinfo hints;

    if (addr->sa_len < sizeof(sockaddr_in_t))
    {
        return NET_ERROR_PARAMETER;
    }

    (void) memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (0 == lwip_getaddrinfo((char *)name, NULL, &hints, &hostinfo))
    {
        if (hostinfo->ai_family == AF_INET)
        {
            uint8_t len = addr->sa_len;
            sockaddr_in_t *saddr = (sockaddr_in_t *) addr;

            (void) memset(saddr, 0, len);
            saddr->sin_len = len;
            saddr->sin_family = NET_AF_INET;
            (void) memcpy(&saddr->sin_addr, &((sockaddr_in_t *)(hostinfo->ai_addr))->sin_addr, 4);
            ret = NET_OK;
        }
        lwip_freeaddrinfo(hostinfo);
    }

    return ret;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
