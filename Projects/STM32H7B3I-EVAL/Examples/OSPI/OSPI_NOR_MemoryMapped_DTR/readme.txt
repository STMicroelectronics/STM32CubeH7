/**
  @page OSPI_NOR_MemoryMapped_DTR OSPI NOR memory mapped example
  
  @verbatim
  ********************* COPYRIGHT(c) 2018 STMicroelectronics *******************
  * @file    OSPI/OSPI_NOR_MemoryMapped_DTR/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the OSPI NOR memory mapped mode example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example describes how to erase part of the OSPI NOR memory, write data in IT mode and 
access to OSPI NOR memory in memory-mapped mode to check the data in a forever loop.The memory is 
configured in octal DTR mode.

This projects is configured for STM32H7B3xxQ devices using STM32CubeH7 HAL and running on 
STM32H7B3I-EVAL board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz 
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

LED_GREEN toggles each time a new comparison is good
LED_RED is on as soon as a comparison error occurs
LED_YELLOW is on as soon as an error is returned by HAL API

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
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

Memory, OSPI, OCTOSPI, Erase, Write, sector, memory-mapped mode, NOR, DTR mode, Interruption

@par Directory contents 

  - OSPI/OSPI_NOR_MemoryMapped/Src/main.c                 Main program
  - OSPI/OSPI_NOR_MemoryMapped/Src/system_stm32h7xx.c     STM32H7xx system clock configuration file
  - OSPI/OSPI_NOR_MemoryMapped/Src/stm32h7xx_it.c         Interrupt handlers 
  - OSPI/OSPI_NOR_MemoryMapped/Inc/stm32h7b3i_eval_conf.h STM32H7B3I_EVAL board configuration file.
  - OSPI/OSPI_NOR_MemoryMapped/Src/stm32h7xx_hal_msp.c    HAL MSP module
  - OSPI/OSPI_NOR_MemoryMapped/Inc/main.h                 Main program header file  
  - OSPI/OSPI_NOR_MemoryMapped/Inc/stm32h7xx_hal_conf.h   HAL Configuration file
  - OSPI/OSPI_NOR_MemoryMapped/Inc/stm32h7xx_it.h         Interrupt handlers header file
     

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ device.

  - The jumpers JP4  and JP23 must be fitted on OSPI1 position .
    
  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


