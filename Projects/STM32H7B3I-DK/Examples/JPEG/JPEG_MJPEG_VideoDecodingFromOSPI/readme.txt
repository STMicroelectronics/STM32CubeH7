/**
  @page JPEG_MJPEG_VideoDecodingFromOSPI add here description
  
  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecodingFromOSPI/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the MJPEG video decoding from OSPI Flash example.
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

@par Example Description

How to use the HW JPEG decoder to decode an MJPEG video file located in the
external OSPI flash and display it on the LCD-TFT screen.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for To be defined Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.

The example performs the following actions :
  - Initialize the OSPI interface and configure it to memory mapped mode.
    
  - Function "AVI_ParserInit" is called to initialize the AVI parser and read the avi file header.
    then In the video decoding loop and for each iteration 
      - function "AVI_GetFrame" is called to retrieve the address and size of one frame from the AVI file. 
      - If the frame corresponds to a video frame then start the decoding using the function "JPEG_Decode_DMA" (Non blocking method).
        Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c. 
        This module is a wrapper on top of the JPEG HAL driver.
     
  - Once the decoding ends, 
    - if the decoded frame is the first one, call HAL routine "HAL_JPEG_GetInfo" 
      to retrieve the image parameters : image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.
      These parameters are used to initialize the DMA2D that will perform the copy of the decoded frame 
      to the display frame buffer all with the YCbCr to RGB conversion (necessary for the display on the RGB LCD).
        
  - The jpeg HW decoder outputs are organized in YCbCr blocks. The DMA2D is used to convert YCbCr Blocks to 
        ARGB8888 pixels into the LCD Frame buffer for display.
        The DMA2D is configured in order to copy the result image in the center of the LCD screen.
     
  - When the end of file is reached, the file decoding characteristics are displayed:
       Resolution, JPEG quality, Chroma sampling, average decoding time and number of decoded frames.
       
  - Video rate is by default set to native frame rate of original video (to disable regulation , you can comment "USE_FRAMERATE_REGULATION" flag in main.h)

  - The Red LED3 is On in case of error.
   
@note To Run this example , you can use an MJPEG “.avi” file from 
      \Utilities\Media\Video or use your own MJPEG file.
     This file must have the following properties:
        - renamed as "video.avi"
        - resolution : less or equal to the LCD-DSI screen (i.e 480x272 for the STM32H7B3I-DK board)
        - Width  : <=480
        - Height : <= 272
      The MJPEG file can be in “AVI” container format. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@note The compiler optimizations level could be set to High/Balanced giving a good trade-off between the code size and the performance (decode time).


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

Display, Graphics, JPEG, MJPEG, Video, Decoding, LCD, JPEG Hardware Decoder,
DMA, FatFS, File system, avi, YCbCr, RGB

@par Directory contents

  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/main.h                        Header for main.c module  
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/decode_dma.h                  Header for decode_dma.c module   
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/AVI_parser.h                  Header for AVI_parser.c module   
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/stm32h7b3i_discovery_conf.h     STM32H7B3I-DK board configuration file
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/mx25lm51245g_conf.h           MX25LM51245G OSPI memory configuration file
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/is42s16800j_conf.h            IS42S16800J SDRAM memory configuration file
  - JPEG/MJPEG_VideoDecodingFromOSPI/Inc/ft5336_conf.h                 FT5336 capacitive touch panel configuration file
  - JPEG/MJPEG_VideoDecodingFromOSPI/Src/stm32h7xx_it.c          Interrupt handlers
  - JPEG/MJPEG_VideoDecodingFromOSPI/Src/main.c                        Main program
  - JPEG/MJPEG_VideoDecodingFromOSPI/Src/decode_dma.c                  JPEG Decoding with MDMA module
  - JPEG/MJPEG_VideoDecodingFromOSPI/Src/AVI_parser.c                  AVI parser module 
  - JPEG/MJPEG_VideoDecodingFromOSPI/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - JPEG/MJPEG_VideoDecodingFromOSPI/Src/system_stm32h7xx.c      STM32H7xx system source file  
  

@par Hardware and Software environment

  - This example runs on To be defined device.
    
  - This example has been tested with STMicroelectronics STM32H7B3I-DISCO
    evaluation boards and can be easily tailored to any other supported device 
    and development board. 
    
  - STM32H7B3I-DISCO Set-up
  - Flash one of the MJPEG video files to the OSPI external Flash using STM32CubeProgrammer utility
      The file must have the following properties:
        - resolution : less or equal to the LCD screen (i.e 480x272 for the STM32H7B3I-DK board)
        - Width  : <=480
        - Height : <= 272
      The MJPEG file can be in “AVI” container format.
  - Use STM32CubeProgrammer utility version v2.1.0 or higher.
  - Click on "External Loader" from the bar menu then check "MX25LM51245G_STM32H7B3I-DISCO" box
  - Rename the video file : change the extension to .bin : example video1.avi.bin
  - From STM32CubeProgrammer utility go to "Memory & file edition > Open File" and Open the renamed video file.
  - From STM32CubeProgrammer utility go to "Erasing & Programming" and set the "Start address" to 0x90000000. Press "Start Programming" button and wait to the end of the programming operation.
  - Once the video file programmed , close the STM32CubeProgrammer utility and run the example.
  
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
 
