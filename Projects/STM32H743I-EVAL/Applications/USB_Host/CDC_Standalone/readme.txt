/**
  @page CDC_Standalone USB Host Communication Class (CDC) application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Host CDC application.
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

This application is a part of the USB Host Library package using STM32Cube firmware. It describes how to use
USB host application based on the Communication Class (CDC) on the stm32h7xx devices.

This is a typical application on how to use the stm32h7xx USB OTG Host peripheral to operate with an USB 
CDC device application based on the two CDC transfer directions with a dynamic serial configuration: 

 - Transmission: 
   The user can select in the Host board, using the menu, a file among the ones available on the SD 
   and send it to the Device board. The content of the file could be visualized through the Hyperterminal
   (the link configuration is imposed initially by the device and could be checked using the 
   configuration menu). Data to be transmitted is stored in CDC_TX_Buffer buffer.

 - Reception: 
   The data entered by the user using the Hyperterminal in ASCII format are transferred by the device
   board to the Host board and displayed on its LCD screen. The CDC_RX_Buffer is the buffer used for 
   data reception.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from PLL3.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

When the application is started, the connected USB CDC device is detected in CDC mode and gets 
initialized. The STM32H743xx device behaves as a CDC Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial no and product name information and displays it on the LCD screen.

A menu is displayed and the user can select any operation from the menu using the Joystick buttons:
 - "Send Data" operation starts the Data Transmission.
 - "Receive Data" operation starts the Data Reception.
 - "Configuration" operation defines the desired Host CDC configuration (Baudrate,Parity, DataBit and StopBit)
   The baudrate comes with a default value of 115,2 kbps (BAUDRATE = 115200).
 - "Re-Enumerate" operation performs a new Enumeration of the device.

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
      - "USE_USB_FS" when using USB Full Speed (FS) Core 
	  
It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.

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

Connectivity, USB_Host, USB, UART, Full Speed, High Speed, CDC, PSTN, HyperTerminal, VCP, Com port

@par Directory contents

  - USB_Host/CDC_Standalone/Src/main.c                  Main program
  - USB_Host/CDC_Standalone/Src/system_stm32h7xx.c      stm32h7xx system clock configuration file
  - USB_Host/CDC_Standalone/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Host/CDC_Standalone/Src/menu.c                  CDC State Machine
  - USB_Host/CDC_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/CDC_Standalone/Src/explorer.c              Explore the uSD content
  - USB_Host/CDC_Standalone/Src/cdc_configuration.c     CDC settings State Machine
  - USB_Host/CDC_Standalone/Src/cdc_receive.c           CDC Receive State Machine
  - USB_Host/CDC_Standalone/Src/cdc_send.c              CDC Send State Machine
  - USB_Host/CDC_Standalone/Inc/main.h                  Main program header file
  - USB_Host/CDC_Standalone/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Host/CDC_Standalone/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Host/CDC_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/CDC_Standalone/Inc/ffconf.h                FAT file system module configuration file
  
@par Hardware and Software environment

  - This application runs on stm32h7xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32H743I-EVAL  Set-up
    - Insert a microSD card into the STM32H743I-EVAL uSD slot (CN13)
    - Plug the CDC device into the STM32H743I-EVAL board through 'USB micro A-Male 
      to B-Male' cable to the connector:
      - CN14 : to use USB High Speed (HS) 
      - CN18: to use USB Full Speed (FS)
        @note Make sure that :
         - jumper JP2 must be removed when using USB OTG FS

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32H743I-EVAL_USBH-HS: to configure the project for stm32h7xx devices using USB OTG HS peripheral
   - STM32H743I-EVAL_USBH-FS: to configure the project for stm32h7xx devices using USB OTG FS peripheral
 - Run the application
 

 */
