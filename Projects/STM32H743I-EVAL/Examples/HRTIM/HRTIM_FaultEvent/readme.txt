/**
  @page HRTIM_FaultEvent HRTIM Fault Event example
  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_FaultEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Fault event generation in PWM mode with HRTIM1 
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

This example shows how to configure the HRTIM peripheral in PWM mode and how to 
configure an use the Fault event.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals 
and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals 
and D3 Domain APB4 peripherals to  run at 100MHz

In this example HRTIM1 input clock (HRTIM1CLK) is set to APB2 clock (PCLK2),
since APB2 prescaler is equal to 2.
  HRTIM1CLK = 2*PCLK2
  PCLK2 = HCLK/2

   => HRTIM1CLK = HCLK = SystemCoreClock/2

The PWM waveforms can be displayed using an oscilloscope.

    HRTIM1 TIMA period is set to 65535, so the frequency of the TIMA PWM is 3.06KHz
    HRTIM1 TIMA TA1 duty cycle = (HRTIM1_TIMA_CMP1/ HRTIM1_TIMA_PER)* 100 = 50%
    HRTIM1 TIMA TA2 duty cycle = (HRTIM1_TIMA_CMP2/ HRTIM1_TIMA_PER)* 100 = 75%

The PWM waveforms can be displayed using an oscilloscope, when the level of Fault2 pin(PC.11) 
is low( connected to GND) the PWM signals are generated, if the level of the Fault pin switchs to High
(3.3V) the PWM signals will be cleared.

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

HRTIM, Timer, Fault event, Waveforms, PWM, Oscilloscope

@par Directory contents 

  - HRTIM/HRTIM_FaultEvent/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_FaultEvent/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HRTIM/HRTIM_FaultEvent/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_FaultEvent/Src/stm32h7xx_it.c          Interrupt handlers
  - HRTIM/HRTIM_FaultEvent/Src/main.c                  Main program
  - HRTIM/HRTIM_FaultEvent/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - HRTIM/HRTIM_FaultEvent/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics STM32H743I-EVAL 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32H743I-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - HRTIM1_TIMA_TA1 : PC.06 (Pin45 in CN7 connector)
        - HRTIM1_TIMA_TA2 : PC.07 (Pin42 in CN7 connector)

   Connect the Fault2 pin (PC.11(pin40 in CN7 connector)) to GND, to generate the fault event, connect this pin to 3.3V.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
