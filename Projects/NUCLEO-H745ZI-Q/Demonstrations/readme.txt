/**
  @page Demonstrations Demonstrations NUCLEO-H745ZI-Q

  @verbatim
  ******************************************************************************
  * @file    Demonstrations/readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32H7xx-Nucleo Demo
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

Demonstration firmware based on STM32Cube. This example helps you to discover
STM32 Cortex-M devices that are plugged onto your STM32 Nucleo board

The NUCLEO-H745ZI-Q Demonstration shows how to use the Dual Core STM32H745ZI devices to
send notifications from Cortex-M7 to Cortex-M4.
The Cortex-M7 manages bmp images display on the Adafruit 1.8" TFT shield when plugged (from SD-card)
Each time a new image is displayed the Cortex-M7 sends notification to Cortex-M4 that blinks LED2.
If the Adafruit 1.8" TFT shield is not plugged then Cortex-M7 toggles LED1 at a given frequency (can be changed pressing user button)
and sends a notification to cortex-M4 that toggles LED2


The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain,
the access will not be safe until “clock ready flag” of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).


Below you find the sequence to discover the demonstration :

 - Check the availability of adafruit 1.8" TFT shield on top of STM32 Nucleo
   board. This is done by reading the state of IO PF.03 pin (mapped to JoyStick
   available on adafruit 1.8" TFT shield). If the state of PF.03 is high then
   the adafruit 1.8" TFT shield is available.

   - Adafruit 1.8" TFT shield is not available:
     Cortex-M7 toggles LED1 with a first frequency equal to ~1Hz.
     Pressing User button lets LED2 toggling with a second frequency equal to ~5Hz.
     Pressing User button again, changes LED1 toggling frequency to ~10Hz.
     This is done in an infinite loop.
     Each Time Cortex-M7 toggles LED1 it releases HW semaphore zero to notify the Cortex-M4.
     The Cortex-M4 ,in a infinite loop, wait for a notification from Cortex-M7 and once received
     toggles LED2 without any delay.
     As result the Cortex-M4 toogls LED2 infintly with the same frequency as Cortex-M7

   - Adafruit 1.8" TFT shield is available:
   Cortex-M7 Process :
   -------------------
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

       - Each time an new image is displayed the Cortex-M7 releases HW semaphore zero to notify the Cortex-M4.*

   Cortex-M4 Process :
   -------------------
     The Cortex-M4 ,in a infinite loop, wait for a notification from Cortex-M7 and once received
     set LED2 On, LED2 Off.
     As result, each time the Cortex-M7 display an new image, the Cortex-M4 blinks LED2

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


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process,
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@note The clock setting is configured to have the max product performance (max clock frequency)
      so not optimized in term of power consumption.

@note It is highly recommended to set the SPI parameter "MasterKeepIOState" to "SPI_MASTER_KEEP_IO_STATE_ENABLE"
      at init time. This configuration allows to avoid glitches on SPI lines especially the clock.
      Glitches that could lead to Master/Slave desynchronization. This config allows SPI Master to keep
      control on output state even if it is disabled.


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

Demonstrations,  multiprocessor, Adafruit, Shield, TFT, Joystick, SD Card, Display,

@par Directory contents

  - Demonstrations/Common/Src/system_stm32h7xx.c   STM32H7xx system configuration source file

  - Demonstrations/CM7/Inc/fatfs_storage.h         Header for fatfs_storage.c module
  - Demonstrations/CM7/Inc/ffconf.h                FAT file system module configuration file
  - Demonstrations/CM7/Inc/main.h                  Header for main.c module for Cortex-M7
  - Demonstrations/CM7/Inc/sd_diskio.h             Header for sd_diskio.c module
  - Demonstrations/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M7
  - Demonstrations/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M7
  - Demonstrations/CM7/Src/fatfs_storage.c         Storage (FatFs) driver
  - Demonstrations/CM7/Src/main.c                  Main program for Cortex-M7
  - Demonstrations/CM7/Src/sd_diskio.c             SD Disk I/O driver
  - Demonstrations/CM7/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M7

  - Demonstrations/CM4/Src/main.c                  Main program  for Cortex-M4
  - Demonstrations/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M4
  - Demonstrations/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - Demonstrations/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - Demonstrations/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - Demonstrations/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4

@par Hardware and Software environment

  - This demonstration runs on STM32H745ZI/STM32H755ZI devices.

  - This demonstration has been tested with STMicroelectronics NUCLEO-H745ZI-Q 
    board and can be easily tailored to any other supported device
    and development board.

  - A SDSC microSD card (capacity up to 4GB), must contain the .bmp files
    available within the "BMP_128x160" folder under "Utilities/Media/Pictures"

  - Adafruit 1.8" TFT shield must be connected on CN7,CN8, CN9 and CN10 Arduino connectors,
    for more details please refer to UM1726.

For more details about the adafruit 1.8" TFT shield, please visit:
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H745I_Nucleo_CM4 first then STM32H745I_Nucleo_CM7) :
     - Rebuild all files
     - Load images into target memory
 - Run the demonstration


 */

