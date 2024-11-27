/**
  @page LTDC_Display_1Layer LTDC Display Layer 1 example
  
  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC_Display_1Layer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Display 1 layer example.
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

  This example provides a description of how to configure LTDC peripheral to 
  display an RGB image of size 480x272 and format RGB565 (16 bits/pixel) on LCD using only one layer.
  In this basic example the goal is to explain the different fields of the LTDC 
  structure. 

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
  The CPU at 400MHz 
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.
 
  After LCD initialization, the LCD layer 1 is configured to display image 
  (modelled by an array) loaded directly from flash memory by LTDC integrated DMA.
  

  LCD_TFT synchronous timings configuration :
  -------------------------------------------
 
                                             Total Width
                          <--------------------------------------------------->
                    Hsync width HBP             Active Width                HFP
                          <---><--><--------------------------------------><-->
                      ____    ____|_______________________________________|____ 
                          |___|   |                                       |    |
                                  |                                       |    |
                      __|         |                                       |    |
         /|\    /|\  |            |                                       |    |
          | VSYNC|   |            |                                       |    |
          |Width\|/  |__          |                                       |    |
          |     /|\     |         |                                       |    |
          |  VBP |      |         |                                       |    |
          |     \|/_____|_________|_______________________________________|    |
          |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
 Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
 Height    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |Height |      |         |/ / / / / / Active Display Area / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
          |     \|/_____|_________|_______________________________________|    |
          |     /|\     |                                                      |
          |  VFP |      |                                                      |
         \|/    \|/_____|______________________________________________________|
         
         
  Each LCD device has its specific synchronous timings values.
  This example use :
  The TFT LCD 5.7 inch Ampire 640x480 (MB1063 B-01) : Default LCD mount on the EVAL board
   In this case the synchronous timings are as following :
      Horizontal Synchronization (Hsync) = 30
      Horizontal Back Porch (HBP)        = 114
      Active Width                       = 640
      Horizontal Front Porch (HFP)       = 16

      Vertical Synchronization (Vsync)   = 3
      Vertical Back Porch (VBP)          = 32
      Active Height                       = 480
      Vertical Front Porch (VFP)         = 10
	  
  It can also use the TFT LCD 4.3 inch Ampire 480x272 (MB1046 B-01)
    In this case the synchronous timings are as following :
      Horizontal Synchronization (Hsync) = 41
      Horizontal Back Porch (HBP)        = 2
      Active Width                       = 480
      Horizontal Front Porch (HFP)       = 2

      Vertical Synchronization (Vsync)   = 10
      Vertical Back Porch (VBP)          = 2
      Active Height                       = 272
      Vertical Front Porch (VFP)         = 2
  
  LCD_TFT windowing configuration :
  ---------------------------------

  To configure the active display window, this example configures the 
  horizontal/vertical start and stop. 

  HorizontalStart = (Offset_X + Hsync + HBP);
  HorizontalStop  = (Offset_X + Hsync + HBP + Window_Width - 1); 
  VarticalStart   = (Offset_Y + Vsync + VBP);
  VerticalStop    = (Offset_Y + Vsync + VBP + Window_Height - 1);
  
  Window_width and Window_heigh should be in line with the image size to be 
  displayed.

STM32H743I Eval board's LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, LTDC, Display Controller, TFT, LCD, Graphics, RGB888, 1 Layer, RGB, 


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

@par Directory contents

    - LTDC/LTDC_Display_1Layers/Inc/stm32h7xx_hal_conf.h  HAL configuration file
    - LTDC/LTDC_Display_1Layers/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - LTDC/LTDC_Display_1Layers/Inc/main.h                Main configuration file
    - LTDC/LTDC_Display_1Layers/Src/stm32h7xx_it.c        Interrupt handlers
    - LTDC/LTDC_Display_1Layers/Src/main.c                Main program 
    - LTDC/LTDC_Display_1Layers/Src/stm32h7xx_hal_msp.c   HAL MSP module	
    - LTDC/LTDC_Display_1Layers/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file
    - LTDC/LTDC_Display_1Layers/Inc/RGB565_480x272.h      Image of size 480x272 and format RGB565 to be displayed on LCD

@par Hardware and Software environment  

  - This example runs on STM32H743xx devices.
  
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
                           

 */
                                   
