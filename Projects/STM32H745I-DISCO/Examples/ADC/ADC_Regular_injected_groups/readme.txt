/**
  @page ADC_Regular_injected_groups ADC conversion example showing the usage of
   the 2 ADC groups: regular group for ADC conversions on main stream and 
   injected group for ADC conversions limited on specific events (conversions
   injected within main conversions stream),
   using related peripherals (GPIO, DMA, Timer), voltage input from DAC, user 
   control by push button and LED

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_Regular_injected_groups/readme.txt 
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

This example provides a short description of how to use the ADC peripheral to
perform conversions using the two ADC groups: regular group for ADC conversions
on main stream and injected group for ADC conversions limited on specific events
(conversions injected within main conversions stream).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H745xx Devices :
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
   waveform voltage on a spare DAC channel: DAC_CHANNEL_1 on pin PA.04 (Connector STMOD pin 13 )
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
 - Injected group: 
   Conversions are software-triggered by a mere call to HAL_ADCEx_InjectedStart_IT()  
   to convert VREFINT channel at each press on Tamper button.

ADC conversion results:
 - ADC regular conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - DMA and ADC are configured to operate continuously, in circular mode.
   When DMA transfer half-buffer and full buffer lengths are reached, callbacks 
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
 - ADC injected conversions results are retrieved upon JEOC interrupt by
   HAL_ADCEx_InjectedConvCpltCallback() function.
 - uhADCxConvertedValue_Injected variable contains the  ADC conversion result of injected group.

Board settings:
 - ADC is configured to convert ADC_CHANNEL_7 on pin PF.08 (Connector CN7 pin 2).
 - Channel configured on regular group:
   User has to connect the Disco Board ADC3 ADC_CHANNEL_7 PF.08 pin
   to the DAC1 output PA.04 pin (STMod+ connector  pin 13 ).
 - Voltage is increasing at each click on push button, from 0 to maximum range in 4 steps.
   Clicks on push button follow circular cycles: At clicks counter maximum value reached, counter is set back to 0.
 - Channel configured on injected group:
   The voltage input is provided from internal reference voltage VrefInt.

STM32H745I-DISCO board LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off in function of ADC conversion
   result.
    - Turned-off if voltage measured by injected group is below voltage measured by regular group (average of results table)
    - Turned-on if voltage measured by injected group is above voltage measured by regular group (average of results table)
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.

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

Analog, ADC, Analog to Digital, Regular conversion, Injected conversion, Continuous conversion, software-triggered, Timer-triggered, Measurement, Voltage, Button

@par Directory contents 

  - ADC/ADC_Regular_injected_groups/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration file

  - ADC/ADC_Regular_injected_groups/CM7/Src/main.c                 Main program for Cortex-M7
  - ADC/ADC_Regular_injected_groups/CM7/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7
  - ADC/ADC_Regular_injected_groups/CM7/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M7
  - ADC/ADC_Regular_injected_groups/CM7/Inc/main.h                 Main program header file for Cortex-M7  
  - ADC/ADC_Regular_injected_groups/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - ADC/ADC_Regular_injected_groups/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7

  - ADC/ADC_Regular_injected_groups/CM4/Src/main.c                 Main program for Cortex-M4
  - ADC/ADC_Regular_injected_groups/CM4/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M4
  - ADC/ADC_Regular_injected_groups/CM4/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M4
  - ADC/ADC_Regular_injected_groups/CM4/Inc/main.h                 Main program header file for Cortex-M4  
  - ADC/ADC_Regular_injected_groups/CM4/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M4
  - ADC/ADC_Regular_injected_groups/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M4
  

@par Hardware and Software environment 

  - This example runs on STM32H745xx devices.

  - This example has been tested with STMicroelectronics STM32H745I-DISCO
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745I_DISCO_CM7 and STM32H745I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
