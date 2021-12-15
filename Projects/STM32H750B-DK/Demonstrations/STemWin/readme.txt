/**
  @page STemWin   STM32H750B-DK STemWin Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    STemWin/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H750B-DK Demonstration
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

The STM32 H7 STemWin demonstration is running on STM32H750B-DK boards 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals and initialize the Flash interface.
The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as time base for the HAL drivers.
This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms. 
The systick is then used for FreeRTOS time base.
The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The binary and Media demonstration files are available from the STM32H750B-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h750b-dk.html

Multimedia content is also available in the /Media folder within the demonstration binaries package 
package and content is to be copied (as it is) on uSB key root using the host PC then to be connected on the STM32H750B-DK board
USB-FS connector (CN13)
 
Below is the listing of these media files.

Media
\---------
   |   BehindTheDemo.avi
    Audio
    \---
       |   Clear Day.wav
       |   music.wav
    BMP
    \---
       |   image06.bmp
       |   image07.bmp
       |   image08.bmp
    Video
    \---
       |   video0_480_272.avi
       |   video1_480_272.avi
       |   video2_480_272.avi
	   
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
 
 Only the AVI files format with 480x272 pixels as resolution are supported.
 
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
 For demonstration not touching the rocket will lead to a game that is not ending.
 The CPU load metrics are displayed on the middle of top screen.
 
 + System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.  

@par Keywords

Demonstration, FreeRTOS, RTOS, WAV, Video Player, AVI, JPEG, MJPEG, Audio player, Graphics,
Game, System, CPU, SAI, QSPI, USB, 

@par Hardware and Software environment

  - This demonstration runs on STM32H750xx devices.
    
  - This demonstration has been tested with STMicroelectronics STM32H750B-DK
    boards and can be easily tailored to any other supported device 
    and development board.
     
@par How to use it ? 

To load the demonstration, use STM32CubeProgrammer to program both internal Flash and external QSPI memory.

In order to program the demonstration you must do the following:
2- Open STM32CubeProgrammer, go to "External loaders" menu then check "MT25TL01G_STM32H750B-DISCO" box 
3- Connect the STM32H750B-DK board to PC with USB cable through CN4 and click on STM32CubeProgrammer "Connect" button
4- Use the generated hex file  with STM32CubeProgrammer to program both internal Flash and external QSPI memory:
   - From Erasing and programming Menu browse to select the "STM32Cube_Demo-STM32H750B-DK-VX.Y.Z_FULL.hex" file
     then click on start programming
   
   You can also use the script "program_hexfile.bat" (available within the binaries package) to directly program this hex file into the STM32H750B-DK board.
   This script will invoke the STM32CubeProgrammer with the correct QSPI flasher (MT25TL01G_STM32H750B-DISCO)  
   
5- copy the audio and video files provided under "/Media" in a USB key storage
6- Plug/connect the Media storage: USB key USB-FS Connector CN13.
-> The internal Flash and the external QSPI are now programmed and the demonstration is shown on the board.


The binary and Media demonstration files are available from the STM32H750B-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h750b-dk.html
Direct link:
https://www.st.com/resource/en/compiled_demos/stm32h750b-dk_demo.zip

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
 
