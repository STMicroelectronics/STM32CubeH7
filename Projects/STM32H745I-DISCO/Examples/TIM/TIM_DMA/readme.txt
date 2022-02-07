﻿/**
  @page TIM_DMA TIM DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    TIM/TIM_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TIM DMA example.
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

This example provides a description of how to use DMA with TIMER Update request
to transfer Data from memory to TIMER Capture Compare Register 3 (TIMx_CCR3).

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

This projects is configured for STM32H745xx devices using STM32CubeH7 HAL and running on 
STM32H745I-DISCO board from STMicroelectronics.

  The following configuration values are used in this example:

    - TIM3CLK = 2*PCLK2 = HCLK
    - Counter repetition = 3 
    - Prescaler = 0 
    - TIM3 counter clock = HCLK

  The objective is to configure TIM3 channel 1 to generate complementary PWM 
  (Pulse Width Modulation) signal with a frequency equal to 17.57 KHz, and a variable 
  duty cycle that is changed by the DMA after a specific number of Update DMA request.

  The number of this repetitive requests is defined by the TIM3 Repetition counter,
  each 4 Update Requests, the TIM3 Channel 1 Duty Cycle changes to the next new 
  value defined by the aCCValue_Buffer.
  
  The PWM waveform can be displayed using an oscilloscope.
 
  In nominal mode (except at start) , it should looks like this :
    
       .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   
        ___________     _______         ___             ___________    __
      _|           |___|       |_______|   |___________|           |__|             
       <----57us -----><----57us -----><----57us -----><----57us ----->

LED2 is ON when there are an error.

@note PWM signal frequency value mentioned above is theoretical (obtained when the system clock frequency 
      is exactly 400 MHz). Since the generated system clock frequency may vary from one board to another observed
      PWM signal frequency might be slightly different.

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.      

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, DMA, PWM, Frequency, Duty Cycle, Waveform, Oscilloscope, Output, Signal

@par Directory contents 

  - TIM/TIM_DMA/Common/Src/system_stm32h7xx.c      STM32H7xx system configuration source file
  
  - TIM/TIM_DMA/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M7
  - TIM/TIM_DMA/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M7
  - TIM/TIM_DMA/CM7/Inc/main.h                  Header for main.c module for Cortex-M7  
  - TIM/TIM_DMA/CM7/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M7
  - TIM/TIM_DMA/CM7/Src/main.c                  Main program for Cortex-M7

  - TIM/TIM_DMA/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - TIM/TIM_DMA/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - TIM/TIM_DMA/CM4/Inc/main.h                  Header for main.c module for Cortex-M4  
  - TIM/TIM_DMA/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - TIM/TIM_DMA/CM4/Src/main.c                  Main program for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H745xx devices.
    
  - This example has been tested with STM32H745I-DISCO board and can be
    easily tailored to any other supported device and development board.
    
  - STM32H745I-DISCO Set-up
    - Connect the TIM3 pin to an oscilloscope to monitor the different waveforms: 
    - TIM3 CH1 (PA.6 (D3 on CN6)).    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H745_DISCO_CM7 and STM32H745_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example



 */
