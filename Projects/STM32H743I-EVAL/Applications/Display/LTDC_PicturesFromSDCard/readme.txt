/**
  @page LTDC_PicturesFromSDCard LTDC pictures from SD card application
  
  @verbatim
  ******************************************************************************
  * @file    Display/LTDC_PicturesFromSDCard/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC pictures from SD card application.
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

This application describes how to display pictures on LCD saved under SD card.
 
The user has to create a "Media" directory under the micro SD card root and
copy inside it the pictures stored in "/BMP_320x240" and "/BMP_480x272" folders provided under 
"/Utilities/Media/Pictures"
   
 
@note : The user can add pictures of his choice but the file type, resolution
        and pixel format described below must be respected.
  
  At the beginning, the program checks the existence of an SD card. If the SD card
  is not inserted, a warning message is displayed on the LCD :" Please insert SD Card. "
  Once the LCD, SD card and file systems initialized and configured, 
  a content check of the "Media" directory is done and the number of ".BMP" files 
  is retained.
  
@note : An associate LCD eval driver is used in this application 
  
@note : The maximum number of BMP files is fixed to 25. It can be raised 
        until reaching the maximum of SD card memory space.

  At the beginning of the main program the HAL_Init() function is called to reset
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

  The following steps are performed to scroll all the images stored in the 
  SD Card :
 
  Step1 :
  ------
  The foreground layer is set, the image copied from SD card to intermediate
  SDRAM memory and then copied to LCD frame buffer. The image is fully visible when 
  the level of transparency is increased until it becomes totally opaque (reaches 255).
 
  Press tamper button to display next image.
 
  Step2 :
  ------
  When the Tamper button is pressed the transparency of the foreground layer
  begins to decrease until becoming totally transparent and the background layer 
  is set, the image copied from SD card to intermediate SDRAM memory and then 
  copied to LCD frame buffer and finally the level of transparency of the 
  background layer increases until it reaches 255 (totally opaque). 
            
  Press tamper button to display next image.
 
  Step3 :
  ------
  When the Tamper button is pressed the transparency of the background layer
  begins to decrease until becoming totally transparent and return to step1.
  
@note 
  => If the "Media" directory is empty, a warning message is displayed on 
     the LCD : "  No Bitmap files...  "
    
  => If the file type stored in the "Media" directory is not supported,
     a warning message is displayed on the LCD : " file type not supported. "
    
  => If the SD card is removed while running the application, no warning message
     is displayed on the LCD. A reset is needed.
   
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32H7xx devices clock frequency is set to 400MHz

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

Graphics, Display, LTDC, SDMMC, LCD, Pictures, SD Card, BMP, RGB565, RGB888, ARGB8888

@par Directory contents

    - Display/LTDC_PicturesFromSDCard/Inc/main.h                             Main configuration file
    - Display/LTDC_PicturesFromSDCard/Inc/stm32h7xx_it.h                     Interrupt handlers header file
    - Display/LTDC_PicturesFromSDCard/Inc/stm32h7xx_hal_conf.h               HAL Configuration file 
    - Display/LTDC_PicturesFromSDCard/Inc/ffconf.h                           FAT file system module configuration file
    - Display/LTDC_PicturesFromSDCard/Inc/fatfs_storage.h                    Header for fatfs_storage.c
    - Display/LTDC_PicturesFromSDCard/Inc/sd_diskio.h                        Header for sd_diskio.c module
    - Display/LTDC_PicturesFromSDCard/Src/main.c                             Main program 
    - Display/LTDC_PicturesFromSDCard/Src/fatfs_storage.c                    Storage (FatFs) driver
    - Display/LTDC_PicturesFromSDCard/Src/sd_diskio.c                        SD Disk I/O driver 
    - Display/LTDC_PicturesFromSDCard/Src/stm32h7xx_it.c                     Interrupt handlers
    - Display/LTDC_PicturesFromSDCard/Src/system_stm32h7xx.c                 STM32H7xx system clock configuration file


@par Hardware and Software environment  

  - This application runs on STM32H743xx/STM32H753xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL  Set-up

@par How to use it ?

In order to make the program work, you must do the following :
 - The bitmap images, available under "/BMP_320x240" and "/BMP_480x272" folders 
   provided under "/Utilities/Media/Pictures" should be copied inside a 
   dedicated directory named "Media" at the micro SD card root.
   The images should have the following properties:
     file type    : *.bmp
     resolution   : up to 480x272 with AM480272H3TMQW LCD and up to 640x480 with 
                    AM-640480G5TNQW-T00H LCD
     pixel format : RGB565, RGB888 or ARGB8888     
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application 
                

 */
                                   
