﻿/**
  @page HID_Standalone USB Host Human Interface (HID) application

  @verbatim
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host HID application.
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

This application is a part of the USB Host Library package using STM32Cube firmware. It describes how to use
USB host application based on the Human Interface Class (HID) on the STM32H7B3XXQ devices.

This is a typical application on how to use the STM32H7B3XXQ USB OTG Host peripheral to interact with an USB
HID Device such as a Mouse or a Keyboard.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3XXQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.

The High speed OTG IP in full speed mode USB module uses
internally a 48-MHz clock which is coming from PLL3.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

When the application is started, the connected HID device (Mouse/Keyboard) is detected in HID mode and 
gets initialized. This application is based on interacting with a HID device (Mouse/Keyboard) through a HID routine.

The STM32 MCU behaves as a HID Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial number and product name information and displays it on The UART Hyperterminal.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720
"STM32Cube USB Host library".


@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_HS_IN_FS" when using USB High speed OTG IP in full speed Core

- @note In case of using an AZERTY keyboard, user should add "AZERTY_KEYBOARD" define to ensure correct 
displaying taped characters.

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages to uart terminal, stm32h7xx_hal_uart.c driver needs to be added to the application sources.
   Debug messages are displayed on the uart terminal using ST-Link.


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Connectivity, USB_Host, USB, HID, Human Interface, Full Speed, High Speed, Mouse, Keyboard

@par Directory contents

  - USB_Host/HID_Standalone/Core/Src/main.c                   Main program
  - USB_Host/HID_Standalone/Core/Src/system_stm32h7xx.c       STM32H7xx system clock configuration File
  - USB_Host/HID_Standalone/Core/Src/stm32h7xx_it.c           Interrupt handlers
  - USB_Host/HIS_Standalone/Core/Src/stm32h7xx_hal_msp.c      HAL MSP Module
  - USB_Host/HID_Standalone/Core/Inc/main.h                   Main program header File
  - USB_Host/HID_Standalone/Core/Inc/stm32h7xx_hal_conf.h     HAL configuration File
  - USB_Host/HID_Standalone/Core/Inc/stm32h7b3i_eval_conf.h   Hardware configuration File
  - USB_Host/HID_Standalone/Core/Inc/stm32h7xx_it.h           Interrupt handlers header File
  - USB_Host/HID_Standalone/USB_Host/App/usb_host.c           HID State Machine
  - USB_Host/HID_Standalone/USB_Host/App/mouse.c              HID mouse functions File
  - USB_Host/HID_Standalone/USB_Host/App/keyboard.c           HID keyboard functions File
  - USB_Host/HID_Standalone/USB_Host/App/usb_host.h           HID State Machine Header File
  - USB_Host/HID_Standalone/USB_Host/App/mouse.h              HID mouse functions Header File
  - USB_Host/HID_Standalone/USB_Host/App/keyboard.h           HID keyboard functions Header File
  - USB_Host/HID_Standalone/USB_Host/Target/usbh_conf.c       General low level driver configuration
  - USB_Host/HID_Standalone/USB_Host/Target/usbh_conf.h       USB Host driver configuration File

@par Hardware and Software environment

  - This application runs on STM32H7B3XXQ devices.

  - This application has been tested with STMicroelectronics STM32H7B3I-EVAL
    boards and can be easily tailored to any other supported device
    and development board.

  - STM32H7B3I-EVAL Set-up
    - Plug the Mouse/Keyboard into the STM32H7B3I-EVAL board through 'USB micro A-Male
      to A-Female' cable to the connector:
      - CN8 : to use USB High speed (HS), Make sure that JP9, JP11 and JP19 are fitted in USBHS position and that JP10 and JP16 are fitted.
      - CN27: to use USB High speed OTG IP in full speed (HS_IN_FS)
  -Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.
    A virtual COM port will then appear in the HyperTerminal:

@note Make sure that jumpers JP20 and JP21 are fitted.

      - Hyperterminal configuration:
      - Data Length = 8 Bits
      - One Stop Bit
      - No parity
      - BaudRate = 115200 baud
      - Flow control: None

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Open the configured uart hyperterminal in order to display debug messages.
 - Run the application
 - Open the configured uart hyperterminal in order to display debug messages.
@note
   The user has to check the list of the COM ports in Device Manager to find out the number of the
   COM ports that have been assigned (by OS) to the Stlink VCP .



 */
