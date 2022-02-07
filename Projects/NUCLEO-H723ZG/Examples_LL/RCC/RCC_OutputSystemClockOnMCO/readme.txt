/**
  @page RCC_OutputSystemClockOnMCO RCC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/RCC/RCC_OutputSystemClockOnMCO/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC example.
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

Configuration of MCO pins (PA8 and PC9) to output the system clock.

At start-up, User push-button and MCO pin are configured. The program configures SYSCLK
to the max frequency using the PLL with HSE as clock source.

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723XX Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

The signal on PA8 and PC9 can be monitored with an oscilloscope
to check the different MCO configuration set at each User push-button press.
Different configuration will be observed
 - HSI frequency with frequency value around @64MHz.
 - PLLCLK frequency value divided by 10, hence around @52MHz.
 - SYSCLK frequency value divided by 7, hence around @74MHz.
 - HSE frequency value divided by 2, hence around @4MHz.

When user press User push-button, a LED1 toggle is done to indicate a change in MCO config.

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

System, RCC, PLL, HSI, PLLCLK, SYSCLK, HSE, Clock, Oscillator

@par Directory contents 

  - RCC/RCC_OutputSystemClockOnMCO/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - RCC/RCC_OutputSystemClockOnMCO/Inc/main.h                  Header for main.c module
  - RCC/RCC_OutputSystemClockOnMCO/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_OutputSystemClockOnMCO/Src/stm32h7xx_it.c          Interrupt handlers
  - RCC/RCC_OutputSystemClockOnMCO/Src/main.c                  Main program
  - RCC/RCC_OutputSystemClockOnMCO/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723XX devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H723ZG Set-up
    - Connect the MCO pin to an oscilloscope to monitor the different waveforms:
      - PA.08: connected to pin 24 of CN12 connector for Nucleo-144  (MB1364)
      - PC.09: connected to pin 1 of CN12 connector for Nucleo-144 (MB1364)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
