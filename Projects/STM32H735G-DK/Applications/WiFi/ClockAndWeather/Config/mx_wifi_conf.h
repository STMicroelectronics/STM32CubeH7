/**
 ******************************************************************************
 * @file    mx_wifi_conf.h
 * @author  MXCHIP
 * @version V1.0.0
 * @date    1-Aug-2019
 * @brief   This file contains configurations for mx_wifi module.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2014 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */
#ifndef MX_WIFI_CONF_H
#define MX_WIFI_CONF_H
#define IPC_MASTER

#define MX_WIFI_USE_UART_INTERRUPT                  1
#define MX_WIFI_USE_SPI                             (0)
#ifdef HAS_RTOS
#define MX_WIFI_USE_CMSIS_OS                        1
#endif
#ifdef __cplusplus
 extern "C" {
#endif  

#include "mx_wifi_conf_template.h"


   
#ifdef __cplusplus
}
#endif
#endif /* MX_WIFI_CONF_H */

