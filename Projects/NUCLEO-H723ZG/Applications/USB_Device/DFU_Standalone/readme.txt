/**
  @page DFU_Standalone USB Device Firmware Upgrade (DFU) application

  @verbatim
  ******************************************************************************
  * @file    USB_Device/DFU_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB DFU application.
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

Compliant implementation of the Device Firmware Upgrade (DFU)
capability to program the embedded Flash memory through the USB peripheral.


At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 520 MHz.
The high speed OTG IP in full speed mode (HS_IN_FS) uses
internally a 48-MHz clock, which is generated from HSI48.
The DFU transactions are based on Endpoint 0 (control endpoint) transfer. All requests and status
control are sent/received through this endpoint.

The Internal flash memory is split as follows:
 - DFU area located in [0x08000000 : USBD_DFU_APP_DEFAULT_ADD-1]: Only read access
 - Application area located in [USBD_DFU_APP_DEFAULT_ADD : Device's end address]: Read, Write, and Erase
   access

In this application, two operating modes are available:
 1. DFU operating mode:
    This mode is entered after an MCU reset in case:
     - The DFU mode is forced by the user: the user presses the TAMP button.
     - No valid code found in the application area: a code is considered valid if the MSB of the initial
       Main Stack Pointer (MSP) value located in the first address of the application area is equal to
       0xC000

 2. Run-time application mode:
    This is the normal run-time activities. A binary which toggles LEDs on the NUCLEO-H723ZG board is
    provided in Binary directory.

@note After each device reset (unplug the NUCLEO-H723ZG board from PC), Plug the NUCLEO-H723ZG board with Key User push-button button
pressed to enter the DFU mode.

Traditionally, firmware is stored in Hex, S19 or Binary files, but these formats do not contain the
necessary information to perform the upgrade operation, they contain only the actual data of the program
to be downloaded. However, the DFU operation requires more information, such as the product identifier,
vendor identifier, Firmware version and the Alternate setting number (Target ID) of the target to be
used, this information makes the upgrade targeted and more secure. To add this information, DFU file
format is used. For more details refer to the "DfuSe File Format Specification" document (UM0391).

To generate a DFU image, download "DFUse Demonstration" tool and use DFU File Manager to convert a
binary image into a DFU image. This tool is for download from www.st.com
To download a *.dfu image, use "DfuSe Demo" available within "DFUse Demonstration" install directory.
To download a *.bin or *.hex image, use "STM32CubeProgrammer" available from ST web site.

Please refer to UM0412, DFuSe USB device firmware upgrade STMicroelectronics extension for more details
on the driver installation and PC host user interface.

@note A binary which toggles LEDs on the NUCLEO-H723ZG board is provided in Binary directory.

@note The application needs to ensure that the SysTick time base is set to 1 millisecond
      to have correct HAL configuration.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The build-in USB peripheral of the stm32h7xx does not provide a specific interrupt for USB cable plug/unplug
      detection. The correct way to detect the USB cable plug/unplug is to detect the availability of the VBUS line
      using a normal GPIO pin (external interrupt line).


@par USB Library Configuration

It is possible to fine tune needed USB Device features by modifying defines values in USBD configuration
file “usbd_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
 - USBD_DFU_APP_DEFAULT_ADD, specifying the address from where user's application will be downloaded.

Device's end address is the end address of the flash memory and it is dependent on the device in use.

@par Keywords

Connectivity, USB_Device, USB, DFU, Firmware upgrade

@par Directory contents

  - USB_Device/DFU_Standalone/Core/Src/main.c                           Main program
  - USB_Device/DFU_Standalone/Core/Src/system_stm32h7xx.c               stm32h7xx system clock configuration file
  - USB_Device/DFU_Standalone/Core/Src/stm32h7xx_it.c                   Interrupt handlers
  - USB_Device/DFU_Standalone/USB_Device/App/usb_device.c               USB Device application code
  - USB_Device/DFU_Standalone/USB_Device/App/usb_desc.c                 USB device descriptor
  - USB_Device/DFU_Standalone/USB_Device/App/usbd_dfu_flash.c           Internal flash memory management
  - USB_Device/DFU_Standalone/USB_Device/Target/usbd_conf.c             General low level driver configuration
  - USB_Device/DFU_Standalone/Core/Inc/main.h                           Main program header file
  - USB_Device/DFU_Standalone/Core/Inc/stm32h7xx_it.h                   Interrupt handlers header file
  - USB_Device/DFU_Standalone/Core/Inc/stm32h7xx_hal_conf.h             HAL configuration file
  - USB_Device/DFU_Standalone/USB_Device/App/usb_device.h               USB Device application header file
  - USB_Device/DFU_Standalone/USB_Device/App/usbd_desc.h                USB device descriptor header file
  - USB_Device/DFU_Standalone/USB_Device/App/usbd_dfu_flash.h           Internal flash memory management header file
  - USB_Device/DFU_Standalone/USB_Device/Target/usbd_conf.h             USB device driver Configuration file


@par Hardware and Software environment

  - This application runs on STM32H723xx devices.

  - This application has been tested with STMicroelectronics NUCLEO-H723ZG
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-H723ZG board Set-up
    - Install the DFU driver available in "DfuSe Demonstrator" installation directory
      For Windows 8.1 and later : Update STM32 DFU device driver manually from Windows Device Manager.
      The install of required device driver is available under:
      "Program Files\STMicroelectronics\Software\DfuSe v3.0.5\Bin\Driver\Win8.1" directory.
    - Connect the NUCLEO-H723ZG board to the PC.
    - Connect the NUCLEO-H723ZG board to the PC through micro A-Male to standard A Male cable connected to the connector.
     - CN13 : to use USB High Speed OTG IP in full speed (HS_IN_FS)

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - To run the application, proceed as follows:

  * With "DfuSe Demonstrator":
   - Install "DfuSe Demonstrator"
   - Install the DFU driver available in "DfuSe Demonstrator" installation directory
   - For Windows 8.1 and later : Update STM32 DFU device driver manually from Windows Device Manager.
     The install of required device driver is available under:
     "Program Files\STMicroelectronics\Software\DfuSe v3.0.6\Bin\Driver\Win8.1" directory.
   - Open "DfuSe Demo", choose the "STM32H7xx_Nucleo_LED_Toggle@0x08020000.dfu" provided in Binary
     directory, upgrade and verify to check that it is successfully downloaded.
   - This application allows also to upload a dfu file (either the provided DFU file or by creating a new dfu file).
     To check that the upload was successfully performed, choose the dfu uploaded file, upgrade and verify.
   - To run the downloaded application, execute the command "leave the DFU mode" or simply reset the board.

  * With "STM32CubeProgrammer":
   - Download the appropriate STM32CubeProgrammer from ST web site and install it.
   - Once you've updated the STM32 DFU device driver with DfuSe Driver you must verify from Windows Device Manager
     that DFU driver is selected with the STM32CubeProgrammer driver.
   - Open "STM32CubeProgrammer", select the usb interface and connect to DFU device.
   - Download the "STM32H7xx_Nucleo_LED_Toggle@0x08020000.bin" provided in Binary directory and set the start address.
   - To run the downloaded application, reset the board.


 */
