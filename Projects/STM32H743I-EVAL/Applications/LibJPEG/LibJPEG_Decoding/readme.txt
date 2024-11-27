/**
  @page LibJPEG_Decoding LibJPEG decoding application
  
  @verbatim
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LibJPEG decoding application.
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

This application demonstrates how to use the libjpeg API to  decode a jpeg file.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

Then a jpeg file is read from the SD Card memory, decoded and displayed on the LCD.

The image transferring to the LCD_FRAME_BUFFER can be done with two ways; 
with CPU or with DMA2D. In main.h file, uncomment "USE_DMA2D" define for DMA2D   
transfer. Otherwise, transfer is made by CPU.

On success the green led , that's LED1, is toggling.
in case of error it is the red led that, LED3,is blinking.


@note The JPEG images,with 320x240 size,  should be copied at the micro SD card root
@note customize the "IMAGE_FILE" define in the main.h to point to the image to decode.

@note  the DMA2D can't access the DTCM memory region, starting at 0x20000000, thus when the flag
USE_DMA2D is used, the application should be linked at SRAM1 (0x24000000)

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

Display, Graphics, LibJPEG, SDMMC, LTDC, SD Card, LCD, Decoding, Flash memory, DMA2D, ARGB8888, RGB, BMP, JPG

@par Directory contents

    - LibJPEG/LibJPEG_Decoding/Inc/main.h                      Main program header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32h7xx_it.h              Interrupt handlers header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32h7xx_hal_conf.h        HAL Configuration file 
    - LibJPEG/LibJPEG_Decoding/Inc/ffconf.h                    FAT file system module configuration file
    - LibJPEG/LibJPEG_Decoding/Inc/decode.h                    decoder header file
    - LibJPEG/LibJPEG_Decoding/Inc/jmorecfg.h                  Lib JPEG configuration file (advanced configuration).
    - LibJPEG/LibJPEG_Decoding/Inc/jconfig.h                   Lib JPEG configuration file.
    - LibJPEG/LibJPEG_Decoding/Inc/jdata_conf.h                Write/Read methods definition
    - LibJPEG/LibJPEG_Decoding/Inc/sd_diskio.h                 the FatFs sd diskio header file
    - LibJPEG/LibJPEG_Decoding/Src/main.c                      Main program  
    - LibJPEG/LibJPEG_Decoding/Src/stm32h7xx_it.c              Interrupt handlers
    - LibJPEG/LibJPEG_Decoding/Src/system_stm32h7xx.c          stm32h7xx system clock configuration file
    - LibJPEG/LibJPEG_Decoding/Src/decode.c                    decoder file
    - LibJPEG/LibJPEG_Encoding/Src/jdata_conf.c                Write/Read methods definition
    - LibJPEG/LibJPEG_Decoding/Inc/sd_diskio.c                 the FatFs sd diskio implementation	

@par Hardware and Software environment

  - This application runs on stm32h743xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL setup
    - Connect a uSD Card to the MSD connector (CN13).


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
                                   
