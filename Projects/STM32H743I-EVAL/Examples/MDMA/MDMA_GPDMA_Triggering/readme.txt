/**
  @page MDMA_GPDMA_Triggering MDMA with GPDMA triggering Example
  
  @verbatim
  ******************************************************************************
  * @file    MDMA/MDMA_GPDMA_Triggering/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the MDMA with GPDMA triggering Example.
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

This example describes how to use the MDMA with HW trigger set to D2 Domain GP-DMA
transfer complete flag.

Note that the MDMA has access to the D-TCM RAM (Data Tightly Coupled Memory located @0x20000000) 
which is not the case of the D2 domain GP-DMAs. 
This examples shows how to receive data using a D2 GP-DMA
(DMA1 stream 5 in this case) and transfer these data automatically to the D-TCM RAM
where it can be processed by the CPU . Knowing that the CPU access to the D-TCM
RAM is faster then any other RAM of the device, CPU data processing will be faster.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The UART peripheral configuration is ensured by the UART_Config() function.
This later is calling the HAL_UART_Init() which in return calls HAL_UART_MspInit()
function which core is implementing the configuration of the needed UART resources 
according to the used hardware .You may update the function HAL_UART_MspInit() 
to change UART configuration.
Function UART_Config() send a start message to the user PC HyperTerminal to ask for
sending 12 characters.

The MDMA channel 0 is then configured in HW request mode with HW transfer request trigger
set to DMA1 stream5 transfer complete flag. Meaning that the MDMA will proceed for the transfer
once the DMA1 stream 5 ends its own transfer.
The MDMA channel 0 is configured in block transfer mode with source and destination
data size set to "byte".

The MDMA transfer is then started to transfer content of "aUART_RxBuffer" buffer 
to "aMDMA_RxBuffer" buffer knowing that MDMA HW trigger is DMA1 stream 5 which is 
used for UART reception.
As consequence MDMA transfer will be triggered once the UART received user message to 
"aUART_RxBuffer" buffer through the DMA1 stream 5, the MDMA will then automatically 
transfer it to the "aMDMA_RxBuffer" buffer.
note that "aMDMA_RxBuffer" buffer is located in the D-TCM RAM (@0x20000000).

The function HAL_UART_Receive_DMA() is then called to  allow 
the reception of Data from HyperTerminal to buffer "aUART_RxBuffer". 
Instead of waiting on the end of the UART reception operation, the program waits 
for the MDMA transfer operation ends or error. Indeed when MDMA transfer ends it means 
that the user message has been transferred using UART through the DMA1 stream 5 to 
"aUART_RxBuffer" buffer and the MDMA has transferred the content to the final 
destination "aMDMA_RxBuffer" buffer. 


the received MDMA Buffer "aMDMA_RxBuffer" is then sent back to the user using UART 
in polling model as the DMA1 has no access to the DTCM RAM where the buffer "aMDMA_RxBuffer" 
is located.
    
In the final step an ending message is sent to the user. 

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the MDMA transfer process is complete.
 - LED3 is ON when there is an error in transmission/reception process.

The UART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.


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

System, MDMA, DMA, GPDMA, Stream, DTCM RAM, Transfer, UART, HyperTerminal, Triggering, Message, Polling mode

@par Directory contents 

  - MDMA/MDMA_GPDMA_Triggering/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - MDMA/MDMA_GPDMA_Triggering/Inc/stm32h7xx_it.h          DMA  and MDMA interrupt handlers header file
  - MDMA/MDMA_GPDMA_Triggering/Inc/main.h                  Header for main.c module
  - MDMA/MDMA_GPDMA_Triggering/Src/stm32h7xx_it.c          DMA and MDMA interrupt handlers
  - MDMA/MDMA_GPDMA_Triggering/Src/main.c                  Main program
  - MDMA/MDMA_GPDMA_Triggering/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - MDMA/MDMA_GPDMA_Triggering/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H743I-EVAL Set-up
      Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN2 (USART1) and PC serial port if you want to display data on the HyperTerminal.

@note Make sure that jumper JP8 is on RX position for STM32H743I-EVAL  and jumper JP7 is on TX position. 

  - Hyperterminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - Flow control: None

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
