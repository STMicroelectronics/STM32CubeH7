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
application exploiting FatFs offered features with multidrives (USB Disk, uSD) 
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

The 48 MHz clock for the USB FS can be derived from  the following source:
    PLL3 clock (clocked by the HSE): If the USB uses the PLL3 as clock source, the PLL3 VCO clock must be programmed
    to output 336 MHz frequency, and PLL3Q divider set to 7 (USBCLK = PLL3VCO/PLL3Q).

The application is based on writing and reading back a text file from two drives,
and it's performed using FatFs APIs to access the FAT volume as described
in the following steps:

 - Link the USBH and uSD disk I/O drivers;
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
written text file is available on the uSD card and the usb disk.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file “ffconf.h” available under the project includes 
directory, in a way to fit the application requirements. In this case, two drives
are used, so the Max drive number is set to: _VOLUMES = 2 in “ffconf.h” file.
         
STM32 Eval board's LEDs can be used to monitor the application status:
  - LED4 (BLUE) is ON when the usD operations are done successfully.
  - LED3 (RED)  is ON otherwise.

  - LED1 (GREEN)  is ON when the USB disk operations are successfull
  - LED2 (ORANGE) is ON otherwise


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note the application is using the uSD internal DMA, that can't access the  DTCM memory @0x20000000,
thus it is using the D1 SRAM @0x24000000.

@note it is possible to dynamically plug/unplug either the uSD or the USB disk as the application is managing
the plug/unplug events.

@note: for some uSD's, replacing it  while the application is running makes the application
       fail. It is recommended to reset the board using the "Reset button" after replacing
       the uSD.

For more details about FatFs implementation on STM32Cube, please refer to UM1721 "Developing Applications 
on STM32Cube with FatFs".


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

FatFs, SDMMC, SD Card, uSD, USB Disk, File system, FAT Volume, Format, Mount, Read, Write, multidrives


@par Directory contents


  - FatFs/FatFs_MultiDrives/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - FatFs/FatFs_MultiDrives/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - FatFs/FatFs_MultiDrives/Inc/main.h                        Main program header file
  - FatFs/FatFs_MultiDrives/Inc/ffconf.h                      FAT file system module configuration file   
  - FatFs/FatFs_MultiDrives/Inc/usbh_diskio.h                 FatFs usb diskio driver header file
  - FatFs/FatFs_MultiDrives/Inc/usbh_conf.h                   USBHost library config header file
  - FatFs/FatFs_MultiDrives/Inc/sd_diskio_dma_rtos.h          FatFs uSD diskio driver header file
  - FatFs/FatFs_MultiDrives/Inc/FreeRTOSConfig.h              FreeRTOS configuration file   
  - FatFs/FatFs_MultiDrives/Src/stm32h7xx_it.c                Interrupt handlers
  - FatFs/FatFs_MultiDrives/Src/main.c                        Main program
  - FatFs/FatFs_MultiDrives/Src/sd_diskio_dma_rtos.c          FatFS uSD dikio driver
  - FatFs/FatFs_MultiDrives/Src/usbh_diskio.c                 FatFS usb diskio driver
  - FatFs/FatFs_MultiDrives/Src/stm32h7xx_hal_timebase_tim.c  Timer based functions 
  - FatFs/FatFs_MultiDrives/Src/system_stm32h7xx.c            stm32h7xx system clock configuration file
  - FatFs/FatFs_MultiDrives/Src/usbh_conf.c                   USB Host config implementation

 @par Hardware and Software environment

  - This application runs on stm32h743xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.  

  - STM32H743I-EVAL Set-up
   - Connect a uSD Card to the MSD connector (CN13).
   - Connect a USB disk  to either the USB-FS connector (CN18) or the USB-HS one(CN14).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
