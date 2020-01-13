/**
  ******************************************************************************
  * @file    net_ethernet_driver.c
  * @author  MCD Application Team
  * @brief   Ethernet specific BSD-like socket wrapper
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

#include <string.h>
#include "net_connect.h"
#include "net_internals.h"
#include "net_ip_ethernet.h"
#include "net_ip_lwip.h"
#include "lwip/tcpip.h"
#include "lwip/netdb.h"
#include "lwip/dhcp.h"
#include "netif/etharp.h"

#include "netif/etharp.h"

#include "whd.h"
#include "whd_debug.h"
#include "whd_resource_api.h"
#include "whd_wifi_api.h"
#include "whd_network_types.h"
#include "whd_types_int.h"


#define ETHTYPE_EAPOL    0x888E

uint32_t        whd_start(void);
whd_interface_t ifp;

/* global constructor of the ethernet network interface */
int32_t cypress_whd_net_driver(net_if_handle_t *pnetif);
static err_t low_level_output( struct netif *netif, struct pbuf *p );
static  err_t low_level_init(struct netif *netif);
static void netif_change_handler(struct netif *netif);




static int32_t net_cypress_whd_if_init(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_deinit(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_start(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_stop(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_connect(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_disconnect(net_if_handle_t *pnetif);



/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */

extern iplib_t iplib_lwip;
static  iplib_t *iplib = &iplib_lwip;




int32_t cypress_whd_net_driver(net_if_handle_t *pnetif)
{
    int32_t ret;
    ret = net_cypress_whd_if_init(pnetif);
    return ret;
}

static int32_t  net_cypress_whd_if_init(net_if_handle_t *pnetif)
{
    int32_t ret;
    net_if_drv_t *pdrv = NET_MALLOC(sizeof(net_if_drv_t));
    if (pdrv != NULL)
    {
        pdrv->if_class = NET_INTERFACE_CLASS_ETHERNET;
        pdrv->if_init = net_cypress_whd_if_init;
        pdrv->if_deinit = net_cypress_whd_if_deinit;
        pdrv->if_start = net_cypress_whd_if_start;
        pdrv->if_stop = net_cypress_whd_if_stop;
        pdrv->if_connect = net_cypress_whd_if_connect;
        pdrv->if_disconnect = net_cypress_whd_if_disconnect;
        pdrv->ping = icmp_ping;
        pnetif->pdrv = pdrv;

        /* start lwip now , because pbuf alloc are mandatory for whd */
        tcpip_init(NULL, NULL);

        /* start whd  soft and hard */
        if (whd_start()!=WHD_SUCCESS)
        {
          NET_DBG_ERROR("can't perform intialization of whd driver and module\n");
          ret = NET_ERROR_MODULE_INITIALIZATION;
        }
        else
        {
         (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
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

whd_ssid_t myssid;

struct netif whd_netif;

#define MAX_MTU 1500

void     link_socket_to_lwip(void *);

int32_t net_cypress_whd_if_start(net_if_handle_t *pnetif)
{
  int32_t ret;
  uint32_t       i;

//  strcpy((char*) myssid.value,"GNBA350-IotSpot");
#if 0
  const char              security_key[256]="stmicroMCD";
  strcpy((char*) myssid.value,"GNBA350-homekit");
#endif
   const char              security_key[256]="Egoistes";
  strcpy((char*) myssid.value,"Bbox-60F7EB11");

  
  myssid.length=strlen((char*)myssid.value);

  ret = whd_wifi_join(ifp , (whd_ssid_t const*) &myssid, WHD_SECURITY_WPA2_AES_PSK, (uint8_t const*) security_key, strlen(security_key));
  if (ret != 0)
  {
    NET_DBG_ERROR("can't join  %s\n",myssid.value);
    ret = NET_ERROR_WIFI_CANT_JOIN;
  }
  else
  {
    struct netif*       netif;
     
    NET_DBG_PRINT("join  %s\n",myssid.value);

    netif = NET_MALLOC(sizeof(struct netif));
    if (NULL == netif)
    {
      NET_DBG_ERROR( "Could not allocate netif interface\n" );
      return -1;
    }
    (void) memset(netif, 0x00, sizeof(struct netif));
    
  

    pnetif->pdrv->context = netif;
    link_socket_to_lwip(pnetif->pdrv);

    
    /* Add the network interface */
    if ( NULL == netif_add( netif, NULL,NULL,NULL, (void*) ifp, low_level_init, ethernet_input ) )
    {
        NET_DBG_ERROR( "Could not add network interface\n" );
        return -1;
    }
      NET_DBG_PRINT( "after netif  %lx\n",netif->ip_addr);

    for(i=0;i<6;i++)
    {
      pnetif->macaddr.mac[i] = netif->hwaddr[i];
    }
#if LWIP_IPV6
    /* Set the IPv6 linklocal address using our MAC */
    NET_DBG_PRINT ("Setting IPv6 link-local address\n") );

    netif_create_ip6_linklocal_address( netif, 1 );
    IP_HANDLE(interface).ip6_autoconfig_enabled = 1;
#endif

    /* Register a handler for any address changes  and when interface goes up or down*/
    netif_set_status_callback( netif, netif_change_handler );
    netif_set_link_callback(netif, netif_change_handler);

    
    netif_set_down(netif);
    netif_set_default(netif); 
      
    netif_set_up(netif);
    
    (void) strncpy(pnetif->DeviceName, "Wlan WHD murata 1LD", sizeof(pnetif->DeviceName));
    (void) strncpy(pnetif->DeviceID, "Unknown", sizeof(pnetif->DeviceID));
    (void) strncpy(pnetif->DeviceVer, "Unknown", sizeof(pnetif->DeviceVer));
    (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
     netif_set_link_up(netif);

  }
  return ret;
}


  

static int32_t net_cypress_whd_if_connect(net_if_handle_t *pnetif)
{
  struct netif* netif=pnetif->pdrv->context;

  int32_t       ret = NET_OK;
  

  if (pnetif->dhcp_mode)
  {
    if (dhcp_start(netif) != 0)
    {
       NET_DBG_ERROR( "Failed while startingS DHCP\n" );
    }
  }
  else
  {
     NET_DBG_PRINT( "Static IP  %x\n",pnetif->static_ipaddr);
     netif_set_addr(pnetif->net_ip_if->obj, (ip4_addr_t const *) &pnetif->static_ipaddr, (ip4_addr_t const *) &pnetif->static_netmask,
                       (ip4_addr_t const *) &pnetif->static_gateway); 

     dhcp_inform(netif);
  }
  return ret;
}  

static int32_t net_cypress_whd_if_disconnect(net_if_handle_t *pnetif)
{ 
    int32_t       ret;
     ret =  iplib->disconnect(pnetif);
    if (ret == NET_OK)
    {
        ret = net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
    }
    NET_DBG_PRINT("%s %d\n",__FILE__,__LINE__);
    return ret;
}

static int32_t net_cypress_whd_if_stop(net_if_handle_t *pnetif)
{
    int32_t ret;
    ret = iplib->remove_if(pnetif);
    net_ethernetif_deinit();
    if (ret == NET_OK)
    {
        ret =     net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);

    }
    return ret;
}

static int32_t net_cypress_whd_if_deinit(net_if_handle_t *pnetif)
{
    int32_t ret = NET_OK;
    NET_FREE(pnetif->pdrv);
    pnetif->pdrv = NULL;
    return ret;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
void app_start_whd(void);

/* are defined in whd_resources , and based from preproc includ epath for NVRAM and Binary */

extern  whd_resource_source_t resource_ops;
extern  whd_buffer_funcs_t      buffer_funcs_ops;
extern  whd_netif_funcs_t       netif_funcs_ops;
extern  whd_sdio_config_t       sdio_config;
extern whd_init_config_t       whd_init_config;

#define THREAD_STACK_SIZE   	    5120
#define THREAD_PRIORITY   	        osPriorityHigh
#define COUNTRY                     WHD_COUNTRY_AUSTRALIA
#define DEFAULT_OOB_PIN		        0
#define WLAN_INTR_PRIORITY	        1
#define WLAN_POWER_UP_DELAY_MS      250

#define SDIO_ENUMERATION_TRIES      500
#define SDIO_RETRY_DELAY_MS         1
#define SDIO_BUS_LEVEL_MAX_RETRIES  5

cyhal_sdio_t              sdhc_obj;
const char              security_key[256]="stmicroMCD";
void    whd_init_hardware_sdio(void *);

uint32_t       whd_start(void)
{
    whd_driver_t whd_driver;
    uint32_t          res=0;

    whd_init_hardware_sdio(&sdhc_obj);

    /* Allocated stack  for thread */
    whd_init_config.thread_stack_size = (uint32_t) THREAD_STACK_SIZE;
    whd_init_config.thread_stack_start = (uint8_t *)malloc(THREAD_STACK_SIZE);

    whd_init_config.thread_priority = (uint32_t) THREAD_PRIORITY;
    whd_init_config.country = COUNTRY;


    // Each wifi chip, will have it's own instance of whd_driver.
    // Each whd_driver may use multiple instance of whd_interface_t structs to define behavior and functionality.
    // Most of the WHD function calls take this structure as input.
    // The default primary interface is created automatically at the time of power up of wifi chip, whd_wifi_on(..).
    // Primary interface is STA/AP role neutral.


    // 4. Call whd_init per wifi chip (in other words per bus slot, two SDIO Wifi chip requires two calls.)
    res=whd_init(&whd_driver, &whd_init_config, &resource_ops, &buffer_funcs_ops, &netif_funcs_ops);
    if (res != WHD_SUCCESS)
    {
      NET_DBG_PRINT("Failed when performing whd driver initialization\n");
      return res;
    }
    /* Attach a bus SDIO or SPI */
    res=whd_bus_sdio_attach(whd_driver, &sdio_config, sdhc_obj);
    if (res != WHD_SUCCESS)
    {
      NET_DBG_PRINT("Failed when performing whd driver initialization\n");
      return res;
    }

    /* orhd_bus_spi_attach(whd_driver, &whd_spi_cfg, &spi_obj); */

    /*. Switch on Wifi, download firmware and create a primary interface, returns whd_interface_t */
    res=whd_wifi_on(whd_driver, &ifp);
    if (res != WHD_SUCCESS)
    {
      NET_DBG_PRINT("Failed when performing whd driver initialization\n");
      return res;
    }
    return res;
}


void whd_quit()
{
    // or 11b. It can an start AP also here, then interface will be in AP role
    //whd_wifi_init_ap(ifp ..)
    //whd_wifi_start_ap(ifp);

    //12 Leave the AP
    whd_wifi_leave(ifp );
    // or Stop the AP
    //whd_wifi_stop_ap(ifp , ...);

    //13. Switch off Wifi
    whd_wifi_off(ifp );

    //Deletes all the interface and De-init the whd, free whd_driver memory
    whd_deinit(ifp);
}



static  err_t low_level_init(struct netif *netif)
{
    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
  
    //NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->name[0] = 'c';
    netif->name[1] = 'y';

    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...)
     */
    netif->output = etharp_output;
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif
    netif->linkoutput = low_level_output;

    
     /* Set MAC hardware address length ( 6)*/
    netif->hwaddr_len = (u8_t) ETHARP_HWADDR_LEN;

    /* Setup the physical address of this IP instance. */
    if ( whd_wifi_get_mac_address((whd_interface_t) netif->state , (whd_mac_t*)  netif->hwaddr  ) != WHD_SUCCESS )
    {
        WPRINT_WHD_DEBUG(("Couldn't get MAC address\n"));
        return ERR_VAL;
    }

    /* Set Maximum Transfer Unit */
    netif->mtu = (u16_t) WHD_PAYLOAD_MTU;

    /* Set device capabilities. Don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = (u8_t) ( NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP );

    /* Do whatever else is needed to initialize interface. */
#if LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
    netif_set_igmp_mac_filter(netif, lwip_igmp_mac_filter);
#endif
    return (err_t) ERR_OK;
   
}



 /* This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output( struct netif *netif, struct pbuf *p )
{

    netif->state=ifp;
    if ( whd_wifi_is_ready_to_transceive( (whd_interface_t) netif->state ) == WHD_SUCCESS )
    {
        NET_DBG_PRINT("Transmit buffer\n");
    /* Take a reference to this packet */
        pbuf_ref( p );

        LWIP_ASSERT( "No chained buffers", ( ( p->next == NULL ) && ( ( p->tot_len == p->len ) ) ) );
        whd_network_send_ethernet_data( p, (whd_interface_t) netif->state );

        LINK_STATS_INC( link.xmit );
    NET_DBG_PRINT("%s %d\n",__FILE__,__LINE__);

        return (err_t) ERR_OK;
    }
    else
    {
          NET_DBG_PRINT("cannot transmit wifi not rdy\n");

        /* Stop lint warning about packet not being freed - it is not being referenced */ /*@-mustfree@*/
        return (err_t) ERR_INPROGRESS; /* Note that signalling ERR_CLSD or ERR_CONN causes loss of connectivity on a roam */
        /*@+mustfree@*/
    }
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param p : the incoming ethernet packet
 */
void cy_network_process_ethernet_data( /*@only@*/ whd_buffer_t buff, whd_interface_t interface )
{
    struct eth_hdr* ethernet_header;
    struct netif*   tmp_netif;
    u8_t            result;
    uint16_t        ethertype;
    struct pbuf *buffer = (struct pbuf*) buff;

    if ( buffer == NULL )
        return;

    /* points to packet payload, which starts with an Ethernet header */
    ethernet_header = (struct eth_hdr *) buffer->payload;

    ethertype = lwip_htons( ethernet_header->type );
    NET_DBG_PRINT("get packet %x\n",ethertype);

#ifdef FILTER
    if (filter_ethernet_packet_callback != NULL && filter_ethertype == ethertype && filter_interface == interface)
    {
        filter_ethernet_packet_callback(buffer->payload, filter_userdata);
    }
#endif
    /* Check if this is an 802.1Q VLAN tagged packet */
    if ( ethertype == WHD_ETHERTYPE_8021Q )
    {
        /* Need to remove the 4 octet VLAN Tag, by moving src and dest addresses 4 octets to the right,
         * and then read the actual ethertype. The VLAN ID and priority fields are currently ignored. */
        uint8_t temp_buffer[ 12 ];
        memcpy( temp_buffer, buffer->payload, 12 );
        memcpy( ( (uint8_t*) buffer->payload ) + 4, temp_buffer, 12 );

        buffer->payload = ( (uint8_t*) buffer->payload ) + 4;
        buffer->len = (u16_t) ( buffer->len - 4 );

        ethernet_header = (struct eth_hdr *) buffer->payload;
        ethertype = lwip_htons( ethernet_header->type );
    }
#ifdef DEEP_SLEEP
    if ( WHD_DEEP_SLEEP_IS_ENABLED() && ( WHD_DEEP_SLEEP_SAVE_PACKETS_NUM != 0 ) )
    {
        if ( wiced_deep_sleep_save_packet( buffer, interface ) )
        {
            return;
        }
    }
#endif
    switch ( ethertype )
    {
        case WHD_ETHERTYPE_IPv4:
        case WHD_ETHERTYPE_ARP:
#if PPPOE_SUPPORT
        /* PPPoE packet? */
        case ETHTYPE_PPPOEDISC:
        case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
            /* Find the netif object matching the provided interface */
            for ( tmp_netif = netif_list; ( tmp_netif != NULL ) && ( tmp_netif->state != (void*) interface ); tmp_netif = tmp_netif->next )
            {
            }

            if ( tmp_netif == NULL )
            {
                /* Received a packet for a network interface is not initialised Cannot do anything with packet - just drop it. */
                result = pbuf_free( buffer );
                LWIP_ASSERT("Failed to release packet buffer", ( result != (u8_t)0 ) );
                buffer = NULL;
                return;
            }

            /* Send to packet to tcpip_thread to process */
            if ( tcpip_input( buffer, tmp_netif ) != ERR_OK )
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));

                /* Stop lint warning - packet has not been released in this case */ /*@-usereleased@*/
                result = pbuf_free( buffer );
                /*@+usereleased@*/
                LWIP_ASSERT("Failed to release packet buffer", ( result != (u8_t)0 ) );
                buffer = NULL;
            }
            break;
#if 0
        // FIXME , not supported todays
        case WHD_ETHERTYPE_EAPOL:
            whd_eapol_receive_eapol_packet( buffer, interface );
            break;
#endif
        default:
            result = pbuf_free( buffer );
            LWIP_ASSERT("Failed to release packet buffer", ( result != (u8_t)0 ) );
            buffer = NULL;
            break;
    }
}


static void netif_change_handler(struct netif *netif)
{
    net_if_handle_t *pnetif = netif->state;

    NET_DBG_PRINT("netif_change_handler Link-%s  %s\n",netif_is_link_up(netif)?"Up":"Down",netif_is_up(netif)?"Up":"Down");

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