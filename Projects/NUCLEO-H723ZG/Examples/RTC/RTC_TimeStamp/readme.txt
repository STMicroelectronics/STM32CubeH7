/**
  @page RTC_TimeStamp RTC time stamp Example

  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_TimeStamp/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RTC time stamp example.
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
Configuration of the RTC HAL API to demonstrate the timestamp feature.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit()function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLOCK,
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

@note LSI oscillator clock is used as RTC clock source by default.
      The user can use also LSE as RTC clock source. The user uncomment the adequate
      line on the main.h file.
      @code
        #define RTC_CLOCK_SOURCE_LSI
        /* #define RTC_CLOCK_SOURCE_LSE */
      @endcode
      LSI oscillator clock is delivered by a 32 kHz RC.
      LSE (when available on board) is delivered by a 32.768 kHz crystal.

HAL_RTCEx_SetTimeStamp_IT()function is then called to initialize the time stamp feature
with interrupt mode. It configures the time stamp pin to be rising edge and enables
the time stamp detection on time stamp pin.
HAL_RTC_SetTime()and HAL_RTC_SetDate() functions are then called to initialize the
time and the date.

The associated firmware performs the following:
1. After startup the program configure the RTC (Time date) and enable the feature
   timeStamp.

2. When applying a high level on the time stamp pin (PC.13) by pressing the User push-button,
   a time stamp event is detected and the calendar is saved in the time stamp structures.
   Each time the user presses on the User push-button, the current time and time stamp are updated and displayed
   on the debugger in aShowTime and aShowTimeStamp variables.

- LED3 is toggling : This indicates that the system generates an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

RTC, Timer, Timestamp, Counter, LSE, LSI, Current time, Real Time Clock

@par Directory contents

  - RTC/RTC_TimeStamp/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - RTC/RTC_TimeStamp/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - RTC/RTC_TimeStamp/Inc/main.h                  Header for main.c module
  - RTC/RTC_TimeStamp/Inc/stm32h7xx_nucleo_conf.h STM32H7xx board configuration file
  - RTC/RTC_TimeStamp/Src/stm32h7xx_it.c          Interrupt handlers
  - RTC/RTC_TimeStamp/Src/main.c                  Main program
  - RTC/RTC_TimeStamp/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - RTC/RTC_TimeStamp/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
  - This example has been tested with STMicroelectronics NUCLEO-H723ZG
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 */
