/**
  @page ClockAndWeather STM32H7B3I-DK Clock and Weather Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H7B3I-DK Clock and Weather Demonstration
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

 This module provides a WiFi clock and weather application based on TouchGFX.


@par Clock and Weather Demonstration feature
  - Allow to retrieve the time and weather forecast for up six cities
  - Display temperature in Celsius or Fahrenheit
  - Display time in 24/12 hours format
  - User can get the time and forecast weather for additional cities by adding 
  a config file (cw_cfg.ini) under SD card at root folder: fill the location by city name
  - Support open and encrypted access point
  - Background is updated according to day time 


@par Hardware and Software environment

  - This demonstration runs on STM32H7B3xx devices.    
  - This demonstration has been tested with STMicroelectronics STM32H7B3I-DK
    boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

To load the demonstration, use STM32CubeProgrammer to program both internal Flash and external OSPI memory.

In order to program the demonstration you must do the following:
2- Open STM32CubeProgrammer, go to "External loaders" menu then check "MX25LM51245G_STM32H7B3I_DISCO" box
3- Connect the STM32H7B3I-DK board to PC with USB cable through CN14 and click on STM32CubeProgrammer "Connect" button
4- Use the generated hex file  with STM32CubeProgrammer to program both internal Flash and external OSPI memory:
   - From Erasing and programming Menu browse to select the hex file
     then click on start programming
   
   You can also use the script "program_hexfile.bat" (available within the binaries package) to directly program this hex file into 
   the STM32H7B3I-DK board.
   This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H7B3I_DISCO)
   
5- copy the config template provided under "/Media/ClockAndWeather" in a SD card storage at root folder
6- Plug the Media storage in CN4.
-> The internal Flash and the external OSPI are now programmed and the demonstration is shown on the board.
7- To get the time and weather forecast for a specific cities make sure before running the demo to add the config file (cw_cfg.ini)
   under SD card at root folder and fill the location field with cities names. (ex: location = sydney).


The binary and Media demonstration files are available from the STM32H7B3I-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/STM32H7B3I-DK.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h7b3i-dk_demo.zip

@par Keywords

Graphics, Connectivity, WiFi, TouchGFX, FreeRTOS, FatFS, Demonstration, Clock, Weather 

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case, it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"



 */
 
