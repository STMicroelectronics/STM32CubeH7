/**
  @page FreeRTOS FreeRTOS application

  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Thread Creation application with execution from external memory
  *          AN5188.
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

How to implement thread creation using CMSIS RTOS API with execution from external memory.

This application creates two threads with the same priority, which execute in
a periodic cycle of 18 seconds.

In the first 6 seconds, the thread 1 toggles LED3 each 200 ms and the
thread 2 toggles LED2 each 500 ms.
In the following 6 seconds, the thread 1 suspends itself and the thread 2
continue toggling LED2.
In the last 6 seconds, the thread 2 resumes execution of thread 1 then
suspends itself, the thread 1 toggles the LED3 each 400 ms.

This application provide different configuration with linker files which allows different eXecution schemas
  Supported configuration by STM32H7B3I-DISCO:
    - XiP From OSPI, Data on Internal SRAM
    - XiP From OSPI, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM
  Make sure that choosen config matches ExtMem_Boot config in memory.h file.

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
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case, it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

External Memory, OSPI, code execution, Loader, Boot, XiP, BootROM, Value line, RTOS

@par Directory contents

 - "FreeRTOS/Inc": contains the FreeRTOS firmware header files
    - FreeRTOS/FreeRTOS_DelayUntil/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_DelayUntil/Inc/stm32h7xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_DelayUntil/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_DelayUntil/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file

 - "FreeRTOS/Src": contains the FreeRTOS firmware source files
    - FreeRTOS/FreeRTOS_DelayUntil/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_DelayUntil/Src/stm32h7xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32H7B0xxQ devices.

  - This application has been tested with STMicroelectronics STM32H7B0xxQ
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc template.
     The default configuration is:
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_OSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this application (see below).
  4. Start debugging this application or reset for free running.

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
        - If BINARY_AREA is USE_SDMMC then Copy-Paste the Project.bin (application binary output file) to the µSD card.
        - In order to debug the  external memory application, user shall attach the debugger, and perform a reset


 */
