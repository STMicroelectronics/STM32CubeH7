/**
  @page DMAMUX_RequestGen DMA & DMAMUX request generator Example
  
  @verbatim
  ******************************************************************************
  * @file    DMA/DMAMUX_RequestGen/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA & request generator Example.
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

This example shows how to use the DMA with the DMAMUX request generator to generate DMA transfer requests upon 
LPTIM2 output signal, knowing that the LPTIM2 is configured in PWM with 2sec period.

The example uses the BDMA_Channel0 channel 0 configured in memory to peripheral mode with the DMAMUX request generator 0.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
The BDMA_Channel0 is configured in memory to peripheral mode to ensure data transfer from the source transmission 
buffer (SRC_Buffer_LED2_Toggle) to the LED2 GPIO ODR register (in order to toggle LED2).
The DMA is configured in circular mode so the transfer will restart automatically each time the amount of data 
to be transmitted has been reached.

@note the Domain 3 BDMA has access to  Domain SRAM and peripherals only,  thus the source buffer (SRC_Buffer_LED2_Toggle)
has been placed to the D3 SRAM (@0x38000000)

The DMAMUX request generator block is configured using function "HAL_DMAEx_ConfigMuxRequestGenerator" 
with the following parameters :
- SignalID      : set to HAL_DMAMUX2_REQUEST_GEN_LPTIM2_OUT which corresponds to LPTIM2 output signal.
- Polarity      : Set to RISING to use rising edge the LPTIM2 output signal for DMA requests generation.
- RequestNumber : 1 i.e. on each rising edge of the LPTIM2 output signal, a DMA request is generated.

The DMA request generator is then enabled using function “HAL_DMAEx_EnableMuxRequestGenerator”.

The function "LPTIM_Config" is then used to configure the LPTIM2 to generate a PWM with a period of 2sec. 
Note that the example doesn’t need to configure a GPIO to LPTIM2 output alternate function as the LPTIM2 output signal 
is used internally to trigger the DMAMUX request generator in order to generate DMA transfer requests every 2sec.
Note that the example doesn’t need to enable and set EXTI15_10 NVIC Interrupt as the EXTI15_10 
event signal is used to trigger the DMAMUX only and not the CPU.

Then the DMA transfer is started in non-blocking mode using the HAL function "HAL_DMA_Start_IT"
On each LPTIM2 output event (i.e every 2sec) the DMAMUX request generator generates a DMA request.
As consequence the DMA will serve the request and write a new value to the GPIOB ODR register to toggle the LED1 
without any CPU intervention.
As consequence LED1 will toggle every 2sec.

The CPU is only used to intercept a DMA transfer interrupt error or a DMAMUX overrun interrupt error if any.
Then it sets the LED3 (Red LED) to On in this case.


STM32H7B3I-DISCO board's LEDs can be used to monitor the transfer status:
 - LED2 toggles each 2 seconds
 - LED3 is ON when there is an error during the DMA transfer.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, DMA, DMAMUX, Request generator, Data Transfer, Stream

@par Directory contents 

  - DMA/DMAMUX_RequestGen/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - DMA/DMAMUX_RequestGen/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - DMA/DMAMUX_RequestGen/Inc/stm32h7b3i_discovery_conf.h    HAL configuration file  
  - DMA/DMAMUX_RequestGen/Inc/main.h                  Header for main.c module  
  - DMA/DMAMUX_RequestGen/Src/stm32h7xx_it.c          DMA interrupt handlers
  - DMA/DMAMUX_RequestGen/Src/main.c                  Main program
  - DMA/DMAMUX_RequestGen/Src/system_stm32h7xx.c      FAMILYNAME_UC system source file

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ device.
    
  - This example has been tested with STM32H7B3I-DISCO board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
