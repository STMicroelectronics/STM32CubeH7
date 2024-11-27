/**
  @page Demonstrations STM32H735G-DK Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H735G-DK Full Demonstration
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

The STM32Cube Demonstration platform is built around the powerful TouchGFX graphical libraries
and the FreeRTOS real time operating system and uses almost the whole STM32
capability to offer a large scope of usage based on the STM32Cube HAL BSP and several middleware components.

STM32H735G-DK Demonstration features :
  CPU frequency at 550MHz with on the fly check and re-programming of the CPU_FREQ_BOOST flash option byte

  - EEMBC® CoreMark®  benchmark
      Running from internal flash and giving 2777 CoreMark® with CPU at 550MHz

  - Oscilloscope/Signals Generator: 
     - Code Execution from external OctoSPI flash
     - LCD display from external OctoSPI RAM

  - TouchGFX graphical demonstration
    - Code Execution from external OctoSPI flash
    - LCD display from external OctoSPI RAM


@par Demonstration Overview

This STM32H7 demonstration is running on STM32H735G-DK-DK board 

The binary and demonstration files are available from the STM32H735G-DK-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h735g-dk.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h735g-dk_demo.zip
		 
The demonstration's executables STM32Cube_Demo-STM32H735G-DK-V1.0.0.hex can be flashed 
to the target using the STM32CubeProgrammer.



Below you find an overview of the different offered modules:

STM32H735G-DK
====================

The menu launcher features the following modules:
	- Rotating Sub-demonstration icons
	- Description of each Sub-demonstration
    - Run a Sub-demonstration

This Demo MenuLauncher must be programmed into the oard first to allow running sub-demonstrations 

+ EEMBC(R) CoreMark(R) sub-demonstration:
=========================
A simple touch on the screen will start the benchmark and display the score on real STM32H735 device giving: ~2777 Coremarks

Algorithm implemented with preconditions:
CPU frequency: CM7 @ 550MHZ
Code : Flash Bank1
RW data : DTCM
CM7 L1-Cache ON 
Number of iterations : CM7 35000 
Compiler: IAR embedded workbench for ARM 7.80

This demonstration is provided as binary only (no source code)

+ Oscilloscope_SignalsGenerator sub-demonstration:
=========================
Oscilloscope/Signals Generator Demonstration based on The STM32H735G-DK  board
The Demo purpose is to promote the analog peripherals of the STM32H7xx devices    
(ADC, TIMER, DMA, UART, DAC)

  - Analog new features to be highlighted
    ADC oversampling, 16 bits resolution, Fast channels mode, dual ADC mode.
 
  - Data acquisition and sampling using the STM32H7 peripherals 

  - Dedicated PC GUI :
     To change the oscilloscope settings, visualize the acquired signals
     and perform mathematical processing.

     On PC side, you must do the following :
       - Setup your Oscilloscope PC application (\PC_Software\setup.exe)
       - Run the "Serial Oscilloscope" application on your PC
       - Configure your UART communication in the GUI by selecting the Serial Port and Baude rate(115200),
       - Connect a (Type A to Micro-B) USB cable from the STM32H735G-DK board (Connector CN15) to the PC 
       - Start the connection between the Board and the PC GUI using the dedicated PC GUI button 
       - Configure the desired sampling rate frequency.
       - Put an external signal with amplitude less than 3.3 V to the input (PA0,CN9 Connector pin A2 of the STM32H735G-DK board)..
       - Click on Start Scope button.

  - Signal Generator (with AutoTest mode):
      Using the DAC to generate different test signal patterns.
      Configuration using STM32H735G-DK HMI on LCD display
      Signal Type : DC, Square, Sine, Triangle, Escalator or Noise
      Signal Frequency : 5,000 KHz to 130,000 KHz
      Signal Amplitude  : 0,100 V to 3,300 V
      Brief Status Current output signal

  - FreeRTOS Multithread:
    One Thread to handle data acquisition and communication with the PC tool.
    One Thread to handle Signal generator and LCD display (DAC, LTDC)



+ TouchGFX sub-demonstration:
=========================

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
 
 ++ Smart recipe with instruction
 --------------------------------------
 From the recipe selector the different recipes can be controlled through
 and the top recipe can be selected.

 ++ Washing machine UI
 --------------------------------------------
 Available from bathroom screen.
 
 ++ Bathtub controller for light, fan speed and temperature
 ----------------------------------------------------------
 Available from bathroom screen. allowing to control the temperature, jet and light.
 
@par Keywords

Demonstration, FreeRTOS, RTOS, Graphics, CoreMark, EEMBC, ADC, DAC, TIMER, Oscilloscope, SignalsGenerator, TouchGFX, MenuLauncher

@par Hardware and Software environment

  - This demonstration runs on STM32H735G devices.
    
  - This demonstration has been tested with STMicroelectronics STM32H735G-DK.
     
@par How to use it ? 

The Full demonstration binary files are available from the STM32H735G-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h735g-dk.html

In order program this sub-demo into the STM32H735G-DK board :
 - Open STM32CubeProgrammer and click on connect
 - Go to external loaders menu then select MX25LM51245G_STM32H735G-DK to program external OSPI memory
 - From Erasing and programming Menu browse to select the hex file from \Binary folder then click on start programming
  
 Or you can also use the script "program_hexfile.bat" (available within the binaries package) to directly program 
 this hex file into the STM32H735G-DK board.
 This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H735G-DK)



 */
 
