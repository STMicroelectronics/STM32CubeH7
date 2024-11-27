/**
  @page LTDC_ColorKeying_FromOSPI LTDC Color Keying From OSPI example

  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying_FromOSPI/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LTDC Color Keying from OSPI example.
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

This example describe how to enable and use the LTDC color keying functionality and use the OSPI memory.

First , an image of size 480x270 in ARGB4444 format featuring ST logo is loaded into the OSPI using STM32CubeProgrammer.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735xx Devices :
The CPU at 520 MHz
The HCLK for D1 Domain AXI peripherals, D2 Domain AHB peripherals and D3 Domain AHB  peripherals at 260 MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 130 MHz.

The example includes also CPU_CACHE_Enable() to enable the CPU Cache. This function is provided to
to enhance the performance in case of use of AXI interface with several masters.

@note Some code parts can be executed in the ITCM-RAM (64 KB up to 256kB) which decreases critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

After LCD initialization, LCD layer 1 and color keying configuration, an image
 of size 480x270 in ARGB4444 format featuring ST slogan surrounded by a white frame ( color key ARGB4444==0xFFFFFF) is displayed on LCD.

 Color keying is enabled/disabled on the image when the tamper button is pressed.
 The effect is that at each button press the light blue color is made transparent or not thanks
 to color keying. At time when the color key is enabled, the background of the slogan is the background color
 configured for layer 1, which is all zeroes (black color).
 Note that user button press is scanned in polling in the main.c, no use of EXTI in this case.

 Color Keying :
 --------------
 If the Color Keying is enabled, all pixels of the image are compared to that color key pixel value
 (0xFFFFFF), practically only the white zone of the slogan correspond to that color key in the image.
 For the pixels that match the color key RGB value, all channels (ARGB) of those pixels
 are set to 0 (those pixels are then becoming transparent and does not appear on LCD screen, the background
 of image : black is seen instead).

STM32H735xx Discovery board's LED can be used to monitor the transfer status:
 - LED2 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


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

Display, LTDC, Display Controller, TFT, LCD, Graphics, ARGB4444, BMP, color keying,

@par Directory contents

    - LTDC/LTDC_ColorKeying_FromOSPI/Inc/s70kl1281_conf.h            specific configuration required for the s70kl1281 component : RAM OSPI
    - LTDC/LTDC_ColorKeying_FromOSPI/Inc/stm32h735g_discovery_conf.h STM32H735_DK board configuration file
    - LTDC/LTDC_ColorKeying_FromOSPI/Inc/stm32h7xx_hal_conf.h        HAL configuration file
    - LTDC/LTDC_ColorKeying_FromOSPI/Inc/stm32h7xx_it.h              Interrupt handlers header file
    - LTDC/LTDC_ColorKeying_FromOSPI/Inc/main.h                      Main configuration file
    - LTDC/LTDC_ColorKeying_FromOSPI/Inc/ARGB4444_480x270.bin        Image of size 480x270 and format ARGB4444 to be loaded to OSPI and to be displayed on LCD
    - LTDC/LTDC_ColorKeying_FromOSPI/Inc/mx25lm51245g_conf           specific configuration required for the mx25lm51245g component : FLASH OSPI
                                                                     
    - LTDC/LTDC_ColorKeying_FromOSPI/Src/stm32h7xx_it.c              Interrupt handlers
    - LTDC/LTDC_ColorKeying_FromOSPI/Src/main.c                      Main program
    - LTDC/LTDC_ColorKeying_FromOSPI/Src/stm32h7xx_hal_msp.c         HAL MSP module
    - LTDC/LTDC_ColorKeying_FromOSPI/Src/system_stm32h7xx.c          STM32H7xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32H735xx devices.

  - This example has been tested with STMicroelectronics STM32H735G-DK
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
- Step1: 
  - Open STM32CubeProgrammer application.
  - Select External Flash loader "MX25LM51245G_STM32H735G-DK".
  - choose the binary image "ARGB4444_480x270.bin"
  - define the Start Address: 0x90000000
  - Click on "Start Programming" button.
- Step2:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example


 */
