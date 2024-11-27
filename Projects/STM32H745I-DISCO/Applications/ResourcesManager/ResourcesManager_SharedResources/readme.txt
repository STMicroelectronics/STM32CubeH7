/**
  @page ResourcesManager_SharedResources SharedResources application
  
  @verbatim
  ******************************************************************************
  * @file    ResourcesManager/ResourcesManager_SharedResources/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the ResourcesManager SharedResources application application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

This application shows how to use the ResourcesManager in order to share resources between cores.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

In this application:
    - System Init, voltage scaling, UART and system clock configuration are done only once by CPU1(CM7). 
      
    - CPU1(CM7) and CPU2 follow the same sequence.
    - Each CPU Initializes the ReourceManager Utility.
    - Each CPU request the UART1 resource in a loop:
      * If the reource is busy (used by the other CPU), try again .
      * If the reource is free (Released), the CPU can use the UART reource and sent his message.
   
STM32H745I-DISCO board's LEDs and Uart messages can be used to monitor the execution status:
    - CM7 using the the UART1 resource:
      * LED1 ON during 1 Seconde.
      * UART message will be print the UART Terminal:"This Message is from CM7 Core"
      * When there is an error occurrence, LED1 toggles each 100ms .
    - CM4 using the the UART1 resource:
      * LED2 ON during 2 Seconde.
      * UART message will be print the UART Terminal:"THIS MESSAGE IS FROM CM4 CORE"  
      * When there is an error occurrence, LED2 toggles each 100ms .

    
STM32H745I-DISCO board's USART3 is connected to stlink Virtual Communication Port (VCP):

The UART is configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - Stop Bit = 1
    - Parity = None
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      

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

ResourcesManager, multiprocessor, Resource share, Dual

@par Directory contents

    - ResourcesManager/ResourcesManager_SharedResources/Commmon/Src/system_stm32h7xx.c  STM32H7xx system configuration source file
	
    - ResourcesManager/ResourcesManager_SharedResources/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M7
    - ResourcesManager/ResourcesManager_SharedResources/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M7
    - ResourcesManager/ResourcesManager_SharedResources/CM7/Inc/main.h                  Header for main.c module for Cortex-M7  
    - ResourcesManager/ResourcesManager_SharedResources/CM7/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M7
    - ResourcesManager/ResourcesManager_SharedResources/CM7/Src/main.c                  Main program for Cortex-M7
    - ResourcesManager/ResourcesManager_SharedResources/CM7/Src/stm32h7xx_hal_msp.c     The MSP for Cortex-M7
    
    - ResourcesManager/ResourcesManager_SharedResources/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
    - ResourcesManager/ResourcesManager_SharedResources/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
    - ResourcesManager/ResourcesManager_SharedResources/CM4/Inc/main.h                  Header for main.c module for Cortex-M4  
    - ResourcesManager/ResourcesManager_SharedResources/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
    - ResourcesManager/ResourcesManager_SharedResources/CM4/Src/main.c                  Main program for Cortex-M4
    - ResourcesManager/ResourcesManager_SharedResources/CM4/Src/stm32h7xx_hal_msp.c     The MSP for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H745xx devices.
    
  - This application has been tested with STM32H745I_DISCO  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your 2 images into target memory for each core.
 - Reset the Board so the 2 CPUs start running their respective code.



 */
