/**
  @page IWDG_RefreshUntilUserEvent IWDG example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/IWDG/IWDG_RefreshUntilUserEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IWDG_RefreshUntilUserEvent example.
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

How to configure the IWDG peripheral to ensure periodical counter update and 
generate an MCU IWDG reset when a User push-button is pressed. The peripheral 
is initialized with LL unitary service functions to optimize 
for performance and size.

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H7A3XXQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

Example Configuration:
Configure the IWDG (prescaler, counter) and enable it.
Infinite refresh of the IWDG down-counter done in the main loop.
LED1 is blinking fast & continuously.

Example Execution:
When User push-button is pressed, the down-counter automatic refresh mechanism is 
disabled and thus, reset occurs. After a reset, when re-entering the main function, 
RCC IWDG Reset Flag is checked and if we are back from a IWDG reset the LED1
is switched ON.

Waiting a new User push-button pressed to re-activate the IWDG

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

System, IWDG, Timeout, Reload Counter, refresh, MCU Reset, Downcounter, LSI, Timer, Measure Frequency, window

@par Directory contents 

  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/main.h                  Header for main.c module
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32_assert.h          Template file to include assert_failed function
  - IWDG/IWDG_RefreshUntilUserEvent/Src/stm32h7xx_it.c          Interrupt handlers
  - IWDG/IWDG_RefreshUntilUserEvent/Src/main.c                  Main program
  - IWDG/IWDG_RefreshUntilUserEvent/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7A3XXQ devices.
    
  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
