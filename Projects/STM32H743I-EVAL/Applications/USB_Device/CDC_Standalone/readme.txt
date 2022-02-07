/**
  @page CDC_Standalone USB Device Communication (CDC) application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Device/CDC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Device CDC application.
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

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to 
use USB device application based on the Device Communication Class (CDC) following the PSTN subprotocol
in the STM32H743xx devices using the OTG-USB and UART peripherals.

This is a typical application on how to use the STM32H743xx USB OTG Device peripheral where the STM32 MCU
behaves as a USB-to-RS232 bridge following the Virtual COM Port (VCP) implementation.
 - On one side, the STM32 exchanges data with a PC host through USB interface in Device mode.
 - On the other side, the STM32 exchanges data with other devices (same host, other host,
   other devices�) through the UART interface (RS232).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs: main PLL1 or PLL3.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS is derived from the PLL3 clock (clocked by the HSE)

When the VCP application is started, the STM32 MCU is enumerated as serial communication port and is
configured in the same way (baudrate, data format, parity, stop bit) as it would configure a standard 
COM port. The 7-bit data length with no parity control is NOT supported.

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

In High Speed (HS) mode, enabling USB-DMA will result in data being sent only by multiple of 4 packet 
sizes.  This is due to the fact that USB DMA does not allow sending data from non word-aligned addresses.
For this specific application, it is advised to not enable the DMA capability unless required.

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
      
@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, USB_Device, USB, UART, Full Speed, High Speed, CDC, PSTN,  USB-to-RS232, Bridge, VCP, Com port

@par Directory contents 

  - USB_Device/CDC_Standalone/Src/main.c                  Main program
  - USB_Device/CDC_Standalone/Src/system_stm32h7xx.c      STM32H743xx system clock configuration file
  - USB_Device/CDC_Standalone/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Device/CDC_Standalone/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - USB_Device/CDC_Standalone/Src/usbd_cdc_interface.c    USBD CDC interface
  - USB_Device/CDC_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/CDC_Standalone/Src/usbd_desc.c             USB device CDC descriptor
  - USB_Device/CDC_Standalone/Inc/main.h                  Main program header file
  - USB_Device/CDC_Standalone/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Device/CDC_Standalone/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Device/CDC_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/CDC_Standalone/Inc/usbd_desc.h             USB device MSC descriptor header file
  - USB_Device/CDC_Standalone/Inc/usbd_cdc_interface.h    USBD CDC interface header file  


@par Hardware and Software environment

  - This application runs on STM32H743xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL  
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32H743I-EVAL  Set-up
    - Connect the STM32H743I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN14 : to use USB High Speed (HS) 
      - CN18: to use USB Full Speed (FS)
              Please ensure that jumper JP2 is not fitted.
    - Connect the STM32H743I-EVAL board to the PC (or to another evaluation board) through RS232 (USART)
      serial cable CN2 connector. Please ensure that jumper JP7 and JP8 are fitted in RS232 TX/RX position.        
    - For loopback mode test: remove RS232 cable on CN2 and connect directly USART TX and RX pins:
      PB14 and PB15 (with a cable or a jumper)


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32H743I-EVAL_USBH-HS: to configure the project for STM32H7xx devices using USB OTG HS peripheral
   - STM32H743I-EVAL_USBH-FS: to configure the project for STM32H7xx devices using USB OTG FS peripheral
   
 - Run the application
 - Install the USB virtual COM port driver
 - Find out the number of the COM port assigned to the STM32 CDC device
 - Open a serial terminal application and start the communication


 */
