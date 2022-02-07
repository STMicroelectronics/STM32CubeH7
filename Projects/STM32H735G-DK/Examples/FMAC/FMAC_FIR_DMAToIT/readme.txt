/**
  @page FMAC_FIR_DMAToIT FMAC FIR filter from DMA mode to IT mode example

  @verbatim
  ******************************************************************************
  * @file    FMAC/FMAC_FIR_DMAToIT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of FMAC_FIR_DMAToIT example.
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

How to use the FMAC peripheral to perform a FIR filter from DMA mode to IT mode.

The FMAC peripheral is configured to perform a FIR filter with
5 feed-forward taps and a gain equal to 1. The input and output
thresholds are set to 1. The clipping feature can be enabled or disabled thanks
to the compilation switch CLIP_ENABLED in main.h.

The FMAC peripheral is configured so as to use the best of its 256 x 16-bit
internal memory. This local memory is divided into three buffers:
 -   0 to   4: coefficient buffer;
 -   5 to 131: input buffer;
 - 132 to 254: output buffer;
 -        255: unused.

DMA mode is used to transfer input data from memory to the FMAC peripheral.
IT mode is used to transfer output data from FMAC peripheral to memory.

The coefficient buffer contains the vector of feed-forward taps, named B
(size comprised in [2:127]; here, size N = 5 elements).
This buffer is initialized during the configuration step in DMA mode.

The size of the input buffer is the sum of:
 - the size of the coefficient vector B (N = 5 elements);
 - the size of the additional space D1 needed for throughput improvement
   (minimum: 0; here, D1 = 122 elements).
The input buffer is filled during the preload step in DMA mode;
new values are added in DMA mode once the preloaded ones have been consumed.

The size of the output buffer is the one of the additional space D2 needed for a
better throughput (minimum: 1; here, D2 = 123 elements).
The output buffer is read in IT mode when the result is available.

The input data will be loaded in four times to demonstrate the preload and the writing in DMA mode:
 - 100 elements written during the preload (the input buffer isn't entirely filled);
 - 120 elements provided after starting the filter;
 - 110 elements provided when requested through callback (when the previous data has been used);
 - 182 elements provided when requested through callback (when the previous data has been used).

The output data will be read in two times through interrupts:
 - buffer of 256 elements provided when starting the filter;
 - buffer of 249 elements provided through callback when the previous buffer has been filled.

With 512 input elements, the FMAC peripheral will be able to compute 508 output elements.
These output elements are stored in aCalculatedFilteredData1[] and aCalculatedFilteredData2[].
This result is compared to the expected one contained in aRefFilteredData[] in order to detect
a possible error.

STM32 board LED is used to monitor the example status:
  - LED1 is ON when correct FMAC FIR results have been calculated.
  - LED2 is blinking (1 second period) if an error is detected
    in the FIR filter results or if there is an initialization or configuration error.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Keywords

FMAC, FIR, filter, DMA, Interrupt

@par Directory contents

  - FMAC/FMAC_FIR_DMAToIT/Src/main.c                   Main program
  - FMAC/FMAC_FIR_DMAToIT/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - FMAC/FMAC_FIR_DMAToIT/Src/stm32h7xx_it.c           Interrupt handlers
  - FMAC/FMAC_FIR_DMAToIT/Src/stm32h7xx_hal_msp.c      HAL MSP module
  - FMAC/FMAC_FIR_DMAToIT/Inc/main.h                   Main program header file
  - FMAC/FMAC_FIR_DMAToIT/Inc/stm32h7xx_hal_conf.h     HAL Configuration file
  - FMAC/FMAC_FIR_DMAToIT/Inc/stm32h7xx_it.h           Interrupt handlers header file
  - FMAC/FMAC_FIR_DMAToIT/Inc/stm32h735g_discovery_conf.h  BSP configuration file


@par Hardware and Software environment

  - This example runs on STM32H735xx devices.

  - This example has been tested with STM32H735G-DK board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
