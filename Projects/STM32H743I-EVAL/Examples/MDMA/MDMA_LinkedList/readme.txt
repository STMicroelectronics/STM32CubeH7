/**
  @page MDMA_LinkedList MDMA linked list transfer Example
  
  @verbatim
  ******************************************************************************
  * @file    MDMA/ MDMA_LinkedList /readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the MDMA linked list transfer Example.
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
This example shows how to use the MDMA to perform a list of transfers. The transfer list is organized 
as a linked list , each time the current transfer ends the MDMA automatically relaod the next transfer parameters
and starts it (without CPU intervention). 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

The MDMA is configured using the function "MDMA_Config"
This function configure the MDMA for a linked list transfer, the linked list contains 3 nodes.
Node 0 transfer parameters are configured within the MDMA Handle init parameters.
Next nodes are filled using HAL function "HAL_MDMA_LinkedList_CreateNode"
Nodes are connected to the linked list using the HAL function "HAL_MDMA_LinkedList_AddNode"
After setting all nodes parameters (node 0 using HAL_MDMA_Init and next nodes using HAL_MDMA_LinkedList_CreateNode/HAL_MDMA_LinkedList_AddNode) 
start the transfer in interrupt mode using function "HAL_MDMA_Start_IT". 
Note that the source and destination addresses given to the function HAL_MDMA_Start_IT 
must correspond to the node 0 source and destination addresses, same for the transfer data length.
Note that using the MDMA the transfer data length is always expressed in bytes whatever
is the source and data size (byte, half word, word or double word)

Wait for end of the MDMA transmission or error interrupts ; the MDMA transmission ends when all the transfer nodes are served.

STM32H743I Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when transfer  is complete (all transfer nodes are served).

 - LED3 is ON when there is an error in transmission process.  

@note using the MDMA in linked list transfer the following requirement must be filled :
- Node 0 parameters are always given within the MDMA handle init parameter and set during the function "HAL_MDMA_Init"
- Next nodes must be defines as MDMA_LinkNodeTypeDef types and filled using function "HAL_MDMA_LinkedList_CreateNode".
- Nodes addresses must be 64 bits aligned and must be in the AXI address space.
  this constraints must be respected when defining the MDMA_LinkNodeTypeDef nodes variables.


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

System, MDMA, Linked List, SRAM1, AXISRAM, AHBSRAM, SW Request

@par Directory contents 

  - MDMA/MDMA_LinkedList/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - MDMA/MDMA_LinkedList/Inc/stm32h7xx_it.h          MDMA interrupt handlers header file
  - MDMA/MDMA_LinkedList/Inc/main.h                  Header for main.c module  
  - MDMA/MDMA_LinkedList/Src/stm32h7xx_it.c          MDMA interrupt handlers
  - MDMA/MDMA_LinkedList/Src/main.c                  Main program
  - MDMA/MDMA_LinkedList/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
