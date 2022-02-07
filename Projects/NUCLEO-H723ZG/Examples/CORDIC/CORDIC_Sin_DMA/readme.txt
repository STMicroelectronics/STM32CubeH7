/**
  @page CORDIC_Sin_DMA CORDIC Sines calculation in DMA mode example

  @verbatim
  ******************************************************************************
  * @file    CORDIC/CORDIC_Sin_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Sines calculation in DMA mode example.
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

How to use the CORDIC peripheral to calculate array of sines in DMA mode.

In this example, the CORDIC peripheral is configured in sine function, q1.31
format for both input and output data, and with 6 cycles of precision.
The input data provided to CORDIC peripheral are angles in radians
divided by PI, in q1.31 format. The output data are sines in q1.31 format.
For 6 cycles of precision, the maximal expected residual error of the
calculated sines is 2^-19.

DMA is used to transfer input data from memory to the CORDIC peripheral and
output data from CORDIC peripheral to memory, so that CPU is offloaded.

The calculated sines are stored in aCalculatedSin[] array.
The residual error of calculation results is verified, by comparing to
reference values in aRefSin[] obtained from double precision floating point
calculation.

STM32 board LED is used to monitor the example status:
  - LED1 is ON when correct CORDIC sine results are calculated.
  - LED2 is blinking (1 second period) when exceeding residual error
    on CORDIC sine results is detected or when there is an initialization error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Mathematic, CORDIC, sine, cosine, DMA, math, PI, radians, Trigonometric

@par Directory contents

  - FMAC/FMAC_FIR_DMAToIT/Inc/stm32h7xx_nucleo_conf.h  STM32H7xx_Nucleo board configuration file
  - CORDIC/CORDIC_Sinus_DMA/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CORDIC/CORDIC_Sinus_DMA/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CORDIC/CORDIC_Sinus_DMA/Inc/main.h                  Header for main.c module
  - CORDIC/CORDIC_Sinus_DMA/Src/stm32h7xx_it.c          Interrupt handlers
  - CORDIC/CORDIC_Sinus_DMA/Src/main.c                  Main program
  - CORDIC/CORDIC_Sinus_DMA/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - CORDIC/CORDIC_Sinus_DMA/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.

  - This example has been tested with STMicroelectronics NUCLEO-H723ZG
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
