/**
  @page SPI_FullDuplex_ComPolling SPI Full Duplex Polling example

  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComPolling/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex Polling example.
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

Data buffer transmission/reception between two boards via SPI using Polling mode.

Board: NUCLEO-H723ZG (embeds a STM32H723ZGT6 device)
CLK Pin: PB3 (CN07.D23)
MISO Pin: PB4 (CN7.D25)
MOSI Pin: PB5 (CN07.D22)

   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          |SPI1          |                      |          SPI1|          |
  |          |              |                      |              |          |
  |          |           CLK|______________________|CLK           |          |
  |          |              |                      |              |          |
  |          |          MISO|______________________|MISO          |          |
  |          |              |                      |              |          |
  |          |          MOSI|______________________|MOSI          |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |                      GND|______________________|GND                      |
  |_STM32_Board 1___________|                      |_STM32_Board 2___________|

HAL architecture allows user to easily change code to move to IT or DMA mode.
To see others communication modes please check following examples:
SPI\SPI_FullDuplex_ComDMA
SPI\SPI_FullDuplex_ComIT

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK is scheduled at 520 MHz/2 and used for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals.
The APB clock dividers are scheduled at 520 MHz/4 and used for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals and D3 Domain APB4 peripherals.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK &
GPIO). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive() function allows the reception and the
transmission of a predefined data buffer at the same time (Full Duplex Mode)
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive(), at the same time SPI Slave transmits aTxBuffer
and receives aRxBuffer through HAL_SPI_TransmitReceive().
The end of this step is monitored through the HAL_SPI_GetState() function
result.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

@note Timeout is set to 5 seconds which means that if no communication occurs during 5 seconds,
      a timeout error is generated.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 toggles quickly on master board waiting User push-button to be pressed.
 - LED1 turns ON when the transmission process is complete.
 - LED2 turns ON when the reception process is complete.
 - LED3 turns ON when there is an error in transmission/reception process.
 - LED3 toggles when there is a timeout error in transmission/reception process.

@note SPIx used instance and associated resources can be updated in "main.h"
      file depending on used hardware configuration.

@note You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
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

Connectivity, SPI, Full-duplex, Polling, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents

  - SPI/SPI_FullDuplex_ComPolling/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_ComPolling/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComPolling/Inc/main.h                  Header for main.c module
  - SPI/SPI_FullDuplex_ComPolling/Src/stm32h7xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_ComPolling/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_ComPolling/Src/system_stm32h7xx.c      stm32h7xx system source file
  - SPI/SPI_FullDuplex_ComPolling/Src/stm32h7xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32H723xx devices.

  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H723ZG Set-up
    - Connect Master board SCK pin (PB3) to Slave Board SCK pin (PB3) (connector CN07.D23)
    - Connect Master board MISO pin (PB4) to Slave Board MISO pin (PB4) (connector CN7.D25)
    - Connect Master board MOSI pin (PB5) to Slave Board MOSI pin (PB5) (connector CN07.D22)
    - Connect Master board GND  to Slave Board GND

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
