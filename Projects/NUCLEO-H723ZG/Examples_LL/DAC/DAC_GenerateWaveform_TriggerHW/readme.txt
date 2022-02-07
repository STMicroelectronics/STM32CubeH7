/**
  @page DAC_GenerateWaveform_TriggerHW DAC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/DAC/DAC_GenerateWaveform_TriggerHW/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DAC_GenerateWaveform_TriggerHW example.
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

How to use the DAC peripheral to generate a voltage waveform from a digital data
stream transferred by DMA. This example is based on the STM32H7xx
DAC LL API. The peripheral initialization uses LL unitary service
functions for optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

Example configuration:
One DAC channel (DAC1 channel1) is configured to connect DAC channel output on GPIO pin
to get the samples from DMA transfer and to get conversion trigger from timer.

Other peripherals configured in this example:
one DMA channel and one timer are configured to provide samples to be generated
by DAC peripheral:
sinus waveform, frequency 1kHz, amplitude Vdda.
(these settings are configurable by changing literals values in file header).

Example execution:
From the main program execution, LED1 is toggling quickly while waiting for
user button press.
DMA and timer are configured and activated to transfer data and trig DAC conversion.
Then, the DAC is configured and activated: waveform is generated on DAC output
indefinitely.
DAC channel output value is provided by DMA transfer, a new value is loaded
at each trigger from trimer.
Finally, LED1 is turned-on.

Connection needed:
None. 
Oscilloscope for monitoring DAC channel output (cf pin below).
Other peripheral used:
  1 GPIO for push button
  1 GPIO for DAC channel output PA.04 (Arduino connector CN7 pin 17 or Morpho connector CN11 pin 32)
  For waveform voltage generation: 1 DMA channel, 1 timer.


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

Analog, DAC, Waveform, Voltage output, Oscilloscope, Signal, Hardware trigger, DMA, Timer, Sinus

@par Directory contents 

  - DAC/DAC_GenerateWaveform_TriggerHW/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - DAC/DAC_GenerateWaveform_TriggerHW/Inc/main.h                  Header for main.c module
  - DAC/DAC_GenerateWaveform_TriggerHW/Inc/stm32_assert.h          Template file to include assert_failed function
  - DAC/DAC_GenerateWaveform_TriggerHW/Src/stm32h7xx_it.c          Interrupt handlers
  - DAC/DAC_GenerateWaveform_TriggerHW/Src/main.c                  Main program
  - DAC/DAC_GenerateWaveform_TriggerHW/Src/system_stm32h7xx.c      STM32H7xx system source file


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
