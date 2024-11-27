/**
  @page SSL_Server SSL Server application

  @verbatim
  ******************************************************************************
  * @file    mbedTLS/SSL_Server/readme.txt
  * @author  MCD Application Team
  * @brief   Description of SSL Server application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

This application describes how  to run an SSL server application based on mbedTLS
crypto library and LwIP TCP/IP stack.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals and initialize the Flash interface.
The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as time base for the HAL drivers.
This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms.
The systick is then used for FreeRTOS time base.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

Scenario 1:
===========
In this screnario the Server (STM32H743I-EVAL):
- connects to local network either through DHCP or static IP address.
- use a web browser on a PC to connect to the url https://<Server_Board_Ipadress>:4433
- once the connection is established, the server sends a message to the client
- the message is displayed on the browser webpage

Scenario 2:
===========
In this second scenario, 2 boards can be used to test the application
- connects to local network either through DHCP or static IP address.
- a second STM32H743I-EVAL board running the "SSL_Server" application
  connects to the the url https://<Server_Board_Ipadress>:4433
- the  client sends an encrypted message to the server.
- the server board responds with the suitable message.
- the received message is displayed on the LCD.

Application logs are displayed on the LCD.

In case of success the green led is on.
In case of errors the red led is on.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note the message sent by the server depends on the ciphersuite agreed with the client.
      it is similar to :
      " mbed TLS Test Server
        Successful connection using: TLS-ECDHE-RSA-WITH-AES-256-GCM-SHA384"

For more details about this application, refer to UM1723 "STM32Cube mbedTLS application".


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Security, mbedTLS, HASH, RNG, CRYP, SSL Client, Crypto, TCP/IP, LwIP, AES 128,
AES 192, AES 256, Triple DES, MD5, SHA-1

@par Directory contents

    - mbedTLS/SSL_Server/Inc/main.h                         Main program header file
    - mbedTLS/SSL_Server/Inc/lwipopts.h                     LwIP stack configuration options
    - mbedTLS/SSL_Server/Inc/ethernetif.h                   header for ethernetif.c file
    - mbedTLS/SSL_Server/Inc/stm32h7xx_it.h                 Interrupt handlers header file
    - mbedTLS/SSL_Server/Inc/utilities_conf.h               Utilities configuration file
    - mbedTLS/SSL_Server/Inc/exc7200_conf.h                 EXC7200 TS component configuration file
    - mbedTLS/SSL_Server/Inc/is42s32800g_conf.h             IS42S32800G SDRAM configuration file
    - mbedTLS/SSL_Server/Inc/stmpe811_conf.h                STMPE811 IO expander component configuration file
    - mbedTLS/SSL_Server/Inc/ts3510_conf.h                  TS3510 TS component configuration file
    - mbedTLS/SSL_Server/Inc/stm32h743i_eval_conf.h         STM32H743I-EVAL configuration file
    - mbedTLS/SSL_Server/Inc/mbedtls_config.h               mbedTLS library configuration options
    - mbedTLS/SSL_Server/Inc/FreeRTOSConfig.h               FreeRTOS configuration options
    - mbedTLS/SSL_Server/Inc/stm32h7xx_hal_conf.h           Library Configuration file
    - mbedTLS/SSL_Server/Src/main.c                         Main program
    - mbedTLS/SSL_Server/Src/ssl_server.c                   SSL server main thread
    - mbedTLS/SSL_Server/Src/ethernetif.c                   Interfacing the LwIP stack to ETH driver
    - mbedTLS/SSL_Server/Src/net_socket.c                   mbedTLS TCP/IP socket API implementation using LwIP.
    - mbedTLS/SSL_Server/Src/stm32h7xx_it.c                 Interrupt handlers
    - mbedTLS/SSL_Server/Src/system_stm32h7xx.c             STM32 system clock configuration file
    - mbedTLS/SSL_Server/Src/stm32h7xx_hal_msp.c            HAL MSP module
    - mbedTLS/SSL_Server/Src/stm32h7xx_hal_timebase_tim.c   HAL time base functions



@par Hardware and Software environment

  - This application runs on stm32h743xx Devices.

  - This application has been tested with two scenarios.
      - Using a Windows PC equipped with a web browser as a client.
     - using two stm32h743_eval boards the first as server, the second as client.

  - Scenario 1:
      - Remote PC Set-up
	  - Make sure that the PC is in the same local network as the baord.
          - The Pc has a web browser that supports TLS and secure connections (IE > 10, Chrome > 52, FireFox > 41)

  - Scenario 2:
      - a second STM32H743I-EVAL running the "mdedTLS/SSL_Server" application

  - STM32H743I-EVAL Set-up
    - Connect STM32H743I-EVAL to your local network (through a straight ethernet cable).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application.

@Note: 1) Make sure that the url passed to the client is https://Server_board_address:4433

       2) the Server is using a non trusted certificate, thus some clients, in particular web browser, may show errors during connection
           please manage to bypass that error to get the application running.

       3) If using two boards for testing and the DHCP is enabled then the "SSL_Server" needs to be
           un first, to get the board IP Address and use it in the "SSL_Server" application.

       4) If the board is connected to a LAN, ensure that it has a unique MAC address. The MAC address can be defined in the file
          "Inc/stm32h7xx_hal_conf.h"



 */
