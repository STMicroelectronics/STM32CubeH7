/**
  @page LedToggling Led Toggling Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    LedToggling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPIO IO Toggle example.
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

How to configure and use GPIOs through the HAL API.

This example provide different configuration with linker files which allows different eXecution schemas
Supported configuration by STM32H7B3I-DISCO:
  - XiP From OSPI, Data on Internal SRAM
  - XiP From OSPI, Data on External SDRAM
  - BootROM : Execution From External SDRAM , Data on Internal SRAM
  
PG2 IO (configured in output pushpull mode) toggles in a forever loop.
On STM32H7B3I-DISCO board this IO is connected to LED2.

The SystemClock_Config() function is used to configure the system clock for To be defined Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly�defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

External Memory, code execution, Loader, Boot, XiP, BootROM, Value line, GPIO, LED, Toggle

@par Directory contents

  - GPIO/GPIO_IOToggle/Inc/main.h                       Header for main.c module  
  - GPIO/GPIO_IOToggle/Inc/stm32h7b3i_discovery_conf.h  STM32H7B3I-DISCO board configuration file
  - GPIO/GPIO_IOToggle/Inc/stm32h7xx_hal_conf.h         HAL configuration file
  - GPIO/GPIO_IOToggle/Inc/stm32h7xx_it.h               Interrupt handlers header file

  - GPIO/GPIO_IOToggle/Src/main.c                       Main program
  - GPIO/GPIO_IOToggle/Src/stm32h7xx_it.c               Interrupt handlers
  - GPIO/GPIO_IOToggle/Src/system_stm32h7xx.c           STM32H7xx system source file
  


@par Hardware and Software environment

  - This example runs on STM32H7B0xxQ devices.
    
  - This example has been tested with STM32H7B3I-DISCO board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc template.
     The default configuration is: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_OSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this example (see below).
  4. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load this example to the external memory:
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
        - If BINARY_AREA is USE_SDMMC then Copy-Paste the Project.bin (application binary output file) to the �SD card.
        - In order to debug the  external memory application, user shall attach the debugger, and perform a reset



 */
