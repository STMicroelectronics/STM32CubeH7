/**
  @page STemWin   STM32H7B3I-DK STemWin Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    STemWin/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H7B3I-DK STemWin Demonstration
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

@par Demo Description

The STM32Cube Demonstration platform comes on top of the STM32CubeTM as a firmware
package that offers a full set of software components based on a modules architecture
allowing re-using them separately in standalone applications. All these modules are
managed by the STM32Cube Demonstration kernel allowing to dynamically adding new
modules and access to common resources (storage, graphical components and widgets,
memory management, Real-Time operating system)

The STM32Cube Demonstration platform is built around the powerful graphical library
STemWin and the FreeRTOS real time operating system and uses almost the whole STM32
capability to offer a large scope of usage based on the STM32Cube HAL BSP and several
middleware components.

@par STemWin Demonstration Overview

The STM32 H7 STemWin demonstration is running on STM32H7B3I-DK boards 

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.
	   
Below you find an overview of the different offered modules in the STemWin demonstration:
 ++ Graphical Effect
 -------------------
 The Graphic effect module demonstrates the platform’s computing capabilities
 to render a real-time effect at full screen resolution.
 • The implemented filters are
  - Edge detection filter
  - Smoothing filter
  - Sharping filter
  - Raising filter
  - Motion blur filter
 The CPU load metrics are displayed on the middle of top screen. 
 
 ++ Rocket Game
 --------------
 The Rocket Game shows the graphic performance of the Chrom-ART.
 Control the Rocket by moving it on the screen. The player has to collect
 the maximum number of coins to get the best score.
 For demonstration not touching the rocket will lead to a game that is not ending.
 The CPU load metrics are displayed on the middle of top screen.
 
 + System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.  

@par Keywords

Demonstration, STemWin, FreeRTOS, RTOS, Graphics, Game, System, CPU, OSPI 

@par Hardware and Software environment

  - This demonstration runs on STM32H7B3xx devices.
    
  - This demonstration has been tested with STMicroelectronics STM32H7B3I-DK
    boards and can be easily tailored to any other supported device 
    and development board.
     
@par How to use it ? 

To load the demonstration, use STM32CubeProgrammer to program both internal Flash and external OSPI memory.

In order to program the demonstration you must do the following:
2- Open STM32CubeProgrammer, go to "External loaders" menu then check "MX25LM51245G_STM32H7B3I_DISCO" box
3- Connect the STM32H7B3I-DK board to PC with USB cable through CN14 and click on STM32CubeProgrammer "Connect" button
4- Use the generated hex file  with STM32CubeProgrammer to program both internal Flash and external OSPI memory:
   - From Erasing and programming Menu browse to select the hex file
     then click on start programming
   
   You can also use the script "program_hexfile.bat" (available within the binaries package) to directly program this hex file into 
   the STM32H7B3I-DK board.
   This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H7B3I_DISCO)
   
5- copy the BMP files format with 480x272 pixels as resolution provided under "/Media/Pictures" in a SD card storage at BMP folder

Below is the listing of BMP file :
    BMP
    \---
       |   image06.bmp
       |   image07.bmp
       |   image08.bmp
	   
6- Plug the Media storage in CN4.
-> The internal Flash and the external OSPI are now programmed and the demonstration is shown on the board.


The binary and Media demonstration files are available from the STM32H7B3I-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/STM32H7B3I-DK.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h7b3i-dk_demo.zip

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case, it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"



 */
 
