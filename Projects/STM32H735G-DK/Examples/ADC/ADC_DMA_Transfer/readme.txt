/**
  @page ADC_DMA_Transfer ADC DMA Transfer example

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_DMA_Transfer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC DMA Transfer example.
  ******************************************************************************
  *
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

How to configure and use the ADC to convert an external analog input and get 
the result using a DMA transfer through the HAL API.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735XX Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

The ADC is configured to continuously convert ADC3 channel ADC_CHANNEL_0, resolution is set to 12 bits right aligned, conversion is software-triggered.

DMA1_Stream1 is configured in Circular mode to transfer continuously
the content of ADC_DR (Data Register) to "aADCxConvertedData" variable which 
stores the conversion result.

User may watch variable array "aADCxConvertedData" and check that its values are modified
according to the voltage applied to pin PC2_C (pin A4 in connector CN9).

STM32 board LED can be used to monitor the conversion:
  - LED1 is ON when there is an error in initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, ADC, DMA, Continuous conversion, Circular, Measurement, Voltage

@par Directory contents 

  - ADC/ADC_DMA_Transfer/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DMA_Transfer/Inc/stm32h7xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_DMA_Transfer/Inc/main.h                  Header for main.c module  
  - ADC/ADC_DMA_Transfer/Src/stm32h7xx_it.c          HAL interrupt handlers
  - ADC/ADC_DMA_Transfer/Src/main.c                  Main program
  - ADC/ADC_DMA_Transfer/Src/stm32h7xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_DMA_Transfer/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H735xx devices.

  - This example has been tested with STM32H735G-DK board and can be
    easily tailored to any other supported device and development board.

  - STM32H735G-DK Set-up
    Use an external power supply, adjust supply voltage and connect it to pin PC2_C (pin A4 in connector CN9).

  - To monitor the conversion result, put the "aADCxConvertedData" variable in the debugger live watch.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
