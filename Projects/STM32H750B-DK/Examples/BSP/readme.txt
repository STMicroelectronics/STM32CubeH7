/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    BSP/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

@par Example Description 

This example provides a description of how to use the different BSP drivers. 

In this example, the code is executed from QSPI external memory while data are in internal
SRAM memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using Tamper push-button. 

 ** Push the User button to start first Test.  
Green Led (LED1) will blink between each test.Press user key to start another test:

 ** Touchscreen demo1 **
Use touchscreen functionality to select or activate colored circle inside a rectangle.

 ** Touchscreen demo2 **
Use touchscreen functionality to select 2 touch points, their positions will be
captured and displayed on the LCD.

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** AUDIO PLAY **
This example shows how to play an audio file using the DMA circular mode and 
how to handle the buffer update.
Plug a headphone to hear the sound 
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in 
       the flash at @0x90080000 using STM32CubeProgrammer utility

 ** AUDIO RECORD **
This example shows how to record an audio stream from microphones mounted on STM32H750B-DISCO
and play it using the DMA circular mode and how to handle the buffer update.
Plug a headphone to hear the sound.  

** MMC **
This example shows how to erase, write and read the MMC. 

 ** SDRAM **
This example shows how to erase, write and read the SDRAM memory.

 ** SDRAM DMA **
This example shows how to erase, write and read the SDRAM memory in DMA mode.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

BSP, Board support package, DMA, Audio play, Headphone, Audio record, microphone, Touchscreen, LCD, LTDC,
MMC, SDRAM, QSPI, QUADSPI, Display, LED, Button

@par Directory contents 

  - BSP/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  - BSP/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - BSP/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7
  - BSP/Inc/main.h                 Main program header file for Cortex-M7
  - BSP/Inc/stlogo.h               Image used for BSP example
  - BSP/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7  
  - BSP/Src/main.c                 Main program for Cortex-M7
  - BSP/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M7   
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/audio_play.c           Play audio 
  - BSP/Src/audio_record.c         Record and Play audio
  - BSP/Src/sdram.c                SDRAM Read/Write features
  - BSP/Src/sdram_dma.c            SDRAM Read/Write DMA mode

  
@par Hardware and Software environment  

  - This example runs on STM32H750Bxx device.
    
  - This example has been tested with STMicroelectronics STM32H750B-DISCO 
    and can be easily tailored to any other supported device and development board.
    
 
@par How to use it ? 


 - Use STM32CubeProgrammer, available on www.st.com or any other in system programming
   tool to load "BSP/Binary/audio_sample_tdm.bin" file to the external QSPI flash 
   at the address 0x90080000 ( on STM32CubeProgrammer : Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO").

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc template.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with "Examples/BSP" example (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load the BSP example to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file relative to this example
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

     @note Make sure that the tool flash loader does not erase or overwrite the
        loaded audio file at address 0x90080000 by limiting the application
        end address to 0x9007FFFF. This is already done for the example project

 - Connect a headphone and a speaker to the audio jack connectors (CN9).


 */
