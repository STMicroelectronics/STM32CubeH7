/**
  @page FatFs_USBDisk_Standalone   FatFs with USB disk drive application
 
  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_USBDisk_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with USB disk drive application
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
middleware component as a generic FAT file system module and STM32 USB On-The-Go
(OTG) host library, in Full Speed (FS) and High Speed (HS) modes, 
in order to develop an application exploiting FatFs offered features with USB disk drive configuration.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

The 48 MHz clock for the USB FS can be derived from the following source:
    PLL3 clock (clocked by the HSE): If the USB uses the PLL3 as clock source, the PLL3 VCO clock must be programmed
    to output 336 MHz frequency, and PLL3Q divider set to 7 (USBCLK = PLL3VCO/PLL3Q).
           
The application is based on writing a text file to a drive, and it's performed 
using FatFs APIs to access the FAT volume as described in the following steps: 

 - Link the USB Host disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the USB drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Unlink the USB Host disk I/O driver.
 
It is worth noting that the application manages any error occurred during the 
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the USB disk.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file “ffconf.h” available under the project includes 
directory, in a way to fit the application requirements. 

STM32 Eval board's LEDs can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


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

Connectivity, FatFS, USB Host, USB OTG, FAT, File system, Full Speed, High Speed, HS-IN-FS

@par Directory contents
 
  - FatFs/FatFs_USBDisk_Standalone/Inc/STM32h7xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_USBDisk_Standalone/Inc/STM32h7xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_USBDisk_Standalone/Inc/main.h                  Main program header file
  - FatFs/FatFs_USBDisk_Standalone/Inc/ffconf.h                FAT file system module configuration file   
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_conf.h             usb otg low level configuration header file
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_diskio.h           FatFs diskio driver header file
  - FatFs/FatFs_USBDisk_Standalone/Src/STM32h7xx_it.c          Interrupt handlers
  - FatFs/FatFs_USBDisk_Standalone/Src/main.c                  Main program
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_conf.c             usb otg low level configuration file
  - FatFs/FatFs_USBDisk_Standalone/Src/usbh_diskio.c           FatFs usb diskio driver
  - FatFs/FatFs_USBDisk_Standalone/Src/system_STM32h7xx.c      STM32H7xx system clock configuration file
         
 
@par Hardware and Software environment

  - This application runs on STM32H743XXX devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32H743I-EVAL Set-up
    - Plug the USB key into the STM32H743I-EVAL board through 'USB micro A-Male 
      to A-Female' cable to the connector:
      - CN14 : to use USB High Speed (HS) 
      - CN18 : to use USB Full Speed (FS) with embedded PHY(U8)
    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32H743I-EVAL_USBH-HS: to configure the project for STM32H7xx devices using USB OTG HS peripheral
   - STM32H743I-EVAL_USBH-FS: to configure the project for STM32H7xx devices using USB OTG FS peripheral
 - Run the application

 @note
 the USB flash disk gets formatted by the application, please make sure to backup any relevant data before using it.

 */
 
