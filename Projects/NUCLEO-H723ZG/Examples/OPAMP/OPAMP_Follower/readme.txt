/**
  @page OPAMP_Follower OPAMP used as follower
  @verbatim
  ******************************************************************************
  * @file    OPAMP/OPAMP_Follower/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the OPAMP follower example.
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

How to configure the OPAMP peripheral in follower mode interconnected
with DAC and COMP.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

OPAMP1 is configured as following:
   - Follower mode.
   - OPAMP Non Inverting input is connected internally to DAC_OUT1 that perform a simple
     conversion in 8 bits right alignment of 0x7F value.
   - OPAMP Output is connected to COMP1 inverting input internally (share same pin PC4).

Test Description:

   - Push the user Push button to enter normal mode( LED1 On).
   - Push the user alternatively to switch from normal power mode to high speed mode
     (LED2 On)and vice versa.
   - Inject voltage higher than 1.65V to COMP1 non inverting input(PB2 pin 15 in CN10).
   - check COMP1 output (PE12 pin 26 of CN10) .
   - If COMP1 output is high test PASS otherwise test FAIL.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set
      to 1 millisecond to have correct HAL operation.


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

Analog, OPAMP, follower, DAC, Signals generation, Waveform, Amplitude

@par Directory contents

  - OPAMP/OPAMP_InternalFollower/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_InternalFollower/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - OPAMP/OPAMP_InternalFollower/Inc/main.h                        Header for main.c module
  - OPAMP/OPAMP_InternalFollower/Inc/stm32h7xx_nucleo_conf.h      STM32H7xx_Nucleo board configuration file
  - OPAMP/OPAMP_InternalFollower/Src/stm32h7xx_it.c          DMA interrupt handlers
  - OPAMP/OPAMP_InternalFollower/Src/main.c                        Main program
  - OPAMP/OPAMP_InternalFollower/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_InternalFollower/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H723xx devices.

  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
