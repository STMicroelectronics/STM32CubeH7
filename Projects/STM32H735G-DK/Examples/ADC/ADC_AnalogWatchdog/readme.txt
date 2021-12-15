/**
  @page ADC_AnalogWatchdog ADC conversion example with analog watchdog, using 
   related peripherals (GPIO, DMA, Timer), voltage input from DAC, user control
   by User Button and LED

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

How to use the ADC peripheral to perform conversions with an analog watchdog 
and out-of-window interrupts enabled.
One compilation switch is available to select ADC configuration continuous mode
and external trigger (located in main.h):
 - "ADC_TRIGGER_FROM_TIMER" defined: ADC is operating in single conversion mode
   and conversions are triggered by external trigger: timer.
 - "ADC_TRIGGER_FROM_TIMER" not defined: ADC is operating in continuous mode
   and first conversion is triggered by software trigger.

One compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: For this example purpose, generates a
   waveform voltage on a spare DAC channel DAC_CHANNEL_2 (pin PA.05).
   If ADC channel and DAC channel are selected on the same GPIO (default configuration
   in this example):
   connection is done internally in GPIO pad, user has nothing to connect.
   If ADC channel and DAC channel are selected on two different GPIO:
   user has just to connect a wire between DAC channel output and ADC input to run this example.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example.

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trigger ADC conversions.
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
 - Regular group:
   Conversions are triggered by external event (timer at 1kHz).
 - Continuous mode is disabled (and sequencer disabled: only 1 channel selected) to yield only 1 conversion at each conversion trigger.
 - Analog watchdog 1 is enabled, minimum and maximum thresholds are respectively set 
   to 1/8 and 5/8 of full range scale (between about 0.41V and 2.06V with full range of 3.3V).

ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedData".
 - DMA and ADC are configured to operate continuously, in circular mode.
   When DMA transfer half-buffer and full buffer lengths are reached, callbacks
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.

Board settings:
 - ADC is configured to convert ADC_CHANNEL_0 (pin PA.00).
 - Channel configured on regular group:
   The voltage input on ADC channel is provided from DAC channel.
   ADC (pin PA.00) and DAC channel (pin PA.05) are not sharing the same pad.
   ==> Therefore, an external connection is needed to run this example (Link PA.00 and PA.05 with a wire).
@note  You can also use the CN1 connector as ADC1 ADC_CHANNEL_0 input (SB5 has to be fitted).
 - Voltage is increasing at each click on User Button, from 0V to maximum range (Vdda) in 5 steps
   (with vdda=3.3V: 0V, 0.8V, 1.65V, 2.5V, 3.3V), then starting back from 0V.
   Note: Initial voltage value is Vdda/2.

To observe voltage level applied on DAC channel through GPIO, connect a voltmeter on
pin PA.05 (Connect PA5 (DAC Channel2)(pin 13 in STMod+)).

To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA.00 (Connector CN9 pin A2).


STM32H735G-DK board LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off with respect to ADC conversion
   result.
    - Turned-off if voltage is within AWD window
    - Turned-on if voltage is out of AWD window
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.


@note In case of noise on voltage applied on ADC channel input (due to connectors and wires parasitics),
      ADC analog watchdog may trig at a voltage level with an uncertainty of tens of mV.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, ADC, Analog to Digital, analog, monitor, Continuous conversion, Measurement, Voltage

@par Directory contents 

  - ADC/ADC_AnalogWatchdog/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_AnalogWatchdog/Inc/stm32h7xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/Inc/main.h                  Header for main.c module
  - ADC/ADC_AnalogWatchdog/Inc/stm32h735g_discovery_conf.h    HAL configuration file  
  - ADC/ADC_AnalogWatchdog/Src/stm32h7xx_it.c          HAL interrupt handlers
  - ADC/ADC_AnalogWatchdog/Src/main.c                  Main program
  - ADC/ADC_AnalogWatchdog/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - ADC/ADC_AnalogWatchdog/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32H735xx devices.

  - This example has been tested with STM32H735G-DK board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
