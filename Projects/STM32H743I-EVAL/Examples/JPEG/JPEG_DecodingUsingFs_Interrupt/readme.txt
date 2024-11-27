/**
  @page JPEG_DecodingUsingFs_Interrupt : JPEG example : JPEG decoding using Fatfs and with interrupt example
  
  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the JPEG decoding using Fatfs and with interrupt example.
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

This example demonstrates how to read jpeg file from SDCard memory using Fatfs, decode it 
using the JPEG HW decoder in interrupt mode and display the final ARGB8888 image 
on the LCD-TFT screen.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Open the jpg file "image.jpg" in read access.
  - Call the function "JPEG_Decode_IT" to start decoding the jpg file with Interrupt (Non blocking method).
    Function "JPEG_Decode_IT" is implemented in a separate module decode_interrupt.c.
    This module is a wrapper on top of the JPEG HAL driver
  
  - Once the decoding ends, 
    - call HAL routine "HAL_JPEG_GetInfo" to retrieve the image parameters : 
      image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.
      These parameters are used to initialize the DMA2D that will perform the copy of the decoded image 
      to the display frame buffer all with the YCbCr to RGB conversion (necessary for the display on the RGB LCD).
    - Close the jpg file.
    

   - The jpeg HW decoder outputs are organized in YCbCr blocks. The DMA2D is used to convert YCbCr Blocks to 
        ARGB8888 pixels into the the LCD Frame buffer for display.
        The DMA2D is configured in order to copy the result image in the center of the LCD screen.

   - The Red LED3 is On in case of error.		
        
@note To run this example , you can use an JPEG “.jpg” file from\Utilities\Media\Pictures\JPG
      or use your own .jpg image.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).

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

Display, Graphics, JPEG, Decoding, Flash memory, DMA2D, ARGB8888, Hardware decoding, LCD, YCbCr, RGB,
FatFS, File system, SD Card, SDRAM

@par Directory contents

  - JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/decode_interrupt.h            Header for decode_interrupt.c module
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/sd_diskio.h                   Header for sd_diskio.c module  
  
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Src/stm32h7xx_it.c                Interrupt handlers
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Src/main.c                        Main program
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Src/decode_interrupt.c            JPEG Decoding with interrupt module
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Src/sd_diskio.c                   SD Disk I/O driver module  
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Src/stm32h7xx_hal_msp.c           HAL MSP module 
  - JPEG/JPEG_DecodingUsingFs_Interrupt/Src/system_stm32h7xx.c            STM32H7xx system source file  
  

@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STMicroelectronics STM32H743I-EVAL  
    evaluation boards and can be easily tailored to any other supported device 
    and development board. 
    
  - STM32H743I-EVAL Set-up
  - Copy a jpg image to a uSD card (under the root)
      The image must have the following properties:
        - named as "image.jpg"
        - dimensions : less or equal to the display screen:
               - Width  : <= 640
               - Height : <= 480
  - Connect the uSD Card to the Micro SD connector (CN13).
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
 
