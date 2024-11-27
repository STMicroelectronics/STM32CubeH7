/**
  @page FWupgrade_Standalone Description of the binary file
  
  @verbatim
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/Binary/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the binary file.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. All rights reserved.
  *
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

@par Application Description 

This directory contains a binary to be loaded into Flash memory using Device 
Firmware Upgrade application.
   
This binary is a simple LED toggling application, it starts toggling LED1 and LED3
alternatively with LED2 and LED4. The 4 LEDs lights one by one for a period of 100 ms
and the cycle repeats giving the running light appearance.
The system Timer (Systick) is used for to generate the delay.
The offset address of this binary is 0x08020000 which matches the definition in 
Firmware upgrade application "APPLICATION_ADDRESS".

@par Hardware and Software environment

  - This application runs on STM32H7xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.  


 */
