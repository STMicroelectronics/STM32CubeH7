/**
  @page Templates  Description of the Template BootCM4_CM7 example

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    Templates/BootCM4_CM7/readme.txt
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


This project provides a reference template that can be used to build
any firmware application where both cores are used. It is mainly dedicated for devices
where CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be ensured by any Domain D2 wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain,
the access will not be safe until �clock ready flag� of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).


This projects is configured for STM32H747xx devices using STM32CubeH7 HAL and running on
STM32H747I-DISCO board from STMicroelectronics.

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
      This configuratuion can be done using '#pragma location = ".itcmram"' to be placed above function declaration,
      or using the toolchain GUI (file options) to execute a whole source file in the ITCM-RAM.
      For fast data access, critical variables, application RAM, heap, stack could be put as well in the DTCM-RAM (128 KB).

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly�defined to be aligned to L1-CACHE line size (32 bytes).
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, HAL template, multiprocessor, Clock Configuration, HSE, System clock, Oscillator, PLL

@par Directory contents

  - Templates/BootCM4_CM7/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration file

  - Templates/BootCM4_CM7/CM7/Src/main.c                 Main program for Cortex-M7
  - Templates/BootCM4_CM7/CM7/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7
  - Templates/BootCM4_CM7/CM7/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M7
  - Templates/BootCM4_CM7/CM7/Inc/main.h                 Main program header file for Cortex-M7
  - Templates/BootCM4_CM7/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - Templates/BootCM4_CM7/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7

  - Templates/BootCM4_CM7/CM4/Src/main.c                 Main program for Cortex-M4
  - Templates/BootCM4_CM7/CM4/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M4
  - Templates/BootCM4_CM7/CM4/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M4
  - Templates/BootCM4_CM7/CM4/Inc/main.h                 Main program header file for Cortex-M4
  - Templates/BootCM4_CM7/CM4/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M4
  - Templates/BootCM4_CM7/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H747xx devices.

  - This example has been tested with STMicroelectronics STM32H747I-DISCO
    boards and can be easily tailored to any other supported device
    and development board.
  - User Option Bytes requirement (with STM32CubeProgrammer tool)
     BOOT_CM4(BCM4) = 1                 Enable cortex M4 Boot
     BOOT_CM7(BCM7) = 1                 Enable cortex M7 Boot

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H747I_DISCO_CM4 then STM32H747I_DISCO_CM7) :
     - Rebuild all files
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example




 */
