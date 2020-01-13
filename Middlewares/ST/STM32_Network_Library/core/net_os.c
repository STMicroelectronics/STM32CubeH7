/**
  ******************************************************************************
  * @file    net_os.c
  * @author  MCD Application Team
  * @brief   OS needed functions implementation
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
#include "stdarg.h"

#if (osCMSIS >= 0x20000U)
#define osSemaphoreWait         osSemaphoreAcquire
#endif /* osCMSIS */

#ifdef NET_USE_RTOS
extern void *pxCurrentTCB;


static osSemaphoreId   net_mutex[NET_LOCK_NUMBER];

void net_init_locks(void)
{
#if (osCMSIS < 0x20000U)
    static osSemaphoreDef_t mutex_def[NET_LOCK_NUMBER] = {0};
#endif
#ifdef NET_MBEDTLS_HOST_SUPPORT
    net_tls_init();
#endif /* NET_MBEDTLS_HOST_SUPPORT */
    for (int32_t i = 0; i < NET_LOCK_NUMBER; i++)
    {
#if (osCMSIS < 0x20000U)
        net_mutex[i] = osSemaphoreCreate(&mutex_def[i], 1);
#else
        net_mutex[i] = osSemaphoreNew( 1 , 1 , NULL);

#endif
        NET_ASSERT(net_mutex[i] > 0, "Failed on mutex creation");
    }
}

void net_destroy_locks(void)
{
#ifdef NET_MBEDTLS_HOST_SUPPORT
    net_tls_destroy();
#endif /* NET_MBEDTLS_HOST_SUPPORT */
    for (int32_t i = 0; i < NET_LOCK_NUMBER; i++)
    {
        (void)osSemaphoreDelete(net_mutex[i]);
    }
}

void net_lock(int32_t sock, uint32_t timeout_in)
{
    int32_t   ret;
    uint32_t timeout = timeout_in;
    if (timeout ==  NET_OS_WAIT_FOREVER)
    {
        /*MISRA issue so hand coded  t = osWaitForever;*/
        timeout = 0xffffffffU;
    }
    ret = osSemaphoreWait(net_mutex[sock], timeout);
    NET_ASSERT(ret == 0, "Failed locking mutex");
}

void net_unlock(int32_t sock)
{
    int32_t   ret;
    ret = (int32_t) osSemaphoreRelease(net_mutex[sock]);
    NET_ASSERT(ret == 0, "Failed unlocking mutex");
}

void net_lock_nochk(int32_t sock, uint32_t timeout_in)
{
    uint32_t timeout= timeout_in;
    if (timeout == NET_OS_WAIT_FOREVER)
    {
        /* MISRA issue so hand coded  t = osWaitForever;*/
        timeout = 0xffffffffU;
    }
    (void) osSemaphoreWait(net_mutex[sock], timeout);
}

void net_unlock_nochk(int32_t sock)
{
    (void) osSemaphoreRelease(net_mutex[sock]);
}

void    *net_calloc(size_t n, size_t m)
{
    void *p;
    p = malloc(n * m);
    if (p != NULL)
    {
        (void) memset(p, 0, n * m);
    }
    return p;
}

void    *calloc(size_t n, size_t m)
{
    void *p;
    p = NET_MALLOC(n * m);
    if (p != NULL)
    {
        (void) memset(p, 0, n * m);
    }
    return p;
}

void *malloc(size_t n)
{
    return pvPortMalloc(n);
}

void free(void *p)
{
    vPortFree(p);
}

/* needed for IAR EWARM heap management */
void *realloc(void *ptr, size_t size)
{
    void *ret;
    if (ptr == NULL)
    {
        if (size != (size_t) 0)
        {
            ret = malloc(size);
        }
        else
        {
            ret = NULL;
        }
    }
    else
    {
        if (size != (size_t) 0)
        {
            void *new_ptr;
            new_ptr = malloc(size);
            if (new_ptr != NULL)
            {
                (void) memcpy(new_ptr, ptr, size);
                free(ptr);
                ret = new_ptr;
            }
            else
            {
                ret = NULL;
            }
        }
        else
        {
            free(ptr);
            ret = NULL;
        }
    }
    return ret;
}
#endif /* NET_USE_RTOS */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

