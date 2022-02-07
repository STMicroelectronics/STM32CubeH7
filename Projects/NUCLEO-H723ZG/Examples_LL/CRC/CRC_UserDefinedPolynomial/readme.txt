/**
  @page CRC_UserDefinedPolynomial CRC : 8-bit CRC calculation using user-defined polynomial value
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/CRC/CRC_UserDefinedPolynomial/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRC_UserDefinedPolynomial example.
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

How to configure and use the CRC calculation unit to compute an 8-bit CRC code
for a given data buffer, based on a user-defined generating polynomial. The
peripheral initialization is done using LL unitary service functions for
optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

The CRC peripheral is configured to work with a user defined polynomial value (8-bit long value).
In this example, the polynomial is set manually to 0x9B that is X^8 + X^7 + X^4 + X^3 + X + 1.
Generated CRC value is then 8-bit long.

Example execution:
After startup from reset and system configuration, CRC configuration is performed (use of user-defined Polynomial value).
CRC code of a given data buffer is computed. 
Data buffer length has been chosen as not an exact nb of u32 (32-bit words), in order to illustrate 
use of offered API for feeding the calculator (u32, u16 or u8 inputs).
The calculated CRC code is stored in ubCRCValue variable.
Once calculated, CRC value (ubCRCValue) is compared to the CRC expected value (ubExpectedCRCValue),
and if both are equal, LED1 is turned On. 
In case of errors, LED1 is blinking (1sec period).

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC, user-defined, generating polynomial

@par Directory contents 

  - CRC/CRC_UserDefinedPolynomial/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRC/CRC_UserDefinedPolynomial/Inc/main.h                  Header for main.c module
  - CRC/CRC_UserDefinedPolynomial/Inc/stm32_assert.h          Template file to include assert_failed function
  - CRC/CRC_UserDefinedPolynomial/Src/stm32h7xx_it.c          Interrupt handlers
  - CRC/CRC_UserDefinedPolynomial/Src/main.c                  Main program
  - CRC/CRC_UserDefinedPolynomial/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
 
