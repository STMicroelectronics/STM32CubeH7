/**
  @page HID-CM4_MSC-CM7  Dual USBD instances using USBD FS on CM4 and USBD HS on CM7
  
  @verbatim
  ******************************************************************************
  * @file    Applications/USB_Device/HID-CM4_MSC-CM7
  * @author  MCD Application Team
  * @brief   Description of the HID-CM4_MSC-CM7 application.
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

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to use
USB device application based on the STM32H7xx multi core support feature integrating the Mass Storage Class (MSC)
and Human Interface (HID) in the same project.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

 
In this example:

  - The Cortex-M7 configures the USB OTG HS peripheral as device to communicate with a PC Host using the Bulk Only Transfer (BOT) 
    and Small Computer System Interface (SCSI) transparent commands. 
    The microSD card, plugged on the board connector CN13 is used as storage media. 
    The STM32 MCU is enumerated as a MSC device using the 
    native PC Host MSC driver to which the STM32H747I-EVAL board is connected.
    
    
  - The Cortex-M4 configures the USB OTG FS peripheral as device, where the STM32 MCU is
    enumerated as a HID device using the native PC Host HID driver to which the STM32H747I_EVAL
    board is connected, in order to emulate the Mouse directions using Tamper button mounted on the board. 

When the application is started, the user has to :

- Connect a USB cable bteween the PC and the eval board USB-HS (CN14).
  Connect a uSD to the board connector CN13. 
  The HS device is then automatically detected by the PC as a mass storage media. 
  A new removable drive appears in the system window and write/read/format operations can be performed 
  as with any other removable drive.
  
- Connect a USB cable bteween the PC and the eval board USB-FS (CN18).
  The FS device is then automatically detected by the PC as a HID (mouse).
  Press the eval board Tamper button to move the PC mouse.  
- LED 2 and LED4 are on to indicate that the example work correctly .  
The 48 MHz clock for the USB FS PHY is derived from the PLL3 clock (clocked by the HSE).
This configuration is done by the Cortex-M7 in the function "SystemClock_Config"


@note For Cortex-M7, Some code parts (for instance ISR routines, vector table, critical routines )
      can be executed from the ITCM-RAM (64 KB) allowing zero wait state access.
      Which decreases critical task execution time compared to code execution from internal Flash memory. 
      This configuration can be done using '#pragma location = ".itcmram"' to be placed above function declaration, 
      or using the toolchain GUI (file options) to execute a whole source file in the ITCM-RAM.
      For fast data access, critical variables, application RAM, heap, stack could be put as well in the DTCM-RAM (128 KB).

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"
 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, USB_Device, USB, SDMMC, HID, Full Speed, FS, High Speed, HS, Mouse, Button, Tamper, Button, Bulk Only Transfer, BOT, SCSI, microSD, SD, MSC, Storage

@par Directory contents 

  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/Common/Src/system_stm32h7xx.c   STM32H7xx system configuration file
  
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Src/main.c                  Main program                
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Src/stm32h7xx_it.c          Interrupt handlers
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Src/usbd_conf.c             General low level driver configuration
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Src/usbd_desc.c             USB device MSC descriptor
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Src/ubsd_storage.c          Media Interface Layer
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Inc/main.h                  Main program header file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Inc/usbd_conf.h             USB device driver Configuration file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Inc/usbd_desc.h             USB device MSC descriptor header file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM7/Inc/ubsd_storage.h          Media Interface Layer header file

  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Src/main.c                  Main program          
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Src/stm32h7xx_it.c          Interrupt handlers
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Src/usbd_conf.c             General low level driver configuration
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Src/usbd_desc.c             USB device HID descriptor
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Inc/main.h                  Main program header file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Inc/usbd_conf.h             USB device driver Configuration file
  - Applications/USB_Device/HID_MSC_Standalone_CoreSync/CM4/Inc/usbd_desc.h             USB device HID descriptor header file
    

  
@par Hardware and Software environment  

  - This example runs on STM32H747xx devices.
    
  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.

  - STM32H747I_EVAL Set-up
    - Connect the STM32H747I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector CN14 : to use USB High Speed (HS) 

    - Connect the STM32H747I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector CN18 : to use USB Full Speed (FS) 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747_EVAL_CM7_MSC-HS and STM32H747_EVAL_CM4_HID-FS) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example




 */
