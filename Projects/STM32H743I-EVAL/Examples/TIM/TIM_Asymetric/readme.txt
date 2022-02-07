/**
  @page TIM_Asymetric TIM Asymetric example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    TIM/TIM_Asymetric/readme.txt 
  * @author  MCD Application Team
  * @brief   TIM Asymetric Example Description.
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

This example shows how to configure the TIM peripheral to generate an Asymetric signal.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices(SystemCoreClock) :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals 
and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals 
and D3 Domain APB4 peripherals to  run at 100MHz

TIM8 is configured to generate an Asymetric signal with a programmable Phase-Shifted signal on TIM8_CH2:
   - TIM8 Channel 1 is configured in PWM2 mode
   - TIM8 Channel 2 is configured in Asymetric PWM2 mode
   - The counter mode is center aligned mode
   - The pulse length and the phase shift are programmed consecutively in TIM8_CCR2 and TIM8_CCR1.

TIM1 is configured to generating the reference signal on Channel1 used by TIM8:
   - TIM1 is generating a PWM signal with frequency equal to 4.54 KHz.
   - TIM1 is used as master for TIM8, the update event of TIM1 genarates the Reset counter
     of TIM8 to synchronize the two signals: the refernce signal (TIM1_CH1) and the shifted signal (TIM8_CH2). 

In this example TIM1 and TIM8 input clock (TIM8CLK) is set to APB2 clock (2*PCLK2)     
TIM1 and TIM8 signals are at frequency of  ((SystemCoreClock/2) / (Period + 1))  
               
TIM8 is generating a signal with the following caracteristics:
  - Pulse length = (TIM8_CCR1 + TIM8_CCR2) / TIM8_CLK
  - Phase shift = TIM8_CCR2/TIM8_CLK
 with TIM8_CLK = ((SystemCoreClock/2) / (Period + 1)), as the prescaler is equal to zero. 
 The Pulse is low level of the output signal of Channel 2 because TIM cannel is configured as Asymetric PWM2 mode.

LED3 is ON when there are an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Timer, PWM, Asymetric signal, Duty Cycle, Waveform, Oscilloscope, Output, Signal, commutation, timing mode

@par Directory contents 

  - TIM/TIM_Asymetric/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_Asymetric/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - TIM/TIM_Asymetric/Inc/main.h                  Header for main.c module  
  - TIM/TIM_Asymetric/Src/stm32h7xx_it.c          Interrupt handlers
  - TIM/TIM_Asymetric/Src/main.c                  Main program
  - TIM/TIM_Asymetric/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_Asymetric/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics STM32H743I-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32H743I-EVAL Set-up
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
       - PA.08 (TIM1_CH1)(CN7 pin 52)
       - PC.06 (TIM8_CH1)(CN7 pin 45)
       - PC.07 (TIM8_CH2)(CN7 pin 42) 
    The shift is measured using the TIM1_CH1 as reference signal.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
