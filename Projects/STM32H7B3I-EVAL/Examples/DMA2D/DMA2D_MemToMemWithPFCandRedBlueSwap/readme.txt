/**
  @page DMA2D_MemToMemWithPFCandRedBlueSwap DMA2D M2M with PFC and Red/Blue swap example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32H7xx DMA2D Memory to Memory with 
  *          Pixel Format Conversion and Red/Blue swap example.
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

How to configure the DMA2D peripheral in Memory-to-memory transfer mode with
pixel format conversion and red and blue swap, and display the result on the
LCD. 

The source buffer in format RGB565 and size 320x240 located in Flash memory
is copied by the DMA2D peripheral in Memory-to-memory transfer mode with PFC to
the SDRAM LCD frame buffer for display. The DMA2D output offset and destination
address are calculated so as to locate the image in the center of the LCD
screen.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32H7B3XXQ Devices :
  The CPU at 280 MHz.
  The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
  The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.
  In this basic example the goal is to explain the utility of the DMA2D field "RedBlueSwap" 
  in the case of Memory-to-memory with PFC transfer mode.
  User can easily check the difference on the displayed image between "RedBlueSwap" set to DMA2D_RB_REGULAR and DMA2D_RB_SWAP:
  Red and Blue pixels components are mutually swapped.
    
  The example acts as follow :
  - first the LCD screen are initialized
  Then, in an infinite while loop
  - The DMA2D is configured to perform a transfer from input RGB565 image to the output ARGB8888 display frame buffer.
  - The parameter "RedBlueSwap" is changed for each iteration (from DMA2D_RB_REGULAR to DMA2D_RB_SWAP and vice versa).
  - then start the DMA2D and wait for the end of the transfer.
  - wait for 2 sec and go to the next loop.
  
  when the parameter "RedBlueSwap" is set to DMA2D_RB_REGULAR : display the message "Red and Blue Swap : OFF"
  when the parameter "RedBlueSwap" is set to DMA2D_RB_SWAP : display the message "Red and Blue Swap : ON "
  
 => Output offset is the number of pixel to be skipped after each transferred line
                      
 how to calculate the size of the transferred data ? 
 => selected color format gives the number of bits per pixel and we have 
    (RGB565 => 16bits/pixel) the number of pixel per line and the number of line, 
    therefore :
    
    data_size = (bits per pixel) X (pixel per line + output offset) X (number of line) 


STM32H7B3I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is On when there are errors in LTDC Init process
 - LED3 is On when there are errors in DMA2D transfer

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


 @note  The header files of the images used in this example can be generated with 
        STemWin bitmap converter released with this package.
        \Middlewares\ST\STemWin\Software\BmpCvtST.exe      


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the Â MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
  If the application needs to use DMA(or other masters) based access or requires more RAM, then Â the user has to:
                          - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
                          - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
                          - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
                            Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
                            Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Display, Graphics, DMA2D, LCD, SRAM, RGB565, Blending, Memory to memory, Pixel Format Conversion,
Red Blue swap, LTDC, Pixel

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/stm32h7xx_hal_conf.h  HAL configuration file 
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/RGB565_320x240.h      RGB565 input image to be transferred by DMA2D
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I-EVAL board configuration file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/ft5336_conf.h         FT5336 capacitive touch panel configuration file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/is42s32800j_conf.h    IS42S32800J SDRAM memory configuration file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/stmpe811_conf.h       STMPE811 IO Expander devices configuration file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/stm32h7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/stm32h7xx_hal_msp.c   HAL MSP module
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file

@par Hardware and Software environment  

  - This example runs on STM32H7B3xxQ devices.
  
  - This example has been tested with STM32H7B3I-EVAL boards and can be easily 
    tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 
                          

 */
                                   
