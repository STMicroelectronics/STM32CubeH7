/**
  @page SPI_FullDuplex_ComIT SPI Full Duplex IT LL example

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    SPI_FullDuplex_ComIT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the  SPI Full Duplex IT LL example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example shows how to perform SPI data buffer transmission/reception between
two instances in the same board by using interruptions.

   _______________________________
  |        ____________________   |
  |       |     SPI1           |  |
  |       |                    |  |
  |       |PB3 : CN7.D23   SCK |__|_____________
  |       |                    |  |             |
  |       |PB4 : CN7.D25   MISO|__|_______      |
  |       |                    |  |       |     |
  |       |PB5 : CN7.D22   MOSI|__|__     |     |
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

Then, two functions are called in order to initiate LED_OK (LED1) & LED_ERROR (LED3).

The configuration of GPIO pins belong to SPI1 is ensured by the GPIOPins_SPI1_Config() function.
With the same way, the configuration of GPIO pins belong to SPI6 is ensured by the GPIOPins_SPI6_Config() function.

SPI1_Config() function is called to configure and enable the SPI MASTER (SPI1).
SPI6_Config() function is called to configure and enable the SPI SLAVE (SPI6).

The SPI communication is then initiated.
The LL_SPI_StartMasterTransfer() function allows the transmission and the reception
of a predefined data buffer at the same time (Full Duplex Mode).

For this example the SPIx_TxBuffer is predefined and the SPI1_RxBuffer, SPI6_RxBuffer size is same as SPIx_TxBuffer.

In a first step, SPI Master starts the communication by sending SPIx_TxBuffer and receiving
SPI1_RxBuffer. At the same time, SPI Slave transmits SPIx_TxBuffer and receives SPI1_RxBuffer.

The callback functions (SPI1_Tx_Callback, SPI1_Rx_Callback,SPI6_Tx_Callback, SPI6_Rx_Callback, SPI1_EOT_Callback,
SPI6_EOT_Callback and SPI_TransferError_Callback) update
the variables SPI1_ReceiveIndex, SPI1_TransmitIndex, SPI6_ReceiveIndex, SPI6_TransmitIndex and SPIx_NbDataToTransmit
used in the main function to check the transfer status.
Finally, SPIx_TxBuffer and SPI6_RxBuffer are compared through Buffercmp() in order to check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 turns ON when the transmission and reception process is complete with success.
 - LED3 turns ON when there is an error in transmission/reception process.


@note SPI1&SPI6 instanceS used and associated resources can be updated in "main.c"
      file depending on hardware configuration used.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Directory contents

  - SPI/SPI_FullDuplex_ComIT/Src/main.c            Main program
  - SPI/SPI_FullDuplex_ComIT/Src/stm32h7xx_it.c    Interrupt handlers
  - SPI/SPI_FullDuplex_ComIT/Src/system_stm32h7xx.c  stm32h7xx system source file
  - SPI/SPI_FullDuplex_ComIT/Inc/main.h            Header for main.c module
  - SPI/SPI_FullDuplex_ComIT/Inc/stm32_assert.h    Parameters check file
  - SPI/SPI_FullDuplex_ComIT/Inc/stm32h7xx_it.h    Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.

  - This example has been tested with Nucleo-H7A3ZI-Q boards and can be
    easily tailored to any other supported device and development board.

  - Nucleo-H7A3ZI-Q Set-up
    - Connect Master Instance [PB3 : CN7.D23] to Slave Instance [PA5 : CN7.D13]
    - Connect Master Instance [PB4 : CN7.D25] to Slave Instance [PA6 : CN7.D12]
    - Connect Master Instance [PB5 : CN7.D22] to Slave Instance [PA7 : CN7.D11]

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
