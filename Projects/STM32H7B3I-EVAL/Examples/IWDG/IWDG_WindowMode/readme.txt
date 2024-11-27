/**
  @page IWDG_WindowMode IWDG Reset with window mode

  @verbatim
  ******************************************************************************
  * @file    IWDG/IWDG_WindowMode/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the IWDG reset with window mode.
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

How to periodically update the IWDG reload counter and simulate a software fault
that generates a MCU IWDG reset when a programmed time period has elapsed.

This projects is configured for STM32H7B3xxQ devices using STM32CubeH7 HAL and running on
STM32H7B3I-EVAL board from STMicroelectronics.

At the beginning of the main program:
- The HAL_Init() function is called to:
  - Reset all the peripherals
  - Initialize the Flash interface and the systick.
- The SystemClock_Config() function is used to configure the system clock:
  - The CPU at 280MHz
  - The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280MHz.
  - The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals at 125MHz.

The IWDG time-out is set to 762 ms (the time-out may vary due to LSI frequency
dispersion).

The Window option is enabled with a window register value set to 400 ms.
To prevent a reset, the down-counter must be reloaded when its value is:
 -lower than the window register value (400ms)
 -greater than 0x000 (0 ms)
The IWDG counter is therefore refreshed each 450 ms in the main program infinite loop to
prevent a IWDG reset (762 ms - 450 ms = 312 ms within the interval).
LED1 is also toggled each 450 ms indicating that the program is running.
LED3 will turn on if any error occurs.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

Once the EXTI Line event occurs by pressing the Tamper push-button (PC.13),
the refresh period is set to 200 ms.
This will make refresh being outside window value. As a result, when the IWDG counter is reloaded,
the IWDG reset occurs.
If the IWDG reset is generated, after the system resumes from reset, LED1 turns on for 4 seconds.
Then, the program execution continues normally as described above with the IWDG being refreshed
periodically within the interval.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@note It is recommended to enable the cache and maintain its coherence.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

System, IWDG, Timeout, Reload Counter, MCU Reset, Downcounter, LSI, Timer, Measure Frequency, Window

@par Directory contents

  - IWDG/IWDG_WindowMode/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - IWDG/IWDG_WindowMode/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I_EVAL board configuration file
  - IWDG/IWDG_WindowMode/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_WindowMode/Inc/main.h                  Header for main.c module

  - IWDG/IWDG_WindowMode/Src/stm32h7xx_it.c          Interrupt handlers
  - IWDG/IWDG_WindowMode/Src/main.c                  Main program
  - IWDG/IWDG_WindowMode/Src/system_stm32h7xx.c      stm32h7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL 
    board and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example




 */
