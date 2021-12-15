/**
  @page LTDC_Display_2Layers LTDC Display 2 layers example
  
  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC_Display_2Layers/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Display 2 layers example.
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

  How to configure the LTDC peripheral to display two layers at the same time.
  The goal of this example is to explain how to blend two layers and how to display 
  an image with L8 (8bits per pixels indexed) as pixel format. 

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
  The CPU at 280 MHz.
  The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
  The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

  After LCD initialization, the LCD layer 1 and Layer 2 are configured as following :
  - Layer 1 is configured to display an image loaded from flash memory with direct
    color (RGB565) as pixel format and 320x240 (QVGA) size.
  - Layer 2 is configured to display an image loaded from flash memory with indirect
    color (L8) as pixel format and 320x240 (QVGA) size.
    To display an image with an indirect color as pixel format a color lookup table (CLUT) 
    is loaded, then every byte from image data (L8_320x240) is considered as 
    a position in color lookup table (CLUT).(see example below)  

  The blending is always active and the two layers can be blended following 
  the configured blending factors and the constant alpha.
  In this example the constant alpha for layer 2 is decreased to see the layer 1
  in the intersection zone.

  Example :
  ------------------------------------------------------------------------------
  |Image data   |  position   CLUT(RGB Value)  |   Alpha  | Output (ARGB value)|
  |-------------|------------------------------|----------|------------------- |
  |00 10 FF 25  |  0   -----> 0xFD10EA         | 0xFF     | 0xFFFD10EA         |
  |             |  .                           |          | 0xFF1548AD         |
  |             |  .                           |          | 0xFFAE6547         |
  |             |  .                           |          | 0xFFDA14EA         |
  |             |  .                           |          | .                  |
  |             |  16  -----> 0x1548AD         |          | .                  |
  |             |  .                           |          | .                  |
  |             |  .                           |          | .                  |
  |             |  37  -----> 0xDA14EA         |          | .                  |
  |             |  .                           |          |                    |
  |             |  .                           |          |                    |
  |             |  255 -----> 0xAE6547         |          |                    |
  ------------------------------------------------------------------------------       

  The images change unlike their positions on LCD in an infinite loop.

STM32H7B3I-DK board's LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
     If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Display, LTDC, Display Controller, TFT, LCD, Graphics, RGB888, 2 Layers, RGB, 

@par Directory contents

    - LTDC/LTDC_Display_2Layers/Inc/stm32h7xx_hal_conf.h  HAL configuration file
    - LTDC/LTDC_Display_2Layers/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - LTDC/LTDC_Display_2Layers/Inc/main.h                Main configuration file
    - LTDC/LTDC_Display_2Layers/Inc/stm32h7b3i_discovery_conf.h  STM32H7B3I-DK board configuration file
    - LTDC/LTDC_Display_2Layers/Inc/is42s16800j_conf.h    IS42S16800J SDRAM memory configuration file
    - LTDC/LTDC_Display_2Layers/Inc/ft5336_conf.h         FT5336 capacitive touch panel configuration file
    - LTDC/LTDC_Display_2Layers/Src/stm32h7xx_it.c        Interrupt handlers
    - LTDC/LTDC_Display_2Layers/Src/main.c                Main program 
    - LTDC/LTDC_Display_2Layers/Src/stm32h7xx_hal_msp.c   HAL MSP module	
    - LTDC/LTDC_Display_2Layers/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file
    - LTDC/LTDC_Display_2Layers/Inc/RGB565_320x240.h      Image layer 1 to be displayed on LCD : QVGA (320x240) in RGB565
    - LTDC/LTDC_Display_2Layers/Inc/L8_320x240.h          Image layer 2 to be displayed on LCD QVGA (320x240) with CLUT8 format.

@par Hardware and Software environment  

  - This example runs on STM32H7B3xxQ devices.
  
  - This example has been tested with STM32H7B3I-DK board and can be
    easily tailored to any other supported device and development board.    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
                           

 */
                                   
