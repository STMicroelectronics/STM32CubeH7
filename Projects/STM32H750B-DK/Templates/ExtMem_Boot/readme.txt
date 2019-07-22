/**
  @page Boot Templates for External memory code execution Readme file

  @verbatim
  ******************************************************************************
  * @file    Templates/ExtMem_Boot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of implementation of the AN5188 (External memory boot
  *          with reduced internal flash) on STM32H7xx devices.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
   @endverbatim

@par Example Description

This directory contains a set of sources files and pre-configured projects that 
describes how to build an application for execution from external memory using 
the ExtMem_Boot firmware.

@note You must not touch any part of the system that been initialized by the
      ExtMem_Boot firmware to avoid system failure, more details in the AN5188.

The STM32H750xx value line come with only 128KB internal flash. 
These value line devices are intended for code execution from external memories.
This example shows how to boot from internal flash, configure external memories 
then jump to user application located on external memory.

On the STM32H750B_DISCO, user can select QSPI flash or external SDRAM for code execution.
In case of code execution from external SDRAM, the application binary can be stored 
on the QSPI flash or the eMMC flash. 
the Boot application will then copy the application binary to the target execution memory.
User can select internal RAM or external SDRAM for application data memory

Define "CODE_AREA" , "DATA_AREA" and "BINARY_AREA" in file memory.h are used respectively 
to select the Code execution area, the application data area and the binary storage area.
The ExtMem_Boot program is located at 0x08000000 (Internal flash).  
 
Please Refer to the bellow "Table 1. IAP implementation on STM32H750B-DISCO" for different configurations and corresponding settings.
                                  
@par Keywords

Middleware, External Memory Boot, Loader, Boot

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

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

@par Directory contents

 - "ExtMem_Boot/Inc": contains the ExtMem_Boot firmware header files
    - ExtMem_Boot/Inc/main.h                 The main include file of the project.
    - ExtMem_Boot/Inc/memory.h               This file provides all the headers for all external memory
                                                       control function along with configuration defines
    - ExtMem_Boot/Inc/memory_msp.h           Header for msp function needed for external memory initialization.
    - ExtMem_Boot/Inc/stm32h7xx_hal_conf.h   Library Configuration file
    - ExtMem_Boot/Inc/stm32h7xx_it.h         Header for stm32h7xx_it.c

 - "ExtMem_Boot/Src": contains the ExtMem_Boot firmware source files
    - ExtMem_Boot/Src/main.c                 Main program
    - ExtMem_Boot/Src/stm32h7xx_it.c         Interrupt handlers
    - ExtMem_Boot/Src/stm32h7xx_hal_msp.c    Microcontroller specific packages initialization file.
    - ExtMem_Boot/Src/system_stm32h7xx.c     STM32H7xx system source file
    - ExtMem_Boot/Src/memory_msp.c           This file contains external memory control function along
                                                       with configuration defines
    - ExtMem_Boot/Src/qspi.c                 This file contains configuration required to initialize QSPI
                                                       and configure in memory mapped mode.
    - ExtMem_Boot/Src/fmc.c                  This file contains configuration for all kind of memory
                                                       controlled by the FMC
    - ExtMem_Boot/Src/mmc.c                  This file contains configuration for eMMC memory

@par Hardware and Software environment

  - This example runs on STM32H750xx devices.

  - This example has been tested with STM32H750B-DISCO and can be
    easily tailored to any other supported device and development board.

Table 1. IAP implementation on STM32H750B-DISCO
/****************|***** Configuration ********* |************* Implementation **************************| 
|                |                              | QSPI Flash is used for code execution                 |
|                | DATA_AREA: USE_INTERNAL_SRAM | The user application image is linked and stored       |
|     XIP        |            USE_EXTERNAL_SDRAM| on the QSPI flash located at 0x90000000.              |
|                | CODE_AREA: USE_QSPI          |                                                       | 
|----------------|----------------------------- |-------------------------------------------------------| 
|                | BINARY_AREA: USE_SPI_NOR     | Code execution from external SDRAM                    | 
|                |              USE_MMC         | The user application is linked to the SDRAM 0xD0000000| 
|    BootROM     | DATA_AREA: USE_INTERNAL_SRAM | the binary image stored on storage memory             |
|                | CODE_AREA: USE_EXTERNAL_SDRAM| either eMMC or SPI_Nor memory and will be copied      |
|                |                              | into SDRAM at boot                                    | 
|----------------|----------------------------- |-------------------------------------------------------| 

(1) User application location address is defined in the memory.h file as:
      -QSPI:    #define APPLICATION_ADDRESS           ((uint32_t)0x90000000)
      -SDRAM:   #define APPLICATION_ADDRESS           ((uint32_t)0xD0000000)
Following configuration are supported by this boot firmware, Please refer to fmc.c and qspi.c
for exact memories part number:
  - XiP (Execute In Place):
      External Flash is configured and set to memory mapped mode.
      Set the Main Stack Pointer and then jump to application entry point.
        - QSPI : @ 0x90000000
            CODE_AREA   USE_QSPI
        - SDRAM : @ 0xD0000000
            CODE_AREA   USE_EXTERNAL_SDRAM
  - Volatile Memory :
      Memory Configuration (memory mapped mode)
        - Internal SRAM  : @ 0x20000000
             DATA_AREA    USE_INTERNAL_SRAM
        - External SDRAM : @ 0xD0000000
             DATA_AREA    USE_EXTERNAL_SDRAM

Supported configuration by STM32H750B-DISCO:
    - XiP From QSPI, Data on Internal SRAM
    - XiP From QSPI, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM

@Note Configuration used in ExtMem_Boot and ExtMem_Application must be inline otherwise required resources
      will not be initialized as required.


@par How to use it?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h.
     The default configuration is: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with your application (see below).
  4. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the application to the external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program:
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using SW4STM32 :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" in case of XiP from QSPI
            - From Erasing & Programming menu, browse and open the output binary file relative to your application
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

      - For a BootROM configuration (BootROM external SDRAM):
        - If BINARY_AREA is USE_SPI_NOR then use the STM32CubeProgarmmer tool, select QSPI external flash loader "MT25TL01G_STM32H750B-DISCO"
          and load the Project.bin (application binary output file) to the QSPI memory at the address 0x90000000
        - If BINARY_AREA is USE_MMC then use the STM32CubeProgarmmer tool, select eMMC e loader "MTFC4GACAJCN_STM32H750B-DISCO"
          and load the Project.bin (application binary output file) to the eMMC memory at the address 0xA0000000.
        - In order to debug the  external memory application, user shall attach the debugger, and perform a reset

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
