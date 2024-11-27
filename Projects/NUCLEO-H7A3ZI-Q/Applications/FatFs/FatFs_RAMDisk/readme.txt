/**
  @page FatFs_RAMDisk   FatFs with RAM disk drive application
 
  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_RAMDisk/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FatFs with RAM disk drive application
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
  */

@par Application Description

This application provides a description on how to use STM32Cube firmware with FatFs 
middleware component as a generic FAT file system module, in order to develop an
application exploiting FatFs offered features with RAM disk (SDRAM) drive 
configuration. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 280 MHz.
 
The application is based on writing and reading back a text file from a drive,
and it's performed using FatFs APIs to access the FAT volume as described
in the following steps: 

 - Link the SDRAM disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the SDRAM drive;
 - Create a FAT file system (format) on the SDRAM drive;
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
 
Nucleo-H7A3ZI-Q's board's LEDs can be used to monitor the application status:
  - LED1 ON when the application runs successfully.
  - LED3 ON when any error occurs. 


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
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7 Series"

For more details about FatFs implementation on STM32Cube, please refer to UM1721 "Developing Applications 
on STM32Cube with FatFs".

@par Keywords

FatFs, RAM disk, SDRAM, File system, FAT Volume, Format, Mount, Read, Write

@par Directory contents
 
  - FatFs/FatFs_RAMDisk/Core/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_RAMDisk/Core/Inc/stm32h7xx_nucleo_conf.h BSP configuration file
  - FatFs/FatFs_RAMDisk/Core/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_RAMDisk/Core/Inc/main.h                  Main program header file
  - FatFs/FatFs_RAMDisk/FATFS/Target/sdram_diskio.h      FatFS sdram diskio driver header file
  - FatFs/FatFs_RAMDisk/FATFS/Target/ffconf.h            FAT file system module configuration file   
  - FatFs/FatFs_RAMDisk/Core/Src/stm32h7xx_it.c          Interrupt handlers
  - FatFs/FatFs_RAMDisk/Core/Src/stm32h7xx_hal_msp.c     MSP configuration file
  - FatFs/FatFs_RAMDisk/Core/Src/main.c                  Main program
  - FatFs/FatFs_RAMDisk/FATFS/Target/sdram_diskio.c      FatFS sdram diskio driver implementation
  - FatFs/FatFs_RAMDisk/Core/Src/system_stm32h7xx.c      stm32h7xx system clock configuration file
  - FatFs/FatFs_RAMDisk/FATFS/App/app_fatfs.c            FatFs application code
  - FatFs/FatFs_RAMDisk/FATFS/App/app_fatfs.h            Header file for App_fatfs.c file    
 
@par Hardware and Software environment

  - This application runs on STM32H7A3xxQ devices.
    
  - This application has been tested with STMicroelectronics Nucleo-H7A3ZI-Q  
    evaluation boards and can be easily tailored to any other supported device 
    and development board.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */