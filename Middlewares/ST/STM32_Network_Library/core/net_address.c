/**
  ******************************************************************************
  * @file    net_address.c
  * @author  MCD Application Team
  * @brief   Implements network address conversion routines
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

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
char_t *net_ntoa_r(const net_in_addr_t *addr, char_t *buf, int32_t buflen)
{
    uint32_t s_addr;
    char_t inv[3];
    char_t *rp;
    uint8_t *ap;
    uint8_t rem;
    uint8_t i;
    int32_t len = 0;

    s_addr = S_ADDR(*addr);

    rp = buf;
    ap = (uint8_t *)&s_addr;
    for (uint8_t n = 0; n < (uint8_t) 4; n++)
    {
        i = 0;
        do
        {
            rem = *ap % (uint8_t)10;
            *ap /= (uint8_t)10;
            inv[i] = (uint8_t)'0' + rem;
            i++;
        } while (*ap != 0U);
        while (i != 0U)
        {
            i--;
            if (len++ >= buflen)
            {
                return NULL;
            }
            *rp = (signed char) inv[i];
            rp++;
        }
        if (len++ >= buflen)
        {
            return NULL;
        }
        *rp = (signed char) '.';
        rp++;
        ap++;
    }
    --rp;
    *rp = 0;
    return buf;
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
char_t *net_ntoa(const net_in_addr_t *addr)
{
    static char_t str[16];
    return net_ntoa_r(addr, str, 16);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
int32_t net_aton(int32_t af, const char_t *cp, net_in_addr_t *addr)
{
    uint32_t val;
    uint32_t base;
    char_t c0;
    uint32_t parts[4];
    uint32_t *pp = parts;

    if (af != NET_AF_INET)
    {
        return -1;
    }

    c0 = *cp;
    for (;;)
    {
        /*
         * Collect number up to ``.''.
         * Values are specified as for C:
         * 0x=hex, 0=octal, 1-9=decimal.
         */
        if (!NET_ISDIGIT(c0))
        {
            return (0);
        }
        val = 0;
        base = 10;
        if (c0 == '0')
        {
            c0 = (char) *++cp;
            if (c0 == 'x' || c0 == 'X')
            {
                base = 16;
                c0 = (char) *++cp;
            }
            else
            {
                base = 8;
            }
        }
        for (;;)
        {
            if (NET_ISDIGIT(c0))
            {
                val = (val * base) + (uint32_t)c0 - (uint32_t) '0';
                c0 = (char) *++cp;
            }
            else if ((base == 16U) && NET_ISXDIGIT(c0))
            {
                val = (val << 4) | ((uint32_t)c0 + 10U - (uint32_t)(NET_ISLOWER(c0) ? 'a' : 'A'));
                c0 = (char) *++cp;
            }
            else
            {
                break;
            }
        }
        if (c0 == '.')
        {
            /*
             * Internet format:
             *  a.b.c.d
             *  a.b.c   (with c treated as 16 bits)
             *  a.b (with b treated as 24 bits)
             */
            if (pp >= parts + 3)
            {
                return (0);
            }
            *pp++ = val;
            c0 = (char) *++cp;
        }
        else
        {
            break;
        }
    }
    /*
     * Check for trailing characters.
     */
    if (c0 != '\0' && (NET_ISSPACE(c0) == false))
    {
        return (0);
    }
    /*
     * Concoct the address according to
     * the number of parts specified.
     */
    switch (pp - parts + 1)
    {

    case 0:
        return (0);       /* initial nondigit */

    case 1:             /* a -- 32 bits */
        break;

    case 2:             /* a.b -- 8.24 bits */
        if (val > 0xffffffUL)
        {
            return (0);
        }
        val |= parts[0] << 24;
        break;

    case 3:             /* a.b.c -- 8.8.16 bits */
        if (val > 0xffffU)
        {
            return (0);
        }
        val |= (parts[0] << 24) | (parts[1] << 16);
        break;

    case 4:             /* a.b.c.d -- 8.8.8.8 bits */
        if (val > 0xffU)
        {
            return (0);
        }
        val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
        break;
    default:
        break;
    }

    if (addr != NULL)
    {
        S_ADDR(*addr) = NET_HTONL(val);
    }
    return (1);
}

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */
int32_t net_aton_r(const char_t *cp)
{
    net_in_addr_t val;
    int32_t       ret;
    if (net_aton(NET_AF_INET, cp, &val) != 0)
    {
        ret = (int32_t) S_ADDR(val);
    }
    else
    {
        ret = 0;
    }
    return (ret);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
