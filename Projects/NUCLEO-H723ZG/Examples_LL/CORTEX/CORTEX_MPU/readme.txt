/**
  @page CORTEX_MPU CORTEX example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/CORTEX/CORTEX_MPU/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CORTEX_MPU example.
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

Presentation of the MPU feature. This example configures a memory area as 
privileged read-only, and attempts to perform read and write operations in
different modes.

If the access is permitted LED1 is toggling. If the access is not permitted,
a memory management fault is generated and LED1 is ON.

To generate an MPU memory fault exception due to an access right error, press 
user button.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

System, Cortex, Memory regions, Privileged area, permitted access, memory management fault, MPU memory fault

@par Directory contents 

  - CORTEX/CORTEX_MPU/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CORTEX/CORTEX_MPU/Inc/main.h                  Header for main.c module
  - CORTEX/CORTEX_MPU/Inc/stm32_assert.h          Template file to include assert_failed function
  - CORTEX/CORTEX_MPU/Src/stm32h7xx_it.c          Interrupt handlers
  - CORTEX/CORTEX_MPU/Src/main.c                  Main program
  - CORTEX/CORTEX_MPU/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

@Note Using STM32CubeIDE debugger, when hard fault is generated user shall press the debugger "Run" button to continue the execution 


 */
