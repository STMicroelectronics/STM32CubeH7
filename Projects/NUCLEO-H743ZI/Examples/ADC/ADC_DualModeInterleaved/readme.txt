/**
  @page ADC_DualModeInterleaved Two ADC in interleaved dual-mode, data transfer by DMA

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    ADC/ADC_DualModeInterleaved/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Dual interleaved mode example
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

This example provides a short description of how to use two ADC peripherals to
perform conversions in interleaved dual-mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

A compilation switch is available to select ADC configuration continuous mode
and external trigger (located in main.h):
 - "ADC_TRIGGER_FROM_TIMER" defined: ADC is operating in not continuous mode
   and conversions are triggered by external trigger: timer.
 - "ADC_TRIGGER_FROM_TIMER" not defined: ADC is operating in continuous mode
   and first conversion is triggered by software trigger.

A compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: For this example purpose, generates a
   waveform voltage by DAC peripheral on a spare DAC channel: DAC_CHANNEL_1 on pin PA.04 (connector CN11 pin 32).
   If ADC channel and DAC channel are selected on the same GPIO (default configuration
   in this example):
   connection is done internally in GPIO pad, user has nothing to connect.
   If ADC channel and DAC channel are selected on two different GPIO:
   user has just to connect a wire between DAC channel output and ADC input to run this example.
   This voltage follows a 5-step curve, ADC sampling rate is set so that for a given
   ADC, two conversions are carried out for one DAC output level.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input

Note: In this example, ADC sampling rate is not set as the highest possible (ADC parameters set for this purpose: resolution 16 bits, ... ).
      ADC sampling rate can be increased by modifying resolution and sampling time, but this will decrease the conversion accuracy.

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trigger ADC conversions.
 - DMA peripheral is used to transfer ADC converted data.
   Both ADC are using the same DMA (DMA of ADC master). DMA is used as well by DAC in case of WAVEFORM_VOLTAGE_GENERATION_FOR_TEST enabled. 

ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCDualConvertedValues".
   ADC master and ADC slave results are concatenated in a common 32-bit data register:
   - ADC master results in the 16 LSB [15:0] (with ADC resolution 16 bits, bits effectively used are [15:0])
   - ADC slave results in the 16 MSB [31:16] (with ADC resolution 16 bits, bits effectively used are [31:16])


 - DMA are configured to operate continuously, in circular mode.
   Data is transferred by words every ADC1+ADC2 conversions.
   When DMA transfer half-buffer and complete buffer length are reached, callbacks 
   HAL_ADC_ConvCpltCallback() and HAL_ADC_ConvCpltCallback() are called.

 - In this example, dual conversion values are extracted from the common register and
   stored into 2 arrays corresponding to each ADC conversion values
   (aADCxConvertedValues, aADCyConvertedValues).


Board settings:
  ADC1 and ADC2 are configured to convert the same channel: ADC_CHANNEL_18 on pin PA.04 (connector CN11 pin 32).

  If compilation switch "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined, the voltage input on ADC channel comes 
  from the DAC channel. This means that, if they are different, ADC pin PA.04 and DAC pin PA.04 must
  be connected by a wire. 

NUCLEO-H743ZI board LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off in function of ADC conversion
   status:
    - Turned-off if ADC conversions have just filled up half of the buffer (DMA half-transfer interrupt has been triggered)
    - Turned-on if ADC conversions have filled up the complete buffer (DMA transfer complete interrupt has been triggered)
   The buffer fill up carries on indefinitely. 
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Analog, ADC, Analog to Digital, Dual mode, Interleaved, Continuous conversion, Software Trigger, DMA, Measurement

@par Directory contents 

  - ADC/ADC_DualModeInterleaved/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DualModeInterleaved/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_DualModeInterleaved/Inc/main.h                  Header for main.c module  
  - ADC/ADC_DualModeInterleaved/Src/stm32h7xx_it.c          DMA interrupt handlers
  - ADC/ADC_DualModeInterleaved/Src/main.c                  Main program
  - ADC/ADC_DualModeInterleaved/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - ADC/ADC_DualModeInterleaved/Src/system_stm32h7xx.c      stm32h7xx system source file

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
