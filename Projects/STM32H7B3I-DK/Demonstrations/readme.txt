/**
  @page Demonstrations Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32H7B3I-DK Demonstration
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

The STM32Cube demonstration platform comes on top of the STM32Cube(TM) as a firmware
package that offers a full set of software components based on a modular architecture.
All modules can be reused separately in standalone applications.
The STM32Cube demonstration platform is built around the FreeRTOS real-time operating
system and offers a large scope of use cases based on the STM32Cube HAL BSP and several
middleware components.

The demonstration's executables STM32Cube_Demo-STM32H7B3I-DK-V1.0.0.hex can be flashed
to the target using the STM32CubeProgrammer.

Multimedia content is also available in the Storage(SDCard) folder within the demonstration 
package and is to be copied on mass storage using the host PC. Below is the listing of these 
additional files.

Storage
\---------
	Audio
	\---
	   |   Clear Day.wav
	   |   music.wav
	   |   Corporate Inspiration.mp3
	   |   ST Life augmented.mp3

	Pictures\BMP_480x272
	\---
	   |   image06.bmp
	   |   image07.bmp
	   |   image08.bmp

	Video
	\---
	   |   video0_480_272.avi
	   |   video1_480_272.avi
	   |   video2_480_272.avi
	   |   video3_480_272.avi

    cw_cfg.ini: clock and weather configuration file


Binary files:
  STM32Cube_Demo-STM32H7B3I-DK-V1.0.0.hex: STM32H7B3I-DK Demonstration Full binary file
  program_hexfile.bat: Allows to directly program this hex file into the STM32H7B3I-DK board.
  This script will invoke the STM32CubeProgrammer with the correct OSPI flasher 
 (MX25LM51245G_STM32H7B3I_DISCO)

The demonstration starts Menu Launcher by displaying the supported demos icons.
The user can slide the icons bar and select between six demonstrations:
STemWin, TouchGFX , Embedded Wizard, Audio player, Video player and Clock and Weather
When a Sub-Demo is selected, the Menu Launcher will Display demo overview and jump to the selected Sub-Demo
when selecting start button.
For more details on the supported demos, please refer to the readme.txt file for each demo.

@par Keywords

Demonstrations, Adafruit, Shield, TFT, Joystick, SD Card, Display 

@par Hardware and Software environment

  - This demonstration runs on STM32H7B3xx devices.    
  - This demonstration has been tested with STMicroelectronics STM32H7B3I-DK boards and can be easily tailored to 
    any other supported device and development board.
     
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

-> The internal Flash and the external OSPI are now programmed and the demonstration is shown on the board.


The binary and Media demonstration files are available from the STM32H7B3I-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/STM32h7b3i-DK.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h7b3i-dk_demo.zip

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
 
