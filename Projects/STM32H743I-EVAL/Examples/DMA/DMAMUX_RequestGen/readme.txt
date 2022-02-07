/**
  @page DMAMUX_RequestGen DMA & DMAMUX request generator Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA/DMAMUX_RequestGen/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA & request generator Example.
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
This example shows how to use the DMA with the DMAMUX request generator to generate DMA transfer requests upon 
an EXTI0 rising edge signal.

The example uses the BDMA channel 0 configured in memory to peripheral mode.
The DMA request is set to the DMAMUX request generator 0.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The BDMA channel 0 is configured in memory to peripheral mode to ensure data transfer from the source transmission 
buffer (SRC_Buffer_LED1_Toggle) to the GPIOF ODR register (in order to toggle LED1).
The DMA is configured in circular mode so the transfer will restart automatically each time the amount of data 
to be transmitted has been reached.

@note the Domain 3 BDMA has access to  Domain 3 SRAMs and peripherals only,  thus the source buffer (SRC_Buffer_LED1_Toggle)
has been placed to the D3 SRAM (@0x38000000)

The DMAMUX request generator block is configured using function "HAL_DMAEx_ConfigMuxRequestGenerator" 
with the following parameters :
- SignalID      : set to HAL_DMAMUX2_REQUEST_GEN_EXTI0 which corresponds to EXTI0 signal.
- Polarity  : Set to RISING to use rising edge the EXTI0 for DMA requests generation.
- RequestNumber : 1 i.e on each rising edge of the EXTI0 signal a DMA request is generated.

The DMA request generator is then enabled using function �HAL_DMAEx_EnableMuxRequestGenerator�.

The function "EXTI0_IRQHandler_Config" is then used to configure the PA.0 pin to  
external Interrupt Mode with Rising edge trigger detection. 
Note that the example doesn�t need to enable and set EXTI 0 NVIC Interrupt as the EXTI0 
event signal is used to trigger the DMAMUX only and not the CPU.

Then the DMA transfer is started in non-blocking mode using the HAL function "HAL_DMA_Start_IT"
Note that PA.0 pin is connected to the �Wakeup� button of the evaluation board.
Each time the Wakeup button is pressed an EXTI0 event is generated and the DMAMUX will generate a DMA request
upon the rising edge of the EXTI0 signal.
As consequence the DMA will serve the request and write a new value to the GPIOF ODR register to toggle the LED1 
without any CPU intervention.
.

The CPU is only used to intercept a DMA transfer interrupt error or a DMAMUX overrun interrupt error if any,
and sets the LED3 (Red LED) to On in this case.  


STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 toggles each time the Wakeup button is pressed .
 - LED3 is ON when there is an error during the DMA transfer.  



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

System, DMA, DMAMUX, Request generator, Data Transfer, Stream

@par Directory contents 

  - DMA/DMAMUX_RequestGen/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - DMA/DMAMUX_RequestGen/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - DMA/DMAMUX_RequestGen/Inc/main.h                  Header for main.c module  
  - DMA/DMAMUX_RequestGen/Src/stm32h7xx_it.c          DMA interrupt handlers
  - DMA/DMAMUX_RequestGen/Src/main.c                  Main program
  - DMA/DMAMUX_RequestGen/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
