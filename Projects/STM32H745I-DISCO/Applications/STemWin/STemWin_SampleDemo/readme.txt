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

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
      case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
      registers (including the backup registers) and RCC_BDCR register are set to their reset values.
	   
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

  - STemWin/SampleDemo/CM7/Inc/GUIConf.h                        	   Header for GUIConf.c
  - STemWin/SampleDemo/CM7/Inc/LCDConf.h                        	   Header for LCDConf*.c
  - STemWin/SampleDemo/CM7/Inc/rtc.h                            	   Header for rtc.c
  - STemWin/SampleDemo/CM7/Inc/calibration.h                    	   Header for calibration.c
  - STemWin/SampleDemo/CM7/Inc/main.h                           	   Main program header file
  - STemWin/SampleDemo/CM7/Inc/stm32h7xx_hal_conf.h             	   Library Configuration file
  - STemWin/SampleDemo/CM7/Inc/stm32h7xx_it.h                   	   Interrupt handlers header file
  - STemWin/SampleDemo/CM7/Src/rtc.c                            	   Main file to configure RTC clock
  - STemWin/SampleDemo/CM7/Src/calibration.c                    	   Main file to calibrate TS
  - STemWin/SampleDemo/CM7/Src/GUIConf.c                        	   Display controller initialization file
  - STemWin/SampleDemo/CM7/Src/LCDConf.c                        	   Configuration file for the GUI library
  - STemWin/SampleDemo/CM7/Src/main.c                           	   Main program file
  - STemWin/SampleDemo/CM7/Src/stm32h7xx_it.c                   	   STM32H7xx Interrupt handlers
  - STemWin/SampleDemo/CM7/Demo/GUIDEMO_*.c                     	   All the sources files provided for SEGGER DEMO
  - STemWin/SampleDemo/CM7/Demo/GUIDEMO.h                       	   DEMO modules configuration file
  - STemWin/SampleDemo/common/Src/system_stm32h7xx.c               	   STM32H7xx system file
 
  - STemWin/SampleDemo/CM4/Inc/main.h                                  CM4 Main program header file 
  - STemWin/SampleDemo/CM4/Inc/stm32h7xx_hal_conf.h                    CM4 HAL configuration file
  - STemWin/SampleDemo/CM4/Inc/stm32h7xx_it.h                          CM4 Interrupt handlers header file
  - STemWin/SampleDemo/CM4/Src/main.c                                  CM4 main program
  - STemWin/SampleDemo/CM4/Src/stm32h7xx_it.c                          CM4 Interrupt handlers


@par Hardware and Software environment 

  - This application runs on STM32H745xx devices.
  - This application has been tested with STMicroelectronics STM32H745I-Disco
    discovery boards.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild CM7 and CM4 applications and load both images into target memory
 - reset the board to get the applications running.
 

 */
