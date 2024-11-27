/**
  @page FatFs_Dual_Instance   Dual FatFs instances using uSD card drive on CM7 and USB Host on CM4

  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_Dual_Instance/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs Dual instance application
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

This application provides a description on how to use STM32Cube™ firmware with FatFs
middleware component as a generic FAT file system module. The objective is to develop an
application making the most of the features offered by FatFs to configure a microSD and USB drives.

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
 
 two FatFs instances, completely independent, run on Cortex M7 and Cortex M4.
 The first accesses the uSD device and the second uses the USB host IP.Below
 are  the scenarioes for each application.

the Core_CM7 application:
 - Link the uSD disk I/O driver.
 - Register the file system object (mount) to the FatFs module for the uSD drive.
 - Create a FAT file system (format) on the uSD drive.
 - Create and Open new text file object with write access.
 - Write data to the text file.
 - Close the open text file.
 - Open text file object with read access.
 - Read back data from the text file.
 - Close the open text file.
 - Check on read data from text file.
 - LED1 is on on success, LED3 is on otherwise.
 - Unlink the uSD disk I/O driver.

the Core_CM4 application:
 - Link the USB disk I/O driver.
 - Register the file system object (mount) to the FatFs module for the USBdrive.
 - Create a FAT file system (format) on the USB drive.
 - Create and Open new text file object with write access.
 - Write data to the text file.
 - Close the open text file.
 - Open text file object with read access.
 - Read back data from the text file.
 - Close the open text file.
 - Check on read data from text file.
 - LED4 is on on success, LED2 is on otherwise.
 - Unlink the USB disk I/O driver.


It is worth noting that the application manages any error occurred during the
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file “ffconf.h” available under the project includes
directory, in a way to fit the application requirements.

STM32 Eval board's LEDs can be used to monitor the application status:
  - LED1 and LED4 are ON when repsectively the CM7 and the CM4 applications run successfully.
  - LED3 and LED2 are ON when any error occurs on repectively the CM7 or the CM4 application.

It is possible to dynamically hot plug/unplug the uSD card and the usb key as
the applications will handle the plug/unplug events.

@note: for some uSD's, replacing it  while the application is running makes the application
       fail. It is recommended to reset the board using the "Reset button" after replacing
       the uSD.

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

FatFs, SDMMC, USB, Dual drive, microSD, uSD, File system, FAT Volume, Format, Read, Write

@par Directory contents

  - FatFs/FatFs_Dual_Instance/CM4/Inc/ffconf.h               CM4 FAT file system module configuration file
  - FatFs/FatFs_Dual_Instance/CM4/Inc/main.h                 CM4 Main program header file
  - FatFs/FatFs_Dual_Instance/CM4/Inc/stm32h7xx_hal_conf.h   CM4 HAL configuration file
  - FatFs/FatFs_Dual_Instance/CM4/Inc/stm32h7xx_it.h         CM4 Interrupt handlers header file
  - FatFs/FatFs_Dual_Instance/CM4/Inc/usbh_conf.h            USB Host configuration header file
  - FatFs/FatFs_Dual_Instance/CM4/Inc/usbh_diskio.h          USB Host FatFs header file
  - FatFs/FatFs_Dual_Instance/CM4/Src/main.c                 Cortex M4 main program
  - FatFs/FatFs_Dual_Instance/CM4/Src/stm32h7xx_it.c         Cortex M4 Interrupt handlers header file
  - FatFs/FatFs_Dual_Instance/CM4/Src/usbh_conf.c            USB host configuration file
  - FatFs/FatFs_Dual_Instance/CM4/Src/usbh_diskio.c          USB Host FatFs driver
  - FatFs/FatFs_Dual_Instance/CM7/Inc/ffconf.h               CM7 FAT file system module configuration file
  - FatFs/FatFs_Dual_Instance/CM7/Inc/main.h                 CM7 Main program header file
  - FatFs/FatFs_Dual_Instance/CM7/Inc/sd_diskio_dma.h        uSD diskio_dma header file
  - FatFs/FatFs_Dual_Instance/CM7/Inc/stm32h7xx_hal_conf.h   CM7 HAL configuration file
  - FatFs/FatFs_Dual_Instance/CM7/Inc/stm32h7xx_it.h         Cortex M7 Interrupt handlers header file
  - FatFs/FatFs_Dual_Instance/CM7/Src/main.c                 Cortex M7 main program
  - FatFs/FatFs_Dual_Instance/CM7/Src/sd_diskio_dma.c        FatFs uSD/DMA diskio driver
  - FatFs/FatFs_Dual_Instance/CM7/Src/stm32h7xx_it.c         Cortex M7 Interrupt handlers 
  - FatFs/FatFs_Dual_Instance/Common/Src/system_stm32h7xx.c  stm32h7xx system clock configuration file



@par Hardware and Software environment

  - This application runs on STM32H747xx devices.

  - This application has been tested with STMicroelectronics STM32H747I-EVAL
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32H747I-EVAL Set-up
    - Connect a uSD Card to the MSD connector (CN13).
    - Plug the USB key into the STM32H747I_EVAL board through 'USB micro A-Male to A-Female'
      cable to the connector CN14 (OTG_HS)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild CM7 and CM4 applications and load both images into target memory
 - reset the board to get the applications running.

@note
Both the uSD and the USB thumb get formatted, please make sure to backup any relevant data before using it.


 */
