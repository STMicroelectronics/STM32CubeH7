/**
  @page Demonstrations   STM32H745-Discovery Demonstration Firmware

  @verbatim
  ******************************************************************************
  * @file    Demonstrations/STM32H745-Discovery_Demo/readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32H745-Discovery Demonstration
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

@par Demo Description

The STM32Cube Demonstration platform is built around the powerful analog peripherals
and uses almost STM32 capabilities to offer a large scope of usage based on
the STM32Cube HAL.


@par Demonstration Overview

The STM32H7 demonstration is running on the STM32H745I Dual Core Discovery board,
and aims to highlight the Dual Core aspects and the analog features of the STM32H745I Dual Core device.

The Demonstration is based on a main menu that can be used to switch between:
    - Oscilloscope/Signals Generator application,
    - EEMBC’s CoreMark® application(provided in binary format)
    - system information display.

System Information menu:
=========================
Shows system information details: Board Name, used device part number, CPUs speed and
demonstration firmware version.

EEMBC’s CoreMark® Application:
==============================
EEMBC’s CoreMark® is a benchmark that measures the performance of the microcontroller (MCUs)
and central processing units (CPUs) used in embedded systems.
This application shows Cortex-M7 and Cortex-M4 scores based on this benchmark.

Oscilloscope/Signals Generator Application:
===========================================
Demonstrates dual cores and analog features capabilities. Each core is configured to handle a
specific analog application:

 - The Cortex-M7 Core: is used to handle the Signal Generator application (SG).
 - The Cortex-M4 Core: is used to handle the Digital Signal Oscilloscope (DSO).

When the Digital Signal Oscilloscope (DSO) is used with an external Signal Generator,
the Signal Generator application (SG) can be shut down by pressing the User Button in order
to put the Cortex-M7 domain (D1 domain) in STANDBY mode.
Consequently, only the Cortex-M4 domain (D2 domain) keep running the oscilloscope application.

++ Digital Signal Oscilloscope (Running on Cortex-M4 Core )
 + Signal conversion process
 Today's oscilloscopes are mainly classified according to their maximum conversion
 frequency which depends on their Analog to Digital Converters (ADC).
 STM32H7 devices include three ADCs, able to work separately or in interleaved mode.
 In this application ADC is configured as follow:
    - Sampling time: 1.5 ADC's clock
    - ADC Clock: 36 MHz through an external clock source provided by PLL2.
    - SAR Time: 7.5 ADC's Clock with 14 bits conversion resolution.

 * Timing configuration:
 The minimum conversion time is (7.5+1.5) ADC period = 9 * 1/36MHz = 0.250 µs.
 The maximum conversion frequency is 1/0.250µs = 4 M Samples/s.
 TIM3 is used to continuously trigger the ADC.

 + Human Machine Interface
 The Digital Signal Oscilloscope (DSO) comes with a PC graphical user interface to display signals.
this GUI is available under .\PC_Software

 + Communication between the GUI and the STM32H7 is ensured by UART through ST-Link VCP.

@note The configured Baud rate on the oscilloscope is 115200 bit/s.
@note The Digital Signal Oscilloscope supports signals with a maximum 3.3 V amplitude.

++ Signals Generator Application (Running on Cortex-M7 Core )
 This application uses the DAC capabilities to generate different wave forms with configurable frequency
 and amplitude and displayed on the board LCD.

 + Signals Generations Configuration:
   The LCD User graphical interface is used to control the application configuration and shows the current output status:
    - Signal Type buttons: to select the type of output signal (DC,Square,Sinus,Triangle,Escalator or Noise).
    - Signal Frequency buttons: to configure the frequency of output signal (5.000Khz -> 130.000Khz).
    - Signal Amplitude buttons: to configure the amplitude of output signal (0.100 V -> 3.300 V).

 + Signals Generations Status:
    - Plots on the LCd the shape of the current output signal.
    - Shows the Frequency and the amplitude of  current output signal.

 + Standby mode:
    - User can put the Signals Generations Module in standby mode if not needed .
    - Standby mode entry and exit is controlled by pressing the USER button.

@par Keywords

Demonstration, multiprocessor, Oscilloscope, Analog, ADC,DAC, DMA, Timers, UART, RAM, signals generation.

@par Hardware and Software environment

  - This demonstration runs on STM32H745I devices.
  - This demonstration has been tested with STM32H745I-DISCO discovery board.

@note Note1
This demonstration is based on both cores(Cortex-M4 and Cortex-M7) use. It is based on the following boot scheme where
CPU1 (Cortex-M7) is booting and CPU2(Cortex-M4)clock is gated.(with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1(Cortex-M7) seen as the master CPU.


@note Note2
The execution scheme depends on the selected application from main menu:
- EEMBC’s CoreMark® application: both CPUs are executing from flash without contention(from different bank each).
- Oscilloscope/Signals Generator Application: CPU1(Cortex-M7) is executing from flash.
It copies the CPU2(Cortex-M4) code to the  D2 domain RAM and configures its boot address(using HAL_SYSCFG_Cortex-M4BootAddConfig).
Finally, CPU2 (Cortex-M4) will be released.


@par How to use it ?

In order to make the demonstration work, you must do the following :
  - Use STM32CubeProgrammer tool to disable the Cortex-M4 boot at FLASH Option bytes (Cortex-M4 is gated)
  - Open your preferred toolchain
  - Rebuild in order OSCILLO_SG_CM4,OSCILLO_SG_CM7 then DEMO_MENU project configurations
  - Load your applications images:
     - DEMO_MENU
     - OSCILLO_SG_CM7 image (This image includes Cortex-M7 and Cortex-M4 Code).
     - EEMBC’s CoreMark® application image located at "Binaries\STM32H7xx_EEMBC.hex"
  - Reset the Board so the main menu is displayed, you can select the desired application
  
The Full demonstration binary files are available from the STM32H745I-DISCO page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h745i-disco.html
Direct link:
https://www.st.com/resource/en/compiled_demos/stm32h745i-disco_demo.zip 

  - STM32CubeDemo_Prod_STM32H745I-DISCO_v1.0.0_FULL.hex --> Full ready to use STM32H745I-DISCO demonstration hex file
    including internal flash and external flash (QSPI) images
  - STM32CubeDemo_Prod_STM32H745I-DISCO_v1.0.0_InternalFlash.hex --> Internal flash only hex file
  - STM32CubeDemo_Prod_STM32H745I-DISCO_v1.0.0_ExternalFlash.hex --> External flash (QSPI) only hex file
  - STM32H7xx_EEMBC.hex --> EEMBC(R) Internal and external flash only hex file
  
  Flashing the STM32H745I-DISCO demonstration hex file
  - In order to flash the ready to use full demonstration hex file into the STM32H745I-DISCO
    - Run the STM32CubeProgrammer:
        - Disable the Cortex-M4 boot at FLASH Option bytes setting menu (Cortex-M4 is gated)
    	- Select the external flash loader (MT25TL01G_STM32H745I-DISCO) from Erasing and programming Menu 
		- From Erasing and programming Menu browse to select the STM32CubeDemo_Prod_STM32H745I-DISCO_v1.0.0_FULL.hex file
		  then click on start programming
  

For Oscilloscope/Signals Generator application,in order to use the auto-test mode :
  - Connect the Signal Generator Output (PA4, STMod+ Connector pin13) to Oscilloscope
    input (PA0,CN7 Connector pin A2).
  - a demo mode is invoked in case of several seconds without user action on the touch screen
   otherwise normal mode is running

On PC side, you must do the following :
  - Setup your Oscilloscope PC application (STM32H745-Discovery_Demo\PC_Software\setup.exe)
  - Run your Oscilloscope "Serial Oscilloscope"
  - Configure your UART communication in the GUI by selecting the Serial Port and Baud rate(115200),
  - Start the connection between the Board and the GUI
  - Configure the desired sampling rate frequency.
  - Click on Start Scope button.
    - make sure to Apply an external signal to the board input (PA0,CN7 Connector pin A2)..



 */
