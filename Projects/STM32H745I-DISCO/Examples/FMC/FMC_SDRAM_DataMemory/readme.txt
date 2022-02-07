﻿/**
  @page FMC_SDRAM_DataMemory SDRAM memory functionalities example

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    FMC/FMC_SDRAM_DataMemory/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FMC SDRAM example.
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

This example describes how to configure the FMC controller to access the SDRAM
memory including heap and stack.

The SDRAM is IS42S32800G.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain,
the access will not be safe until “clock ready flag” of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

The example scenario does not reflect a real application case, its purpose is to
provide only the procedure to follow to use the external SDRAM as data memory.

This example does not use the default library startup file. It uses a modified
startup file provided with the example. The user has to add the new startup
file in the project source list. While startup, the SDRAM memory is configured
and initialized to be ready to contain data.

The user has to configure his preferred toolchain using the provided linker file.
The RAM zone is modified in order to use the external SDRAM memory as a RAM.

The external SDRAM initialisation is managed by the system_stm32h7xx.c file using this function : SystemInit_ExtMemCtl().

At this stage, all the used data can be located in the external SDRAM memory.

The user can use the debugger's watch to evaluate "uwTabAddr" and "MSPValue" variables
values which should be above 0xD0000000.

If uwTabAddr and MSPValue values are in the external SDRAM memory, LED1 is ON, otherwise the LED2 is ON.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FMC, SDRAM, Read, Write, Initialization, Access, Data Memory, Heap, Stack,

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

@par Directory contents

  - FMC/FMC_SDRAM_DataMemory/CM7//Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SDRAM_DataMemory/CM7//Inc/main.h                  Header for main.c module
  - FMC/FMC_SDRAM_DataMemory/CM7//Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FMC/FMC_SDRAM_DataMemory/CM7//Src/main.c                  Main program
  - FMC/FMC_SDRAM_DataMemory/CM7//Src/stm32h7xx_it.c          Interrupt handlers
  - FMC/FMC_SDRAM_DataMemory/CM7//Src/system_stm32h7xx.c      STM32H7xx system source file

  - FMC/FMC_SDRAM_DataMemory/CM4//Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SDRAM_DataMemory/CM4//Inc/main.h                  Header for main.c module
  - FMC/FMC_SDRAM_DataMemory/CM4//Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FMC/FMC_SDRAM_DataMemory/CM4//Src/main.c                  Main program
  - FMC/FMC_SDRAM_DataMemory/CM4//Src/stm32h7xx_it.c          Interrupt handlers
  - FMC/FMC_SDRAM_DataMemory/CM4//Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H745xx devices.

  - This example has been tested with STMicroelectronics STM32H745I-Discovery 
    and can be easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H745I_DISCO_CM7 and STM32H745I_DISCO_CM4) :
     - Rebuild all files
     - Load images into target memory
 - Run the example


 */
