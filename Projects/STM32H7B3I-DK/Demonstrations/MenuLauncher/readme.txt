/**
  @page MenuLauncher   STM32H7B3I-DK Menu Launcher Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H7B3I-DK Menu Launcher Demonstration
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

The Menu Launcher application is a sliding icons bar solution which provides a menu of supported demos.
It's based on the STM32H7B3xx and and the TouchGFX APIs. 
This module will allow the user to select a sub-demo from the list of six demos displayed: Audio player,
Video player, TouchGFX, STemWin, EmbedddedWizrad and Clock&Weather.
When selecting a demo, an overview is displayed and the user can choose between jumping to the demo or return to main screen.

Note:
before running any demo, make sure that the selected demo is already programmed.
For more details on the supported demos, please refer to the readme.txt files delivered with the demos packages.

@par Keywords

Demonstration, MenuLauncher

@par How to use it?

In order to make the program work, you must do the following :
 - Open STM32CubeProgrammer and click on connect
 - Go to external loaders menu then select MX25LM51245G_STM32H7B3I_DISCO to program external OSPI memory
 - From Erasing and programming Menu browse to select the hex file then click on start programming
  
 Or you can also use the script "program_hexfile.bat" (available within the binaries package) to directly program 
 this hex file into the STM32H7B3I-DK board.
 This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H7B3I_DISCO)

@par Hardware and Software environment

  - This demonstration runs on STM32H7B3xx devices.    
  - This demonstration has been tested with STMicroelectronics STM32H7B3I-DK
    boards.



 */
 
