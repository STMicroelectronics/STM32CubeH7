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

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to
use USB device application based on the Device Firmware Upgrade (DFU) on the STM32H7xx devices.

This is a typical application on how to use the STM32H7xx USB OTG Device peripheral to implement the DFU
capability for programming the embedded flash memory of the STM32H7xx devices.

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
the access will not be safe until “clock ready flag” of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs: main PLL1 or PLL3.

The 48 MHz clock for the USB FS is derived from the PLL3 clock (clocked by the HSE)

The DFU transactions are based on Endpoint 0 (control endpoint) transfer. All requests and status
control are sent/received through this endpoint.

The Internal flash memory is split as follows:
 - DFU area located in [0x08000000 : USBD_DFU_APP_DEFAULT_ADD-1]: Only read access
 - Application area located in [USBD_DFU_APP_DEFAULT_ADD : Device's end address]: Read, Write, and Erase
   access

In this application, two operating modes are available:
 1. DFU operating mode:
    This mode is entered after an MCU reset in case:
     - The DFU mode is forced by the user: the user presses the key button.
     - No valid code found in the application area: a code is considered valid if the MSB of the initial
       Main Stack Pointer (MSP) value located in the first address of the application area is equal to
       0x2000

 2. Run-time application mode:
    This is the normal run-time activities. A binary which toggles LEDs on the discovery board is
    provided in Binary directory.

@note After each device reset, hold down the User button on the evaluation board to enter the DFU mode.

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

It is possible to fine tune needed USB Device features by modifying defines values in USBD configuration
file “usbd_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
 - USBD_DFU_APP_DEFAULT_ADD, specifying the address from where user's application will be downloaded.

Device's end address is the end address of the flash memory and it is dependent on the device in use.

@par Keywords

Connectivity, USB_Device, USB, DFU, Firmware upgrade

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

@par Directory contents

  - USB_Device/DFU_Standalone/CM7/Common/Src/system_stm32h7xx.c      stm32h7xx system clock configuration file
  - USB_Device/DFU_Standalone/CM7/Src/main.c                  Main program
  - USB_Device/DFU_Standalone/CM7/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Device/DFU_Standalone/CM7/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/DFU_Standalone/CM7/Src/usbd_desc.c             USB device descriptor
  - USB_Device/DFU_Standalone/CM7/Src/usbd_dfu_flash.c        Internal flash memory management
  - USB_Device/DFU_Standalone/CM7/Inc/main.h                  Main program header file
  - USB_Device/DFU_Standalone/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Device/DFU_Standalone/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Device/DFU_Standalone/CM7/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/DFU_Standalone/CM7/Inc/usbd_desc.h             USB device descriptor header file
  - USB_Device/DFU_Standalone/CM7/Inc/usbd_dfu_flash.h        Internal flash memory management header file

  - USB_Device/DFU_Standalone/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - USB_Device/DFU_Standalone/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - USB_Device/DFU_Standalone/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - USB_Device/DFU_Standalone/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - USB_Device/DFU_Standalone/CM4/Src/main.c                  Main program  for Cortex-M4

@par Hardware and Software environment

  - This application runs on STM32H745xx devices.

  - This application has been tested with STMicroelectronics STM32H745I-DISCO
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32H745I-DISCO
    - Connect the STM32H745I-DISCO board to the PC through 'USB micro A-Male
      to A-Male' cable to the connector:
      - CN13: to use USB Full Speed (FS)

@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain
 - For CM4 target configuration (STM32H745I-DISCO_CM4) :
     - Rebuild all files
     - Load image into target memory

 - For CM7 target configuration  :
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32H745I-DISCO_USBD-FS: to configure the project for stm32h7xx devices using USB OTG FS peripheral
 - To run the application, proceed as follows:

   * With "DfuSe Demonstrator":
    - Install "DfuSe Demonstrator"
    - Install the DFU driver available in "DfuSe Demonstrator" installation directory
      For Windows 8.1 and later, the STM32 DFU device driver needs to be updated manually from Windows Device Manager.
      The required device driver is available under "Program Files\STMicroelectronics\Software\DfuSe v3.0.5\Bin\Driver\Win8.1" directory.
    - Open "DfuSe Demo" and download the "STM32H745I_DISCO_USBD_0x08020000.dfu" provided in Binary directory.
    - To run the downloaded application, execute the command "leave the DFU mode" or simply reset the board.

   * With "STM32CubeProgrammer":
    - Download the appropriate STM32CubeProgrammer from ST web site and install it.
    - Once you've updated the STM32 DFU device driver with DfuSe Driver you must verify from Windows Device Manager
      that DFU driver is selected with the STM32CubeProgrammer driver.
    - Open "STM32CubeProgrammer", select the usb interface and connect to DFU device.
    - Download the "STM32H745I_DISCO_USBD_0x08020000.bin" provided in Binary directory and set the start address.
    - To run the downloaded application, reset the board.


 */
