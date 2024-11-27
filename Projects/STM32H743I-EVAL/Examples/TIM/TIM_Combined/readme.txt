/**
  @page TIM_Combined TIM combined example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_Combined/readme.txt 
  * @author  MCD Application Team
  * @brief   TIM Combined Example Description.
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

This example shows how to configure the TIM1 peripheral to generate 3 PWM combined 
signals with TIM1 Channel5.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)=> TIM1CLK = 2*PCLK2
TIM1CLK = 2*PCLK2, Prescaler = 0, TIM1 counter clock = 2*PCLK2

The objective is to generate 3 combined PWM signal at 8.78 KHz (in center aligned mode):
    - TIM1_Period = ((2*PCLK2) / (8.78*2)) - 1
 The channel 1  duty cycle is set to 50%.
 The channel 2  duty cycle is set to 37.5%.
 The channel 3  duty cycle is set to 25%.
               
The Timer pulse is calculated as follows:
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  
The channel 5  is used in PWM2 mode with duty cycle set to 6.22%.

The 3 resulting signals are made of an AND logical combination of two reference PWMs:
  - Channel 1 and Channel 5
  - Channel 2 and Channel 5
  - Channel 3 and Channel 5

The TIM1 waveform can be displayed using an oscilloscope.
LED3 is ON when there are an error

                    ______   ______                  _______   ______
                   |      | |      |                |       | |      |
TIM1_CH1(PA.08)____|      |_|      |________________|       |_|      |__________
                      ____   ____                       ____   ____             
                     |    | |    |                     |    | |    |
TIM1_CH2(PA.09)______|    |_|    |_____________________|    |_|    |___________
                        __   __                           __   __                 
                       |  | |  |                         |  | |  |
TIM1_CH3(PA.10)________|  |_|  |_________________________|  |_|  |______________

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

Timer, PWM, combined signal, Duty Cycle, Waveform, Oscilloscope, Output, Signal

@par Directory contents 

  - TIM/TIM_Combined/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_Combined/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - TIM/TIM_Combined/Inc/main.h                  Header for main.c module  
  - TIM/TIM_Combined/Src/stm32h7xx_it.c          Interrupt handlers
  - TIM/TIM_Combined/Src/main.c                  Main program
  - TIM/TIM_Combined/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_Combined/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics STM32H743I-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32H743I-EVAL Set-up
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
       - PA.08 (TIM1_CH1)(CN7 pin52)
       - PA.09 (TIM1_CH2)(CN7 PIN 46)
       - PA.10 (TIM1_CH3)(CN7 PIN 47)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
