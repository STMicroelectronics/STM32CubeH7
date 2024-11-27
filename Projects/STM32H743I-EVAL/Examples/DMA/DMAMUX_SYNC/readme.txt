/**
  @page DMAMUX_SYNC DMA & DMAMUX Synchronization Example
  
  @verbatim
  ******************************************************************************
  * @file    DMA/DMAMUX_SYNC/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA & DMAMUX Synchronization Example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 
This example shows how to use the DMA with the DMAMUX to synchronize a transfer with LPTIM1 output signal.
The USART1 is used in DMA synchronized mode to send a countdown from 10 to 00 with a period of 2sec. 

The example uses the USART1 in DMA synchronized mode to send a countdown from 10 to 00 with 2sec period.
The DMAMUX synchronization block is configured to synchronize the DMA transfer with the LPTIM1 output signal.
Each rising edge of the synchronization signal (i.e LPTIM1 output signal) will authorize 4 USART1 requests to be
transmitted to the the USART1 peripheral using the DMA. these four requests represent the 2 characters '\n\r' plus 
the 2 characters count down itself from '10' to '00'.
The LPTIM1 is configured to generate a PWM with 2 seconds period.


The example requires to connect the board to an HyperTerminal PC application through USART1 connector (CN2).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The DMA is configured in memory to peripheral mode to ensure data transfer 
from the source transmission buffer (TxSyncMessage) to the USART1 (in order to be transmitted to the UART/HyperTerminal).

The DMAMUX synchronization block is configured using function "HAL_DMAEx_ConfigMuxSync" with the following parameters :
- SyncSignalID  : set to HAL_DMAMUX1_SYNC_LPTIM1_OUT which corresponds to LPTIM1_OUT signal.
- SyncPolarity  : Set to RISING to use rising edge of LPTIM1 output signal for synchronization.
- RequestNumber : 4 i.e four USART1 requests are authorized after each rising edge of the sync signal.
- EventState : enabled , in order to generate an event each time "RequestNumber" are transmitted.
  Note that this event could be used with the DMAMUX external request generator to trigger another DMA stream transfer.

The LPTIM1 is configured using function "LPTIM_Config" in order to configure and start a PWM on the LPTIM1 timer with
2 seconds period and 50% duty cycle.

The USART1 peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit()function which core is implementing
the configuration of the needed UART resources according to the used hardware (CLOCK, 
GPIO and NVIC). 
DMA & DMAMUX configurations are performed in the main.c module as the example first purpose is to show 
the DMA & DMAMUX configuration when using the DMAMUX synchronization block. 
In normal conditions DMA and DMAMUX configuration should be placed in the HAL_UART_MspInit()function.

A first example brief description message is then transmitted using the USART1 to the PC HyperTerminal
in Polling mode (using message buffer ).

Then a second UART transfer is started in DMA synchronized mode to transfer the countdown buffer "TxEndMessage".
As result the countdown from 10 to 00 with a period of 2 seconds should be displayed on the HyperTerminal. 
The end of this step is monitored through the HAL_UART_GetState() function result.

In the last step an ending message is sent using the USART1 in polling mode through the "TxEndMessage" buffer.  

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED3 is ON when there is an error in transmission process.  

The UART is configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - parity none
    - Hardware flow control disabled (RTS and CTS signals)

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note This example can be easily modified to use any other peripheral (than the USART1) with a DMA synchronized transfer.
In this case user will need to replace the USART1 initialization and processing by the used IP initialization and processing functions.
User can also change the synchronization signal (based on the sync signals list available in the reference manual),
in this case the sync signal IP should be configured (replacing the function LPTIM_Config by the required sync signal IP configuration)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

System, DMA, DMAMUX, LPTIM, USART, Data Transfer, Memory to periph, synchronization

@par Directory contents 

  - DMA/DMAMUX_SYNC/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - DMA/DMAMUX_SYNC/Inc/stm32h7xx_it.h          DMA interrupt handlers header file
  - DMA/DMAMUX_SYNC/Inc/main.h                  Header for main.c module  
  - DMA/DMAMUX_SYNC/Src/stm32h7xx_it.c          DMA interrupt handlers
  - DMA/DMAMUX_SYNC/Src/main.c                  Main program
  - DMA/DMAMUX_SYNC/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - DMA/DMAMUX_SYNC/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H743I-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN2 (USART1) and PC serial port in order to display data on the HyperTerminal.


  - HyperTerminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - None parity
    - Baud Rate = 115200 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
