/**
  @page EXTI_ToggleLedOnIT EXTI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/EXTI/EXTI_ToggleLedOnIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EXTI example.
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

How to configure the EXTI and use GPIOs to toggle the user LEDs 
available on the board when a user button is pressed. It is based on the
STM32H7xx LL API. The peripheral initialization uses LL unitary service
functions for optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

In this example, one EXTI line (EXTI15_10) is configured to generate
an interrupt on each falling edge.

In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:
    - EXTI15_10 is connected to PC.13 pin
      - when falling edge is detected on EXTI15_10 by pressing User push-button, LED1 toggles

On NUCLEO-H723ZG:
    - EXTI15_10 is connected to User push-button

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

System, GPIO, Output, EXTI, Button, LED, Toggle

@par Directory contents 

  - EXTI/EXTI_ToggleLedOnIT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - EXTI/EXTI_ToggleLedOnIT/Inc/main.h                  Header for main.c module
  - EXTI/EXTI_ToggleLedOnIT/Inc/stm32_assert.h          Template file to include assert_failed function
  - EXTI/EXTI_ToggleLedOnIT/Src/stm32h7xx_it.c          Interrupt handlers
  - EXTI/EXTI_ToggleLedOnIT/Src/main.c                  Main program
  - EXTI/EXTI_ToggleLedOnIT/Src/system_stm32h7xx.c      STM32H7xx system source file

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
