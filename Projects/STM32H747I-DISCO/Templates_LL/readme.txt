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

It is mainly dedicated for devices  
where CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

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

   - Replace file stm32f7xx_it.h
   - Replace file stm32f7xx_it.c

  This projects is configured for STM32H747xx devices using STM32CubeH7 HAL and
  running on STM32H747I-DISCO board from STMicroelectronics.

  The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
    - The Cortex-M7 at 400MHz 
    - Cortex-M4 at 200MHz.
    - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
      and D3 Domain AHB4  peripherals at 200MHz.
    - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
      and D3 Domain APB4 peripherals to run at 100MHz.

  The template includes also another function used to enable the cortex-M7 CPU Cache
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
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence
                between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between
                CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

System, LL template, multiprocessor, Clock Configuration, HSE, System clock, Oscillator, PLL

@par Directory contents

  - Templates_LL/Common/Src/system_stm32h7xx.c   STM32H7x system source file
  - Templates_LL/Common/Inc/ll_common.h          common ll header file for cortex-M7 
  - Templates_LL/CM7/Inc/stm32_assert.h          Template file to include assert_failed function for cortex-M7 and cortex-M4  

  - Templates_LL/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file for cortex-M7
  - Templates_LL/CM7/Inc/main.h                  Header for main.c module for cortex-M7
  - Templates_LL/CM7/Src/stm32h7xx_it.c          Interrupt handlers for cortex-M7
  - Templates_LL/CM7/Src/main.c                  Main programfor cortex-M7

  - Templates_LL/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for cortex-M7
  - Templates_LL/CM4/Inc/main.h                  Header for main.c module for cortex-M7
  - Templates_LL/CM4/Src/stm32h7xx_it.c          Interrupt handlers for cortex-M7
  - Templates_LL/CM4/Src/main.c                  Main programfor cortex-M7
  

@par Hardware and Software environment

  - This template runs on STM32H747xx devices.

  - This template has been tested with STMicroelectronics STM32H747I-DISCO
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM4 first then STM32H747I_DISCO_CM7) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example



 */
