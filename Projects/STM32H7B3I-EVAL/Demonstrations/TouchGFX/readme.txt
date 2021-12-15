/**
  @page TouchGFX   STM32H7B3I-EVAL Touch GFX Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H7B3I-EVAL Touch GFX Demonstration
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

 ++ Time & Calendar
 ------------------
 The Time & Calendar module allows to show and adjust the time and
 date by changing the real time configuration (RTC).
 
  ++ Home Control
 ----------------
 The Home Control module allows to Control home and shows vivid
 graphs and access control.
 To run statistics modules a password is needed: "L"

  ++ Graphics Effect
 ----------------
Demonstrates the capabilities of the texture mapper, by rotate,
scale, and change the opacity of a 3D cube.

  ++ External Hardware
 ---------------------
 The External Hardware module allows to control the graphic via the
 potentiometer and adjust the screen brightness.
 Make sure JP5 is plugged in position POTPA0_C.

  ++ Bird Eat Coin Game
 --------------------------
 Bird Eat Coin Zombie Game shows the graphic performance of the
 Chrom-ART and TouchGFX graphical stack.

  ++ 2048 Puzzle Game
 --------------------
 The 2048 Puzzle Game shows high quality graphics and smooth
 animations.

@par Keywords

Demonstration, TouchGFX

@par How to use it?

In order to make the program work, you must do the following :
 - Open STM32CubeProgrammer and click on connect
 - Go to external loaders menu then select MX25LM51245G_STM32H7B3I-EVAL to program external OSPI memory
 - From Erasing and programming Menu browse to select the hex file then click on start programming
  
 Or you can also use the script "program_hexfile.bat" (available within the binaries package) to directly program 
 this hex file into the STM32H7B3I-EVAL board.
 This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H7B3I-EVAL)  

@par Hardware and Software environment

  - This demonstration runs on STM32H7B3xx devices.    
  - This demonstration has been tested with STMicroelectronics STM32H7B3I-EVAL boards.


*/
