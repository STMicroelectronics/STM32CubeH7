/**
  @page HID_Standalone USB Device Human Interface (HID) application

  @verbatim
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB HID application.
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

Use of the USB device application based on the Human Interface (HID).

This is a typical application on how to use the STM32H7xx USB OTG Device peripheral where the STM32 MCU is
enumerated as a HID device using the native PC Host HID driver to which the NUCLEO-H7A3ZI-Q
board is connected, in order to emulate the Mouse directions using User push-button mounted on the
NUCLEO-H7A3ZI-Q board.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 280 MHz.

This example supports remote wakeup (which is the ability of a USB device to bring a suspended bus back
to the active condition), and the User push-button is used as the remote wakeup source.

By default, in Windows powered PC the Power Management feature of USB mouse devices is turned off.
This setting is different from classic PS/2 computer functionality. Therefore, to enable the Wake from
standby option, user must manually turn on the Power Management feature for the USB mouse.

To manually enable the wake from standby option for the USB mouse, proceed as follows:
 - Start "Device Manager",
 - Select "Mice and other pointing devices",
 - Select the "HID-compliant mouse" device (make sure that PID & VID are equal to 0x5710 & 0x0483 respectively)
 - Right click and select "Properties",
 - Select "Power Management" tab,
 - Finally click to select "Allow this device to wake the computer" check box.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Device library, please refer to UM1734
"STM32Cube USB Device library".

@par Keywords

Connectivity, USB_Device, USB, HID, Full Speed, Mouse, Remote Wakeup

@par Directory contents

  - USB_Device/HID_Standalone/Core/Src/main.c                  Main program
  - USB_Device/HID_Standalone/Core/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - USB_Device/HID_Standalone/Core/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Device/HID_Standalone/USB_Device/Target/usbd_conf.c    General low level driver configuration
  - USB_Device/HID_Standalone/USB_Device/App/usbd_desc.c       USB device HID descriptor
  - USB_Device/HID_Standalone/USB_Device/App/usbd_device.c     USB Device
  - USB_Device/HID_Standalone/Core/Inc/main.h                  Main program header file
  - USB_Device/HID_Standalone/Core/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Device/HID_Standalone/Core/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_Standalone/USB_Device/Target/usbd_conf.h    USB device driver Configuration file
  - USB_Device/HID_Standalone/USB_Device/App/usbd_desc.h       USB device HID descriptor header file
  - USB_Device/HID_Standalone/USB_Device/App/usbd_device.h     USB Device header


@par Hardware and Software environment

  - This application runs on STM32H7xx devices.

  - This application has been tested with STMicroelectronics NUCLEO-H7A3ZI-Q board
    and can be easily tailored to any other supported device and development board.

  - NUCLEO-H7A3ZI-Q board Set-up
    - Connect the NUCLEO-H7A3ZI-Q board to the PC through micro A-Male to standard A Male cable connected to the connector.
     - CN13 : to use USB High Speed OTG IP in full speed (HS_IN_FS)

    - Press the User push-button  to move the cursor.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */
