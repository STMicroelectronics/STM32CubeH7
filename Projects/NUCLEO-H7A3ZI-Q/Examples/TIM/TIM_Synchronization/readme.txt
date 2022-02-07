/**
  @page TIM_Synchronization Timers Synchronization example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_Synchronization/readme.txt 
  * @author  MCD Application Team
  * @brief   How to command 2 Timers as slaves (TIM3 & TIM4) using a Timer
  *          as master (TIM1)

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

Synchronization of TIM1 and TIM3/TIM4 timers in Parallel mode.

Timers synchronisation in parallel mode:


                                                       ___________ 
                                                 ITR0 |  SLAVE 1  |
                                     _________________|    TIM3   |
   ___________                      |                 |___________|
  |   MASTER  |TRGO_Update          |                 
  |    TIM1   |---------------------|               
  |___________|                     |                  ___________ 
                                    |_________________|  SLAVE 2  |
                                                 ITR0 |    TIM4   |
                                                      |___________|
                         

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.
						 
1/ TIM1 is configured as Master Timer:
   - PWM Mode is used
   - The TIM1 Update event is used as Trigger Output 
 
2/ TIM3 and TIM4 are slaves for TIM1,
   - PWM Mode is used
   - The ITR0(TIM1) is used as input trigger for both slaves
   - Gated mode is used, so starts and stops of slaves counters
     are controlled by the Master trigger output signal(update event).

The TIM1 counter clock is 280 MHz.

  The Master Timer TIM1 is running at:
  TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 1.09375 MHz and
  a the duty cycle equal to: TIM1_CCR1/(TIM1_ARR + 1) = 50%

  The TIM3 is running at: 
  (TIM1 frequency)/ ((TIM3 period +1)* (TIM1_Repetition_Counter+1)) = 72.916 KHz and
  a duty cycle equal to TIM3_CCR1/(TIM3_ARR + 1) = 33.3%

  The TIM4 is running at:
  (TIM1 frequency)/ ((TIM4 period +1)* (Repetition_Counter+1)) = 109.375 KHz and
  a duty cycle equal to TIM4_CCR1/(TIM4_ARR + 1) = 50%


The PWM waveform can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Timers, PWM, Parallel synchronization, Master, Slave, Duty Cycle, Waveform,
Oscilloscope, Output, Signal,

@par Directory contents 

  - TIM/TIM_Synchronization/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_Synchronization/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - TIM/TIM_Synchronization/Inc/main.h                  Header for main.c module  
  - TIM/TIM_Synchronization/Inc/stm32h7xx_nucleo_conf.h  STM32H7xx_Nucleo board configuration file
  - TIM/TIM_Synchronization/Src/stm32h7xx_it.c          Interrupt handlers
  - TIM/TIM_Synchronization/Src/main.c                  Main program
  - TIM/TIM_Synchronization/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_Synchronization/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.
    
  - This example has been tested with Nucleo-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.      

  - Nucleo-H7A3ZI-Q Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PA.08)
      - TIM3 CH1 (PC.06)
      - TIM4 CH1 (PB.06)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 */
