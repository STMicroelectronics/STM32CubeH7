/**
  @page Demonstrations STM32H750B-DK Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H750B-DK Full Demonstration
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

The STM32Cube Demonstration platform is built around the powerful graphical libraries
STemWin and TouchGFX, and the FreeRTOS real time operating system and uses almost the whole STM32
capability to offer a large scope of usage based on the STM32Cube HAL BSP and several
middleware components.

@par Demonstration Overview

This STM32H7 demonstration is running on STM32H750B-DK board 

  - 'STM32Cube_Demo-STM32H750B-DK-V1.0.0_FULL.hex' : 
       - an out of the box Demo integrating the StemWin 
       - TouchGFX demonstration module .

The binary and Media demonstration files are available from the STM32H750B-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h750b-dk.html
Direct link:
https://www.st.com/resource/en/compiled_demos/stm32h750b-dk_demo.zip
		 
The demonstration's executables STM32Cube_Demo-STM32H750B-DK-V1.0.0_FULL.hex can be flashed 
to the target using the STM32CubeProgrammer.

Multimedia content is also available in the /Media folder within the demonstration 
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

Below you find an overview of the different offered modules in the StemWin, Embedded Wizard and TouchGFX demonstrations:

STM32H750B-DK
====================

In the menu launcher the user can either select a demonstration to run or display the
info about the system.

Info
=====
In the launcher main screen, the user can press the "i" icon to show or hide
the system info window. The info contains:
- MCU: 		 STM32H750
- Board: 	 STM32H750B-DK
- CPU Speed: CM7 400 MHz
- Firm. Ver: V1.4.0
  
+ STemWin demonstration:
========================

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
 
 + System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.  

+ TouchGFX demonstration:
=========================

 ++ About
 --------
 In the about screen, you can scroll upd and down to view different information
 regarding the demonstration and TouchGFX. A behind the demo video can also be played if
 a USB key with this video present is inserted. 

 ++ Home Controller 
 -----------------
 To view room information and the state of the devices in the room.
 From the Home UI screen you can select either to view the temperature in the two rooms,
 or the state of devices by pressing on either kitchen or bathroom buttons.
 
 ++ Extractor Hood controller for speed and light
 ------------------------------------------------
 Allowing to turn the fan on and off as well as setting it's speed.
 From Extractor Hood screen the light in the kitchen can also be controlled. 
 
 ++ Oven UI
 ----------
 The Over screen allows to select a program, set the temperature and timer.
 To set the temperature and timer, press on the temperature/timer buttons.
 Pressing the start button.
 
 ++ Smart recipe with video instruction
 --------------------------------------
 From the recipe selector the different recipes can be controlled through
 and the top recipe can be selected.
 When the recipe is scrolled to the top a play button is visible allowing
 to start an instruction video. 

 ++ Washing machine UI with video instruction
 --------------------------------------------
 Available from bathroom screen. allowing to select a washing program and providing 
 video instructions. 
 
 ++ Bathtub controller for light, fan speed and temperature
 ----------------------------------------------------------
 Available from bathroom screen. allowing to control the temperature, jet and light. 
 video instructions.
 
@par Keywords

Demonstration, STemWin, FreeRTOS, RTOS, WAV, Video Player, AVI, JPEG, MJPEG, MP3, Audio player, Graphics,
Game, System, CPU, SAI, QSPI, SD, 

@par Hardware and Software environment

  - This demonstration runs on STM32H750xx devices.
    
  - This demonstration has been tested with STMicroelectronics STM32H750B-DK.
     
@par How to use it ? 

To load the demonstration, use STM32CubeProgrammer to program both internal Flash and external QSPI memory.

In order to program the demonstration you must do the following:
2- Open STM32CubeProgrammer, go to "External loaders" menu then check "MT25TL01G_STM32H750B-DISCO" box 
3- Connect the STM32H750B-DK board to PC with USB cable through CN4 and click on STM32CubeProgrammer "Connect" button
4- Use "STM32Cube_Demo-STM32H750B-DK-V1.0.0_FULL.hex" file provided under "Binary" with STM32CubeProgrammer
   to program both internal Flash and external QSPI memory:
   - From Erasing and programming Menu browse to select the "STM32Cube_Demo-STM32H750B-DK-VX.Y.Z_FULL.hex" file
     then click on start programming
   
   You can also use the script "program_hexfile.bat" to directly program this hex file into the STM32H750B-DK board.
   This script will invoke the STM32CubeProgrammer with the correct QSPI flasher (MT25TL01G_STM32H750B-DISCO)  
   
5- copy the audio and video files provided under "/Media" in a USB key storage
6- Plug/connect the Media storage: USB key USB-FS Connector CN13.
-> The internal Flash and the external QSPI are now programmed and the demonstration is shown on the board.



 */
 
