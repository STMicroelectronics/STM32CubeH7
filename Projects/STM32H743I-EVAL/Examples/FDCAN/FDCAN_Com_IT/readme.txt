/**
  @page FDCAN_Com_IT FDCAN Communication IT example

  @verbatim
  ******************************************************************************
  * @file    FDCAN/FDCAN_Com_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FDCAN_Com_IT ewample.
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

This example shows how to achieve Interrupt Process Communication between two
FDCAN units.

Boards: STM32H743I-EVAL (embeding an STM32H743xx device)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |FDCAN1        |                     |        FDCAN1|          |
  |          |              |                     |              |          |
  |          |       CAN    |_____________________|    CAN       |          |
  |          |    connector |                     | connector    |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |   _    _           _    |                     |   _    _           _    |
  |  |_|  |_|         (_)   |                     |  |_|  |_|         (_)   |
  | LED3  LED1      Tamper  |                     | LED3  LED1      Tamper  |
  |                         |                     |                         |
  |                         |                     |                         |
  |      STM32 Board 1      |                     |      STM32 Board 2      |
  |                         |                     |                         |
  |_________________________|                     |_________________________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices:
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 100MHz.

Then, on both boards :
 - FDCAN module is configured to receive messages with pre-defined ID to its Rx FIFO 0.
 - Rx FIFO 0 Watermark is configured to 2 and the corresponding notification is activated.
 - FDCAN module is started.

When Tamper push-button is pressed in any Board:
 - its LED1 is turned OFF
 - its FDCAN module sends 2 messages

On the other board, after receiving the 2 messages:
 - received payload is compared to expected data
 - if the result is OK, LED1 is tuned ON

If at any time of the process an error is encountered LED3 is turned ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
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

FDCAN, Interrupt, IT, FD, BRS, TxDelay

@par Directory contents 

  - FDCAN/FDCAN_Com_IT/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FDCAN/FDCAN_Com_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FDCAN/FDCAN_Com_IT/Inc/main.h                  Header for main.c module  
  - FDCAN/FDCAN_Com_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - FDCAN/FDCAN_Com_IT/Src/main.c                  Main program
  - FDCAN/FDCAN_Com_IT/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - FDCAN/FDCAN_Com_IT/Src/system_stm32h7xx.c      stm32h7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with a couple of STM32H743I-EVAL boards embedding a 
    STM32H743xx device and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL set-up
    - Connect a CAN cable to the CAN connectors of the two EVAL boards (CN3)
    - Jumpers JP1 and JP2 => fitted
    - Solder bridges SB59 and SB60 => open
    - Solder bridge SB50 => closed

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memories
 - Run the example


 */
