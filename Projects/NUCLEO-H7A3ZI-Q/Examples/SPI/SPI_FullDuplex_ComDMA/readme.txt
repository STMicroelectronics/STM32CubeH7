/**
  @page SPI_FullDuplex_ComDMA SPI Full Duplex DMA example

  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComDMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex DMA example.
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

Data buffer transmission/reception between two boards via SPI using DMA.

Board: NUCLEO-H7A3ZI-Q (embeds a STM32H7A3ZIT6Q device)
CLK Pin: PA5 (CN07.D10)
MISO Pin: PA6 (CN7.D12)
MOSI Pin: PA7 (CN07.D14)

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

HAL architecture allows user to easily change code to move to Polling or IT 
mode. To see others communication modes please check following examples:
SPI\SPI_FullDuplex_ComPolling
SPI\SPI_FullDuplex_ComIT

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280 MHz.
The HCLK is scheduled at 280 MHz and used for CD Domain AXI and AHB1/AHB2/AHB3 peripherals and for SRD Domain AHB4 peripherals.
The APB clock dividers are scheduled at 280 MHz/2 and used for CD Domain APB1/APB2/APB3 peripherals and SRD Domain APB4 peripherals.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_DMA() function allows the reception and the 
transmission of a predefined data buffer at the same time (Full Duplex Mode). 
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through 
HAL_SPI_TransmitReceive_DMA(), at the same time SPI Slave transmits aTxBuffer 
and receives aRxBuffer through HAL_SPI_TransmitReceive_DMA(). 
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update 
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 toggles quickly on master board waiting User push-button to be pressed.
 - LED1 turns ON when the transmission process is complete.
 - LED2 turns ON when the reception process is complete.
 - LED3 turns ON when there is an error in transmission/reception process.  

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
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Connectivity, SPI, Full-duplex, DMA, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents 

  - SPI/SPI_FullDuplex_ComDMA/Inc/stm32h7xx_hal_conf.h   HAL configuration file
  - SPI/SPI_FullDuplex_ComDMA/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComDMA/Inc/main.h                 Header for main.c module
  - SPI/SPI_FullDuplex_ComDMA/Inc/stm32h7xx_nucleo_conf.h  STM32H7xx_Nucleo board configuration file
  - SPI/SPI_FullDuplex_ComDMA/Src/stm32h7xx_it.c         Interrupt handlers
  - SPI/SPI_FullDuplex_ComDMA/Src/main.c                 Main program
  - SPI/SPI_FullDuplex_ComDMA/Src/system_stm32h7xx.c     stm32h7xx system source file
  - SPI/SPI_FullDuplex_ComDMA/Src/stm32h7xx_hal_msp.c    HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.

  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H7A3ZI-Q Set-up
    - Connect Master board SCK pin (PA5) to Slave Board SCK pin (PA5) (connector CN07.D10)
    - Connect Master board MISO pin (PA6) to Slave Board MISO pin (PA6) (connector CN7.D12)
    - Connect Master board MOSI pin (PA7) to Slave Board MOSI pin (PA7) (connector CN07.D14)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
 
