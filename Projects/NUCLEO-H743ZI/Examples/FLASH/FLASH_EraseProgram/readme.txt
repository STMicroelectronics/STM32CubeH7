/**
  @page FLASH_EraseProgram FLASH Erase and Program example
  
  @verbatim
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH Erase and Program example.
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

How to configure and use the FLASH HAL API to erase and program the internal
Flash memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz. 

After Reset, the Flash memory Program/Erase Controller is locked. A dedicated function
is used to enable the FLASH control register access.
Before programming the desired addresses, an erase operation is performed using 
the flash erase sector feature. The erase procedure is done by filling the erase init 
structure giving the starting erase sector and the number of sectors to erase.
At this stage, all these sectors will be erased one by one separately.

@note: if problem occurs on a sector, erase will be stopped and faulty sector will 
be returned to user (through variable 'SECTORError').

Once this operation is finished, sector word programming operation will be performed 
in the Flash memory. The written data is then read back and checked.

The NUCLEO-H743ZI board LEDs can be used to monitor the transfer status:
 - LED1 is ON when there are no errors detected after data programming 
 - LED2 is ON when there are errors detected after data programming 
 - LED3 is ON when there is an issue during erase or program procedure

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
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Memory, FLASH, Erase, Program, Sector, Mass Erase

@par Directory contents 

  - FLASH/FLASH_EraseProgram/Inc/stm32h7xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_EraseProgram/Inc/stm32h7xx_it.h              Header for stm32h7xx_it.c
  - FLASH/FLASH_EraseProgram/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_EraseProgram/Src/stm32h7xx_it.c              Interrupt handlers
  - FLASH/FLASH_EraseProgram/Src/main.c                      Main program
  - FLASH/FLASH_EraseProgram/Src/system_stm32h7xx.c          STM32H7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32H743ZI devices.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.
      
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */
