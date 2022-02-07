/**
  @page DMA_FIFOMode DMA FIFO mode Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    DMA/DMA_FIFOMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA FIFO mode example.
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

This example provides a description of how to use a DMA to transfer 
a word data buffer from Flash memory to embedded SRAM with FIFO mode
enabled through the HAL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

DMA1_Stream7 is configured to transfer the contents of a 32-word data 
buffer stored in Flash memory to the reception buffer declared in RAM.

In this example the FIFO mode is enabled and the FIFO threshold is configured
as "FULL". Also an incremental burst transfer of 4 beats is requested. 

The start of transfer is triggered by software. DMA1_Stream7 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the stream enable bit for DMA1_Stream7.
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.
 

STM32H743I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transfer is complete without error.
 - LED2 is ON when there is a transfer error. 
 - LED3 is ON when there are errors in initialisation.
 
It is possible to select a different stream for the DMA transfer
example by modifying defines values in the file main.h.

@note On the STM32H743xx the ITCM and DTCM RAM are only accessible by the Cortex M7 and the MDMA.
      The ITCM and DTCM RAM are not accessible by the classic DMAs thus the source and destination buffer
      should not be located in these RAMs. 
      In this example the RAM area is configured (at the IDE linker file) to the D1 AXI-SRAM  (@0x24000000) 
      which is accessible by the DMA. The source buffer of the DMA is then located in the internal FLASH 
      and the destination buffer is located in the RAM area which is configured to the D1 AXI-SRAM. 
	  
@note CPU Data Cache maintenance :
      It is recommended to invalidate the CPU Data cache after the DMA transfer. 
      As the destination buffer may be used by the CPU, this guarantees Up-to-date data when CPU access 
      to the destination buffer located in the D1 AXI-SRAM (which is cacheable).

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

System, DMA, FIFO Mode, Data Transfer, FIFO Threshold, Memory to memory, Stream

@par Directory contents 

  - DMA/DMA_FIFOMode/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file
  - DMA/DMA_FIFOMode/Src/stm32h7xx_it.c        Interrupt handlers
  - DMA/DMA_FIFOMode/Src/main.c                Main program
  - DMA/DMA_FIFOMode/Inc/stm32h7xx_hal_conf.h  HAL Configuration file
  - DMA/DMA_FIFOMode/Inc/stm32h7xx_it.h        Interrupt handlers header file
  - DMA/DMA_FIFOMode/Inc/main.h                Main program header file

     
@par Hardware and Software environment

  - This example runs on STM32H743xx devices.

  - This example has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

   

 */
