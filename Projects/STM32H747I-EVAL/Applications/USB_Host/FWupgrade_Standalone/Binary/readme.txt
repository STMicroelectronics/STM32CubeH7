/**
  @page FWupgrade_Standalone Description of the binary file
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    USB_Host/FWupgrade_Standalone/Binary/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the binary file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This directory contains a binary to be loaded into Flash memory using Device 
Firmware Upgrade application.
   
This binary is a simple LED toggling applicaion, it starts toggling LED1 and LED3
alternatively with LED2 and LED4. The 4 LEDs lights one by one for a period of 100 ms
and the cycle repeats giving the running light appearance.
The system Timer (Systick) is used for to generate the delay.
The offset address of this binary is 0x08020000 which matches the definition in 
Firmware upgrade application "APPLICATION_ADDRESS".

@par Hardware and Software environment

  - This application runs on STM32H7xx devices.
    
  - This application has been tested with STMicroelectronics STM32H747I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.  

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
