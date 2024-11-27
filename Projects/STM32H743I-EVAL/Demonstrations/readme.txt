/**
  @page Demonstrations  STM32H743I-EVAL Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H743I-EVAL Full Demonstration
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

@par Demonstration Overview

The STM32 H7 demonstration is running on STM32H743I EVAL boards 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals and initialize the Flash interface.
The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as time base for the HAL drivers.
This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms. 
The systick is then used for FreeRTOS time base.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

Tow flavours of the Demonstration binaries are available: 
  - 'STemWinDemo_STM32H743I-EVAL_VX.Y.Z.hex': based on StemWin and which source code 
     is provided within the STM32Cube_FW_H7 package.
  - 'STM32CubeDemo_STM32H743-EVAL_DEMO_V1.2.0_FULL.hex' : an out of the box Demo integrating the StemWin
    demo in addition to third parties graphical Demo modules: 
	   - TouchGFX demonstration module based on TouchGFX graphic library.
	     Free evaluation version is available at www.touchgfx.com.
	   - Embedded Wizard demonstration module from TARA systems.
         Free evaluation version is available at www.embedded-wizard.de.
		 
Below you find an overview of the different offered modules in the StemWin, TouchGFX and Embedded Wizard demonstrations:
  
+ STemWin demonstraion:

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
 
 Only the AVI files format with 640x480 pixels as resolution are supported.
 
 ++ Clock & Weather
 ------------------
 The clock & Weather module allows to show and adjust the time and date by
 changing the real time configuration (RTC).

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
 
 + System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.  

+ Embedded Wizard demonstration:
 
 ++ Video Player
 ---------------
 The video player module provides a video solution based on the
 STM32H7xxx and the Embedded Wizard APIs. It supports the playing
 movie in AVI format.
 
 The video files need to be named:
 video0.avi
 video1.avi
 video2.avi
 video3.avi

 ++ Screen Saver
 ---------------
 The screen saver is used to display a rotating clock that displays the
 current time while it is spinning around the three axis (x, y and z).
 The spinning operation uses intensive CPU calculation to show case the
 CPU capabilities.

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
 
 ++ TapTap Plane Game
 --------------------
 Control a plane by touching anywhere on the screen. The player has to collect 
 the maximum number of stars to get the best score.
 To show case the processing capabilities of STM32H7 the user can enable or disable 
 the Chrom-ART by pressing on the Chrom-ART button at the bottom right of the screen. 
 
 ++ Wave Form Generator
 ----------------------
 The Wave form generator module show case the possibility to emulate
 wave form generation and signal frequency display
 
 ++ Graphic accelerator
 ---------------------- 
 The Graphic accelerator module is used to showcase the CHROM-ART acceleration 
 capabilities and how it offloads the CPU.
 5 operations are used:
 - Alpha8 blend
 - Rectangle copy
 - Bitmap copy
 - Rectangle blend
 - Bitmap blend 
 
TouchGFX demonstration:

 ++ Video Player
 ---------------
 The video player module provides a video solution based on the
 STM32H7xxx and the Touch GFX APIs. It supports the playing movie
 in AVI format.
 
 ++ Audio Player
 ---------------
 The audio player module provides a complete audio solution based on
 the STM32H7xxx and delivers a high-quality music experience. It
 supports playing music in WAV format but may be extended to support
 other compressed formats such as MP3 and WMA audio formats.

 ++ Time & Calendar
 ------------------
 The Time & Calendar module allows to show and adjust the time and
 date by changing the real time configuration (RTC).
 
  ++ Home Control
 ----------------
 The Home Control module allows to Control home and shows vivid
 graphs and access control.
 To run statistics modules a password is needed: "L"

  ++ Light Effect
 ----------------
 The Light Effect module shows the STM32H7 microcontroller
 calculation capabilities via an advanced controlled light effect.

  ++ External Hardware
 ---------------------
 The External Hardware module allows to control the graphic via the
 potentiometer and adjust the screen brightness.

  ++ Knight Hit Zombie Game
 --------------------------
 The Knight Hit Zombie Game shows the graphic performance of the
 Chrom-ART and TouchGFX graphical stack.

  ++ 2048 Puzzle Game
 --------------------
 The 2048 Puzzle Game shows high quality graphics and smooth
 animations.
      
@par Keywords

Demonstration, FreeRTOS, RTOS, WAV, Video Player, AVI, JPEG, MJPEG, Audio player, Graphics,
Game, System, CPU, SAI, QSPI, SD, 

@par Hardware and Software environment

  - This demonstration runs on STM32H743xx/STM32H753xx devices.
    
  - This demonstration has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
     
@par How to use it ? 

The QSPI external flash loader is not integrated with supported toolchains, it’s only supported with STM32CubeProgrammer
To load the demonstration, use STM32CubeProgrammer to program both internal Flash and external QSPI memory.

In order to program the demonstration you must do the following:
1- Open STM32CubeProgrammer V1.3.0 or later, go to "External Loader" section and select the 
   "MT25TL01G_STM32H7xxI-EVAL" box 
2- Connect the STM32H743I-EVAL board to PC with USB cable through CN23
3- Use "STM32CubeDemo_STM32H743I-EVAL_VX.Y.Z.hex" file provided under “Binary” with STM32CubeProgrammer
   to program both internal Flash and external QSPI memory.

The binary and Media demonstration files are available from the STM32H743I-EVAL page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h743i-eval.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h743i-eval_demo.zip
   
4- copy the audio and video files provided under "/Media" in the SDCard
5- Plug the SDCard on CN13 connector
-> The internal Flash and the external QSPI are now programmed and the demonstration is shown on the board.


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
 
