/**
  @page CRC_PolynomialUpdate CRC operation with Polynomial update (HAL/LL mixed usage example)

  @verbatim
  ******************************************************************************
  * @file    CRC/CRC_PolynomialUpdate/readme.txt
  * @author  MCD Application Team
  * @brief   Description of CRC operation including user updates on generating polynomial.
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

How to use the CRC peripheral through the STM32H7xx CRC HAL and LL API.
The LL API is used for performance improvement. The CRC calculation unit
computes the 8-bit CRC code of a given buffer of 32-bit data words, based on
a user-defined generating polynomial. In this example, the polynomial is first
set manually to 0x9B (X^8 + X^7 + X^4 + X^3 + X + 1).
In a second step, the generating polynomial value and length are updated and
set to 0x1021  (X^16 + X^12 + X^5 + 1) for a new CRC calculation.
These updates are performed using the CRC LL API.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 520 MHz.

The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK).
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

For each computation, calculated CRC code is stored in uwCRCValue variable.
Once calculated, the CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue1 and uwExpectedCRCValue2).

STM32 board LEDs are used to monitor the example status:
  - LED1 is ON when correct CRC values are calculated
  - LED3 is ON when there is an error in initialization or if an incorrect CRC value is calculated.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
 to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC, user-defined, generating polynomial, polynomial update

@par Directory contents

  - CRC/CRC_PolynomialUpdate/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRC/CRC_PolynomialUpdate/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRC/CRC_PolynomialUpdate/Inc/main.h                  Header for main.c module
  - CRC/CRC_PolynomialUpdate/Inc/stm32h7xx_nucleo_conf.h    HAL configuration file
  - CRC/CRC_PolynomialUpdate/Src/stm32h7xx_it.c          Interrupt handlers
  - CRC/CRC_PolynomialUpdate/Src/main.c                  Main program
  - CRC/CRC_PolynomialUpdate/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - CRC/CRC_PolynomialUpdate/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */

