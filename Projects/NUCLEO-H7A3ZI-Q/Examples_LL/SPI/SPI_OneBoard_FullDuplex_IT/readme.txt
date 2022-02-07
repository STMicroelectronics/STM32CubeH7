/**
  @page SPI_OneBoard_FullDuplex_IT SPI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_OneBoard_FullDuplex_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_OneBoard_FullDuplex_IT example.
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

This example shows how to perform SPI data buffer transmission/reception between
two instances in the same board by using interruptions. This  example is based on
 the STM32H7xx SPI LL API. The peripheral initialization uses LL unitary service functions
 for optimization purposes (performance and size).


   _______________________________
  |        ____________________   |
  |       |     SPI1           |  |
  |       |                    |  |
  |       |PB5 : CN7.D22   MOSI|__|_____________
  |       |                    |  |             |
  |       |PB4 : CN7.D25   MISO|__|_______      |
  |       |                    |  |       |     |
  |       |PB3 : CN7.D23    SCK|__|__     |     |
  |       |                    |  |  |    |     |
  |       |____________________|  |  |    |     |
  | SPI Master                    |  |    |     |
  |                               |  |    |     |
  |                               |  |    |     |
  |                               |  |    |     |
  |                               |  |    |     |
  |        ____________________   |  |    |     |
  |       |     SPI6           |  |  |    |     |
  |       |                    |  |  |    |     |
  |       |PA5 : CN7.D13    SCK|__|__|    |     |
  |       |                    |  |       |     |
  |       |PA6 : CN7.D12   MISO|__|_______|     |
  |       |                    |  |             |
  |       |PA7 : CN7.D11   MOSI|__|_____________|
  |       |                    |  |
  |       |____________________|  |
  | SPI Slave                     |
  |                               |
  |_______________________________|

@note The connection between the pins should use a short wires.

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using IT mode through the STM32H7xx SPI LL API.
Configure_SPI1() function is called to configure and enable the SPI MASTER (SPI1) and also associated GPIO pins.
Configure_SPI6() function is called to configure and enable the SPI SLAVE (SPI6) and also associated GPIO pins.

The SPI communication is then initiated.
The LL_SPI_StartMasterTransfer() function allows the transmission and the reception
of a predefined data buffer at the same time (Full Duplex Mode).

For this example the aTxBuffer is predefined and the SPI1_RxBuffer, SPI6_RxBuffer size is same as aTxBuffer.

In a first step, SPI Master starts the communication by sending aTxBuffer and receiving
SPI1_RxBuffer. At the same time, SPI Slave transmits aTxBuffer and receives SPI1_RxBuffer.

The callback functions (SPI1_Tx_Callback, SPI1_Rx_Callback, SPI6_Tx_Callback, SPI6_Rx_Callback, SPI1_EOT_Callback,
SPI1_EOT_Callback and SPI_TransferError_Callback) update
the variables SPI1_ReceiveIndex, SPI1_TransmitIndex, SPI6_ReceiveIndex, SPI6_TransmitIndex and NbDataToTransmit
used in the main function to check the transfer status. Finally, aTxBuffer with SPI1_RxBuffer and aTxBuffer
 with SPI6_RxBuffer are respectively compared through Buffercmp() in order to check buffers correctness.

STM32 board's LED1 can be used to monitor the transfer status:
 - LED1 blinks Fast (200ms) waiting User push-button to be pressed to start the transfer.
 - LED1 turns ON when the transmission and reception process is complete with success.
 - LED1 blinks Slowly (1s) when there is an error in transmission/reception process.

@note SPI1 & SPI6 instanceS used and associated resources can be updated in "main.c"
      file depending on hardware configuration used.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents

  - SPI/SPI_OneBoard_FullDuplex_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SPI/SPI_OneBoard_FullDuplex_IT/Inc/main.h                  Header for main.c module
  - SPI/SPI_OneBoard_FullDuplex_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_OneBoard_FullDuplex_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - SPI/SPI_OneBoard_FullDuplex_IT/Src/main.c                  Main program
  - SPI/SPI_OneBoard_FullDuplex_IT/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.

  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H7A3ZI-Q Set-up
    - Connect Master board PB3 : CN7.D23 (connected to pin D23 of CN7 connector) to Slave Board PA5 : CN7.D13 (connected to pin D13 of CN7 connector)
    - Connect Master board PB4 : CN7.D25 (connected to pin D25 of CN7 connector) to Slave Board PA6 : CN7.D12 (connected to pin D12 of CN7 connector)
    - Connect Master board PB5 : CN7.D22 (connected to pin D22 of CN7 connector) to Slave Board PA7 : CN7.D11 (connected to pin D11 of CN7 connector)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
