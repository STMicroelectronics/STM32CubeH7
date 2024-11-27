/**
  @page Demonstrations NUCLEO-H723ZG

  @verbatim
  ******************************************************************************
  * @file    Demonstrations/readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32H7xx-Nucleo Demonstrations
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

Demonstration firmware based on STM32Cube. This demonstration helps you to discover
STM32 Cortex-M devices that are plugged onto your STM32 Nucleo board

This project is configured for STM32H723xx devices using STM32CubeH7 HAL and running on
NUCLEO-H723ZG board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz
The HCLK for D1 Domain AXI peripherals, D2 Domain AHB peripherals and D3 Domain AHB  peripherals at 260 MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 130 MHz.
Below you find the sequence to discover the demonstration :

 - Check the availability of adafruit 1.8" TFT shield on top of STM32 Nucleo
   board. This is done by reading the state of IO PF.03 pin (mapped to Joystick
   available on adafruit 1.8" TFT shield). If the state of PF.03 is high then
   the adafruit 1.8" TFT shield is available.

   - Adafruit 1.8" TFT shield is not available:
     LED2 is toggling with a first frequency equal to ~1Hz.
     Pressing User button lets LED2 toggling with a second frequency equal to ~5Hz.
     Pressing User button again, changes LED2 toggling frequency to ~10Hz.
     This is done in an infinite loop.

   - Adafruit 1.8" TFT shield is available:
     A menu will be displayed on TFT. Follow instructions below:

     - Joystick DOWN to continue menu display
     - Choose the desired display mode: Press Joystick DOWN for automatic mode
       or Joystick UP for manual mode.

       - Manual Mode selected:
         Images available on SD Card, are displayed by pressing Joystick RIGHT
         to display next image or Joystick LEFT to display previous one.
         Pressing long (~1s) Joystick SEL, switches display mode to automatic one.

       - Automatic Mode selected:
         Images available on SD Card are displayed sequentially in a forever loop.


It is worth noting that the application manages some errors occurred during the
access to uSD card to parse bmp images:

 - If SD Card is not FAT formatted, a message will be displayed on TFT.
   In this case, format the SD card and put into its root directory the .bmp
   files available within the "BMP_128x160" folder under "Utilities/Media/Pictures"

 - If the SD card is empty or it contains files other than bitmap image, a message will
   be displayed on TFT mentioning that no image found in the SD card.
   In this case, put into the SD card's root directory the .bmp
   files available within the "BMP_128x160" folder under "Utilities/Media/Pictures"
   which are respecting the following criteria:
    o Dimensions: 128x160
    o Width: 128 pixels
    o Height: 160 pixels
    o Bit depth: 16
    o Item type: BMP file
    o The name of the bmp image file must not exceed 11 characters (including
      .bmp extension).

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decreases critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
	  the ITCM-RAM.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords
 
Demonstrations, Adafruit, Shield, TFT, Joystick, SD Card, Display

@par Directory contents

  - Demonstrations/Inc/fatfs_storage.h         Header for fatfs_storage.c module
  - Demonstrations/Inc/ffconf.h                FAT file system module configuration file
  - Demonstrations/Inc/main.h                  Header for main.c module
  - Demonstrations/Inc/sd_diskio.h             Header for sd_diskio.c module
  - Demonstrations/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - Demonstrations/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - Demonstrations/Inc/stm32h7xx_nucleo_bus.h  Bus header file
  - Demonstrations/Inc/stm32h7xx_nucleo_conf.h Nucleo configuration file
  - Demonstrations/Inc/adafruit_802_conf.h     Adafruit Shield configuration file

  - Demonstrations/Src/fatfs_storage.c         Storage (FatFs) driver
  - Demonstrations/Src/main.c                  Main program
  - Demonstrations/Src/sd_diskio.c             SD Disk I/O driver
  - Demonstrations/Src/stm32h7xx_it.c          Interrupt handlers
  - Demonstrations/Src/system_stm32h7xx.c      STM32H7xx system source file
  - Demonstrations/Src/stm32h7xx_nucleo_bus.c  Bus IO operations file


@par Hardware and Software environment

  - This demonstration runs on STM32H723xx devices.

  - This demonstration has been tested with STMicroelectronics NUCLEO-H723ZG
    boards and can be easily tailored to any other supported device
    and development board.

  - A SDSC microSD card must contain the .bmp files available within the
    "BMP_128x160" folder under "Utilities/Media/Pictures"

  - Adafruit 1.8" TFT shield must be connected on CN7,CN8, CN9 and CN10 Arduino connectors,
    for more details please refer to UM1726.

For more details about the adafruit 1.8" TFT shield, please visit:
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the demonstration



 */
