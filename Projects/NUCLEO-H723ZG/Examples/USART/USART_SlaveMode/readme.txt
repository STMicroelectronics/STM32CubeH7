/**
  @page USART_SlaveMode USART Slave Mode example

  @verbatim
  ******************************************************************************
  * @file    USART/USART_SlaveMode/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USART Slave Mode example.
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

This example describes an USART-SPI communication (transmit/receive) between two
boards where the USART is configured as a slave.

Boards: NUCLEO-H723ZG (embeds a STM32H723xx device)

   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          |     SPI1     |                      |    USART2    |          |
  |          |              |                      |              |          |
  |          |PB03(CN07.D23)|SCK_________________CK|(CN09.D51)PD07|          |
  |          |              |                      |              |          |
  |          |PB04(CN7.D25) |MISO________________TX|(CN09.D53)PD05|          |
  |          |              |                      |              |          |
  |          |PB05(CN07.D22)|MOSI________________RX|(CN09.D52)PD06|          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_BOARD_Master____________|                      |_BOARD_Slave_____________|


Two identical boards are connected as shown on the picture above.
Board 1: SPI Master board
Board 2: USART Slave board

In a first step, after the user press on the User push-button, Master (SPI) starts
the communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive(), at the same time Slave (USART) transmits aTxBuffer
and receives aRxBuffer through HAL_USART_TransmitReceive().
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

WARNING: as both boards do not behave the same way, "SLAVE_BOARD" compilation
switch is defined in Src/main.c and must be enabled at compilation time before
loading the executable in the USART slave board.
The SPI master board needs to be loaded with an executable software obtained
with SLAVE_BOARD disabled.

NUCLEO-H723ZG board LEDs are used to monitor the transfer status:
- While board 1 is waiting for the user to press the User push-button, its
  LED2 is blinking rapidly (100 ms period).
- When the transmission/reception processes starts, LED2 is turned off.
- When the test passes, LED1 on both boards is turned on, otherwise the
  test has failed.
- If there is an initialization or transfer error, LED3 is turned on.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

@note USARTx/SPIx instances used and associated resources can be updated in
"main.h" file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use
      HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@note It is highly recommended to set the SPI parameter "MasterKeepIOState" to "SPI_MASTER_KEEP_IO_STATE_ENABLE"
      at init time. This configuration allows to avoid glitches on SPI lines especially the clock.
      Glitches that could lead to Master/Slave desynchronization. This config allows SPI Master to keep
      control on output state even if it is disabled.


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

Connectivity, USART, SPI, Full-duplex, Master, Slave Transmission, Reception

@par Directory contents

  - USART/USART_SlaveMode/Inc/stm32h7xx_hal_conf.h HAL configuration file
  - USART/USART_SlaveMode/Inc/stm32h7xx_it.h       interrupt handlers header file
  - USART/USART_SlaveMode/Inc/main.h               Header for main.c module
  - USART/USART_SlaveMode/Inc/stm32h7xx_nucleo_conf.h  STM32H7xx_Nucleo board configuration file
  - USART/USART_SlaveMode/Src/stm32h7xx_it.c       interrupt handlers
  - USART/USART_SlaveMode/Src/main.c               Main program
  - USART/USART_SlaveMode/Src/stm32h7xx_hal_msp.c  HAL MSP module
  - USART/USART_SlaveMode/Src/system_stm32h7xx.c   STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.

  - This example has been tested with two NUCLEO-H723ZG boards embedding
    a NUCLEO-H723ZG device and can be easily tailored to any other supported device
    and development board.

    - NUCLEO-H723ZG set-up
    - Connect Master board PB03(CN07.D23 PIN15) to Slave Board (CN09.D51 PIN2)PD07
    - Connect Master board PB04(CN7.D25 PIN19) to Slave Board (CN09.D53 PIN4)PD05
    - Connect Master board PB05(CN07.D22 PIN13) to Slave Board (CN09.D52 PIN3)PD06
    - Connect Master board GND to Slave Board GND

@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory :
    - Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
    - Comment "#define SLAVE_BOARD" and load the project in Master Board
  - Run the example


 */
