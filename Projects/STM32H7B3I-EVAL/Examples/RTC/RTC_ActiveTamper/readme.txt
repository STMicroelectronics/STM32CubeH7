/**
  @page RTC_ActiveTamper RTC ActiveTamper example

  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_ActiveTamper/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RTC ACTIVE TAMPER example.
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

How to program active tampers.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

In the associated software, the system clock is set to 280 MHz the SysTick is programmed to
generate an interrupt each 1 ms.
The LSE clock is used as RTC clock source by default.

This example activate/deactivate Active Tampers and update the Seed at run time.
2 tampers Input share the same tamper Output.
Tamper interrupts and erase of backup registers are checked.

Please connect the following pins together :

  - TAMP_IN2  (PI08 - pin 8 in CN5)
  - TAMP_OUT2 (PC13 - pin 6 in CN5)
  - (Optional) Oscilloscope probe to visualize the signal

Execute the software, wait a little and disconnect a pin.
One of the below scenario should occur :
 - LED1 toggles for ever : an error occurs.
 - LED1 is ON : Tamper interrupt detected and backup registers are erased.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

System, RTC, Tamper, Active Tamper, Reset, LSE

@par Directory contents

  - RTC/RTC_ActiveTamper/Inc/stm32h7xx_hal_conf.h     HAL configuration file
  - RTC/RTC_ActiveTamper/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - RTC/RTC_ActiveTamper/Inc/main.h                         Header file for main.c
  - RTC/RTC_ActiveTamper/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - RTC/RTC_ActiveTamper/Src/stm32h7xx_it.c           Interrupt handlers
  - RTC/RTC_ActiveTamper/Src/main.c                         Main program
  - RTC/RTC_ActiveTamper/Src/stm32h7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices
  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL
    evaluation board and can be easily tailored to any other supported device
    and development board.

 @par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Connect all pins as required
  - Run the example
  - Disconnect a pin


 */
