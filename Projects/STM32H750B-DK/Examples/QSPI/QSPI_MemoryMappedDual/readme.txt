/**
  @page QSPI_MemoryMappedDual QSPI memory mapped dual mode example
  
  @verbatim
  ******************************************************************************
  * @file    QSPI/QSPI_MemoryMappedDual/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the QSPI memory mapped dual mode example.
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


This example describes how to erase part of the QSPI memory, write data in IT mode and access to QSPI memory 
in memory-mapped dual mode to check the data in a forever loop.

In this example, the code is executed from internal flash while data are in internal SRAM memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

This example uses the QSPI memory in dual mode. You may need to power Off/Power On the board after flashing 
this example if the previous setting of the QSPI memory is single mode.

This example uses the QSPI memory in dual mode. You may need to power Off/Power On the board after flashing 
this example if the previous setting of the QSPI memory is single mode.

LED1 is turned on as soon as the comparison is successful.
The code enter a infinite while loop as soon as a comparison error occurs or an error is returned by HAL API.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Memory, QSPI, Erase, Read, Write, Mapped, Dual

@par Directory contents 

  - QSPI/QSPI_MemoryMappedDual/Inc/stm32h7xx_hal_conf.h HAL configuration file
  - QSPI/QSPI_MemoryMappedDual/Inc/stm32h7xx_it.h       Interrupt handlers header file
  - QSPI/QSPI_MemoryMappedDual/Inc/main.h               Header for main.c module  
  - QSPI/QSPI_MemoryMappedDual/Src/stm32h7xx_it.c       Interrupt handlers
  - QSPI/QSPI_MemoryMappedDual/Src/main.c               Main program
  - QSPI/QSPI_MemoryMappedDual/Src/system_stm32h7xx.c   STM32H7xx system source file
  - QSPI/QSPI_MemoryMappedDual/Src/stm32h7xx_hal_msp.c  HAL MSP file    
	
@par Hardware and Software environment  

  - This example runs on STM32H750xx devices.
  
  - This example has been tested with STM32H750B-DISCO board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H750B_DISCO) : 
     - Rebuild all files 
     - Load images into target memory
 - Run the example
                          

 */
                                   
