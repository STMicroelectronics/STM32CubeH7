/**
  @page JPEG_EncodingFromFLASH_DMA : JPEG example : JPEG encoding with DMA of an image stored in FLASH example
  
  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the JPEG encoding from FLASH and with DMA example.
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

This example demonstrates how to read an RGB image stored in the internal FLASH, encode it 
using the JPEG HW encoder in DMA mode and save it in SDCard.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3XXQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Create the JPEG file with write access.
  - Call the function "JPEG_Encode_DMA" to start encoding the jpg file with DMA (Non blocking method).
    the RGB input image name is automatically selected according to the selected RGB format as follow :
    - Image_RGB888 for RGB888 (#define JPEG_RGB_FORMAT      JPEG_RGB888)
    - Image_RGB565 for RGB565 (#define JPEG_RGB_FORMAT      JPEG_RGB565)
    - Image_ARGB8888 for ARGB8888 (#define JPEG_RGB_FORMAT      JPEG_ARGB8888)
    Function "JPEG_Encode_DMA" is implemented in a separate module encode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  - Save encoded data in jpg file
  - Once the encoding ends, close the jpg file.
  - The LED1 is On if the test ends properly.
  - The LED2 is On in case of LTDC Init or µSD errors.

@note The jpeg HW encoder inputs are organized in YCbCr blocks.
      The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert the input RGB pixels
      to YCbCr blocks.
      The JPEG utilities files allowing to convert RGB pixels to YCbCr blocks are available under \Utilities\JPEG

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
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7 Series”

@par Keywords

Display, Graphics, JPEG, Encoding, Flash memory, DMA, ARGB8888, Hardware Encoding, LCD, YCbCr, RGB,
SD Card, SDRAM

@par Directory contents

  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stm32h7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stm32h7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/encode_dma.h                  Header for encode_dma.c module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/sd_diskio.h                   Header for sd_diskio.c module    
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/jpeg_utils_conf.h             JPEG utilities module configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/image_320_240_rgb.h           Specification of RGB image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stm32h7b3i_eval_conf.h     STM32H7B3I-EVAL board configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/ft5336_conf.h                 FT5336 capacitive touch panel configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/is42s32800j_conf.h            IS42S32800J SDRAM memory configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stmpe811_conf.h               STMPE811 IO Expander devices configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/stm32H7xx_it.c                Interrupt handlers
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/main.c                        Main program
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/encode_dma.c                  JPEG Encoding with DMA module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/sd_diskio.c                   SD Disk I/O driver module      
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/stm32h7xx_hal_msp.c           HAL MSP module 
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/system_stm32h7xx.c            STM32H7xx system source file  
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_argb_8888.c             Example of ARGB888 image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_rgb_565.c               Example of RGB565 image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_rgb_888.c               Example of RGB888 image to load in FLASH
  

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ device.
    
  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL  
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H7B3I-EVAL  Set-up
  - Connect a formatted uSD Card to the Micro SD connector (CN28).
  - Make sure JP12 and JP15 are on SDIO1 position.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
