/**
  @page STemWin   STM32H747I-EVAL STemWin Demonstration
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    STemWin/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H747I-EVAL Demonstration
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

The STM32 H7 STemWin demonstration is running on STM32H747I-EVAL boards 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals and initialize the Flash interface.
The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as time base for the HAL drivers.
This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms. 
The systick is then used for FreeRTOS time base.
The SystemClock_Config() function is used to configure the system clock for STM32H747xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

  
Below you find an overview of the different offered module in the STemWin demonstration:

 ++ Audio Player
 ---------------
 The audio player module provides a complete audio solution based on the
 STM32H7xxx and delivers a high-quality music experience. It supports playing
 music in WAV format but may be extended to support other compressed
 formats such as MP3 and WMA audio formats.
 
 ++ Video Player
 ---------------
 The video player module provides a video solution based on the STM32H7xxx
 and the STemWin movie APIs. It supports the playing movie in AVI format.
 
 Only the AVI files format with 800x480 pixels as resolution are supported.
 
 ++ Clock & Weather
 ------------------
 The clock & Weather module allows to show and adjust the time and date by
 changing the real time configuration (RTC).

 ++ Graphical Effect
 -------------------
 The Graphic effect module demonstrates the platforms computing capabilities
 to render a real-time effect at full screen resolution.
    The implemented filters are
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
 
 ++ DUAL CORE
 --------------
 The dual core module shows the difference between using dual core and single core.
 when dual core is activated, the fractal video works on CM4 and the rest of the 
 videos runs on CM7, and when dual core deactivated , all videos works on CM7.
 When the dual core activated, the fps will be higher and CPU load will be lower 
 than with a single core.
 
 + System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.  

@par Keywords

Demonstration, STemWin, multiprocessor, FreeRTOS, RTOS, WAV, Video Player, AVI, JPEG, MJPEG, Audio player, Graphics,
Game, System, CPU, SAI, QSPI, SD, 

@par Hardware and Software environment

  - This demonstration runs on STM32H747xx devices.
    
  - This demonstration has been tested with STMicroelectronics STM32H747I_EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
     
@par How to use it ? 

The QSPI external flash loader is not integrated with supported toolchains, its only supported with 
STM32CubeProgrammer.
To load the demonstration, use STM32CubeProgrammer to program both internal Flash and external QSPI memory.

In order to program the demonstration you must do the following:
1- Open STM32CubeProgrammer, click on setting icon from the bar menu then check
   "MT25TL01G_STM32H7xxI-EVAL" box 
2- Connect the STM32H747I-EVAL board to PC with USB cable through CN23
3- Use "STM32CubeDemo_STM32H747I-EVAL_VX.Y.Z.hex" file provided under Binary with STM32 STM32CubeProgrammer
   to program both internal Flash and external QSPI memory.
   
4- copy the audio and video files provided under "/Media" in a media storage
5- Plug/connect the Media storage: the SDCard on CN13 connector
-> The internal Flash and the external QSPI are now programmed and the demonstration is shown on the board.

The binary and Media demonstration files are available from the STM32H747I-EVAL page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h747i-eval.html

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



 */
 
