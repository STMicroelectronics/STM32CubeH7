/**
  @page LwIP_HTTP_Server_Socket_RTOS LwIP HTTP Server Socket Application
  
  @verbatim
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Socket_RTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP http server Socket API Application.
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

This application guides STM32Cube HAL API users to run a http server application 
based on Socket API of LwIP TCP/IP stack
The communication is done with a web browser application in a remote PC.

this HTTP server contains two html pages:
  + the first page (home page) is static, it gives information about STM32H7
    and LwIP stack.
  + the second page is dynamically refreshed (every 1 s), it shows the RTOS 
    statistics in runtime

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
 + LED1: ethernet cable is connected.
 + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP to 1 in lwipopts.h)

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz
The HCLK for D1 Domain AXI peripherals, D2 Domain AHB peripherals and D3 Domain AHB  peripherals at 260 MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 130 MHz.

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

Connectivity, LwIP, Ethernet, HTTP Server, Socket, TCP/IP, FreeRTOS, DHCP	

@par Directory contents

  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/utilities_conf.h        Utilities configuration file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/ft5336_conf.h           FT3536 configuration file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/stm32h735g_discovery_conf.h STM32H735G-DK configuration file  
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/stm32h7xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/main.h                  Main program header file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/mx25lm51245g_conf.h     MX25LM51245G OSPI memory configuration file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/s70kl1281_conf.h        S70KL1281 OctalSPI memory configuration file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/FreeRTOSConfig.h        FreeRTOS configuration options
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/httpserver_socket.h     header for httpserver-socket.c
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/stm32h7xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/main.c                  Main program
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/httpserver_socket. c    httpserver socket main thread
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/stm32h7xx_hal_timebase_tim.c HAL time base based on the hardware TIM
  - LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/fsdata_custom.c         ROM filesystem data (html pages)
  

@par Hardware and Software environment

  - This application runs on STM32H723xx devices.
    
  - This application has been tested with the following environments:
     - STM32H735G-DK board
     - Http client: Google Chrome (v78)
     - DHCP server:  PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server    
      
  - STM32H735G-DK Set-up
    - Connect the discovery board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - PC must share the same LAN network configuration with the discovery board


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
