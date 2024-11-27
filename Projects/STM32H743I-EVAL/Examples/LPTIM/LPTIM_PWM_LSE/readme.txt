/**
  @page LPTIM_PWM_LSE LPTIM PWM Low Power example with LSE clock source

  @verbatim
  ******************************************************************************
  * @file    LPTIM/LPTIM_PWM_LSE/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LPTIM PWM LSE example
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example describes how to configure and use LPTIM to generate a PWM in low power mode
using the LSE as a counter clock, through the HAL LPTIM API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

In this example, the LPTIM instance used is LPTIM1 and the low power mode is Stop mode.


The counter clock is LSE (32.768 KHz), Autoreload equal to 99 so the output
frequency (FrequencyOutput) will be equal to 327.680.

  FrequencyOutput = Counter Clock Frequency / (Autoreload + 1)
                  = 32768 / 100
                  = 327.680 Hz

Pulse value equal to 49 and the duty cycle (DutyCycle) is computed as follow:

  DutyCycle = 1 - ((PulseValue + 1)/ (Autoreload + 1))
  DutyCycle = 50%

To minimize the power consumption, after starting generating the PWM signal,
the MCU enters in Stop mode. Note that GPIOs are configured in Low Speed to
enhance the consumption.

Tamper push-button pin (PC.13)is configured as input with external interrupt (EXTI15_10),
falling edge. When Tamper push-button is pressed, wakeup event is generated and PWM signal
generation is stopped.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example need to ensure that the SysTick time base is always set to 1 millisecond
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

LPTIM, Timer, Low Power, Counter, PWM, Duty Cycle, Stop mode, Wake up, Button, Interrupt, LSE, Output, Waveform, Oscilloscope

@par Directory contents

  - LPTIM/LPTIM_PWM_LSE/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_PWM_LSE/Inc/main.h                  Header for main.c module
  - LPTIM/LPTIM_PWM_LSE/Src/stm32h7xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_PWM_LSE/Src/main.c                  Main program
  - LPTIM/LPTIM_PWM_LSE/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_PWM_LSE/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.

  - This example has been tested with STMicroelectronics STM32H743I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

  - Connect PD.13 (pin 31 in CN11 connector) to an oscilloscope to monitor the LPTIM1 output waveform.

  - LSE oscillator must be mounted on the board, else this example does not work.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
