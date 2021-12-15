/**
  @page OPAMP_PGA_ExternalBias Amplify the input signal with bias voltage for non inverting mode.  
  @verbatim
  ********************* COPYRIGHT(c) 2017 STMicroelectronics *******************
  * @file    OPAMP/OPAMP_PGA_ExternalBias/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the OPAMP PGA with Bias voltage example.
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

This example shows how to configure OPAMP peripheral in PGA mode with bias voltage for 
the non inverting mode. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
- The CPU at 400MHz 
- The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
- The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz
 
OPAMP1 is configured as following:
   - PGA mode.
   - OPAMP Non Inverting input is connected to DAC_OUT1 that injects a 0.5V internally.
   - OPAMP Inverting input connected to an external voltage called bias voltage.
   - OPAMP Output is connected to an oscilloscope to observe the variation of output voltage.

Test Description:

The purpose of the test is to amplify the amplitude of an input signal with bias voltage for 
non inverting mode.
User will push User button to switch between the non inverting gain 2 and 4.
Meanwhile, the user will try to modify the voltage of the bias and observe the variation of
the opamp output.
  
   - Push the User Push button to set the programmable gain amplifier to 2. 
   - Push the User alternatively to switch from gain equals to 2 to 4 and vice versa.
   - OPAMP1 Non Inverting input(VP) is connected internally to DAC_OUT1 that injects a 0.5V.
   - Inject a bias voltage to OPAMP1 Inverting input VN (PC5 pin 6 in CN12 connector)
   - check OPAMP1 output Vout(PC4 pin 34 in CN12 connector) with an oscilloscope, it will be equal to: 
   
                   Vout = Gain x VP +(1 - Gain) x  VN 
	Step 1: 	
	   * When Gain equals to 2, LED1 is ON , Connect VN(PC5 pin 6 of CN12) to GND:
	     If Vout(PC4 pin 34 in CN12 )  equals to 1V , Test PASS.
	   * When Gain equals to 4, LED1 is OFF,LED2 is ON , Keep the connection of VN(PC5 pin 6 of CN12) to GND:
	     If Vout(PC4 pin 34 in CN12 ) equals to 2V , Test PASS.
		 
	Step 2: 	
	   * When Gain equals to 2, LED1 is ON , Inject 1v to VN(PC5 pin 6 of CN12):
	     If Vout(PC4 pin 34 in CN12 ) equals to 0V , Test PASS.
	   * When Gain equals to 4, LED1 is OFF,LED2 is ON , Inject 0.5v to VN(PC5 pin 6 of CN12):
	     If Vout(PC4 pin 34 in CN12 ) equals to 0.5V , Test PASS.	 
        	    

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

Analog, OPAMP, PGA, Bias voltage, Amplify, Amplitude, DAC, Button, Led, Output, Voltage, Oscilloscope

@par Directory contents 

  - OPAMP/OPAMP_PGA_ExternalBias/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_PGA_ExternalBias/Inc/stm32h7xx_it.h          Header for stm32h7xx_it.c
  - OPAMP/OPAMP_PGA_ExternalBias/Inc/main.h                  Header for main.c  
  - OPAMP/OPAMP_PGA_ExternalBias/Src/stm32h7xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_PGA_ExternalBias/Src/main.c                  Main program
  - OPAMP/OPAMP_PGA_ExternalBias/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_PGA_ExternalBias/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board..

      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */

