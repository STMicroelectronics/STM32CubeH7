/**
  @page IAP_Main AN4657 STM32H7xx In-Application Programming using the USART Readme file
  
  @verbatim
  ******************************************************************************
  * @file    IAP_Main/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of implementation of the AN4657 (in-application programming
  *          using the USART (IAP)) on STM32H7xx devices.
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

@par Application Description

 This directory contains a set of sources files and pre-configured projects that 
 describes how to build an In-Application Programming (IAP) that uses an USART
 interface to load an application binary.

 At the beginning of the main program the HAL_Init() function is called to reset 
 all the peripherals, initialize the Flash interface and the systick.
 The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
 The CPU at 400MHz 
 The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.  

 Application check if Tamper button is pressed, than it open an USART session through
 USART1 (CN2), present a menu for user to choose next operation. Else will check if
 an application is already loaded at the APPLICATION_ADDRESS and jump to it.

 Table 1. IAP implementation on STM32H743I-EVAL
 /*** Platform ***|************* Implementation **************************|***** Configuration *****\
 ****************************************************************************************************
 |    Firmware    | The IAP program is located at 0x08000000. The Flash   |                         |
 |                | routines (program/erase) are executed from the Flash  |                         |
 |                | memory.                                               |                         |
 |                | The size of this program is about 16 Kbytes and       |                         |
 |                | programmed on:                                        | Sector 0                |  
 |                | ------------------------------------------------------|-------------------------|
 |                |                                                       | Bank1                   | 
 |                | The user application (image to be downloaded with the | (Sector 1 - Sector 7)   |
 |                | IAP) will be programmed starting from address         | 896 Kbytes              | 
 |                | (uint32_t)0x08020000(1).                              | Bank2                   | 
 |                | The maximum size of the image to be loaded is:        | (Sector 0 - Sector 7)   | 
 |                |                                                       | 1024 Kbytes             | 
 |                | ------------------------------------------------------|-------------------------|
 |                | The image is uploaded with the IAP from the STM32H7xx | 1920 Kbytes             | 
 |                | internal Flash.                                       | 12 KBytes for this case |
 |                | The size of the image to be uploaded up to:           |                         |
 |----------------|-------------------------------------------------------|-------------------------|
 |    Hardware    | Push-button (active level: high)                      | Tamper push-button      |                                                                     
 |                |                                                       | connected to pin PC13   |
 |                | ------------------------------------------------------|-------------------------| 
 |                | USART used                                            |  USART1   (CN2)         |
 \**************************************************************************************************/
 (1) User application location address is defined in the flash_if.h file as: 
 #define APPLICATION_ADDRESS           ((uint32_t)0x08020000)
 To modify it, change the default value to the desired one. Note that the application must be linked
 relatively to the new address too.
 
 Following picture illustrates the situation in program memory:
 Figure 2. Flash memory usage

 Top Flash Memory address /-------------------------------------------\  0x081FFFFF
                          |                                           |
                          |                                           |
                          |                                           |
                          |          Sector 0 - Sector 7 (Bank 2)     |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          Sector 1 - Sector 7 (Bank 1)     |
                          |                                           |
                          |                                           |
                          |          User code                        |
                          |                                           |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          |-------------------------------------------|  0x08020000
                          |          IAP code                         |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          \-------------------------------------------/	 0x08000000					  

   
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

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

In-Application Programming, IAP, Flash, Bank, USART, Binary, Address, Vector table, Application, Button

@par Directory contents

 - IAP/IAP_Main/Inc/main.h                Main configuration file
 - IAP/IAP_Main/Inc/common.h              Common function header file
 - IAP/IAP_Main/Inc/flash_if.h            Flash Interface header file
 - IAP/IAP_Main/Inc/menu.h                Menu header file
 - IAP/IAP_Main/Inc/ymodem.h              Ymodem communication header file
 - IAP/IAP_Main/Inc/stm32h7xx_hal_conf.h  HAL Configuration file 
 - IAP/IAP_Main/Inc/stm32h7xx_it.h        Interrupt handlers header file
 - IAP/IAP_Main/Src/main.c                Main program
 - IAP/IAP_Main/Src/stm32h7xx_it.c        Interrupt handlers
 - IAP/IAP_Main/Src/flash_if.c            Flash Interface source file
 - IAP/IAP_Main/Src/menu.c                Menu source file
 - IAP/IAP_Main/Src/common.c              Common function source file
 - IAP/IAP_Main/Src/ymodem.c              Ymodem communication source file
 - IAP/IAP_Main/Src/system_stm32h7xx.c    STM32H7xx system source file

@par Hardware and Software environment

  - This application runs on STM32H7xx Devices.
  - This application has been tested with STMicroelectronics STM32H743I-EVAL evaluation board
    and can be easily tailored to any other supported device and development board.

@par Hardware and Software environment

  - This application runs on STM32H743xx device.

  - This application has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H743I-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the boards DB9 connector 
      and PC serial port.
    - Hold the Tamper push-button during reset to enter the IAP.    

  - Terminal configuration: 
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 
    - Ymodem protocol is using CRC16 by default. To switch to checksum, comment #define CRC16_F
      in ymodem.c
 
@par How to use it? 

In order to make the program work, you must do the following:

  1. Generate a binary image for the program provided in the 
     "IAP/IAP_Main/IAP_Binary_Template" project directory. 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP driver, keep the Tamper push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

In order to flash the IAP work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application



 */
