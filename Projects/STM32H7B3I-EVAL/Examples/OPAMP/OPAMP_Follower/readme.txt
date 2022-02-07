/**
  @page OPAMP_Follower Follow an applied signal example  
  @verbatim
  ********************* COPYRIGHT(c) 2019 STMicroelectronics *******************
  * @file    OPAMP/OPAMP_Follower/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the OPAMP Follower signals generation example.
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

This example shows how to configure OPAMP peripheral in follower mode interconnected 
with DAC and COMP.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7b3xx Devices :
- The CPU at 280MHz
- The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
- The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.
 
OPAMP1 is configured as following:
   - Follower mode.
   - OPAMP Non Inverting input is connected internally to DAC_OUT1 that perform a simple
     conversion in 8 bits right alignment of 0x7F value.
   - OPAMP Output is connected to COMP1 inverting input internally (share same pin PC4).

Test Description:
   
   - Push the Tamper button to enter normal mode( LED3 On).
   - Push the Tamper alternatively to switch from normal power mode to high speed mode 
     (LED2 On)and vice versa.
   - Inject voltage higher than 1.65V to COMP1 non inverting input( PB2 ). 
   - check COMP1 output(PA8).
   - If COMP1 output is high test PASS otherwise test FAIL.
   

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

Analog, OPAMP, follower, DAC, Signals generation, Waveform, Amplitude

@par Directory contents 

  - OPAMP/OPAMP_Follower/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_Follower/Inc/stm32h7xx_it.h          Header for stm32h7xx_it.c
  - OPAMP/OPAMP_Follower/Inc/main.h                  Header for main.c  
  - OPAMP/OPAMP_Follower/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I_EVAL board configuration file
  - OPAMP/OPAMP_Follower/Inc/mx25lm51245g_conf.h     MX25LM51245G QSPI memory Description file
  - OPAMP/OPAMP_Follower/Src/stm32h7xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_Follower/Src/main.c                  Main program
  - OPAMP/OPAMP_Follower/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_Follower/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32H7b3xx devices.
    
  - This example has been tested with STM32H7B3I-EVAL  board and can be
    easily tailored to any other supported device and development board..

  - STM32H7B3I-EVAL  Set-up
    
      - Inject Volatge in COMP1 non inverting input( PB2 )  
      - Connect an oscilloscope to (COMP1_OUT, PA8)
      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */

