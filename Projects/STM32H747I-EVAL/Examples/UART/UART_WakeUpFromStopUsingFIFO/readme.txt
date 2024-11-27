/**
  @page UART_WakeUpFromStopUsingFIFO  wake up from STOP mode using UART FIFO level example

  @verbatim
  ******************************************************************************
  * @file    UART/UART_WakeUpFromStopUsingFIFO/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the wake up from STOP mode using UART FIFO level example.
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

This example shows how to use UART HAL API to wake up the MCU from STOP mode
using the UART FIFO level.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H747xx Devices :
The CPU at 400MHz
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz

In the first part of the example the UART is configured for reception with Rx FIFO thhreshold
set to one 1 QUARTER and enter the CPU to Stop Mode.
The UART FIFO threshold interruption is enabled and ,by default, the UART FIFO full is disabled.
The user has to send 4 bytes from the HyperTerminal. at the 4th received byte
the UART FIFO will reach the quarter level which will generate an interruption and wake-up the CPU.

In the second part of the example, the UART is configured for reception with
FIFO threshold interruption disabled and FIFO full enabled. then the CPU goes to stop mode.
The user has to send 16 bytes (size of the UART FIFO) from the HyperTerminal.
at the 16th received byte, the UART FIFO will be full which will generate an interruption and wake-up the CPU.


Board: STM32H747I-EVAL
Tx Pin: PB.14
Rx Pin: PB.15
   _________________________
  |           ______________|                       _______________
  |          |USART         |                      | HyperTerminal |
  |          |              |                      |               |
  |          |           TX |______________________|RX             |
  |          |              |                      |               |
  |          |              |     RS232 Cable      |               |
  |          |              |                      |               |
  |          |           RX |______________________|TX             |
  |          |              |                      |               |
  |          |______________|                      |_______________|
  |                         |
  |                         |
  |                         |
  |                         |
  |_STM32_Board_____________|

LED1 is ON when MCU is not in STOP mode.
LED3 is ON when there is an error occurrence.

The UART is configured as follows:
    - BaudRate = 9600 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.

@note When the parity is enabled, the computed parity is inserted at the MSB
      position of the transmitted data.

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

Connectivity, UART, Baud rate, RS-232, full-duplex, HyperTerminal, Transmission, Reception, Asynchronous, Low power, Stop mode, Wakeup, Interrupt, FIFO, Threshold

@par Directory contents

  - UART/UART_WakeUpFromStopUsingFIFO/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - UART/UART_WakeUpFromStopUsingFIFO/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - UART/UART_WakeUpFromStopUsingFIFO/Inc/main.h                  Header for main.c module
  - UART/UART_WakeUpFromStopUsingFIFO/Src/stm32h7xx_it.c          Interrupt handlers
  - UART/UART_WakeUpFromStopUsingFIFO/Src/main.c                  Main program
  - UART/UART_WakeUpFromStopUsingFIFO/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - UART/UART_WakeUpFromStopUsingFIFO/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H747xx devices.

  - This example has been tested with STM32H747I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H747I-EVAL Set-up
      Connect a null-modem female/female RS232 cable between the DB9 connector
      CN2 (USART1) and PC serial port to display data on the HyperTerminal.

@note Make sure that jumper JP7 is on TX position and JP8 is on RX position

  - Hyperterminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - Flow control: None

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each configuration (STM32H747I_EVAL_CM7 and STM32H747I_EVAL_CM4) :
   - Rebuild all files
   - Load images into target memory
 - Run the example


 */
