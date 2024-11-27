/**
  @page RTC_Alarm RTC Alarm Example

  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_Alarm/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RTC Alarm example.
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

How to configure and generate an RTC alarm.

In this example, the code is executed from QSPI external memory while data is
in internalSRAM memory.

In this example, the Time is set to 16:30:00 and the Alarm must be generated after
30 seconds at 16:30:30.

Before the 30 seconds are elapsed, LED1 is toggled with a period of one second.
It is turned ON when the RTC Alarm is generated correctly.
The current time is updated and displayed on the debugger in aShowTime variable.

In case of error, LED2 is toggled with a period of one second.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clocks for STM32H750xx Devices:
 - The CPU clock at 400MHz
 - The HCLK for D1 Domain's AXI and AHB3 peripherals, D2 Domain's AHB1 and AHB2 peripherals, D3 Domain's AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1's Domain APB3 peripherals, D2 Domain's APB1 and APB2 peripherals and D3 Domain's APB4 peripherals to  run at 100MHz

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This latter is calling the HAL_RTC_MspInit()function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLOCK,
PWR, RTC clock source and Backup). You may update this function to change RTC configuration.

@note LSE oscillator clock is used as RTC clock source by default.
      The user can use also LSI as RTC clock source. The user uncomment the adequate
      line on the main.h file.
      @code
        #define RTC_CLOCK_SOURCE_LSE
        /* #define RTC_CLOCK_SOURCE_LSI */
      @endcode

      LSI oscillator clock must be calibrated first to deliver exactly a 32 kHz signal.
      LSE (when available on board) is delivered by a 32.768 kHz crystal.

This project is configured for STM32H750xx devices using STM32CubeH7 HAL and running on
STM32H750B-DISCO board from STMicroelectronics.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

System, RTC, Alarm, Wakeup timer, Backup domain, Counter, LSE, LSI

@par Directory contents

 - "RTC/RTC_Alarm/Inc": contains the example firmware header files
    - RTC/RTC_Alarm/Inc/main.h                   Main configuration file
    - RTC/RTC_Alarm/Inc/stm32h7xx_it.h           Interrupt handlers header file
    - RTC/RTC_Alarm/Inc/stm32h7xx_hal_conf.h     HAL Configuration file

 - "RTC/RTC_Alarm/Src": contains the example firmware source files
    - RTC/RTC_Alarm/Src/main.c                   Main program
    - RTC/RTC_Alarm/Src/stm32h7xx_hal_msp.c      Microcontroller specific packages initialization file.
    - RTC/RTC_Alarm/Src/stm32h7xx_it.c           Interrupt handlers
    - RTC/RTC_Alarm/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32H750xx devices.
  - This example has been tested with STMicroelectronics STM32H750B-DISCO
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one:
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this example(see below).
  4. Start debugging the example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load this example to the external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program :
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using STM32CubeIDE  :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" in case of XiP from QSPI
            - From Erasing & Programming menu, browse and open the output binary file relative to your example
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


 */
