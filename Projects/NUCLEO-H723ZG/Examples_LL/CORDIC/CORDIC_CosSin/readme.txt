/**
  @page CORDIC_CosSin CORDIC Cosine and Sine calculation example

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/CORDIC/CORDIC_CosSin/readme.txt
  * @author  MCD Application Team
  * @brief   Description of CORDIC_CosSin example.
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

How to use the CORDIC peripheral to calculate cosine and sine.

This example is based on the STM32H7xx CORDIC LL API.
The peripheral initialization and control is done using LL unitary service
functions for optimization purpose (performance and size).

In this example, the CORDIC peripheral is configured in cosine function, q1.31
format for both input and output data, and with 6 cycles of precision.
The input data provided to CORDIC processor are angles in radians
divided by PI, in q1.31 format. The output data are cosine and sine in q1.31
format. For 6 cycles of precision, the maximal expected residual error of the
calculated sines is 2^-19.

The calculated cosine and sine are stored in cosOutput and sinOutput variables.
The residual error of calculation results is verified, by comparing to
reference values.

STM32 board LED1 is used to monitor the example status:
  - LED1 is ON when correct CORDIC cosine and sine results are calculated.
  - LED1 is blinking (2 second period) when exceeding residual error
    on CORDIC results is detected or when there is an initialization error.


Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Mathematic, CORDIC, sine, cosine, DMA, math, PI, radians, Trigonometric

@par Directory contents

  - CORDIC\CORDIC_CosSin/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CORDIC\CORDIC_CosSin/Inc/main.h                  Header for main.c module
  - CORDIC\CORDIC_CosSin/Inc/stm32_assert.h          Template file to include assert_failed function
  - CORDIC\CORDIC_CosSin/Src/stm32h7xx_it.c          Interrupt handlers
  - CORDIC\CORDIC_CosSin/Src/main.c                  Main program
  - CORDIC\CORDIC_CosSin/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
