/**
  @page FatFs_MultiDrives   FatFs with multi drives application

  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_MultiDrives/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with multi drives application
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

@par Application Description

How to use STM32Cube firmware with FatFs middleware component as a generic
FAT file system module. This example develops an application that exploits
FatFs features, with multidrive (SDRAM, uSD) configurations.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 280 MHz.

The application is based on writing and reading back a text file from two drives,
and it's performed using FatFs APIs to access the FAT volume as described
in the following steps:

 - Link the RAM disk (SDRAM) and uSD disk I/O drivers;
 - Register the file system object (mount) to the FatFs module for both drives;
 - Create a FAT file system (format) on both logical drives;
 - Create and Open new text file objects with write access;
 - Write data to the text files;
 - Close the open text files;
 - Open text file objects with read access;
 - Read back data from the text files;
 - Check on read data from text file;
 - Close the open text files;

It is worth noting that the application manages any error occurred during the
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the uSD card and the RAM disk (SDRAM).

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file ffconf.h available under the project includes
directory, in a way to fit the application requirements. In this case, two drives
are used, so the Max drive number is set to: _VOLUMES = 2 in ffconf.h file.

It is possible to dynamically hot plug/unplug the uSD card, as the application will
handle the plug/unplug events.


@note: for some uSD's, replacing it  while the application is running makes the application
       fail. It is recommended to reset the board using the "Reset button" after replacing
       the uSD.

STM32H7B3I-DK's LED can be used to monitor the application status:
  - LED2 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.
  - LED3 toggle when SD card is unplugged.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

FatFs, SDMMC, SD Card, SDRAM, File system, FAT Volume, Format, Mount, Read, Write, multidrives

@par Directory contents

  - FatFs/FatFs_uSD_Standalone/Core/Inc/stm32h7xx_hal_conf.h               HAL configuration file
  - FatFs/FatFs_uSD_Standalone/Core/Inc/stm32h7xx_it.h                     Interrupt handlers header file
  - FatFs/FatFs_uSD_Standalone/Core/Inc/main.h                             Header for main.c module
  - FatFs/FatFs_uSD_Standalone/Core/Src/stm32h7xx_it.c                     Interrupt handlers
  - FatFs/FatFs_uSD_Standalone/Core/Src/main.c                             Main program
  - FatFs/FatFs_uSD_Standalone/Core/Src/system_stm32h7xx.c                 STM32H7xx system source file
  - FatFs/FatFs_uSD_Standalone/FATFS/Target/ffconf.h                       FAT file system module configuration file
  - FatFs/FatFs_uSD_Standalone/FATFS/Target/sd_diskio.h                    uSD diskio header file
  - FatFs/FatFs_uSD_Standalone/FATFS/Target/sd_diskio.c                    FatFs uSD diskio driver
  - FatFs/FatFs_RAMDisk/FATFS/Target/sdram_diskio.h                        FatFS sdram diskio driver header file
  - FatFs/FatFs_RAMDisk/FATFS/Target/sdram_diskio.c                        FatFS sdram diskio driver implementatio
  - FatFs/FatFs_uSD_Standalone/Core/Inc/stm32h7b3i_discovery_conf.h        BSP configuration file
  - FatFs/FatFs_uSD_Standalone/Core/Inc/is42s16800j_conf.h                 is42s16800j driver configuration file
  - FatFs/FatFs_uSD_Standalone/FATFS/App/app_fatfs.h                       Header file for App_fatfs.c file
  - FatFs/FatFs_uSD_Standalone/FATFS/App/app_fatfs.c                       FatFs application code

 @par Hardware and Software environment

  - This application runs on STM32H7B3XXQ devices

  - This application has been tested with STM32H7B3I-DK board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following:
 - Insert a microSD card in the board appropriate slot
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */
