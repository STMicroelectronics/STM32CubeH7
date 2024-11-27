/**
  @page DAC_SimpleConversion DAC Simple Conversion example
  
  @verbatim
  ******************************************************************************
  * @file    DAC/DAC_SimpleConversion/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DAC Simple Conversion example.
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

This example provides a short description of how to use the DAC peripheral to 
do a simple conversion.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
- The CPU at 280MHz
- The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
- The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

The DAC peripheral is used to perform a simple conversion in 8 bits right alignment 
of 0x7F value, the result of the conversion can be seen by connecting PA4(DAC1 channel1) to an oscilloscope.
The observed value is 1.63V.
      

STM32H7B3I-EVAL board's LEDs can be used to monitor the process status:
  - LED3 is ON and example is stopped (using infinite loop)
  when there is an error during process.


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7”

@par Keywords

Analog, DAC, Conversion, Voltage output, Oscilloscope

@par Directory contents 

  - DAC/DAC_Simple_Conversion/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - DAC/DAC_Simple_Conversion/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - DAC/DAC_Simple_Conversion/Inc/main.h                  Header for main.c module  
  - DAC/DAC_Simple_Conversion/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I_EVAL board configuration file
  - DAC/DAC_Simple_Conversion/Inc/mx25lm51245g_conf.h     MX25LM51245G QSPI memory Description file
  - DAC/DAC_Simple_Conversion/Src/stm32h7xx_it.c          DMA interrupt handlers
  - DAC/DAC_Simple_Conversion/Src/main.c                  Main program
  - DAC/DAC_Simple_Conversion/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - DAC/DAC_Simple_Conversion/Src/system_stm32h7xx.c      STM32H7xx system source file
  

@par Hardware and Software environment  
  - This example runs on STM32H7b3xx devices.
    
  - This example has been tested with STM32H7B3I-EVAL  board and can be
    easily tailored to any other supported device and development board.

  - STM32H7B3I-EVAL  Set-up 	
    
      - Connect PA4 (DAC1 Channel1)(pin 21 in CN6) on STM32H7B3I-EVAL board to an oscilloscope.
      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */

