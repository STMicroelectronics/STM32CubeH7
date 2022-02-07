/**
  @page Templates  Description of the Templates example

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Template example.
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

This projects provides a reference template that can be used to build any firmware application with execution from external memory

This projects is configured for STM32H750xx devices using STM32CubeH7 HAL and running on
STM32H750B-DISCO board from STMicroelectronics.

This application provide different configuration with linker files which allows different eXecution schemas
  Supported configuration by STM32H750B-DISCO:
    - XiP From QSPI, Data on Internal SRAM
    - XiP From QSPI, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM

  Note : In BootROM configuration , application code is initially stored on SPI nor or eMMC flash then loaded to the SDRAM after boot
  Make sure that choosen config matches ExtMem_Boot config in memory.h file.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
The CPU at 400MHz
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

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

Template, External, Memory, Boot, Loader, XiP, BootROM, Value line

@par Directory contents

 - "Templates/Inc": contains the Templates firmware header files
    - Templates/Inc/main.h                          Main configuration file
    - Templates/Inc/stm32h7xx_it.h                  Interrupt handlers header file
    - Templates/Inc/stm32h7xx_hal_conf.h            HAL Configuration file
	- Templates/Inc/stm32h750b_discovery_conf.h     Board Configuration file
	- Templates/Inc/mt25tl01g_conf.h                MT25TL01G (QSPI) Configuration file
	- Templates/Inc/mt48lc4m32b2_conf.h             MT48LC4M32B2 (SDRAM) Configuration file
    - Templates/Inc/ft5336_conf.h                   FT5336 Configuration file
 - "Templates/Src": contains the Templates firmware source files
    - Templates/Src/main.c                   Main program
    - Templates/Src/stm32h7xx_hal_msp.c      Microcontroller specific packages initialization file.
    - Templates/Src/stm32h7xx_it.c           Interrupt handlers
    - Templates/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32H750xx devices.

  - This example has been tested with STMicroelectronics STM32H750B-DISCO
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc template.
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


 */

