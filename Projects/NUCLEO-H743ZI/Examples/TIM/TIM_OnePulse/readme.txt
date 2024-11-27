/**
  @page TIM_OnePulse TIM One Pulse example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM One Pulse example.      
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

This example shows how to use the TIMER peripheral to generate a single pulse when
a rising edge of an external signal is received on the TIMER Input pin.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz
Clock setup for TIM4

================================

  TIM4CLK = 2*PCLK2 = 200 MHz.
  
  Prescaler = (TIM4CLK /TIM4 counter clock) - 1
  
  The prescaler value is computed in order to have TIM4 counter clock 
  set at 20000000 Hz.
  
  The Autoreload value is 65535 (TIM4->ARR), so the maximum frequency value to 
  trigger the TIM4 input is 20000000/65535 [Hz].
 
Configuration of TIM4 in One Pulse Mode
===================================================
 
  - The external signal is connected to TIM4_CH2 pin (PD.13), 
    and a rising edge on this input is used to trigger the Timer.
  - The One Pulse signal is output on TIM4_CH1 (PD.12).

  The delay value is fixed to:
   - Delay =  CCR1/TIM4 counter clock 
           = 16383 / 20000000 [sec]
           
  The pulse value is fixed to : 
   - Pulse value = (TIM_Period - TIM_Pulse)/TIM4 counter clock  
                 = (65535 - 16383) / 20000000 [sec]

  The one pulse waveform can be displayed using an oscilloscope and it looks
  like this.
  
                                ___
                               |   |
  CH2 _________________________|   |__________________________________________
 
                                             ___________________________
                                            |                           |
  CH1 ______________________________________|                           |_____
                               <---Delay----><------Pulse--------------->
  


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

Timer, Output, signals, One Pulse, PWM, Oscilloscope, External signal, Autoreload, Waveform

@par Directory contents 

  - TIM/TIM_OnePulse/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OnePulse/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - TIM/TIM_OnePulse/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OnePulse/Src/stm32h7xx_it.c          Interrupt handlers
  - TIM/TIM_OnePulse/Src/main.c                  Main program
  - TIM/TIM_OnePulse/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OnePulse/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

   - This example runs on STM32H743xx devices.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H743ZI Set-up
   - Connect the external signal to the TIM4_CH2 pin (PD.13) (pin 41 in CN12 connector)
   - Connect the TIM4_CH1 pin(PD.12) (pin 43 in CN12 connector) to an oscilloscope to monitor the waveform.  


@par How to use it ? 

In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example 



 */
