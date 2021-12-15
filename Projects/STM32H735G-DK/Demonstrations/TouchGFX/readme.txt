/**
  @page TouchGFX   STM32H735G-DK TouchGFX Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    TouchGFX/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H735G-DK Touch GFX Demonstration
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

 ++ Washing machine UI with video instruction
 --------------------------------------------
 Available from bathroom screen. allowing to select a washing program and providing 
 
 ++ Bathtub controller for light, fan speed and temperature
 ----------------------------------------------------------
 Available from bathroom screen. allowing to control the temperature, jet and light.


@par How to use it?

The Full demonstration binary files are available from the STM32H735G-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h735g-dk.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h735g-dk_demo.zip

This Demo MenuLauncher must be programmed into the oard first to allow running this sub-demo

In order to make the program work, you must do the following :
 - Open STM32CubeProgrammer and click on connect
 - Go to external loaders menu then select MX25LM51245G_STM32H735G-DK to program external OSPI memory
 - From Erasing and programming Menu browse to select the hex file from \Binary folder then click on start programming
  
 Or you can also use the script "program_hexfile.bat" (available within the binaries package) to directly program 
 this hex file into the STM32H735G-DK board.
 This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H735G-DK)  

@par Keywords

Demonstration, FreeRTOS, RTOS, Graphics, TouchGFX, MenuLauncher, GUI

@par Hardware and Software environment

  - This demonstration runs on STM32H735xx devices.    
  - This demonstration has been tested with STMicroelectronics STM32H735G-DK
    boards.


*/
