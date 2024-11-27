/**
  @page DMA2D_MemToMemWithBlending DMA2D Memory to Memory with blending example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the STM32H7xx DMA2D Memory to Memory with blending example.
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
  Memory_to_Memory with blending transfer mode.

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
 
  The LCD is initialized using the BSP functions. the LCD layer 0 is configured for display
  with color format set to ARGB8888.
  
  Two images in RGB565 color format are used respectively for foreground and background.
   - Image 1 : 
		- input memory address at Flash memory (static image array coded in FLASH).
		- Format : RGB565
		- Size   : 240x130
   - Image 2 : 
		- input memory address at Flash memory (static image array coded in FLASH).
		- Format : RGB565
		- Size   : 240x130

    Then in an infinite while loop the following sequence is implemented :
	
      - Step 1- The DMA2D is used in Pixel Format Conversion mode (PFC) to transfer
          the "Image 1" from the input buffer located in the Flash to the
          LCD frame buffer. The DMA2D destination address and output offset are set 
          in such a way to transfer the input image to the center of the LCD.
          The DMA2D is configured in Pixel Format Conversion mode (PFC) with 
          input color format set to RGB565 (color format of the input image)
          and output color format set to ARGB8888 (color format of LCD display).
		  
      -Step 2- The DMA2D is used in Pixel Format Conversion mode (PFC) to transfer
          the "Image 2" from the input buffer located in the Flash to the
          LCD frame buffer. The DMA2D destination address and output offset are set 
          in such a way to copy the input image to the center of the LCD.
          The DMA2D is configured in Pixel Format Conversion mode (PFC) with 
          input color format set to RGB565 (color format of the input image)
          and output color format set to ARGB8888 (color format of LCD display).
		  
      -Step 3- The DMA2D is used in memory to memory with Blending mode (M2M_BLEND),
	      to blend the foreground and background images. the DMA2D is configured as follow :
	- The DMA2D foreground layer is configured with input pixel format set to RGB565
          the source address set to the "Image 1" input buffer located in the Flash. 
          the input offset is set to zero and the alpha transparency set to 0x7F (semi-transparent).
        - The DMA2D background layer is configured with input pixel format set to ARGB8888,
          The source address set to the center of LCD frame buffer and the input offset
          is set in such a way to fetch the background from the center of LCD frame buffer.
          Knowing that the LCD frame buffer already contains the "Image 2" in ARGB8888 format 
          transferred in previous step.
          The alpha transparency set to 0xFF (fully opaque) for the background layer. 
        - The DMA2D output color format is set to ARGB8888 (color format of LCD display).
          The output offset and destination address are configured in in such a way to place the 
          result of the blending in the center of the LCD frame buffer :
          The output offset is configured same as the background layer input offset.
          The destination address is configured same as the background layer source address. 	

        For each step a message indicates which image is currently displayed 
          - Display Foreground Image : life.augmented
          - Display Background Image : ST Logo
          - Display Blended Image    : ST Logo + life.augmented
        
        a delay of 2sec is inserted after each step.
 
  Next step in the pixel pipeline is the use of the LTDC with only 1 layer which 
  is the output of the DMA2D operation.
  This in order to show on LCD panel the result of the blending of the 
  foreground and background.      
  
  In this example, the background object represents the ST logo and the foreground
  object represents the "life.augmented" slogan.

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

@par Keywords

Display, Graphics, DMA2D, LCD, SRAM, ARGB8888, Blending, Memory to memory, Foreground,
Background, RGB565, LTDC, DSI, Pixel

@par Directory contents

  - DMA2D/DMA2D_MemToMemWithBlending/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Inc/main.h                      Main configuration file for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Inc/stm32h7xx_it.h              Interrupt handlers header file for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Inc/stm32h7xx_hal_conf.h        HAL configuration file for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Inc/RGB565_240x130_1.h          Image used for DMAD2D validation 
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Inc/RGB565_240x130_2.h          Image used for DMAD2D validation
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Src/main.c                      Main program  for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Src/stm32h7xx_it.c              Interrupt handlers for Cortex-M7
  - DMA2D/DMA2D_MemToMemWithBlending/CM7/Src/stm32h7xx_hal_msp.c         HAL MSP module for Cortex-M7

  - DMA2D/DMA2D_MemToMemWithBlending/CM4/Inc/main.h                Main configuration file for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlending/CM4/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlending/CM4/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlending/CM4/Src/main.c                Main program  for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlending/CM4/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M4
  - DMA2D/DMA2D_MemToMemWithBlending/CM4/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M4  	

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
                                   
