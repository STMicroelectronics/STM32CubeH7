/**
  @page LwIP_TCP_Echo_Server LwIP TCP Echo Server Application

  @verbatim
  ******************************************************************************
  * @file    LwIP/LwIP_TCP_Echo_Server/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LwIP TCP Echo server Application.
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

This application guides STM32Cube HAL API users to run TCP Echo Server application
based on Raw API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
  C:\>echotool IP_address /p tcp /r 7 /n 15 /t 2 /d Testing LwIP TCP echo server

where:
    – IP_address is the actual board’s IP address. If DHCP is disabled,
    the following static IP address is used: 192.168.0.10
    – /p transport layer protocol used for communication (TCP)
    – /r is the actual remote port on the echo server (echo port)
    – /n is the number of echo requests (for application, 15)
    – /t is the connection timeout in seconds (for application, 2)
    – /d is the message to be sent for echo

LEDs will inform user about ethernet cable status:
 + LED2: ethernet cable is connected.
 + LED3: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling
the DHCP process (#define LWIP_DHCP to 1 in lwipopts.h), in this case the allocated
address could be read from "gnetif.ip_addr" field:
While the application is executing, using the live watch feature, the value of "gnetif.ip_addr.addr" is displayed in hex format.
The previous hex value should be converted to ip address (using any hex to ip address converter tool available online).
For example if gnetif.ip_addr.addr = 0x0200A8C0, so the the IPv4 address is "192.168.0.2"

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz
The HCLK for D1 Domain AXI peripherals, D2 Domain AHB peripherals and D3 Domain AHB  peripherals at 260 MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 130 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, LwIP, Ethernet, TCP/IP, DHCP, echo server,

@par Directory contents

  - LwIP/LwIP_TCP_Echo_Server/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_TCP_Echo_Server/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_TCP_Echo_Server/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_TCP_Echo_Server/Inc/stm32h7xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_TCP_Echo_Server/Inc/main.h                  Main program header file
  - LwIP/LwIP_TCP_Echo_Server/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_TCP_Echo_Server/Inc/stm32h7xx_nucleo_conf.h   NUCLEO-H723ZG configuration file
  - LwIP/LwIP_TCP_Echo_Server/Inc/tcp_echoserver.h        Header for tcp echoserver application
  - LwIP/LwIP_TCP_Echo_Server/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_TCP_Echo_Server/Src/stm32h7xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_TCP_Echo_Server/Src/main.c                  Main program
  - LwIP/LwIP_TCP_Echo_Server/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - LwIP/LwIP_TCP_Echo_Server/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_TCP_Echo_Server/Src/tcp_echoserver.c        tcp echoserver application


@par Hardware and Software environment

  - This application runs on STM32H723xx devices.

  - This application has been tested with the following environments:
     - NUCLEO-H723ZG board
     - echotool: (http://bansky.net/echotool/) is used as echo client that sends
       data to the server and checking whether they came back
     - DHCP server:  PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server

  - NUCLEO-H723ZG Set-up
    - Connect the nucleo board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)

  - Remote PC Set-up
    - PC must share the same LAN network configuration with the nucleo board


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */
