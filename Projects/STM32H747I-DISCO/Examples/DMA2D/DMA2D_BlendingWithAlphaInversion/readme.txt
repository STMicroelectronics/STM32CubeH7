/**
  @page DMA2D_BlendingWithAlphaInversion DMA2D blending with alpha inversion example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_BlendingWithAlphaInversion/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the STM32H7xx DMA2D blending with alpha inversion example.
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
  Memory_to_Memory with blending transfer and alpha inversion mode.

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
  
 In this transfer mode two input sources are fetched : foreground and background.
  In this example, the foreground and background are configured as following :
   - Foreground object 
   - input memory address at FLASH memory (static image arrays coded in FLASH).
   - Format : RGB565
   - Size   : 240x130
   - Background object 
   - input memory address at FLASH memory (static image arrays coded in FLASH).
   - Format : RGB565
   - Size   : 240x130
   - The constant alpha for foreground is decreased to see the background.
   - The alpha mode for foreground and background is configured to see two 
     superposed images in the resulting image out of the ChromArt DMA2D operation.
   - The Foreground alpha inversion is set sequentially to regular then inverted to demonstrate
     this new DMA2D feature capability.
        
  The DMA2D blends the two sources pixels buffers stored into the FLASH to compute 
  the resulting pixel in the LCD display frame buffer. 
  The DMA2D destination address and output offset are calculated in such a way 
  to place the output blended image to the center of the LCD.
  The transferred data to the output memory address is the result of the blending operation 
  between background and foreground (with Foreground alpha inversion set to regular or inverted).
 
  The example acts as follow :
  In an infinite while loop 
  - Use the DMA2D to blend the two sources images to the LCD display frame buffer.  
  - If the foreground alpha inversion is disabled (set to DMA2D_REGULAR_ALPHA) :    
     “Display Blended Image: Foreground Alpha Inversion OFF”  
  - If the foreground alpha inversion is enabled (set to DMA2D_INVERTED_ALPHA) :    
     “Display Blended Image: Foreground Alpha Inversion ON”  
  - Change the alpha inversion setting, wait for 2 sec and go to the next loop.
  
  User can see the result blending image with foreground alpha inversion enabled and disabled.

@note The example shows how to use the DMA2D blending with alpha inversion set on the foreground layer.
      Nevertheless the same parameter can be applied on the background DMA2D input layer 
      or/and at the output level depending of the user application requirements.

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

Display, Graphics, DMA2D, LCD, SRAM, ARGB4444, Blending, Alpha Inversion, Memory to memory, Foreground,
Background, RGB565, LTDC, DSI, Pixel


@par Directory contents

    - DMA2D/DMA2D_BlendingWithAlphaInversion/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Inc/main.h                Main configuration file for Cortex-M7
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M7
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M7
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Inc/RGB565_240x130_1.h    Image used for DMAD2D foreground fetching
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Inc/RGB565_240x130_2.h    Image used for DMAD2D background fetching
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Src/main.c                Main program  for Cortex-M7
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M7
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM7/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M7

    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM4/Inc/main.h                Main configuration file for Cortex-M4
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM4/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M4
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM4/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M4
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM4/Src/main.c                Main program  for Cortex-M4
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM4/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M4
    - DMA2D/DMA2D_BlendingWithAlphaInversion/CM4/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M4
	
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
                                   
