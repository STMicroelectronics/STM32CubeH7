/**
  ******************************************************************************
  * @file    net_ip_ethernetif.c
  * @author  MCD Application Team
  * @brief   from net_ip to ethernet network interface functions
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
#include "net_ip_ethernet.h"
#include "net_buffers.h"

#if (osCMSIS >= 0x20000U)
#define osSemaphoreWait         osSemaphoreAcquire
#endif /* osCMSIS */

/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT ( 250 ) /*( portMAX_DELAY )*/
/* Stack size of the interface thread */

#define INTERFACE_THREAD_STACK_SIZE 1024
#define BUFFER_LIST_SIZE        10

#define CIRCULAR_INC(a) (a)++;if ((a)==BUFFER_LIST_SIZE) {(a)=0;}

static volatile int32_t  free_buffer_index;
static int32_t     write_buffer_index;
static int32_t     busy_buffer_index;
static net_buf_t *sent_write_buffer[BUFFER_LIST_SIZE];


/* Semaphore to signal incoming packets */
static osSemaphoreId s_xSemaphore = NULL;
static osThreadId ethernetif_thread_handle = NULL;
static  void ethernetif_task(void const *argument);
static  void event_callback(void);
static  void ethernetif_write_done(uint8_t *);


/* SRA CODE FROM HERE ON  */
typedef struct
{
    int32_t (*input)(void *context, net_buf_t *p);
    void (*link_status)(void *context, uint8_t status);
    void *context;
} ethernetif_cb_t;


void net_ethernetif_get_mac_addr(uint8_t *MACAddr_in)
{
    ethernetif_low_get_mac_addr(MACAddr_in);
    return;
}

static  void ethernetif_task(void const *argument)
{
    int32_t semaphore_retval;
    net_buf_t   *netbuf;
    uint8_t     *payload;
    int16_t len;
    static uint8_t link_status = 0;

    const ethernetif_cb_t *cb = (const ethernetif_cb_t *) argument;
    link_status = ethernetif_low_get_link_status();
    if (cb->link_status != NULL)
    {
        cb->link_status(cb->context, link_status);
    }

    for (;;)
    {
        semaphore_retval = osSemaphoreWait(s_xSemaphore, TIME_WAITING_FOR_INPUT);
        if (ethernetif_low_get_link_status() != link_status)
        {
            link_status = 1U - link_status;
            if (cb->link_status != NULL)
            {
                cb->link_status(cb->context, link_status);
            }
        }
        if (semaphore_retval == (int32_t) osOK)
        {
            while ((len = ethernetif_low_inputsize()) > 0)
            {
                payload = NULL;
                netbuf = NET_BUF_ALLOC((uint16_t)len);
                if (netbuf != NULL)
                {
                    payload = netbuf->payload;
                }
                if (ethernetif_low_input(payload, (uint16_t) len) > 0)
                {
                    cb->input(cb->context, netbuf);
                }
            }
        }
    }
}

static  void event_callback()
{
    (void) osSemaphoreRelease(s_xSemaphore);
}
#if (osCMSIS >= 0x20000U )
static const   osThreadAttr_t attr = { 
      .name= "EthIf", 
      .priority =osPriorityHigh,
    . stack_size = INTERFACE_THREAD_STACK_SIZE };
#endif

void net_ethernetif_init(void *context, int32_t (*ethernetif_rx_cb)(void *context, net_buf_t *p),
                         void (*ethernetif_link_status_cb)(void *context, uint8_t status))
{
    static ethernetif_cb_t cb;

    cb.input = ethernetif_rx_cb;
    cb.link_status = ethernetif_link_status_cb;
    cb.context = context;

    free_buffer_index = 0;
    write_buffer_index = 0;
    busy_buffer_index = 0;
    (void) memset(sent_write_buffer, 0, sizeof(sent_write_buffer));

    /* create a  semaphore used for informing ethernetif of frame reception */
    /* ethernet link list is 4 buffers, set semaphore size to 4 to not miss */
    /* interrupt (1 is not working )                                        */
#if (osCMSIS >= 0x20000U)
    s_xSemaphore = osSemaphoreNew(4,4,NULL);
#else
    osSemaphoreDef(SEM);
    s_xSemaphore = osSemaphoreCreate(osSemaphore(SEM), 4);
#endif /* osCMSIS */    

    ethernetif_low_init(event_callback, ethernetif_write_done);


    /* create the task that handles the ETH_MAC */
#if (osCMSIS < 0x20000U )
   
    osThreadDef(EthIf, ethernetif_task, osPriorityHigh, 0, INTERFACE_THREAD_STACK_SIZE);
    ethernetif_thread_handle = osThreadCreate(osThread(EthIf), &cb);
#else
    ethernetif_thread_handle = osThreadNew((osThreadFunc_t)ethernetif_task,&cb,&attr);
#endif
    

    /* USER CODE BEGIN LOW_LEVEL_INIT */

    /* USER CODE END LOW_LEVEL_INIT */
}

void net_ethernetif_deinit(void)
{
    (void) osThreadTerminate(ethernetif_thread_handle);
    ethernetif_thread_handle = NULL;

    (void) osSemaphoreDelete(s_xSemaphore);
    s_xSemaphore = NULL;

    ethernetif_low_deinit();
}



int32_t net_ethernetif_output(void *context,net_buf_t *netbuf)
{
    int32_t        ret;
    uint16_t        len = 0u;
    int32_t        nb = 0;
    net_buf_t      *q;

    /* release returned buffers */
    while (busy_buffer_index != free_buffer_index)
    {
        CIRCULAR_INC(busy_buffer_index);
        (void) NET_BUF_FREE(sent_write_buffer[busy_buffer_index]);
        sent_write_buffer[busy_buffer_index] = 0;
    }

    for (q = netbuf; q != NULL ; q = q->next)
    {
        len += q->len;
        nb++;
    }

    if (nb == 1)
    {
        NET_BUF_REF(netbuf);
        CIRCULAR_INC(write_buffer_index);
        sent_write_buffer[write_buffer_index] = netbuf;
        ret = ethernetif_low_output(netbuf->payload, netbuf->len);
    }
    else
    {
        net_buf_t    *p;
        p = NET_BUF_ALLOC(len);
        if (p != NULL)
        {
            uint8_t     *pp = p->payload;
            for (q = netbuf; q != NULL ; q = q->next)
            {
                (void) memcpy(pp, q->payload, q->len);
                pp = &pp[q->len];
            }
            p->len = (uint16_t) len;
            CIRCULAR_INC(write_buffer_index);
            sent_write_buffer[write_buffer_index] = p;
            ret = ethernetif_low_output(p->payload, p->len);
        }
        else
        {
            while (true) {};
        }
    }
    return ret;
}

static void ethernetif_write_done(uint8_t *buffaddr)
{
    CIRCULAR_INC(free_buffer_index);
}

