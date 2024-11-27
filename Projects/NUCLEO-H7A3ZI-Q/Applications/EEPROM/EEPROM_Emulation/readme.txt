/**
  @page EEPROM_Emulation application to show an eeprom emulation

  @verbatim
  ******************************************************************************
  * @file    EEPROM_Emulation/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the EEPROM_Emulation application.
  ******************************************************************************
  *
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

This application describes the software solution for substituting standalone EEPROM
by emulating the EEPROM mechanism using the on-chip Flash of STM32H7A3xxQ devices.


Please refer to AN3969 of STM32F4xx devices for further details regarding this application to
understand the principles about EEPROM Emulation.

@note The AN3969 contains specifications of STM32F4xx devices Flash memory.For STM32H7A3xxQ devices
      Flash memory specification, please refer to the STM32H7xx Reference Manual .

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

The application requires two Flash memory sectors allocated to non-volatile data:
one that is initially erased, and offers byte-by-byte programmability; the other that is ready
to take over when the former sector needs to be garbage-collected. A header field that occupies
the first flash word (128-bits) of each sector indicates the sector status. Each of these sectors is
considered as a page.
The sectors used are as follows:
  -Flash Bank 2: Sector6, Sector7.

@note The firmware code is executed from Sector0 on Bank1 and the Read/Write
      operations on EEPROM are done on Bank2.

NUCLEO-H7A3ZI-Q 's LEDs can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED2 is Toggling when an operation of write and read is ongoing.
  - LED3 is ON in case of error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The EEPROM algorithm includes flash erase operations, this could lead to data cache incoherencies
      (as the erase operation is performed through a dedicated command).
      When the CPU Data Cache is enabled, user should ensure cache maintenance after erase operations.


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
      Please refer to the AN4839 "Level 1 cache on STM32F7 and STM32H7 Series"

@par Keywords

EEPROM, Emulation, Flash, Program, Erase, Sectors, OPCODE

@par Directory contents

 - EEPROM_Emulation/Inc/stm32h7xx_hal_conf.h          HAL Configuration file
 - EEPROM_Emulation/Inc/main.h                        Header for main.c module
 - EEPROM_Emulation/Inc/eeprom.h                      Header for eeprom.c module
 - EEPROM_Emulation/Inc/stm32h7xx_it.h                Header for stm32h7xx_it.c
 - EEPROM_Emulation/Inc/stm32h7xx_nucleo_conf.h       Nucleo Configuration file
 - EEPROM_Emulation/Src/main.c                        Main program
 - EEPROM_Emulation/Src/eeprom.c                      EEPROM program
 - EEPROM_Emulation/Src/stm32h7xx_it.c                Interrupt handlers
 - EEPROM_Emulation/Src/system_stm32h7xx.c            STM32H7A3xx system clock configuration file

@par Hardware and Software environment

  - This application runs on STM32H7A3xxQ devices.

  - This application has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.

Table 1. EEPROM application implementation on NUCLEO-H7A3ZI-Q
+------------+--------------------------------------------------------------+-----------------------+
|  Platform  |                  Implementation                              |  Configuration        |
+------------+--------------------------------------------------------------+-----------------------+
|            |                                                              |                       |
|  Firmware  | The EEPROM program is located in Bank1 at 0x08000000.        |                       |
|            | The Flash routines (program/erase) are executed from the     |                       |
|            | Flash memory.                                                |                       |
|            | The size of this program is about 15 Kbytes programmed on:   | Bank 1: Sector0       |
|            |                                                              |                       |
|            | -------------------------------------------------------------|-----------------------|
|            |                                                              |                       |
|            | The flash zone used for the EEPROM emulation is located at:  | Bank 2:               |
|            |   -Bank 2 flash at 0x0810C000                                | 8 KB Sector6          |
|            |                                                              | 8 KB Sector7          |
|            |                                                              |                       |
+------------+--------------------------------------------------------------+-----------------------+

@note Before using the application, ensure that dedicated sectors for EEPROM Bank 2: Sector6 and Sector7
      are erased to avoid a match with a valid page OPCODE.

Following picture illustrates the situation in program memory:

                Dual bank flash
  Flash End  /----------------------\  0x081FFFFF Flash End
             |Sector126 -> Sector127|
             |  Flash used for      |
             |  EEPROM emulation    |
             |  mechanism           |
             |----------------------|  0x081FC000 Bank2 Sector126 (8KB)
             |                      |
             |                      |
             |                      |
             |                      |
             |----------------------|  0x08100000 Bank2 Sector0   (8KB)
             |----------------------|  0x080FE000 Bank1 Sector127 (8KB)
             |                      |
             |                      |
             |                      |
             |                      |
             |----------------------|  0x08004000 Bank1 Sector2   (8KB)
             | flash used for       |
             | implement the EEPROM |
             | emulation mechanism  |
             | (Firmware size:15KB) |
             \----------------------/  0x08000000 Bank1 Sector0   (8KB)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */
