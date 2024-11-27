/**
  @page DMA2D_MemToMemWithBlendingAndCLUT DMA2D Memory to Memory with blending and CLUT example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the STM32H7xx DMA2D Memory to Memory with blending and CLUT example.
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

  This example provides a description of how to configure DMA2D peripheral in 
  Memory_to_Memory with blending transfer mode and indexed 256 color images (L8).
  The examples shows also how to use the DMA2D foreground/background CLUT with L8 color mode.   

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

  Two L8 images are used :
  - L8_Image1 : L8 256 color Indexed 320x240 image
  - L8_Image0 : L8 256 color Indexed 320x240 image

  In first stage the DMA2D is used to configure the layer 1 in memory to memory with Pixel Format Conversion.
  - Configure the output color mode as ARGB8888 pixel format.
  - Configure the Foreground (layer 1)
  - Foreground image is loaded from FLASH memory (L8_Image1)
  - CLUT (256 color index table ) loaded from FLASH memory (L8_CLUT_1)
  - Input color mode as L8 pixel format
  - Load the L8_CLUT_1 CLUT (color index table for image L8_Image1) to DMA2D Foreground CLUT and wait for CLUT loading ends.
  - Start the DMA2D transfer where source address is L8_Image1 and destination address is the LTDC display frame buffer.
  - Once the DMA2D transfer ends, the image L8_Image1 is displayed on the LCD. 
  - Wait for 2 sec before next stage.

  In second stage the DMA2D is used to configure the layer 1 in memory to memory with Pixel Format Conversion.
  - Configure the output color mode as ARGB8888 pixel format.
  - Configure the Foreground (layer 1)
  - Foreground image is loaded from FLASH memory (L8_Image0)
  - CLUT (256 color index table ) loaded from FLASH memory (L8_CLUT_0)
  - Input color mode as L8 pixel format
  - Load the L8_CLUT_0 CLUT (color index table for image L8_Image0) to DMA2D Foreground CLUT and wait for CLUT loading ends.
  - Start the DMA2D transfer where source address is L8_Image0 and destination address is the LTDC display frame buffer.
  - Once the DMA2D transfer ends, the image L8_Image0 is displayed on the LCD.
  - Wait for 2 sec before next stage
  
  In third stage the DMA2D is used to configure the layer 1 and layer 0 in memory to memory with blending.
  - Configure the output color mode as ARGB8888 pixel format.
  - Configure the Foreground (layer 1)
  - Foreground image is loaded from FLASH memory (L8_Image1)
  - CLUT for layer 1 (256 color index table ) loaded from FLASH memory (L8_CLUT_1)
  - Input color mode as L8 pixel format.
  - Transparency factor for layer 1 is set to 0x7F (semi-transparent)
  - Load the L8_CLUT_1 CLUT (color index table for image L8_Image1) to DMA2D Foreground CLUT and wait for CLUT loading ends.
  - Configure the Background (layer 0)
  - Background image is loaded from FLASH memory (L8_Image0)
  - CLUT for layer 0 (256 color index table ) loaded from FLASH memory (L8_CLUT_0)
  - Input color mode as L8 pixel format
  - Load the L8_CLUT_0 CLUT (color index table for image L8_Image0) to DMA2D Background CLUT and wait for CLUT loading ends.
  - Start the DMA2D transfer with blending where :
      - source address 1 : for foreground is L8_Image1
      - source address 2 : for background is L8_Image0      
      - destination address is the LTDC display frame buffer.
  - Once the DMA2D transfer ends,  the result blend image from L8_Image0 and L8_Image1 is displayed on the LCD.
  - wait for 2 sec then loop to stage 1.
  
  
  The DMA2D blends the two sources pixels from FLASH to internal FIFOs to compute 
  the resulting pixel in dedicated output buffer (using foregound and background CLUTs to convert L8 source images to ARGB8888 blend image). 
  The transferred data to the output memory address is the result of the blending 
  operation between background and foreground.
  
  Files L8_Image1.h and L8_Image0.h provides the definition of the L8 images where:
  - L8_CLUT_x is the 256 colors CLUT for L8_Imagex : color palette (Color index tables).
  - L8_Imagex is the 256 colors image itself. each pixel is coded on one byte.

STM32H747I-DISCO board's LEDs can be used to monitor the transfer status:
 - LED4 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON when there is an error in LTDC transfer/Init process.

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


 @note  The header files of the images used in this example can be generated with 
        STemWin bitmap converter released with this package.
        \Middlewares\ST\STemWin\Software\BmpCvtST.exe      

@par Keywords

Display, Graphics, DMA2D, LCD, SDRAM, ARGB8888, Blending, Memory to memory, Foreground, L8, CLUT
Background, LTDC, DSI, Pixel, Indexed colors


@par Directory contents

  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Inc/main.h                Main configuration file for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Inc/L8_Image1.h           L8_Image1 definition
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Inc/L8_Image0.h           L8_Image0 definition
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Src/main.c                Main program  for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM7/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M7

  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM4/Inc/main.h                Main configuration file for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM4/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM4/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM4/Src/main.c                Main program  for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM4/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/CM4/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M4  	

@par Hardware and Software environment  

  - This example runs on STM32H747xx/STM32H757xx devices.
  
  - This example has been tested with STM32H747I-DISCO boards and can be easily 
    tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example
                          

 */
                                   
