/**
  @page DualCore_Standalone USB Device Dual Core application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/DualCore_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Device Dual Core application.
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

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to use
USB device application based on the STM32H7xx multi core support feature integrating the Device Communication 
Class (CDC) and Human Interface (HID) in the same project.

This is a typical application on how to use the STM32H7xx USB OTG Device peripheral, where STM32 is 
enumerated as a CDC device in the High Speed mode, and also as a HID device in the Full Speed mode,
using the native PC Host HID/CDC drivers to which the STM32H743I-EVAL board is connected.

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

When the application is started, the user has just to plug the two USB cables into a PC host and two
USB devices (CDC/HID) are automatically detected. A new VCP drive appears in the system window
for the CDC device and serial communication operations can be performed as with any other serial
communication drive. A new HID Mouse device is detected, and the Joystick buttons mounted on 
the STM32H743I-EVAL board, allow to user to emulate the Mouse directions.

Hyperterminal configuration:
  - Word Length = 8 Bits
  - One Stop Bit
  - None parity
  - BaudRate = 115200
  - flow control: None 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".

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

Connectivity, USB_Device, USB, HID, Full Speed, High Speed, CDC, OTG, Enumeration, Joystick, Mouse

@par Directory contents

  - USB_Device/DualCore_Standalone/Src/main.c                  Main program
  - USB_Device/DualCore_Standalone/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - USB_Device/DualCore_Standalone/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Device/DualCore_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/DualCore_Standalone/Src/usbd_cdc_desc.c         USB device CDC descriptor
  - USB_Device/DualCore_Standalone/Src/usbd_hid_desc.c         USB device HID descriptor
  - USB_Device/DualCore_Standalone/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - USB_Device/DualCore_Standalone/Src/usbd_cdc_interface.c    USBD CDC interface 
  - USB_Device/DualCore_Standalone/Inc/main.h                  Main program header file
  - USB_Device/DualCore_Standalone/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Device/DualCore_Standalone/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Device/DualCore_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/DualCore_Standalone/Inc/usbd_desc.h             USB device descriptor header file  
  - USB_Device/DualCore_Standalone/Inc/usbd_cdc_desc.h         USB device CDC descriptor header file
  - USB_Device/DualCore_Standalone/Inc/usbd_hid_desc.h         USB device HID descriptor header file
  - USB_Device/DualCore_Standalone/Inc/usbd_cdc_interface.h    USBD CDC interface header file
	
@par Hardware and Software environment

  - This application runs on STM32H743xx devices.
    
  - This application has been tested with STMicroelectronics STM32H743I-EVAL  
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32H743I-EVAL  Set-up
    - Connect the STM32H743I-EVAL board to the PC through two  'USB micro A-Male
      to A-Male' cables to the connector:
       - CN14 : to use USB High Speed (HS) for CDC device
       - CN18: to use USB Full Speed (FS) for HID device
              Please ensure that jumper JP2 is not fitted.
    - Connect the STM32H743I-EVAL board to the PC (or to another evaluation board) through RS232 (USART)
      serial cable CN2 connector. Please ensure that jumper JP7 and JP8 are fitted in RS232 TX/RX position.        
    - For loopback mode test: remove RS232 cable on CN2 and connect directly USART TX and RX pins:
      PB14 and PB15 (with a cable or a jumper)
      
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

 */
