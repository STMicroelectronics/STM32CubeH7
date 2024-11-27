/**
  @page LwIP_UDP_Echo_Client LwIP UDP Echo Client Application
  
  @verbatim
  ******************************************************************************
  * @file    LwIP/LwIP_UDP_Echo_Client/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP UDP Echo Client Application.
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

This application guides STM32Cube HAL API users to run a UDP Echo Client application 
based on Raw API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
    C:\>echotool /p udp /s
where:
    – /p transport layer protocol used for communication (UDP)
    – /s is the actual mode of connection (Server mode)
    
Each time the user pushes the Tamper button of the STM32H743xI_EVAL board, an echo 
request is sent to the server

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
 + LED1: ethernet cable is connected.
 + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP to 1 in lwipopts.h)

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"

@Note  The internal DMA of the Ethernet peripheral can't access the DTCM memory (@0x20000000)
       of the STM32H7, All data accessible by the Ethernet DMA must be located in the D1 or D2 SRAM.
       For more details please refer to "ethernetif.c" file.
       
@Note  In this application the D1 SRAM (@ 0x24000000) is used as system RAM, so the Ethernet DMA
       will be able to access buffers created dynamically by the LwIP stack.


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

Connectivity, LwIP, Ethernet, TCP/IP, DHCP, UDP echo client

@par Directory contents

  - LwIP/LwIP_UDP_Echo_Client/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_UDP_Echo_Client/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stm32h7xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_UDP_Echo_Client/Inc/main.h                  Main program header file
  - LwIP/LwIP_UDP_Echo_Client/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_UDP_Echo_Client/Inc/utilities_conf.h        Utilities configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/exc7200_conf.h          EXC7200 TS component configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/is42s32800g_conf.h      IS42S32800G SDRAM configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stmpe811_conf.h         STMPE811 IO expander component configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/ts3510_conf.h           TS3510 TS component configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stm32h743i_eval_conf.h  STM32H743I-EVAL configuration file  
  - LwIP/LwIP_UDP_Echo_Client/Inc/udp_echoclient.h        Header for UDP echoclient application
  - LwIP/LwIP_UDP_Echo_Client/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_UDP_Echo_Client/Src/stm32h7xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_UDP_Echo_Client/Src/main.c                  Main program
  - LwIP/LwIP_UDP_Echo_Client/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - LwIP/LwIP_UDP_Echo_Client/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_UDP_Echo_Client/Src/udp_echoclient.c        UDP echoclient application
  
                     
@par Hardware and Software environment

  - This application runs on STM32H743xx devices.
    
  - This application has been tested with the following environments:
     - STM32H743I-EVAL board
     - echotool: (http://bansky.net/echotool/) is used as echo server that sends
       back every incoming data.
     - DHCP server:  PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server     
      
  - STM32H743I-EVAL Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - PC must share the same LAN network configuration with the eval board
      the IP address must be the same as destination address value in (main.h)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
