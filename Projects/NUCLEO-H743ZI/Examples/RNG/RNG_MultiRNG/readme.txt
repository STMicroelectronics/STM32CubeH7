/**
  @page RNG_MultiRNG Multiple Random Numbers Generator example
  
  @verbatim
  ******************************************************************************
  * @file    RNG/RNG_MultiRNG/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of multiple random numbers generation example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The RNG peripheral configuration is ensured by the HAL_RNG_Init() function.
The latter is calling the HAL_RNG_MspInit() function which implements
the configuration of the needed RNG resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change RNG configuration.

After startup, user is asked to press User push-button.
The 8-entry array aRandom32bit[] is filled up by 32-bit long random numbers 
at each key press.

 
The random numbers can be displayed on the debugger in aRandom32bit variable.

In case of error, LED2 is toggling at a frequency of 1Hz.


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

RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period

@par Directory contents 

  - RNG/RNG_MultiRNG/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - RNG/RNG_MultiRNG/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - RNG/RNG_MultiRNG/Inc/main.h                  Header for main.c module
  - RNG/RNG_MultiRNG/Src/stm32h7xx_it.c          Interrupt handlers
  - RNG/RNG_MultiRNG/Src/main.c                  Main program
  - RNG/RNG_MultiRNG/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - RNG/RNG_MultiRNG/Src/system_stm32h7xx.c      STM32H7xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
  
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */
 
