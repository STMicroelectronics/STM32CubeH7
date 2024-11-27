/**
  @page FatFs_uSD_DMA_RTOS   FatFs with uSD card drive in RTOS mode application
 
  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_DMA_RTOS/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD/DMA card drive in RTOS mode application
  ******************************************************************************
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

This application provides a description on how to use STM32Cube firmware with FatFs 
middleware component as a generic FAT file system module, in order to develop an
application exploiting FatFs offered features with microSD drive in RTOS mode 
configuration. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals and initialize the Flash interface.
The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as time base for the HAL drivers.
This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms. 
The systick is then used for FreeRTOS time base.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The application is based on writing and reading back a text file from a drive,
it creates a normal priority thread and it's performed using FatFs APIs to 
access the FAT volume as described in the following steps: 

 - Link the uSD/DMA disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Check on read data from text file;
 - Close the open text file;
 - Unlink the uSD disk I/O driver.

It is worth noting that the application manages any error occurred during the 
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file “ffconf.h” available under the project includes 
directory, in a way to fit the application requirements. 

It is possible to dynamically hotplug/unplug the uSD, as the application is handling
the insert/remove events.

@note: for some uSD's, replacing it  while the application is running makes the application
       fail. It is recommended to reset the board using the "Reset button" after replacing
       the uSD.

STM32 Eval board's LEDs can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to 
      the OS resources memory requirements of the application with +10% margin and rounded to the 
      upper Kbyte boundary.

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

FatFs, SDMMC, SD Card, File system, FAT Volume, Format, Mount, Read, Write, RTOS

@par Directory contents 
 
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/stm32h7xx_hal_conf.h                HAL configuration file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/stm32h7xx_it.h                      Interrupt handlers header file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/main.h                              Main program header file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/ffconf.h                            FAT file system module configuration file   
  - FatFs/FatFs_uSD_DMA_RTOS/Src/usbh_conf.h                         usb otg low level configuration header file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/FreeRTOSConfig.h                        FreeRTOS configuration file
  - FatFs/FatFs_uSD_DMA_RTOS/Inc/sd_diskio_dma_rtos.h                    uSD/DMA diskio driver header file
  - FatFs/FatFs_uSD_DMA_RTOS/Src/stm32h7xx_it.c                          Interrupt handlers
  - FatFs/FatFs_uSD_DMA_RTOS/Src/main.c                              Main program
  - FatFs/FatFs_uSD_DMA_RTOS/Src/system_stm32h7xx.c                  stm32h7xx system clock configuration file
  - FatFs/FatFs_uSD_DMA_RTOS/Src/stm32h7xx_hal_timebase_tim.c        HAL time base functions.
  - FatFs/FatFs_uSD_DMA_RTOS/Src/usbh_diskio.c                       the uSD/DMA FatFs diskio driver
  - FatFs/FatFs_uSD_DMA_RTOS/Src/usbh_conf.c                         usb otg low level configuration file

                                                                                                                                   
 
@par Hardware and Software environment

  - This application runs on STM32H743xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32H743I-EVAL Set-up
    - Connect a uSD Card to the MSD connector (CN13).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

@note
the uSD used in the application gets formatted, please make sure to backup any relevant data before using it.


 */
