/**
  @page SplashScreen   STM32H743I-EVAL SplashScreen Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SplashScreen/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H743I-EVAL SplashScreen module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 
