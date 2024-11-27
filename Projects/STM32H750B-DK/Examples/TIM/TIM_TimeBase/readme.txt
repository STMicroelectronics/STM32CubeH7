/**
  @page TIM_TimeBase Time Base example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM Time Base example
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

How to configure the TIM peripheral to generate a time base of 
one second with the corresponding Interrupt request.

In this example, the code is executed from QSPI external memory while data are in internal
SRAM memory.

In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1),
since APB1 prescaler is equal to 2.
  TIM3CLK = 2*PCLK1
  PCLK1 = HCLK/2 as AHB Clock divider is set to RCC_HCLK_DIV2
  --> TIM3CLK = HCLK
To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
  Prescaler = (TIM3CLK / TIM3 counter clock) - 1
  Prescaler = (HCLK /(10 KHz)) - 1

The TIM3 ARR register value is equal to 10000 - 1, 
Update rate = TIM3 counter clock / (Period + 1) = 1 Hz,
So the TIM3 generates an interrupt each 1 s

When the counter value reaches the auto-reload register value, the TIM update 
interrupt is generated and, in the handler routine, LED1 on board STM32H750B-DISCO
is toggled at the following frequency: 0.5Hz.

TIM3 time base is set to 1 second, as mentioned, which gives a toggling frequency
of 0.5 Hz.
It can be modified. In the example below, this time base is divided by 2 to obtain
a toggling frequency of 1 Hz:
    TimHandle.Init.Period            = (10000 - 1) / 2;

In case of error, LED2 is turned on.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clocks for STM32H750xx Devices:
 - The CPU clock at 400MHz 
 - The HCLK for D1 Domain, AXI and AHB3 peripherals, D2 Domain, AHB1/AHB2 peripherals, D3 Domain, and AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

This project is configured for STM32H750xx devices using STM32CubeH7 HAL and running on 
STM32H750B-DISCO board from STMicroelectronics.
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
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

Timer, TIM, Time base, Interrupt, Clock source

@par Directory contents

 - "TIM/TIM_TimeBase/Inc": contains the example firmware header files 
    - TIM/TIM_TimeBase/Inc/main.h                   Main configuration file
    - TIM/TIM_TimeBase/Inc/stm32h7xx_it.h           Interrupt handlers header file
    - TIM/TIM_TimeBase/Inc/stm32h7xx_hal_conf.h     HAL Configuration file 

 - "TIM/TIM_TimeBase/Src": contains the example firmware source files
    - TIM/TIM_TimeBase/Src/main.c                   Main program 
    - TIM/TIM_TimeBase/Src/stm32h7xx_hal_msp.c      Microcontroller specific packages initialization file.
    - TIM/TIM_TimeBase/Src/stm32h7xx_it.c           Interrupt handlers
    - TIM/TIM_TimeBase/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file


@par Hardware and Software environment  

  - This example runs on STM32H750xx devices.
  - This example has been tested with STMicroelectronics STM32H750B-DISCO
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this example(see below).
  4. Start debugging the example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load this example to the external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program :
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using STM32CubeIDE  :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" in case of XiP from QSPI
            - From Erasing & Programming menu, browse and open the output binary file relative to your example
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)
  

 */
