/**
  @page DMA2D_MemoryToMemory DMA2D : Memory to Memory transfer example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/DMA2D/DMA2D_MemoryToMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D_MemoryToMemory example.
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

How to configure the DMA2D peripheral in Memory-to-memory transfer mode. The
example is based on the STM32H7xx DMA2D LL API. The peripheral
initialization uses LL unitary service functions for optimization purposes
(performance and size).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3XXQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.

The example achieves a DMA2D transfer of an internal SRAM static texture array aBufferInput[] 
of size 16x32 and format ARGB4444 (16 bpp) to a destination buffer in internal SRAM in 
array aBufferResult[].
The number of bytes transferred is then 16x32x2=1024 bytes.

Peripheral initialization done using LL unitary service functions
for optimization purpose (performance and size).

After DMA2D configuration, the data transfer is performed in polling mode and then the transferred 
data are checked in comparing the source and destination buffers aBufferInput[] and aBufferResult[].

STM32H7B3I-DISCO board LED2 is used to monitor the transfer status:
- LED2 is ON in case of successful transfer,
- LED2 is blinking in case of transfer error.

@Note If the  example is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the example needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case, it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Display, Graphics, DMA2D, SRAM, ARGB4444, Memory to Memory

@par Directory contents 

  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemoryToMemory/Inc/main.h                  Header for main.c module
  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32_assert.h          Template file to include assert_failed function
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32h7xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemoryToMemory/Src/main.c                  Main program
  - DMA2D/DMA2D_MemoryToMemory/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STM32H7B3I-DISCO board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
