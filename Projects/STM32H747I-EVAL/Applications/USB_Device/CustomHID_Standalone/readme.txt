/**
  @page CustomHID_Standalone USB Device Custom HID application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Custom HID application.
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
use USB device application based on the Custom HID Class on the STM32H747xx devices.

This is a typical application on how to use the STM32H747xx USB OTG Device peripheral, where the STM32 MCU is 
enumerated as a HID compliant device using the native PC Host HID driver to which the STM32H747I_EVAL
board is connected.

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
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS is derived from the PLL3 clock (clocked by the HSE)

To test the application, user must proceed as follows:
 - Start the "USB HID Demonstrator" PC applet, available for download from www.st.com, 
   and connect STM32 USB to PC
 - The device should be detected and shown in the USB HID target box
 - Press Graphic View button
 - Select "use SET_FEATURE" or "use SET_REPORT" in order to use SET_REPORT request to send HID Report
   for LED control
 - Use the potentiometer of the STM32H747I_EVAL board to transfer the result of the converted voltage
  (via the ADC) the to the PC host (these values are sent to the PC using the endpoint1 IN)   
 - Make sure that following report ID are configured: LED1 ID (0x1), LED2 ID(0x2), LED3 ID(0x3),
   LED4 ID(0x4), BUTTON1_ID(0x6) and Potentiometer_ID(0x7).
 - Select LEDs to switch on/off on the STM32H747I_EVAL board: a SET_REPORT request will be sent

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

Connectivity, USB_Device, USB, HID, Full Speed, High Speed, Joystick, Mouse, OTG

@par Directory contents

  - USB_Device/CustomHID_Standalone/CM7/Src/main.c                  Main program
  - USB_Device/CustomHID_Standalone/CM7/Src/stm32h7xx_it.c          Interrupt handlers
  - USB_Device/CustomHID_Standalone/CM7/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - USB_Device/CustomHID_Standalone/CM7/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/CustomHID_Standalone/CM7/Src/usbd_desc.c             USB device descriptor
  - USB_Device/CustomHID_Standalone/CM7/Inc/main.h                  Main program header file
  - USB_Device/CustomHID_Standalone/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - USB_Device/CustomHID_Standalone/CM7/Inc/stm32hxx_hal_conf.h     HAL configuration file
  - USB_Device/CustomHID_Standalone/CM7/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/CustomHID_Standalone/CM7/Inc/usbd_desc.h             USB device descriptor header file

  - USB_Device/MSC_Standalone/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - USB_Device/MSC_Standalone/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - USB_Device/MSC_Standalone/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - USB_Device/MSC_Standalone/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - USB_Device/MSC_Standalone/CM4/Src/main.c                  Main program  for Cortex-M4
  
@par Hardware and Software environment

  - This application runs on STM32H747xx devices.
    
  - This application has been tested with STMicroelectronics STM32H747I-EVAL  
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32H747I-EVAL  Set-up
    - Connect the STM32H747I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN14 : to use USB High Speed (HS) 
      - CN18: to use USB Full Speed (FS)
              Please ensure that jumper JP2 is not fitted.
		   
@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
 - For CM4 target configuration (STM32H747I-EVAL_CM4) : 
     - Rebuild all files 
     - Load image into target memory
	 
 - For CM7 target configuration  :
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32H747I-EVAL_USBH-HS: to configure the project for STM32H747xx devices using USB OTG HS peripheral
   - STM32H747I-EVAL_USBH-FS: to configure the project for STM32H747xx devices using USB OTG FS peripheral
 - Run the application
 

 */
 
