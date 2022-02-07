/**
  @page FMC_SDRAM SDRAM memory functionalities example
  
  @verbatim
  ********************* COPYRIGHT(c) 2018 STMicroelectronics *******************
  * @file    FMC/FMC_SDRAM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC_SDRAM example.
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

This example describes how to configure the FMC controller to access the SDRAM memory.

The SDRAM is IS42S32800J.
 
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz..

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

Memory, FMC, SDRAM, Read, Write, Initialization, Access

@par Directory contents 

 - FMC/FMC_SDRAM/Inc/stm32h7xx_hal_conf.h        HAL Configuration file
 - FMC/FMC_SDRAM/Inc/main.h                      Header for main.c module 
 - FMC/FMC_SDRAM/Inc/stm32h7xx_it.h              Header for stm32h7xx_it.c
 - FMC/FMC_SDRAM/Inc/stm32h7b3i_eval_conf.h      STM32H7B3I_EVAL board configuration file.
 - FMC/FMC_SDRAM/Src/main.c                      Main program
 - FMC/FMC_SDRAM/Src/stm32h7xx_hal_msp.c         HAL MSP module  
 - FMC/FMC_SDRAM/Src/stm32h7xx_it.c              Interrupt handlers
 - FMC/FMC_SDRAM/Src/system_stm32h7xx.c          STM32H7xx system clock configuration file 
    

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ device.
    
  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


