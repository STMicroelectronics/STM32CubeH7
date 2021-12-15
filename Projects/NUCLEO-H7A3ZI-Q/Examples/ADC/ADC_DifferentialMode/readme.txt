/**
  @page ADC_DifferentialMode ADC conversion of 2 channels in differential mode, voltage input from DAC, user control by push button and LED

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_DifferentialMode/readme.txt 
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

How to use an ADC peripheral to perform a conversion in differential mode 
between 2 ADC channels.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3ZITx Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

ADC settings:
  Continuous mode to have conversions kept running after 1st conversion trig.
  Software start to trig the 1st conversion manually, without external event.
  Channel in differential mode.

ADC conversion results:
  ADC conversions results are transferred automatically by ADC IRQ handler, through
  function HAL_ADC_ConvCpltCallback(), into variable "uhADCxConvertedValue".
  swInputVoltage yields the signed input voltage in mV.

Board settings:
   Instance ADC1 is used in the example.
   User has to connect two voltage sources to the selected ADC channel inputs to run this example.
   - channel_high input is connected to ADC_CHANNEL_5 (pin PB.01, Pin 24 in CN12 Connector)
   - channel_low input is connected to ADC_CHANNEL_5 (pin PB.00, Pin 34 in CN11 connector)
   
   User can read "uhADCxConvertedValue" to get the ADC converted differential value.

 - LED1 connected to PB0 is not used in this example (GPIO used by ADC).
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz. 

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

Analog, ADC, Analog to Digital, Differential Mode, Continuous conversion, Measurement, Voltage

@par Directory contents 

  - ADC/ADC_DifferentialMode/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DifferentialMode/Inc/stm32h7xx_it.h          ADC interrupt handlers header file
  - ADC/ADC_DifferentialMode/Inc/main.h                  Header for main.c module 
  - ADC/ADC_DifferentialMode/Inc/stm32h7xx_nucleo_conf.h    HAL configuration file  
  - ADC/ADC_DifferentialMode/Src/stm32h7xx_it.c          ADC interrupt handlers
  - ADC/ADC_DifferentialMode/Src/main.c                  Main program
  - ADC/ADC_DifferentialMode/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - ADC/ADC_DifferentialMode/Src/system_stm32h7xx.c      stm32h7xx system source file

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
