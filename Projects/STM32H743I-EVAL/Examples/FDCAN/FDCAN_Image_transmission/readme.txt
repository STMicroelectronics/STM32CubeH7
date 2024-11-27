/**
  @page FDCAN_Image_transmission FDCAN Image transmission example

  @verbatim
  ******************************************************************************
  * @file    FDCAN/FDCAN_Image_transmission/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FDCAN_Image_transmission ewample.
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

This example shows the gain in time obtained by the activation of the Bit Rate
Switching (BRS) feature.
The same image is transmitted two times via FDCAN as follow:
 - first, without Bit Rate Switching, at 1 MBits/s bit rate
 - then, with Bit Rate Switching activated, at 4 MBits/s data bit rate

Boards: STM32H743I-EVAL (embeding an STM32H743xx device)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |FDCAN1        |                     |        FDCAN1|          |
  |          |       CAN    |_____________________|    CAN       |          |
  |          |    connector |                     | connector    |          |
  |          |______________|                     |______________|          |
  |  _____________________  |                     |                         |
  | |                     | |                     |                         |
  | |                     | |                     |                         |
  | |         LCD         | |                     |                         |
  | |                     | |                     |                         |
  | |_____________________| |                     |                         |
  |   _                _    |                     |   _                     |
  |  |_|              (_)   |                     |  |_|                    |
  | LED3            Tamper  |                     | LED3                    |
  |                         |                     |                         |
  |      STM32 Board 1      |                     |      STM32 Board 2      |
  |        Receiver         |                     |       Transmitter       |
  |_________________________|                     |_________________________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices:
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 100MHz.

Then:
 - Reception filters and Synchronization message are configured.
 - Tx frames are configured to be sent without Bit Rate Switching.
 - FDCAN modules are started.
 - On Board 1, main program waits for Tamper push-button to be pressed to send
   Transmission request to Board 2.
 - On Board 2, main program waits for Reception of Transmission request message
   to begin sending image data by blocks of 32 messages (maximum length of the
   Tx FIFO).

First step (No BRS):
  When Tamper push-button is pressed in Board 1, a Transmission request is sent to
  Board 2 to inform that it is ready to receive data.
  Then, Board 2 sends first 32 blocks (messages) of image data and waits for
  acknowledgment of receipt to send next 32 blocks.
  On the Board 1 side, every time 32 messages are received and payload retrieved,
  the acknowledgment is automatically sent to Board 2.
  This process ends when the full image is transferred. It is then displayed on the
  LDC via DMA2D as well as the whole measured communication time.

Second step (BRS activated):
  Tx frames are reconfigured to be sent with Bit Rate Switching activated.
  The same process as step 1 is repeated.

If an error is encountered LED3 is turned ON.

@note The measured communication time includes:
       - Adding messages to Tx FIFO
       - Transmission (propagation time)
       - Retrieving messages from Rx FIFO
       - Sending acknowledgment of receipt every 32 messages

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

FDCAN, Bit Rate Switching, BRS, Image, Transmit, Receive, Timing, Button

@par Directory contents 

  - FDCAN/FDCAN_Image_transmission/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FDCAN/FDCAN_Image_transmission/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FDCAN/FDCAN_Image_transmission/Inc/main.h                  Header for main.c module  
  - FDCAN/FDCAN_Image_transmission/Inc/Image.h                 Image data
  - FDCAN/FDCAN_Image_transmission/Src/stm32h7xx_it.c          Interrupt handlers
  - FDCAN/FDCAN_Image_transmission/Src/main.c                  Main program
  - FDCAN/FDCAN_Image_transmission/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - FDCAN/FDCAN_Image_transmission/Src/system_stm32h7xx.c      stm32h7xx system source file

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
 - Uncomment the "#define FDCAN_receiver" into main.c (default configuration)
 - Rebuild all files and load your image into Board 1 target memory
 - Comment the "#define FDCAN_receiver" into main.c
 - Rebuild all files and load your image into Board 2 target memory
 - Run the example


 */
