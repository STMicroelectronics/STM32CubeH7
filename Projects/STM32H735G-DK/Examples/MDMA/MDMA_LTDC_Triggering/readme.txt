/**
  @page MDMA_LTDC_Triggering MDMA with LTDC Line Interrupt Flag used as MDMA trigger Example

  @verbatim
  ******************************************************************************
  * @file    MDMA/MDMA_LTDC_Triggering/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the MDMA with LTDC Line Interrupt Flag used as MDMA trigger Example
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

This example describes how to use the MDMA with HW trigger set to the LTDC Line Interrupt Flag.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

The LTDC is initialized and function "HAL_LTDC_ProgramLineEvent" is called in order to have the LTDC Line Interrupt Flag
each time the LTDC reaches the middle of the LCD screen (LCD height/2)

The MDMA channel 0 is configured as follow :
- Initial transfer is configured using HAL_MDMA_Init function to clear the LCD frame buffer
with default color (black).
- A first transfer node is created and add to the linked list to copy an image (from image 0 to image22 )
to the LCD frame buffer with MDMA transfer trigger set to LTDC Line Interrupt Flag.
- A second transfer node is add with MDMA HW transfer trigger set to DMA1 stream1 transfer complete flag.
  This transfer node is dummy one used to wait for node1 source address update which is done by a
  DMA1 Stream 0.

  The MDMA created linked list is made circular using function "HAL_MDMA_LinkedList_EnableCircularMode"

The DMA1 stream 0 is configured to update the MDMA node1 source address with a new image each time.
This DMA is triggered with the DMA Request generator 0 which is configured with trigger signal set to
LPTIM1. LPTIM1 is configured with reload period set to 100ms. The DMA1 stream 0 transfer is configured
in circular mode. the DMA1 stream 0 synchronization block is configured with Synchro disabled and
with event generation enabled in order to generate and event after each transfer (each update of the MDMA Node1 source address).
As consequence, each 100ms the DMA1 stream 0 will issue a transfer to update the MDMA Node1 source address
with the new image address in the flash.
An event is generated after each transfer (i.e after each update of the MDMA node 1 source address)

The DMA1 Stream 1 is configured in circular transfer with transfer trigger set to DMA request generator 1
which is configured with trigger signal set to DMA1 Stream 0 event.
As consequence, each time DMA1 stream 0 transfer (MDMA node1 source address update) the DMA1 stream1 will
issue a dummy transfer. The Transfer complete of this dummy transfer will trigger the MDMA Node2 transfer.
as the MDMA transfer is a circular linked list, the MDMA will goes back to Node 1 to transfer a new image.



To summarize,  after configuring the MDMA, DMA1 stream0 and DMA1 stream 1 :
- First the Initial MDMA transfer will clear the LCD frame buffer.
- Then the MDMA Node 1 transfer will issue a transfer to copy an image from the flash
  to the LCD frame buffer with MDMA HW trigger set to the LTDC Line Interrupt Flag.
- Then the MDMA Node2 will issue a dummy transfer with MDMA HW trigger set to DMA1 stream 1
  transfer complete flag . meaning that the MDMA will wait for DMA1 stream 1 Transfer complete
  before looping on Node 1 to transfer a new image.
  The DMA1 stream 1 transfer is triggered once the DMA stream0 has issue a transfer to update the
  MDMA Node 1 source address with the next image address. the DMA1 stream 0 is triggered with LPTIM1 i.e
  each 100ms.

As results the LCD will show a new image each 100ms giving an automated animation (moving butterfly)
without CPU load . The CPU can goe to sleep mode.


@note the MDMA linked list Nodes addresses must be 64 bits aligned and must be in the AXI address space.
      this constraints must be respected when defining the MDMA_LinkNodeTypeDef nodes variables.
      The MDMA Node descriptors "Xfer_Node", “Xfer_DummyNode” and the Nodes_SourceAddress table
      are located in the D1 AXI-SRAM which is cacheable.

STM32H735G-DK's LEDs can be used to monitor the transfer status:
 - LED2 is ON when there is an error in MDMA transfer process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Display, Graphics, MDMA, DMA, LCD, SDRAM, RGB565, Memory to memory, LTDC, Pixel, non contiguous, Transfer
linked list, 

@par Directory contents

  - MDMA/MDMA_LTDC_Triggering/Inc/stm32h7xx_hal_conf.h        HAL configuration file
  - MDMA/MDMA_LTDC_Triggering/Inc/stm32h7xx_it.h              DMA  and MDMA interrupt handlers header file
  - MDMA/MDMA_LTDC_Triggering/Inc/main.h                      Header for main.c module
  - MDMA/MDMA_LTDC_Triggering/Inc/Image01.h to Image22.h      Image 100x100 in ARGB8888 to display on LCD
  - MDMA/MDMA_LTDC_Triggering/Inc/stm32h735g_discovery_conf.h      STM32H735G-DK board configuration file
  - MDMA/MDMA_LTDC_Triggering/Src/stm32h7xx_it.c              MDMA interrupt handlers
  - MDMA/MDMA_LTDC_Triggering/Src/main.c                      Main program
  - MDMA/MDMA_LTDC_Triggering/Src/stm32h7xx_hal_msp.c         HAL MSP module
  - MDMA/MDMA_LTDC_Triggering/Src/system_stm32h7xx.c          STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H735xx device.

  - This example has been tested with STM32H735G-DK and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */

