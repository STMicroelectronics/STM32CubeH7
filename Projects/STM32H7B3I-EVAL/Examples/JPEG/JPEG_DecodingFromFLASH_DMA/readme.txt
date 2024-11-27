/**
  @page JPEG_DecodingFromFLASH_DMA : JPEG example : JPEG decoding with DMA of an image stored in FLASH example
  
  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingFromFLASH_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the JPEG decoding from FLASH with DMA example.
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

This example demonstrates how to decode a JPEG image stored in the internal FLASH 
using the JPEG HW decoder in DMA mode and display the final ARGB8888 image on LCD 
mounted on board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz 
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.
  
  
The example performs the following actions :
  - Init the JPEG decoder.
  - Init and configure the LCD.
  - Call the function "JPEG_Decode_DMA" to start decoding the jpg buffer with DMA (Non blocking method).
    Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  
  - Once the decoding ends :
     Call HAL routine "HAL_JPEG_GetInfo" to retrieve the image parameters : 
     image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.
     These parameters are used to initialize the DMA2D that will perform the copy of the decoded image 
     to the display frame buffer all with the YCbCr to RGB conversion (necessary for the display on the RGB LCD).

  - The jpeg HW decoder outputs are organized in YCbCr blocks. The DMA2D is used to convert YCbCr Blocks to 
        ARGB8888 pixels into the LCD Frame buffer for display.
        The DMA2D is configured in order to copy the result image in the center of the LCD screen.

  - The Red LED3 is On in case of error.		

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
 
@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
              In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
              Even though the user must manage the cache coherence for read accesses.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

Display, Graphics, JPEG, Decoding, Flash memory, DMA, MDMA, DMA2D, ARGB8888, Hardware decoding, LCD, YCbCr, RGB,

@par Directory contents

  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/decode_dma.h                  Header for decode_dma.c module   
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/image_320_240_jpg.h           JPEG Image 320x240 to be decoded and displayed on LCD
  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32h7xx_it.c                Interrupt handlers
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/main.c                        Main program
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/decode_dma.c                  JPEG Decoding (from memory) with DMA module
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32h7xx_hal_msp.c           HAL MSP module   
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/system_stm32h7xx.c            STM32H7xx system source file  
  

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ device.
    
  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL  
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
