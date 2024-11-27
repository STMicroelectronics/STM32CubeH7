/**
  @page SSL_Client SSL Client application

  @verbatim
  ******************************************************************************
  * @file    mbedTLS/SSL_Client/readme.txt
  * @author  MCD Application Team
  * @brief   Description of SSL Client application.
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

This application describes how  to run an SSL client application based on mbedTLS
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

Then the application can run one of the following scenarios:

Scenario 1:
===========
In this screnario the client (STM32H743I-EVAL):
- connects to local network either through DHCP or static IP address.
- establishes a secured connected to the Host PC Server.
- sends an encrypted message to the server.
- receives an answser from the server.

Scenario 2:
===========
In this second scenario, the the client (STM32H743I-EVAL):
- connects to local network either through DHCP or static IP address.
- establishes a secured connected to a second STM32H743I-EVAL board running
  the "SSL_Sever" application.
- sends an encrypted message to the second board.
- receives an answser from the server board.

Application logs are displayed on the LCD.

In case of success the green led is toggling.
In case of errors the red led is loggling.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.


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

    - mbedTLS/SSL_Client/Inc/ethernetif.h                   header for ethernetif.c file
    - mbedTLS/SSL_Client/Inc/main.h                         Main program header file
    - mbedTLS/SSL_Client/Inc/utilities_conf.h               Utilities configuration file
    - mbedTLS/SSL_Client/Inc/exc7200_conf.h                 EXC7200 TS component configuration file
    - mbedTLS/SSL_Client/Inc/is42s32800g_conf.h             IS42S32800G SDRAM configuration file
    - mbedTLS/SSL_Client/Inc/stmpe811_conf.h                STMPE811 IO expander component configuration file
    - mbedTLS/SSL_Client/Inc/ts3510_conf.h                  TS3510 TS component configuration file
    - mbedTLS/SSL_Client/Inc/stm32h743i_eval_conf.h         STM32H743I-EVAL configuration file
    - mbedTLS/SSL_Client/Inc/mbedtls_config.h               mbedTLS library configuration options
    - mbedTLS/SSL_Client/Inc/FreeRTOSConfig.h               FreeRTOS configuration options
    - mbedTLS/SSL_Client/Inc/lwipopts.h                     LwIP stack configuration options
    - mbedTLS/SSL_Client/Inc/stm32h7xx_it.h                 Interrupt handlers header file
    - mbedTLS/SSL_Client/Inc/stm32h7xx_hal_conf.h           Library Configuration file
    - mbedTLS/SSL_Client/Src/main.c                         Main program
    - mbedTLS/SSL_Client/Src/ssl_client.c                   SSL client main thread
    - mbedTLS/SSL_Client/Src/ethernetif.c                   Interfacing the LwIP stack to ETH driver
    - mbedTLS/SSL_Client/Src/stm32h7xx_hal_msp.c            HAL MSP module
    - mbedTLS/SSL_Client/Src/stm32h7xx_it.c                 Interrupt handlers
    - mbedTLS/SSL_Client/Src/stm32h7xx_hal_timebase_tim.c   HAL time base functions
    - mbedTLS/SSL_Client/Src/net_socket.c                   mbedTLS TCP/IP socket API implementation using LwIP.
    - mbedTLS/SSL_Client/Src/system_stm32h7xx.c             STM32 system clock configuration file


@par Hardware and Software environment

  - This application runs on stm32h743xx Devices.

  - This application has been tested with two scenarios.
     - Using a Host PC as server, running a Fedora 23 operating System or similar.
	 - Using a Windows PC as server, running Windows 7 operating system or later.
     - using two stm32h743i-eval boards the first as server, the second as client.

  - Scenario 1:
      - Remote PC Set-up
	  - Make sure that the PC is in the same local network as the baord.
	  - Using a Linux Machine
         - Download the mebedTLS source code from (https://tls.mbed.org/download/start/mbedtls-2.4.0-apache.tgz)
	     - build the mbedTLS package following the instructions in the https://github.com/ARMmbed/mbedtls#make
	     - Once done, run the server application "programs/ssl/ssl_server".
	  - Using a Windows Machine
	    - Follow the instructions in the file "Firmware\Utilities\PC_Software\ssl_server\readme.txt" to run the ssl_server application.
	  - the "ssl_server" will wait for remote connections.

  - Scenario 2:
      - a second stm32h743i-eval running the "mdedTLS/SSL_Server" application

  - STM32H743I-EVAL Set-up
    - Connect stm32h743i-eval to your local network (through a straight ethernet cable).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - edit the file "main.h" setup the correct "SERVER_NAME"
 - Rebuild all files and load your image into target memory
 - Run the application.

 @Note: 1) Please ensure that the remote PC IP address is the same IP address
           as the one defined in main.h in the "SERVER_NAME", and the port number
           used for the connection is the same as the one defined in "SERVER_PORT"

        2) Notice that the PC firewall may prevent the application from running correctly, by blocking
	       the communication via the "SERVER_PORT".

        3) If using two boards for testing and the DHCP is enabled then the "SSL_Server" needs to be
	       run first, to get the board IP Address and use it in the "SSL_Client" application.

	    4) If the board is connected to a LAN, ensure that it has a unique MAC address. The MAC address can be defined in the file
           "Inc/stm32h7xx_hal_conf.h"



 */
