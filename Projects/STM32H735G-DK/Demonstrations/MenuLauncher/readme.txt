/**
  @page MenuLauncher STM32H735G-DK TouchGFX Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    MenuLauncher/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H735G-DK Demonstration
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

This is a menu launcher demonstration for the STM32H735G Discovery board, created by ST.

Requirements:
	Resolution: 480 * 272
	BPP: 24

The menu launcher features the following modules:
	- Rotating Sub-demonstration icons
	- Description of each Sub-demonstration
    - Run a Sub-demonstration

@par Keywords

Demonstration, FreeRTOS, RTOS, Graphics, TouchGFX, MenuLauncher, GUI

@par How to use it ?

The Full demonstration binary files are available from the STM32H735G-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h735g-dk.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h735g-dk_demo.zip

In order program this sub-demo into the STM32H735G-DK board :
 - Open STM32CubeProgrammer and click on connect
 - Go to external loaders menu then select MX25LM51245G_STM32H735G-DK to program external OSPI memory
 - From Erasing and programming Menu browse to select the hex file from \Binary folder then click on start programming
  
 Or you can also use the script "program_hexfile.bat" (available within the binaries package) to directly program 
 this hex file into the STM32H735G-DK board.
 This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H735G-DK)


Please use STM32CubeProgrammer utility to flash the application's HEX file to the STM32 target.



*/
