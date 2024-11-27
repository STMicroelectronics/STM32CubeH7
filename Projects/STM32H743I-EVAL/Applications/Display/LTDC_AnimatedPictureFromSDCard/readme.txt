/**
  @page LTDC_AnimatedPictureFromSDCard LTDC animated picture from SD card application
  
  @verbatim
  ******************************************************************************
  * @file    Display/LTDC_AnimatedPictureFromSDCard/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC animated picture from SD card application.
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

  This application describes how to display an animated picture on LCD saved under
  microSD.


  At the beginning of the main program the HAL_Init() function is called to reset
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

  The animated picture is the display of an images sequence with a determined 
  frequency to be seen like a animated image.
 
  The user has to copy the two directories "BACK" and "TOP" available under 
  "/Utilities/Media/Pictures/Animated" to the micro SD card root. 
 
  * Background picture display
  ----------------------------
  At the beginning, the program checks the existence of an SD card. If the SD card
  is not inserted, a warning message is displayed on the LCD :" Please insert SD Card. "
  
  Once the LCD, SD card and file system initialized and configured, a check
  of the existence and the content of the "/BACK" directory is done.

  * Foreground animated pictures display
  -------------------------------------  
  A content check of the "/TOP" directory is done and the number of ".BMP" files 
  is retained.

@note : The maximum number of BMP files is fixed at 25. It can be raised 
        until reaching the maximum of SD card memory space.

		
  The following steps are performed to scroll all the images stored in the 
  SD Card :

  - The foreground layer is set, the image copied from SD card (from "/TOP" directory)
    to intermediate SDRAM memory space and then copied to LCD frame buffer. 
 
  - The color keying feature is applied to remove the bottom of foreground layer (transparent)
    and then replaced by the background layer.
 
  - Jump to next image  

@note :  
  => If the "/Foreground" directory is empty, a warning message is displayed on the LCD : 
     "  No Bitmap files...  "
    
  => If the file type stored in the "/BACK" or "/TOP" directories is not supported,
     a warning message is displayed on the LCD : " file type not supported. "

  => If the SD card is removed while running the application, A reset is needed.
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


-  LED3 is ON when the uSD disk I/O driver is not Linked;


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

Graphics, Display, LTDC, Pictures, SDMMC, SD Card, BMP, Background, Foreground, Layers,

@par Directory contents

    - Display/LTDC_AnimatedPictureFromSDCard/Inc/main.h                 Main configuration file
    - Display/LTDC_AnimatedPictureFromSDCard/Inc/stm32h7xx_it.h         Interrupt handlers header file
    - Display/LTDC_AnimatedPictureFromSDCard/Inc/stm32h7xx_hal_conf.h   HAL Configuration file 
    - Display/LTDC_AnimatedPictureFromSDCard/Inc/ffconf.h               FAT file system module configuration file
    - Display/LTDC_AnimatedPictureFromSDCard/Inc/fatfs_storage.h        Header for fatfs_storage.c
    - Display/LTDC_AnimatedPictureFromSDCard/Inc/sd_diskio.h            Header for sd_diskio.c module
    - Display/LTDC_AnimatedPictureFromSDCard/Src/main.c                 Main program
    - Display/LTDC_AnimatedPictureFromSDCard/Src/stm32h7xx_hal_msp.c    HAL MSP module 
    - Display/LTDC_AnimatedPictureFromSDCard/Src/fatfs_storage.c        Storage (FatFs) driver
    - Display/LTDC_AnimatedPictureFromSDCard/Src/sd_diskio.c            SD Disk I/O driver
    - Display/LTDC_AnimatedPictureFromSDCard/Src/stm32h7xx_it.c         Interrupt handlers
    - Display/LTDC_AnimatedPictureFromSDCard/Src/system_stm32h7xx.c     STM32H7xx system clock configuration file


@par Hardware and Software environment  

  - This application runs on STM32H743xx/STM32H753xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
@par How to use it ?

In order to make the program work, you must do the following :
 - The two directories "BACK" and "TOP" under "/Utilities/Media/Pictures/Animated" 
   folder must be copied at the micro SD card root.
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
  

 */
                                   
