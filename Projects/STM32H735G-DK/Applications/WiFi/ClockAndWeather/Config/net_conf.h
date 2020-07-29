#ifndef NET_CONF_H
#define NET_CONF_H

/* configuration for MX wifi BSPv2 component  */
/* UART interface choice is enforced in this file */
#include "mx_wifi_conf.h"

int32_t wifi_probe(void **ll_drv_obj);

/* not using CubeMX to generate the _msp function , so use the one from net_conf_mxchip_uart.c */
#define NET_MXCHIP_LOCAL_MSP

#ifdef USE_MXCHIP_AT3080
#define USARTmxc                           USART1
#define UARTmxc_IRQn                       USART1_IRQn

#define USARTmxc_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USARTmxc_CLK_DISABLE()             __HAL_RCC_USART1_CLK_DISABLE()
#define USARTmxc_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTmxc_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()


/* Pin definition for Mxchip connection to an UART         */
/* when using CubeMX and setting label correctly under the Gui, this definition
are generated in main.h , so no need to duplicate them */

#define MX_WIFI_RESET_IO_PIN               (GPIO_PIN_0)
#define MX_WIFI_RESET_IO_PORT              (GPIOA)
#define MX_WIFI_RESET_IO_CLK_ENABLE        __HAL_RCC_GPIOA_CLK_ENABLE



/* Definition for USARTx Pins */
#define USARTmxc_TX_GPIO_PORT              GPIOB
#define USARTmxc_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTmxc_TX_PIN                    GPIO_PIN_14
#define USARTmxc_TX_AF                     GPIO_AF4_USART1

#define USARTmxc_RX_GPIO_PORT              GPIOB
#define USARTmxc_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTmxc_RX_PIN                    GPIO_PIN_15
#define USARTmxc_RX_AF                     GPIO_AF4_USART1

#else
#ifdef USE_MXCHIP_MB1400
#define USARTmxc                           UART7
#define UARTmxc_IRQn                       UART7_IRQn

#define USARTmxc_CLK_ENABLE()              __HAL_RCC_UART7_CLK_ENABLE()
#define USARTmxc_CLK_DISABLE()             __HAL_RCC_UART7_CLK_DISABLE()
#define USARTmxc_FORCE_RESET()             __HAL_RCC_UART7_FORCE_RESET()
#define USARTmxc_RELEASE_RESET()           __HAL_RCC_UART7_RELEASE_RESET()


/* Pin definition for Mxchip connection to an UART         */
/* when using CubeMX and setting label correctly under the Gui, this definition
are generated in main.h , so no need to duplicate them */

#define MX_WIFI_RESET_IO_PIN               (GPIO_PIN_11)
#define MX_WIFI_RESET_IO_PORT              (GPIOF)
#define MX_WIFI_RESET_IO_CLK_ENABLE        __HAL_RCC_GPIOF_CLK_ENABLE



/* Definition for USARTx Pins */
#define USARTmxc_TX_GPIO_PORT              GPIOF
#define USARTmxc_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()
#define USARTmxc_TX_PIN                    GPIO_PIN_7
#define USARTmxc_TX_AF                     GPIO_AF7_UART7

#define USARTmxc_RX_GPIO_PORT              GPIOF
#define USARTmxc_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()
#define USARTmxc_RX_PIN                    GPIO_PIN_6
#define USARTmxc_RX_AF                     GPIO_AF7_UART7
#else
#error Undefined MXCHIP board
#endif /* USE_MXCHIP_MB1400 */
#endif /* USE_MXCHIP_AT3080 */



/* configuration of the network library   */

#define NET_MAX_SOCKETS_NBR           MX_WIFI_MAX_SOCKET_NBR

#define NET_DBG_ERROR(...)  
#define NET_DBG_PRINT(...)  
#define NET_ASSERT(test,s)  do {} while (!test)
#define NET_PRINT(...)  
#define NET_PRINT_WO_CR(...)  
#define NET_WARNING(...)  

#include "net_conf_template.h"
#endif