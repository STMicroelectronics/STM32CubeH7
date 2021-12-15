/**
  @page STemWin_SampleDemo Readme file
 
  @verbatim
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

@par Application Description

This directory contains a set of source files that implement demo based on STemWin 
for STM32H7xx devices.

The list of modules to be used is configured into the file Demo/GUIDEMO.h

At the first run, after power OFF/ON, a touch screen calibration is needed: in
order to this, user is just requested to follow the displayed instructions.
Then all the demo modules are run sequentially. Throughout the demonstration, a
control frame window is displayed at the bottom right of the touch screen: it
includes a progress bar and two control buttons (Next + Halt) allowing to skip
or pause the currently running module.

 At the beginning of the main program the HAL_Init() function is called to reset 
 all the peripherals, initialize the Flash interface and the systick.
 The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
 The CPU at 400MHz 
 The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
	   
@par Keywords

Graphics, Display, STemWin, LTDC, LCD, GUI, Touch screen

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

@par Directory contents 

  - STemWin/SampleDemo/Inc/GUIConf.h                        	   Header for GUIConf.c
  - STemWin/SampleDemo/Inc/LCDConf.h                        	   Header for LCDConf*.c
  - STemWin/SampleDemo/Inc/rtc.h                            	   Header for rtc.c
  - STemWin/SampleDemo/Inc/calibration.h                    	   Header for calibration.c
  - STemWin/SampleDemo/Inc/main.h                           	   Main program header file
  - STemWin/SampleDemo/Inc/stm32h7xx_hal_conf.h             	   Library Configuration file
  - STemWin/SampleDemo/Inc/stm32h7xx_it.h                   	   Interrupt handlers header file
  - STemWin/SampleDemo/Src/rtc.c                            	   Main file to configure RTC clock
  - STemWin/SampleDemo/Src/calibration.c                    	   Main file to calibrate TS
  - STemWin/SampleDemo/Src/GUIConf.c                        	   Display controller initialization file
  - STemWin/SampleDemo/Src/LCDConf.c                        	   Configuration file for the GUI library
  - STemWin/SampleDemo/Src/main.c                           	   Main program file
  - STemWin/SampleDemo/Src/stm32h7xx_it.c                   	   STM32H7xx Interrupt handlers
  - STemWin/SampleDemo/Demo/GUIDEMO_*.c                     	   All the sources files provided for SEGGER DEMO
  - STemWin/SampleDemo/Demo/GUIDEMO.h                       	   DEMO modules configuration file
  - STemWin/SampleDemo/common/Src/system_stm32h7xx.c               STM32H7xx system file

@par Hardware and Software environment 

  - This application runs on STM32H750xx devices.
  - This application has been tested with STMicroelectronics STM32H750B-DISCO
    discovery boards.


In order to make the program work, you must do the following :

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc template.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this application application (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load this STemWin_HelloWorld application to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file relative to this application
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000) 

 */
