/**
  @page MDMA_LinkedList MDMA linked-list transfer example

  @verbatim
  ******************************************************************************
  * @file    MDMA/MDMA_LinkedList/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the MDMA linked-list transfer example.
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

How to use the MDMA to perform a list of transfers. The transfer list is organized as linked-list, 
each time the current transfer ends the MDMA automatically reload the next transfer parameters,
and starts it (without CPU intervention). 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

The MDMA is configured using the "MDMA_Config" function.
This function configures the MDMA for a linked-list transfer which contains 3 nodes.
Node 0 transfer parameters are configured within the MDMA Handle initialization parameters.
Next nodes are filled using HAL function "HAL_MDMA_LinkedList_CreateNode".
Nodes are connected to the linked-list using the HAL function "HAL_MDMA_LinkedList_AddNode".
After setting all nodes parameters (node 0 using HAL_MDMA_Init() and next nodes using HAL_MDMA_LinkedList_CreateNode()/HAL_MDMA_LinkedList_AddNode()), 
start the transfer in interrupt mode using HAL_MDMA_Start_IT() function. 
Note that the source and destination addresses given to the function HAL_MDMA_Start_IT() 
must correspond to the node 0 source and destination addresses, same for the transfer data length.
Note that using the MDMA, the transfer data length is always expressed in bytes whatever
the source and destination data size (byte, half word, word or double word).

At the end, the main program waits for the end of MDMA transmission or error interrupts: the MDMA transmission ends when all the transfer nodes are served.

STM32H7B3I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when transfer is completed (all transfer nodes are served).
 - LED2 is ON when there is an error in transmission process.  

@note Using the MDMA in linked-list transfer, the following requirement must be filled :
      - Node 0 parameters are always given within the MDMA handle initialization parameter and set through the function "HAL_MDMA_Init"
      - Next nodes must be defines as MDMA_LinkNodeTypeDef types and filled using function "HAL_MDMA_LinkedList_CreateNode".
      - Nodes addresses must be 64 bits aligned and must be in the AXI address space.
        This constraints must be respected when defining the MDMA_LinkNodeTypeDef nodes variables.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

MDMA, Linked-List, SRAM1, AXISRAM, AHBSRAM, SW Request

@par Directory contents

    - MDMA/MDMA_LinkedList/Inc/stm32h7xx_hal_conf.h    HAL configuration file
    - MDMA/MDMA_LinkedList/Inc/stm32h7xx_it.h          Interrupt handlers header file
    - MDMA/MDMA_LinkedList/Inc/main.h                  Main program header file
    - MDMA/MDMA_LinkedList/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I-EVAL board configuration file
    - MDMA/MDMA_LinkedList/Src/stm32h7xx_it.c          Interrupt handlers
    - MDMA/MDMA_LinkedList/Src/main.c                  Main program
    - MDMA/MDMA_LinkedList/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL 
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
