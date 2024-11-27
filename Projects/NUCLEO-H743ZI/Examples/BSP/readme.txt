/**
  @page BSP Example on how to use the BSP drivers
  
  @verbatim
  ******************************************************************************
  * @file    Examples/BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description STM32H7xx_Nucleo_144 BSP example.
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

This example provides a description of how to use the different BSP drivers.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The BSP examples detects the presence of Adafruit 1.8" TFT shield with joystick and uSD.

If the Adafruit shield is NOT connected, then:
 - Blue led (led2) blinks waiting the user button is pushed.
 - Then blue led swithces off and green & red leds start blinking at high frequency.
 - Pushing user button again and again blinking frequency decreases until looping 
   to high frequency.

If the Adafruit shield is connected, then this example shows how to use the different 
functionalities of LCD, SD card and joystick by switching between tests using user button.
At the beginning of each example the blue led (led2) blinks waiting the user button to start.
  - Firstly, use the joystick button to move a pointer inside a rectangle 
    (up/down/right/left) and change the pointer color(select).
  - Secondly, this example shows how to use the different LCD features to display 
    string with different fonts, to display different shapes and to draw a bitmap.
  - Thirdly, this example shows how to erase, write and read the SD card.
At the end of the three examples when pushing the user button the application loops 
to the beginning (first examples). 


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


@note It is highly recommended to set the SPI parameter "MasterKeepIOState" to "SPI_MASTER_KEEP_IO_STATE_ENABLE"
      at init time. This configuration allows to avoid glitches on SPI lines especially the clock.
      Glitches that could lead to Master/Slave desynchronization. This config allows SPI Master to keep
      control on output state even if it is disabled.

@par Keywords

Board Support Package, BSP, Adafruit, TFT, LCD, Joystick, SD Card

@par Directory contents

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32h7xx.c         STM32H7xx system clock configuration file
  - BSP/Src/stm32h7xx_it.c             Interrupt handlers 
  - BSP/Src/joystick.c                 Joystick feature
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/sd.c                       SD features
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32h7xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32h7xx_it.h             Interrupt handlers header file
  - BSP/Inc/stlogo.h                   Image used for BSP example


@par Hardware and Software environment

 - This example runs on STM32H743ZI Devices 
  
 - This example has been tested with STMicroelectronics NUCLEO-H743ZI  board
   and can be easily tailored to any other supported device and development board.

 - NUCLEO-H743ZI  board Set-up
   - Connect the Adafruit 1.8" TFT shield (https://www.adafruit.com/products/802)
   - Make sure the SB146 is closed and the SB165 and SB174 are opened on the NUCLEO-H743ZI board
   
@par How to use it ? 

In order to make the program work, you must do the following:
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example
 
      

 */
