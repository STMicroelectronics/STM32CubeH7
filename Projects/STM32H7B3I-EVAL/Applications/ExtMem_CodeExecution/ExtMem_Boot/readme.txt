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

This directory contains a set of sources files and pre-configured projects that describes 
how to build an application for execution from external memory using the ExtMem_Boot firmware.

@note You must not touch any part of the system that been initialized by the
      ExtMem_Boot firmware to avoid system failure, more details in the AN5188.

The STM32H7B3I-EVAL comes with  2MB internal flash.
This application shows how to boot from internal flash, configure external memories
then jump to user application located on external memory.

On the STM32H7B3I-EVAL, user can select OSPI flash , FMC-NOR flash, external SDRAM
or external SRAM for code execution.

In case of code execution from external SRAM/SDRAM, the application binary can be stored on the micro-SD card or on the OSPI flash 
then the Boot application will copy the application binary to the target execution memory.

User can select internal RAM, external SRAM or external SDRAM for application data memory

Define "CODE_AREA" , "DATA_AREA" and "BINARY_AREA" in file memory.h are used respectively
to select the Code execution area, the application data area and the binary storage area.
The ExtMem_Boot program is located at 0x08000000 (Internal flash).

Please Refer to the below "Table 1. IAP implementation on STM32H7B3I-EVAL" for different configurations and corresponding settings.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

The project includes also two functions used to configure MPU feature and to enable the CPU Cache,
respectively MPU_Config() and CPU_CACHE_Enable().
These functions are provided as template implementation that User may integrate in his application,
to enhance the performance in case of use of AXI interface with several masters.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
	  the ITCM-RAM.

@note For the chosen configuration check the appropriate JP positions 
- Make sure JP4 and JP23 are in OCTOSPI1 position.
- Make sure JP2 is in SRAM position.
- Make sure JP32 and JP35 are in NOR position 
- Make sure JP15 and JP12 are in SDIO1 position

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

External, OSPI, Memory, Boot, Loader, XiP, BootROM, Value line

@par Directory contents

 - "ExtMem_Boot/Inc": contains the ExtMem_Boot firmware header files
    - ExtMem_Boot/Inc/main.h                 The main include file of the project.
    - ExtMem_Boot/Inc/memory.h               This file provides all the headers for all external memory
                                                       control function along with configuration defines
    - ExtMem_Boot/Inc/memory_msp.h           Header for msp function needed for external memory initialization.
    - ExtMem_Boot/Inc/stm32h7xx_hal_conf.h   Library Configuration file
    - ExtMem_Boot/Inc/stm32h7xx_it.h         Header for stm32h7xx_it.c
    - ExtMem_Boot/Inc/stm32h7b3i_eval_conf.h STM32H7B3I_EVAL board configuration file
    - ExtMem_Boot/Inc/mx25lm51245g_conf.h    MX25LM51245G OSPI memory Description file
    - ExtMem_Boot/Inc/ft5336_conf.h          Specific configuration for the capacitive touch panel that can be modified by user.
    - ExtMem_Boot/Inc/is42s32800j_conf.h     Configurations required for the IS42S32800J SDRAM memory.

 - "ExtMem_Boot/Src": contains the ExtMem_Boot firmware source files
    - ExtMem_Boot/Src/main.c                 Main program
    - ExtMem_Boot/Src/stm32h7xx_it.c         Interrupt handlers
    - ExtMem_Boot/Src/stm32h7xx_hal_msp.c    Microcontroller specific packages initialization file.
    - ExtMem_Boot/Src/system_stm32h7xx.c     STM32H7xx system source file
    - ExtMem_Boot/Src/memory_msp.c           This file contains external memory control function along
                                                       with configuration defines
    - ExtMem_Boot/Src/ospi.c                 This file contains configuration required to initialize OSPI
                                                       and configure in memory mapped mode.
    - ExtMem_Boot/Src/fmc.c                  This file contains configuration for all kind of memory
                                                       controlled by the FMC
    - ExtMem_Boot/Src/sdcard.c               This file contains configuration for µSD memory

@par Hardware and Software environment

  - This application runs on STM32H7B3IxxQ devices.

  - This application has been tested with STM32H7B3I-EVAL and can be
    easily tailored to any other supported device and development board.

Table 1. IAP implementation on STM32H7B3I-EVAL 
/****************|***** Configuration ********* |************* Implementation **************************|
|                |                              | OSPI Flash is used for code execution                 |
|                | DATA_AREA: USE_INTERNAL_SRAM | The user application image is linked and stored       |
|                |            USE_EXTERNAL_SDRAM| on the OSPI flash located at 0x90000000.              |
|                |            USE_EXTERNAL_SRAM |                                                       |
|                | CODE_AREA: USE_OSPI          |                                                       |
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
      -OSPI:    #define APPLICATION_ADDRESS           ((uint32_t)0x90000000)
      -NOR :    #define APPLICATION_ADDRESS           ((uint32_t)0x60000000)
      -SDRAM:   #define APPLICATION_ADDRESS           ((uint32_t)0xD0000000)
      -SRAM:    #define APPLICATION_ADDRESS           ((uint32_t)0x68000000)
Following configuration are supported by this boot firmware, Please refer to fmc.c and ospi.c
for exact memories part number:
  - XiP (Execute In Place):
      External Flash is configured and set to memory mapped mode.
      Set the Main Stack Pointer and then jump to application entry point.
        - OSPI : @ 0x90000000
            CODE_AREA   USE_OSPI
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

Supported configuration by STM32H7B3I-EVAL :
    - XiP From OSPI, Data on Internal SRAM
    - XiP From OSPI, Data on External SRAM
    - XiP From OSPI, Data on External SDRAM
    - XiP From NOR, Data on Internal SRAM
    - XiP From NOR, Data on External SRAM
    - XiP From NOR, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM
    - BootROM : Execution From External SRAM , Data on Internal SRAM


@Note Configuration used in ExtMem_Boot and ExtMem_Application must be inline otherwise required resources
      will not be initialized as required.


@par How to use it?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in ExtMem_CodeExecution\ExtMem_Boot\Inc application.
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Use corresponding project configuration for ExtMem_CodeExecution\ExtMem_Application\..
  4. Program the external memory with "ExtMem_CodeExecution\ExtMem_Applicaiton\.." (see below).
  5. Start debugging user application as usual or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
	  - Note that for SDCard configurations 
        + STM32CubeIDE : Rebuild all files (Optimize for Size: -Os) and load your image into target memory

In order to load the ExtMem_Application\... to th external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program:
      - For an XiP configuration (eXecute in Place from OSPI, FMC-NOR):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using STM32CubeIDE you must do the following and in the debug configuration -> Debugger Tab :

            - In debug probe choose ST-LINK GDB server
            - check External Loader 
            - Press the Scan button
            - Select the correct external loader for your memory:
              + Select the OSPI external flash loader "MX25LM51245G_STM32H7B3I-EVAL" in case of XiP from OSPI and Press Debug
              + Select the FMC-NOR external flash loader "M29W128GL_STM32H7B3I-EVAL" in STM32CubeProgrammer tool in case of XiP from FMC-NOR
			    and load the application binary output file to the FMC-NOR memory at the address 0x60000000
		    - Run the example
            

      - For a BootROM configuration (BootROM externalSRAM/external SDRAM):
        - if BINARY_AREA is USE_SPI_NOR then use the STM32CubeProgrammer tool, select OSPI external flash loader "MX25LM51245G_STM32H7B3I-EVAL"
          and load the project.bin (application binary output file) to the OSPI memory at the address 0x90000000
        - if BINARY_AREA is USE_SDCARD then copy the project.bin to a micro-SD to be plugged on CN28 and reset the board.
        - In order to debug the  ExtMem_Application/.., user shall attach the debugger, and perform a reset


 */
