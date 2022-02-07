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

How to configure the DMA2D peripheral in Memory-to-memory blending transfer
mode and with indexed 256 color images (L8). The examples also show how to use
the DMA2D foreground/background CLUT with L8 color mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735XX Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

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

STM32H735G-DK board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there are Ltdc Init process or dma2d transfer errors.

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
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Display, Graphics, DMA2D, LCD, SDRAM, ARGB8888, Blending, Memory to memory, Foreground, L8, CLUT
Background, LTDC, Pixel, Indexed colors

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/stm32h7xx_hal_conf.h  HAL configuration file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/L8_Image1.h           L8_Image1 definition
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/L8_Image0.h           L8_Image0 definition
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/stm32h735g_discovery_conf.h  STM32H735G-DK board configuration file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/ft5336_conf.h         FT5336 capacitive touch panel configuration file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/s70kl1281_conf.h      S70KL1281DABHI023 OctoSPI RAM memory configuration file
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Inc/mx25lm51245g_conf.h   MX25LM51245G OSPI FLASH memory configuration file

    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/main.c                Main program
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/stm32h7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/stm32h7xx_hal_msp.c   HAL MSP module
    - DMA2D/DMA2D_MemToMemWithBlendingAndCLUT/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32H735xx devices.

  - This example has been tested with STM32H735G-DK boards and can be easily
    tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5)


 */

