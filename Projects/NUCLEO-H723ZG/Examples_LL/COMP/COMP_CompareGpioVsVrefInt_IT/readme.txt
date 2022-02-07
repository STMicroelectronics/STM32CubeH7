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

How to use a comparator peripheral to compare a voltage level applied on a GPIO
pin to the internal voltage reference (VREFINT), in interrupt mode. This example
is based on the STM32H7xx COMP LL API. The peripheral initialization
uses LL unitary service functions for optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

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
LED1 is turned on or off depending on comparator output state:
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
      (on board Nucleo: by default, Vdda connected to 3.3V)
 - Either use an external power supply

Other peripheral used:
  1 GPIO for LED1
  1 GPIO for analog input of comparator: PB.02 (Arduino connector CN10 pin D27, Morpho connector CN12 pin 22)
  EXTI line 20 (connected to COMP1 output)


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
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

Analog, comparator, stop mode, voltage compare, voltage level, Vref, Interrupt.

@par Directory contents 

  - COMP/COMP_CompareGpioVsVrefInt_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_IT/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_IT/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_IT/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
