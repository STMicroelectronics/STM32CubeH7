/**
  @page FreeRTOS FreeRTOS Thread Creation application

  @verbatim
  ******************************************************************************
  * @file    ExtMem_CodeExecution/ExtMem_Application/FreeRTOS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Thread Creation application with execution from external memory
  *          AN5188.
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

How to implement thread creation using CMSIS RTOS API with execution from external memory.

This application creates two threads with the same priority, which execute in
a periodic cycle of 18 seconds.

In the first 6 seconds, the thread 1 toggles LED3 each 200 ms and the
thread 2 toggles LED1 each 500 ms
In the second 6 seconds, the thread 1 suspends itself and the thread 2
continues toggling LED1
In the last 6 seconds, the thread 2 resumes execution of thread 1 then
suspends itself, the thread 1 toggles the LED3 each 400 ms

This application provide different configuration with linker files which allows different eXecution schemas
  Supported configuration by STM32H743I-Eval:
    - XiP From QSPI, Data on Internal SRAM
    - XiP From QSPI, Data on External SRAM
    - XiP From QSPI, Data on External SDRAM
    - XiP From NOR, Data on Internal SRAM
    - XiP From NOR, Data on External SRAM
    - XiP From NOR, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM
    - BootROM : Execution From External SRAM , Data on Internal SRAM
@note In BootROM configuration , application code is initially stored on sdcard or SPI nor then loaded to the SDRAM or SRAM after boot.
@note Make sure that chosen configuration matches ExtMem_Boot configuration in memory.h file.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size config TOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to
      the OS resources memory requirements of the application with +10% margin and rounded to the
      upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications
on STM32Cube with RTOS".


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

External Memory, QSPI, code execution, Loader, Boot, XiP, BootROM, Value line, RTOS, Threading 

@par Directory contents

 - "FreeRTOS/Inc": contains the FreeRTOS firmware header files
    - FreeRTOS/Inc/main.h                Main program header file
    - FreeRTOS/Inc/stm32h7xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - FreeRTOS/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file

 - "FreeRTOS/Src": contains the FreeRTOS firmware source files
    - FreeRTOS/Src/main.c                Main program
    - FreeRTOS/Src/stm32h7xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32H750xx devices.

  - This application has been tested with STMicroelectronics STM32H743 Eval
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in ExtMem_CodeExecution\ExtMem_Boot application.
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Use corresponding project configuration for ExtMem_Application/FreeRTOS.
  4. Program the external memory with "ExtMem_Application/FreeRTOS" application (see below).
  5. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the ExtMem_Application/FreeRTOS to th external memory:
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
              - From Erasing & Programming menu, browse and open the output hex file relative to the ExtMem_Application\FreeRTOS configuration
              - Load the hex file into the external QSPI flash using "Start Programming"

      - For a BootROM configuration (BootROM externalSRAM/external SDRAM):
        - if BINARY_AREA is USE_SPI_NOR then use the STM32CubeProgarmmer tool, select QSPI external flash loader "MT25TL01G_STM32H7xxI-EVAL"
          and load the project.bin (application binary output file) to the QSPI memory at the address 0x90000000
        - if BINARY_AREA is USE_SDCARD then copy the project.bin to a micro-SD to be plugged on CN13 and reset the board.
        - In order to debug the  ExtMem_Application\FreeRTOS, user shall attach the debugger, and perform a reset


 */
