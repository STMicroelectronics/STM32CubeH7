/**
  @page DualCore_Standalone USB Host Dual Core application

  @verbatim
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host Dual Core HID and MSC application.
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
USB host application based on the STM32H7x multi core support feature integrating Mass Storage (MSC) and
Human Interface (HID) in the same project.

This is a typical application on how to use the STM32H7x USB OTG Host peripherals to operate with an USB MSC
device connected to the High Speed port and a HID device connected to the Full Speed port.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)

System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain,
the access will not be safe until �clock ready flag� of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

The Full Speed (FS) USB module uses internally a 48-MHz clock which is coming from PLL3.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

When the application is started, the connected USB MSC device to HS port is detected in MSC mode and
gets initialized, also the connected USB HID device to FS port is detected in HID mode and gets
initialized. The STM32 MCU behaves as a MSC/HID Host, it enumerates the devices and extracts VID, PID,
manufacturer name, Serial number and product name information and displays it on the LCD screen.

A menu is displayed and the user can select any operation from the menu using the Joystick buttons:
 - "Mass Storage demo" operation starts the Mass Storage application.
 - "Human Interface demo" operation starts the Human Interface application.
 - "Re-Enumerate MSC" operation performs a new Enumeration of the MSC device.
 - "Re-Enumerate HID" operation performs a new Enumeration of the HID device (keyboard/mouse).

@note In case of using an AZERTY keyboard, user should add "AZERTY_KEYBOARD" define to ensure correct
displaying taped characters.

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
      - "USE_USB_HS" and "USE_USB_FS" when using USB Dual Core Mode

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file �usbh_conf.h� available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.


@par Keywords

Connectivity, USB_Host, USB, HID, MSC, Full Speed, High Speed, OTG, enumeration, mass storage, mouse, USB Stick



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

@par Directory contents

  - USB_Host/DualCore_Standalone/CM7/Common/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - USB_Host/DualCore_Standalone/CM7/Src/main.c                  Main program
  - USB_Host/DualCore_Standalone/CM7/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Host/DualCore_Standalone/CM7/Src/dual_menu.c             Main Menu State Machine
  - USB_Host/DualCore_Standalone/CM7/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/DualCore_Standalone/CM7/Src/usbh_diskio.c           USB diskio interface for FatFs
  - USB_Host/DualCore_Standalone/CM7/Src/explorer.c              Explore the USB flash disk content
  - USB_Host/DualCore_Standalone/CM7/Src/file_operations.c       Write/read file on the disk
  - USB_Host/DualCore_Standalone/CM7/Src/msc_menu.c              MSC State Machine
  - USB_Host/DualCore_Standalone/CM7/Src/hid_menu.c              HID State Machine
  - USB_Host/DualCore_Standalone/CM7/Src/mouse.c                 HID mouse functions
  - USB_Host/DualCore_Standalone/CM7/Src/keyboard.c              HID keyboard functions
  - USB_Host/DualCore_Standalone/CM7/Inc/main.h                  Main program header file
  - USB_Host/DualCore_Standalone/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Host/DualCore_Standalone/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Host/DualCore_Standalone/CM7/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/DualCore_Standalone/CM7/Inc/ffconf.h                FAT file system module configuration file

  - USB_Host/DualCore_Standalone/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - USB_Host/DualCore_Standalone/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - USB_Host/DualCore_Standalone/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - USB_Host/DualCore_Standalone/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - USB_Host/DualCore_Standalone/CM4/Src/main.c                  Main program  for Cortex-M4


@par Hardware and Software environment

  - This application runs on STM32H7xx devices.

  - This application has been tested with STMicroelectronics STM32H747I-EVAL 
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32H747I-EVAL  Set-up
    - Plug the USB key into the STM32H747I-EVAL board through 'USB micro A-Male
      to A-Female' cable to the connector:
      - CN14 : to use USB High Speed (HS)
    - Plug the HID device(Mouse/Keyboard) into the STM32H747I-EVAL board through 'USB micro A-Male
      to A-Female' cable to the connector:
      - CN18: to use USB Full Speed (FS)
     @note Make sure that :
       - jumper JP2 must be removed when using USB OTG FS

@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain.
 - For CM4 target configuration (STM32H747I-Eval_CM4) :
     - Rebuild all files
     - Load image into target memory

 - For CM7 target configuration :
 - Rebuild all files and load your image into target memory
 - Run the application


 */
