/**
  @page Oscilloscope_SignalsGenerator  STM32H735G-DK Demonstration Firmware

  @verbatim
  ******************************************************************************
  * @file    Demonstrations/Oscilloscope_SignalsGenerator/readme.txt
  * @author  MCD Application Team
  * @brief   Description of STM32H735G-DK Demonstration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Demo Description


The STM32Cube Demonstration platform is built around the powerful analogs peripherals
and uses almost the whole STM32 capability to offer a large scope of usage based on
the STM32Cube HAL.

@par Demonstration Overview

The STM32H7 demonstration is running on the STM32H735G Discovery board.
The demonstration was created in purpose to highlight the analog features
of the STM32H735G chip.

To describe and to demonstrate those purposes , two RTOS tasks were configured to handle a
specific analog application:

 - The Task 1 : is used to handle the Signal Generator application (SG).
 - The Task 2 : is used to handle the Digital Signal Oscilloscope (DSO).

The Signal Generator application can be discarded when the user want to use only the
Digital Signal Oscilloscope with an external Signal Generator. 


++ Oscilloscope Application (Task 1 )
 + Signal conversion process
 --------------------------
 The oscilloscopes of today are mainly classified according to their maximum conversion
 frequency which depends basically on their powerful Analog to Digital Converters (ADC).
 STM32H7 include three ADCs, able to work separately or in interleaved mode depending on our
 purpose.
 In purpose to prouve their performaces to achieve the best oscilloscope results, four
 improuvments have been implemented with success:

      1. In purpose to reach up the maximum possible conversion frequency, three specific factors
      have been considered (Sampling time, Register of Successive Approximation (SAR) Time, ADC's Clock).

 In this example to reach up the maximum ADC conversion frequency, Sampling time was configured
 as minimum (1.5 ADC's clock) and ADC's Clock was configured as maximum possible (80MHz/2=40 MHz) throw
 an external clock source provided by PLL2. SAR Time was selected as 7.5 ADC's Clock referring
 to the conversion resolution which is fixed on 14 bits to ensure a better conversion quality.
  * Timing configuration:
  In this demonstration, the system clock is 520 MHz,ADC clock source is configured as external throw PLL2:
          * Sampling Time = 1.5 ADC Clock
          * Sar Time = 7.5 ADC Clock
          * ADC Clock = 40 MHz
 The minimum conversion time is (7.5+1.5) ADC period = 9 * 1/40MHz = 0.225 µS.
 The maximum conversion frequency is 1/0.225µS = 4.44 M Samples/s.

     2. In purpose to save the maximum possible of memory where samples are stocked specially for
     slow frequency. An algorithm based on Timer triggering was implemented to modify from the HMI
     the conversion frequency in the range of [500 Samples/S, 4 M Samples/S].

 TIM3, as a General-purpose 16 bits Timers was configured to make a hardware continuous triggering to
 our ADC, the counting cycle from 0 to 65 536 finish with a hardware configured interrupt called
 update IT restarting the conversion cycle.
 The counting frequency, which depends on the two factors (Prescalar and Period) is the responsible
 to modify the conversion rate.

     3. In purpose to improuve the maximum conversion rate from 4 M Samples/S to 8 M Samples/S,
     an Interleaved conversion mode configured by ADC1 and ADC2 was made with success.

 ADC1 configured on the best possible performance (4 M Samlpes/S) is gone to convert the first part of
 the signal and trig the second slave ADC (ADC2) to finish the conversion. This mechanism which use a
 specific timing is described on ADC 8 M Samples/S conversion example.


 + Data manipulation process
 ---------------------------
 In order to minimize the power consumption of our DSO, Data manipulation from ADC's to the RAM was
 ensured by Direct Memory Access (DMA). Two different methods of data management are created depending
 on the conversion mode.
      1. Single Mode : Direct transfer data using a circular 16 bits DMA.
      2. Dual Mode   : Master 16 bits results are stocked in the 16 MSB bits of the 32 common Data register CDR
                       and wait for the slave ADC to provide the other 16 bits. the moment when the register is
                       filled, the data are transferred using a circular 32 bits DMA mode.

 + Humain Machine Interface
 --------------------------
 A DSO need A graphical unit interface to display signals, A GUI made using Visual Studio with C# are created to
display continuously signals coming from the MCU.


 + Communication between the GUI and the MCU
 -------------------------------------------
 In order the make a full duplex communication between GUI and MCU, UART communication protocol was made
 to transfer samples from the MCU to the GUI and receive from there the user command including the new configuration.

 + Signal Frequency detection
 -------------------------------------------
 In purpose to caracterise the externals signals displayed in the digital signal oscilloscope, A simple mechanism based
 on the time between samples information was made on the GUI to detect the frequency of the signal.

    - Signal Period = Number of samples per period * time between two successive samples.
    - Signal Frequency = 1 / Signal Period.

@note The configured Baude rate on the oscilloscope is 115200 bit/s.
@note Only signals with amplitude less than 3.3 V have to put to the input.

++ Signals Generator Application (Task 2 )
 This Task t uses the DAC capabilities  to generate different waves forms 
 with the possibility to configure the frequency
 and the amplitude of the generated signal.

 + Signals Generations Configuration:
   A User graphical inaterface is used to control the application configuration
   and shows the current output status:
    - Signal Type buttons: to select the type of output signal (DC,Square,Sinus,Triangle,Escalator or Noise).
    - Signal Frequency buttons: to configure the frequency of output signal (5.000Khz -> 130.000Khz).
    - Signal Amplitude buttons: to configure the amplitude of output signal (0.100 V -> 3.300 V).

 + Signals Generations Status:
    - Shows the graphic of  current output signal.
    - Shows the Frequency and the amplitude of  current output signal.


@note The STM32H735 devices can reach a maximum clock frequency of 520 MHz.

@par Keywords

Demonstration, Oscilloscope, Analog, ADC,DAC, Firmware Cube, DMA, Timers, UART, GUI, RAM.

@par Hardware and Software environment

  - This demonstration runs on STM32H735G devices.
  - This demonstration has been tested with STM32H735G-Discovery RevA discovery board.


@par How to use it ?


In order to make the demonstration work, you must do the following :
  - Open your preferred toolchain
  - Rebuild STMH735G_DISCO project configuration.
  - Load your  STMH735G_DISCO image.

In order to use the auto-test mode :
  - Connect the Signal Generator Output (PA5, STMod+ Connector pin13) to Oscilloscope
    input (PA0,CN9 Connector pin A2).

On PC side, you must do the following :
  - Setup your Oscilloscope PC application (\Utilities\PC_Software\Oscilloscope\setup.exe)
  - Run the "Serial Oscilloscope" application on your PC
  - Configure your UART communication in the GUI by selecting the Serial Port and Baude rate(115200),
  - Connect a (Type A to Micro-B) USB cable from the STM32H735G-DK board (Connector CN15) to the PC 
  - Start the connection between the Board and the PC GUI using the dedicated PC GUI button 
  - Configure the desired sampling rate frequency.
  - Put an external signal with amplitude less than 3.3 V to the input (PA0,CN9 Connector pin A2 of the STM32H735G-DK board)..
  - Click on Start Scope button.

  In order to detect the signal frequency, you must do the following
  - Select a scope view presenting a full period on your screen,
  - Freeze the signal by clicking on Trig button on the screen,
  - Count the number of samples per period,
  - Put the number of samples counted in the GUI TextBox and click on compute.

The Full demonstration binary files are available from the STM32H735G-DK page in the Binary Delivery section.
Please refer to : www.st.com/en/product/stm32h735g-dk.html
Direct link: 
https://www.st.com/resource/en/compiled_demos/stm32h735g-dk_demo.zip

This Demo MenuLauncher must be programmed into the oard first to allow running this sub-demo

In order program this sub-demo into the STM32H735G-DK board :
 - Open STM32CubeProgrammer and click on connect
 - Go to external loaders menu then select MX25LM51245G_STM32H735G-DK to program external OSPI memory
 - From Erasing and programming Menu browse to select the hex file from \Binary folder then click on start programming
  
 Or you can also use the script "program_hexfile.bat" (available within the binaries package) to directly program 
 this hex file into the STM32H735G-DK board.
 This script will invoke the STM32CubeProgrammer with the correct OSPI flasher (MX25LM51245G_STM32H735G-DK)
 

 */
