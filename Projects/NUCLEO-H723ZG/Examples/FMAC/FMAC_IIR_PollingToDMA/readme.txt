/**
  @page FMAC_IIR_PollingToDMA FMAC IIR filter from polling mode to DMA mode example

  @verbatim
  ******************************************************************************
  * @file    FMAC/FMAC_IIR_PollingToDMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of FMAC_IIR_PollingToDMA example.
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

How to use the FMAC peripheral to perform an IIR filter from polling mode to DMA mode.

The FMAC peripheral is configured to perform an IIR filter with
7 feed-forward taps, 6 feedback taps and a gain equal to 1. The input and output
thresholds are set to 1. The clipping feature is disabled.

The FMAC peripheral is configured so as to use the best of its 256 x 16-bit
internal memory. This local memory is divided into three buffers:
 -   0 to  12: coefficient buffer;
 -  13 to 133: input buffer;
 - 134 to 254: output buffer;
 -        255: unused.

Polling mode is used to transfer input data from memory to the FMAC peripheral.
DMA mode is used to transfer output data from FMAC peripheral to memory, so that CPU is offloaded.

The coefficient buffer contains the concatenation of:
 - the vector of feed-forward taps, named B (here, size N = 7 elements);
 - the vector of feedback taps, named A (size comprised in [1; N-1]; here, size M = 6 elements).
This buffer is initialized during the configuration step in polling mode.

The size of the input buffer is the sum of:
 - the size of the coefficient vector B (N = 7 elements);
 - the size of the additional space D1 needed for throughput improvement
   (minimum: 0; here, D1 = 114 elements).
The input buffer is filled during the preload step in polling mode;
no new values are added once the preloaded ones have been consumed.

The size of the output buffer is the sum of:
 - the size of the coefficient A buffer (M = 6 elements);
 - the size of the additional space D2 needed for a better throughput
   (minimum: 1; here, D2 = 115 elements).
The output buffer is initialized during the preload step in polling mode
and read in DMA mode when the result is available.

With N+D1 input elements, the FMAC peripheral will be able to compute D1+1 output elements.
These output elements are stored in aCalculatedFilteredData[]. This result is compared to
the expected one contained in aRefFilteredData[] in order to detect a possible error.

STM32 board LED is used to monitor the example status:
  - LED1 is ON when correct FMAC IIR results have been calculated.
  - LED2 is blinking (1 second period) if an error is detected
    in the IIR filter results or if there is an initialization or configuration error.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Keywords

FMAC, FIR, filter, DMA, Polling

@par Directory contents

  - FMAC/FMAC_IIR_PollingToDMA/Src/main.c                   Main program
  - FMAC/FMAC_IIR_PollingToDMA/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - FMAC/FMAC_IIR_PollingToDMA/Src/stm32h7xx_it.c           Interrupt handlers
  - FMAC/FMAC_IIR_PollingToDMA/Src/stm32h7xx_hal_msp.c      HAL MSP module
  - FMAC/FMAC_IIR_PollingToDMA/Inc/main.h                   Main program header file
  - FMAC/FMAC_IIR_PollingToDMA/Inc/stm32h7xx_hal_conf.h     HAL Configuration file
  - FMAC/FMAC_IIR_PollingToDMA/Inc/stm32h7xx_it.h           Interrupt handlers header file
  - FMAC/FMAC_IIR_PollingToDMA/Inc/stm32h7xx_nucleo_conf.h  BSP configuration file


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
