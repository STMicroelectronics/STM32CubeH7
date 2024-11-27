/**
  @page RNG_MultiRNG example
  
  @verbatim
  ******************************************************************************
  * @file    RNG/RNG_MultiRNG/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of multiple random numbers generation example.
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

Configuration of the RNG using the HAL API. This example uses the RNG to generate 32-bit long random numbers.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.

CPU1 (Cortex-M7) : The RNG peripheral configuration is ensured by the HAL_RNG_Init() function.
The latter is calling the HAL_RNG_MspInit() function which implements
the configuration of the needed RNG resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change RNG configuration.

After startup, user is asked to press User push-button.
The 8-entry array aRandom32bit[] is filled up by 32-bit long random numbers 
at each User press.
 
The random numbers can be displayed on the debugger in aRandom32bit variable.

In case of error, LED3 is toggling at a frequency of 1Hz.


This projects is configured for STM32H747xx devices using STM32CubeH7 HAL and running on 
STM32H747I-EVAL board from STMicroelectronics.

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
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
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

RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period

@par Directory contents 

  - RNG/RNG_MultiRNG/Common/Src/system_stm32h7xx.c   STM32H7xx system configuration file
  
  - RNG/RNG_MultiRNG/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M7
  - RNG/RNG_MultiRNG/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M7
  - RNG/RNG_MultiRNG/CM7/Inc/main.h                  Header for main.c module for Cortex-M7
  - RNG/RNG_MultiRNG/CM7/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M7
  - RNG/RNG_MultiRNG/CM7/Src/main.c                  Main program for Cortex-M7
  - RNG/RNG_MultiRNG/CM7/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M7

  - RNG/RNG_MultiRNG/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - RNG/RNG_MultiRNG/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - RNG/RNG_MultiRNG/CM4/Inc/main.h                  Header for main.c module for Cortex-M4
  - RNG/RNG_MultiRNG/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - RNG/RNG_MultiRNG/CM4/Src/main.c                  Main program for Cortex-M4
     
@par Hardware and Software environment

  - This example runs on STM32H747xx devices.
  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747_EVAL_CM7 and STM32H747_EVAL_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example
 

 */
 
