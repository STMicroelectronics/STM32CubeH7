/**
  @page STemWin_fonts STemWin fonts Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    STemWin/STemWin_fonts/CM7/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STemWin fonts application. 
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

This directory contains a set of source files that implement a simple "fonts" 
application based on STemWin for STM32H7xx devices.

The application shows the capability of STemWin to display different languages 
with different fonts styles and sizes.

The used fonts are not part of the fonts shipped with STemWin. 
The fonts are generated with Font converter tool.

To display Unicode characters, we need to save the text with the needed language in UTF-8 format, 
convert UTF-8 text into C code and generate the adueqate font that to be used in the main application.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
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

Detailed steps:

Saving the text in UTF-8 format
-------------------------------
- Save the text with the needed language in "UTF-8" format. (you can use Notepad.exe)
- Using U2C software tool, converts "UTF-8" text into C code 
  (U2C software available under "Middlewares/ST/STemWin/Software") 
- copy the generated C code in the main application

pattern generation
------------------
To optimize as possible the generated fonts sizes we can generate only the used characters, 
for this, we have to save the text file (containing the text to be displayed) in "unicode" format 

Font generation
---------------
 - Opens font converter tool (available under "Middlewares/ST/STemWin/Software")
 - choose the adequate font (depending on the chosen language)
 - disable all characters ranges (Edit/Disable all characters)
 - load the generated pattern (text.txt saved as "unicode" format)
 - save the font
 ==> only used characters in text.txt file are generated.
 - include the font

 @note for arabic fonts the following ranges needs to be activated:
  - 600 -> 6FF
  - FE80 -> FEFC
  
 How to interact with the application
 ------------------------------------
 - Press on top right button to select the language and highlight the displayed text.
 - Press on bottom button to shows more information about the application.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note For the Cortex-M7, If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
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

Graphics, Display, STemWin, LTDC, Fonts, Characters, Style, Size

@par Directory contents 

  - STemWin/STemWin_fonts/CM7/Core/Inc/main.h                             Main program header file
  - STemWin/STemWin_fonts/CM7/Core/Inc/STM32H7xx_hal_conf.h               Library Configuration file
  - STemWin/STemWin_fonts/CM7/Core/Inc/STM32H7xx_it.h                     Interrupt handlers header file
  - STemWin/STemWin_fonts/CM7/Core/Src/main.c                             Main program file
  - STemWin/STemWin_fonts/CM7/Core/Src/STM32H7xx_it.c                     STM32H7xx Interrupt handlers
  - STemWin/STemWin_fonts/CM7/Core/system_STM32H7xx.c                     STM32H7xx system file
  - STemWin/STemWin_fonts/CM7/STemWin/App/font_app.c                      fonts application
  - STemWin/STemWin_fonts/CM7/STemWin/Target/GUIConf.c                    Display controller initialization
  - STemWin/STemWin_fonts/CM7/STemWin/Target/LCDConf_.c                   Configuration file for the GUI library (MB1075 LCD)  
  - STemWin/STemWin_fonts/CM7/STemWin/Target/GUIConf.h                    Header for GUIConf.c
  - STemWin/STemWin_fonts/CM7/STemWin/Target/LCDConf.h                    Header for LCDConf.c
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/arabic.c   	          arabic flag picture     
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/chinese.c  	          chinese flag picture
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/english.c  	          english flag picture
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/info.c     	          info logo picture
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/italian.c  	          italian flag picture
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/STLogo.c   	          STLogo logo picture
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/world.c    	          world flag picture
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/french.c   	          french flag picture
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/arabic25.c      arabic font with 25 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/arabic60.c      arabic font with 60 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/arabic100.c     arabic font with 100 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/chinese25B.c    chinese font with 25 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/chineseI60.c    chinese font with 60 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/chinese100R.c   chinese font with 100 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/eng25.c         new generated font with 25 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/eng60.c         new generated font with 60 pixels as width
  - STemWin/STemWin_fonts/CM7/STemWin/App/generated/fonts/eng100.c        new generated font with 100 pixels as width

  - STemWin/STemWin_fonts/CM4/Inc/stm32h7xx_hal_conf.h    				  HAL configuration file for Cortex-M4
  - STemWin/STemWin_fonts/CM4/Inc/stm32h7xx_it.h          				  Interrupt handlers header file for Cortex-M4
  - STemWin/STemWin_fonts/CM4/Inc/main.h                  				  Main configuration file for Cortex-M4
  - STemWin/STemWin_fonts/CM4/Src/stm32h7xx_it.c          				  Interrupt handlers for Cortex-M4
  - STemWin/STemWin_fonts/CM4/Src/main.c                  				  Main program  for Cortex-M4 
  
@par Hardware and Software environment  

  - This application runs on STM32H747xx devices.

  - This application has been tested with STMicroelectronics STM32H747I-DISCO 
    boards and can be easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
 - For CM4 target configuration (STM32H747I-Discovery_CM4) : 
     - Rebuild all files 
     - Load image into target memory
	 
 - For CM7 target configuration :
  - Rebuild all files and load your image into target memory
  - Run the application
 

 */
