/**
  @page ADC_AnalogWatchdog ADC conversion example with analog watchdog, using 
   related peripherals (GPIO, DMA, Timer), voltage input from DAC, user control
   by User push-button and LED

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

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3ZITx Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

One compilation switch is available to select ADC configuration continuous mode
and external trigger (located in main.h):
 - "ADC_TRIGGER_FROM_TIMER" defined: ADC is operating in single conversion mode
   and conversions are triggered by external trigger: timer.
 - "ADC_TRIGGER_FROM_TIMER" not defined: ADC is operating in continuous mode
   and first conversion is triggered by software trigger.

One compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: For this example purpose, generates a
   waveform voltage on a spare DAC channel DAC_CHANNEL_1 (pin PA.04).
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
 - ADC is configured to convert ADC_CHANNEL_18 (pin PA.04).
 - Channel configured on regular group:
   The voltage input on ADC channel is provided from DAC channel.
   ADC and DAC channel have been chosen to have the same pad shared at device level: pin PA.04.
   ==> Therefore, there is no external connection needed to run this example.
 - Voltage is increasing at each click on User push-button, from 0V to maximum range (Vdda) in 5 steps
   (with vdda=3.3V: 0V, 0.8V, 1.65V, 2.5V, 3.3V), then starting back from 0V.
   Note: Initial voltage value is Vdda/2.

To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA.04 (Arduino connector CN7 pin 17, connector CN11 pin 32).


NUCLEO-H7A3ZI-Q board LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off with respect to ADC conversion
   result.
    - Turned-off if voltage is within AWD window
    - Turned-on if voltage is out of AWD window
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz.


@note In case of noise on voltage applied on ADC channel input (due to connectors and wires parasitics),
      ADC analog watchdog may trig at a voltage level with an uncertainty of tens of mV.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Analog, ADC, Analog to Digital, Analog, Monitor, Continuous conversion, Measurement, Voltage

@par Directory contents 

  - ADC/ADC_AnalogWatchdog/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_AnalogWatchdog/Inc/stm32h7xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/Inc/main.h                  Header for main.c module
  - ADC/ADC_AnalogWatchdog/Inc/stm32h7xx_nucleo_conf.h    HAL configuration file  
  - ADC/ADC_AnalogWatchdog/Src/stm32h7xx_it.c          HAL interrupt handlers
  - ADC/ADC_AnalogWatchdog/Src/main.c                  Main program
  - ADC/ADC_AnalogWatchdog/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - ADC/ADC_AnalogWatchdog/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32H7A3xxQ devices.

  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
