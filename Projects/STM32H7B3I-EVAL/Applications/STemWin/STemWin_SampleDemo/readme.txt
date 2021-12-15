/**
  @page STemWin_SampleDemo Readme file

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    STemWin/STemWin_SampleDemo/readme.txt
  * @author  MCD Application Team
  * @brief
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

This directory contains a set of source files that implement a sample
demonstration application allowing to show some of the STemWin Library capabilities
on STM32H7B3xxQ Devices.

The list of modules to be used is configured into the file Demo/GUIDEMO.h

All the demo modules are running sequentially. Throughout the demonstration, a
control frame window is displayed at the bottom right of the touch screen: it
includes a progress bar and two control buttons (Next + Halt) allowing to skip
or pause the currently running module.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the application is not running normally as mentionned above , you can accordingly modify either
      the Heap and Stack of the application or the GUI_NUMBYTES define in the GUIConf.c file

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

Graphics, Display, STemWin, LTDC, LCD, GUI, Touch screen

@par Directory contents

  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf.h                  Header for GUIConf.c
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf.h                  Header for LCDConf.c
  - STemWin/STemWin_HelloWorld/Core/Inc/main.h                           Main program header file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32h7xx_hal_conf.h             Library Configuration file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32h7xx_it.h                   Interrupt handlers header file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32h7b3i_eval_conf.h           STM32H7B3I_EVAL board configuration file
  - STemWin/STemWin_HelloWorld/Core/Inc/is42s32800j_conf.h               Configurations required for the IS42S32800J SDRAM memory.
  - STemWin/SampleDemo/Core/Src/rtc.c                                    Main file to configure RTC clock
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf_stm32h7b3i_eval.c  Display controller initialization
  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf_stm32h7b3i_eval.c  Configuration file for the GUI library
  - STemWin/STemWin_HelloWorld/Core/Src/main.c                           Main program file
  - STemWin/STemWin_HelloWorld/Core/Src/stm32h7xx_it.c                   STM32H7xx Interrupt handlers
  - STemWin/STemWin_HelloWorld/Core/Src/system_stm32h7xx.c               STM32H7xx system file
  - STemWin/SampleDemo/STemWin/App/GUIDEMO_*.c                           All the sources files provided for SEGGER DEMO
  - STemWin/SampleDemo/STemWin/App/GUIDEMO.h                             DEMO modules configuration file

@par Hardware and Software environment

  - This application runs on STM32H7B3xxQ Devices.

  - This example has been tested with STM32H7B3I-EVAL boards and can be easily
    tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the application


 */
