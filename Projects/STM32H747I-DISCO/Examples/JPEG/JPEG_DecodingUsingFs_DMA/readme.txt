/**
  @page JPEG_DecodingUsingFs_DMA : JPEG example : JPEG decoding using Fatfs and with DMA example
  
  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the JPEG decoding using Fatfs and with DMA example.
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

This example demonstrates how to read jpeg file from SDCard memory using Fatfs, decode it 
using the JPEG HW decoder in DMA mode and display the final ARGB8888 image on KoD DSI-LCD 
mounted on board or a HDMI monitor Connected through the DSI-HDMI bridge board MB1232.A.

The example can automatically detect if the KoD DSI-LCD is connected or if the DSI-HDMI 
bridge is connected to the DSI connector (CN15), if no DSI slave is connected the RED LED will turn On.

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

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Open the jpg file "image.jpg" in read access.
  - Call the function "JPEG_Decode_DMA" to start decoding the jpg file with DMA (Non blocking method).
    Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c.
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

@note In case of DISCO's LCD use in example code, the DATA_IN_ExtSDRAM define
      need to be added  in the project preprocessor to avoid SDRAM multiple configuration
     (the LCD uses SDRAM as frame buffer, and its configuration is done by the BSP_SDRAM_Init()). 

@par Keywords

Display, Graphics, JPEG, Decoding, Flash memory, DSI, LTDC, DMA, DMA2D, MDMA, ARGB8888, Hardware decoding, LCD, YCbCr, RGB, HDMI
FatFS, File system, SD Card, SDRAM

@par Directory contents

  - JPEG/JPEG_DecodingUsingFs_DMA/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file

  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/stm32h7xx_hal_conf.h          HAL configuration file for cortex-M7
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/stm32h7xx_it.h                Interrupt handlers header file for cortex-M7
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/main.h                        Header for main.c module for cortex-M7  
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/decode_dma.h                  Header for decode_dma.c module
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Inc/sd_diskio.h                   Header for sd_diskio.c module
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Src/stm32h7xx_it.c                Interrupt handlers for cortex-M7
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Src/main.c                        Main program for cortex-M7
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Src/decode_dma.c                  JPEG Decoding with DMA module
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Src/sd_diskio.c                   SD Disk I/O driver module
  - JPEG/JPEG_DecodingUsingFs_DMA/CM7/Src/stm32h7xx_hal_msp.c           HAL MSP module for cortex-M7 

  - JPEG/JPEG_DecodingUsingFs_DMA/CM4/Inc/stm32h7xx_hal_conf.h          HAL configuration file for cortex-M4
  - JPEG/JPEG_DecodingUsingFs_DMA/CM4/Inc/stm32h7xx_it.h                Interrupt handlers header file for cortex-M4
  - JPEG/JPEG_DecodingUsingFs_DMA/CM4/Inc/main.h                        Header for main.c module for cortex-M4
  - JPEG/JPEG_DecodingUsingFs_DMA/CM4/Src/stm32h7xx_it.c                Interrupt handlers for cortex-M4
  - JPEG/JPEG_DecodingUsingFs_DMA/CM4/Src/main.c                        Main program for cortex-M4
  - JPEG/JPEG_DecodingUsingFs_DMA/CM4/Src/stm32h7xx_hal_msp.c           HAL MSP module for cortex-M4   
  

@par Hardware and Software environment

  - This example runs on STM32H747xx/STM32H757xx device.
    
  - This example has been tested with STMicroelectronics STM32H747I-DISCO  
    discovery boards and can be easily tailored to any other supported device 
    and development board. 
    
  - STM32H747I_DISCO Set-up
  - Copy a jpg image to a uSD card (under the root)
      The image must have the following properties:
        - named as "image.jpg"
        - resolution : less or equal to the screen resolution (i.e 800x480 if using DSI-LCD or 720x576 for DSI-HDMI)
           - Width  : if using DSI-LCD <= 800, if using DSI-HDMI <= 720
           - Height : if using DSI-LCD <= 480, if using DSI-HDMI <= 576
  - Connect the uSD Card to the Micro SD connector (CN6).
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
