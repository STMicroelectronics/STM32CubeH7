/**
  @page ExtMem_Boot External memory code execution Readme file

  @verbatim
  ******************************************************************************
  * @file    ExtMem_Boot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of implementation of the AN5188 (External memory boot
  *          with reduced internal flash) on STM32H7xx devices.
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

@par Example Description

This directory contains a set of sources files and pre-configured projects that 
describes how to build an application for execution from external memory using 
the ExtMem_Boot firmware.

@note You must not touch any part of the system that been initialized by the
      ExtMem_Boot firmware to avoid system failure, more details in the AN5188.

The STM32H7B0xxQ value line come with only 128KB internal flash. 
These value line devices are intended for code execution from external memories.
This example shows how to boot from internal flash, configure external memories 
then jump to user application located on external memory.

On the STM32H7B3I_DISCO, user can select OSPI flash or external SDRAM for code execution.
In case of code execution from external SDRAM, the application binary can be stored 
on the OSPI flash or the SDMMC flash. 
the Boot application will then copy the application binary to the target execution memory.
User can select internal RAM or external SDRAM for application data memory

Define "CODE_AREA" , "DATA_AREA" and "BINARY_AREA" in file memory.h are used respectively 
to select the Code execution area, the application data area and the binary storage area.
The ExtMem_Boot program is located at 0x08000000 (Internal flash).  
 
Please Refer to the bellow "Table 1. IAP implementation on STM32H7B3I_DISCO" for different configurations and corresponding settings.
                                  
At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B0xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

The template includes also two functions used to configure MPU feature and to enable the CPU Cache,
respectively MPU_Config() and CPU_CACHE_Enable().
These functions are provided as template implementation that User may integrate in his application,
to enhance the performance in case of use of AXI interface with several masters.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

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
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case, it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

External, Memory, OSPI, SDMMC, SD Card, Boot, Loader, XiP, BootROM, Value line

@par Directory contents

 - "ExtMem_Boot/Inc": contains the ExtMem_Boot firmware header files
    - ExtMem_Boot/Inc/ffconf.h                    FAT file system module configuration file
    - ExtMem_Boot/Inc/is42s16800j_conf.h          Configurations required for the IS42S16800J SDRAM memory.
    - ExtMem_Boot/Inc/main.h                      The main include file of the project.
    - ExtMem_Boot/Inc/memory.h                    This file provides all the headers for all external memory control function along with configuration defines
    - ExtMem_Boot/Inc/mx25lm51245g_conf.h         Configurations required for the MX25LM51245G Macronix memory.
    - ExtMem_Boot/Inc/sd_diskio.h                 uSD diskio header file
    - ExtMem_Boot/Inc/stm32h7b3i_discovery_conf.h STM32H7B3I-DISCO board configuration file
    - ExtMem_Boot/Inc/stm32h7xx_hal_conf.h        Library Configuration file
    - ExtMem_Boot/Inc/stm32h7xx_it.h              Header for stm32h7xx_it.c

 - "ExtMem_Boot/Src": contains the ExtMem_Boot firmware source files
    - ExtMem_Boot/Src/main.c                      Main program
    - ExtMem_Boot/Src/stm32h7xx_it.c              Interrupt handlers
    - ExtMem_Boot/Src/system_stm32h7xx.c          STM32H7xx system source file
    - ExtMem_Boot/Src/ospi.c                      This file contains configuration required to initialize OSPI  and configure in memory mapped mode.
    - ExtMem_Boot/Src/fmc.c                       This file contains configuration for all kind of memory  controlled by the FMC
    - ExtMem_Boot/Src/sdmmc.c                     This file contains configuration for Micro SD card
    - ExtMem_Boot/Src/sd_diskio.c                 FatFs uSD diskio driver

@par Hardware and Software environment

  - This example runs on STM32H7B0xxQ devices.

  - This example has been tested with STM32H7B3I-DISCO and can be
    easily tailored to any other supported device and development board.

Table 1. IAP implementation on STM32H7B3I-DISCO
/****************|***** Configuration ********* |************* Implementation **************************| 
|                |                              | OSPI Flash is used for code execution                 |
|                | DATA_AREA: USE_INTERNAL_SRAM | The user application image is linked and stored       |
|     XIP        |            USE_EXTERNAL_SDRAM| on the OSPI flash located at 0x90000000.              |
|                | CODE_AREA: USE_OSPI          |                                                       | 
|----------------|----------------------------- |-------------------------------------------------------| 
|                | BINARY_AREA: USE_SPI_NOR     | Code execution from external SDRAM                    | 
|                |              USE_SDMMC       | The user application is linked to the SDRAM 0xD0000000| 
|    BootROM     | DATA_AREA: USE_INTERNAL_SRAM | the binary image stored on storage memory             |
|                | CODE_AREA: USE_EXTERNAL_SDRAM| either SD or SPI_Nor memory and will be copied        |
|                |                              | into SDRAM at boot                                    | 
|----------------|----------------------------- |-------------------------------------------------------| 

(1) User application location address is defined in the memory.h file as:
      -OSPI:    #define APPLICATION_ADDRESS           ((uint32_t)0x90000000)
      -SDRAM:   #define APPLICATION_ADDRESS           ((uint32_t)0xD0000000)
Following configuration are supported by this boot firmware, Please refer to fmc.c and ospi.c
for exact memories part number:
  - XiP (Execute In Place):
      External Flash is configured and set to memory mapped mode.
      Set the Main Stack Pointer and then jump to application entry point.
        - OSPI : @ 0x90000000
            CODE_AREA   USE_OSPI
        - SDRAM : @ 0xD0000000
            CODE_AREA   USE_EXTERNAL_SDRAM
  - Volatile Memory :
      Memory Configuration (memory mapped mode)
        - Internal SRAM  : @ 0x20000000
             DATA_AREA    USE_INTERNAL_SRAM
        - External SDRAM : @ 0xD0000000
             DATA_AREA    USE_EXTERNAL_SDRAM

Supported configuration by STM32H7B3I-DISCO:
    - XiP From OSPI, Data on Internal SRAM
    - XiP From OSPI, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM

@Note Configuration used in ExtMem_Boot and ExtMem_Application must be inline otherwise required resources
      will not be initialized as required.


@par How to use it?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h.
     The default configuration is: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_OSPI
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
      - For an XiP configuration (eXecute in Place from OSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug

      - For a BootROM configuration (BootROM external SDRAM):
        - If BINARY_AREA is USE_SPI_NOR then use the STM32CubeProgarmmer tool, select OSPI external flash loader "MX25LM51245G_STM32H7B3I-DISCO"
          and load the Project.bin (application binary output file) to the OSPI memory at the address 0x90000000
        - If BINARY_AREA is USE_SDMMC Copy_Paste the Project.bin to the the µSD Card then Connect it to the Micro SD connector (CN4). 
        - In order to debug the  external memory application, user shall attach the debugger, and perform a reset


 */
