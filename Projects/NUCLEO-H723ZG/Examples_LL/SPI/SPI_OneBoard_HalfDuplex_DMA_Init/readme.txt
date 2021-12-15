/**
  @page SPI_OneBoard_HalfDuplex_DMA_Init SPI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_OneBoard_HalfDuplex_DMA_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_OneBoard_HalfDuplex_DMA_Init example.
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

Configuration of GPIO and SPI peripherals to transmit 
bytes from an SPI Master device to an SPI Slave device in DMA mode. This example
is based on the STM32H7xx SPI LL API. The peripheral initialization uses the 
LL initialization function to demonstrate LL init usage.

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

This example works with only one NUCLEO-H723ZG.

SPI1 Peripheral is configured in Master mode Half-Duplex Tx.
SPI6 Peripheral is configured in Slave mode Half-Duplex Rx.
GPIO associated to User push-button is linked with EXTI. 

Example execution:

LED1 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD start a Half-Duplex communication through DMA.
On MASTER side (SPI1), Clock will be generated on SCK line, Transmission done on MOSI Line.
On SLAVE side (SPI6) reception is done through the MISO Line.

LED1 is On if data is well received.

In case of errors, LED1 is blinking Slowly (1s).

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

Connectivity, SPI, Half-duplex, DMA, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents 

  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/stm32h7xx_it.c          Interrupt handlers
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/main.c                  Main program
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H723xx devices.

  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H723ZG Set-up
    - Connect Master SCK  PB3 to Slave SCK  PA5
    - Connect Master MOSI PB5 to Slave MISO PA6

Relation with Board connector:
  PB3 is connected to pin 15 of CN7 connector
  PB5 is connected to pin 13 of CN7 connector
  PA5 is connected to pin 10 of CN7 connector
  PA6 is connected to pin 12 of CN7 connector

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
