/**
  @page HRTIM_Arbitrary_Waveform HRTIM Arbitary Waveform example
  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_Arbitrary_Waveform/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HRTIM1 Arbitary waveform generation
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

This example shows how to configure the HRTIM1 peripheral to generate an arbitary signals.
In fact the reset source and set sources of the TIMD TD1 and TD2 can be a combination of several 
sources, in this example we select more than sources for set and reset in order 
to show the capability of our HRTIM to generate different waveforms.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

HRTIM1 TIMD is used in this example, the set source for TD1 output is selected to be:
the TIMD period, the compare unit 1 and the compare unit 2, the reset source are also
the same sources, so the result waveform will be a logical AND between the three sources.
The set source for TD2 output is selected to be: the TIMD period and the compare unit 2, 
the reset source are: the compare unit 1 and the compare unit 3
so the result waveform will be a combination between these sources.

In this example HRTIM1 input clock (HRTIM1CLK) is set to APB2 clock (PCLK2),
since APB2 prescaler is equal to 2.
  HRTIM1CLK = 2*PCLK2
  PCLK2 = HCLK/2

  => HRTIM1CLK = HCLK = SystemCoreClock/2

The waveforms can be displayed using an oscilloscope.

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

HRTIM, Timer, Logical operation, Set source, Reset source, Arbitrary, Waveforms, Oscilloscope

@par Directory contents 

  - HRTIM/HRTIM_Arbitrary_Waveform/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_Arbitrary_Waveform/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HRTIM/HRTIM_Arbitrary_Waveform/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_Arbitrary_Waveform/Src/stm32h7xx_it.c          Interrupt handlers
  - HRTIM/HRTIM_Arbitrary_Waveform/Src/main.c                  Main program
  - HRTIM/HRTIM_Arbitrary_Waveform/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - HRTIM/HRTIM_Arbitrary_Waveform/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-H743ZI 
    board and can be easily tailored to any other supported device 
    and development board.      

  - NUCLEO-H743ZI Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - HRTIM1_TIMD_TD1: PA.11 (pin 14 in CN12 connector with SB16 fitted on MB1364)
        - HRTIM1_TIMD_TD1: PA.12 (pin 12 in CN12 connector with SB17 fitted on MB1364)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
