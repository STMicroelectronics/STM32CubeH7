/**
  @page JPEG_MJPEG_VideoDecoding : MJPEG video decoding example
  
  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecoding/readme.txt 
  * @author  MCD Application Team  
  * @brief   Description of the MJPEG video decoding example.
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

@par Example Description

This example demonstrates how to use the HW JPEG decoder to decode an MJPEG video file
and display it on the LCD-TFT screen.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Open the mjpeg file "video.avi" in read access.
    
  - Function "AVI_ParserInit" is called to initialize the AVI parser, then In the video decoding loop and for each iteration 
    function "AVI_GetFrame" is called to read one frame from the AVI file. If the frame corresponds to a video frame 
    then start the decoding using the function "JPEG_Decode_DMA" (Non blocking method).
    Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver.
     
  - Once the decoding ends, 
    - if the decoded frame is the first one, call HAL routine "HAL_JPEG_GetInfo" 
      to retrieve the image parameters : image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.
      These parameters are used to initialize the DMA2D that will perform the copy of the decoded frame 
      to the display frame buffer all with the YCbCr to RGB conversion (necessary for the display on the RGB LCD).
        
   - The jpeg HW decoder outputs are organized in YCbCr blocks. The DMA2D is used to convert YCbCr Blocks to 
        ARGB8888 pixels into the the LCD Frame buffer for display.
        The DMA2D is configured in order to copy the result image in the center of the LCD screen.
     
     - When the end of file is reached, close the jpg file and display the file decoding characteristics :
       Resolution, JPEG quality, Chroma sampling, average decoding time and number of decoded frames.
   - Video rate is by default set to native framerate of original video (to disable regulation , you can comment "USE_FRAMERATE_REGULATION" flag in main.h)       

   - The Red LED3 is On in case of error.

@note To Run this example , you can use an MJPEG “.avi” file from 
      \Utilities\Media\Video or use your own MJPEG file.
     This file must have the following properties:
        - renamed as "video.avi"
        - resolution : less or equal to the LCD-TFT screen (i.e 640x480 for the STM32H743I-EVAL board)
        - Width  : <= 640
        - Height : <= 480
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
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Display, Graphics, JPEG, MJPEG, Video, Decoding, LCD, JPEG Hardware Decoder,
DMA, FatFS, File system, avi, YCbCr, RGB

@par Directory contents

  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/decode_dma.h                  Header for decode_dma.c module
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/sd_diskio.h                   Header for sd_diskio.c module  
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/AVI_parser.h                  Header for AVI_parser.c module   
  
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/stm32h7xx_it.c                Interrupt handlers
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/main.c                        Main program
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/decode_dma.c                  JPEG Decoding with MDMA module
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/sd_diskio.c                   SD Disk I/O driver module  
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/AVI_parser.c                  AVI parser module 
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/stm32h7xx_hal_msp.c           HAL MSP module 
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/system_stm32h7xx.c            STM32H7xx system source file  
  

@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STMicroelectronics STM32H743I-EVAL  
    evaluation boards and can be easily tailored to any other supported device 
    and development board. 
    
  - STM32H743I-EVAL Set-up
  - Copy an MJPEG video file to the uSD card (under the root)
      The file must have the following properties:
        - named as "video.avi"
        - resolution : less or equal to the LCD-DSI screen (i.e 640x480 for the STM32H743I-EVAL board)
        - Width  : <= 640
        - Height : <= 480
      The MJPEG file can be in “AVI” container format. 
  - Connect the uSD Card to the Micro SD connector (CN13).
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
