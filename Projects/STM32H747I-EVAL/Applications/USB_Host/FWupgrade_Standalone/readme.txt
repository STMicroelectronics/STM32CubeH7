/**
  @page FWupgrade_Standalone USB Host Firmware Upgrade application

  @verbatim
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host Firmware Upgrade application
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
USB host application based on the In-Application programming (IAP) on the STM32H7xx devices.

This is a typical application on how to use the STM32H7xx USB OTG Host peripheral for firmware upgrade
application or IAP, allowing user to erase and write to on-chip flash memory.

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

The Full Speed (FS) USB module uses internally a 48-MHz clock which is coming from PLL3.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

This application uses the USB Host to:
 - DOWNLOAD: Reads the defined image (.bin) file “DOWNLOAD_FILENAME” from the thumb drive and writes it
             to the embedded Flash memory.
 - UPLOAD:   Reads the entire embedded Flash memory and saves the contents to the defined file name
             “UPLOAD_FILENAME” in the thumb drive.
 - JUMP:     Executes the user code at the defined user application start address “APPLICATION_ADDRESS”.
             Image which must be defined from this flash address: 0x08020000

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

The user application (binary file) to be loaded into the Flash memory using the firmware upgrade
application is built by the following configuration settings:
	- Set the program load address to APPLICATION_ADDRESS in the toolchain linker file.
	- Relocate the vector table to address APPLICATION_ADDRESS using the VECT_TAB_OFFSET definition
	  inside the system_stm32h7xx.c file.

@par Keywords

Connectivity, USB_Host, USB, Firmware upgrade, IAP, Binary



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

@par Directory contents

  - USB_Host/FWupgrade_Standalone/Common/CM7/Src/system_stm32h7xx.c      STM32H7xx system clock configuration file
  - USB_Host/FWupgrade_Standalone/CM7/Src/main.c                  Main program
  - USB_Host/FWupgrade_Standalone/CM7/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Host/FWupgrade_Standalone/CM7/Src/iap_menu.c              IAP State Machine
  - USB_Host/FWupgrade_Standalone/CM7/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/FWupgrade_Standalone/CM7/Src/command.c               IAP command functions
  - USB_Host/FWupgrade_Standalone/CM7/Src/flash_if.c              Flash layer functions
  - USB_Host/FWupgrade_Standalone/CM7/Src/usbh_diskio.c           USB diskio interface for FatFs
  - USB_Host/FWupgrade_Standalone/CM7/Inc/main.h                  Main program header file
  - USB_Host/FWupgrade_Standalone/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Host/FWupgrade_Standalone/CM7/Inc/command.h               IAP command functions header file
  - USB_Host/FWupgrade_Standalone/CM7/Inc/flash_if.h              Flash layer functions header file
  - USB_Host/FWupgrade_Standalone/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Host/FWupgrade_Standalone/CM7/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/FWupgrade_Standalone/CM7/Inc/ffconf.h                FatFs Module Configuration file

  - USB_Host/FWupgrade_Standalone/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - USB_Host/FWupgrade_Standalone/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - USB_Host/FWupgrade_Standalone/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - USB_Host/FWupgrade_Standalone/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - USB_Host/FWupgrade_Standalone/CM4/Src/main.c                  Main program  for Cortex-M4

  - USB_Host/FWupgrade_Standalone/Binary/
    This folder contains images that can be loaded and executed by the FW upgrade application:
    - STM32H747I_EVAL_SysTick_0x08020000.bin


@par Hardware and Software environment

  - This application runs on STM32H7xx devices.

  - This application has been tested with STMicroelectronics STM32H747I-EVAL 
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32H747I-EVAL  Set-up
    - Plug the USB key into the STM32H747I-EVAL board through 'USB micro A-Male
      to A-Female' cable to the connector:
      - CN14 : to use USB High Speed (HS)
      - CN18: to use USB Full Speed (FS)
        @note Make sure that :
         - jumper JP2 must be removed when using USB OTG FS


@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain.
 - For CM4 target configuration (STM32H747I-Discovery_CM4) :
     - Rebuild all files
     - Load image into target memory

 - For CM7 target configuration :
 - In the workspace toolbar select the project configuration:
   - STM32H747I-EVAL_USBH-HS: to configure the project for STM32H7xx devices using USB OTG HS peripheral
   - STM32H747I-EVAL_USBH-FS: to configure the project for STM32H7xx devices using USB OTG FS peripheral
 - To run the application, proceed as follows:

1. Load the binary image of the user program to the root directory of a USB key. You can use the
   provided binary images under the USB_Host/FWupgrade_Standalone/Binary folder.
   The binary should be renamed to “image.bin”.

2. Program the firmware upgrade application into the internal Flash memory.
   a. Open the project (under USB_Host/FWupgrade_Standalone) with your preferred toolchain.
   b. Compile and load the project into the target memory and run the project.

After the board reset and depending on the Tamper button state:
   1. Tamper button pressed: The firmware upgrade application is executed.
   2. Tamper button not pressed: A test on the user application start address will be performed and one of
      the below processes is executed:
      – User vector table available: User application is executed.
      – User vector table not available: firmware upgrade application is executed.

During the firmware upgrade application execution, there is a continuous check on the Tamper button pressed
state time. Depending on the state time of the Tamper button, one of the following processes is executed:
 - If Tamper Button is pressed for more than > 3 seconds at firmware startup:
   UPLOAD command will be executed immediately after completed execution of the DOWNLOAD command
 - If Tamper Button is pressed for less than< 3 seconds at firmware startup:
   Only the DOWNLOAD command is executed.

STM32 Eval board's LEDs can be used to monitor the application status:
 - Red LED blinks in infinite loop
	 –> Error: USB key disconnected.

 - Red LED blinks in infinite loop and Orange/Blue LEDs are ON
	 –> Error: Flash programming error.

 - Red LED blinks in infinite loop and Green LED is ON
	 –> Error: Download done and USB key disconnected.

 - Red LED blinks in infinite loop and Blue/Orange/Green LEDs are ON
	 –> Error: Binary file not available

 - Red/Blue/Orange LEDs blink in infinite loop
	 –> Error: Buffer size limit, Exceed 32Kbyte

 - Red LED blinks in infinite loop and Blue LED is ON
	 –> Error: No available Flash memory size to load the binary file.

 - Red/Orange LEDs blink in infinite loop
	 –> Error: Flash erase error.

 - Red/Blue LEDs are ON
	 –> UPLOAD condition verified and the user should release the Tamper button.

 - Blue LED is ON
	 –> DOWNLOAD ongoing.

 - Orange/Red LEDs are ON
	 –> DOWNLOAD done and UPLOAD is ongoing.

 - Orange/Blue LEDs are ON
	 –> UPLOAD is ongoing.

 - Red LED blinks in infinite loop and Orange LED is ON
	 –> USB key read out protection ON.

 - Green LED is ON
	 –> DOWNLOAD and UPLOAD done with success; and the MCU waiting until you press the Tamper button before
      executing the JUMP command.


 */
