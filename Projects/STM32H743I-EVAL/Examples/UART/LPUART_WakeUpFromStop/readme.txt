/**
  @page LPUART_WakeUpFromStop LPUART Wake up from Stop mode example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    UART/LPUART_WakeUpFromStop/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPUART-triggered MCU wake-up from STOP mode.
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

This example shows how to configure a LPUART to wake up the MCU from STOP mode
when a given stimulus is received.

Board: STM32H743I-EVAL (embeds a STM32H743xx device)
Tx Pin: PB.6 pin 18 CN7
Rx Pin: PB.7 pin 16 CN7
   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |LPUART        |                     |        LPUART|          |
  |          |              |                     |              |          |
  |          |           TX |_____________________| RX           |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |           RX |_____________________| TX           |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                         |                     |                         |
  | STM32 Board 1           |                     | STM32 Board 2           |
  | (follows a sequence of  |                     | (sends stimuli to wake  |
  | RUN/STOP modes)         |                     |  up STM32 Board 1)      |
  |_________________________|                     |_________________________|


Board 1 enters STOP mode and is awoken by Board 2 which sends
the proper data to wake up the board 1 MCU.
When the proper wake-up event is recognized, the WUF interrupt is triggered which wakes
up board 1.
To confirm its wake up, board 1 sends a confirmation message to board 2 which
checks it is the expected message.
This cycle is repeated 4 times to verify 4 different events
1�) wake-up by RXNE flag rising
2�) wake-up by Start Bit detection
3�) wake-up by 7-bit long address match
4�) wake-up by 4-bit long address match

WARNING: as both boards do not behave the same way, "BOARD_IN_STOP_MODE" 
compilation switch is defined in UART/LPUART_WakeUpFromStop/Src/main.c and must be
enabled at compilation time before loading the executable in board 1 (that which
is set in STOP mode).
The stimuli-transmitting board (board 2) needs to be loaded with an executable
software obtained with BOARD_IN_STOP_MODE disabled. 

At the very beginning, board 1 LED1 is ON then is turned off.
This means that board 1 has entered STOP mode.
Parallely, board 2 LED1 is quickly blinking (100 ms period). The user has 
just to wait for board 1 LED1 to be turned off then to press board 2 TAMPER push-button 
to send the first wake-up stimulus to board 1. 
This starts the test sequence and no more action is required from the user
(a single TAMPER push-button press is needed to start the full 4 wake-up events
test sequence).

Board 1 LED1 behavior as described below allows to follow the 
test progress:

ON ____          _______          _______          _______          ____
       |        |       |        |       |        |       |        |    
OFF    |________|       |________|       |________|       |________|    
      Enter    Exit    Enter    Exit    Enter    Exit    Enter    Exit  
      Stop     Stop    Stop     Stop    Stop     Stop    Stop     Stop  
      Mode     Mode    Mode     Mode    Mode     Mode    Mode     Mode  
            (RXNE flag)      (Start Bit      (7-bit add.       (4-bit add.
                              Detection)        match)            match)
                               


Simultaneously, board 2 LED1 behavior shown below allows to follow the 
test progress on the other side:

  BLINKING   
ON ****________          _______          _______          _______          ____
               |        |       |        |       |        |       |        |    
OFF            |________|       |________|       |________|       |________|    
     TAMPER   WkUp   2nd stim  WkUp   3rd stim  WkUp   4th stim  WkUp    LED ON  
     button   conf     sent    conf     sent    conf     sent    conf    to mark  
    pushed,   rcv'ed          rcv'ed           rcv'ed           rcv'ed   passed
  1st stimulus                                                            test 
     sent                                                                    
  (RXNE flag)      (Start Bit       (7-bit add.      (4-bit add.
                    Detection)         match)           match)
     
    
- If the test fails or if there is an initialization or transfer error, LED1
transmits a sequence of three dots, three dashes, three dots.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 100MHz.

Additionally the HSI is enabled (in stm32h7xx_hal_msp.c) to demonstrate LPUART wake-up capabilities.
Each time board 1 is awoken, the system clock (SYSCLK) at 400 MHz is restored.


The LPUART is configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - Parity none
    - Hardware flow control disabled (RTS and CTS signals)


@note LPUART instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note This example can not be used in DEBUG mode due to the fact that the 
    Cortex-M7 core is no longer clocked during low power mode and so debugging 
    features are not available.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, LPUART, UART, Baud rate, RS-232, full-duplex, Transmission, Reception, Asynchronous, Low power, Stop mode, Wakeup, Interrupt, Stimulus, Button

@par Directory contents 

  - UART/LPUART_WakeUpFromStop/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - UART/LPUART_WakeUpFromStop/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - UART/LPUART_WakeUpFromStop/Inc/main.h                  Header for main.c module  
  - UART/LPUART_WakeUpFromStop/Src/stm32h7xx_it.c          Interrupt handlers
  - UART/LPUART_WakeUpFromStop/Src/main.c                  Main program
  - UART/LPUART_WakeUpFromStop/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - UART/LPUART_WakeUpFromStop/Src/system_stm32h7xx.c      stm32h7xx system source file


@par Hardware and Software environment 

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with a couple of STM32H743I-EVAL boards embedding a 
    STM32H743xx device and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL set-up
    - Connect a wire between 1st board PB.6 (pin 18 on CN7 (LPUART Tx)) and 2nd board (PB.7 pin 16 on CN7 (LPUART Rx))
    - Connect a wire between 1st board PB.7 (pin 16 on CN7 (LPUART Rx)) and 2nd board (PB.6 pin 18 on CN7 (LPUART Tx))


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
