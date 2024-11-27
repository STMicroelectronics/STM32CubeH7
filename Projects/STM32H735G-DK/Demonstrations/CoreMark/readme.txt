/**
  @page CoreMark STM32H735G-DK EEMBC(R) CoreMark(R) Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of STM32H735G-DK EEMBC(R) CoreMark(R) Demonstration
  ******************************************************************************
  @endverbatim

@par Demo Description

Cortex-M7 EEMBC® Coremark1.0 on the fly computation 
A simple touch on the screen will start the benchmark and display the score on real STM32H735 device giving: ~2777 Coremarks

Algorithm implemented with preconditions:
CPU frequency: CM7 @ 550MHZ
Code : Flash Bank1
RW data : DTCM
CM7 L1-Cache ON 
Number of iterations : CM7 35000 
Compiler: IAR embedded workbench for ARM 7.80

@par Keywords

Demonstration, FreeRTOS, RTOS, Graphics, TouchGFX, Core Mark, EMMBC

This demonstration is provided as binary only (no source code)

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
 
