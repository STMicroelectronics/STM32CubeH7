/**
  @page ADC_AnalogWatchdog ADC conversion example with analog watchdog, using
   related peripherals (GPIO, DMA, Timer), voltage input from DAC, user control
   by user button, LED, and optionally by potentiometer (RV1).

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_AnalogWatchdog/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

This example provides a short description of how to use the ADC peripheral to
perform conversions with analog watchdog and out-of-window interruptions enabled.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

One compilation switch is available to select ADC configuration continuous mode
and external trigger (located in main.h):
 - "ADC_TRIGGER_FROM_TIMER" defined: ADC is operating in not continuous mode
   and conversions are trigger by external trigger: timer.
 - "ADC_TRIGGER_FROM_TIMER" not defined: ADC is operating in continuous mode
   and first conversion is trigger by software trigger.

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
 - ADC regular conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - DMA and ADC are configured to operate continuously, in circular mode.
   When DMA transfer half-buffer and full buffer lengths are reached, callbacks
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.

Board settings:
 - ADC is configured to convert ADC_CHANNEL_18 (pin PA.04).
Two ways are possible to feed analog data to the ADC channel.
 1°) The Eval Board potentiometer (RV1) is able to make the ADC_CHANNEL_18 voltage vary,
 by connecting PA.0_c pin to PA04 pin .
 Therefore, the user can turn the potentiometer then press the tamper button.
Pressing the tamper button leads to a software check that verifies whether or not
a converted data is out of the analog watchdog 1 range. If this the case, LED1
is turned on, otherwise, if the converted data at the time of the button press
is not out-of-range, LED1 is turned off.

 2°) Alternatively, the user can connect the Eval Board ADC1 ADC_CHANNEL_18 PA.04 pin
 to the DAC1 output PA.04 pin.
 The voltage at the input of the ADC is increased in successive steps (up to 4 steps)
  at each click on the tamper button.
  When the voltage has reached its maximum value, the voltage restarts from its minimum value for a new steps cycle.
  Again, at each tamper button key press, the converted data is compared to the analog watchdog thresholds
 and LED1 is turned on if a data is out-of-range.

To observe voltage level applied on ADC channel through GPIO, connect a voltmeter on
pin PA.04 (connector CN6 pin 19).

STM32H743I-EVAL board LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off with respect to ADC conversion
   result.
    - Turned-off if voltage is within AWD window
    - Turned-on if voltage is out of AWD window
 - Error: In case of error, LED4 is toggling at a frequency of 1Hz.

@note LED3 is ON during test because it is connected to PA4 (DAC channel1).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
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

Analog, ADC, Analog to Digital, analog, monitor, Continuous conversion, Measurement, Voltage

@par Directory contents

  - ADC/ADC_AnalogWatchdog/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_AnalogWatchdog/Inc/stm32h7xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/Inc/main.h                  Header for main.c module
  - ADC/ADC_AnalogWatchdog/Src/stm32h7xx_it.c          HAL interrupt handlers
  - ADC/ADC_AnalogWatchdog/Src/main.c                  Main program
  - ADC/ADC_AnalogWatchdog/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - ADC/ADC_AnalogWatchdog/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H743xx devices.

  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
