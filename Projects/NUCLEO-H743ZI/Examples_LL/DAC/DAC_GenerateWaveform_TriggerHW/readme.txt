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
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example describes how to use the DAC peripheral to generate a waveform voltage
from digital data stream transferred by DMA. This example is based on the STM32H7xx
DAC LL API. The peripheral initialization is done using LL unitary service functions
for optimization purpose (performance and size).

Example configuration:
One DAC channel (DAC1 channel1) is configured to connect DAC channel output on GPIO pin
to get the samples from DMA transfer and to get conversion trigger from timer.

Other peripherals configured in this example:
one DMA channel and one timer are configured to provide samples to be generated
by DAC peripheral:
sinus waveform, frequency 1kHz, amplitude Vdda.
(these settings are configurable by changing literals values in file header).

Example execution:
From the main program execution, LED is toggling quickly while waiting for
user button press.
DMA and timer are configured and activated to transfer data and trig DAC conversion.
Then, the DAC is configured and activated: waveform is generated on DAC output
indefinitely.
DAC channel output value is provided by DMA transfer, a new value is loaded
at each trigger from timer.
Finally, LED is turned-on.

Connection needed:
None. 
Oscilloscope for monitoring DAC channel output (cf pin below).
Other peripheral used:
  1 GPIO for push button
  1 GPIO for DAC channel output PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  For waveform voltage generation: 1 DMA channel, 1 timer.

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

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
