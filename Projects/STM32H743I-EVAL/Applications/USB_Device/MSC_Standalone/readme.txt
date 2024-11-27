/**
  @page MSC_Standalone USB Device Mass Storage (MSC) application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/MSC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Device MSC application.
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
use USB device application based on the Mass Storage Class (MSC) on the STM32H7xx devices.

This is a typical application on how to use the STM32H7xx USB OTG Device peripheral to communicate with a PC
Host using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent commands, 
while the microSD card is used as storage media. The STM32 MCU is enumerated as a MSC device using the 
native PC Host MSC driver to which the STM32H743I-EVAL board is connected.

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

When the application is started, the user has just to plug the USB cable into a PC host and the device 
is automatically detected. A new removable drive appears in the system window and write/read/format 
operations can be performed as with any other removable drive.

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

Connectivity, USB_Device, USB, SDMMC, MSC, Mass Storage, SD Card, Full Speed, High Speed, BOT, SCSI, Removable drive, Write, Read, Format

@par Directory contents

  - USB_Device/MSC_Standalone/Src/main.c                  Main program
  - USB_Device/MSC_Standalone/Src/system_stm32h7xx.c      STM32h7xx system clock configuration file
  - USB_Device/MSC_Standalone/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Device/MSC_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/MSC_Standalone/Src/usbd_desc.c             USB device MSC descriptor
  - USB_Device/MSC_Standalone/Src/ubsd_storage.c          Media Interface Layer
  - USB_Device/MSC_Standalone/Inc/main.h                  Main program header file
  - USB_Device/MSC_Standalone/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Device/MSC_Standalone/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - USB_Device/MSC_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/MSC_Standalone/Inc/usbd_desc.h             USB device MSC descriptor header file
  - USB_Device/MSC_Standalone/Inc/ubsd_storage.h          Media Interface Layer header file   

	
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
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32H743I-EVAL_USBH-FS: to configure the project for STM32H743xx devices using USB OTG FS peripheral
   - STM32H743I-EVAL_USBH-HS: to configure the project for STM32H743xx devices using USB OTG HS peripheral
 - Run the application
 

 */
 
