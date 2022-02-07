/**
  @page VideoPlayer   STM32H7B3I-DK Video Player Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H7B3I-DK Video Player Demonstration
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

 The video player module provides a video solution based on the STM32H7xxx and the Touch GFX APIs.
 You can use the *.avi files provided under "Utilities/Media/Video" or any other ones.

@par Video Player Demonstration feature
  - Support movies in AVI mjpeg/wav format (audio and video)
  - Display total and elapsed times  
  - Info bar and video controls are auto-hidden after few seconds on non interaction
  - Display all supported videos available on media storage with preview
  - Volume control
  - Support repeat all and Shuffle


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
   
5- copy Only the avi files format with 480x272 pixels as resolution provided under "/Media/Video" in a SD card storage at video folder
6- Plug the Media storage in CN4.
-> The internal Flash and the external OSPI are now programmed and the demonstration is shown on the board.


The binary and Media demonstration files are available from the STM32H7B3I-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/STM32H7B3I-DK.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h7b3i-dk_demo.zip

@par Keywords

Graphics, TouchGFX, FreeRTOS, FatFS, Demonstration, Video, Player, AVI, MJPEG, JPEG

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
 
