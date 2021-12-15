/**
  @page LTDC_Paint LTDC Paint application
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    Display/LTDC_Paint/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Paint application.
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

@par Application Description

 This application describes how to configure LCD touch screen and attribute 
 an action related to configured touch zone and how to save BMP picture in USB Disk.
 
 In this application, the code is executed from QSPI external memory while data are in internal
 SRAM memory.
 
 At the beginning of the main program the HAL_Init() function is called to reset 
 all the peripherals, initialize the Flash interface and the systick.
 The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
 The CPU at 400MHz 
 The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.  

 After LCD and touch screen initialization, the menu is displayed on the screen. 
 
 The menu contains palette of colors, clear icon, save icon, different kinds 
 of draw sizes, the current selected size, selected color and working rectangle. 

 Select one of proposed colors and select the size of brush to draw 
 inside the working rectangle.
 
 Clear the working window by pressing the clear icon or/and save the picture 
 in USB Mass Stroage by pressing the save icon.

                                          LCD Menu
               ____________________________________________________________
          /   |______|---------------------------------------------------- |
          |   |______||                                                   ||
          |   |______||                                                   ||
          |   |______||                                                   ||
          |   |______||                                                   ||
          |   |______||                                                   ||
palette   |   |______||               Working Rectangle                   ||
   of   __|   |______||                                                   ||
 Colors   |   |______||                                                   ||
          |   |______||                                                   ||
          |   |______||                                                   ||
          |   |______||                                                   ||
          |   |______||___________________________________________________|| 
          \   |______|__________________ _________        __               | 
              |clear |  choose the brush|   Save  |      |__|         O    | 
              |______|________size______|_USBDisk_|__selected color__size__|


  - LED1 is on to indicate the end of saving operation and a message is displayed 
    on LCD to indicate the beginning and the end of the saving operation.

Notes: 
  -   USB and LTDC share same pll3 as clock source: USB uses PLL3Q as devider to get 48 Mhz 
      and LTDC used PLL3R as devider to get needed frequency.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Graphics, Display, LTDC, USB, Pictures, BMP, Background, Foreground, Layers, Touchscreen, Calibration

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

    - Display/LTDC_Paint/Inc/main.h                 Main configuration file
    - Display/LTDC_Paint/Inc/stm32h7xx_it.h         Interrupt handlers header file
    - Display/LTDC_Paint/Inc/stm32h7xx_hal_conf.h   HAL Configuration file 
    - Display/LTDC_Paint/Inc/ffconf.h               FAT file system module configuration file
    - Display/LTDC_Paint/Inc/usbh_conf.h            Configuration file for USB module
    - Display/LTDC_Paint/Inc/usbh_diskio.h.h        Header for usbh_diskio.c module
    - Display/LTDC_Paint/Inc/color.h                Image used to display colored pens in case of 480x272 LCD screen
    - Display/LTDC_Paint/Inc/save.h                 Image used to display save icon
    - Display/LTDC_Paint/Src/main.c                 Main program 
    - Display/LTDC_Paint/Src/stm32h7xx_it.c         Interrupt handlers
    - Display/LTDC_Paint/Src/usbh_conf.c            Main function to configure USB
    - Display/LTDC_Paint/Src/usbh_diskio.c          USB Host Disk I/O driver
    - Display/LTDC_Paint/Src/system_stm32H7xx.c     STM32H7xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32H750xx device.

  - This application has been tested with STM32H750B-DISCO board and can be
    easily tailored to any other supported device and development board.

  - STM32H750B-DISCO Set-up
  - Plug the USB key into the STM32H750B-DISCO board through 'USB micro A-Male 
    to A-Female' cable (FS mode: connector CN13).

@par How to use it ? :

In order to make the program work, you must do the following :

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc template.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with "Display/LTDC_Paint" application (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load the LTDC_Paint application to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
   - Open the STM32CubeProgrammer tool
   - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" in case of XiP from QSPI
   - From Erasing & Programming menu, browse and open the output binary file relative to this application
   - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


 */
                                   
