/**
  @page DMA2D_RegToMemWithLCD DMA2D Register to Memory with LCD example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_RegToMemWithLCD/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D Register to Memory with LCD example.
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

  This example provides a description of how to configure DMA2D peripheral in 
  Register_to_Memory transfer mode and display the result on LCD.
    
  This mode allowsto fill a user-defined area by a fixed color.
    
  The register to Memory mode of the DMA2D is used to fill a buffer with a fixed color in format ARGB4444 (16 bpp).

  The fixed color value used (in ARGB8888 format) is 0x00FFFF00. It is used by the DMA2D
  to fill the LCD frame buffer.
  
  0x00FFFF00 yields an equal combination of Red and Green, that is yellow
  (0x00FF0000 would mean only Red, 0x0000FF00 only Green, 0x000000FF only Blue).   

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
 
  After DMA2D configuration, the data transfer is performed and then the LCD is 
  configured to display color zone created after this transfer on LCD.
       
STM32H743I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON when there is an error in LTDC transfer/Init process.

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

Display, Graphics, DMA2D, LCD, ARGB4444, Blending, Register to memory, LTDC, Pixel

@par Directory contents

    - DMA2D/DMA2D_RegToMemWithLCD/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_RegToMemWithLCD/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_RegToMemWithLCD/Inc/stm32h7xx_hal_conf.h  HAL configuration file 
    - DMA2D/DMA2D_RegToMemWithLCD/Src/main.c                Main program  
    - DMA2D/DMA2D_RegToMemWithLCD/Src/stm32h7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_RegToMemWithLCD/Src/stm32h7xx_hal_msp.c   HAL MSP module    
    - DMA2D/DMA2D_RegToMemWithLCD/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file


@par Hardware and Software environment  

  - This example runs on STM32H743xx devices.
  
  - This example has been tested with STM32H743I-EVAL boards and can be easily 
    tailored to any other supported device and development board. 
                
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
                                   
