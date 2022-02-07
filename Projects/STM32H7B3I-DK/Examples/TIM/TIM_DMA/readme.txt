/**
  @page TIM_DMA TIM DMA example
  
  @verbatim
  ******************************************************************************
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

Use of the DMA with TIMER Update request 
to transfer data from memory to TIMER Capture Compare Register 3 (TIMx_CCR3).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

  The following configuration values are used in this example:

    - TIM8CLK = SystemCoreClock
    - Counter repetition = 3 
    - Prescaler = 0 
    - TIM8 counter clock = SystemCoreClock
    - SystemCoreClock is set to 280 MHz for STM32H7xx

  The objective is to configure TIM8 channel 3 to generate PWM
  (Pulse Width Modulation) signal with a frequency equal to 17.57 KHz, and a variable 
  duty cycle that is changed by the DMA after a specific number of Update DMA request.

  The number of this repetitive requests is defined by the TIM8 Repetition counter,
  each 4 Update Requests, the TIM8 Channel 3 Duty Cycle changes to the next new 
  value defined by the aCCValue_Buffer.
  
  The PWM waveform can be displayed using an oscilloscope.
  
  In nominal mode (except at start), it should look like this :
    
       .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   
        ___________     _______         ___             ___________    __
      _|           |___|       |_______|   |___________|           |__|             
       <----57us -----><----57us -----><----57us -----><----57us ----->



@note PWM signal frequency value mentioned above is theoretical (obtained when the system clock frequency 
      is exactly 280 MHz). Since the generated system clock frequency may vary from one board to another, the observed
      PWM signal frequency might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Timers, DMA, PWM, Frequency, Duty Cycle, Waveform, Oscilloscope, Output, Signal

@par Directory contents 

  - TIM/TIM_DMA/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_DMA/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - TIM/TIM_DMA/Inc/main.h                  Header for main.c module  
  - TIM/TIM_DMA/Inc/stm32h7b3i_discovery_conf.h  STM32H7B3I-DK board configuration file
  - TIM/TIM_DMA/Src/stm32h7xx_it.c          Interrupt handlers
  - TIM/TIM_DMA/Src/main.c                  Main program
  - TIM/TIM_DMA/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_DMA/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.
  - In this example, the clock is set to 280 MHz.
    
  - This example has been tested with STMicroelectronics STM32H7B3I-DK 
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32H7B3I-DK Set-up
    - Connect the TIM8 pin to an oscilloscope to monitor the different waveforms: 
    - TIM8 CH3 (PI.7) is connected to pin 2 (D9) on CN10 Connector

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
