/**
  @page MDMA_RepeatBlock_Rotation MDMA Repeat BLock Transfer Rotation example description
  
  @verbatim
  ******************************************************************************
  * @file    MDMA/MDMA_RepeatBlock_Rotation/readme.txt 
  * @author  MCD Application Team  
  * @brief   Description of the MDMA Repeat BLock Rotation Transfer example.
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

  This example provides a description of how to use the MDMA in repeat block trigger mode 
  in order to copy an RGB565 image to the LCD frame buffer.
  
  The example demonstrates also the capabilities of the MDMA to increment/decrement 
  the source and destination address independently from the data size and the capability 
  to increment/decrement the source/destination address after each block.
  By using these capabilities it is possible to apply mirror and/or 180° rotation effect
  on the destination image.
  
  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
 
  The LCD is initialized and configured for display in RGB565 format. 
  The LCD Frame buffer is located on the external SDRAM.
  
  Each time the MDMA is initialized with different configuration and transfer is started:
  The MDMA transfer is started using the HAL API HAL_MDMA_Start_IT. The BlockDataLength is set to the size of a line
  in the LCD frame buffer (in bytes). The BlockCount is set to the number of lines.

   - config 0:
    The MDMA is initialized and started to clear the LCD frame buffer with a default color (Gray).
    The MDMA is configured with SW trigger and trigger mode set to repeat block.
   
    The Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp.
    The Source Increment is disabled : the source address always corresponds to the address of the uint16 variable 
    that holds the default color value.
    
    The destination Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp. 
    The source and destination Block address offset are set to zero null, the source blocks are contiguous.

  - config 1 :
    The MDMA is initialized and started to copy the source image (located in the internal Flash)
    to the LCD frame buffer at the center position. 
    Each block corresponds to a line of the source/destination image.
    The number of blocks corresponds to the number of lines of the image.      
    
    The MDMA is configured with SW trigger and trigger mode set to repeat block.
    
    The Source and destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp.
    The source and destination Increment are also set to half word.
    
    The Source Block address offset is set to zero, the source blocks are contiguous
    
    The Destination Block address offset is set in order to place the image at the center of the LCD frame buffer
    The destination blocks are not contiguous. 
    The LCD_Offset is equal to LCD X size - image width.
   
  - config 2 :
    The MDMA is initialized and started to copy the source image to the LCD frame buffer at the center 
    position with mirror effect.
    Each block corresponds to a line of the source/destination image.
    The number of blocks corresponds to the number of lines of the image.
    
    The Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp 
    
    The Source Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
    Knowing that the source address is set to the source image start address.
    
    The Destination Decrement by half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
    Knowing that the destination address is set to the last pixel address in the first line 
    of the destination image.
    
    The Source Block address offset is zero, the source blocks are contiguous.
    
    The Destination Block address offset is set in order to place the image in the center of the LCD frame buffer.
    The destination blocks are not contiguous. 
    The LCD_Offset is = to LCD X size - image width.
    Knowing that the destination address is set to the last pixel address in the first line 
    of the destination image, the destination block offset takes also into account that 
    the destination address is decrementing.

 - config 3 :
    The MDMA is initialized and started to copy the source image to the LCD frame buffer 
    at the center position with 180° rotation effect. 
    Each block corresponds to a line of the source/destination image.
    The number of blocks corresponds to the number of lines of the image.
    
    The Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp 
    
    The Source Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
    Knowing that the source address is set to the source image start address.
    
    The Destination Decrement by half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
    Knowing that the destination address is set to the last pixel of the destination image.
    
    The Source Block address offset is zero, the source blocks are contiguous.

    The Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
    The destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width.
    Knowing that the destination address is set to  the last pixel address of 
    the destination image, the destination block offset takes also into account that the destination address is decrementing.

 - config 4 :
    The MDMA is initialized and started to copy the source image to the LCD frame buffer 
    at the center position with 180° rotation and mirror effect.
    Each block corresponds to a line of the source/destination image.

    The number of blocks corresponds to the number of lines of the image 

    The Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp 
    
    The Source Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
    Knowing that the source address is set to the source image start address.
    
    The Source Block address offset is zero, the source blocks are contiguous.
    
    Source and Destination Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp 
    knowing that the destination address is set to the first pixel address of the last line 
    in the destination image.
 
    The Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
    The destination blocks are not contiguous. 
    The LCD_Offset is = to LCD X size - image width.
    Knowing that the destination address is set to the first pixel address of the last line 
    in the destination image, the destination block offset allows to decrement the destination address 
    after each block in order to start the next block at the previous line.
    
STM32H743I Eval board's LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process or during the MDMA transfer.
 - LED1 is ON when a process iteration is complete

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
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

System, Display, Graphics, MDMA, LCD, SDRAM, RGB565, Memory to memory, LTDC, Pixel, non contiguous,
Repeat blocks

@par Directory contents

    - MDMA/MDMA_Repeat_BLock/Inc/stm32h7xx_hal_conf.h  HAL configuration file
    - MDMA/MDMA_Repeat_BLock/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - MDMA/MDMA_Repeat_BLock/Inc/main.h                Main configuration file
    - MDMA/MDMA_Repeat_BLock/Src/stm32h7xx_it.c        Interrupt handlers
    - MDMA/MDMA_Repeat_BLock/Src/main.c                Main program
    - MDMA/MDMA_Repeat_BLock/Src/stm32h7xx_hal_msp.c   HAL MSP module  	
    - MDMA/MDMA_Repeat_BLock/Src/system_stm32h7xx.c    STM32H7xx system source file
    - MDMA/MDMA_Repeat_BLock/Inc/RGB565_480x272.h      Image of size 480x272 and format RGB565 to be displayed on LCD

@par Hardware and Software environment  

  - This example runs on STM32H743xx devices.
  
  - This example has been tested with STM32H743I-EVAL board  and can be
    easily tailored to any other supported device and development board.    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
                           

 */
                                   
