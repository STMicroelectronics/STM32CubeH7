/**
  @page COMP_CompareGpioVsVrefInt_IT COMP example

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_IT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the COMP_CompareGpioVsVrefInt_IT example.
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

This example describes how to use a comparator peripheral to compare a voltage
level applied on a GPIO pin with the internal voltage reference (VREFINT), in interrupt
mode. This example is based on the STM32H7xx COMP LL API. The peripheral initialization
is done using LL unitary service functions for optimization purpose (performance and size).

Example configuration:
Comparator instance COMP1 is configured to compare:
 - input plus set to a GPIO pin (cf pin below) in analog mode.
 - input minus set to internal voltage reference VrefInt (voltage level 1.2V,
   refer to device datasheet for min/typ/max values)
Comparator interruption is enabled through EXTI line 20 with trigger edge
set to both edges rising and falling.
Comparator is set in power mode ultra low power.

Example execution:
From the main program execution, comparator is enabled.
Each time the voltage level applied on GPIO pin (comparator input plus)
is crossing VrefInt voltage threshold (comparator input minus),
the comparator generates an interruption.
LED 2 is turned on or off depending on comparator output state:
turned on if comparator voltage level on input plus is higher than input minus,
turned-off otherwise.

Connection needed:
A voltage must be supplied to the analog input pin (cf pin below),
between Vdda=3.3V and 0V. Otherwise this pin can be let floating (but in this
case COMP comparison level will be undetermined).

To check comparator threshold, you can:
 - Either use a wire to connect the GPIO pin of comparator input (cf pin below)
   to other connectors on board:
    - below comparator threshold: to ground
    - above comparator threshold: to voltage of power supply Vdda
      (on board Nucleo: Vdda=3.3V, available on CN6 pin 4 or CN7 pin 5)
 - Either use an external power supply

Other peripheral used:
  1 GPIO for LED 2
  1 GPIO for analog input of comparator: PB.02
  EXTI line 20 (connected to COMP1 output)

@par Keywords

Analog, Comparator, Stop mode, Voltage compare, Voltage level, Vref, Interrupt.

@par Directory contents

  - COMP/COMP_CompareGpioVsVrefInt_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_IT/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_IT/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_IT/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.

  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
