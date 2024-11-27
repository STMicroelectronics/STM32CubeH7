/**
  @page MDMA_RepeatBlock_ZoomOut MDMA RepeatBlock ZoomOut example descreption
  
  @verbatim
  ******************************************************************************
  * @file    MDMA/MDMA_RepeatBlock_ZoomOut/readme.txt
  * @author  MCD Application Team
  * @brief   MDMA RepeatBlock ZoomOut example.
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

  This example provides a description of how to use the MDMA in repeat block trigger mode 
  in order to decimate an RGB565 image and copy it to the LCD frame buffer.
  
  The example demonstrates the capabilities of the MDMA to increment/decrement 
  the source/destination address independently from the source/destination data size 
  and the capability to increment/decrement the source/destination address after each block.
  
  Using these capabilities it is possible to decimate an RGB565 image by a factor of 2 or 4
  on the destination image. 
  
  In this example the hal function HAL_MDMA_Start_IT is used to start a repeat block transfer
  where each MDMA block corresponds to an image line.
  The BlockDataLength corresponds to the number of bytes in a line (i.e number of pixel x bytesPerPixel)
  The BlockCount corresponds to the total number of line in the image.   
  
  The principle is that each MDMA block represents an image line. and each data in the block
  represents an RGB565 pixel (half word). 
  then by setting the source data size to half word and the source data increment to word (2 half-words)
  it is possible to skip one pixel over two in each line.
  by setting the source block offset to a line width it is possible to skip one line over 2.
  The results gives a decimated image by 2 horizontally and vertically.
  
  The same approach can be used to decimate an RGB565 image by 4. In this case
  the source data increment is set to double word (4 half-words) allowing to skip 3 pixels 
  over 4 at the source. the source block offset is set to 3 lines width allowing to skip 3 lines
  over 4 from the source image. 
  The results gives a decimated image by 4 horizontally and vertically.
  

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
 
  The LCD is initialized and configured for display in ARGB8888 format (32 bpp). 
  The LCD Frame buffer is located on the external SDRAM.
  The MDMA destination buffer(drawing buffer) is located on the external SDRAM too. The MDMA destination buffer is in RGB565 format (16 bpp).
  Each time the MDMA transfer ends (decimating a new image in the MDMA destination buffer) the DMA2D is used
  to copy the MDMA destination buffer to the LCD display frame buffer all with RGB565 to ARGB8888 color conversion.
  
  First the MDMA is initialized and started to clear the MDMA destination buffer with a default color (Gray)
  the MDMA is configured with SW trigger and trigger mode set to repeat block.   
  The Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp.
  The Source Increment is disabled : the source address always corresponds to the address of the uint16 variable 
  that holds the default color value.    
  The destination Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp. 
  The source and destination Block address offset are set to zero null, the source blocks are contiguous.
  
  Then, each time the MDMA is initialized to decimate the source image by a factor of 2 or 4. 
  The decimation is possible thanks to the capability of the MDMA to increment the source and destination
  address independently from the source/destination data size.  
  The Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp.
  In case of a decimation by 2, the source increment is set to WORD (2 half-words) allowing to skip one pixel over two.
  In case of a decimation by 4, the source increment is set to DOUBLE WORD (4 half-words) allowing to skip 3 pixels.
  knowing that an RGB565 pixel is defined on one half word (16bits), this allows to decimate horizontally the original image by 2 or 4.
  - The source block address offset is set Image_width * bytes_per_pixels = Image_width * 4. This allows
    to jump one line over 2 from the source image allowing to decimate vertically the original image by 2.
    
  In case of a decimation by 2 , the source block address offset is set to :
              Image_width * bytes_per_pixels = Image_width * 2. 
This allows  to jump one line over 2 from the source image allowing to decimate vertically the original image by 2. 
  
  In case of a decimation by 4 , the source block address offset is set to :
             3 * Image_width * bytes_per_pixels = 3 * Image_width * 2. 
 This allows  to jump Three lines from the source image allowing to decimate vertically the original image by 4. 
    
  The destination block address offset is set to the "LCD_Offset" calculated in order to place the destination
  image at the center of the LCD screen.

  
STM32H747I Eval board's LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process or during the MDMA transfer.
 - LED4 is ON when a process iteration is complete

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
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Display, Graphics, MDMA, LCD, SDRAM, RGB565, Memory to memory, LTDC, Pixel, non-contiguous,
Repeat blocks

@par Directory contents

  - MDMA/MDMA_RepeatBlock_ZoomOut/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  
  - MDMA/MDMA_RepeatBlock_ZoomOut/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M7
  - MDMA/MDMA_RepeatBlock_ZoomOut/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M7
  - MDMA/MDMA_RepeatBlock_ZoomOut/Inc/main.h                Main configuration file for Cortex-M7
  - MDMA/MDMA_RepeatBlock_ZoomOut/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M7
  - MDMA/MDMA_RepeatBlock_ZoomOut/Src/main.c                Main program for Cortex-M7
  - MDMA/MDMA_RepeatBlock_ZoomOut/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M7 
  - MDMA/MDMA_RepeatBlock_ZoomOut/Inc/RGB565_480x272.h      Image of size 480x272 and format RGB565 to be decimated and displayed in ARGB8888 on LCD  

  - MDMA/MDMA_RepeatBlock_ZoomOut/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - MDMA/MDMA_RepeatBlock_ZoomOut/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - MDMA/MDMA_RepeatBlock_ZoomOut/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - MDMA/MDMA_RepeatBlock_ZoomOut/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - MDMA/MDMA_RepeatBlock_ZoomOut/CM4/Src/main.c                  Main program  for Cortex-M4
  - MDMA/MDMA_RepeatBlock_ZoomOut/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M4  
  
@par Hardware and Software environment  

  - This example runs on STM32H747xx devices.
  
  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_EVAL_CM7 and STM32H747I_EVAL_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example
                           

 */
 
