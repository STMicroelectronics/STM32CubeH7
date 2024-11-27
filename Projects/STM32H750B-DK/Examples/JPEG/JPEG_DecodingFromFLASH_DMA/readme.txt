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
using the JPEG HW decoder in DMA mode and display the final ARGB8888 image on the LCD.


  In this example, the code is executed from QSPI external memory while data are in internal
  SRAM memory.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

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

  - The Red LED2 is On in case of error.

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"		

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).

@par Keywords

Display, Graphics, JPEG, Decoding, Flash memory, LTDC, DMA, MDMA, DMA2D, ARGB8888, Hardware decoding, LCD, YCbCr, RGB
      


@par Directory contents

  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/system_stm32h7xx.c     STM32H7xx system configuration source file
  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/main.h                        Header for main.c module 
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/decode_dma.h                  Header for decode_dma.c module   
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/image_320_240_jpg.h           JPEG Image 320x240 to be decoded and displayed on LCD  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32h7xx_it.c                Interrupt handlers
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/main.c                        Main program
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/decode_dma.c                  JPEG Decoding (from memory) with DMA module
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32h7xx_hal_msp.c           HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32H750xx devices.
  
  - This example has been tested with STM32H750B-DISCO boards and can be easily 
    tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this example(see below).
  4. Start debugging the example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load this example to the external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program :
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using STM32CubeIDE  :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" in case of XiP from QSPI
            - From Erasing & Programming menu, browse and open the output binary file relative to your example
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


 */
