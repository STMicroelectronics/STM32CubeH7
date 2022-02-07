/**
  @page HAL_TimeBase_TIM HAL Time base example
  
  @verbatim
  ******************************************************************************
  * @file    HAL/HAL_TimeBase_TIM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HAL time base example.
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

How to customize HAL using a general-purpose timer as main source of time base 
instead of Systick.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

In this example the used timer is TIM6.

Time base duration is kept unchanged: 1ms  since PPP_TIMEOUT_VALUEs are defined 
and handled in milliseconds basis.

The example brings, in user file, a new implementation of the following HAL weak functions:

HAL_InitTick() 
HAL_SuspendTick()
HAL_ResumeTick()

This implementation will overwrite native implementation in stm32h7xx_hal.c
and so user functions will be invoked instead when called.

The following time base functions are kept as implemented natively:

HAL_IncTick()
HAL_Delay()

When user pushes the User Button, the Tick increment is suspended if it is already
enabled, else it will be resumed.
In an infinite loop, LED1 toggles spaced out over 1s delay, except when tick increment is suspended.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in TIM6 ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the TIM6 interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the TIM6 interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the TIM6 time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

System, General purpose Timer, Time base, HAL

@par Directory contents  

  - HAL/HAL_TimeBase_TIM/Inc/stm32h7xx_hal_conf.h         HAL configuration file
  - HAL/HAL_TimeBase_TIM/Inc/stm32h7xx_it.h               Interrupt handlers header file
  - HAL/HAL_TimeBase_TIM/Inc/main.h                       Header for main.c module  
  - HAL/HAL_TimeBase_TIM/Inc/stm32h735g_discovery_conf.h       STM32H735G-DK board configuration file
  - HAL/HAL_TimeBase_TIM/Src/stm32h7xx_it.c               Interrupt handlers
  - HAL/HAL_TimeBase_TIM/Src/main.c                       Main program
  - HAL/HAL_TimeBase_TIM/Src/stm32h7xx_hal_timebase_tim.c HAL time base functions
  - HAL/HAL_TimeBase_TIM/Src/system_stm32h7xx.c           STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H735xx devices.
    
  - This example has been tested with STMicroelectronics STM32H735G-DK board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
