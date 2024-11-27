/**
  @page FatFs_uSD_RTOS    FatFs with uSD disk drive application

  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_RTOS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD disk drive application
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
  */

@par Application Description

This application provides a description on how to use STM32Cube firmware with FatFs
middleware component as a generic FAT file system module, in order to develop an
application exploiting FatFs offered features with uSD disk drive
configuration.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735XX Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

The application is based on writing and reading back a text file from a drive,
and it's performed using FatFs APIs to access the FAT volume as described
in the following steps:

 - Link the uSD disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Close the open text file;
 - Check on read data from text file;

It is worth noting that the application manages any error occurred during the
access to FAT volume, when using FatFs APIs.

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file ffconf.h available under the project includes
directory, in a way to fit the application requirements.

STM32H735G-DK's board's LEDs can be used to monitor the application status:
  - LED1 toggles every 200ms when the application runs successfully.
  - LED1 ON when FatFs is  correctly initialized.
  - LED2 ON when any error occurs.
  - LED2 toggles every 200ms when the uSD is unplugged.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about FatFs implementation on STM32Cube, please refer to UM1721 "Developing Applications
on STM32Cube with FatFs".

@par Keywords

FatFs, SDMMC, SD Card, File system, FAT Volume, Format, Mount, Read, Write, RTOS

@par Directory contents

  - FatFs/FatFs_uSD_RTOS/Core/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_uSD_RTOS/Core/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_uSD_RTOS/Core/Inc/main.h                        Main program header file
  - FatFs/FatFs_uSD_RTOS/Core/Inc/stm32h735g_discovery_conf.h     BSP configuration file    (FatFs)
  - FatFs/FatFs_uSD_RTOS/FATFS/Target/sd_diskio.h               FatFS uSD diskio driver header file
  - FatFs/FatFs_uSD_RTOS/FATFS/Target/ffconf.h                  FAT file system module configuration file
  - FatFs/FatFs_uSD_RTOS/Core/Src/stm32h7xx_it.c          Interrupt handlers
  - FatFs/FatFs_uSD_RTOS/Core/Src/stm32h7xx_hal_msp.c     MSP configuration file
  - FatFs/FatFs_uSD_RTOS/Core/Src/main.c                        Main program
  - FatFs/FatFs_uSD_RTOS/FATFS/Target/sd_diskio.c               FatFS uSD diskio driver implementation
  - FatFs/FatFs_uSD_RTOS/Core/Src/system_stm32h7xx.c      stm32h7xx system clock configuration file
  - FatFs/FatFs_uSD_RTOS/FATFS/App/app_fatfs.c                  FatFs application code
  - FatFs/FatFs_uSD_RTOS/FATFS/App/app_fatfs.h                  Header file for App_fatfs.c file

@par Hardware and Software environment

  - This application runs on STM32H735xx  devices.

  - This application has been tested with STMicroelectronics STM32H735G-DK
    evaluation boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */