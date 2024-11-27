/**
  @page SPI_FullDuplex_ComIT SPI Full Duplex IT example

  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex IT example.
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
two boards via IT.

   _________________________                        _________________________
  |           ______________|                      |__________________       |
  |          |     SPI1     |                      |        SPI1      |      |
  |          |              |                      |                  |      |
  |          |      CLK(PA5)|______________________|(PA5)CLK  CN7.D13 |      |
  |          |              |                      |                  |      |
  |          |     MISO(PA6)|______________________|(PA6)MISO CN7.D12 |      |
  |          |              |                      |                  |      |
  |          |     MOSI(PB5)|______________________|(PB5)MOSI CN7.D11 |      |
  |          |              |                      |                  |      |
  |          |______________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32H7 Master _________|                      |_STM32H7 Slave __________|

@note The connection between the pins should use a short wires and a common Ground.

HAL architecture allows user to easily change code to move to Polling or DMA 
mode. To see others communication modes please check following examples:
SPI\SPI_FullDuplex_ComPolling
SPI\SPI_FullDuplex_ComDMA

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H745xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals, Cortex-M4, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK, 
GPIO, IT and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_IT() function allows the reception and the 
transmission of a predefined data buffer at the same time (Full Duplex Mode). 
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through 
HAL_SPI_TransmitReceive_IT(), at the same time SPI Slave transmits aTxBuffer 
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

@note SPIx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.

@note You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note It is highly recommended to set the SPI parameter "MasterKeepIOState" to "SPI_MASTER_KEEP_IO_STATE_ENABLE"
      at init time. This configuration allows to avoid glitches on SPI lines especially the clock.
      Glitches that could lead to Master/Slave desynchronization. This config allows SPI Master to keep
      control on output state even if it is disabled.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use IT(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI

@par Directory contents 

  - SPI/SPI_FullDuplex_ComIT/Common/Src/system_stm32h7xx.c  STM32H7xx system configuration source file 
  
  - SPI/SPI_FullDuplex_ComIT/CM7/Inc/stm32h7xx_hal_conf.h   HAL configuration file
  - SPI/SPI_FullDuplex_ComIT/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComIT/CM7/Inc/main.h                 Header for main.c module  
  - SPI/SPI_FullDuplex_ComIT/CM7/Src/stm32h7xx_it.c         Interrupt handlers
  - SPI/SPI_FullDuplex_ComIT/CM7/Src/main.c                 Main program
  - SPI/SPI_FullDuplex_ComIT/CM7/Src/system_stm32h7xx.c     stm32h7xx system source file
  - SPI/SPI_FullDuplex_ComIT/CM7/Src/stm32h7xx_hal_msp.c    HAL MSP file
  
  - SPI/SPI_FullDuplex_ComIT/CM4/Inc/stm32h7xx_hal_conf.h   HAL configuration file
  - SPI/SPI_FullDuplex_ComIT/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComIT/CM4/Inc/main.h                 Header for main.c module  
  - SPI/SPI_FullDuplex_ComIT/CM4/Src/stm32h7xx_it.c         Interrupt handlers
  - SPI/SPI_FullDuplex_ComIT/CM4/Src/main.c                 Main program
  - SPI/SPI_FullDuplex_ComIT/CM4/Src/system_stm32h7xx.c     stm32h7xx system source file
  - SPI/SPI_FullDuplex_ComIT/CM4/Src/stm32h7xx_hal_msp.c    HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32H745xx devices.

  - This example has been tested with NUCLEO-H745ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H745ZI-Q Set-up
    - Connect Master board PA5 to Slave Board PA5
    - Connect Master board PA6 to Slave Board PA6
    - Connect Master board PA7 to Slave Board PA7
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - For this configuration STM32H745I_EVAL_CM4 :
     o  Rebuild all files
     o  Load images into target memory
 - For STM32H745I_EVAL_CM7 :
     o  Rebuild all files
     o  Uncomment "#define MASTER_BOARD" and load the project in Master Board 
	 o  Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
 
