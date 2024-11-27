/**
  @page Templates  Description of the Templates BootCM7_CM4Gated_RAM example

  @verbatim
  ******************************************************************************
  * @file    Templates/BootCM7_CM4Gated_RAM/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Templates example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description


This project provides a reference template that can be used to build any firmware application
where both cores are used. It is mainly dedicated for parts where CPU1 (Cortex-M7 in D1 Domain) is booting
and CPU2(Cortex-M4 in D2 Domain) is gated (with respect to configured boot Flash options).
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1(Cortex-M7) seen as the master CPU.
Once done , CPU1(Cortex-M7) copy the CPU2 code to the D2 domain RAM an configure its boot address(using HAL_SYSCFG_CM4BootAddConfig).
Finally, CPU2 boot will be released through hold boot function (an RCC feature)


This projects is configured for STM32H745xx devices using STM32CubeH7 HAL and running on
NUCLEO-H745ZI-Q board from STMicroelectronics.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
    and D3 Domain APB4 peripherals to run at 100MHz.


@note For Cortex-M7, Some code parts (for instance ISR routines, vector table, critical routines )
      can be executed from the ITCM-RAM (64 KB) allowing zero wait state access.
      Which decreases critical task execution time compared to code execution from internal Flash memory.
      This configuration can be done using '#pragma location = ".itcmram"' to be placed above function declaration,
      or using the toolchain GUI (file options) to execute a whole source file in the ITCM-RAM.
      For fast data access, critical variables, application RAM, heap, stack could be put as well in the DTCM-RAM (128 KB).

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, HAL template, multiprocessor, Clock Configuration, HSE bypass mode,System clock, Oscillator, PLL

@par Directory contents

  - Templates/BootCM7_CM4Gated_RAM/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration file

  - Templates/BootCM7_CM4Gated_RAM/CM7/Src/main.c                 Main program for Cortex-M7
  - Templates/BootCM7_CM4Gated_RAM/CM7/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7
  - Templates/BootCM7_CM4Gated_RAM/CM7/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M7
  - Templates/BootCM7_CM4Gated_RAM/CM7/Inc/main.h                 Main program header file for Cortex-M7
  - Templates/BootCM7_CM4Gated_RAM/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - Templates/BootCM7_CM4Gated_RAM/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7

  - Templates/BootCM7_CM4Gated_RAM/CM4/Src/main.c                 Main program for Cortex-M4
  - Templates/BootCM7_CM4Gated_RAM/CM4/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M4
  - Templates/BootCM7_CM4Gated_RAM/CM4/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M4
  - Templates/BootCM7_CM4Gated_RAM/CM4/Inc/main.h                 Main program header file for Cortex-M4
  - Templates/BootCM7_CM4Gated_RAM/CM4/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M4
  - Templates/BootCM7_CM4Gated_RAM/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H745xx/STM32H755xx devices.

  - This example has been tested with STMicroelectronics NUCLEO-H745ZI-Q
    boards and can be easily tailored to any other supported device
    and development board.
  - User Option Bytes requirement (with STM32CubeProgrammer tool)
     BOOT_CM4(BCM4) = 0                 Disable cortex M4 Boot
     BOOT_CM7(BCM7) = 1                 Enable cortex M7 Boot

@par How to use it ?

In order to make the program work, you must do the following :
 - Use STM32CubeProgrammer tool to disable the CM4 boot at FLASH Option bytes (CM4 is gated)
 - Open your preferred toolchain
 - For each target configuration (STM32H745I_Nucleo_CM4 first then STM32H745I_Nucleo_CM7) :
     - Rebuild all files

- For STM32H745I_Nucleo_CM7 target configuration:
     - Load CM7 image into target memory (CM4 image is included in a section of the CM7 output hex file)

- Run the STM32H745I_Nucleo_CM7 target configuration :
    The CM7 will copy the CM4 code from the flash to the target execution RAM then release it.




 */
