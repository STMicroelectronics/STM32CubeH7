/**
  @page TIM_OCToggle TIM_OCToggle example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OCToggle/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four different 
  *          signals with four different frequencies.
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

  This example shows how to configure the TIM peripheral to generate four different 
  signals with four different frequencies.

 At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz
  
  The TIM1 frequency is set to 2*PCLK2 = HCLK = SystemCoreClock/2 (HZ), and the objective is
  to get TIM1 counter clock at 20 MHz so the Prescaler is computed as following:
     - Prescaler = (TIM1CLK /TIM1 counter clock) - 1

  The TIM1 CCR1 register value is equal to 40961:
  CC1 update rate = TIM1 counter clock / CCR1_Val = 488.269 Hz,
  so the TIM1 Channel 1 generates a periodic signal with a frequency equal to 244.13 Hz.

  The TIM1 CCR2 register value is equal to 20480:
  CC2 update rate = TIM1 counter clock / CCR2_Val = 976.56 Hz,
  so the TIM1 channel 2 generates a periodic signal with a frequency equal to 488.28 Hz.

  The TIM1 CCR3 register value is equal to 10240:
  CC3 update rate = TIM1 counter clock / CCR3_Val = 1953.1 Hz,
  so the TIM1 channel 3 generates a periodic signal with a frequency equal to 976.56 Hz.

  The TIM1 CCR4 register value is equal to 5120:
  CC4 update rate = TIM1 counter clock / CCR4_Val =  3906.25 Hz,
  so the TIM1 channel 4 generates a periodic signal with a frequency equal to 1953.12 Hz.

@note PWM signal frequency values mentioned above are theoretical (obtained when the system clock frequency
      is exactly 400 MHz). Since the generated system clock frequency may vary from one board to another observed
      PWM signal frequency might be slightly different.
	  
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

Timer, Output, signals, Output compare toggle, PWM, Oscilloscope

@par Directory contents 

  - TIM/TIM_OCToggle/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCToggle/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - TIM/TIM_OCToggle/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCToggle/Src/stm32h7xx_it.c          Interrupt handlers
  - TIM/TIM_OCToggle/Src/main.c                  Main program
  - TIM/TIM_OCToggle/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCToggle/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-H743ZI 
    board and can be easily tailored to any other supported device 
    and development board.      

  - NUCLEO-H743ZI Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  (PA.08 Pin23 of CN12)
      - TIM1_CH2  (PA.09 Pin21 of CN12)
      - TIM1_CH3  (PA.10 Pin33 of CN12)   
      - TIM1_CH4  (PE.14 Pin51 of CN12)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
