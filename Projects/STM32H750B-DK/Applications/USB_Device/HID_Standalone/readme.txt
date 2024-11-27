/**
  @page HID_Standalone USB Device Humain Interface (HID) application

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

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to
use USB device application based on the Human Interface (HID) on the STM32H7x devices.

In this application, the code is executed from QSPI external memory while data are in internal
SRAM memory.

This is a typical application on how to use the STM32H750xx USB OTG Device peripheral where the STM32 MCU is
enumerated as a HID device using the native PC Host HID driver to which the STM32H750B_DK
board is connected, in order to emulate the Mouse directions using "User" button mounted on the
STM32H750B_DISCO board (Press board "User" button to move the PC mouse).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H750xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz. 

The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs: main PLL1 or PLL3.

The 48 MHz clock for the USB FS is derived from the PLL3 clock (clocked by the HSE)

This application supports remote wake-up (which is the ability of a USB device to bring a suspended bus back
to the active condition), and the User button is used as the remote wake-up source.

By default, in Windows powered PC the Power Management feature of USB mouse devices is turned off.
This setting is different from classic PS/2 computer functionality. Therefore, to enable the Wake from
standby option, user must manually turn on the Power Management feature for the USB mouse.

To manually enable the wake from standby option for the USB mouse, proceed as follows:
 - Start "Device Manager",
 - Select "Mice and other pointing devices",
 - Select the "HID-compliant mouse" device (make sure that PID &VID are equal to 0x5710 & 0x0483 respectively)
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


@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_FS" when using USB Full Speed (FS) Core

@Note For the Cortex-M7, If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
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

Connectivity, USB_Device, USB, HID, Full Speed, Mouse, Joystick, Button, Remote Wakeup

@par Directory contents

  - USB_Device/HID_Standalone/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - USB_Device/HID_Standalone/Src/main.c                  Main program
  - USB_Device/HID_Standalone/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Device/HID_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_Standalone/Inc/main.h                  Main program header file
  - USB_Device/HID_Standalone/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Device/HID_Standalone/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_Standalone/Inc/usbd_desc.h             USB device HID descriptor header file

@par Hardware and Software environment

  - This application runs on STM32H750xx devices.

  - This application has been tested with STMicroelectronics STM32H750B-DISCO
    discovery boards and can be easily tailored to any other supported device
    and development board.

  - STM32H750B-DISCO Set-up
    - Connect the STM32H750B-DISCO board to the PC through 'USB micro A-Male
      to A-Male' cable to the connector:
      - CN13: to use USB Full Speed (FS)

@par How to use it ? :

In order to make the program work, you must do the following :

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc template.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this application (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load this application to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using STM32CubeIDE  :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "MT25TL01G_STM32H750B-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file relative to this application
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


 */
