/**
  @page LwIP_UDPTCP_Echo_Server_Netconn_RTOS LwIP UDP/TCP Echo Server Netconn Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    LwIP/LwIP_UDPTCP_Echo_Server_Netconn_RTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP UDP/TCP Echo Server Netconn API Application
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

This application guides STM32Cube HAL API users to run a UDP/TCP Echo Server application 
based on Netconn API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
  C:\>echotool IP_address /p tcp /r 7 /n 15 /t 2 /d Testing LwIP TCP echo server
or
  C:\>echotool IP_address /p udp /r 7 /l 7 /n 15 /t 2 /d Testing LwIP UDP echo server
where:
    � IP_address is the actual board�s IP address. By default, the following 
    static IP address is used: 192.168.0.10
    � /p transport layer protocol used for communication (TCP or UDP)
    � /r is the actual remote port on the echo server (echo port)
    - /l is the actual local port for the client (echo port)
    � /n is the number of echo requests (for application, 15)
    � /t is the connection timeout in seconds (for application, 2)
    � /d is the message to be sent for echo 

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
 + LED1: ethernet cable is connected.
 + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP to 1 in lwipopts.h)

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals and initialize the Flash interface.
The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as time base for the HAL drivers.
This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms. 
The systick is then used for FreeRTOS time base.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"


@Note  The internal DMA of the Ethernet peripheral can't access the DTCM memory (@0x20000000)
       of the STM32H7, All data accessible by the Ethernet DMA must be located in the D1 or D2 SRAM.
       For more details please refer to "ethernetif.c" file.
       
@Note  In this application the D1 SRAM (@ 0x24000000) is used as system RAM, so the Ethernet DMA
       will be able to access buffers created dynamically by the LwIP stack.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, LwIP, Ethernet, Echo Server, Netconn, TCP/IP, RTOS, DHCP	

@par Directory contents

  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/stm32h7xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/main.h                  Main program header file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/FreeRTOSConfig.h        FreeRTOS configuration options
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/tcpecho.h               header for tcpecho.c
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Inc/udpecho.h               header for udpecho.c
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/stm32h7xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/main.c                  Main program
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/system_stm32h7xx.c      STM32 system clock configuration file
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/tcpecho.c               tcp echo server main thread
  - LwIP/LwIP_UDPTCP_Echo_Server_Netconn/Src/udpecho.c               udp echo server main thread
  
        
@par Hardware and Software environment

  - This application runs on STM32H743xx devices.
    
  - This application has been tested with the following environments:
     - STM32H743I-EVAL board
     - echotool: (http://bansky.net/echotool/) is used as echo client that sends
       data to the server and checking whether they came back 
     - DHCP server:  PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server    
      
  - STM32H743I-EVAL Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - PC must share the same LAN network configuration with the eval board


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
