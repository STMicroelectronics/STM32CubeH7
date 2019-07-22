/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "udpecho.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UDPECHO_THREAD_PRIO  (osPriorityAboveNormal)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void
udpecho_thread(void *arg)
{
  struct netconn *conn;
  struct netbuf *buf, *tx_buf;
  err_t err;
  LWIP_UNUSED_ARG(arg);
  
  conn = netconn_new(NETCONN_UDP);
  netconn_bind(conn, IP_ADDR_ANY, 7);
  
  LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);
  
  while (1) {
    err = netconn_recv(conn, &buf);
    if (err == ERR_OK) {
      
      tx_buf = netbuf_new();
      netbuf_alloc(tx_buf, buf->p->tot_len);
      
      pbuf_take(tx_buf->p, (const void *)buf->p->payload, buf->p->tot_len);
      
      err = netconn_sendto(conn, tx_buf, (const ip_addr_t *)&(buf->addr), buf->port);
      if(err != ERR_OK) {
        LWIP_DEBUGF(LWIP_DBG_ON, ("netconn_send failed: %d\n", (int)err));
      } else {
        LWIP_DEBUGF(LWIP_DBG_ON, ("got %s\n", buffer));
      }
      netbuf_delete(tx_buf);
    }
    netbuf_delete(buf);
  }
}
/*-----------------------------------------------------------------------------------*/
void
udpecho_init(void)
{
  sys_thread_new("udpecho_thread", udpecho_thread, NULL, (configMINIMAL_STACK_SIZE*2), UDPECHO_THREAD_PRIO);
}

#endif /* LWIP_NETCONN */
