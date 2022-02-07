/**
  @page BSP example

  @verbatim
  ******************************************************************************
  * @file    BSP/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BSP example
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

This example provides a short description of how to use the BSP drivers.
The STM32H7xx_Nucleo offers the Leds' and COM services.
Tha Adafruit (reference 802) offers LCD, Joystick and SD services.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

The program checks the presence of Adafruit shield by reading the state of pin PB.01:
1- The shield is available
  The following demos can be accessed by pressing Key Button:
  - Joystik: Use Joystik buttons to move "x" inside a rectangle displayed on LCD
  - LCD: Several shapes and images are displayed on the LCD
  - SD:
      + SD block erase
      + SD write
      + SD read
      + Result's comparison
      + Status of these operations is displayed on LCD.

2- The shield is not available
  The COM port is initialized. Please, open an external hyperterminal to see message displayed.
  Press Key button to toggle LED1 and LED3. Their state is displayed on the hyperterminal.
  This is done in an infinite loop.
  Hyperterminal configuration:
    - Data Length = 8 Bits
    - One Stop Bit
    - None parity
    - BaudRate = 115200 baud
    - Flow control: None

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decreases critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
	  the ITCM-RAM.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7 Series

@par Keywords

Board Support Package, BSP, Adafruit, TFT, LCD, Joystick, SD Card

@par Directory contents

  - Src/main.c                       Main program
  - Src/stm32h7xx_it.c               Interrupt handlers
  - Src/stm32h7xx_nucleo_bus.c       Handles the Bus operations
  - Src/joystick.c                   Joystick test
  - Src/lcd.c                        LCD test
  - Src/sd.c                         SD test
  - Inc/main.h                       Main program header file
  - Inc/stlogo.h                     ST logo
  - Inc/stm32h7xx_it.h               Interrupt handlers header file
  - Inc/stm32h7xx_nucleo_bus.h       Header of the Bus drivers
  - Inc/adafruit_802_conf.h          Adafruit Configuration file
  - BSP/Inc/stm32h7xx_nucleo_conf.h  Nucleo Configuration file

@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.

  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
