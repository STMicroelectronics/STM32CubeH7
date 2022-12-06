/**
  @page Demonstrations  STM32H747I-DISCO Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32H747I-DISCO Demonstration
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

The STM32Cube demonstration platform comes on top of the STM32Cube(TM) as a firmware
package that offers a full set of software components based on a modular architecture.
All modules can be reused separately in standalone applications.
All these modules are managed by the STM32Cube demonstration kernel that allows to
dynamically add new modules and access common resources (storage, graphical
components and widgets, memory management, real-time operating system).
The STM32Cube demonstration platform is built around the FreeRTOS real-time operating
system and offers a large scope of use cases based on the STM32Cube HAL BSP and several
middleware components.

The demonstration's executable STM32Cube_Demo-STM32H747I-DISCO-VX.Y.Z.hex can be flashed 
to the target using the STM32CubeProgrammer. 

The demonstration can run only on STM32H747I-DISCO with LCD.


Multimedia content is also available in the Storage(SDCard) folder within the demonstration 
package and is to be copied on mass storage using the host PC. Below is the listing of these 
additional files.

Storage
\---------
   |   ocean_birds.avi
   |   rain_drops.avi
   |   Stream.avi
   |   tropicalfish.avi
	Audio
	\---
	   |   Clear Day.wav
	   |   music.wav
	   |   nina_128kb.mp3
	   |   richard_192Kbps.mp3
	   |   siekka_128Kbps.mp3
	BMP
	\---
	   |   image06.bmp
	   |   image07.bmp
	   |   image08.bmp
	Video
	\---
	   |   video0_240_192.avi
	   |   video0_800_480.avi
	   |   video1_240_192.avi
	   |   video1_800_480.avi
	   |   video2_240_192.avi
	   |   video2_800_480.avi
	   |   video3_240_192.avi


Firmware files:
  STEW_firmware.bin            STemWin Binary firmware for MB1166 LCD
  STEW_resources.bin           STemWin Binary resources for MB1166 LCD
  TGFX_firmware.bin            Touch GFX Binary firmware for MB1166 LCD
  TGFX_resources.bin           Touch GFX Binary resources for MB1166 LCD
  EWIZ_firmware.bin            Embedded Wizard Binary firmware for MB1166 LCD
  EWIZ_resources.bin           Embedded Wizard Binary resources for MB1166 LCD

After connection of the LCD on the board, either the MB1315 or the MB1166,
the demonstration starts by displaying an animation with the ST logo and goes
into the Menu Launcher screen where the user can select between three demonstrations,
that's one demonstration per graphics stack: STemWin, TouchGFX and Embedded Wizard.
When a Sub-Demo is selected, the Menu Launcher will check if the firmware and resources
are already existing in both internal and external flashes then if exist it will jump
to the selected Sub-Demo otherwise a pop-up message is displayed requesting a confirmation
to load the suitable firmware and resources. After confirmation the MenuLauncher starts
loading the firmware and resources binaries into receptively the internal flash and the
quadspi memories, a progress bar is shown to follow the loading progress.

Once loading is terminated, the binary Sub-Demo is started.

@note: each Sub-Demo has to load its firmware and resources separately if not already done.
@note: depending on the Sub-Demo, resource loading may take several minutes.


STM32H747I-DISCO
====================

In the menu launcher the user can either select a demonstration to run or display the
info about the system.


Info
=====
In the launcher main screen, the user can press the "i" icon to show or hide
the system info window. The info contains:
- MCU: 		 STM32H7x7 Dual Core
- Board: 	 STM32H7x7I-DISCO
- CPU Speed: CM7 400 MHz, CM4 200 MHz
- Firm. Ver: V1.4.0


STemWin
=======

The STemWin demonstration offers the following modules:

  - Audio player
  - Video Player
  - Clock & Weather
  - Graphical Effect
  - Rocket Game
  - Dual Core
  - System info

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
 
 ++ Dual Core
 --------------
 The dual core module shows the difference between using dual core and single core.
 when dual core is activated, the fractal video works on CM4 and the rest of the 
 videos runs on CM7, and when dual core deactivated , all videos works on CM7.
 When the dual core activated, the fps will be higher and CPU load will be lower 
 than with a single core.

 The Corext-M4 binary file required for this sub-module is available under \Demonstrations\extra_modules\ST. 
 
 + System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.  


TouchGFX
=========

The demonstration features the following modules:
    - Bird Eat Coin Game
    - 2048 - Game
    - Audio player
    - MJPEG Video player
    - Home Automation
    - Date and Clock
    - Hardware integration (Junction temperature sensor and LCD brightness)
	- Back icon


Embedded Wizard
==================
This Sub-Demo offers the following sub-demos:
 - Climate Cabinet
 - Washing Machine
 - Paper Cutter
 - Fitness Tracker
 - Waveform Generator
 - Charts Demo
 - Graphics Accelerator
 - Brick Game


In each demonstration, on the top right side home button to go back to the launcher menu.
An arrow in the bottom center side of the display once clicked a pop up display will appear
to show information about GUI development.On click the luncher menu appear. 

+ Climate Cabinet
------------------

Upon Climate Cabinet demonstration pick up, the following scrolling list show up:
 An up and down sliding for:
  - 'Temperature and Humidity' button
  - 'Dwell Time' button
  - 'Heating and Cooling' Time button
  - 'Start' button

 ++ 'Temperature and Humidity' button
 ------------------------------------
 This option allows to adjust temperature and humidity parameters.

 ++ 'Dwell Time' button
 -----------------------
 This option allows to adjust dwell time parameters.


 ++ 'Heating and Cooling Time' button
 ------------------------------------
 This option allows to adjust Heating and Cooling parameters.

 ++ 'Start' button
 ------------------
 Pressing this button allows to immediately start the demonstration.

+ Washing Machine
-------------------

Upon Washing Machine demonstration pick up, the following icons show up on scroll list along the screen left side :

  - Program
  - Temperature
  - U/min
  - Options
  - Finish
  - 'Start' button

When this demonstration is selected, five column table containing different options of the washing machine, every column
is a scrolling list.

 ++ Program
 -----------
 Contain seven different modes: Hot/Colored Wash, Easy Care, Delicates, Sportswear, WoolHandwash, Eco, Drain or Spin.

 ++ Temperature
 ---------------
 This option allows to fix temperature for the washing, it goes between cold and 95 degree.

 ++ Finish
 -----------
 This option allows to set uo the duration for the washing period.

 ++ 'Start' button
 -----------------
 Pressing this button  allows to immediately come back to the initial menu.

+ Paper Cutter
--------------
Upon Paper Cutter demonstration pick up, four options are available:
  - Feed
  - Speed
  - Output Format
  - Batch
  - 'Start' button

 ++ Feed
 --------
 This custom contains four different options;pinfeed perforated, pinfeed non-perforated, pinless perforated and pinless non-perforated.
 Once one of the four options is selected, click on the green circular green 'OK' button to save the choice.
 To cancel the choice, on the bottom right size, click on the cancel button.  

 ++ Speed
 ----------
 This custom allows to control paper generation speed, User can choose from 600 to 3600 paper  speed per hour.
 Two arrows placed on the top center side of the screen to fix the speed of produced papers.
 Once  selected, click on the green circular green 'OK' button to save the choice.
 To cancel the choice, on the bottom right size, click on the cancel button.

 ++ Output Format
 -----------------
 This custom allows to choose the size of th paper, different din paper formats are available from A3 to A8 on the right side of the screen,
 On the left side a graphical representation of the paper which user can manually pick his preferred din paper format. 
 Once wanted dimension selected, click on the green circular green 'OK' button to save the choice.
 To cancel the choice, on the bottom right size, click on the cancel button.

 ++ Batch
 ---------
 This custom allows to control number of paper to produce,by adjusting from 10 to 1000 user can produce a custom paper number.
 Two arrows placed on the top center side of the screen to fix the number of produced papers.
 
 ++ 'Start' button
 -------------------
 Pressing this button on the left bottom side allows to immediately come back to the initial menu.


++ Waveform Generator
-----------------------
 Upon Waveform Generator demonstration pick up, A waveform graph  is loaded to the screen display on the left side
 contains oscillations from two input channels.
 On the right side of the display a scrolling list for the two input channels containing two slider to calibrate the frequency,
 the amplitude and different oscillations formats(sine, square,ramp or white noise wave) for the user to choose between them.


 
++ Graphics Accelerator
------------------------
 Upon Graphics Accelerator demonstration pick up, a graphical demonstration of different shapes is lunched; Alpha8 Blend, Bitmap Copy,
 Rectangle Blend or rectangle Copy.
 The following buttons show up:

  - 'Next' button
  - 'on/off' button
 On the right side of the screen many features are visible like how many frames per second(FPS) or how much the CPU is loaded
 and the name of the shape playing in the demonstration.
 
 ++ 'Next' button
 ------------------
 When this button is clicked a random graphical shapes are generated different from the previous.


 ++ 'on/off' button
 -------------------
 Pressing this button allows to activate or deactivate Chrom-ART.
 
@note the application doesn't require uSD to run
@note the application starts automatically when left without interaction for
      some time

The binary and Media demonstration files are available from the STM32H747I-DISCO page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h747i-disco.html
Direct link:
https://www.st.com/resource/en/compiled_demos/stm32h747i-disco_demo.zip

@par Keywords

Demonstrations, Adafruit, Shield, TFT, Joystick, SD Card, Display, 

@par Hardware and Software environment

 1 - The jumpers configuration is as follows:
     JP3 fitted 

 2 - Open STM32CubeProgrammer, go to Erasing and Programming menu then check 
	 "MT25TL01G_STM32H747I-DISCO" box

     Connect the STM32H747I-DISCO board to PC with USB cable through CN2
     Use the demonstration STM32Cube_Demo-STM32H747I-DISCO-VX.Y.Z.hex binary file with the
     STM32CubeProgrammer to program both internal Flash and external QSPI memory.

 3 - Copy the audio and binary files provided under storage (SDCard) in the SD card and 
     insert it into CN12 connector
 
 5 - Run the demonstration

  - This demonstration runs on STM32H747Ixx devices.

  - This application has been tested with STM32H747I-DISCO and can be
    tailored to any other supported device and development board.


 */
 
