/**
  @page Demonstrations Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32H7B3I-EVAL Demonstration
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
The STM32Cube demonstration platform is built around the FreeRTOS real-time operating
system and offers a large scope of use cases based on the STM32Cube HAL BSP and several
middleware components.

The demonstration's executables STM32Cube_Demo-STM32H7B3I-EVAL-V1.0.0.hex can be flashed 
to the target using the STM32CubeProgrammer.

Multimedia content is also available in the Storage(SDCard) folder within the demonstration 
package and is to be copied on mass storage using the host PC. Below is the listing of these 
additional files.

Storage
\---------
	Audio
	\---
	   |   Clear Day.wav
	   |   music.wav
	   |   Corporate Inspiration.mp3
	   |   ST Life augmented.mp3
       
	Pictures\BMP_800x480
	\---
	   |   image06.bmp
	   |   image07.bmp
	   |   image08.bmp
       
    cw_cfg.ini: clock and weather configuration file
	
Binary files:
  STM32Cube_Demo-STM32H7B3I-EVAL-V1.0.0.hex: STM32H7B3I-EVAL Demonstration Full binary file 
  program_hexfile.bat: Allows to directly program this hex file into the STM32H7B3I-EVAL board.
  This script will invoke the STM32CubeProgrammer with the correct OSPI flasher 
 (MX25LM51245G_STM32H7B3I-EVAL) 

The demonstration starts Menu Launcher by displaying the supported demos icons.
The user can slide the icons bar and select between five demonstrations:
STemWin, TouchGFX , Embedded Wizard, Audio player and Clock and Weather
When a Sub-Demo is selected, the Menu Launcher will Display demo overview and jump to the selected Sub-Demo
when selecting start button.

Below you find an overview of the different offered modules in the STemWin, TouchGFX , Embedded Wizard, 
Audio player and Clock and Weather demonstrations:

+ Audio Player demonstration:
=============================
 The audio player application provides a complete audio solution based on the STM32H7B3xx and and the TouchGFX APIs.
 It delivers a high-quality music experience. It supports playing music in WAV and mp3 formats but may
 be extended to support other compressed formats such as WMA audio format.
 You can use the *.wav and *.mp3 audio provided under "Utilities/Media/Audio" or any other ones.

 The Audio Player Demonstration features:
  - Support playing music in WAV and MP3 formats.
  - Offer playing audio and the system in Low power mode(main domain in DSTOP2)
  - Display total and elapsed times
  - Support repeat all and Shuffle
  - Volume control
  - Support progress bar navigation (seek to set the audio at specific position)
  - Display all supported audio files available on media storage

+ ClockAndWeather demonstration:
=============================
 The ClockAndWeather module provides a clock and weather application based on the STM32H7xxx and 
 the Touch GFX APIs

 The ClockAndWeather Demonstration features:
  - Allow to retrieve the time and weather forecast for Paris city (city by default)
  - Display temperature in Celsius or Fahrenheit
  - Display time in 24/12 hours format
  - User can get the time and forecast weather for specific city by adding the config file (cw_cfg.ini) 
    under SD card at root folder then fill the location field by city name
  - Support open and encrypted access point
  - Background is updated according to day time 

+ EmbeddedWizard demonstration:
===============================
Below you find an overview of the different offered modules in the EmbeddedWizard demonstration:

 ++ Climate Cabinet
 ------------------ 
 The ClimateCabinet demo shows the implementation of a control panel for a climatic exposure test cabinent. 
 The user can define a heating time, a nominal temperature and humidity, a dwell time and the final cooling time.
 
 ++ WashingMachine 
 -----------------
 This demo shows the implementation of a washing machine with a couple of fancy scrollable list widgets to choose the washing
 program and parameters. The speciality of this sample application is the magnification effect of the centered list items and 
 the soft fade-in/fade-out effects.
 
 ++ PaperCutter
 --------------
 This demo shows the implementation of a paper cutting machine, where the user can define the pagination and format of the paper
 as well as the cutting speed and the amount of papers. The application contains many rect effects and fade-in/fade-out effects 
 to change dynamically the layout of the screen.
 
 ++ WaveformGenerator
 --------------------
 This WaveformGenerator demo application combines waveforms with different amplitudes and frequencies. The implementation
 shows the usage of the class Charts::Graph to paint a list of coordinates.
 
 ++ GraphicsAccelerator
 ----------------------
 This application demonstrates the graphics performance of the DMA2D hardware graphics accelerator. Sets of basic drawing operations 
 are executed permanently and continuously, while the user can switch on/off the hardware accelerator.
 
 ++ BrickGame
 ------------
 The sample application BrickGame implements a classic "paddle and ball" game. In the game, a couple of brick rows are arranged in the
 upper part of the screen. A ball travels across the screen, bouncing off the top and side walls of the screen. When a brick is hit, 
 the ball bounces away and the brick is destroyed. The player has a movable paddle to bounce the ball upward, keeping it in play.
 
 ++ BezierClock
 --------------
 The sample application BezierClock implements a fancy digital clock and timer application with animated digits. The application uses
 vector graphics to render dynamically the different digits for clock and timer. The change from one digit to another is handled by moving 
 the vector points to get a smooth transition animation.
 
+ STemWin demonstration:
========================
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
 
 ++ System Info
 --------------  
 The system info module provides information about the core, supported boards, 
 CPU speed and demonstration version.

+ TouchGFX demonstration:
=========================
Below you find an overview of the different offered modules in the TouchGFX demonstration:

 ++ Home Control 
 ---------------
 The Home Control module allows to Control home and shows vivid  graphs and access control. 
 To run statistics modules a password is needed: "L“
 
 ++ Time & Calendar
 ------------------------------------------------
 The Time & Calendar module allows to show and adjust the time and date by changing 
 the real time configuration (RTC). 
 
 ++ External Hardware
 --------------------
 The External Hardware module allows to control the graphic via the  potentiometer and adjust the screen brightness.
 
 ++ Bird Eat Coin Game
 ----------------------
 Bird Eat Coin Zombie Game shows the graphic performance of the  Chrom-ART and TouchGFX graphical stack.

 ++ 2048 Puzzle Game
 --------------------
 The 2048 Puzzle Game shows high quality graphics and smooth  animations.

@par Keywords

Demonstrations, Adafruit, Shield, TFT, Joystick, SD Card, Display

@par Hardware and Software environment

  - This demonstration runs on STM32H7B3xx devices.    
  - This demonstration has been tested with STMicroelectronics STM32H7B3I-EVAL revD boards 
     
@par How to use it ? 

To load the demonstration, use STM32CubeProgrammer to program both internal Flash and external OSPI memory.

In order to program the demonstration you must do the following:
1- Open STM32CubeProgrammer, go to "External loaders" menu then check "MX25LM51245G_STM32H7B3I-EVAL" box
2- Connect the STM32H7B3I-EVAL board to PC with USB cable through CN26 and click on STM32CubeProgrammer "Connect" button
3- Use the generated hex file  with STM32CubeProgrammer to program both internal Flash and external OSPI memory:
   - From Erasing and programming Menu browse to select the hex file then click on start programming
   
   You can also use the script "program_hexfile.bat" (available within the binaries package) to directly program this hex file into 
   the STM32H7B3I-EVAL board.
   This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H7B3I-EVAL)

4- copy the BMP files provided under "/Media" in a SD card storage at BMP folder
   copy the audio files provided under "/Media" in a SD card storage at audio folder
5- Plug the Media storage in CN28.
-> The internal Flash and the external OSPI are now programmed and the demonstration is shown on the board.
      

 */
 
