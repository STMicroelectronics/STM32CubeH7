/**
  @page FMC_SDRAM SDRAM memory functionnalities example
  
  @verbatim
  ******************************************************************************
  * @file    FMC/FMC_SDRAM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC_SDRAM example.
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

This example describes how to configure the FMC controller to access the SDRAM memory.

The SDRAM is IS42S32800G.

In this example, the code is executed from QSPI external memory while data are in internal SRAM memory.
 
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
  
The goal of this example is to explain the different steps to configure the FMC 
and make the SDRAM device ready for access, without using the BSP layer.          

In this example, the SDRAM device is configured and initialized explicitly 
following all initialization sequence steps. After initializing the device, user 
can perform read/write operations on it. A data buffer is written to the SDRAM 
memory, then read back and checked to verify its correctness.

The user can chose his own configuration by commenting/uncommenting the defines for  
undesired/desired configurations in "main.h", for example, to configure the SDRAM 
data bus width as 32 bits, uncomment the define for the configuration 
"FMC_SDRAM_MEM_BUS_WIDTH_32".

If the data is read correctly from SDRAM, the LED1 is ON, otherwise the LED3 is Toggled.

@note - The FMC mode register definition is configured using defines for the external 
        memory device mode register, defined in the main header file.
      - The function "BSP_SDRAM_Initialization_Sequence()" is used to program the SDRAM
        device. It is considered as a specific function which depends on the SDRAM device. 
        When changing the external device, you may have to apply some changes 
        in this function. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FMC, SDRAM, Read, Write, Initialization, Access



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

@par Directory contents 

  - FMC/FMC_SDRAM/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SDRAM/Inc/main.h                  Header for main.c module  
  - FMC/FMC_SDRAM/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FMC/FMC_SDRAM/Src/main.c                  Main program
  - FMC/FMC_SDRAM/Src/stm32h7xx_msp.c          HAL MSP module
  - FMC/FMC_SDRAM/Src/stm32h7xx_it.c          Interrupt handlers
  - FMC/FMC_SDRAM/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This application runs on STM32H750xx devices.

  - This application has been tested with STMicroelectronics STM32H750B-DISCO
    boards and can be easily tailored to any other supported device
    and development board.   

@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this example "Examples\FMC\FMC_SDRAM" (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load this FMC example to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using STM32CubeIDE  :
       - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" in case of XiP from QSPI
            - From Erasing & Programming menu, browse and open the output binary file relative to your example
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)
 

 */
