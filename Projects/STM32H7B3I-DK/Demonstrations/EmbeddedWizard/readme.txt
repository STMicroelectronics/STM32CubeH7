/**
  @page EmbeddedWizard STM32H7B3I-DK Embedded Wizard GUI Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  TARA Systems
  * @brief   Description of STM32H7B3I-DK GUI Demonstration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 TARA Systems.
  * All rights reserved.</center></h2>
  *
  * This software is delivered "as is" and shows the usage of other software 
  * components. It is provided as an example software which is intended to be 
  * modified and extended according to particular requirements.
  * 
  * TARA Systems hereby disclaims all warranties and conditions with regard to
  * the software, including all implied warranties and conditions of
  * merchantability and non-infringement of any third party IPR or other rights
  * which may result from the use or the inability to use the software.
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

This is an Embedded Wizard GUI demonstration for the STM32H7B3I Discovery board,
created by TARA Systems.

Parameters:
    Resolution: 480 * 272
	BPP: 16 (RGB565)

The demonstration features the following examples:
    - Climate Cabinet: The ClimateCabinet demo shows the implementation of a
       control panel for a climatic exposure test cabinent. The user can define
       a heating time, a nominal temperature and humidity, a dwell time and the
       final cooling time.
    - WashingMachine: This demo shows the implementation of a washing machine
       with a couple of fancy scrollable list widgets to choose the washing
       program and parameters. The speciality of this sample application is the
       magnification effect of the centered list items and the soft fade-in/
       fade-out effects.
    - PaperCutter: This demo shows the implementation of a paper cutting
       machine, where the user can define the pagination and format of the paper
       as well as the cutting speed and the amount of papers. The application
       contains many rect effects and fade-in/fade-out effects to change
       dynamically the layout of the screen.
    - WaveformGenerator: This WaveformGenerator demo application combines
       waveforms with different amplitudes and frequencies. The implementation
       shows the usage of the class Charts::Graph to paint a list of
       coordinates.
    - GraphicsAccelerator: This application demonstrates the graphics
       performance of the DMA2D hardware graphics accelerator. Sets of basic
       drawing operations are executed permanently and continuously, while the
       user can switch on/off the hardware accelerator.
    - BrickGame: The sample application BrickGame implements a classic "paddle
       and ball" game. In the game, a couple of brick rows are arranged in the
       upper part of the screen. A ball travels across the screen, bouncing off
       the top and side walls of the screen. When a brick is hit, the ball
       bounces away and the brick is destroyed. The player has a movable paddle
       to bounce the ball upward, keeping it in play.
    - BezierClock: The sample application BezierClock implements a fancy digital
       clock and timer application with animated digits. The application uses
       vector graphics to render dynamically the different digits for clock and
       timer. The change from one digit to another is handled by moving the
       vector points to get a smooth transition animation.

@par How to use it ?

Please use STM32CubeProgrammer utility to flash the application's HEX file to
the STM32 target.

@par Keywords

Demonstration, Embedded Wizard, GUI, Graphics, Connectivity, Clock, Weather 

@par Getting started with Embedded Wizard

In order to get familiar with Embedded Wizard and the GUI development work-flow,
we highly recommend to study our online documentation:
  https://doc.embedded-wizard.de/welcome-to-embedded-wizard
  https://doc.embedded-wizard.de/quick-tour
  https://doc.embedded-wizard.de/getting-started-stm32

Furthermore, we have collected many 'Questions and Answers' covering typical
Embedded Wizard programming aspects. Please visit our community:
  http://ask.embedded-wizard.de

Please use this forum to drop your questions, answers and ideas.


*/
