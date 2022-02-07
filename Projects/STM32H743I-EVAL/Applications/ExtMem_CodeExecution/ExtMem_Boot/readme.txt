/**
  @page ExtMem_Boot Boot application for External memory execution Readme file

  @verbatim
  ******************************************************************************
  * @file    ExtMem_CodeExecution/ExtMem_Boot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of External memory boot
  *          with reduced internal flash (ExtMem_CodeExecution on STM32H7xx devices) AN5188.
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

This directory contains a set of sources files and pre-configured projects that
describes how to build an application for execution from external memory using
the ExtMem_Boot firmware.

@note You must not touch any part of the system that been initialized by the
      ExtMem_Boot firmware to avoid system failure, more details in the AN5188.

The STM32H750xx value line come with only 128KB internal flash.
These value line are intended for code execution from external memories.
This application shows how to boot from internal flash, configure external memories
then jump to user application located on external memory.

On the STM32H750I_EVAL, user can select QSPI flash , FMC-NOR flash, external SDRAM
or external SRAM for code execution.

In case of code execution from external SRAM/SDRAM, the application binary can be stored
on the micro-SD card or on the QSPI flash.
The Boot application will then copy the application binary to the target execution memory.

User can select internal RAM, external SRAM or external SDRAM for application data memory

Define "CODE_AREA" , "DATA_AREA" and "BINARY_AREA" in file memory.h are used respectively
to select the Code execution area, the application data area and the binary storage area.
The ExtMem_Boot program is located at 0x08000000 (Internal flash).

Please Refer to the bellow "Table 1. IAP implementation on STM32H743I-EVAL" for different configurations and corresponding settings.

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      In case of constraints it is possible to configure the MPU as "Write through/not shareable" to guarantee the cache coherence at write access but the user
      has to ensure the cache maintenance at read access though.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

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
                must be  properly defined to be aligned to L1-CACHE line size (32 bytes).
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"


@par Keywords

External, Memory, QSPI, SDMMC, SD Card, Boot, Loader, XiP, BootROM, Value line

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
    - ExtMem_Boot/Src/sdcard.c               This file contains configuration for µSD memory

@par Hardware and Software environment

  - This application runs on STM32H750xx devices.

  - This application has been tested with STM32H743I-Eval and can be
    easily tailored to any other supported device and development board.

Table 1. IAP implementation on STM32H743I-EVAL
/****************|***** Configuration ********* |************* Implementation **************************|
|                |                              | QSPI Flash is used for code execution                 |
|                | DATA_AREA: USE_INTERNAL_SRAM | The user application image is linked and stored       |
|                |            USE_EXTERNAL_SDRAM| on the QSPI flash located at 0x90000000.              |
|                |            USE_EXTERNAL_SRAM |                                                       |
|                | CODE_AREA: USE_QSPI          |                                                       |
|                |----------------------------- |-------------------------------------------------------|
|    XIP         |                              | FMC-NOR Flash is used for code execution              |
|                | DATA_AREA: USE_INTERNAL_SRAM | The user application image is linked and stored       |
|                |            USE_EXTERNAL_SDRAM| on the FMC-NOR flash located at 0x60000000.           |
|                |            USE_EXTERNAL_SRAM |                                                       |
|                | CODE_AREA: USE_NOR           |                                                       |
|----------------|----------------------------- |-------------------------------------------------------|
|                | BINARY_AREA: USE_SPI_NOR     | Code execution from external SDRAM                    |
|                |              USE_SDCARD      | The user application is linked to the SDRAM 0xD0000000|
|                | DATA_AREA: USE_INTERNAL_SRAM | the binary image stored on storage memory             |
|                | CODE_AREA: USE_EXTERNAL_SDRAM| either SDCard or SPI_Nor memory and will be copied    |
|                |                              | into SDRAM at boot                                    |
|    BootROM     |----------------------------- |-------------------------------------------------------|
|                | BINARY_AREA: USE_SPI_NOR     | Code execution from external SRAM                     |
|                |              USE_SDCARD      | The user application is linked to the SRAM 0x68000000 |
|                | DATA_AREA: USE_INTERNAL_SRAM | the binary image stored on storage memory             |
|                | CODE_AREA: USE_EXTERNAL_SRAM | either SDCard or SPI_Nor memory and will be copied    |
|                |                              | into SRAM at boot                                     |
|----------------|----------------------------- |-------------------------------------------------------|

(1) User application location address is defined in the memory.h file as:
      -QSPI:   #define APPLICATION_ADDRESS           ((uint32_t)0x90000000)
      -NOR :    #define APPLICATION_ADDRESS           ((uint32_t)0x60000000)
      -SDRAM:   #define APPLICATION_ADDRESS           ((uint32_t)0xD0000000)
      -SRAM:    #define APPLICATION_ADDRESS           ((uint32_t)0x68000000)
Following configuration are supported by this boot firmware, Please refer to fmc.c and qspi.c
for exact memories part number:
  - XiP (Execute In Place):
      External Flash is configured and set to memory mapped mode.
      Set the Main Stack Pointer and then jump to application entry point.
        - QSPI : @ 0x90000000
            CODE_AREA   USE_QSPI
        - NOR  : @ 0x60000000
            CODE_AREA   USE_NOR
        - SDRAM : @ 0xD0000000
            CODE_AREA   USE_EXTERNAL_SDRAM
        - SRAM  : @ 0x68000000
            CODE_AREA   USE_EXTERNAL_SRAM

  - Volatile Memory :
      Memory Configuration (memory mapped mode)
        - Internal SRAM  : @ 0x20000000
             DATA_AREA    USE_INTERNAL_SRAM
        - External SDRAM : @ 0xD0000000
             DATA_AREA    USE_EXTERNAL_SDRAM
        - External SRAM  : @ 0x68000000
             DATA_AREA    USE_EXTERNAL_SRAM

Supported configuration by STM32H743I-EVAL:
    - XiP From QSPI, Data on Internal SRAM
    - XiP From QSPI, Data on External SRAM
    - XiP From QSPI, Data on External SDRAM
    - XiP From NOR, Data on Internal SRAM
    - XiP From NOR, Data on External SRAM
    - XiP From NOR, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM
    - BootROM : Execution From External SRAM , Data on Internal SRAM


@Note Configuration used in ExtMem_Boot and ExtMem_Application must be inline otherwise required resources
      will not be initialized as required.


@par How to use it?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h.
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Use corresponding project configuration for ExtMem_CodeExecution/ExtMem_Application/..
  4. Program the external memory with "ExtMem_CodeExecution/ExtMem_Applicaiton/.." (see below).
  5. Start debugging user application as usual or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the ExtMem_Application/... to th external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program:
      - For an XiP configuration (eXecute in Place from QSPI, FMC-NOR):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using SW4STM32 :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MT25TL01G_STM32H7xxI-EVAL" in case of XiP from QSPI
            - Select the FMC-NOR external flash loader "M29W128GL_STM32H743I-EVAL" in case of XiP from FMC-NOR
            - From Erasing & Programming menu, browse and open the output hex file relative to the ExtMem_Application\... configuration
            - Load the hex file into the external QSPI flash using "Start Programming"

      - For a BootROM configuration (BootROM externalSRAM/external SDRAM):
        - if BINARY_AREA is USE_SPI_NOR then use the STM32CubeProgarmmer tool, select QSPI external flash loader "MT25TL01G_STM32H7xxI-EVAL"
          and load the project.bin (application binary output file) to the QSPI memory at the address 0x90000000
        - if BINARY_AREA is USE_SDCARD then copy the project.bin to a micro-SD to be plugged on CN13 and reset the board.
        - In order to debug the  ExtMem_Application/.., user shall attach the debugger, and perform a reset


 */
