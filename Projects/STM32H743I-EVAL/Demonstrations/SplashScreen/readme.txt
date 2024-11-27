/**
  @page SplashScreen   STM32H743I-EVAL SplashScreen Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    SplashScreen/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H743I-EVAL SplashScreen module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Demo Description
The SplashScreen module aims to display a Splash Screen video eachtime the board is powered on
The splash screen video can be interrupted by the user by simply touching the screen
This module will allow the user to select a Sub-Demo from the list of three demos displayed
at startup time. The user can select STemWin, EmbeddedWizard or TouchGFX Sub-Demo
Once a Sub-Demo is selected the SplashScreen module will store its startup address into backup
register and then do Software Reset and then jump to that selected Sub-Demo code.
Inside each Sub-Demo a "Back To Main Menu" button is implemented allowing the usert to go back
to this module by simply doing a software reset with reset backup registers.



 */
 
