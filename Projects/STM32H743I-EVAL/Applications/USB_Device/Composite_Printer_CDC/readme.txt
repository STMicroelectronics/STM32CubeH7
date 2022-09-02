/**
  @page Composite_Printer_CDC USB Device application

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    USB_Device/Composite_Printer_CDC/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Composite_Printer_CDC application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                     www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description


This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to
use USB device application based on the Printer CDC composite device on the STM32H743xx devices.

This is a typical application on how to use the STM32H743xx USB OTG Device peripheral where the STM32 MCU is
enumerated as a composite device containing 2 functions:
    1 CDC functions: A typical application on how to use the STM32H743xx USB OTG Device peripheral where the STM32 MCU
                     behaves as a USB-to-RS232 bridge following the Virtual COM Port (VCP) implementation.
                      - On one side, the STM32 exchanges data with a PC host through USB interface in Device mode.
                      - On the other side, the STM32 exchanges data with other devices (same host, other host,
                        other devices…) through the UART interface (RS232).
                     This function is duplicated so that PC Host revognizes two different instances of CDC VCP ports.
    1 Printer function: Use of the USB device application based on the Device Printer Class (PRNT) and
			following the printer subprotocol. This application uses the USB Device .
			A typical application on how to use the STM32H7xx USB OTG Device peripheral where the STM32 MCU
			behaves as a USB printing support implementation.
		        - the STM32 receive text files  sent by  PC host through USB interface in Device mode.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 216 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs: main PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL VCO clock must be programmed
    to output 432 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI VCO clock must be programmed
    to output 384 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).



CDC Function:
---------------
When the VCP application is started, the STM32 MCU is enumerated as serial communication port and is
configured in the same way (baudrate, data format, parity, stop bit) as it would configure a standard
COM port. The 7-bit data length with no parity control is NOT supported.

All description below is duplicated twice for the CDC (two CDC instances are enumerated at same time)
so that two VCP ports can be used simultaneously and with different configurations.
For the purpose of this example, the two CDC instances are mapped to the same physical UART interface,
but this can be easilly changed afterwards by modifying the low layer interface in usbd_cdc_interface.c/.h

During enumeration phase, three communication pipes "endpoints" are declared in the CDC class
implementation (PSTN sub-class):
 - 1 x Bulk IN endpoint for receiving data from STM32 device to PC host:
   When data are received over UART they are saved in the buffer "UserTxBuffer". Periodically, in a
   timer callback the state of the buffer "UserTxBuffer" is checked. If there are available data, they
   are transmitted in response to IN token otherwise it is NAKed.
   The polling period depends on "CDC_POLLING_INTERVAL" value.

 - 1 x Bulk OUT endpoint for transmitting data from PC host to STM32 device:
   When data are received through this endpoint they are saved in the buffer "UserRxBuffer" then they
   are transmitted over UART using interrupt mode and in meanwhile the OUT endpoint is NAKed.
   Once the transmission is over, the OUT endpoint is prepared to receive next packet in
   HAL_UART_TxCpltCallback().

 - 1 x Interrupt IN endpoint for setting and getting serial-port parameters:
   When control setup is received, the corresponding request is executed in CDC_Itf_Control().
   In this application, two requests are implemented:
    - Set line: Set the bit rate, number of Stop bits, parity, and number of data bits
    - Get line: Get the bit rate, number of Stop bits, parity, and number of data bits
   The other requests (send break, control line state) are not implemented.

@note Receiving data over UART is handled by interrupt while transmitting is handled by DMA allowing
      hence the application to receive data at the same time it is transmitting another data (full-
      duplex feature).

The support of the VCP interface is managed through the ST Virtual COM Port driver available for
download from www.st.com.

@note The user has to check the list of the COM ports in Device Manager to find out the number of the
      COM ports that have been assigned (by OS) to the VCP interface.

This application uses UART as a communication interface. The UART instance and associated resources
(GPIO, NVIC) can be tailored in "usbd_cdc_interface.h" header file according to your hardware
configuration. Moreover, this application can be customized to communicate with interfaces other than UART.
For that purpose a template CDC interface is provided in:
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src directory.

To run this application, the user can use one of the following configurations:

 - Configuration 1:
   Connect USB cable to host and UART (RS232) to a different host (PC or other device) or to same host.
   In this case, you can open two hyperterminals to send/receive data to/from host to/from device.

 - Configuration 2:
   Connect USB cable to Host and connect UART TX pin to UART RX pin on the STM32H743I-EVAL board
   (Loopback mode). In this case, you can open one terminal (relative to USB com port or UART com port)
   and all data sent from this terminal will be received by the same terminal in loopback mode.
   This mode is useful for test and performance measurements.

@note If using this example in loopback mode (ie. USART Tx IO connected to USART Rx IO on STM32 side)
and with baudrates higher than 9600, there might be communication errors.


Printer Function:
------------------

When the printer application is started, the STM32 MCU is enumerated as  Printer port

During enumeration phase, two Printer pipes "endpoints" are declared in the PRNT class
implementation (printer sub-class):

 - 1 x Bulk OUT endpoint for transmitting text files  from PC host to STM32 device:
   When data are received through this endpoint they are saved in the buffer "UserRxBuffer" then they
   are transmitted over FatFS to SD card
 - 1 x Bulk IN endpoint for transmitting  data from STM32 device to PC host:
   return status or printer related information

@note The user has to check the list of the USB bus controllers in Device Manager to find USB printing support

This application uses FatFS Middlewares to to save the printed files in the SD card

In High Speed (HS) mode, enabling USB-DMA will result in data being sent only by multiple of 4 packet
sizes.  This is due to the fact that USB DMA does not allow sending data from non word-aligned addresses.
For this specific application, it is advised to not enable the DMA capability unless required.

uSD Configuration:

- Link the uSD disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Close the open text file;
 - Check on read data from text file;
 - Unlink the uSD disk I/O driver.

To run this application, the user can use the following configurations:

- Connect USB cable to host .
- Add the printer to Windows Control Panel :
 • Open the Control Panel, select Devices and Printers and then Add Printer.
 • Click The printer that I want isn’t listed.
 • Select Add a local printer or network printer with manual settings, then click Next.
 • Select Use an existing port and USB001/USB003 (Virtual printer port for USB), then click Next.
 • Select Generic and Generic / Text Only, then click Next.
 • Select Use the driver that is currently installed (recommended), then click Next.
 • Change the name to STM32 Printer class FS/HS mode , then click Next.
 • Select Do not share this printer, then click Next.
 • Click Finish.

 @note pages management

 the text files sent to STM32 will not be identical to text file saved in SDCard because the host added spaces and line breaks to make the page setup,
 there is the printer page formatting in the middle,The most common formats are Unix and Windows format.
 A primary difference is that different character or sequence of characters is used to signify an end of a line.
 On Unix, it’s LF character (\n, 0A or 10 in decimal).
On Windows, it’s a sequence of two characters, CR and LF (\r + \n, 0D + 0A or 13 + 10 in decimal).

@note To use USB Device Librairy v2.10.0, USE_USB_DEVICE_LIB_V2_10_0 should be defined. To use  USB Device 
Librairy v2.11.0, USE_USB_DEVICE_LIB_V2_10_0 should not be defined.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Device library, please refer to UM1734
"STM32Cube USB Device library".


@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_FS" when using USB Full Speed (FS) Core

@par Keywords

Connectivity, USB Device, Printer,  Full Speed, High Speed, SD Card,
Write, Read, Format

@par Directory contents

  - USB_Device/Composite_Printer_CDC/Src/usbd_conf.c            General low level driver configuration
  - USB_Device/Composite_Printer_CDC/Inc/usbd_conf.h            USB device driver Configuration file
  - USB_Device/Composite_Printer_CDC/Src/usbd_cdc_interface.c   USBD CDC interface
  - USB_Device/Composite_Printer_CDC/Inc/usbd_cdc_interface.h   USBD CDC interface header file
  - USB_Device/Composite_Printer_CDC/Src/usbd_desc.c            USB device descriptor
  - USB_Device/Composite_Printer_CDC/Inc/usbd_desc.h            USB device descriptor header file
  - USB_Device/Composite_Printer_CDC/Src/main.c                 Main program
  - USB_Device/Composite_Printer_CDC/Src/stm32h7xx_hal_msp.c    Board specific components managmeent
  - USB_Device/Composite_Printer_CDC/Src/stm32h7xx_it.c         Interrupt handlers
  - USB_Device/Composite_Printer_CDC/Src/system_stm32h7xx.c     System configuration management
  - USB_Device/Composite_Printer_CDC/Inc/main.h                 Main program header file
  - USB_Device/Composite_Printer_CDC/Inc/stm32h7xx_hal_conf.h   HAL configuration file
  - USB_Device/Composite_Printer_CDC/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - USB_Device/Composite_Printer_CDC/Src/ubsd_printer_if.c      printer Interface user
  - USB_Device/Composite_Printer_CDC/Inc/ubsd_printer_if.h      printer Interface user header file

@par Hardware and Software environment

  - This application runs on STM32H743xx devices.

  - This application has been tested with STMicroelectronics STM32H743I-Eval
    boards and can be easily tailored to any other supported device and development
    board.

  - STM32746G-Discovery Set-up
    - Insert a microSD card into the STM32H743I-Eval uSD slot (CN3)
    - Connect the STM32746G-Discovery board to the PC through 'USB micro A-Male
      to A-Male' cable to the connector:
      - CN12 : to use USB High Speed (HS)
      - CN13: to use USB Full Speed (FS)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32746G-DISCOVERY_USBH-HS: to configure the project for STM32H743xx devices using USB OTG HS peripheral
   - STM32746G-DISCOVERY_USBH-FS: to configure the project for STM32H743xx devices using USB OTG FS peripheral
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */




