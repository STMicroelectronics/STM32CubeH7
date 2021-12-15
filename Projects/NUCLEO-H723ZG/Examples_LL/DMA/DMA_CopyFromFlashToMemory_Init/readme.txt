/**
  @page DMA_CopyFromFlashToMemory_Init DMA example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/DMA/DMA_CopyFromFlashToMemory_Init/readme.txt 
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

How to use a DMA to transfer a word data buffer
from Flash memory to embedded SRAM. The peripheral initialization uses LL
initialization functions to demonstrate LL init usage.

At the beginning of the main program the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

Then the LED_Init() function is used to initialize the LED1.

Then the Configure_DMA() function is used to configure the DMA2_Stream0 to transfer the contents of a 32-word data
buffer stored in Flash memory to the reception buffer declared in RAM.

The start of transfer is triggered by software(LL_DMA_EnableStream()). DMA2_Stream0 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the channel enable bit for DMA2_Stream0.
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.

Finally, aSRC_Const_Buffer and aDST_Buffer are compared through Buffercmp() in order to 
check buffers correctness.  

NUCLEO-H723ZG's LED1 can be used to monitor the transfer status:
- LED1 is turned ON if the DMA data transfer is successfully completed.
- LED1 is blinking every 1 sec in case of error.


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

System, DMA, Data Transfer, Memory to memory, Stream, Flash, SRAM

@par Directory contents 

  - DMA/DMA_CopyFromFlashToMemory_Init/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - DMA/DMA_CopyFromFlashToMemory_Init/Inc/main.h                  Header for main.c module  
  - DMA/DMA_CopyFromFlashToMemory_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - DMA/DMA_CopyFromFlashToMemory_Init/Src/stm32h7xx_it.c          Interrupt handlers
  - DMA/DMA_CopyFromFlashToMemory_Init/Src/main.c                  Main program
  - DMA/DMA_CopyFromFlashToMemory_Init/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
