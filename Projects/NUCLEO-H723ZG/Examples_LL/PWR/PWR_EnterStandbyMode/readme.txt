/**
  @page PWR_EnterStandbyMode PWR standby example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY mode example.
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

How to enter the Standby mode and wake up from this mode by using an external 
reset or a wakeup interrupt.

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

In the associated software, the system clock is set to 520 MHz, an EXTI line
is connected to the User push-button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.
LED1 is toggled each 200 ms in order to indicate that MCU is in RUN mode.

When a falling edge is detected on the EXTI line, an interrupt is generated. 
In the EXTI handler routine, the wake-up pin LL_PWR_WAKEUP_PIN4 is enabled and the 
corresponding wake-up flag cleared. Then, the system enters Standby mode causing 
LED1 to stop toggling. 

Next, the user can wake-up the system in pressing the User push-button which is 
connected to the wake-up pin LL_PWR_WAKEUP_PIN4.
A falling edge on WKUP pin will wake-up the system from Standby.

Alternatively, an external RESET of the board will lead to a system wake-up as well.

After wake-up from Standby mode, program execution restarts in the same way as 
after a RESET and LED1 restarts toggling.

LED1 is used to monitor the system state as follows:
 - LED1 fast toggling: system in RUN mode
 - LED1 slow toggling: system in RUN mode after exiting from Standby mode
 - LED1 off : system in Standby mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in Standby mode, remove JP4 jumper 
      and connect an ampere meter to JP4 to measure IDD current.

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M7 core is no longer clocked during low power mode 
      so debugging features are disabled.
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

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset

@par Directory contents 

  - PWR/PWR_EnterStandbyMode/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStandbyMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStandbyMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStandbyMode/Src/stm32h7xx_it.c          Interrupt handlers
  - PWR/PWR_EnterStandbyMode/Src/main.c                  Main program
  - PWR/PWR_EnterStandbyMode/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-H723ZG
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-H723ZG Set-up
    - LED1 connected to PB.00 pin
    - User push-button connected to pin PC.13 (External line 15 to 10)
    - WakeUp Pin LL_PWR_WAKEUP_PIN4 connected to PC.13

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
