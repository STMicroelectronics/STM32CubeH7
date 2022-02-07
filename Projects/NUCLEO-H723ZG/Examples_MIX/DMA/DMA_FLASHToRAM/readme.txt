/**
  @page DMA_FLASHToRAM DMA example

  @verbatim
  ******************************************************************************
  * @file    Examples_MIX/DMA/DMA_FLASHToRAM/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the DMA example.
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

How to use a DMA to transfer a word data buffer from Flash memory to embedded
SRAM through the STM32H7xx DMA HAL and LL API. The LL API is used for 
performance improvement.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

DMA2_Stream0 is configured to transfer the contents of a 32-word data
buffer stored in Flash memory to the reception buffer declared in RAM.

The start of transfer is triggered by LL API. DMA2_Stream0 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the stream enable bit for DMA2_Stream0.
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.

Board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transfer is complete (into the Transfer Complete interrupt
   routine).
 - LED2 is ON when there is a transfer error
 - LED3 is ON when a Error_Handler is called

It is possible to select a different stream for the DMA transfer
example by modifying defines values in the file main.h.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

System, DMA, Data Transfer, Memory to memory, Stream, Flash, RAM

@par Directory contents

  - DMA/DMA_FLASHToRAM/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - DMA/DMA_FLASHToRAM/Src/stm32h7xx_it.c          Interrupt handlers
  - DMA/DMA_FLASHToRAM/Src/main.c                  Main program
  - DMA/DMA_FLASHToRAM/Inc/stm32h7xx_hal_conf.h    HAL Configuration file
  - DMA/DMA_FLASHToRAM/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - DMA/DMA_FLASHToRAM/Inc/main.h                  Main program header file
  - DMA/DMA_FLASHToRAM/Inc/stm32h7xx_nucleo_conf.h    STM32H7xx_Nucleo board configuration file

@par Hardware and Software environment

  - This example runs on STM32H723xx Devices.

  - This example has been tested with STMicroelectronics NUCLEO-H723ZG
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
