/**
  @page Templates  Description of the Templates example

  @verbatim
  ******************************************************************************
  * @file    Templates/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Templates example.
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

This projects provides a reference template that can be used to build any firmware application.

This projects is configured for STM32H723xx devices using STM32CubeH7 HAL and running on
NUCLEO-H723ZG board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz
The HCLK for D1 Domain AXI peripherals, D2 Domain AHB peripherals and D3 Domain AHB  peripherals at 260 MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 130 MHz.

The template includes also two functions used to configure MPU feature and to enable the CPU Cache,
respectively MPU_Config() and CPU_CACHE_Enable().
These functions are provided as template implementation that User may integrate in his application,
to enhance the performance in case of use of AXI interface with several masters.

NUCLEO-H723ZG LEDs toggle if the program is successfully

@note Some code parts can be executed in the ITCM-RAM (64 KB up to 256kB) which decreases critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

System, HAL template, Clock Configuration, HSE bypass mode,System clock, Oscillator, PLL

@par Directory contents

  - Templates/Src/main.c                       Main program
  - Templates/Src/system_stm32h7xx.c           STM32H7xx system clock configuration file
  - Templates/Src/stm32h7xx_it.c               Interrupt handlers
  - Templates/Src/stm32h7xx_hal_msp.c          HAL MSP module
  - Templates/Inc/main.h                       Main program header file
  - Templates/Inc/stm32h7xx_hal_conf.h         HAL Configuration file
  - Templates/Inc/stm32h7xx_nucleo_conf.h      BSP nucleo Configuration file
  - Templates/Inc/stm32h7xx_it.h               Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.

  - This example has been tested with STMicroelectronics NUCLEO-H723ZG
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example




 */
