/**
  ******************************************************************************
  * @file    net_address.h
  * @author  MCD Application Team
  * @brief   Header for the network address management functions
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

#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

#include "string.h"
#include <stdlib.h>
#include "net_types.h"

#define NET_PP_HTONL(x) ((((x) & 0x000000ffUL) << 24) | \
                         (((x) & 0x0000ff00UL) <<  8) | \
                         (((x) & 0x00ff0000UL) >>  8) | \
                         (((x) & 0xff000000UL) >> 24))


#define NET_ASLWIP_MAKEU32(a,b,c,d) (((uint32_t)((a) & 0xff) << 24) | \
                                     ((uint32_t)((b) & 0xff) << 16) | \
                                     ((uint32_t)((c) & 0xff) << 8)  | \
                                     (uint32_t)((d) & 0xff))


#define NET_IPADDR4_INIT(u32val)                    u32val
#define NET_IPADDR4_INIT_BYTES(a,b,c,d)             NET_IPADDR4_INIT(NET_PP_HTONL(NET_ASLWIP_MAKEU32(a,b,c,d)))

typedef struct net_if_handle_s net_if_handle_t;

/* generic socket address structure to support IPV6 and IPV4               */
/* size is 16 bytes and is aligned on LWIP definition to ease integration */

typedef struct con_sockaddr
{
    uint8_t     sa_len;
    uint8_t     sa_family;
    char_t        sa_data[14];
} sockaddr_t;


typedef uint32_t ipaddr_t;



typedef uint32_t net_in_addr_t;
#define S_ADDR(a) a

/* IPV4 address , with 8 stuffing byte */
typedef struct
{
    uint8_t  sin_len;
    uint8_t  sin_family;
    uint16_t sin_port;
    net_in_addr_t  sin_addr;
#define NET_SIN_ZERO_LEN 8
    char_t            sin_zero[NET_SIN_ZERO_LEN];
} sockaddr_in_t;


/** MAC address. */
typedef struct
{
    uint8_t mac[6];
} macaddr_t;


#define LWIP_DONT_PROVIDE_BYTEORDER_FUNCTIONS
#define NET_HTONL(A)   ((((uint32_t)(A) & 0xff000000U) >> 24) | \
                        (((uint32_t)(A) & 0x00ff0000U) >> 8) | \
                        (((uint32_t)(A) & 0x0000ff00U) << 8) | \
                        (((uint32_t)(A) & 0x000000ffU) << 24))

#define NET_NTOHL       NET_HTONL

#define NET_HTONS(A)     ((((unsigned long)(A) & 0xff00U) >> 8U) | \
                          (((unsigned long)(A) & 0x00ffU) << 8U))
#define NET_NTOHS       NET_HTONS

#define NET_NTOAR(N,A)   A[3] = (uint8_t)((N & 0xff000000) >> 24);\
  A[2] = (uint8_t)((N & 0x00ff0000) >> 16);\
  A[1] = (uint8_t)((N & 0x0000ff00) >> 8); \
  A[0] = (uint8_t)((N & 0x000000ff) >> 0); \

#define NET_ARTON(A)     ((uint32_t)((A[3] << 24) |\
                                     (A[2] << 16) |\
                                     (A[1] <<  8) |\
                                     (A[0] <<  0)))



#define NET_IN_RANGE(c, lo, up)  ((c) >= (lo) && (c) <= (up))
#define NET_IS_PRINT(c)          (NET_IN_RANGE(c, 0x20, 0x7f))
#define NET_ISDIGIT(c)           (NET_IN_RANGE(c, '0', '9'))
#define NET_ISXDIGIT(c)          (NET_ISDIGIT(c) || NET_IN_RANGE(c, 'a', 'f') || NET_IN_RANGE(c, 'A', 'F'))
#define NET_ISLOWER(c)           (NET_IN_RANGE(c, 'a', 'z'))
#define NET_ISSPACE(c)           (((c) == ' ')\
                                  || ((c) == '\f') || ((c) == '\n') || ((c) == '\r') || ((c) == '\t') || ((c) == '\v'))

char_t *net_ntoa(const net_in_addr_t *addr);
char_t *net_ntoa_r(const net_in_addr_t *addr, char_t *buf, int32_t buflen);

int32_t net_aton_r(const char_t *cp);
int32_t net_aton(int32_t af, const char_t *cp, net_in_addr_t *addr);
#endif /* NET_ADDRESS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
