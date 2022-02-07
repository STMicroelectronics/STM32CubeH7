/**
  @page ADC_Oversampling ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples/ADC/ADC_Oversampling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_Oversampling example.
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

This example describes how to use a ADC peripheral with ADC oversampling.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3ZITx Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

Description of ADC oversampling:
Multiple successive ADC conversions and average computation
are performed automatically, by ADC hardware, and therefore
off-load the CPU to do the equivalent task.
This feature can be used for the functions: averaging,
data rate reduction, SNR improvement, basic filtering.
This example uses 3 configurations of oversampling,
for comparison of final data to evaluate oversampling benefits.

Example configuration:
The ADC is configured to convert continuously ADC_CHANNEL_3 connected to PA.06 (connector CN12 pin 13)
while the oversampler is enabled. 
Oversampling ratio is set to 1024 so the maximum output result 
is 0xFFFF * 1024 = 0x3FFFC00 (26 bits). Result is 6-bit right shift.

uwConvertedValue variable contains the 20-bit conversion result and 
uwInputVoltage yields the input voltage in mV. 

Connection needed:
A voltage between 0V and Vdda=3.3V can be supplied to the analog input pin PA.06 
(connector CN12 pin 13) to perform a ADC conversion at a determined voltage level.
Otherwise, this pin can be left floating (in this case ADC conversion data will be undetermined).

Other peripherals used:
  1 GPIO for LED
  1 GPIO for analog input: PA.06 (connector CN12 pin 13)

The converted value is monitored through debugger: uwConvertedValue and uwInputVoltage variables.
STM32 board LED can be used to monitor the conversion:
  - LED3 toggle each 500ms in case of error

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decreases critical task execution time, compared  
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be 
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
	  the ITCM-RAM.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
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

Analog, ADC, Analog to Digital, oversampling, Continuous conversion, Measurement, Voltage

@par Directory contents

  - ADC/ADC_Oversampling/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_Oversampling/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - ADC/ADC_Oversampling/Inc/main.h                  Header for main.c module
  - ADC/ADC_Oversampling/Inc/stm32h7xx_nucleo_conf.h    HAL configuration file  
  - ADC/ADC_Oversampling/Src/stm32h7xx_it.c          Interrupt handlers
  - ADC/ADC_Oversampling/Src/main.c                  Main program
  - ADC/ADC_Oversampling/Src/system_stm32h7xx.c      STM32H7xx system source file


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
