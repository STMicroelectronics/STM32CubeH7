/**
  @page SPI_FullDuplex_ComIT SPI Full Duplex IT LL example

  @verbatim
  ******************************************************************************
  * @file    SPI_FullDuplex_ComIT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the  SPI Full Duplex IT LL example.
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

@par Example Description

This example shows how to perform SPI data buffer transmission/reception between
two instances in the same board by using interruptions.

   _______________________________
  |        ____________________   |
  |       |     SPI1           |  |
  |       |                    |  |
  |       |CN7.D13    (pin10)  |__|_____________
  |       |                    |  |             |
  |       |CN7.D12    (pin12)  |__|_______      |
  |       |                    |  |       |     |
  |       |CN7.D11    (pin14)  |__|__     |     |
  |       |                    |  |  |    |     |
  |       |____________________|  |  |    |     |
  | SPI Master                    |  |    |     |
  |                               |  |    |     |
  |                               |  |    |     |
  |                               |  |    |     |
  |                               |  |    |     |
  |        ____________________   |  |    |     |
  |       |     SPI4           |  |  |    |     |
  |       |                    |  |  |    |     |
  |       |CN10.D4     (pin08) |__|__|    |     |
  |       |                    |  |       |     |
  |       |CN10.D3     (pin10) |__|_______|     |
  |       |                    |  |             |
  |       |CN10.D39    (pin26) |__|_____________|
  |       |                    |  |
  |       |____________________|  |
  | SPI Slave                     |
  |                               |
  |_______________________________|

@note The connection between the pins should use a short wires.

At the beginning of the main program the SystemClock_Config() function is called
to configure the system clock for STM32H743xx Devices :
 - The CPU at 400MHz
 - The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals
   and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2
   peripherals and D3 Domain APB4 peripherals to run at 100MHz.

Then, two functions are called in order to initiate LED1 (Green LED) & LED3 (Red LED).

The configuration of GPIO pins belong to SPI1 is ensured by the GPIOPins_SPI1_Config() function.
With the same way, the configuration of GPIO pins belong to SPI4 is ensured by the GPIOPins_SPI4_Config() function.

SPI1_Config() function is called to configure and enable the SPI MASTER (SPI1).
SPI4_Config() function is called to configure and enable the SPI SLAVE (SPI4).

The SPI communication is then initiated.
The LL_SPI_StartMasterTransfer() function allows the transmission and the reception
of a predefined data buffer at the same time (Full Duplex Mode).

For this example the SPIx_TxBuffer is predefined and the SPI1_RxBuffer, SPI4_RxBuffer size is same as SPIx_TxBuffer.

In a first step, SPI Master starts the communication by sending SPIx_TxBuffer and receiving
SPI1_RxBuffer.At the same time, SPI Slave transmits SPIx_TxBuffer and receives SPI1_RxBuffer.

The callback functions (SPI1_Tx_Callback, SPI1_Rx_Callback,SPI4_Tx_Callback, SPI4_Rx_Callback, SPI1_EOT_Callback,
SPI4_EOT_Callback and SPI_TransferError_Callback) update
the variables SPI1_ReceiveIndex, SPI1_TransmitIndex, SPI4_ReceiveIndex, SPI4_TransmitIndex and SPIx_NbDataToTransmit
used in the main function to check the transfer status.
Finally, SPIx_TxBuffer and SPI4_RxBuffer are compared through Buffercmp() in order to check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 turns ON when the transmission and reception process is complete with success.
 - LED3 turns ON when there is an error in transmission/reception process.


@note SPI1&SPI4 instanceS used and associated resources can be updated in "main.c"
      file depending on hardware configuration used.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@Note User can choose where to locate the default RAM region as follows:

        - EWARM IDE:
            Go to Project options-->Linker--> Config--> Linker configuration file:
                - For DTCM RAM select: $PROJ_DIR$\stm32h743xx_flash_dtcmram.icf
                - For D1-AXI SRAM select: $PROJ_DIR$\stm32h743xx_flash_ram_d1.icf

        - MDK-ARM IDE:
            Go to Project options --> Target:
                - For DTCM RAM check IRAM1 and uncheck IRAM2
                - For D1-AXI SRAM check IRAM2 and uncheck IRAM1

        - SW4STM32 IDE:
            Go to Project properties: C/C++ Build-->Settings--> Tools settings --> MCU GCC Linker --> General:
                - For DTCM RAM select: ../STM32H743ZITx_FLASH_DTCMRAM.ld
                - For D1-AXI SRAM select: ../STM32H743ZITx_FLASH_RAM_D1.ld


@par Keywords

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents

  - SPI/SPI_FullDuplex_ComIT/Src/main.c            Main program
  - SPI/SPI_FullDuplex_ComIT/Src/stm32h7xx_it.c    Interrupt handlers
  - Templates/Src/system_stm32h7xx.c               stm32h7xx system source file
  - SPI/SPI_FullDuplex_ComIT/Inc/main.h            Header for main.c module
  - SPI/SPI_FullDuplex_ComIT/Inc/stm32_assert.h    Parameters check file
  - SPI/SPI_FullDuplex_ComIT/Inc/stm32h7xx_it.h    Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.

  - This example has been tested with NUCLEO-H743ZI boards and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H743ZI Set-up
    - Connect Master Instance [PA5 : CN7.D13 (pin10)] to Slave Instance [PE12 : CN10.D39 (pin26)]
    - Connect Master Instance [PA6 : CN7.D12 (pin12)] to Slave Instance [PE13 : CN10.D3  (pin10)]
    - Connect Master Instance [PA7 : CN7.D11 (pin14)] to Slave Instance [PE14 : CN10.D4  (pin08)]
]

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example




 */
