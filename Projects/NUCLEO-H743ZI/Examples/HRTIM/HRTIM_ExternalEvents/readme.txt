/**
  @page HRTIM_ExternalEvents HRTIM External Events example
  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_ExternalEvents/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HRTIM set/reset outputs with external events
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

This example shows how to use the External Event as source of set and reset of the HRTIM.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

HRTIM_TIMA pins TA1 and TA2. TIMA TA1/TA2 set and reset sources are selected to be the EEV8.
So if a low level is detected on EEV8 pin, TA1/TA2 will go to low level 
and if a high level is detected on EEV8 pin, TA1/TA2 will be at high level.

An external square signal is connected to the EEV8 pin (PB.06), the signal generated on TA1/TA2
is identical to this external square signal.

HRTIM1 TIMA frequency and duty cycles are the same of the external signal connected to EEV8 (PB.06).

The PWM waveforms can be displayed using an oscilloscope.

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

HRTIM, Timer, Externel event, EEV8, Source, Signal generator, Waveforms, PWM, Oscilloscope

@par Directory contents 

  - HRTIM/HRTIM_ExternalEvents/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_ExternalEvents/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HRTIM/HRTIM_ExternalEvents/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_ExternalEvents/Src/stm32h7xx_it.c          Interrupt handlers
  - HRTIM/HRTIM_ExternalEvents/Src/main.c                  Main program
  - HRTIM/HRTIM_ExternalEvents/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - HRTIM/HRTIM_ExternalEvents/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-H743ZI 
    board and can be easily tailored to any other supported device 
    and development board.      

  - NUCLEO-H743ZI Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - HRTIM1_TA1 : PC.06 (pin 04 in CN12 connector)
        - HRTIM1_TA2 : PC.07 (pin 19 in CN12 connector)

   Connect the EEV8 pin (PB.06(pin 17 in CN12 connector)) to an external square signal.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
