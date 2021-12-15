/**
  @page I2C_OneBoard_Communication_IT I2C example (IT Mode)
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_Communication_IT I2C example (IT Mode).
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

@par Example Description

How to handle the reception of one data byte from an I2C slave device 
by an I2C master device. Both devices operate in interrupt mode. The peripheral is initialized 
with LL unitary service functions to  optimize for performance and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using only one NUCLEO-H723ZG.

The user can disable internal pull-up through "#define EXTERNAL_PULL_UP_AVAILABLE"
This help for an integration of this example inside an ecosystem board with external pull-up */

I2C1 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C4 Peripheral is configured in Master mode with EXTI (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI. 

LED1 blinks quickly to wait for user-button press. 

Example execution:
Press the User push-button to initiate a read request by Master.
This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXE interrupt occurs.
When byte is received on I2C4, an RXNE interrupt occurs.
When RXDR register is read, Master auto-generate a NACK and STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C1 IRQ handler routine by a clear of NACK flag.
The STOP condition generate a STOP interrupt in both side (Slave and Master). I2C1 and I2C4 IRQ handler routine are then
clearing the STOP flag in both side.

LED1 is On if data is well received.

In case of errors, LED1 is blinking.


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Connectivity, Communication, I2C, IT , Master, Slave, Transmission, Reception, Fast mode,

@par Directory contents 

  - I2C/I2C_OneBoard_Communication_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_Communication_IT/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_Communication_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_Communication_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_Communication_IT/Src/main.c                  Main program
  - I2C/I2C_OneBoard_Communication_IT/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H723ZG Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.8 and PB.9)
    to respectively SCL and SDA pins of I2C4 (PF.14 and PF.15).
      - I2C1_SCL  PB.8 (CN7, pin 2) : connected to I2C4_SCL PF.14 (CN12, pin 50)
      - I2C1_SDA  PB.9 (CN7, pin 4) : connected to I2C4_SDA PF.15 (CN12, pin 60)

  - Launch the program. Press User push-button to initiate a read request by Master 
      then Slave send a byte.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
