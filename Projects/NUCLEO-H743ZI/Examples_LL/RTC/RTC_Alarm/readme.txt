/**
  @page RTC_Alarm RTC example

  @verbatim
  ******************** (C) COPYRIGHT 2018 STMicroelectronics *******************
  * @file    Examples_LL/RTC/RTC_Alarm/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RTC example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to configure and generate Alarm using the RTC peripheral.

The peripheral initialization is done using LL unitary service functions for
optimization purpose (performance and size).

In this example, after start-up, the SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
- The CPU at 400MHz
- The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
- The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

The RTC peripheral configuration is ensured by the Configure_RTC() function
(configure of the needed RTC resources according to the used hardware CLOCK,
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

@note LSE oscillator clock is used as RTC clock source by default.
      The user can use also LSI as RTC clock source. The user uncomment the adequate
      line on the main.c file.
      @code
        #define RTC_CLOCK_SOURCE_LSE
        /* #define RTC_CLOCK_SOURCE_LSI */
      @endcode
      LSI oscillator clock is delivered by a 32 kHz RC.
      LSE (when available on board) is delivered by a 32.768 kHz crystal.

Configure_RTC_Alarm function is then called to initialize the
time, date and alarm.

In this example, the Time is set to 11:59:55 PM and the Alarm must be generated after
30 seconds on 12:00:25 AM.

The current time and date are updated and displayed on the debugger in aShowTime
and aShowDate variables (watch or live watch).

- LED2 is turned ON when the RTC Alarm is generated correctly.
- LED3 is toggling : This indicates that the system generates an error.

@par Directory contents

  - RTC/RTC_Alarm/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - RTC/RTC_Alarm/Inc/main.h                  Header for main.c module
  - RTC/RTC_Alarm/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_Alarm/Src/stm32h7xx_it.c          Interrupt handlers
  - RTC/RTC_Alarm/Src/main.c                  Main program
  - RTC/RTC_Alarm/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.

  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
