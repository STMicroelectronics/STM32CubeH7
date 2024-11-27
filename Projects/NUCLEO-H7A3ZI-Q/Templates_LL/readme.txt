/**
  @page Templates_LL  Description of the Templates_LL example

  @verbatim
  ******************************************************************************
  * @file    Templates_LL/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Templates_LL example.
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

This projects provides a reference template through the LL API that can be used
to build any firmware application.

This project LL template provides:
 - Inclusion of all LL drivers (include files in "main.h" and LL sources files
   in IDE environment, with option "USE_FULL_LL_DRIVER" in IDE environment)
   Note: If optimization is needed afterwards, user can perform a cleanup by
   removing unused drivers.
 - Clock configuration (file: main.c)

To port a LL example to the targeted board:
1. Select the LL example to port.
   To find the board on which LL examples are deployed, refer to LL examples list
   in "STM32CubeProjectsList.html", table section "Examples_LL"
   or AN4731: STM32Cube firmware examples for STM32H7 Series

2. Replace source files of the LL template by the ones of the LL example, except
   code specific to board.
   Note: Code specific to board is specified between tags:
   /* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
   /* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

   - Replace file main.c, with updates:
     - Keep clock configuration of the LL template: function "SystemClock_Config()"

   - Replace file stm32h7xx_it.h
   - Replace file stm32h7xx_it.c

  This projects is configured for STM32H7A3xxQ devices using STM32CubeH7 HAL and
  running on the NUCLEO-H7A3ZI-Q board from STMicroelectronics.

  At the beginning of the main program the SystemClock_Config()function is called
  to configure the system clock for STM32H7A3xxQ Devices :
        - The CPU at 280MHz
        - The HCLK for CD Domain AXI, AHB3, AHB1 and AHB2 peripherals and SRD Domain
           AHB4 peripherals at 280MHz.
        - The APB clock dividers for CD Domain APB3, APB1 and APB2 peripherals,
          CD Domain peripherals and SRD Domain APB4 peripherals to run at 140MHz.

  The template includes also another function used to enable the CPU Cache
  "CPU_CACHE_Enable()". This functions is provided as template implementation that
  User may integrate in his application, to enhance the performance.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical
      task execution time, compared to code execution from Flash memory. This feature
      can be activated using '#pragma location = ".itcmram"' to be placed above
      function declaration, or using the toolchain GUI (file options) to execute
      a whole source file in the ITCM-RAM.

@note The application needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct operation.

@Note If the  application is using the DTCM/ITCM memories
      (@0x20000000/ 0x0000000: not cacheable and only accessible by the Cortex M7 and the MDMA),
      no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires
      more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence
                between CPU and other masters (DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between
                CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case, it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@Note User can choose where to locate the default RAM region as follows:

		- EWARM IDE:
			Go to Project options-->Linker--> Config--> Linker configuration file:
				- For DTCM RAM select: $PROJ_DIR$\stm32h7a3xxq_flash.icf
				- For CD-AXI SRAM select: $PROJ_DIR$\stm32h7a3xxq_flash_rw_sram1.icf

		- MDK-ARM IDE:
			Go to Project options --> Target:
				- For DTCM RAM check IRAM1 and uncheck IRAM2 
				- For CD-AXI SRAM check IRAM2 and uncheck IRAM1

		- Cube IDE:
			Go to Project properties: C/C++ Build-->Settings--> Tools settings --> MCU GCC Linker --> General:
				- For DTCM RAM select: ../STM32H7A3ZITXQ_FLASH.ld
				- For CD-AXI SRAM select: ../STM32H7A3ZITXQ_FLASH_SRAM1.ld

@par Keywords

System, LL template, Clock Configuration, HSE bypass mode,System clock, Oscillator, PLL

@par Directory contents

  - Templates_LL/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - Templates_LL/Inc/main.h                  Header for main.c module
  - Templates_LL/Inc/stm32_assert.h          Template file to include assert_failed function
  - Templates_LL/Src/stm32h7xx_it.c          Interrupt handlers
  - Templates_LL/Src/main.c                  Main program
  - Templates_LL/Src/system_stm32h7xx.c      STM32H7x system source file



@par Hardware and Software environment

  - This template runs on STM32H7A3xxQ devices.

  - This template has been tested with STMicroelectronics NUCLEO-H7A3ZI-Q
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example




 */
